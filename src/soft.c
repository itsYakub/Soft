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
// - SOFT_INTERNAL_MACROS;
// - SOFT_INTERNAL;
// - SOFT_API_FUNC_WINDOW;
// - SOFT_API_FUNC_EVENTS;
// - SOFT_API_FUNC_INPUT;
// - SOFT_API_FUNC_RENDER;
// - SOFT_API_FUNC_SHAPES;
// - SOFT_API_FUNC_LOGGING;
// - SOFT_API_FUNC_TEXT;
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

// ------------------------------------------------------
#include "SDL_keycode.h"
#include "SDL_scancode.h"
#pragma region SOFT_INCLUDES
// ------------------------------------------------------

// ------------------------------
// Include header conventions:
// 1. Parent header.
//      (#include "soft.h")
// 2. Standard library headers.
//      (i.e. #include <stdio.h>)
// 3. Dependency headers.
//      (i.e. #include "SDL.h")
// 4. Internal headers.
// ------------------------------

// Parent header
#include "soft.h"

// Standard library headers
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>

// Dependency headers
#include "SDL.h"
#include "SDL_events.h"
#include "SDL_mouse.h"
#include "SDL_stdinc.h"
#include "SDL_video.h"
#include "SDL_blendmode.h"
#include "SDL_render.h"
#include "SDL_pixels.h"
#include "SDL_version.h"

// ------------------------------------------------------
#pragma endregion
// ------------------------------------------------------

// ------------------------------------------------------
#pragma region SOFT_INTERNAL_MACROS
// ------------------------------------------------------

#define SOFT_CHARBUF_SIZE_MAX 256
#define SOFT_KEYCODE_COUNT_TOTAL 128
#define SOFT_MOUSEBUTTON_COUNT_TOTAL 3

// Keyword definitions
#define internal static
#define global static

// ------------------------------------------------------
#pragma endregion
// ------------------------------------------------------

// ------------------------------------------------------
#pragma region SOFT_INTERNAL
// ------------------------------------------------------

// CORE: Global state struct
struct {
    struct {
        char version[SOFT_CHARBUF_SIZE_MAX];
        bool valid;
    } Platform;

    // CORE.Window: Window state
    struct {
        SDL_Window* window;

        softConfigFlags config_flags;

        char* title;

        iVec2 display_size;
        iVec2 screen_size;
        iVec2 screen_display_size_delta;

        bool quit;
        bool cursor_on_screen;

        bool valid;
    } Window;

    // CORE.Render: Renderer state
    struct {
        Pixel* data;
        SDL_Renderer* renderer;
        SDL_Texture* texture;

        iVec2 size;

        bool data_valid;
        bool renderer_valid;
        bool texture_valid;
    } Render;

    // CORE.Input: Input state
    struct {
        // CORE.Input.Mouse: Mouse state
        struct {
            bool mouse_button_pressed_current[SOFT_MOUSEBUTTON_COUNT_TOTAL];
            bool mouse_button_pressed_previous[SOFT_MOUSEBUTTON_COUNT_TOTAL];

            iVec2 position_current;
            iVec2 position_previous;
            iVec2 wheel_move;

            iVec2 offset;
            iVec2 scale;
        } Mouse;

        // CORE.Input.Keyboard: Keyboard state
        struct {
            softKeyCode exit_key;
            bool key_pressed_current[SOFT_KEYCODE_COUNT_TOTAL];
            bool key_pressed_previous[SOFT_KEYCODE_COUNT_TOTAL];
        } Keyboard;

    } Input;

} CORE;

internal void softSetPixel(int x, int y, Color color) {
    if(!CORE.Render.data_valid) {
        softLogError("Pixel data not valid. Returning...");
        return;
    }

    if(x < 0 || x >= CORE.Window.screen_size.x) { 
        return; 
    }

    if(y < 0 || y >= CORE.Window.screen_size.y) { 
        return; 
    }

    CORE.Render.data[y * CORE.Window.display_size.x + x] = softColorToPixel(color);
}

