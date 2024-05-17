// ---------------------------------------------------------------------------------
// Soft - Real-Time CPU Renderer
// ---------------------------------------------------------------------------------
// Author: https://github.com/itsYakub
// ---------------------------------------------------------------------------------
// Version history:
// - Version 1.0 (Current):
//      > Release date: 
// ---------------------------------------------------------------------------------
// Macro Definitions:
// - SOFT_DISABLE_VERBOSITY - Disables logging.
//      Add this macro if you want to get rid of the info / warning / error logging.
// ---------------------------------------------------------------------------------
// Sections:
// - SOFT_INCLUDES;
// - SOFT_MACROS;
// - SOFT_TYPEDEFS;
// - SOFT_TYPEDEFS_ENUMS;
// - SOFT_FUNC_WINDOW;
// - SOFT_FUNC_EVENTS;
// - SOFT_FUNC_INPUT;
// - SOFT_FUNC_RENDER;
// - SOFT_FUNC_SHAPES;
// - SOFT_FUNC_LOGGING;
// - SOFT_FUNC_TEXT;
// - SOFT_MACROS_COLOR
// - SOFT_FUNC_COLOR
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

typedef uint32_t Pixel;
typedef struct { uint8_t r; uint8_t g; uint8_t b; uint8_t a; } Color;
typedef struct { float r; float g; float b; float a; } fColor;
typedef struct { int x; int y; } iVec2;
typedef struct { iVec2 position; iVec2 size; } Rect;
typedef struct { iVec2 position; int r; } Circle;
typedef struct { iVec2 a; iVec2 b; } Line;

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

    KEY_DEL             // Key: DEL
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

// ------------------------------------------------------
#pragma endregion
// ------------------------------------------------------

// ------------------------------------------------------
#pragma region SOFT_FUNC_WINDOW
// ------------------------------------------------------

SAPI int softInit(int width, int height, const char* title);
SAPI int softInitPlatform();
SAPI int softInitWindow(int width, int height, const char* title);
SAPI int softInitRenderer();
SAPI int softInitRenderTexture(int width, int height);
SAPI int softInitPixelData();

SAPI void softClose();
SAPI void softClosePlatform();
SAPI void softCloseWindow();
SAPI void softCloseRenderer();
SAPI void softUnloadRenderTexture();
SAPI void softUnloadPixelData();

SAPI void softSetConfigFlags(softConfigFlags flags);
SAPI void softSetWindowTitle(const char* title);
SAPI bool softWindowShoulClose();
SAPI void softCloseCallback();

SAPI iVec2 softGetWindowSize();
SAPI iVec2 softGetWindowCenter();
SAPI iVec2 softGetDisplaySize();
SAPI iVec2 softGetDisplayCenter();

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

SAPI iVec2 softGetMousePosition();
SAPI iVec2 softGetPreviousMousePosition();
SAPI iVec2 softGetMouseDelta();
SAPI iVec2 softGetMouseWheel();

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

SAPI void softClearBuffer();
SAPI void softClearBufferColor(Color color);
SAPI void softBlit();

// ------------------------------------------------------
#pragma endregion
// ------------------------------------------------------

// ------------------------------------------------------
#pragma region SOFT_FUNC_SHAPES
// ------------------------------------------------------

SAPI void softDrawRectangle(Rect rect, Color color);
SAPI void softDrawRectangleLines(Rect rect, Color color);
SAPI void softDrawLine(Line line, Color color);
SAPI void softDrawCircle(Circle circle, Color color);
SAPI void softDrawCircleLines(Circle circle, Color color);

// ------------------------------------------------------
#pragma endregion
// ------------------------------------------------------

// ------------------------------------------------------
#pragma region SOFT_FUNC_LOGGING
// ------------------------------------------------------

SAPI void softLog(SoftLogLevel log_level, const char *restrict txt, ...);
SAPI void softLogInfo(const char *restrict txt, ...);
SAPI void softLogWarning(const char *restrict txt, ...);
SAPI void softLogError(const char *restrict txt, ...);

// ------------------------------------------------------
#pragma endregion
// ------------------------------------------------------

// ------------------------------------------------------
#pragma region SOFT_FUNC_TEXT
// ------------------------------------------------------

SAPI const char* softTextFormat(const char *restrict txt, ...);
SAPI const bool softTextEmpty(const char *restrict txt, ...);
SAPI const int softTextLength(const char *restrict txt);

// ------------------------------------------------------
#pragma endregion
// ------------------------------------------------------

// ------------------------------------------------------
#pragma region SOFT_MACROS_COLOR
// ------------------------------------------------------

#define RED (Color) { 255, 0, 0, 255 }
#define GREEN (Color) { 0, 255, 0, 255 }
#define BLUE (Color) { 0, 0, 255, 255 }
#define YELLOW (Color) { 255, 255, 0, 255 }
#define PINK (Color) { 255, 0, 255, 255 }
#define WHITE (Color) { 255, 255, 255, 255 }
#define BLACK (Color) { 0, 0, 0, 255 }

// ------------------------------------------------------
#pragma endregion
// ------------------------------------------------------

// ------------------------------------------------------
#pragma region SOFT_FUNC_COLOR
// ------------------------------------------------------

SAPI Pixel softColorToPixel(Color color);
SAPI Pixel softColorFToPixel(fColor color);
SAPI Color softPixelToColor(Pixel pixel);
SAPI fColor softPixelToColorF(Pixel pixel);

SAPI bool softColorCompare(Color a, Color b);
SAPI bool softPixelColorCompare(Pixel a, Pixel b);

// ------------------------------------------------------
#pragma endregion
// ------------------------------------------------------

#endif // SOFT_H