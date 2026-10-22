/* Copyright 2022 DOIO
 * Copyright 2022 HorrorTroll <https://github.com/HorrorTroll>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include QMK_KEYBOARD_H

// OLED animation
#include "lib/layer_status/layer_status.h"

// Each layer gets a name for readability, which is then used in the keymap matrix below.
// The underscores don't mean anything - you can have a layer called STUFF or any other name.
// Layer names don't all need to be of the same length, obviously, and you can also skip them
// entirely and just use numbers.

enum layer_names {
    _BASE,
    _FN,
    _FN1,
    _FN2
};

// enum layer_keycodes { };

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

/*
       ┌───┬───┬───┬───┐   ┌───┐ ┌───┐
       │ 1 │ 2 │ 3 │ 4 │   │Ply│ │TO1│
       ├───┼───┼───┼───┤   └───┘ └───┘
       │ 5 │ 6 │ 7 │ 8 │
       ├───┼───┼───┼───┤
       │ 9 │ 0 │ ↑ │Ent│      ┌───┐
       ├───┼───┼───┼───┤      │Mut│
       │Fn2│ ← │ ↓ │ → │      └───┘
       └───┴───┴───┴───┘
       ┌───┬───┬───┬───┐   ┌───┐ ┌───┐
       │ ! │ @ │ # │ $ │   │   │ │   │
       ├───┼───┼───┼───┤   └───┘ └───┘
       │ % │ ^ │ & │ * │
       ├───┼───┼───┼───┤
       │ ( │ ) │   │   │      ┌───┐
       ├───┼───┼───┼───┤      │   │
       │   │   │   │   │      └───┘
       └───┴───┴───┴───┘
*/
    /*  Row:    0         1        2        3         4      */
    [_BASE] = LAYOUT(
                KC_1,     KC_2,    KC_3,    KC_4,     KC_MPLY,
                KC_5,     KC_6,    KC_7,    KC_8,     TO(_FN),
                KC_9,     KC_0,    KC_UP,   KC_ENT,   KC_MUTE,
                MO(_FN2), KC_LEFT, KC_DOWN, KC_RIGHT
            ),

/*
       ┌───┬───┬───┬───┐   ┌───┐ ┌───┐
       │   │   │   │   │   │   │ │   │
       ├───┼───┼───┼───┤   └───┘ └───┘
       │   │   │   │   │
       ├───┼───┼───┼───┤
       │   │   │   │   │      ┌───┐
       ├───┼───┼───┼───┤      │   │
       │   │   │   │   │      └───┘
       └───┴───┴───┴───┘
*/
    /*  Row:    0        1        2        3        4       */
    [_FN] = LAYOUT(
                _______, _______, _______, _______, _______,
                _______, _______, _______, _______, TO(_FN1),
                _______, _______, _______, _______, _______,
                _______, _______, _______, _______
            ),

/*
       ┌───┬───┬───┬───┐   ┌───┐ ┌───┐
       │   │   │   │   │   │   │ │   │
       ├───┼───┼───┼───┤   └───┘ └───┘
       │   │   │   │   │
       ├───┼───┼───┼───┤
       │   │   │   │   │      ┌───┐
       ├───┼───┼───┼───┤      │   │
       │   │   │   │   │      └───┘
       └───┴───┴───┴───┘
*/
    /*  Row:    0        1        2        3        4       */
    [_FN1] = LAYOUT(
                _______, _______, _______, _______, _______,
                _______, _______, _______, _______, TO(_FN2),
                _______, _______, _______, _______, _______,
                _______, _______, _______, _______
            ),

/*
       ┌───┬───┬───┬───┐   ┌───┐ ┌───┐
       │Spi│Spd│   │   │   │   │ │TO0│
       ├───┼───┼───┼───┤   └───┘ └───┘
       │Sai│Sad│   │   │
       ├───┼───┼───┼───┤
       │Tog│Mod│Hui│   │      ┌───┐
       ├───┼───┼───┼───┤      │   │
       │   │Vai│Hud│Vad│      └───┘
       └───┴───┴───┴───┘
*/
    /*  Row:    0        1        2        3        4        */
    [_FN2] = LAYOUT(
                RGB_SPI, RGB_SPD, _______, QK_BOOT, _______,
                RGB_SAI, RGB_SAD, _______, _______, TO(_BASE),
                RGB_TOG, RGB_MOD, RGB_HUI, _______, _______,
                _______, RGB_VAI, RGB_HUD, RGB_VAD
            ),
};

