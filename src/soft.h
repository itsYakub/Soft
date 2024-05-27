// ---------------------------------------------------------------------------------
// Soft - Real-Time CPU Renderer
// ---------------------------------------------------------------------------------
// Author: https://github.com/itsYakub
// ---------------------------------------------------------------------------------
// Version history:
// - Version 1.0 (Current):
//      > Release date: 
// ---------------------------------------------------------------------------------
// Sections:
// - SOFT_INCLUDES;
// - SOFT_MACROS;
// - SOFT_TYPEDEFS;
// - SOFT_TYPEDEFS_ENUMS;
// - SOFT_FUNC_CONFIG;
// - SOFT_FUNC_WINDOW;
// - SOFT_FUNC_EVENTS;
// - SOFT_FUNC_INPUT;
// - SOFT_FUNC_RENDER;
// - SOFT_FUNC_DRAW;
// - SOFT_FUNC_LOGGING;
// - SOFT_FUNC_TEXT;
// - SOFT_MACROS_COLOR;
// - SOFT_FUNC_COLOR;
// - SOFT_FUNC_TIME;
// - SOFT_MACROS_MATH;
// - SOFT_FUNC_MATH;
// - SOFT_FUNC_IMAGE;
// ---------------------------------------------------------------------------------
// External Dependencies:
// - SDL2: https://github.com/libsdl-org/SDL.git
// ---------------------------------------------------------------------------------
// LICENCE:
// Copyright (c) 2024 Jakub Oleksiak <yakubofficialmail@gmail.com>
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
// DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
// OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE
// OR OTHER DEALINGS IN THE SOFTWARE.
// ---------------------------------------------------------------------------------

#ifndef SOFT_H
#define SOFT_H

// ------------------------------------------------------
#include <math.h>
#pragma region SOFT_INCLUDES
// ------------------------------------------------------

// ------------------------------
// Include header conventions:
// 1. Parent header.
//      (#include "rastr.h")
// 2. Standard library headers.
//      (i.e. #include <stdio.h>)
// 3. Dependency headers.
//      (i.e. #include "SDL.h")
// 4. Internal headers.
// ------------------------------

// Standard library headers
#include <stdint.h>
#include <stdbool.h>

// ------------------------------------------------------
#pragma endregion
// ------------------------------------------------------

// ------------------------------------------------------
#pragma region SOFT_MACROS
// ------------------------------------------------------

#ifndef SAPI
    #define SAPI // SAPI - Soft API
#endif

#define SOFT_VERSION "1.0"
#define SOFT_VERSION_MAJOR 1
#define SOFT_VERSION_MINOR 0

#define SOFT_SUCCESS 0
#define SOFT_FAILED 1

// ------------------------------------------------------
#pragma endregion
// ------------------------------------------------------

// ------------------------------------------------------
#pragma region SOFT_TYPEDEFS
// ------------------------------------------------------

typedef uint8_t                  u8;
typedef uint16_t                 u16;
typedef uint32_t                 u32;
typedef int8_t                   i8;
typedef int16_t                  i16;
typedef int                      i32;
typedef float                    f32;
typedef double                   d32;
typedef char*                    string;
typedef u32                      Pixel;
typedef Pixel*                   PixelBuffer;
typedef struct { i32 x; i32 y; } iVec2;

typedef struct { u8 r; u8 g; u8 b; u8 a; }                                  Color;
typedef struct { iVec2 position; iVec2 size; }                              Rect;
typedef struct { iVec2 position; i32 r; }                                   Circle;
typedef struct { iVec2 a; iVec2 b; }                                        Line;
typedef struct { f32 initial_time; f32 current_time; bool finished; }       Timer;
typedef struct { PixelBuffer data; iVec2 size; i32 channels; }                      Image;

// ------------------------------------------------------
#pragma endregion
// ------------------------------------------------------

// ------------------------------------------------------
#pragma region SOFT_TYPEDEFS_ENUMS
// ------------------------------------------------------