internal softKeyCode keycode_to_scancode[] = {
    KEY_NULL,
    
    0,
    0,
    0,

    KEY_A,
    KEY_B,
    KEY_C,
    KEY_D,
    KEY_E,
    KEY_F,
    KEY_G,
    KEY_H,
    KEY_I,
    KEY_J,
    KEY_K,
    KEY_L,
    KEY_M,
    KEY_N,
    KEY_O,
    KEY_P,
    KEY_Q,
    KEY_R,
    KEY_S,
    KEY_T,
    KEY_U,
    KEY_V,
    KEY_W,
    KEY_X,
    KEY_Y,
    KEY_Z,

    KEY_ONE, 
    KEY_TWO, 
    KEY_THREE, 
    KEY_FOUR, 
    KEY_FIVE, 
    KEY_SIX, 
    KEY_SEVEN, 
    KEY_EIGHT, 
    KEY_NINE, 
    KEY_ZERO, 

    0, // SDL_SCANCODE_RETURN

    KEY_ESCAPE,
    KEY_BACKSPACE,
    KEY_TAB,
    KEY_SPACE,

    KEY_MINUS,
    KEY_EQUALS,
    KEY_LSQB,
    KEY_RSQB,
    KEY_BSLASH,

    0, // SDL_SCANCODE_NONUSHASH

    KEY_SEMI,
    KEY_APOS,
    KEY_GRAVE,
    KEY_COMMA,
    KEY_PERIOD,
    KEY_SLASH,

    0, // SDL_SCANCODE_CAPSLOCK
    0, // SDL_SCANCODE_F1
    0, // SDL_SCANCODE_F2
    0, // SDL_SCANCODE_F3
    0, // SDL_SCANCODE_F4
    0, // SDL_SCANCODE_F5
    0, // SDL_SCANCODE_F6
    0, // SDL_SCANCODE_F7
    0, // SDL_SCANCODE_F8
    0, // SDL_SCANCODE_F9
    0, // SDL_SCANCODE_F10
    0, // SDL_SCANCODE_F11
    0, // SDL_SCANCODE_F12
    0, // SDL_SCANCODE_PRINTSCREEN
    0, // SDL_SCANCODE_SCROLLLOCK
    0, // SDL_SCANCODE_PAUSE
    0, // SDL_SCANCODE_INSERT
    0, // SDL_SCANCODE_HOME
    0, // SDL_SCANCODE_PAGEUP

    KEY_DEL,

    0, // SDL_SCANCODE_END
    0, // SDL_SCANCODE_PAGEDOWN
    0, // SDL_SCANCODE_RIGHT
    0, // SDL_SCANCODE_LEFT
    0, // SDL_SCANCODE_DOWN
    0, // SDL_SCANCODE_UP
};

internal softKeyCode SDLScancodeToSoftKeyCode(SDL_Scancode code) {
    if(code >= 0 || code < sizeof(keycode_to_scancode) / sizeof(softKeyCode)) {
        return keycode_to_scancode[code];
    }

    return KEY_NULL;
}

// ------------------------------------------------------
#pragma endregion
// ------------------------------------------------------

// ------------------------------------------------------
#pragma region SOFT_API_FUNC_WINDOW
// ------------------------------------------------------

SAPI int softInit(int width, int height, const char* title) { 
    softInitPlatform();
    softInitWindow(width, height, title);
    softInitRenderer();
    softInitRenderTexture(softGetDisplaySize().x, softGetDisplaySize().y);
    softInitPixelData();

    softLogInfo("Initialization process overview:");
    softLogInfo("   > Window state: %s", CORE.Window.valid ? "OK" : "FAIL");
    softLogInfo("   > Renderer state: %s", CORE.Render.renderer_valid ? "OK" : "FAIL");
    softLogInfo("   > Render Texture state: %s", CORE.Render.texture_valid ? "OK" : "FAIL");
    softLogInfo("   > Pixel Data state: %s", CORE.Render.data_valid ? "OK" : "FAIL");
    softLogInfo("Initialization finished. Hello World!");

    return SOFT_SUCCESS;
}

SAPI int softInitPlatform() {
    softLogInfo("Initializing Soft v.%s", SOFT_VERSION);
    int init = SDL_Init(SDL_INIT_VIDEO);

    if(init != 0) {
        softLogError("%s", strerror(errno));
        
        return SOFT_FAILED;
    }

    strcpy(CORE.Platform.version, softTextFormat("%d.%d.%d", SDL_MAJOR_VERSION, SDL_MINOR_VERSION, SDL_PATCHLEVEL));
    CORE.Platform.valid = true;

    softLogInfo("   > Platform Version: %s", CORE.Platform.version);
    softLogInfo("   > Version: %s", SOFT_VERSION);
    softLogInfo("   > Version major: %i", SOFT_VERSION_MAJOR);
    softLogInfo("   > Version minor: %i", SOFT_VERSION_MINOR);

    return SOFT_SUCCESS;
}

