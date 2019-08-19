/*
Copyright 2018 Sekigon
Copyright 2019 hatano.h

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include QMK_KEYBOARD_H
#include "app_ble_func.h"

enum custom_keycodes {
    // BLE keys
    AD_WO_L = SAFE_RANGE, /* Start advertising without whitelist  */
    SEL_BLE,              /* Select BLE HID Sending               */
    SEL_USB,              /* Select USB HID Sending               */
    TOG_HID,              /* Toggle BLE/USB HID Sending           */
    DELBNDS,              /* Delete all bonding                   */
    ADV_ID0,              /* Start advertising to PeerID 0        */
    ADV_ID1,              /* Start advertising to PeerID 1        */
    ADV_ID2,              /* Start advertising to PeerID 2        */
    ADV_ID3,              /* Start advertising to PeerID 3        */
    ADV_ID4,              /* Start advertising to PeerID 4        */
    ADV_ID5,              /* Start advertising to PeerID 5        */
    BATT_LV,              /* Display battery level in milli volts */
    DEL_ID0,              /* Delete bonding of PeerID 0           */
    DEL_ID1,              /* Delete bonding of PeerID 1           */
    DEL_ID2,              /* Delete bonding of PeerID 2           */
    DEL_ID3,              /* Delete bonding of PeerID 3           */
    DEL_ID4,              /* Delete bonding of PeerID 4           */
    DEL_ID5,              /* Delete bonding of PeerID 5           */
    ENT_DFU,              /* Start bootloader                     */
    ENT_SLP,              /* Deep sleep mode                      */
};

extern keymap_config_t keymap_config;

enum layer_number {
  _QWERTY = 0,
  _SYMB,
  _META,
  _CONFIG,
};

// Fillers to make layering more clear
#define _______ KC_TRNS
#define XXXXXXX KC_NO

// define KC_xxx code
#define KC_MAC    AG_SWAP
#define KC_WIN    AG_NORM
#define KC_SLP	  ENT_SLP

#define KC_____   _______
#define KC_XXXX   XXXXXXX

#define KC_KNA    KC_LANG1
#define KC_EISU   KC_LANG2

#define KC_META      MO(_META)
#define KC_CONFIG    MO(_CONFIG)
#define KC_SYMB_ESC  LT(_SYMB, KC_ESC)
#define KC_META_ESC  LT(_META, KC_ESC)
#define KC_LGUI_EISU LGUI_T(KC_EISU)
#define KC_RGUI_KNA  RGUI_T(KC_KNA)
#define KC_SFT_SPC   SFT_T(KC_SPC)
#define KC_LPAREN    S(KC_9)
#define KC_RPAREN    S(KC_0)

