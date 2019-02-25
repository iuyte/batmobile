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

  return selectMenu(menu);
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
      return atext + std::to_string(motor->getTemperature() * (9 / 5) + 32);
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
  void align() { lv_obj_align(line, cont, LV_ALIGN_IN_TOP_LEFT, 0, 20 * number); }

private:
  ValueType valueType;
};

pros::Mutex     mutex;
bool            bpressed = false;
static lv_res_t selectionPress(lv_obj_t *btn, const char *txt) {
  bpressed = true;

  return LV_RES_OK;
}

extern long int glt;

void infoLoop(void *generated) {
  unsigned int nlines = 0;
  char         tline[20];
  char         batteryText[20];
  char         cataPotText[20];
  string       autonT = "autonomous: ";
  string       gyroT  = "gyro: ";
  cont                = lv_cont_create(lv_scr_act(), NULL);
  lv_cont_set_fit(cont, false, false);
  lv_obj_set_size(cont, LV_HOR_RES, LV_VER_RES);

  bool *displayController = (bool *)(generated);

  // clang-format off
  Line lines[] = {
          Line("catapult",    &catapult::motor, Line::Velocity,    nlines++),
          Line("catapult",    &catapult::motor, Line::Temperature, nlines++),
          Line("catapult",    &catapult::motor, Line::Power,       nlines++),
          Line("intake",      &intake,          Line::Velocity,    nlines++),
          Line("left drive",  &drive::left,     Line::Position,    nlines++),
          Line("right drive", &drive::right,    Line::Position,    nlines++),
          Line("left drive",  &drive::left,     Line::Velocity,    nlines++),
          Line("right drive", &drive::right,    Line::Velocity,    nlines++),
          Line("left drive",  &drive::left,     Line::Temperature, nlines++),
          Line("right drive", &drive::right,    Line::Temperature, nlines++),
  };

  static SwitcherMenu rootMenu("main menu", {
          SwitcherMenu("flags", {
            SwitcherMenu("+ cap", {
              SwitcherMenu("red", {}, &autonRedFlagsCap),
              SwitcherMenu("blue", {}, &autonRedFlagsCap),
            }),
            SwitcherMenu("+ 2nd tree", {
              SwitcherMenu("red", {}, &autonRedFlagsX2),
              SwitcherMenu("blue", {}, &autonRedFlagsX2),
          })}),
          SwitcherMenu("caps", {
              SwitcherMenu("red", {}, &autonRedCaps),
              SwitcherMenu("blue", {}, &autonBlueCaps),
          }),
          SwitcherMenu("other", {
            SwitcherMenu("data", {}, &printData),
            SwitcherMenu("test", {}, &autonTest),
            SwitcherMenu("skills", {
              SwitcherMenu("traditional", {}, &autonSkills1),
              SwitcherMenu("new", {}, &autonSkills2),
            }),
          }),
          SwitcherMenu("none", {}, &doNothing),
  });
  // clang-format on

  for (auto &&line : lines)
    line.align();

  auto autonLine = lv_label_create(cont, NULL);
  lv_obj_align(autonLine, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 20 * nlines);
  auto gyroLine = lv_label_create(cont, NULL);
  lv_obj_align(gyroLine, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 20 * (nlines + 1));

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

  // Create a simple style with ticker line width
  static lv_style_t style_lmeter;
  lv_style_copy(&style_lmeter, &lv_style_pretty_color);
  style_lmeter.line.width      = 2;
  style_lmeter.line.color      = LV_COLOR_SILVER;
  style_lmeter.body.main_color = LV_COLOR_LIME;
  style_lmeter.body.grad_color = LV_COLOR_ORANGE;

  // Create the speed meter
  lv_obj_t *meter;
  meter = lv_lmeter_create(cont, NULL);
  lv_lmeter_set_range(meter, 1300, 2400);    // Set the range
  lv_lmeter_set_value(meter, 0);             // Set the current value
  lv_lmeter_set_style(meter, &style_lmeter); // Apply the new style
  lv_obj_set_size(meter, 80, 80);
  lv_obj_align(meter, NULL, LV_ALIGN_IN_BOTTOM_RIGHT, -10, 0);
  lv_lmeter_set_style(meter, &style_lmeter);

  // Add a label to show the current value
  lv_obj_t *m_label;
  m_label = lv_label_create(meter, NULL);
  lv_obj_align(m_label, NULL, LV_ALIGN_CENTER, 2, 0);
  lv_label_set_style(m_label, &lv_style_plain_color);

  mutex.give();
  Rate  rate;
  short batteryCapacity, catapultVal;
  while (true) {
    if (bpressed) {
      lv_obj_set_hidden(cont, true);
      for (auto &&line : lines)
        line.clear();
      lv_label_set_text(autonLine, "");
      lv_label_set_text(gyroLine, "");

      auto s    = selectMenu(&rootMenu);
      auton     = s.first;
      autonName = s.second;

      bpressed = false;
      lv_obj_set_hidden(cont, false);
    }

    // update info lines
    for (auto &&line : lines)
      line.draw();

    // update autonomous name
    lv_label_set_text(autonLine, (autonT + autonName).c_str());
    lv_label_set_text(
            gyroLine,
            (gyroT + std::to_string(drive::getAngle()) + " " + std::to_string(glt)).c_str());

    catapultVal     = (short)(catapult::pot.get());
    batteryCapacity = (short)pros::battery::get_capacity();

    // update battery capacity
    lv_bar_set_value(bar, batteryCapacity);
    snprintf(batteryText, 15, "%3d%%", batteryCapacity);
    lv_label_set_text(bar_label, batteryText);

    // update catapult meter
    lv_lmeter_set_value(meter, catapultVal);
    snprintf(cataPotText, 15, "%4d", catapultVal);
    lv_label_set_text(m_label, cataPotText);

    // print values to the controllers
    snprintf(tline, 15, "%d | %f", catapultVal, catapult::motor.getTemperature());
    if (*displayController)
      controller::master.setText(2, 0, tline);

    // run 20 times per second
    rate.delay(20_Hz);
  }
}