SAPI int softInitWindow(int width, int height, const char* title) {
    softLogInfo("Initializing Window.");

    CORE.Window.screen_size = (iVec2) { width, height };

    SDL_DisplayMode mode;
    SDL_GetCurrentDisplayMode(0, &mode);
    CORE.Window.display_size = (iVec2) { mode.w, mode.h };

    CORE.Window.quit = false;

    CORE.Input.Mouse.offset = (iVec2) { 0 };
    CORE.Input.Mouse.scale = (iVec2) { 1, 1 };

    CORE.Input.Keyboard.exit_key = KEY_ESCAPE;

    // TODO(yakub): Fix the Config Flag system

    uint32_t flags = 0;
    if(CORE.Window.config_flags & FLAG_WINDOW_RESIZABLE) flags |= SDL_WINDOW_RESIZABLE;
    if(CORE.Window.config_flags & FLAG_WINDOW_FULLSCREEN) flags |= SDL_WINDOW_FULLSCREEN;
    if(CORE.Window.config_flags & FLAG_WINDOW_MAXIMIZED) flags |= SDL_WINDOW_MAXIMIZED;
    if(CORE.Window.config_flags & FLAG_WINDOW_MINIMIZED) flags |= SDL_WINDOW_MINIMIZED;
    if(CORE.Window.config_flags & FLAG_WINDOW_HIGHDPI) flags |= SDL_WINDOW_ALLOW_HIGHDPI;

    CORE.Window.window = SDL_CreateWindow(
        title, 
        SDL_WINDOWPOS_CENTERED, 
        SDL_WINDOWPOS_CENTERED, 
        width, 
        height, 
        flags
    );

    if(!CORE.Window.window) {
        softLogError("%s", strerror(errno));

        softClosePlatform();
        
        return SOFT_FAILED;
    }

    CORE.Window.valid = true;

    softLogInfo("   > Window size: %ix%i", CORE.Window.screen_size.x, CORE.Window.screen_size.y);
    softLogInfo("   > Display size: %ix%i", CORE.Window.display_size.x, CORE.Window.display_size.y);

    return SOFT_SUCCESS;
}

SAPI int softInitRenderer() {
    softLogInfo("Initializing Renderer.");

    if(!CORE.Window.window) {
        softLogError("%s", strerror(errno));

        softClosePlatform();

        return SOFT_FAILED;
    }

    uint32_t flags = 0;
    flags |= SDL_RENDERER_TARGETTEXTURE;
    if(CORE.Window.config_flags & FLAG_WINDOW_VSYNC) flags |= SDL_RENDERER_PRESENTVSYNC;

    CORE.Render.renderer = SDL_CreateRenderer(
        CORE.Window.window, 
        -1, 
        flags
    );

    if(!CORE.Render.renderer) {
        softLogError("%s", strerror(errno));

        softCloseWindow();
        softClosePlatform();
        
        return SOFT_FAILED;
    }

    CORE.Render.renderer_valid = true;

    return SOFT_SUCCESS;
}

SAPI int softInitRenderTexture(int width, int height) {
    softLogInfo("Initializing RenderTexture.");

    if(!CORE.Window.window || !CORE.Render.renderer) {
        softLogError("%s", strerror(errno));

        softCloseWindow();
        softCloseRenderer();
        softClosePlatform();
        
        return SOFT_FAILED;
    }

    CORE.Render.size.x = width;
    CORE.Render.size.y = height;

    CORE.Render.texture = SDL_CreateTexture(
        CORE.Render.renderer, 
        SDL_PIXELFORMAT_ABGR8888, 
        SDL_TEXTUREACCESS_STREAMING, 
        width, 
        height
    );

    if(!CORE.Render.texture) {
        softLogError("%s", strerror(errno));

        softCloseWindow();
        softCloseRenderer();
        softClosePlatform();
        
        return SOFT_FAILED;
    }

    CORE.Render.texture_valid = true;

    softLogInfo("   > RenderTexture size: %ix%i", CORE.Window.display_size.x, CORE.Window.display_size.y);

    return SOFT_SUCCESS;
}