typedef enum {
    FLAG_WINDOW_RESIZABLE =     1 << 0,
    FLAG_WINDOW_FULLSCREEN =    1 << 1,
    FLAG_WINDOW_MAXIMIZED =     1 << 2,
    FLAG_WINDOW_MINIMIZED =     1 << 3,
    FLAG_WINDOW_HIGHDPI =       1 << 4,
    FLAG_WINDOW_VSYNC =         1 << 5
} softConfigFlags;

typedef enum {
    // Section: ASCII TABLE

    KEY_NULL = 0,

    KEY_BACKSPACE = 8,
    KEY_TAB = 9,
    KEY_ESCAPE = 27,

    KEY_SPACE = 32,     // Key: ' '
    KEY_EXCL,           // Key: !
    KEY_QUOT,           // Key: "
    KEY_NUM,            // Key: #
    KEY_DOLLAR,         // Key: $
    KEY_PERCENT,        // Key: %
    KEY_AMP,            // Key: &
    KEY_APOS,           // Key: '
    KEY_LPAREN,         // Key: (
    KEY_RPAREN,         // Key: )
    KEY_AST,            // Key: *
    KEY_PLUS,           // Key: +
    KEY_COMMA,          // Key: ,
    KEY_MINUS,          // Key: -
    KEY_PERIOD,         // Key: .
    KEY_SLASH,          // Key: /
    
    KEY_ZERO,           // Key: 0
    KEY_ONE,            // Key: 1
    KEY_TWO,            // Key: 2
    KEY_THREE,          // Key: 3
    KEY_FOUR,           // Key: 4
    KEY_FIVE,           // Key: 5
    KEY_SIX,            // Key: 6
    KEY_SEVEN,          // Key: 7
    KEY_EIGHT,          // Key: 8
    KEY_NINE,           // Key: 9

    KEY_COLON,          // Key: :
    KEY_SEMI,           // Key: ;
    KEY_LT,             // Key: <
    KEY_EQUALS,         // Key: =
    KEY_GT,             // Key: >
    KEY_QUEST,          // Key: ? 
    KEY_AT,             // Key: @

    KEY_A,              // Key: A
    KEY_B,              // Key: B
    KEY_C,              // Key: C
    KEY_D,              // Key: D
    KEY_E,              // Key: E
    KEY_F,              // Key: F
    KEY_G,              // Key: G
    KEY_H,              // Key: H
    KEY_I,              // Key: I
    KEY_J,              // Key: J
    KEY_K,              // Key: K
    KEY_L,              // Key: L
    KEY_M,              // Key: M
    KEY_N,              // Key: N
    KEY_O,              // Key: O
    KEY_P,              // Key: P
    KEY_Q,              // Key: Q
    KEY_R,              // Key: R
    KEY_S,              // Key: S
    KEY_T,              // Key: T
    KEY_U,              // Key: U
    KEY_V,              // Key: V
    KEY_W,              // Key: W
    KEY_X,              // Key: X
    KEY_Y,              // Key: Y
    KEY_Z,              // Key: Z

    KEY_LSQB,           // Key: [ 
    KEY_BSLASH,         // Key: '\'
    KEY_RSQB,           // Key:  ]
    KEY_HAT,            // Key: ^
    KEY_LOWBAR,         // Key: _
    KEY_GRAVE,          // Key: `

    KEY_A_LOWER,        // Key: a
    KEY_B_LOWER,        // Key: b
    KEY_C_LOWER,        // Key: c
    KEY_D_LOWER,        // Key: d
    KEY_E_LOWER,        // Key: e
    KEY_F_LOWER,        // Key: f
    KEY_G_LOWER,        // Key: g
    KEY_H_LOWER,        // Key: h
    KEY_I_LOWER,        // Key: i
    KEY_J_LOWER,        // Key: j
    KEY_K_LOWER,        // Key: k
    KEY_L_LOWER,        // Key: l
    KEY_M_LOWER,        // Key: m
    KEY_N_LOWER,        // Key: n
    KEY_O_LOWER,        // Key: o
    KEY_P_LOWER,        // Key: p
    KEY_Q_LOWER,        // Key: q
    KEY_R_LOWER,        // Key: r
    KEY_S_LOWER,        // Key: s
    KEY_T_LOWER,        // Key: t
    KEY_U_LOWER,        // Key: u
    KEY_V_LOWER,        // Key: v
    KEY_W_LOWER,        // Key: w
    KEY_X_LOWER,        // Key: x
    KEY_Y_LOWER,        // Key: y
    KEY_Z_LOWER,        // Key: z

    KEY_LCUB,           // Key: {
    KEY_BAR,            // Key: |
    KEY_RCUB,           // Key: }
    KEY_TILDE,          // Key: ~

    KEY_DEL,            // Key: DEL

    // Section: EXTENDED LIST

    KEY_LEFT,
    KEY_RIGHT,
    KEY_UP,
    KEY_DOWN,
    KEY_CAPS,
    KEY_LSHIFT,
    KEY_RSHIFT,
    KEY_LALT,
    KEY_RALT,
    KEY_LCTRL,
    KEY_RCTRL,
    KEY_ENTER,
    KEY_HOME,
    KEY_NUMLOCK,

    KEY_F1,
    KEY_F2,
    KEY_F3,
    KEY_F4,
    KEY_F5,
    KEY_F6,
    KEY_F7,
    KEY_F8,
    KEY_F9,
    KEY_F10,
    KEY_F11,
    KEY_F12,

} softKeyCode;

