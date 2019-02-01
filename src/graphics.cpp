#include "autonomous.h"

void doNothing() {}

struct SwitcherMenu {
  const char *         name;
  vector<SwitcherMenu> submenus;
  vfptr                fnc;
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
  if (selected == " ") {
    selected = "";
    return LV_RES_OK;
  }
  cout << selected << std::endl;
  pressed = true;
  return LV_RES_OK;
}

std::pair<vfptr, string> selectMenu(SwitcherMenu *menu) {
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
  lv_obj_align(btnMtx, NULL, LV_ALIGN_CENTER, 0, 0);

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
    if (controller::master.getDigital(ControllerDigital::R1) ||
        controller::partner.getDigital(ControllerDigital::R1) ||
        (pros::competition::is_autonomous() && !pros::competition::is_disabled())) {
      // delete the buttons
      lv_obj_del(btnMtx);

      return std::pair(auton, autonName);
    }

    delay(5);
  }

  // delete the buttons
  lv_obj_del(btnMtx);

  SwitcherMenu *newMenu;
  for (auto &&m : menu->submenus) {
    if (string(m.name) == selected) {
      if (m.fnc == nullptr) {
        if (m.submenus.size())
          return selectMenu(&m);
        return selectMenu(menu);
      } else {
        return std::pair(m.fnc, string(menu->name).append(string("->")).append(string(m.name)));
      }
    }
  }
}

lv_obj_t *cont;

class Line {
private:
  string         atext;
  AbstractMotor *motor;
  unsigned int   number;

public:
  typedef enum { Position, Temperature, Velocity, Power } ValueType;
  lv_obj_t *line;

  void create() { line = lv_label_create(cont, NULL); }

  void destroy() { lv_obj_del(line); }

  Line(string name, AbstractMotor *motor, ValueType valueType, unsigned int number) :
      motor(motor), valueType(valueType), number(number) {
    switch (valueType) {
    case Position:
      atext = name + " position: ";
      break;
    case Temperature:
      atext = name + " temperature (°): ";
      break;
    case Velocity:
      atext = name + " velocity (rpm): ";
      break;
    case Power:
      atext = name + " power: ";
      break;
    default:
      break;
    }

    create();
    delay(1);
  }

  ~Line() { destroy(); }

  string get() {
    switch (valueType) {
    case Position:
      return atext + std::to_string(motor->getPosition());
    case Temperature:
      return atext + std::to_string(motor->getTemperature());
    case Velocity:
      return atext + std::to_string(motor->getActualVelocity());
    case Power:
      return atext + std::to_string(motor->getPower() * (127 / 11));
    default:
      return string("error");
    }
  }

  void draw() { lv_label_set_text(line, get().c_str()); }
  void clear() { lv_label_set_text(line, ""); }
  void align() { lv_obj_align(line, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 20 * number); }

private:
  ValueType valueType;
};

pros::Mutex     mutex;
bool            bpressed = false;
static lv_res_t selectionPress(lv_obj_t *btn, const char *txt) {
  bpressed = true;

  return LV_RES_OK;
}