SAPI int softInitPixelData() {
    softLogInfo("Initializing Render Data.");

    if(!CORE.Window.window || !CORE.Render.renderer || !CORE.Render.texture) {
        softLogError("%s", strerror(errno));

        softCloseWindow();
        softCloseRenderer();
        softUnloadRenderTexture();
        softClosePlatform();
        
        return SOFT_FAILED;
    }

    CORE.Render.data = calloc(CORE.Window.display_size.x * CORE.Window.display_size.y, sizeof(Pixel));

    if(!CORE.Render.data) {
        softLogError("%s", strerror(errno));

        softCloseWindow();
        softCloseRenderer();
        softUnloadRenderTexture();
        softClosePlatform();
        
        return SOFT_FAILED;
    }

    CORE.Render.data_valid = true;

    softLogInfo("   > Pixel count: %i (%i bytes)", CORE.Window.display_size.x * CORE.Window.display_size.y, CORE.Window.display_size.x * CORE.Window.display_size.y * sizeof(Pixel));

    return SOFT_SUCCESS;
}

SAPI void softClose() {
    softLogInfo("Closing Soft v.%s", SOFT_VERSION);

    softUnloadPixelData();
    softUnloadRenderTexture();
    softCloseRenderer();
    softCloseWindow();
    softClosePlatform();
}

SAPI void softClosePlatform() {
    SDL_Quit();

    softLogInfo("Quitting. Goodbye World...");
    CORE.Platform.valid = false;
}

SAPI void softCloseWindow() {
    if(!CORE.Window.window) {
        softLogWarning("Window already closed. Returning...");

        return;
    }

    softLogInfo("Closing window.");

    SDL_DestroyWindow(CORE.Window.window);
    CORE.Window.valid = false;
}

SAPI void softCloseRenderer() {
    if(!CORE.Render.renderer) {
        softLogWarning("Renderer already closed. Returning...");

        return;
    }

    softLogInfo("Unloading renderer.");

    SDL_DestroyRenderer(CORE.Render.renderer);
    CORE.Render.renderer_valid = false;
}

SAPI void softUnloadRenderTexture() {
    if(!CORE.Render.texture) {
        softLogWarning("Render Texture already unloaded. Returning...");

        return;
    }

    softLogInfo("Unloading render texture.");

    SDL_DestroyTexture(CORE.Render.texture);
    CORE.Render.texture_valid = false;
}

SAPI void softUnloadPixelData() {
    if(!CORE.Render.data) {
        softLogWarning("Pixel Data already unloaded. Returning...");

        return;
    }

    softLogInfo("Unloading render data.");

    SDL_free(CORE.Render.data);
    CORE.Render.data_valid = false;
}

SAPI void softSetConfigFlags(softConfigFlags flags) {
    CORE.Window.config_flags |= flags;
}

SAPI void softSetWindowTitle(const char* title) {
    SDL_SetWindowTitle(CORE.Window.window, title);
    strcpy(CORE.Window.title, title);
}

SAPI bool softWindowShoulClose() {
    return CORE.Window.quit;
}

SAPI void softCloseCallback() {
    CORE.Window.quit = true;
}

SAPI iVec2 softGetWindowSize(){
    return CORE.Window.screen_size;
}

SAPI iVec2 softGetWindowCenter(){
    return (iVec2) { CORE.Window.screen_size.x / 2, CORE.Window.screen_size.y / 2 };
}

SAPI iVec2 softGetDisplaySize(){
    return CORE.Window.display_size;
}

SAPI iVec2 softGetDisplayCenter(){
    return (iVec2) { CORE.Window.display_size.x / 2, CORE.Window.display_size.y / 2 };
}

// ------------------------------------------------------
#pragma endregion
// ------------------------------------------------------

// ------------------------------------------------------
#pragma region SOFT_API_FUNC_EVENTS
// ------------------------------------------------------


