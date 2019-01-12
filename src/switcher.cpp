#include "autonomous.h"

void  doNothing() {}
vfptr auton = &autonBlueFlags;

struct SwitcherMenu {
  const char *              name;
  vector<SwitcherMenu> submenus;
  vfptr                     fnc;
  SwitcherMenu(const char *name, std::initializer_list<SwitcherMenu> menus, vfptr fnc = nullptr) :
      name(name), fnc(fnc) {
    for (auto &&m : menus) {
      submenus.push_back(m);
    }
  }
};

string selected;
bool   pressed;

lv_res_t buttonPress(lv_obj_t *btn, const char *text) {
  selected = string(text);
  cout << selected << std::endl;
  pressed = true;
  return LV_RES_OK;
}

vfptr selectMenu(SwitcherMenu *menu) {
  // reset the selected text and pressed notifier
  selected = "";
  pressed  = false;

  // set the button matrix
  const char *btnMap[6] = {" ", " ", "\n", " ", " ", ""};
  for (std::size_t i = 0; i < menu->submenus.size(); i++) {
    if (i < 2) {
      btnMap[i] = menu->submenus[i].name;
    } else if (i < 5) {
      btnMap[i + 1] = menu->submenus[i].name;
    } else
      break;
  }

  // init lvgl button matrix
  lv_obj_t *btnMtx = lv_btnm_create(lv_scr_act(), NULL);
  lv_btnm_set_map(btnMtx, btnMap);
  lv_btnm_set_action(btnMtx, &buttonPress);
  lv_obj_set_size(btnMtx, LV_HOR_RES, LV_VER_RES);

  // button style - I copied/pasted all this style stuff from a place where I found a style I liked
  static lv_style_t style_bg;
  static lv_style_t style_btn_rel;
  static lv_style_t style_btn_pr;
  lv_style_copy(&style_bg, &lv_style_plain);
  style_bg.body.main_color    = LV_COLOR_SILVER;
  style_bg.body.grad_color    = LV_COLOR_SILVER;
  style_bg.body.padding.hor   = 0;
  style_bg.body.padding.ver   = 0;
  style_bg.body.padding.inner = 0;
  lv_style_copy(&style_btn_rel, &lv_style_btn_rel);
  style_btn_rel.body.main_color   = LV_COLOR_MAKE(0x30, 0x30, 0x30);
  style_btn_rel.body.grad_color   = LV_COLOR_BLACK;
  style_btn_rel.body.border.color = LV_COLOR_SILVER;
  style_btn_rel.body.border.width = 1;
  style_btn_rel.body.border.opa   = LV_OPA_50;
  style_btn_rel.body.radius       = 0;
  lv_style_copy(&style_btn_pr, &style_btn_rel);
  style_btn_pr.body.main_color = LV_COLOR_MAKE(0x55, 0x96, 0xd8);
  style_btn_pr.body.grad_color = LV_COLOR_MAKE(0x37, 0x62, 0x90);
  style_btn_pr.text.color      = LV_COLOR_MAKE(0xbb, 0xd5, 0xf1);
  lv_btnm_set_style(btnMtx, LV_BTNM_STYLE_BG, &style_bg);
  lv_btnm_set_style(btnMtx, LV_BTNM_STYLE_BTN_REL, &style_btn_rel);
  lv_btnm_set_style(btnMtx, LV_BTNM_STYLE_BTN_PR, &style_btn_pr);

  // wait for something to be selected
  while (!pressed) {
    if (controller::master.getDigital(ControllerDigital::R1)) {
      selected = "none";
      break;
    }
    delay(20);
  }

  // delete the button
  lv_obj_del(btnMtx);

  SwitcherMenu *newMenu;
  for (auto &&m : menu->submenus) {
    if (string(m.name) == selected) {
      if (m.fnc == nullptr) {
        if (m.submenus.size()) return selectMenu(&m);
        return selectMenu(menu);
      } else {
        return m.fnc;
      }
    }
  }
}

void chooseAuton() {
  // clang-format off
  SwitcherMenu rootMenu("select autonomous", {
        SwitcherMenu("red", {
          SwitcherMenu("flags", {}, &autonRedFlags),
          SwitcherMenu("caps", {}, &autonDriveStraight),
        }),
        SwitcherMenu("blue", {
          SwitcherMenu("flags", {}, &autonBlueFlags),
          SwitcherMenu("caps", {}, &autonDriveStraight),
        }),
        SwitcherMenu("other", {
          SwitcherMenu("data", {}, &printData),
          SwitcherMenu("skills", {}, &autonSkills),
        }),
        SwitcherMenu("none", {}, &doNothing),
  });
  // clang-format on */

  auton = selectMenu(&rootMenu);
}