typedef enum {
    BUTTON_LEFT = 0,
    BUTTON_RIGHT,
    BUTTON_MIDDLE
} softMouseButtons;

typedef enum {
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR
} SoftLogLevel;

typedef enum {
    FLIP_DEFAULT = 0,
    FLIP_H,
    FLIP_V,
    FLIP_HV
} SoftImageFlip;

// ------------------------------------------------------
#pragma endregion
// ------------------------------------------------------

// ------------------------------------------------------
#pragma region SOFT_FUNC_CONFIG
// ------------------------------------------------------

SAPI void softAlphaBlendState(bool state);

// ------------------------------------------------------
#pragma endregion
// ------------------------------------------------------

// ------------------------------------------------------
#pragma region SOFT_FUNC_WINDOW
// ------------------------------------------------------

SAPI i32 softInit(i32 width, i32 height, const string title);
SAPI i32 softInitPlatform(void);
SAPI i32 softInitWindow(i32 width, i32 height, const string title);
SAPI i32 softInitRenderer(void);

SAPI void softClose(void);
SAPI void softClosePlatform(void);
SAPI void softCloseWindow(void);
SAPI void softCloseRenderer(void);

SAPI i32 softInitDefaultPixelBuffer(void);
SAPI void softUnloadPixelBuffer(void);

SAPI PixelBuffer softCreatePixelBuffer(i32 width, i32 height);
SAPI i32 softSetCurrentPixelBuffer(PixelBuffer pixel_buffer);

SAPI bool softWindowShoulClose(void);
SAPI void softCloseCallback(void);

SAPI iVec2 softGetWindowSize(void);
SAPI iVec2 softGetWindowCenter(void);
SAPI iVec2 softGetDisplaySize(void);
SAPI iVec2 softGetDisplayCenter(void);

SAPI void softSetConfigFlags(softConfigFlags flags);
SAPI void softSetWindowTitle(const string title);

// ------------------------------------------------------
#pragma endregion
// ------------------------------------------------------

// ------------------------------------------------------
#pragma region SOFT_FUNC_EVENTS
// ------------------------------------------------------

SAPI void softPollEvents();

// ------------------------------------------------------
#pragma endregion
// ------------------------------------------------------

// ------------------------------------------------------
#pragma region SOFT_FUNC_INPUT
// ------------------------------------------------------