SAPI void softPollEvents() {
    for(int key = 0; key < SOFT_KEYCODE_COUNT_TOTAL; key++) {
        CORE.Input.Keyboard.key_pressed_previous[key] = CORE.Input.Keyboard.key_pressed_current[key];
    }

    for(int button = 0; button < SOFT_MOUSEBUTTON_COUNT_TOTAL; button++) {
        CORE.Input.Mouse.mouse_button_pressed_previous[button] = CORE.Input.Mouse.mouse_button_pressed_current[button];
    }

    SDL_Event event = { 0 };
    while(SDL_PollEvent(&event)) {
        switch(event.type) {
            case SDL_WINDOWEVENT:
                switch (event.window.event) {
                    case SDL_WINDOWEVENT_SIZE_CHANGED:
                        CORE.Window.screen_size.x = event.window.data1;
                        CORE.Window.screen_size.y = event.window.data2;

                        CORE.Window.screen_display_size_delta.x = CORE.Window.display_size.x - CORE.Window.screen_size.x;
                        CORE.Window.screen_display_size_delta.y = CORE.Window.display_size.y - CORE.Window.screen_size.y;

                        break;

                    case SDL_WINDOWEVENT_CLOSE:
                        softCloseCallback();
                        
                        break;

                    case SDL_WINDOWEVENT_ENTER:
                        CORE.Window.cursor_on_screen = true;
                        
                        break;

                    case SDL_WINDOWEVENT_LEAVE:
                        CORE.Window.cursor_on_screen = false;
                        
                        break;
                }

                break;

            case SDL_KEYDOWN: {
                softKeyCode key = SDLScancodeToSoftKeyCode(event.key.keysym.scancode);

                if(key != KEY_NULL && key < SOFT_KEYCODE_COUNT_TOTAL) {
                    CORE.Input.Keyboard.key_pressed_current[key] = true;
                }

                if(softKeyPressed(CORE.Input.Keyboard.exit_key)) {
                    softCloseCallback();
                }

                break;
            }

            case SDL_KEYUP: {
                softKeyCode key = SDLScancodeToSoftKeyCode(event.key.keysym.scancode);

                if(key != KEY_NULL && key < SOFT_KEYCODE_COUNT_TOTAL) {
                    CORE.Input.Keyboard.key_pressed_current[key] = false;
                }

                break;
            }

            case SDL_MOUSEMOTION:
                CORE.Input.Mouse.position_previous = CORE.Input.Mouse.position_current;

                CORE.Input.Mouse.position_current.x = event.motion.x;
                CORE.Input.Mouse.position_current.y = event.motion.y;
                
                break;

            case SDL_MOUSEWHEEL:
                CORE.Input.Mouse.wheel_move.x = event.wheel.x;
                CORE.Input.Mouse.wheel_move.y = event.wheel.y;
                
                break;

            case SDL_MOUSEBUTTONDOWN: {
                softMouseButtons button;

                if(event.button.button == SDL_BUTTON_LEFT) button = BUTTON_LEFT;
                else if(event.button.button == SDL_BUTTON_MIDDLE) button = BUTTON_MIDDLE;
                else if(event.button.button == SDL_BUTTON_RIGHT) button = BUTTON_RIGHT;

                CORE.Input.Mouse.mouse_button_pressed_current[button] = true;
                
                break;
            }

            case SDL_MOUSEBUTTONUP:{
                softMouseButtons button;

                if(event.button.button == SDL_BUTTON_LEFT) button = BUTTON_LEFT;
                else if(event.button.button == SDL_BUTTON_MIDDLE) button = BUTTON_MIDDLE;
                else if(event.button.button == SDL_BUTTON_RIGHT) button = BUTTON_RIGHT;

                CORE.Input.Mouse.mouse_button_pressed_current[button] = false;

                break;
            }
        }
    }
}

// ------------------------------------------------------
#pragma endregion
// ------------------------------------------------------

// ------------------------------------------------------
#pragma region SOFT_API_FUNC_INPUT
// ------------------------------------------------------


SAPI iVec2 softGetMousePosition() {
    return (iVec2) {
        (CORE.Input.Mouse.position_current.x + CORE.Input.Mouse.offset.x) * CORE.Input.Mouse.scale.x,
        (CORE.Input.Mouse.position_current.y + CORE.Input.Mouse.offset.y) * CORE.Input.Mouse.scale.y
    };
}

SAPI iVec2 softGetPreviousMousePosition() {
    return (iVec2) {
        (CORE.Input.Mouse.position_previous.x + CORE.Input.Mouse.offset.x) * CORE.Input.Mouse.scale.x,
        (CORE.Input.Mouse.position_previous.y + CORE.Input.Mouse.offset.y) * CORE.Input.Mouse.scale.y
    };
}

SAPI iVec2 softGetMouseDelta() {
    return (iVec2) {
        CORE.Input.Mouse.position_current.x - CORE.Input.Mouse.position_previous.x,
        CORE.Input.Mouse.position_current.y - CORE.Input.Mouse.position_previous.y
    };
}

SAPI iVec2 softGetMouseWheel() {
    return CORE.Input.Mouse.wheel_move;
}

SAPI bool softKeyPressed(softKeyCode key) {
    bool pressed = false;

    if(CORE.Input.Keyboard.key_pressed_current[key] && !CORE.Input.Keyboard.key_pressed_previous[key]) {
        pressed = true;
    }

    return pressed;
}