void infoLoop(void *none) {
  unsigned int nlines = 0;
  char         tline[20];
  string       autonT = "autonomous: ";
  cont                = lv_cont_create(lv_scr_act(), NULL);
  lv_cont_set_fit(cont, false, false);
  lv_obj_set_size(cont, LV_HOR_RES, LV_VER_RES);

  // clang-format off
  Line lines[] = {
          Line("flywheel",    &launcher,     Line::Velocity,    nlines++),
          Line("flywheel",    &launcher,     Line::Temperature, nlines++),
          Line("flywheel",    &launcher,     Line::Power,       nlines++),
          Line("intake",      &intake,       Line::Velocity,    nlines++),
          Line("lift",        &lift,         Line::Position,    nlines++),
          Line("left drive",  &drive::left,  Line::Position,    nlines++),
          Line("right drive", &drive::right, Line::Position,    nlines++),
          Line("left drive",  &drive::left,  Line::Velocity,    nlines++),
          Line("right drive", &drive::right, Line::Velocity,    nlines++),
  };

  static SwitcherMenu rootMenu("", {
          SwitcherMenu("red", {
            SwitcherMenu("flags", {}, &autonRedFlags),
            SwitcherMenu("caps", {}, &autonRedCaps),
          }),
            SwitcherMenu("blue", {
              SwitcherMenu("flags", {}, &autonBlueFlags),
              SwitcherMenu("caps", {}, &autonBlueCaps),
          }),
            SwitcherMenu("other", {
              SwitcherMenu("data", {}, &printData),
              SwitcherMenu("skills", {}, &autonSkills),
          }),
            SwitcherMenu("none", {}, &doNothing),
  });
  // clang-format on

  for (auto &&line : lines)
    line.align();

  auto autonLine = lv_label_create(cont, NULL);
  lv_obj_align(autonLine, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 20 * nlines);

  // init lvgl button matrix
  lv_obj_t *  btn    = lv_btnm_create(cont, NULL);
  const char *text[] = {"auton?", ""};
  lv_btnm_set_map(btn, text);

  lv_btnm_set_action(btn, &selectionPress);

  lv_obj_set_size(btn, LV_HOR_RES / 5, LV_VER_RES / 4);
  lv_obj_align(btn, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 0);

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
  lv_btnm_set_style(btn, LV_BTNM_STYLE_BG, &style_bg);
  lv_btnm_set_style(btn, LV_BTNM_STYLE_BTN_REL, &style_btn_rel);
  lv_btnm_set_style(btn, LV_BTNM_STYLE_BTN_PR, &style_btn_pr);

  // Create a bar and an indicator style
  static lv_style_t style_bar;
  static lv_style_t style_indic;

  lv_style_copy(&style_bar, &lv_style_pretty);
  style_bar.body.main_color   = LV_COLOR_BLACK;
  style_bar.body.grad_color   = LV_COLOR_GRAY;
  style_bar.body.radius       = LV_RADIUS_CIRCLE;
  style_bar.body.border.color = LV_COLOR_WHITE;

  lv_style_copy(&style_indic, &lv_style_pretty);
  style_indic.body.grad_color   = LV_COLOR_GREEN;
  style_indic.body.main_color   = LV_COLOR_LIME;
  style_indic.body.radius       = LV_RADIUS_CIRCLE;
  style_indic.body.shadow.width = 10;
  style_indic.body.shadow.color = LV_COLOR_LIME;
  style_indic.body.padding.hor  = 3; // Make the indicator a little bit smaller
  style_indic.body.padding.ver  = 3;

  // Create a label for the bar
  lv_obj_t *bar_label = lv_label_create(cont, NULL);
  lv_label_set_text(bar_label, "Battery");
  lv_obj_align(bar_label, cont, LV_ALIGN_IN_TOP_RIGHT, 0, LV_VER_RES / 4 + 5);

  // Create a bar
  lv_obj_t *bar = lv_bar_create(cont, NULL);
  lv_bar_set_style(bar, LV_BAR_STYLE_BG, &style_bar);
  lv_bar_set_style(bar, LV_BAR_STYLE_INDIC, &style_indic);
  lv_obj_set_size(bar, LV_HOR_RES / 5, 20);
  lv_obj_align(bar, bar_label, LV_ALIGN_OUT_BOTTOM_RIGHT, 0, 5);

  mutex.give();
  bool blpressed = false;
  Rate rate;
  while (true) {
    if (bpressed) {
      lv_obj_set_hidden(cont, true);
      for (auto &&line : lines)
        line.clear();
      lv_label_set_text(autonLine, "");

      auto s    = selectMenu(&rootMenu);
      auton     = s.first;
      autonName = s.second;

      bpressed = false;
      lv_obj_set_hidden(cont, false);
    }
    for (auto &&line : lines)
      line.draw();

    lv_label_set_text(autonLine, (autonT + autonName).c_str());
    lv_bar_set_value(bar, (int16_t)pros::battery::get_capacity());
    lv_label_set_text(bar_label, std::to_string((int16_t)pros::battery::get_capacity()).append("%").c_str());

    if (blpressed) {
      blpressed = false;
    }

    sprintf(tline, "v: %.3d | t: %.2f", (int)(launcher.getActualVelocity() + .5),
            launcher.getTemperature());
    controller::master.setText(2, 0, tline);
    controller::partner.setText(2, 0, tline);

    light.set_value(abs(launcher.getActualVelocity() - launcher.getTargetVelocity()) > 5 ||
                    launcher.getTargetVelocity() == 0);

    rate.delay(20);
  }
}