// define keymaps
const uint16_t keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  /* Qwerty
   * ,-------------------------------------------------------------------------------------------------.
   * | Tab  |   Q  |   W  |   E  |   R  |   T  |   /  |   \  |   Y  |   U  |   I  |   O  |   P  |  `   |
   * |------+------+------+------+------+------+------+------+------+------+------+------+------+------|
   * | Ctrl |   A  |   S  |   D  |   F  |   G  |   -  |   +  |   H  |   J  |   K  |   L  |   ;  |  '   |
   * |------+------+------+------+------+------+------+------+------+------+------+------+------+------|
   * | Shift|   Z  |   X  |   C  |   V  |   B  |   ,  |   .  |   N  |   M  |   ,  |   .  |   (  |  )   |
   * |------+------+------+------+------+------+------+------+------+------+------+------+------+------|
   * | Meh  |      |  Alt |  GUI | SYMB | Space|             | Enter| META | GUI  | Alt  |      | CONF |
   * `------+      +---------------------------+             +---------------------------+      +------'
   */
  [_QWERTY] = LAYOUT_kc(
    TAB,   Q, W,    E,        R,         T,       SLSH, BSLS, Y,   U,        I,        O,    P,      GRV, \
    LCTRL, A, S,    D,        F,         G,       MINS, EQL,  H,   J,        K,        L,    SCLN,   QUOT, \
    LSFT,  Z, X,    C,        V,         B,       COMM, DOT,  N,   M,        LBRC,     RBRC, LPAREN, RPAREN, \
    MEH,      LALT, LGUI_EISU, SYMB_ESC, SFT_SPC,             ENT, META_ESC, RGUI_KNA, RALT,         CONFIG \
  ),

  /* Meta
   * ,-------------------------------------------------------------------------------------------------.
   * | CAPS |      |      |   7  |   8  |   9  |      |      |      | End  | Home |      |      |  INS |
   * |------+------+------+------+------+------+------+------+------+------+------+------+------+------|
   * |      |      |      |   4  |   5  |   6  |      |      | Left | Down | Up   | Right| PgDn | PgUp |
   * |------+------+------+------+------+------+------+------+------+------+------+------+------+------|
   * |      |      |   0  |   1  |   2  |   3  |      |      | BSPC | Del  |      |      |      |      |
   * |------+------+------+------+------+------+------+------+------+------+------+------+------+------|
   * |      |      |      |      |      |      |             |      |      |      |      |      |      |
   * `------+      +---------------------------+             +---------------------------+      +------'
   */
  [_META] = LAYOUT_kc(
    CAPS, XXXX, XXXX,    7,    8,    9, ____, ____, XXXX,  END, HOME, XXXX, XXXX,  INS,
    ____, XXXX, XXXX,    4,    5,    6, ____, ____, LEFT, DOWN,   UP, RGHT, PGDN, PGUP,
    ____, XXXX,    0,    1,    2,    3, ____, ____, BSPC,  DEL, XXXX, ____, ____, ____,
    ____,       ____, ____, ____, ____,             ____, ____, ____, ____,       ____
  ),

  /* Symb
   * ,-------------------------------------------------------------------------------------------------.
   * |      |  F1  |  F2  |  F3  |  F4  |  F5  |  F11 |  F12 |  F6  |  F7  |  F8  |  F9  |  F10 |      |
   * |------+------+------+------+------+------+------+------+------+------+------+------+------+------|
   * |      |  !   |  @   |  #   |  $   |  %   |      |      |  ^   |  &   |  *   |  -   |  _   |  =   |
   * |------+------+------+------+------+------+------+------+------+------+------+------+------+------|
   * |      |  1   |  2   |  3   |  4   |  5   |      |      |  6   |  7   |  8   |  9   |  0   |      |
   * |------+------+------+------+------+------+------+------+------+------+------+------+------+------|
   * |      |      |      |      |      |      |             |      |      |      |      |      |      |
   * `------+      +---------------------------+             +---------------------------+      +------'
   */
  [_SYMB] =  LAYOUT(
    _______,   KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,  KC_F11,  KC_F12,   KC_F6,   KC_F7,   KC_F8,   KC_F9,     KC_F10, _______,
    _______, S(KC_1), S(KC_2), S(KC_3), S(KC_4), S(KC_5), _______, _______, S(KC_6), S(KC_7), S(KC_8), KC_MINS, S(KC_MINS),  KC_EQL,
    _______,    KC_1,    KC_2,    KC_3,    KC_4,    KC_5, _______, _______,    KC_6,    KC_7,    KC_8,    KC_9,       KC_0, _______,
    _______,          _______, _______, _______, _______,                   _______, _______, _______, _______,             _______
  ),

  /* Config
   * ,-------------------------------------------------------------------------------------------------.
   * | DFU  | ADV1 | ADV2 | ADV3 | ADV4 | ADV5 | MAC  | MAC  | DEL1 | DEL2 | DEL3 | DEL4 | DEL5 |      |
   * |------+------+------+------+------+------+------+------+------+------+------+------+------+------|
   * |      |      |      |      |      |      | WIN  | WIN  |      |      |      |      |      |      |
   * |------+------+------+------+------+------+------+------+------+------+------+------+------+------|
   * |      |SELBLE|SELUSB|      |      | ADV  | BATT |      |AU_ON |AU_OFF|      |      |      |      |
   * |------+------+------+------+------+------+------+------+------+------+------+------+------+------|
   * |      |      |      |      |      |      |             |      |      |      |      |      |      |
   * `------+      +---------------------------+             +---------------------------+      +------'
   */
  [_CONFIG] =  LAYOUT(
    ENT_DFU, ADV_ID1, ADV_ID2, ADV_ID3, ADV_ID4, ADV_ID5, AG_SWAP, AG_SWAP, DEL_ID1, DEL_ID2, DEL_ID3, DEL_ID4, DEL_ID5, XXXXXXX,
    _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, AG_NORM, AG_NORM, _______, _______, _______, _______, XXXXXXX, _______,
    _______, SEL_BLE, SEL_USB, XXXXXXX, XXXXXXX, AD_WO_L, BATT_LV, XXXXXXX, AU_ON,   AU_OFF,  XXXXXXX, XXXXXXX, XXXXXXX, _______,
    _______,          _______, _______, _______, _______,                   _______, XXXXXXX, _______, _______,          _______
  ),
};

static bool process_record_user_special(uint16_t keycode, bool pressed);

#ifndef TAPPING_TERM_PER_KEY
#define get_tapping_term(kc)	(TAPPING_TERM)
#endif

static bool process_record_user_special(uint16_t keycode, bool pressed) {
  switch (keycode) {
  case DELBNDS:
    if (pressed)
      delete_bonds();
    break;
  case AD_WO_L:
    if (pressed)
      restart_advertising_wo_whitelist();
    break;
  case SEL_BLE:
    if (pressed) {
      set_ble_enabled(true);
      set_usb_enabled(false);
    }
    break;
  case SEL_USB:
    if (pressed) {
      set_ble_enabled(false);
      set_usb_enabled(true);
    }
    break;
  case TOG_HID:
#ifndef NRF_SEPARATE_KEYBOARD_SLAVE
    // workaround:
    // get_ble_enabled() macro(in app_ble_func.h) is incorrect.
    if (pressed) {
      bool ble = get_ble_enabled();

      set_ble_enabled(!ble);
      set_usb_enabled(ble);
    }
#endif
    break;
  case ADV_ID0:
  case ADV_ID1:
  case ADV_ID2:
  case ADV_ID3:
  case ADV_ID4:
  case ADV_ID5:
    if (pressed)
      restart_advertising_id(keycode-ADV_ID0);
    break;
  case DEL_ID0:
  case DEL_ID1:
  case DEL_ID2:
  case DEL_ID3:
  case DEL_ID4:
  case DEL_ID5:
    if (pressed)
      delete_bond_id(keycode-DEL_ID0);
    break;
  case BATT_LV:
    if (pressed) {
      char str[16];

      sprintf(str, "%4dmV", get_vcc());
      send_string(str);
    }
    break;
  case ENT_DFU:
    if (pressed)
      bootloader_jump();
    break;
  case ENT_SLP:
    if (!pressed)
      sleep_mode_enter();
    break;

  default:
    // other unspecial keys
    return true;
  }
  return false;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
  default:
    return process_record_user_special(keycode, record->event.pressed);
  }

  return false;
}