SAPI bool softKeyReleased(softKeyCode key) {    
    bool released = false;

    if(!CORE.Input.Keyboard.key_pressed_current[key] && CORE.Input.Keyboard.key_pressed_previous[key]) {
        released = true;
    }

    return released;
}

SAPI bool softKeyDown(softKeyCode key) {
    return CORE.Input.Keyboard.key_pressed_current[key];
}

SAPI bool softKeyUp(softKeyCode key) {    
    return !CORE.Input.Keyboard.key_pressed_current[key];
}

SAPI bool softMouseButtonPressed(softMouseButtons button) {
    bool pressed = false;

    if(CORE.Input.Mouse.mouse_button_pressed_current[button] && !CORE.Input.Mouse.mouse_button_pressed_previous[button]) {
        pressed = true;
    }

    return pressed;

}

SAPI bool softMouseButtonReleased(softMouseButtons button) {
    bool released = false;

    if(!CORE.Input.Mouse.mouse_button_pressed_current[button] && CORE.Input.Mouse.mouse_button_pressed_previous[button]) {
        released = true;
    }

    return released;

}

SAPI bool softMouseButtonDown(softMouseButtons button) {
    return CORE.Input.Mouse.mouse_button_pressed_current[button];
}

SAPI bool softMouseButtonUp(softMouseButtons button) {
    return !CORE.Input.Mouse.mouse_button_pressed_current[button];
}

// ------------------------------------------------------
#pragma endregion
// ------------------------------------------------------

// ------------------------------------------------------
#pragma region SOFT_API_FUNC_RENDER
// ------------------------------------------------------


SAPI void softClearBuffer() {
    if(!CORE.Render.data_valid) {
        softLogError("Pixel data not valid. Returning...");
        return;
    }

    SDL_memset(CORE.Render.data, 0, CORE.Window.display_size.x * CORE.Window.display_size.y * sizeof(Pixel));
}

SAPI void softClearBufferColor(Color color) {
    if(!CORE.Render.data_valid) {
        softLogError("Pixel data not valid. Returning...");
        return;
    }

    for(int y = 0; y < CORE.Window.display_size.y; y++) {
        for(int x = 0; x < CORE.Window.display_size.x; x++) {
            softSetPixel(x, y, color);
        }
    }
}

SAPI void softBlit() {
    if(!CORE.Render.data_valid) {
        softLogError("Pixel data not valid. Returning...");
        return;
    } else if(!CORE.Render.texture_valid) {
        softLogError("Render Texture not valid. Returning...");
        return;
    } else if(!CORE.Render.renderer_valid) {
        softLogError("Renderer not valid. Returning...");
        return;
    }

    SDL_UpdateTexture(
        CORE.Render.texture,
        NULL,
        CORE.Render.data, 
        CORE.Window.display_size.x * sizeof(Pixel)
    );

    SDL_Rect destination_rect = {
        0,
        0,
        CORE.Window.display_size.x,
        CORE.Window.display_size.y
    };

    SDL_RenderCopyEx(
        CORE.Render.renderer, 
        CORE.Render.texture, 
        NULL, 
        &destination_rect,
        0.0,
        NULL,
        SDL_FLIP_NONE
    );

    SDL_RenderPresent(CORE.Render.renderer);
}

// ------------------------------------------------------
#pragma endregion
// ------------------------------------------------------

// ------------------------------------------------------
#pragma region SOFT_API_FUNC_SHAPES
// ------------------------------------------------------


SAPI void softDrawRectangle(Rect rect, Color color) {
    for(int i = 0; i < rect.size.y; i++) {
        for(int j = 0; j < rect.size.x; j++) {
            softSetPixel( rect.position.x + j, rect.position.y + i, color);
        }
    }
}

SAPI void softDrawRectangleLines(Rect rect, Color color) {
    Line rect_lines[4]  = {
        { (iVec2) { rect.position.x, rect.position.y }, (iVec2) { rect.position.x + rect.size.x, rect.position.y } },
        { (iVec2) { rect.position.x, rect.position.y }, (iVec2) { rect.position.x, rect.position.y + rect.size.y} },
        { (iVec2) { rect.position.x + rect.size.x, rect.position.y }, (iVec2) { rect.position.x + rect.size.x, rect.position.y + rect.size.y } },
        { (iVec2) { rect.position.x, rect.position.y + rect.size.y }, (iVec2) { rect.position.x + rect.size.x, rect.position.y + rect.size.y } },
    };

    for(int i = 0; i < 4; i++) {
        softDrawLine(rect_lines[i], color);
    }
}

