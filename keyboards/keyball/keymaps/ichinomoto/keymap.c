/*
Copyright 2021 @Yowkees

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

#include "pointing_device.h"
#include "../optical_sensor/optical_sensor.h"
#include "print.h"

extern keymap_config_t keymap_config;

bool isScrollMode;

enum keymap_layers {
  _QWERTY,
  _LOWER,
  _RAISE,
  _BALL,
};

enum custom_keycodes {
  QWERTY = SAFE_RANGE,
  LOWER,
  RAISE,
  BALL,
  KC_MBTN1,
  KC_MBTN2,
  KC_MBTN3,
  RGBRST,
};

// common
#define KC_ KC_TRNS
#define KC_XXXX KC_NO
#define KC_RST RESET
#define KC_VD KC__VOLDOWN
#define KC_VU KC__VOLUP

// layer
#define KC_L_SPC LT(_LOWER, KC_SPC)
#define KC_R_ENT LT(_RAISE, KC_ENT)

// shift_t
#define KC_S_EN LSFT_T(KC_LANG2)

// original
#define KC_A_JA LT(_BALL, KC_LANG1)   // cmd or adjust 
#define KC_AL_CP MT(MOD_LALT, KC_CAPS)  // alt or caps lock
#define KC_G_BS MT(MOD_LGUI, KC_BSPC)   // command or back space
#define KC_G_DEL MT(MOD_LGUI, KC_DEL)   // command or delete
#define KC_A_BS LT(_BALL, KC_BSPC)    // adjust or back space
#define KC_A_DEL LT(_BALL, KC_DEL)    // adjust or delete

#define KC_RAISE RAISE    // adjust or delete
#define KC_LOWER LOWER    // adjust or delete
#define KC_BALL BALL    // adjust or delete
#define KC_RGBRST RGBRST    // adjust or delete


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

  [_QWERTY] = LAYOUT_kc( \
  //,--------+--------+--------+--------+--------+--------.           ,--------+--------+--------+--------+--------+--------.
       Q     ,  W     ,  E     ,  R     ,  T     ,MBTN1               ,  Y     ,  U     ,  I     ,  O     ,  P     , BSPC   ,
  //|--------+--------+--------+--------+--------+--------|           |--------+--------+--------+--------+--------+--------|
       A     ,  S     ,  D     ,  F     ,  G     ,MBTN2               ,  H     ,  J     ,  K     ,  L     , SCLN   , QUOT   ,
  //|--------+--------+--------+--------+--------+--------|           |--------+--------+--------+--------+--------+--------|
       Z     ,  X     ,  C     ,  V     ,  B     ,MBTN3               ,  N     ,  M     , COMM   , DOT    , SLSH   , ENT    ,
  //|--------+--------+--------+--------+--------+--------|           |--------+--------+--------+--------+--------+--------|
      LSFT   , LALT   ,     LOWER  ,L_SPC   ,LCTL    ,ESC        ,MBTN1  ,RAISE   ,LOWER     ,      TAB     , BSLS   , RSFT 
  //|--------+--------+    +-------+--------+--------+--------|  |-------+--------+--------+     +--------+--------+--------|
  ),

  [_RAISE] = LAYOUT( \
  //,--------+--------+--------+--------+--------+--------.           ,--------+--------+--------+--------+--------+--------.
     KC_1    ,KC_2    ,KC_3    ,KC_4    ,KC_5    ,KC_MBTN1,           KC_6     ,KC_7    ,KC_8    ,KC_9    ,KC_0    ,KC_DEL  ,
  //|--------+--------+--------+--------+--------+--------|           |--------+--------+--------+--------+--------+--------|
     XXXXXXX ,XXXXXXX ,XXXXXXX ,XXXXXXX ,XXXXXXX ,KC_MBTN2,            KC_LEFT ,KC_DOWN , KC_UP  ,KC_RGHT ,KC_LBRC ,KC_RBRC ,
  //|--------+--------+--------+--------+--------+--------|           |--------+--------+--------+--------+--------+--------|
     XXXXXXX ,XXXXXXX ,XXXXXXX ,XXXXXXX ,XXXXXXX ,KC_MBTN3            ,KC_HOME ,KC_PGDN ,KC_PGUP ,KC_END  ,XXXXXXX ,KC_RSFT ,
  //|--------+--------+--------+--------+--------+--------|           |--------+--------+--------+--------+--------+--------|
     KC_LSFT ,KC_LALT ,    LOWER   ,KC_SPC  ,KC_LCTL ,KC_ESC     ,KC_MBTN3,RAISE  ,LOWER   ,      XXXXXXX ,XXXXXXX ,XXXXXXX      
  //|--------+--------+    +-------+--------+--------+--------|  |-------+--------+--------+     +--------+--------+--------|
  ),

  [_LOWER] = LAYOUT( \
  //,--------+--------+--------+--------+--------+--------.           ,--------+--------+--------+--------+--------+--------.
     KC_EXLM , KC_AT  ,KC_HASH , KC_DLR ,KC_PERC ,KC_MBTN1,            KC_CIRC ,KC_AMPR ,KC_ASTR ,KC_MINS , KC_EQL , KC_DEL ,
  //|--------+--------+--------+--------+--------+--------|           |--------+--------+--------+--------+--------+--------|
     KC_F1   , KC_F2  , KC_F3  , KC_F4  , KC_F5  ,KC_MBTN2,            KC_MBTN1,KC_MBTN2,XXXXXXX ,KC_LCBR ,KC_RCBR ,XXXXXXX ,
  //|--------+--------+--------+--------+--------+--------|           |--------+--------+--------+--------+--------+--------|
     KC_F6   , KC_F7  , KC_F8  , KC_F9  , KC_F10 ,KC_MBTN3            , KC_F11 , KC_F12 ,XXXXXXX ,KC_LBRC ,KC_RBRC ,KC_RSFT ,
  //|--------+--------+--------+--------+--------+--------|           |--------+--------+--------+--------+--------+--------|
     KC_LSFT ,KC_LALT ,    LOWER   ,KC_SPC  ,KC_LCTL ,KC_ESC     ,KC_MBTN3,RAISE  ,LOWER   ,      XXXXXXX ,XXXXXXX ,XXXXXXX      
  //|--------+--------+    +-------+--------+--------+--------|  |-------+--------+--------+     +--------+--------+--------|
  ),

  [_BALL] = LAYOUT( \
  //,-----------------------------------------------------.          ,-----------------------------------------------------.
        RESET,  RGBRST, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,            XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|          |--------+--------+--------+--------+--------+--------|
      RGB_TOG, RGB_HUI, RGB_SAI, RGB_VAI, XXXXXXX, XXXXXXX,            XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|          |--------+--------+--------+--------+--------+--------|
      RGB_MOD, RGB_HUD, RGB_SAD, RGB_VAD, XXXXXXX, XXXXXXX,            XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------+          +--------+--------+--------+--------+--------+--------|
      XXXXXXX, XXXXXXX,     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,   XXXXXXX, XXXXXXX, XXXXXXX,      XXXXXXX, XXXXXXX, XXXXXXX
  //|--------+--------+    +-------+--------+--------+--------|  |-------+--------+--------+    +--------+--------+--------|
  ),

};


#ifdef RGBLIGHT_ENABLE
extern rgblight_config_t rgblight_config;
rgblight_config_t        RGB_current_config;
#endif

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    report_mouse_t currentReport = {};
    uprintf("process_record_user IN %d\n", keycode);

    switch (keycode) {
        case LOWER:
            if (record->event.pressed) {
                layer_on(_LOWER);
                isScrollMode = true;
                update_tri_layer(_LOWER, _RAISE, _BALL);
            } else {
                layer_off(_LOWER);
                isScrollMode = false;
                update_tri_layer(_LOWER, _RAISE, _BALL);
            }
            return false;
            break;
        case RAISE:
            if (record->event.pressed) {
                layer_on(_RAISE);
                update_tri_layer(_LOWER, _RAISE, _BALL);
            } else {
                layer_off(_RAISE);
                update_tri_layer(_LOWER, _RAISE, _BALL);
            }
            return false;
            break;
        case BALL:
            if (record->event.pressed) {
                layer_on(_BALL);
            } else {
                layer_off(_BALL);
            }
            return false;
            break;
        case KC_MBTN1:
            print("push KC_MBTN1");
            currentReport = pointing_device_get_report();
            if (record->event.pressed) {
                currentReport.buttons |= MOUSE_BTN1;
            }
            else {
                currentReport.buttons &= ~MOUSE_BTN1;
            }
            pointing_device_set_report(currentReport);
            pointing_device_send();
            return false;
            break;
        case KC_MBTN2:
            print("push KC_MBTN2");
            currentReport = pointing_device_get_report();
            if (record->event.pressed) {
                currentReport.buttons |= MOUSE_BTN2;
            }
            else {
                currentReport.buttons &= ~MOUSE_BTN2;
            }
            pointing_device_set_report(currentReport);
            pointing_device_send();
            return false;
            break;
        case KC_MBTN3:
            print("push KC_MBTN3");
            currentReport = pointing_device_get_report();
            if (record->event.pressed) {
                currentReport.buttons |= MOUSE_BTN3;
            }
            else {
                currentReport.buttons &= ~MOUSE_BTN3;
            }
            pointing_device_set_report(currentReport);
            pointing_device_send();
            return false;
            break;
        case RGBRST:
            #ifdef RGBLIGHT_ENABLE
              if (record->event.pressed) {
                  eeconfig_update_rgblight_default();
                  rgblight_enable();
                  RGB_current_config.mode = rgblight_config.mode;
              }
            #endif
            break;
    }
    return true;
}
    
/*#ifndef MOUSEKEY_ENABLE
    if (IS_MOUSEKEY_BUTTON(keycode)) {
        report_mouse_t currentReport = pointing_device_get_report();
        if (record->event.pressed) {
            currentReport.buttons |= 1 << (keycode - KC_MS_BTN1);
        } else {
            currentReport.buttons &= ~(1 << (keycode - KC_MS_BTN1));
        }
        pointing_device_set_report(currentReport);
        pointing_device_send();
    }
#endif
    return true;
}*/