SAPI iVec2 softGetMousePosition(void);
SAPI iVec2 softGetPreviousMousePosition(void);
SAPI iVec2 softGetMouseDelta(void);
SAPI iVec2 softGetMouseWheel(void);

SAPI bool softKeyPressed(softKeyCode key);
SAPI bool softKeyReleased(softKeyCode key);
SAPI bool softKeyDown(softKeyCode key);
SAPI bool softKeyUp(softKeyCode key);

SAPI bool softMouseButtonPressed(softMouseButtons button);
SAPI bool softMouseButtonReleased(softMouseButtons button);
SAPI bool softMouseButtonDown(softMouseButtons button);
SAPI bool softMouseButtonUp(softMouseButtons button);

// ------------------------------------------------------
#pragma endregion
// ------------------------------------------------------

// ------------------------------------------------------
#pragma region SOFT_FUNC_RENDER
// ------------------------------------------------------

SAPI void softClearBuffer(void);
SAPI void softClearBufferColor(Pixel pixel);
SAPI void softBlit(void);

// ------------------------------------------------------
#pragma endregion
// ------------------------------------------------------

// ------------------------------------------------------
#pragma region SOFT_FUNC_DRAW
// ------------------------------------------------------

SAPI void softDrawRectangle(Rect rect, Pixel pixel);
SAPI void softDrawRectangleLines(Rect rect, Pixel pixel);
SAPI void softDrawRectangleEx(Rect rect, iVec2 pivot, Pixel pixel);

SAPI void softDrawLine(Line line, Pixel pixel);
SAPI void softDrawLineBezier(iVec2 start, iVec2 end, iVec2 midpoint, i32 resolution, Pixel pixel);

SAPI void softDrawCircle(Circle circle, Pixel pixel);
SAPI void softDrawCircleLines(Circle circle, Pixel pixel);

SAPI void softDrawImage(Image* image, iVec2 position, Pixel tint);
SAPI void softDrawImageEx(Image* image, iVec2 position, iVec2 pivot, SoftImageFlip image_flip, Pixel tint);

// ------------------------------------------------------
#pragma endregion
// ------------------------------------------------------

// ------------------------------------------------------
#pragma region SOFT_FUNC_LOGGING
// ------------------------------------------------------

SAPI void softLog(SoftLogLevel log_level, const string restrict txt, ...);
SAPI void softLogInfo(const string restrict txt, ...);
SAPI void softLogWarning(const string restrict txt, ...);
SAPI void softLogError(const string restrict txt, ...);

// ------------------------------------------------------
#pragma endregion
// ------------------------------------------------------

// ------------------------------------------------------
#pragma region SOFT_FUNC_TEXT
// ------------------------------------------------------

SAPI const string softTextFormat(const string restrict txt, ...);
SAPI const bool softTextEmpty(const string restrict txt, ...);
SAPI const i32 softTextLength(const string restrict txt);

// ------------------------------------------------------
#pragma endregion
// ------------------------------------------------------

// ------------------------------------------------------
#pragma region SOFT_MACROS_COLOR
// ------------------------------------------------------

#define RED 0xFF0000FF
#define GREEN 0xFF00FF00
#define BLUE 0xFFFF0000
#define YELLOW 0xFF00FFFF
#define PINK 0xFFFF00FF
#define WHITE 0xFFFFFFFF
#define BLACK 0xFF000000
#define BLANK 0x00000000

// ------------------------------------------------------
#pragma endregion
// ------------------------------------------------------

// ------------------------------------------------------
#pragma region SOFT_FUNC_COLOR
// ------------------------------------------------------

SAPI Pixel softGetPixelColor(i32 x, i32 y);
SAPI Pixel softGetPixelFromBuffer(PixelBuffer buffer, iVec2 position, iVec2 size);

SAPI Pixel softColorToPixel(Color color);
SAPI Color softPixelToColor(Pixel pixel);