SAPI void softDrawLine(Line line, Color color) {
    // Source: https://en.wikipedia.org/wiki/Digital_differential_analyzer_(graphics_algorithm)

    float dx = line.b.x - line.a.x;
    float dy = line.b.y - line.a.y;
    float steps = 0;
    float i = 0;

    float x = 0;
    float y = 0;

    if (abs((int)(dx)) >= abs((int)(dy))) {
        steps = abs((int)(dx));
    } else {
        steps = abs((int)(dy));
    }

    dx = dx / steps;
    dy = dy / steps;
    x = line.a.x;
    y = line.a.y;

    while (i <= steps) {
        softSetPixel(x, y, color);
        x += dx;
        y += dy;
        i++;
    }
}

SAPI void softDrawCircle(Circle circle, Color color) {
    // Source: https://stackoverflow.com/questions/1201200/fast-algorithm-for-drawing-filled-circles/14976268#14976268

    int x = circle.r;
    int y = 0;
    int xChange = 1 - (circle.r << 1);
    int yChange = 0;
    int radiusError = 0;

    while (x >= y)
    {
        for (int i = circle.position.x - x; i <= circle.position.x + x; i++)
        {
            softSetPixel(i, circle.position.y + y, color);
            softSetPixel(i, circle.position.y - y, color);
        }
        for (int i = circle.position.x - y; i <= circle.position.x + y; i++)
        {
            softSetPixel(i, circle.position.y + x, color);
            softSetPixel(i, circle.position.y - x, color);
        }

        y++;
        radiusError += yChange;
        yChange += 2;
        if (((radiusError << 1) + xChange) > 0)
        {
            x--;
            radiusError += xChange;
            xChange += 2;
        }
    }
}

SAPI void softDrawCircleLines(Circle circle, Color color) {
    // Source: https://zingl.github.io/bresenham.html

    int r = circle.r;
    int x = r * -1;
    int y = 0; 
    int err = 2 - 2 * r; 

    do {
        softSetPixel(circle.position.x - x, circle.position.y + y, color);
        softSetPixel(circle.position.x - y, circle.position.y - x, color);
        softSetPixel(circle.position.x + x, circle.position.y - y, color);
        softSetPixel(circle.position.x + y, circle.position.y + x, color);

        r = err;

        if (r <= y) {
            err += ++y * 2 + 1;
        }

        if (r > x || err > y) {
            err += ++x * 2 + 1;
        }
    } while (x < 0);
}

// ------------------------------------------------------
#pragma endregion
// ------------------------------------------------------

// ------------------------------------------------------
#pragma region SOFT_API_FUNC_LOGGING
// ------------------------------------------------------

SAPI void softLog(SoftLogLevel log_level, const char *restrict txt, ...) {

#ifdef SOFT_DISABLE_VERBOSITY

    return;

#endif

    switch(log_level) {
        case LOG_INFO:
            softLogInfo(txt);
            break;

        case LOG_WARNING:
            softLogWarning(txt);
            break;

        case LOG_ERROR:
            softLogError(txt);
            break;

        default:
            softLogWarning("Invalid log level: %i", (int)(log_level));
            break;
    }
}


SAPI void softLogInfo(const char *restrict txt, ...) {

#ifdef SOFT_DISABLE_VERBOSITY

    return;

#endif

    char buf[256];

    va_list list;
    va_start(list, txt);

        vsnprintf(buf, sizeof(buf), txt, list);

    va_end(list);

    fprintf(stdout, "[INFO] %s\n", buf);
}

SAPI void softLogWarning(const char *restrict txt, ...) {

#ifdef SOFT_DISABLE_VERBOSITY

    return;

#endif

    char buf[256];

    va_list list;
    va_start(list, txt);

        vsnprintf(buf, sizeof(buf), txt, list);

    va_end(list);

    fprintf(stdout, "[WARN] %s\n", buf);
}

SAPI void softLogError(const char *restrict txt, ...) {

#ifdef SOFT_DISABLE_VERBOSITY

    return;

#endif

    char buf[256];

    va_list list;
    va_start(list, txt);

        vsnprintf(buf, sizeof(buf), txt, list);

    va_end(list);

    fprintf(stderr, "[ERR] %s\n", buf);
}

// ------------------------------------------------------
#pragma endregion
// ------------------------------------------------------