#define CLAMP_HID(value) value < -127 ? -127 : value > 127 ? 127 : value

void pointing_device_init(void) {
    if (is_keyboard_master()){
        optical_sensor_init();
    }
}

void keyboard_post_init_user() {
    debug_enable = true;
    debug_mouse = true;
}

void pointing_device_task(void) {
    if (!is_keyboard_master())
        return;
    static int  cnt;
    static int16_t avg_x, avg_y;

    report_mouse_t mouse_rep = pointing_device_get_report();
    report_optical_sensor_t sensor_report = optical_sensor_get_report();

    int16_t raw_x = - sensor_report.x;
    int16_t raw_y = - sensor_report.y;

#if 0
    int8_t clamped_x;
    int8_t clamped_y;
    if (isScrollMode) {
        if (cnt++ % 10 == 0) {
               avg_x = avg_x / 10;
               avg_y = avg_y / 10;
               clamped_x = CLAMP_HID(avg_x);
               clamped_y = CLAMP_HID(avg_y);
            mouse_rep.h = -clamped_x/10;
            mouse_rep.v = -clamped_y/10;
        } else {
            avg_x += raw_x;
            avg_y += raw_y;
        }
    } else {
        if (cnt++ % 10 == 0) {
            avg_x = avg_x / 10;
            avg_y = avg_y / 10;
//            if(avg_x*avg_x >= 225) avg_x*=2;
//            if(avg_y*avg_y >= 225) avg_y*=2;
            clamped_x = CLAMP_HID(avg_x);
            clamped_y = CLAMP_HID(avg_y);
            
            mouse_rep.x = -clamped_x;
            mouse_rep.y = clamped_y;
        }else{
            avg_x += raw_x;
            avg_y += raw_y;
        }
    }
#else
    if (cnt++ % 10 == 0) {
        avg_x = avg_x / 10;
        avg_y = avg_y / 10;
//        if(avg_x*avg_x >= 225) avg_x*=2;
//        if(avg_y*avg_y >= 225) avg_y*=2;
        int8_t clamped_x = CLAMP_HID(avg_x);
        int8_t clamped_y = CLAMP_HID(avg_y);
        
        if (isScrollMode) {
            mouse_rep.h = -clamped_x/10;
            mouse_rep.v = -clamped_y/10;
        } else {
            mouse_rep.x = -clamped_x;
            mouse_rep.y = clamped_y;
        }
    }else{
        avg_x += raw_x;
        avg_y += raw_y;
    }
#endif
        

    if (mouse_rep.x!=0 || mouse_rep.y!=0 || mouse_rep.v!=0 || mouse_rep.h!=0) {
        uprintf("call pointing_device_set_report %d, %d\n", mouse_rep.x, mouse_rep.y);
        pointing_device_set_report(mouse_rep);
        pointing_device_send();
    }
}

