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
    NORM = SAFE_RANGE,	  /* Normal mode */
    GAME,		  /* Game mode */
    AD_WO_L,		  /* Start advertising without whitelist  */
    SEL_BLE,		  /* Select BLE HID Sending		  */
    SEL_USB,		  /* Select USB HID Sending		  */
    TOG_HID,		  /* Toggle BLE/USB HID Sending		  */
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

    // other keys
    xEISU,
    xKANA,

    // layer control
    NUMS,
    SYMS,
    FUNC,
    CONF,

    // combination keycode and layer
    SYM_ENT,
    FNC_KANA,
    NUM_SPC,
};

extern keymap_config_t keymap_config;

enum layer_number {
    _BASE = 0,
    _GAME,
    _NUMS,
    _SYMS,
    _FUNC,
    _CONF,
    _LAYERSIZE,
};

// Fillers to make layering more clear
#define _______ KC_TRNS
#define XXXXXXX KC_NO

#define TD_ESC KC_ESC
#define TD_LCTL LCTL_T(KC_TAB)
#define TD_LSFT LSFT_T(KC_BSPC)
#define TD_SPC NUM_SPC
#define TD_ENT SYM_ENT
#define TD_KANA FNC_KANA

////////////////////////////////////////////////////////////////////////
// 
struct layer_press {
  bool pressed;
  uint16_t time;
};
static struct layer_press layer_pressed[_LAYERSIZE];
static void clear_keystate()
{
  memset(layer_pressed, 0, sizeof(layer_pressed));
}
static bool set_layer(int layer, keyrecord_t *record);
static void unset_layer(keyrecord_t *record);
static bool set_layer_and_key(int layer, uint16_t kc, keyrecord_t *record);
static bool process_record_user_custom(uint16_t keycode, bool pressed);
static bool process_record_user_custom2(uint16_t keycode, bool pressed);

