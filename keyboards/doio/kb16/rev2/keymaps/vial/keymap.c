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

#include QMK_KEYBOARD_H

// レイヤー状態が切り替わるたびに自動で呼ばれるコールバック
layer_state_t layer_state_set_user(layer_state_t state) {
    // 現在のOS側のLED / ロック状態を取得
    led_t led_state = host_keyboard_led_state();

    // NumLock が OFF（0）になっている場合のみ、キーを押して ON にする
    if (!led_state.num_lock) {
        tap_code(KC_NUM_LOCK);
    }

    return state;
}

#include QMK_KEYBOARD_H

// ポケベル打ち用レイヤーの番号（環境に合わせて変更してね）
#define BELL_LAYER 2

// 1打目の数字を保存する変数（0: 待機中、1〜10: 入力済み）
static uint8_t first_stroke = 0;

// ポケベルの2桁数字からローマ字を送信する関数
void send_bell_code(uint8_t row, uint8_t col) {
    switch (row) {
        case 1: // あ行
            switch (col) {
                case 1: SEND_STRING("a"); break;
                case 2: SEND_STRING("i"); break;
                case 3: SEND_STRING("u"); break;
                case 4: SEND_STRING("e"); break;
                case 5: SEND_STRING("o"); break;
            }
            break;
        case 2: // か行
            switch (col) {
                case 1: SEND_STRING("ka"); break;
                case 2: SEND_STRING("ki"); break;
                case 3: SEND_STRING("ku"); break;
                case 4: SEND_STRING("ke"); break;
                case 5: SEND_STRING("ko"); break;
            }
            break;
        case 3: // さ行
            switch (col) {
                case 1: SEND_STRING("sa"); break;
                case 2: SEND_STRING("si"); break; // または shi
                case 3: SEND_STRING("su"); break;
                case 4: SEND_STRING("se"); break;
                case 5: SEND_STRING("so"); break;
            }
            break;
        case 4: // た行
            switch (col) {
                case 1: SEND_STRING("ta"); break;
                case 2: SEND_STRING("ti"); break; // または chi
                case 3: SEND_STRING("tu"); break; // または tsu
                case 4: SEND_STRING("te"); break;
                case 5: SEND_STRING("to"); break;
            }
            break;
        case 5: // な行
            switch (col) {
                case 1: SEND_STRING("na"); break;
                case 2: SEND_STRING("ni"); break;
                case 3: SEND_STRING("nu"); break;
                case 4: SEND_STRING("ne"); break;
                case 5: SEND_STRING("no"); break;
            }
            break;
        case 6: // は行
            switch (col) {
                case 1: SEND_STRING("ha"); break;
                case 2: SEND_STRING("hi"); break;
                case 3: SEND_STRING("hu"); break; // または fu
                case 4: SEND_STRING("he"); break;
                case 5: SEND_STRING("ho"); break;
            }
            break;
        case 7: // ま行
            switch (col) {
                case 1: SEND_STRING("ma"); break;
                case 2: SEND_STRING("mi"); break;
                case 3: SEND_STRING("mu"); break;
                case 4: SEND_STRING("me"); break;
                case 5: SEND_STRING("mo"); break;
            }
            break;
        case 8: // や行
            switch (col) {
                case 1: SEND_STRING("ya"); break;
                case 2: SEND_STRING("(");  break; // 記号カッコ等
                case 3: SEND_STRING("yu"); break;
                case 4: SEND_STRING(")");  break;
                case 5: SEND_STRING("yo"); break;
            }
            break;
        case 9: // ら行
            switch (col) {
                case 1: SEND_STRING("ra"); break;
                case 2: SEND_STRING("ri"); break;
                case 3: SEND_STRING("ru"); break;
                case 4: SEND_STRING("re"); break;
                case 5: SEND_STRING("ro"); break;
            }
            break;
        case 0: // わ行・濁点・長音
            switch (col) {
                case 1: SEND_STRING("wa"); break;
                case 2: SEND_STRING("wo"); break;
                case 3: SEND_STRING("-");  break; // 長音（ー）
                case 4: SEND_STRING("@");  break; // 濁点（日本語キーボードなら「゛」）
                case 5: SEND_STRING("[");  break; // 半濁点（「゜」）
            }
            break;
        default:
            break;
    }
}

// キー入力ごとの割り込み処理
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    // ポケベル打ちレイヤーの時だけ処理を横取りする
    if (get_highest_layer(layer_state) == BELL_LAYER) {
        if (record->event.pressed) {
            uint8_t num = 255;

            // テンキーまたは通常数字キーコードを判定
            switch (keycode) {
                case KC_P1: case KC_1: num = 1; break;
                case KC_P2: case KC_2: num = 2; break;
                case KC_P3: case KC_3: num = 3; break;
                case KC_P4: case KC_4: num = 4; break;
                case KC_P5: case KC_5: num = 5; break;
                case KC_P6: case KC_6: num = 6; break;
                case KC_P7: case KC_7: num = 7; break;
                case KC_P8: case KC_8: num = 8; break;
                case KC_P9: case KC_9: num = 9; break;
                case KC_P0: case KC_0: num = 0; break;
                default:
                    // Backspace 等が押されたら1打目をリセット
                    first_stroke = 0;
                    return true; // 通常のキー動作をそのまま流す
            }

            if (num != 255) {
                if (first_stroke == 0) {
                    // 1打目：番号を記録してPC側へはキー送信しない
                    first_stroke = (num == 0) ? 10 : num; // 0は内部値10として保持
                    return false;
                } else {
                    // 2打目：1打目と組み合わせてローマ字送信
                    uint8_t row = (first_stroke == 10) ? 0 : first_stroke;
                    send_bell_code(row, num);
                    first_stroke = 0; // リセット
                    return false;     // 数字そのものは送らない
                }
            }
        }
    } else {
        // 別レイヤーに移動したら状態リセット
        first_stroke = 0;
    }

    return true; // ポケベルレイヤー以外は通常動作
}