SAPI Color softMixColor(Color base_color, Color return_color, u8 alpha);
SAPI Pixel softMixPixels(Pixel base_pixel, Pixel return_pixel, u8 alpha);
SAPI Color softColorFade(Color color, f32 factor);
SAPI Pixel softPixelFade(Pixel pixel, f32 factor);

// ------------------------------------------------------
#pragma endregion
// ------------------------------------------------------

// ------------------------------------------------------
#pragma region SOFT_FUNC_TIME
// ------------------------------------------------------

SAPI f32 softDeltaTime(void);
SAPI f32 softTime(void);
SAPI i32 softFPS(void);
SAPI void softTargetFPS(u32 framerate);
SAPI void softWait(f32 seconds);

SAPI Timer softTimer(const f32 TIME);
SAPI void softTimerProceed(Timer* timer, f32 delta_time);
SAPI bool softTimerFinished(Timer* timer);
SAPI void softTimerRestart(Timer* timer);
SAPI void softTimerReset(Timer* timer, f32 time);

// ------------------------------------------------------
#pragma endregion
// ------------------------------------------------------

// ------------------------------------------------------
#pragma region SOFT_MACROS_MATH
// ------------------------------------------------------

#define PI 3.1415926535

// ------------------------------------------------------
#pragma endregion
// ------------------------------------------------------

// ------------------------------------------------------
#pragma region SOFT_FUNC_MATH
// ------------------------------------------------------

SAPI f32 softLerpF(f32 start, f32 end, f32 t);
SAPI f32 softPowF(f32 a, f32 n);
SAPI f32 softSqrF(f32 a);
SAPI f32 softSqrtF(f32 a);

SAPI i32 softLerpI(i32 start, i32 end, f32 t);
SAPI i32 softPowI(i32 a, i32 n);
SAPI i32 softSqrI(i32 a);
SAPI i32 softSqrtI(i32 a);

SAPI iVec2 softVectorZero(void);
SAPI iVec2 softVectorOne(void);
SAPI iVec2 softVectorUp(void);
SAPI iVec2 softVectorDown(void);
SAPI iVec2 softVectorLeft(void);
SAPI iVec2 softRight(void);
SAPI iVec2 softVectorAdd(iVec2 a, iVec2 b);
SAPI iVec2 softVectorSub(iVec2 a, iVec2 b);
SAPI iVec2 softVectorMult(iVec2 a, iVec2 b);
SAPI iVec2 softVectorMultFactor(iVec2 a, f32 factor);
SAPI iVec2 softVectorDiv(iVec2 a, iVec2 b);
SAPI iVec2 softVectorDivFactor(iVec2 a, f32 factor);
SAPI iVec2 softVectorLerp(iVec2 start, iVec2 end, f32 t);
SAPI bool softVectorCompare(iVec2 a, iVec2 b);

SAPI Color softColorZero(void);
SAPI Color softColorOne(void);
SAPI Color softColorAdd(Color a, Color b);
SAPI Color softColorSub(Color a, Color b);
SAPI Color softColorMult(Color a, Color b);
SAPI Color softColorDiv(Color a, Color b);
SAPI Color softColorLerp(Color start, Color end, f32 t);
SAPI bool softColorCompare(Color a, Color b);

SAPI bool softPixelCompare(Pixel a, Pixel b);

// ------------------------------------------------------
#pragma endregion
// ------------------------------------------------------

// ------------------------------------------------------
#pragma region SOFT_FUNC_IMAGE
// ------------------------------------------------------

SAPI Image softLoadImage(const string path);
SAPI void softUnloadImage(Image* image);

// ------------------------------------------------------
#pragma endregion
// ------------------------------------------------------

// ------------------------------------------------------
#pragma region SOFT_FUNC_RESOURCES
// ------------------------------------------------------

SAPI void softLoadResourceImage(Image* image);
SAPI void softUnloadResources(void);

// ------------------------------------------------------
#pragma endregion
// ------------------------------------------------------

#endif // SOFT_H