////////////////////////////////////////////////////////////////////////
// define keymaps
const uint16_t keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  /* BASE
   * +------+------+------+------+------+------+                  +------+------+------+------+------+------+
   * | ESC  |   Q  |   W  |   E  |   R  |   T  |                  |   Y  |   U  |   I  |   O  |   P  |  BS  |
   * +------+--+---+--+---+--+---+--+---+--+---+--+     +------+  +--+---+--+---+--+---+--+---+--+---+------+
   * |  Ctrl   |   A  |   S  |   D  |   F  |   G  |     |BS    |     |   H  |   J  |   K  |   L  |  Enter   |   
   * +---------+---+--+---+--+---+--+---+--+---+--+---+ +------+     +---+--+---+--+---+--+------+-+--------+
   * |    Shift    |   Z  |   X  |   C  |   V  |   B  | |Space |         |   N  |   M  |   KANA    |  Shift |
   * +-------------+------+------+------+------+------+ +------+         +------+------+------+----+--------+
   */
    [_BASE] = LAYOUT(
        TD_ESC,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,             KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSPC,
        TD_LCTL, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,   KC_BSPC,  KC_H,    KC_J,    KC_K,    KC_L,    TD_ENT,
        TD_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,   TD_SPC,   KC_N,    KC_M,    TD_KANA, KC_RSFT,
	                  KC_NO,   KC_NO,   KC_NO,                              KC_NO,   KC_NO,   KC_NO
    ),

  /* NUM
   * +------+------+------+------+------+------+                  +------+------+------+------+------+------+
   * |      |   1  |   2  |   3  |   4  |   5  |                  |   6  |   7  |   8  |   9  |   0  |  -   |
   * +------+--+---+--+---+--+---+--+---+--+---+--+     +------+  +--+---+--+---+--+---+--+---+--+---+------+
   * |         |      |   \  |   `  |   [  |   -  |     |      |     |   =  |   ]  |   '  |   ;  |          |   
   * +---------+---+--+---+--+---+--+---+--+---+--+---+ +------+     +---+--+---+--+---+--+------+-+--------+
   * |             |  ,   |  .   |   /  |      |      | |      |         |      |      |           |        |
   * +-------------+------+------+------+------+------+ +------+         +------+------+------+----+--------+
   */
    [_NUMS] = LAYOUT(
        _______, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,             KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS,
        _______, XXXXXXX, KC_BSLS, KC_GRV,  KC_LBRC, KC_MINS, _______, KC_EQL,  KC_RBRC, KC_QUOT, KC_SCLN, _______,
        _______, KC_COMM, KC_DOT,  KC_SLSH, XXXXXXX, XXXXXXX, _______, XXXXXXX, XXXXXXX, _______, _______,
			  _______, _______, _______,                            _______, _______, _______
    ),

  /* SYMS
   * +------+------+------+------+------+------+                  +------+------+------+------+------+------+
   * |      |   !  |   @  |   #  |   $  |   %  |                  |   ^  |   &  |   *  |   (  |   )  |  _   |
   * +------+--+---+--+---+--+---+--+---+--+---+--+     +------+  +--+---+--+---+--+---+--+---+--+---+------+
   * |         |      |   |  |   ~  |   {  |   _  |     |      |     |   +  |   }  |   "  |   :  |          |   
   * +---------+---+--+---+--+---+--+---+--+---+--+---+ +------+     +---+--+---+--+---+--+------+-+--------+
   * |             |  ,   |  .   |   /  |      |      | |      |         |      |      |           |        |
   * +-------------+------+------+------+------+------+ +------+         +------+------+------+----+--------+
   */
    [_SYMS] = LAYOUT(
        _______, KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC,          KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_UNDS,
        _______, XXXXXXX, KC_PIPE, KC_TILD, KC_LCBR, KC_UNDS, _______, KC_PLUS, KC_RCBR, KC_DQUO, KC_COLN, _______,
        _______, KC_LABK, KC_RABK, KC_QUES, XXXXXXX, XXXXXXX, _______, XXXXXXX, XXXXXXX, _______, _______,
			  _______, _______, _______,                            _______, _______, _______
    ),

  /* FUNC
   * +------+------+------+------+------+------+                  +------+------+------+------+------+------+
   * |      |  F1  |  F2  |  F3  |  F4  |  F5  |                  |  F6  |  F7  |  F8  |  F9  | F10  | DEL  |
   * +------+--+---+--+---+--+---+--+---+--+---+--+     +------+  +--+---+--+---+--+---+--+---+--+---+------+
   * |         | vol+ | vol- | mute |      |      |     |      |     | left | down |  up  |right |          |   
   * +---------+---+--+---+--+---+--+---+--+---+--+---+ +------+     +---+--+---+--+---+--+------+-+--------+
   * |             |  ALT |  GUI |  APP |      |      | |      |         |      |      |           |        |
   * +-------------+------+------+------+------+------+ +------+         +------+------+------+----+--------+
   */
    [_FUNC] = LAYOUT(
        _______, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,            KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_DEL,
        _______, KC_VOLU, KC_VOLD, KC_MUTE, XXXXXXX, XXXXXXX, _______, KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, _______,
        KC_DEL,  KC_LALT, KC_LGUI, KC_APP,  XXXXXXX, XXXXXXX, _______, XXXXXXX, XXXXXXX, _______, _______,
			  _______, _______, _______,                            _______, _______, _______ 
    ),

  /* Config
   * +------+------+------+------+------+------+                  +------+------+------+------+------+------+
   * |      |  ADV | ADV0 | ADV1 | ADV2 | ADV3 |                  | ADV4 |      |      |      |      | DFU  |
   * +------+--+---+--+---+--+---+--+---+--+---+--+     +------+  +--+---+--+---+--+---+--+---+--+---+------+
   * |         |SELBLE|SELUSB|      |      | WIN  |     |      |     | MAC  |BAT_LV| GAME | NORM |          |   
   * +---------+---+--+---+--+---+--+---+--+---+--+---+ +------+     +---+--+---+--+---+--+------+-+--------+
   * |             |      |      |      |      |      | |      |         |      |      |           |        |
   * +-------------+------+------+------+------+------+ +------+         +------+------+------+----+--------+
   */
    [_CONF] = LAYOUT(
        _______, AD_WO_L, ADV_ID0, ADV_ID1, ADV_ID2, ADV_ID3,          ADV_ID4, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, ENT_DFU,
        _______, SEL_BLE, SEL_USB, XXXXXXX, XXXXXXX, AG_NORM, _______, AG_SWAP, BATT_LV, GAME,    NORM,    _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
			  _______, _______, _______,                            _______, _______, _______ 
    ),

  /* GAME
   * +------+------+------+------+------+------+                  +------+------+------+------+------+------+
   * |      |   1  |   2  |   3  |   4  |   5  |                  |   6  |   7  |   8  |   9  |   0  |      |
   * +------+--+---+--+---+--+---+--+---+--+---+--+     +------+  +--+---+--+---+--+---+--+---+--+---+------+
   * |         |   A  |   B  |   C  |   D  |   E  |     |      |     |   F  |   G  |   H  |   I  |          |
   * +---------+---+--+---+--+---+--+---+--+---+--+---+ +------+     +---+--+---+--+---+--+------+-+--------+
   * |             |  Alt |  GUI |  APP |      |      | |      |         |      |      |           |        |
   * +-------------+------+------+------+------+------+ +------+         +------+------+------+----+--------+
   */
    [_GAME] = LAYOUT(
        _______, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,             KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    _______,
        _______, KC_A,    KC_B,    KC_C,    KC_D,    KC_E,    _______, KC_F,    KC_G,    KC_H,    KC_I,    _______,
        _______, KC_LALT, KC_LGUI, KC_APP,  XXXXXXX, XXXXXXX, _______, XXXXXXX, XXXXXXX, _______, _______,
			  _______, _______, _______,                            _______, _______, _______ 
    ),
};

