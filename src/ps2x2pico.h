#pragma once
/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2024 No0ne (https://github.com/No0ne)
 *           (c) 2023 Dustin Hoffman
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */
#ifndef _PS2X2PICO_H_
#define _PS2X2PICO_H_


#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "tusb.h"
#include "hardware/pio.h"
#include "pico/util/queue.h"
#include "ws2812.h"

typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

#define KBD_BUFFER_SIZE 16

enum MT8816_Y
{
	D0 = 4 << 4,
	D1 = 3 << 4,
	D2 = 2 << 4,
	D3 = 1 << 4,
	D4 = 0 << 4
};

enum MT8816_X
{
	KA3 = 0,
	KA2 = 1,
	KA1 = 2,
	KA0 = 3,
	KA4 = 4,
	KA5 = 5,
	KA6 = 6,
	KA7 = 7
};

//----------------------------------------------------------
// Keytable code low bit AX0 AX1 AX2 AX3 AY0 AY1 AY2 high bit
//         
//   
// pent    Y4   Y3   Y2   Y0   Y1   Y1   Y0   Y2   Y3   Y4
//         KD0  KD1  KD2  KD3  KD4  KD4  KD3  KD2  KD1  KD0
// X0 KA3   1    2    3    4    5    6    7    8    9    0   KA4 X4
// X1 KA2   Q    W    E    R    T    Y    U    I    O    P   KA5 X5
// X2 KA1   A    S    D    F    G    H    J    K    L   EN   KA6 X6
// X3 KA0  CS    Z    X    C    V    B    N    M   SS   SP   KA7 X7

// A11 - KA3 - X0
// A10 - KA2 - X1
// A9  - KA1 - X2
// A8  - KA0 - X3
// A12 - KA4 - X4
// A13 - KA5 - X5
// A14 - KA6 - X6
// A15 - KA7 - X7

// KD0 - Y4
// KD1 - Y3
// KD2 - Y2
// KD4 - Y1
// KD3 - Y0

extern const enum zx_key{  //Pentagon 128/2024 map
    NC=0xFF,
    //      ряд X7
    SP= D0 | KA7, //SPACE
    SS= D1 | KA7, //SYMBOL SHIFT
    _M= D2 | KA7,
    _N= D4 | KA7,
    _B= D3 | KA7,
    //      ряд X6
    EN= D0 | KA6, // ENTER
    _L= D1 | KA6,
    _K= D2 | KA6,
    _J= D4 | KA6,
    _H= D3 | KA6,
    //      ряд X5
    _P= D0 | KA5,
    _O= D1 | KA5,
    _I= D2 | KA5,
    _U= D4 | KA5,
    _Y= D3 | KA5,
    //      ряд X4
    _0= D0 | KA4,
    _9= D1 | KA4,
    _8= D2 | KA4,
    _7= D4 | KA4,
    _6= D3 | KA4,
    //      ряд X3    
    CS= D0 | KA0, //CAPS SHIFT
    _Z= D1 | KA0,
    _X= D2 | KA0,
    _C= D4 | KA0,
    _V= D3 | KA0,
    //      ряд X2
    _A= D0 | KA1,
    _S= D1 | KA1,
    _D= D2 | KA1,
    _F= D4 | KA1,
    _G= D3 | KA1,   
    //      ряд X1
    _Q= D0 | KA2,
    _W= D1 | KA2,
    _E= D2 | KA2,
    _R= D4 | KA2,
    _T= D3 | KA2,
    //      ряд X0
    _1= D0 | KA3,
    _2= D1 | KA3,
    _3= D2 | KA3,
    _4= D4 | KA3,
    _5= D3 | KA3
} zx_key_pent;


void scancode_s(uint8_t code);
void key_on(uint8_t code); // клавиша нажата
void key_off(uint8_t code);// клавиша отпущена  
void SetAddr(uint8_t addr) ;
void kb_set_leds(uint8_t d);
void kb_res_leds(uint8_t d);


void kb_init(u8 gpio_in);
void kb_send_sc_list(const u8 *list);
void kb_send_key(u8 key, bool is_key_pressed, u8 modifiers);
void tuh_kb_set_leds(u8 leds);
bool kb_task();
void kb_set_leds(u8 byte);
void kb_res_leds(u8 byte);

void ms_init(u8 gpio_in);
void ms_send_movement(u8 buttons, s8 x, s8 y, s8 z);
bool ms_task();


u32 ps2_frame(u8 byte);
typedef void (*rx_callback)(u8 byte, u8 prev_byte);

typedef struct {
  PIO pio;
  uint sm;
  queue_t qbytes;
  queue_t qpacks;
  rx_callback rx;
  u8 last_rx;
  u8 last_tx;
  u8 sent;
  u8 busy;
} ps2out;

void keyboard( uint8_t const *report, uint16_t len); //uint8_t modifiers,
void keyboard_task(ps2out* this);
void ps2out_init(ps2out* this, rx_callback rx);
void ps2out_task(ps2out* this);


typedef struct {
  PIO pio;
  uint sm;
  u8 state;
  u8 last_tx;
  u8 byte;
} ps2in;

void ps2in_init(ps2in* this, PIO pio, u8 data_pin);
void ps2in_task(ps2in* this, ps2out* out);
void ps2in_reset(ps2in* this);
void ps2in_set(ps2in* this, u8 command, u8 byte);

#define PS2_LED_SCROLL_LOCK 1
#define PS2_LED_NUM_LOCK    2
#define PS2_LED_CAPS_LOCK   4

#define KB_EXT_PFX_E0 0xe0 // This is the extended code prefix used in sets 1 and 2
#define KB_BREAK_2_3 0xf0 // The prefix 0xf0 is the break code prefex in sets 2 and 3 (is send when key is released)
#define HID2PS2_IDX_MAX 0x73
#define IS_VALID_KEY(key) (key <= HID2PS2_IDX_MAX || (key >= HID_KEY_CONTROL_LEFT && key <= HID_KEY_GUI_RIGHT))
#define IS_MOD_KEY(key) (key >= HID_KEY_CONTROL_LEFT && key <= HID_KEY_GUI_RIGHT)

#define LEDBR 12

#define KEYBOARD_REPORT_SIZE    20



enum
{
  ITF_NUM_KEYBOARD,
  ITF_NUM_HID,
  ITF_NUM_CDC,
  ITF_NUM_CDC_DATA, // CDC needs 2 interfaces
  ITF_NUM_TOTAL
};


// 20-byte NKRO: support keycodes upto LANG8(0x97)
#define KEYBOARD_REPORT_KEYS    (KEYBOARD_REPORT_SIZE - 2)
#define KEYBOARD_REPORT_BITS    (KEYBOARD_REPORT_SIZE - 1)

// макросы
#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) : bitClear(value, bit))
#define bit(b) (1UL << (b))
// макросы end


extern uint8_t const macro1[]; 
extern uint8_t const macro2[]; 
extern uint8_t const macro3[]; 
extern uint8_t const macro4[]; 
extern uint8_t const macro5[];
extern uint8_t const macro6[]; 
extern uint8_t const macro7[];
extern uint8_t const macro8[];
extern uint8_t const macro9[];
extern uint8_t const macro10[];
extern uint8_t const macro11[];
extern uint8_t const macro12[];


extern uint8_t const ext_code_keys_1_2[]; // keys in this list need to have KB_EXT_PFX_E0 sent before their actual code
extern uint8_t const ext_code_modifier_keys_1_2[]; // keys in this list need to have KB_EXT_PFX_E0 sent before their actual code
extern uint8_t const mod2ps2_1[];
extern uint8_t const mod2ps2_2[];
extern uint8_t const mod2ps2_3[];
extern uint8_t const hid2ps2_1[];
extern uint8_t const hid2ps2_2[];
extern uint8_t const hid2ps2_3[];
extern uint8_t const prt_scn_make_1[];
extern uint8_t const prt_scn_break_1[];
extern uint8_t const break_make_1[];
extern uint8_t const pause_make_1[];
extern uint8_t const prt_scn_make_2[];
extern uint8_t const prt_scn_break_2[];
extern uint8_t const break_make_2[];
extern uint8_t const pause_make_2[];
extern uint8_t const table_key_zx_hid[];
extern uint8_t const table_key_zx_ps[][4];

#endif