// ------------------------------------------------------
#pragma region SOFT_API_FUNC_TEXT
// ------------------------------------------------------

SAPI const char* softTextFormat(const char *restrict txt, ...) {
    // Source: https://github.com/raysan5/raylib/blob/master/src/rtext.c#L1408

    #define CHARBUF_COUNT_TOTAL 4

    global char buf[CHARBUF_COUNT_TOTAL][SOFT_CHARBUF_SIZE_MAX] = { 0 };
    global int buffer_index = 0;

    char* current_buffer = buf[buffer_index];
    SDL_memset(current_buffer, 0, SOFT_CHARBUF_SIZE_MAX * sizeof(char));

    va_list list;
    va_start(list, txt);

        vsnprintf(current_buffer, SOFT_CHARBUF_SIZE_MAX, txt, list);

    va_end(list);

    buffer_index++;
    if(buffer_index >= CHARBUF_COUNT_TOTAL) {
        buffer_index = 0;
    }

    return current_buffer;
}

SAPI const bool softTextEmpty(const char *restrict txt, ...) {
    return txt[0] != '\0';
}

SAPI const int softTextLength(const char *restrict txt) {
    return SDL_strlen(txt);
}

// ------------------------------------------------------
#pragma endregion
// ------------------------------------------------------

// ------------------------------------------------------
#pragma region SOFT_FUNC_COLOR
// ------------------------------------------------------

SAPI Pixel softColorToPixel(Color color) {
    Pixel result = { 0 };

    // NOTE: Remember about the reverse order of the bytes.
    // This library is designed with the little endian architecture in mind!
    // Order of the bytes is as follows: 0xAABBGGRR
    // AA - alpha
    // BB - blue
    // GG - green
    // RR - red
    // We need to shift out bytes in the way that is compatible with the rule of endianess.
    // RR -> result.r - we don't need to shift out bits to the left
    // GG -> result.g - we need to shift to the left by 8 bits (1 byte = 1 uint8_t)
    // BB -> result.b - we need to shift to the left by 16 bits (2 bytes = 2 uint8_t's)
    // AA -> result.a - we need to shift to the left by 24 bits (3 bytes = 3 uint8_t's)

    result = 
        (int)color.r | 
        (int)color.g << 8 | 
        (int)color.b << 16 | 
        (int)color.a << 24;

    return result;
}

SAPI Pixel softColorFToPixel(fColor color) {
    Color regular_color = {
        (uint8_t) color.r * 255,
        (uint8_t) color.g * 255,
        (uint8_t) color.b * 255,
        (uint8_t) color.a * 255,
    };

    return softColorToPixel(regular_color);
}

SAPI Color softPixelToColor(Pixel pixel) {
    Color result = { 0 };

    // NOTE: Remember about the reverse order of the bytes.
    // This library is designed with the little endian architecture in mind!
    // Order of the bytes is as follows: 0xAABBGGRR
    // AA - alpha
    // BB - blue
    // GG - green
    // RR - red
    // When we want to get the values of our struct to the iColor variable, we need to shift our bytes correctly
    // result.r -> RR - this is our starting point, so we don't need to shidt our bytes to the right
    // result.g -> GG - we need to shift to the right by 8 bits (1 byte = 1 uint8_t)
    // result.b -> BB - we need to shift to the right by 16 bits (2 bytes = 2 uint8_t's)
    // result.a -> AA - we need to shift to the right by 24 bits (3 bytes = 3 uint8_t's)


    result.r = ((uint8_t)pixel) & 0xFF;
    result.g = ((uint8_t)pixel >> 8) & 0xFF;
    result.b = ((uint8_t)pixel >> 16) & 0xFF;
    result.a = ((uint8_t)pixel >> 24) & 0xFF;

    return result;
}

SAPI fColor softPixelToColorF(Pixel pixel) {
    Color regular_color = softPixelToColor(pixel);

    return (fColor) { 
        regular_color.r / 255.0f, 
        regular_color.r / 255.0f, 
        regular_color.r / 255.0f, 
        regular_color.r / 255.0f
    };
}

SAPI bool softColorCompare(Color a, Color b) {
    return 
        (a.r == b.r) &&
        (a.g == b.g) &&
        (a.b == b.b) &&
        (a.a == b.a);
}

SAPI bool softIntColorCompare(Pixel a, Pixel b) {
    return a & b;
}

// ------------------------------------------------------
#pragma endregion
// ------------------------------------------------------