////////////////////////////////////////////////////////////////////////
// 
#ifndef TAPPING_TERM_PER_KEY
#define get_tapping_term(kc)	(TAPPING_TERM)
#endif

static bool set_layer(int layer, keyrecord_t *record) {
  if (record->event.pressed) {
    layer_pressed[layer].time = record->event.time;
    layer_pressed[layer].pressed = true;
    layer_on(layer);
    update_tri_layer(_NUMS, _FUNC, _CONF);
  } else {
    layer_off(layer);
    update_tri_layer(_NUMS, _FUNC, _CONF);
    layer_pressed[layer].pressed = false;
  }
  return false;
}

static void unset_layer(keyrecord_t *record) {
  if (record->event.pressed) {
    memset(layer_pressed, 0, sizeof(layer_pressed));
  }
}

static bool set_layer_and_key(int layer, uint16_t kc, keyrecord_t *record) {
  if (keyboard_report->mods & (MOD_BIT(KC_LCTL)|MOD_BIT(KC_RCTL))) {
    // layer is not effect, if ctrl press 
    unset_layer(record);
    process_record_user_custom2(kc, record->event.pressed);
  } else if (record->event.pressed) {
    layer_pressed[layer].time = record->event.time;
    layer_pressed[layer].pressed = true;
    layer_on(layer);
    update_tri_layer(_NUMS, _FUNC, _CONF);
  } else {
    layer_off(layer);
    update_tri_layer(_NUMS, _FUNC, _CONF);
    if (layer_pressed[layer].pressed && 
	(TIMER_DIFF_16(record->event.time, layer_pressed[layer].time) < get_tapping_term(kc))) {
      process_record_user_custom2(kc, true);
      process_record_user_custom2(kc, false);
    }
    layer_pressed[layer].pressed = false;
  }
  return false;
}

////////////////////////////////////////////////////////////////////////
// custum key function
static bool process_record_user_custom2(uint16_t kc, bool pressed) {
  if (process_record_user_custom(kc, pressed)) {
    if (pressed) {
      register_code(kc);
    } else {
      unregister_code(kc);
    }
  }
  return false;
}
static bool process_record_user_custom(uint16_t keycode, bool pressed) {
  switch (keycode) {
  case NORM:
    if (pressed) {
      set_single_persistent_default_layer(_BASE);
    }
    break;
  case GAME:
    if (pressed) {
      set_single_persistent_default_layer(_GAME);
    }
    break;
  case xEISU:
    if (pressed) {
      if (keymap_config.swap_lalt_lgui){
	register_code(KC_LANG2);
      } else {
	SEND_STRING(SS_LALT("`"));
      }
    } else {
      unregister_code(KC_LANG2);
    }
    break;
  case xKANA:
    if (pressed) {
      if (keymap_config.swap_lalt_lgui){
	register_code(KC_LANG1);
      } else {
	SEND_STRING(SS_LALT("`"));
      }
    } else {
      unregister_code(KC_LANG1);
    }
    break;
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
////////////////////////////////////////////////////////////////////////
// extern
void matrix_init_user(void)
{
  clear_keystate();
}
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    // layer function
  case NUMS:
    return set_layer(_NUMS, record);
  case SYMS:
    return set_layer(_SYMS, record);
  case FUNC:
    return set_layer(_FUNC, record);
  case CONF:
    return set_layer(_CONF, record);

    // combine keycode and layer key
  case SYM_ENT:
    return set_layer_and_key(_SYMS, KC_ENT, record);
  case FNC_KANA:
    return set_layer_and_key(_FUNC, xKANA, record);
  case NUM_SPC:
    return set_layer_and_key(_NUMS, KC_SPC, record);

    // special function key
  default:
    unset_layer(record);
    return process_record_user_custom(keycode, record->event.pressed);
  }
  return false;
}