//layer_state_t layer_state_set_user(layer_state_t state) {
//    switch (get_highest_layer(state)) {
//    case _BALL:
//        isScrollMode = true;
//        break;
//    default:
//        isScrollMode = false;
//        break;
//    }
//  return state;
//}


#ifdef OLED_DRIVER_ENABLE

void render_layer_state(void) {
    switch (get_highest_layer(layer_state)) {
        case _QWERTY:
            oled_write_ln_P(PSTR("Layer: Default"), false);
            break;
        case _RAISE:
            oled_write_ln_P(PSTR("Layer: Raise"), false);
            break;
        case _LOWER:
            oled_write_ln_P(PSTR("Layer: Lower"), false);
            break;
        case _BALL:
            oled_write_ln_P(PSTR("Layer: Ball"), false);
            break;
        default:
            oled_write_ln_P(PSTR("Layer: Undefined"), false);
    }
}

void render_logo(void) {
    static const char PROGMEM logo[] = {0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f, 0x90, 0x91, 0x92, 0x93, 0x94, 0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf, 0xb0, 0xb1, 0xb2, 0xb3, 0xb4, 0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf, 0xd0, 0xd1, 0xd2, 0xd3, 0xd4, 0};
    oled_write_P(logo, false);
}

void oled_task_user(void) {
    if (is_keyboard_master()) {
        render_layer_state();
    } else {
        render_logo();
    }
}

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    if (!is_keyboard_master()) return OLED_ROTATION_180;
    return rotation;
}

#endif
