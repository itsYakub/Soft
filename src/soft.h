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
// - SOFT_IMPLEMENTATION - Single-Header implementation of Soft.
//      Add this macro before your inclusion of "soft.h".
//      WARNING: This macro MUST be used ONLY ONCE in the entire program.
// 
// - SOFT_DISABLE_VERBOSITY - Disables logging.
//      Add this macro if you want to get rid of the info / warning / error logging.
// ---------------------------------------------------------------------------------

#ifndef SOFT_H
#define SOFT_H

// ------------------------------------------------------
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

#define RED 0xFF0000FF
#define GREEN 0xFF00FF00
#define BLUE 0xFFFF0000
#define YELLOW 0xFF00FFFF
#define PINK 0xFFFF00FF
#define WHITE 0xFFFFFFFF
#define BLACK 0xFF000000

// ------------------------------------------------------
#pragma endregion
// ------------------------------------------------------

// ------------------------------------------------------
#pragma region SOFT_TYPEDEFS
// ------------------------------------------------------

typedef uint32_t Pixel;
typedef uint32_t Color;
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
    KEY_NULL = 0,

    KEY_BACKSPACE = 8,
    KEY_TAB = 9,
    KEY_ESCAPE = 27,

    KEY_SPACE = 32, // Key: ' '
    KEY_EXCL, // Key: !
    KEY_QUOT, // Key: "
    KEY_NUM, // Key: #
    KEY_DOLLAR, // Key: $
    KEY_PERCENT, // Key: %
    KEY_AMP, // Key: &
    KEY_APOS, // Key: '
    KEY_LPAREN, // Key: (
    KEY_RPAREN, // Key: )
    KEY_AST, // Key: *
    KEY_PLUS, // Key: +
    KEY_COMMA, // Key: ,
    KEY_MINUS, // Key: -
    KEY_PERIOD, // Key: .
    KEY_SLASH, // Key: /
    
    KEY_ZERO, // Key: 0
    KEY_ONE, // Key: 1
    KEY_TWO, // Key: 2
    KEY_THREE, // Key: 3
    KEY_FOUR, // Key: 4
    KEY_FIVE, // Key: 5
    KEY_SIX, // Key: 6
    KEY_SEVEN, // Key: 7
    KEY_EIGHT, // Key: 8
    KEY_NINE, // Key: 9

    KEY_COLON, // Key: :
    KEY_SEMI, // Key: ;
    KEY_LT, // Key: <
    KEY_EQUALS, // Key: =
    KEY_GT, // Key: >
    KEY_QUEST, // Key: ? 
    KEY_AT, // Key: @

    KEY_A, // Key: A
    KEY_B, // Key: B
    KEY_C, // Key: C
    KEY_D, // Key: D
    KEY_E, // Key: E
    KEY_F, // Key: F
    KEY_G, // Key: G
    KEY_H, // Key: H
    KEY_I, // Key: I
    KEY_J, // Key: J
    KEY_K, // Key: K
    KEY_L, // Key: L
    KEY_M, // Key: M
    KEY_N, // Key: N
    KEY_O, // Key: O
    KEY_P, // Key: P
    KEY_Q, // Key: Q
    KEY_R, // Key: R
    KEY_S, // Key: S
    KEY_T, // Key: T
    KEY_U, // Key: U
    KEY_V, // Key: V
    KEY_W, // Key: W
    KEY_X, // Key: X
    KEY_Y, // Key: Y
    KEY_Z, // Key: Z

    KEY_LSQB, // Key: [ 
    KEY_BSLASH, // Key: '\'
    KEY_RSQB, // Key:  ]
    KEY_HAT, // Key: ^
    KEY_LOWBAR, // Key: _
    KEY_GRAVE, // Key: `

    KEY_A_LOWER, // Key: a
    KEY_B_LOWER, // Key: b
    KEY_C_LOWER, // Key: c
    KEY_D_LOWER, // Key: d
    KEY_E_LOWER, // Key: e
    KEY_F_LOWER, // Key: f
    KEY_G_LOWER, // Key: g
    KEY_H_LOWER, // Key: h
    KEY_I_LOWER, // Key: i
    KEY_J_LOWER, // Key: j
    KEY_K_LOWER, // Key: k
    KEY_L_LOWER, // Key: l
    KEY_M_LOWER, // Key: m
    KEY_N_LOWER, // Key: n
    KEY_O_LOWER, // Key: o
    KEY_P_LOWER, // Key: p
    KEY_Q_LOWER, // Key: q
    KEY_R_LOWER, // Key: r
    KEY_S_LOWER, // Key: s
    KEY_T_LOWER, // Key: t
    KEY_U_LOWER, // Key: u
    KEY_V_LOWER, // Key: v
    KEY_W_LOWER, // Key: w
    KEY_X_LOWER, // Key: x
    KEY_Y_LOWER, // Key: y
    KEY_Z_LOWER, // Key: z

    KEY_LCUB, // Key: {
    KEY_BAR, // Key: |
    KEY_RCUB, // Key: }
    KEY_TILDE, // Key: ~

    KEY_DEL // Key: DEL
} KeyCode;

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

SAPI void softSetWindowTitle(const char* title);
SAPI bool softWindowShoulClose();
SAPI void softCloseCallback();

SAPI iVec2 softGetWindowSize();
SAPI iVec2 softGetDisplaySize();

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

SAPI bool softKeyPressed(KeyCode key);
SAPI bool softKeyReleased(KeyCode key);
SAPI bool softKeyDown(KeyCode key);
SAPI bool softKeyUp(KeyCode key);

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

#ifdef SOFT_IMPLEMENTATION

#endif // SOFT_IMPLEMENTATION

#endif // SOFT_H