#include QMK_KEYBOARD_H

extern keymap_config_t keymap_config;

#define BASE 0
#define META 1
#define SYMB 2
#define GAME 3

#define KC_KNA  KC_LANG1
#define KC_EISU KC_LANG2

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

  [BASE] = LAYOUT( \
    KC_TAB,   KC_Q,    KC_W,    KC_E,            KC_R,             KC_T,          KC_SLSH,        KC_BSLS, KC_Y,    KC_U,     KC_I,           KC_O,    KC_P,    KC_GRV, \
    KC_LCTRL, KC_A,    KC_S,    KC_D,            KC_F,             KC_G,          KC_MINS,        KC_EQL,  KC_H,    KC_J,     KC_K,           KC_L,    KC_SCLN, KC_QUOT, \
    KC_LSFT,  KC_Z,    KC_X,    KC_C,            KC_V,             KC_B,          KC_COMM,        KC_DOT,  KC_N,    KC_M,     KC_LBRC,        KC_RBRC, S(KC_9), S(KC_0), \
    KC_MEH,   XXXXXXX, KC_LALT, LGUI_T(KC_EISU), LT(SYMB, KC_ESC), SFT_T(KC_TAB), RCTL_T(KC_SPC), KC_ENT,  KC_BSPC, MO(META), RGUI_T(KC_KNA), KC_RALT, XXXXXXX, TG(GAME) \
  ),

  [META] = LAYOUT( \
    KC_CAPS, XXXXXXX, XXXXXXX, KC_7,    KC_8,    KC_9,    _______, _______, XXXXXXX,  KC_END, KC_HOME, XXXXXXX, XXXXXXX, XXXXXXX, \
    _______, XXXXXXX, XXXXXXX, KC_4,    KC_5,    KC_6,    _______, _______, KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, KC_PGDN, KC_PGUP, \
    _______, XXXXXXX, KC_0,    KC_1,    KC_2,    KC_3,    _______, _______, XXXXXXX, XXXXXXX, _______, _______, _______, _______, \
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, RESET,   _______, _______, _______ \
  ),

  [SYMB] = LAYOUT( \
    _______,   KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,  KC_F11,  KC_F12,   KC_F6,   KC_F7,   KC_F8,  KC_F9,      KC_F10, _______, \
    _______, S(KC_1), S(KC_2), S(KC_3), S(KC_4), S(KC_5), _______, _______, S(KC_6), S(KC_7), S(KC_8), KC_MINS, S(KC_MINS),  KC_EQL, \
    _______,    KC_1,    KC_2,    KC_3,    KC_4,    KC_5, _______, _______,    KC_6,    KC_7,    KC_8,    KC_9,       KC_0, _______, \
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,    _______, _______ \
  ),

  [GAME] = LAYOUT( \
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, \
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, \
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, \
    _______, _______, _______, _______, _______, _______,  KC_SPC, _______, _______, _______, _______, _______, _______, _______ \
  )

};