#ifdef OLED_ENABLE
// 外部ファイル（lib/logo.c）のロゴ描画関数を呼び出す宣言
extern void render_logo(void);

static uint32_t boot_logo_timer = 0;
static bool show_boot_logo = false;

// キーボード初期化完了時（2回目の接続完了直後）に呼ばれる
void keyboard_post_init_user(void) {
    boot_logo_timer = timer_read32();
    show_boot_logo = true;
}

bool oled_task_user(void) {
    // 初期化完了後、指定ミリ秒（例: 3000ms = 3秒間）ロゴを表示
    if (show_boot_logo) {
        if (timer_elapsed32(boot_logo_timer) < 3000) {
            render_logo();
            return false;
        } else {
            show_boot_logo = false;
        }
    }

    // ロゴ表示終了後は通常のレイヤー表示
    render_layer_status();
    return false;
}
#endif

#ifdef ENCODER_MAP_ENABLE
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][2] = {
    [_BASE] = { ENCODER_CCW_CW(KC_MPRV, KC_MNXT), ENCODER_CCW_CW(KC_PGDN, KC_PGUP), ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [_FN]   = { ENCODER_CCW_CW(KC_TRNS, KC_TRNS), ENCODER_CCW_CW(KC_TRNS, KC_TRNS), ENCODER_CCW_CW(KC_TRNS, KC_TRNS) },
    [_FN1]  = { ENCODER_CCW_CW(KC_TRNS, KC_TRNS), ENCODER_CCW_CW(KC_TRNS, KC_TRNS), ENCODER_CCW_CW(KC_TRNS, KC_TRNS) },
    [_FN2]  = { ENCODER_CCW_CW(KC_TRNS, KC_TRNS), ENCODER_CCW_CW(KC_TRNS, KC_TRNS), ENCODER_CCW_CW(KC_TRNS, KC_TRNS) },
};
#endif


#ifdef OLED_ENABLE
// 待機時間（ミリ秒）: 例として1分（60000ms）
#define OLED_DIM_TIMEOUT 60000

// 通常時と減光時の明るさ（0〜255）
#define OLED_BRIGHTNESS_ACTIVE 120
#define OLED_BRIGHTNESS_DIM    10

static bool is_oled_dimmed = false;
static uint32_t oled_activity_timer = 0;

// レイヤー変更キー（MO, TO, TG, DFなど何でも）が発動した瞬間に呼ばれる
layer_state_t layer_state_set_user(layer_state_t state) {
    // 最後に操作した時刻を現在時刻に更新
    oled_activity_timer = timer_read32();
    
    // 減光中であれば即座に通常輝度に戻す
    if (is_oled_dimmed) {
        oled_set_brightness(OLED_BRIGHTNESS_ACTIVE);
        is_oled_dimmed = false;
    }
    return state;
}

// 毎フレーム呼ばれる監視タスク
void housekeeping_task_user(void) {
    // 通常のキー打鍵、またはレイヤー変更からの経過時間を判定
    uint32_t last_input = last_input_activity_elapsed();
    uint32_t last_layer = timer_elapsed32(oled_activity_timer);
    
    // どちらか直近の操作からの経過時間を採用
    uint32_t elapsed = (last_input < last_layer) ? last_input : last_layer;

    if (elapsed > OLED_DIM_TIMEOUT) {
        if (!is_oled_dimmed) {
            oled_set_brightness(OLED_BRIGHTNESS_DIM);
            is_oled_dimmed = true;
        }
    } else {
        if (is_oled_dimmed) {
            oled_set_brightness(OLED_BRIGHTNESS_ACTIVE);
            is_oled_dimmed = false;
        }
    }
}
#endif

