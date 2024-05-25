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
// - SOFT_API_FUNC_CONFIG;
// - SOFT_API_FUNC_WINDOW;
// - SOFT_API_FUNC_EVENTS;
// - SOFT_API_FUNC_INPUT;
// - SOFT_API_FUNC_RENDER;
// - SOFT_API_FUNC_DRAW;
// - SOFT_API_FUNC_LOGGING;
// - SOFT_API_FUNC_TEXT;
// - SOFT_API_FUNC_COLOR;
// - SOFT_API_FUNC_TIME;
// - SOFT_API_FUNC_MATH;
// - SOFT_API_FUNC_IMAGE;
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
#include "SDL_error.h"
#include "SDL_keycode.h"
#include "SDL_scancode.h"
#include "SDL_timer.h"
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

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

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
    // CORE.Config - applications config
    struct {
        bool alpha_blend;
    } Config;

    struct {
        char version[SOFT_CHARBUF_SIZE_MAX];
        bool valid;
    } Platform;

    // CORE.Window: Window state
    struct {
        SDL_Window* window;

        softConfigFlags config_flags;

        char title[SOFT_CHARBUF_SIZE_MAX];

        iVec2 display_size;
        iVec2 screen_size;
        iVec2 screen_display_size_delta;

        bool quit;
        bool cursor_on_screen;

        bool valid;
    } Window;

    // CORE.Render: Renderer state
    struct {
        PixelBuffer data;
        SDL_Renderer* renderer;
        SDL_Texture* texture;

        iVec2 size;

        bool vsync_enabled;

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

    // CORE.Time: Timing state
    struct {
        // Delta time calculation
        f32 current;
        f32 previous;
        f32 delta_time;
        
        f32 frame_target;
        f32 frame_lifespan;

        f32 application_lifetime;
        
        u32 framerate;
    } Time;
} CORE;

internal void softSetPixel(i32 x, i32 y, Pixel pixel) {
    // Check if the pixel buffer exists.
    if(!CORE.Render.data_valid) {
        softLogError("Pixel data not valid. Returning...");
        return;
    }

    // Simple boundary check.
    if(x < 0 || x >= CORE.Window.screen_size.x || y < 0 || y >= CORE.Window.screen_size.y) { 
        return; 
    }

    // Check if the pixel at position [x, y] equals the color given as a parameter.
    // If so, skip the rest of this function; repeating the same action for something that doesn't change is a resource-waste. 
    if(softPixelColorCompare(pixel, softGetPixelColor(x, y))) { 
        return; 
    }

    if(CORE.Config.alpha_blend && !softPixelColorCompare(pixel, BLANK)) {
        pixel = softMixPixels(
            softGetPixelColor(x, y),
            pixel, 
            softPixelToColor(pixel).a
        );
    }

    if(softPixelColorCompare(pixel, BLANK)) {
        pixel = softGetPixelColor(x, y);
    }

    CORE.Render.data[y * CORE.Window.display_size.x + x] = pixel;
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

    KEY_RIGHT, 
    KEY_LEFT, 
    KEY_DOWN, 
    KEY_UP, 
};

internal softKeyCode SDLScancodeToSoftKeyCode(SDL_Scancode code) {
    if(code >= 0 || code < sizeof(keycode_to_scancode) / sizeof(softKeyCode)) {
        return keycode_to_scancode[code];
    }

    return KEY_NULL;
}

internal void softTimeMenagement() {
    CORE.Time.current = softTime();
    f32 frame_time = CORE.Time.current - CORE.Time.previous;
    CORE.Time.previous = CORE.Time.current;

    CORE.Time.delta_time = frame_time;

    if(CORE.Time.delta_time < CORE.Time.frame_target) {
        softWait(CORE.Time.frame_target - CORE.Time.delta_time);

        CORE.Time.current = softTime();
        f32 wait_time = CORE.Time.current - CORE.Time.previous;
        CORE.Time.previous = CORE.Time.current;

        CORE.Time.delta_time += wait_time;
    }
}

// ------------------------------------------------------
#pragma endregion
// ------------------------------------------------------

// ------------------------------------------------------
#pragma region SOFT_API_FUNC_CONFIG
// ------------------------------------------------------

SAPI void softAlphaBlendState(bool state) {
    state ?
        softLogInfo("Alpha-Blending: ENABLED (\"Alpha\" channle will be used during the color calculations).") :
        softLogInfo("Alpha-Blending: DISABLED (\"Alpha\" channle will be immited during the color calculations).");

    CORE.Config.alpha_blend = state;
}

// ------------------------------------------------------
#pragma endregion
// ------------------------------------------------------

// ------------------------------------------------------
#pragma region SOFT_API_FUNC_WINDOW
// ------------------------------------------------------

SAPI i32 softInit(i32 width, i32 height, const string title) { 
    softInitPlatform();
    softInitWindow(width, height, title);
    softInitRenderer();
    softInitRenderTexture(softGetDisplaySize().x, softGetDisplaySize().y);
    softInitPixelBuffer();

    softLogInfo("Initialization process overview:");
    softLogInfo("   > Window state: %s", CORE.Window.valid ? "OK" : "FAIL");
    softLogInfo("   > Renderer state: %s", CORE.Render.renderer_valid ? "OK" : "FAIL");
    softLogInfo("   > Render Texture state: %s", CORE.Render.texture_valid ? "OK" : "FAIL");
    softLogInfo("   > Pixel Data state: %s", CORE.Render.data_valid ? "OK" : "FAIL");
    softLogInfo("Initialization finished. Hello World!");

    return SOFT_SUCCESS;
}

SAPI i32 softInitPlatform(void) {
    softLogInfo("Initializing Soft v.%s", SOFT_VERSION);
    i32 init = SDL_Init(SDL_INIT_VIDEO);

    if(init != 0) {
        softLogError("%s", strerror(errno));
        
        return SOFT_FAILED;
    }

    strcpy(CORE.Platform.version, softTextFormat("%d.%d.%d", SDL_MAJOR_VERSION, SDL_MINOR_VERSION, SDL_PATCHLEVEL));
    
    softAlphaBlendState(true);

    softLogInfo("   > Platform Version: %s", CORE.Platform.version);
    softLogInfo("   > Version: %s", SOFT_VERSION);
    softLogInfo("   > Version major: %i", SOFT_VERSION_MAJOR);
    softLogInfo("   > Version minor: %i", SOFT_VERSION_MINOR);

    CORE.Platform.valid = true;

    return SOFT_SUCCESS;
}

SAPI i32 softInitWindow(i32 width, i32 height, const string title) {
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

    u32 flags = 0;
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

SAPI i32 softInitRenderer(void) {
    softLogInfo("Initializing Renderer.");

    if(!CORE.Window.window) {
        softLogError("%s", SDL_GetError());

        softClosePlatform();

        return SOFT_FAILED;
    }

    u32 flags = 0;
    flags |= SDL_RENDERER_ACCELERATED;

    if(CORE.Window.config_flags & FLAG_WINDOW_VSYNC) {
        flags |= SDL_RENDERER_PRESENTVSYNC;
        CORE.Render.vsync_enabled = true;
    } else {
        CORE.Render.vsync_enabled = false;
    }
        

    CORE.Render.renderer = SDL_CreateRenderer(
        CORE.Window.window, 
        -1, 
        flags
    );

    if(!CORE.Render.renderer) {
        softLogError("%s", SDL_GetError());

        softCloseWindow();
        softClosePlatform();
        
        return SOFT_FAILED;
    }

    CORE.Render.renderer_valid = true;

    return SOFT_SUCCESS;
}

SAPI i32 softInitRenderTexture(i32 width, i32 height) {
    softLogInfo("Initializing RenderTexture.");

    if(!CORE.Window.window || !CORE.Render.renderer) {
        softLogError("%s", SDL_GetError());

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
        softLogError("%s", SDL_GetError());

        softCloseWindow();
        softCloseRenderer();
        softClosePlatform();
        
        return SOFT_FAILED;
    }

    CORE.Render.texture_valid = true;

    softLogInfo("   > RenderTexture size: %ix%i", CORE.Window.display_size.x, CORE.Window.display_size.y);

    return SOFT_SUCCESS;
}

SAPI i32 softInitPixelBuffer(void) {
    softLogInfo("Initializing Pixel Buffer.");

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

SAPI void softClose(void) {
    softLogInfo("Closing Soft v.%s", SOFT_VERSION);

    softUnloadPixelBuffer();
    softUnloadRenderTexture();
    softCloseRenderer();
    softCloseWindow();
    softClosePlatform();
}

SAPI void softClosePlatform(void) {
    SDL_Quit();

    softLogInfo("Quitting. Goodbye World...");
    CORE.Platform.valid = false;
}

SAPI void softCloseWindow(void) {
    if(!CORE.Window.window) {
        softLogWarning("Window already closed. Returning...");

        return;
    }

    softLogInfo("Closing window.");

    SDL_DestroyWindow(CORE.Window.window);
    CORE.Window.valid = false;
}

SAPI void softCloseRenderer(void) {
    if(!CORE.Render.renderer) {
        softLogWarning("Renderer already closed. Returning...");

        return;
    }

    softLogInfo("Unloading renderer.");

    SDL_DestroyRenderer(CORE.Render.renderer);
    CORE.Render.renderer_valid = false;
}

SAPI void softUnloadRenderTexture(void) {
    if(!CORE.Render.texture) {
        softLogWarning("Render Texture already unloaded. Returning...");

        return;
    }

    softLogInfo("Unloading render texture.");

    SDL_DestroyTexture(CORE.Render.texture);
    CORE.Render.texture_valid = false;
}

SAPI void softUnloadPixelBuffer(void) {
    if(!CORE.Render.data) {
        softLogWarning("Pixel Buffer already unloaded. Returning...");

        return;
    }

    softLogInfo("Unloading Pixel Buffer.");

    SDL_free(CORE.Render.data);
    CORE.Render.data_valid = false;
}

SAPI bool softWindowShoulClose(void) {
    return CORE.Window.quit;
}

SAPI void softCloseCallback(void) {
    CORE.Window.quit = true;
}

SAPI iVec2 softGetWindowSize(void){
    return CORE.Window.screen_size;
}

SAPI iVec2 softGetWindowCenter(void){
    return (iVec2) { CORE.Window.screen_size.x / 2, CORE.Window.screen_size.y / 2 };
}

SAPI iVec2 softGetDisplaySize(void){
    return CORE.Window.display_size;
}

SAPI iVec2 softGetDisplayCenter(void){
    return (iVec2) { CORE.Window.display_size.x / 2, CORE.Window.display_size.y / 2 };
}

SAPI void softSetConfigFlags(softConfigFlags flags) {
    CORE.Window.config_flags |= flags;
}

SAPI void softSetWindowTitle(const string title) {
    SDL_SetWindowTitle(CORE.Window.window, title);
    strcpy(CORE.Window.title, title);
}

// ------------------------------------------------------
#pragma endregion
// ------------------------------------------------------

// ------------------------------------------------------
#pragma region SOFT_API_FUNC_EVENTS
// ------------------------------------------------------


SAPI void softPollEvents(void) {
    for(i32 key = 0; key < SOFT_KEYCODE_COUNT_TOTAL; key++) {
        CORE.Input.Keyboard.key_pressed_previous[key] = CORE.Input.Keyboard.key_pressed_current[key];
    }

    for(i32 button = 0; button < SOFT_MOUSEBUTTON_COUNT_TOTAL; button++) {
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


SAPI iVec2 softGetMousePosition(void) {
    return (iVec2) {
        (CORE.Input.Mouse.position_current.x + CORE.Input.Mouse.offset.x) * CORE.Input.Mouse.scale.x,
        (CORE.Input.Mouse.position_current.y + CORE.Input.Mouse.offset.y) * CORE.Input.Mouse.scale.y
    };
}

SAPI iVec2 softGetPreviousMousePosition(void) {
    return (iVec2) {
        (CORE.Input.Mouse.position_previous.x + CORE.Input.Mouse.offset.x) * CORE.Input.Mouse.scale.x,
        (CORE.Input.Mouse.position_previous.y + CORE.Input.Mouse.offset.y) * CORE.Input.Mouse.scale.y
    };
}

SAPI iVec2 softGetMouseDelta(void) {
    return (iVec2) {
        CORE.Input.Mouse.position_current.x - CORE.Input.Mouse.position_previous.x,
        CORE.Input.Mouse.position_current.y - CORE.Input.Mouse.position_previous.y
    };
}

SAPI iVec2 softGetMouseWheel(void) {
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


SAPI void softClearBuffer(void) {
    if(!CORE.Render.data_valid) {
        softLogError("Pixel data not valid. Returning...");
        return;
    }

    SDL_memset(CORE.Render.data, 0, CORE.Window.display_size.x * CORE.Window.display_size.y * sizeof(Pixel));
}

SAPI void softClearBufferColor(Pixel pixel) {
    if(!CORE.Render.data_valid) {
        softLogError("Pixel data not valid. Returning...");
        return;
    }

    for(i32 y = 0; y < CORE.Window.screen_size.y; y++) {
        for(i32 x = 0; x < CORE.Window.screen_size.x; x++) {
            softSetPixel(x, y, pixel);
        }
    }
}

SAPI void softBlit(void) {
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

    softTimeMenagement();
    softPollEvents();
}

// ------------------------------------------------------
#pragma endregion
// ------------------------------------------------------

// ------------------------------------------------------
#pragma region SOFT_API_FUNC_DRAW
// ------------------------------------------------------

SAPI void softDrawRectangle(Rect rect, Pixel pixel) {
    for(i32 i = 0; i < rect.size.y; i++) {
        for(i32 j = 0; j < rect.size.x; j++) {
            softSetPixel(rect.position.x + j, rect.position.y + i, pixel);
        }
    }
}

SAPI void softDrawRectangleLines(Rect rect, Pixel pixel) {
    Line rect_lines[4]  = {
        { (iVec2) { rect.position.x, rect.position.y }, (iVec2) { rect.position.x + rect.size.x, rect.position.y } },
        { (iVec2) { rect.position.x, rect.position.y }, (iVec2) { rect.position.x, rect.position.y + rect.size.y} },
        { (iVec2) { rect.position.x + rect.size.x, rect.position.y }, (iVec2) { rect.position.x + rect.size.x, rect.position.y + rect.size.y } },
        { (iVec2) { rect.position.x, rect.position.y + rect.size.y }, (iVec2) { rect.position.x + rect.size.x, rect.position.y + rect.size.y } },
    };

    for(i32 i = 0; i < 4; i++) {
        softDrawLine(rect_lines[i], pixel);
    }
}

SAPI void softDrawRectangleExtanded(Rect rect, iVec2 pivot, Pixel pixel) {
    softDrawRectangle(
        (Rect) {
            (iVec2) {
                rect.position.x - pivot.x,
                rect.position.y - pivot.y,
            },
            rect.size
        },
        pixel
    );
}

SAPI void softDrawLine(Line line, Pixel pixel) {
    // Source: https://en.wikipedia.org/wiki/Digital_differential_analyzer_(graphics_algorithm)

    f32 dx = line.b.x - line.a.x;
    f32 dy = line.b.y - line.a.y;
    f32 steps = 0;
    f32 i = 0;

    f32 x = 0;
    f32 y = 0;

    if (abs((i32)(dx)) >= abs((i32)(dy))) {
        steps = abs((i32)(dx));
    } else {
        steps = abs((i32)(dy));
    }

    dx = dx / steps;
    dy = dy / steps;
    x = line.a.x;
    y = line.a.y;

    while (i <= steps) {
        softSetPixel(x, y, pixel);
        x += dx;
        y += dy;
        i++;
    }
}

SAPI void softDrawCircle(Circle circle, Pixel pixel) {
    // Source: https://youtu.be/LmQKZmQh1ZQ?list=PLpM-Dvs8t0Va-Gb0Dp4d9t8yvNFHaKH6N&t=3088

    i32 x0 = circle.position.x - circle.r;
    i32 x1 = circle.position.x + circle.r;

    i32 y0 = circle.position.y - circle.r;
    i32 y1 = circle.position.y + circle.r;

    for(i32 y = y0; y < y1; y++) {
        for(i32 x = x0; x < x1; x++) {
            i32 x_delta = x - circle.position.x;
            i32 y_delta = y - circle.position.y;

            if(softSqrI(x_delta) + softSqrI(y_delta) <= softSqrI(circle.r)) {
                softSetPixel(x, y, pixel);
            }
        }
    }
}

SAPI void softDrawCircleLines(Circle circle, Pixel pixel) {
    // Source: https://zingl.github.io/bresenham.html

    i32 r = circle.r;
    i32 x = r * -1;
    i32 y = 0; 
    i32 err = 2 - 2 * r; 

    do {
        softSetPixel(circle.position.x - x, circle.position.y + y, pixel);
        softSetPixel(circle.position.x - y, circle.position.y - x, pixel);
        softSetPixel(circle.position.x + x, circle.position.y - y, pixel);
        softSetPixel(circle.position.x + y, circle.position.y + x, pixel);

        r = err;

        if (r <= y) {
            err += ++y * 2 + 1;
        }

        if (r > x || err > y) {
            err += ++x * 2 + 1;
        }
    } while (x < 0);
}

SAPI void softDrawImage(Image* image, iVec2 position, Pixel tint) {
    softDrawImageExtanded(image, position, softVectorZero(), FLIP_DEFAULT, tint);
}

SAPI void softDrawImageExtanded(Image* image, iVec2 position, iVec2 pivot, SoftImageFlip image_flip, Pixel tint) {
    switch (image_flip) {
        case FLIP_DEFAULT: {
            for(i32 y = 0; y < image->size.y; y++) {
                for(i32 x = 0; x < image->size.x; x++) {
                    softSetPixel(
                        position.x + x - pivot.x, 
                        position.y + y - pivot.y, 
                        softGetPixelFromBuffer(
                            image->data, 
                            (iVec2) { x, y }, 
                            image->size
                        )
                    );
                }
            }

            break;
        }

        case FLIP_H: {
            for(i32 y = 0; y < image->size.y; y++) {
                for(i32 x = image->size.x - 1, pos_x = 0; x >= 0; x--, pos_x++) {
                    softSetPixel(
                        position.x + pos_x - pivot.x, 
                        position.y + y - pivot.y, 
                        softGetPixelFromBuffer(
                            image->data, 
                            (iVec2) { x, y }, 
                            image->size
                        )
                    );
                }
            }
            
            break;
        }

        case FLIP_V: {
            for(i32 y = image->size.y - 1, pos_y = 0; y >= 0; y--, pos_y++) {
                for(i32 x = 0; x < image->size.x; x++) {
                    softSetPixel(
                        position.x + x - pivot.x, 
                        position.y + pos_y - pivot.y, 
                        softGetPixelFromBuffer(
                            image->data, 
                            (iVec2) { x, y }, 
                            image->size
                        )
                    );
                }
            }
            
            break;
        }

        case FLIP_HV: {
            for(i32 y = image->size.y - 1, pos_y = 0; y >= 0; y--, pos_y++) {
                for(i32 x = image->size.x - 1, pos_x = 0; x >= 0; x--, pos_x++) {
                    softSetPixel(
                        position.x + pos_x - pivot.x, 
                        position.y + pos_y - pivot.y, 
                        softGetPixelFromBuffer(
                            image->data, 
                            (iVec2) { x, y }, 
                            image->size
                        )
                    );
                }
            }
            
            break;
        }

        default: {
            softLogWarning("Invalid flip value: %i. Defaulting to value: 0 (FLIP_DEFAULT)...", image_flip);

            for(i32 y = 0; y < image->size.y; y++) {
                for(i32 x = 0; x < image->size.x; x++) {
                    softSetPixel(
                        position.x + x - pivot.x, 
                        position.y + y - pivot.y, 
                        softGetPixelFromBuffer(
                            image->data, 
                            (iVec2) { x, y }, 
                            image->size
                        )
                    );
                }
            }

            break;
        }

    }
}

// ------------------------------------------------------
#pragma endregion
// ------------------------------------------------------

// ------------------------------------------------------
#pragma region SOFT_API_FUNC_LOGGING
// ------------------------------------------------------

SAPI void softLog(SoftLogLevel log_level, const string restrict txt, ...) {
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
            softLogWarning("Invalid log level: %i", (i32)(log_level));
            break;
    }
}


SAPI void softLogInfo(const string restrict txt, ...) {
    char buf[256];

    va_list list;
    va_start(list, txt);

        vsnprintf(buf, sizeof(buf), txt, list);

    va_end(list);

    fprintf(stdout, "[INFO] %s\n", buf);
}

SAPI void softLogWarning(const string restrict txt, ...) {
    char buf[256];

    va_list list;
    va_start(list, txt);

        vsnprintf(buf, sizeof(buf), txt, list);

    va_end(list);

    fprintf(stdout, "[WARN] %s\n", buf);
}

SAPI void softLogError(const string restrict txt, ...) {
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

SAPI const string softTextFormat(const string restrict txt, ...) {
    // Source: https://github.com/raysan5/raylib/blob/master/src/rtext.c#L1408

    #define CHARBUF_COUNT_TOTAL 4

    global char buf[CHARBUF_COUNT_TOTAL][SOFT_CHARBUF_SIZE_MAX] = { 0 };
    global i32 buffer_index = 0;

    string current_buffer = buf[buffer_index];
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

SAPI const bool softTextEmpty(const string restrict txt, ...) {
    return txt[0] != '\0';
}

SAPI const i32 softTextLength(const string restrict txt) {
    return SDL_strlen(txt);
}

// ------------------------------------------------------
#pragma endregion
// ------------------------------------------------------

// ------------------------------------------------------
#pragma region SOFT_API_FUNC_COLOR
// ------------------------------------------------------

SAPI Pixel softGetPixelColor(i32 x, i32 y) {
    if(x < 0 || x >= CORE.Window.screen_size.x || y < 0 || y >= CORE.Window.screen_size.y) {
        return BLACK;
    }

    return CORE.Render.data[y * CORE.Window.display_size.x + x];
}

SAPI Pixel softGetPixelFromBuffer(PixelBuffer buffer, iVec2 position, iVec2 size) {
    if(position.x < 0 || position.x >= size.x || position.y < 0 || position.y >= size.y) {
        return BLACK;
    }

    return buffer[position.y * size.x + position.x];
}

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
    // RR . result.r - we don't need to shift out bits to the left
    // GG . result.g - we need to shift to the left by 8 bits (1 byte = 1 uint8_t)
    // BB . result.b - we need to shift to the left by 16 bits (2 bytes = 2 uint8_t's)
    // AA . result.a - we need to shift to the left by 24 bits (3 bytes = 3 uint8_t's)

    result = 
        color.r | 
        color.g << 8 | 
        color.b << 16 | 
        color.a << 24;

    return result;
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
    // result.r . RR - this is our starting point, so we don't need to shidt our bytes to the right
    // result.g . GG - we need to shift to the right by 8 bits (1 byte = 1 uint8_t)
    // result.b . BB - we need to shift to the right by 16 bits (2 bytes = 2 uint8_t's)
    // result.a . AA - we need to shift to the right by 24 bits (3 bytes = 3 uint8_t's)


    result.r = (pixel >> 8 * 0) & 0xFF;
    result.g = (pixel >> 8 * 1) & 0xFF;
    result.b = (pixel >> 8 * 2) & 0xFF;
    result.a = (pixel >> 8 * 3) & 0xFF;

    return result;
}

SAPI bool softColorCompare(Color a, Color b) {
    return 
        (a.r == b.r) &&
        (a.g == b.g) &&
        (a.b == b.b) &&
        (a.a == b.a);
}

SAPI bool softPixelColorCompare(Pixel a, Pixel b) {
    return a == b;
}

SAPI Color softMixColor(Color base_color, Color return_color, u8 alpha) {
    Color result = return_color;

    // Check if the alpha equals 255.
    // That means the color is opaque, so there's no need to blend it.
    // Not every object on the screen will be opaque, so this saves a lot of computational power.
    if(alpha == 255) {
        return result;
    } else if(alpha == 0) {
        return softPixelToColor(BLANK);
    }

    result.r = base_color.r + (return_color.r - base_color.r) * (alpha / 255.0);
    result.g = base_color.g + (return_color.g - base_color.g) * (alpha / 255.0);
    result.b = base_color.b + (return_color.b - base_color.b) * (alpha / 255.0);
    result.a = alpha;

    return result;
}

SAPI Pixel softMixPixels(Pixel base_pixel, Pixel return_pixel, u8 alpha) {
    Color base = softPixelToColor(base_pixel);
    Color result = softPixelToColor(return_pixel);

    // Check if the alpha equals 255.
    // That means the color is opaque, so there's no need to blend it.
    // Not every object on the screen will be opaque, so this saves a lot of computational power.
    if(alpha == 255) {
        return softColorToPixel(result);
    } else if(alpha == 0) {
        return BLANK;
    }

    result.r = base.r + (result.r - base.r) * (alpha / 255.0);
    result.g = base.g + (result.g - base.g) * (alpha / 255.0);
    result.b = base.b + (result.b - base.b) * (alpha / 255.0);
    result.a = alpha;

    return softColorToPixel(result);
}

SAPI Color softColorFade(Color color, f32 factor) {
    return (Color) {
        color.r,
        color.g,
        color.b,
        factor * 255
    };
}

SAPI Pixel softPixelFade(Pixel pixel, f32 factor) {
    return softColorToPixel(
        softColorFade(
            softPixelToColor(pixel), 
            factor
        )
    );
}

// ------------------------------------------------------
#pragma endregion
// ------------------------------------------------------

// ------------------------------------------------------
#pragma region SOFT_API_FUNC_TIME
// ------------------------------------------------------

SAPI f32 softDeltaTime(void) {
    return CORE.Time.delta_time;
}

SAPI f32 softTime(void) {
    u32 miliseconds = SDL_GetTicks();
    i32 second_size = 1000;

    CORE.Time.application_lifetime = (f32)(miliseconds) / second_size;

    return CORE.Time.application_lifetime;
}

SAPI i32 softFPS(void) {
    return 1 / CORE.Time.delta_time;
}

SAPI void softTargetFPS(u32 framerate) {
    CORE.Time.framerate = framerate;
    CORE.Time.frame_target = 1.0f / framerate;

    softLogInfo("Framerate: %iFPS (%0.04fms)", CORE.Time.framerate, CORE.Time.frame_target);
}

SAPI void softWait(f32 seconds) {
    SDL_Delay(seconds * 1000);
}

SAPI Timer softTimer(const f32 TIME) {
    return (Timer) {
        TIME,
        TIME,
        false
    };
}

SAPI void softTimerProceed(Timer* timer, f32 delta_time) {
    if(!softTimerFinished(timer)) {
        timer->current_time -= delta_time;
    }
}
SAPI bool softTimerFinished(Timer* timer) {
    timer->finished = timer->current_time <= 0.0f;

    return timer->finished;
}

SAPI void softTimerRestart(Timer* timer) {
    timer->current_time = timer->initial_time;
    timer->finished = false;
}

SAPI void softTimerReset(Timer* timer, f32 time) {
    timer->initial_time = timer->initial_time;
    timer->current_time = timer->initial_time;
    timer->finished = false;
}

// ------------------------------------------------------
#pragma endregion
// ------------------------------------------------------

// ------------------------------------------------------
#pragma region SOFT_API_FUNC_MATH
// ------------------------------------------------------

SAPI f32 softLerpF(f32 start, f32 end, f32 t) {
    return start + (end - start) * t;
}

SAPI f32 softPowF(f32 a, f32 n) {
    return pow(a, n);
}

SAPI f32 softSqrF(f32 a) {
    return pow(a, 2);
}

SAPI f32 softSqrtF(f32 a) {
    return sqrt(a);
}

SAPI i32 softLerpI(i32 start, i32 end, f32 t) {
    return start + (end - start) * t;
}

SAPI i32 softPowI(i32 a, i32 n) {
    return pow(a, n);
}

SAPI i32 softSqrI(i32 a) {
    return pow(a, 2);
}

SAPI i32 softSqrtI(i32 a) {
    return sqrt(a);
}

SAPI iVec2 softVectorZero(void) {
    return (iVec2) { 0.0f, 0.0f };
}

SAPI iVec2 softVectorOne(void) {
    return (iVec2) { 1.0f, 1.0f };
}

SAPI iVec2 softVectorUp(void) {
    return (iVec2) { 0.0f, -1.0f };
}

SAPI iVec2 softVectorDown(void) {
    return (iVec2) { 0.0f, 1.0f };
}

SAPI iVec2 softVectorLeft(void) {
    return (iVec2) { -1.0f, 0.0f };
}

SAPI iVec2 softVectorRight(void) {
    return (iVec2) { 1.0f, 0.0f };
}

SAPI iVec2 softVectorAdd(iVec2 a, iVec2 b) {
    return (iVec2) { a.x + b.x, a.y + b.y };
}

SAPI iVec2 softVectorSub(iVec2 a, iVec2 b) {
    return (iVec2) { a.x - b.x, a.y - b.y };
}

SAPI iVec2 softVectorMult(iVec2 a, iVec2 b) {
    return (iVec2) { a.x * b.x, a.y * b.y };
}

SAPI iVec2 softVectorMultFactor(iVec2 a, f32 factor) {
    return (iVec2) { a.x * factor, a.y * factor };
}

SAPI iVec2 softVectorDiv(iVec2 a, iVec2 b) {
    return (iVec2) { a.x / b.x, a.y / b.y };
}

SAPI iVec2 softVectorDivFactor(iVec2 a, f32 factor) {
    return (iVec2) { a.x / factor, a.y / factor };
}

SAPI iVec2 softVectorLerp(iVec2 start, iVec2 end, f32 t) {
    return (iVec2) {
        softLerpI(start.x, end.x, t),
        softLerpI(start.y, end.y, t)
    };
}

SAPI Color softColorZero(void) {
    return (Color) { 0, 0, 0, 0 };
}

SAPI Color softColorOne(void) {
    return (Color) { 1, 1, 1, 1 };
}

SAPI Color softColorAdd(Color a, Color b) {
    return (Color) { 
        a.r + b.r, 
        a.g + b.g, 
        a.b + b.b, 
        a.a + b.a
    };
}

SAPI Color softColorSub(Color a, Color b) {
    return (Color) { 
        a.r - b.r, 
        a.g - b.g, 
        a.b - b.b, 
        a.a - b.a
    };
}

SAPI Color softColorMult(Color a, Color b) {
    return (Color) { 
        a.r * b.r, 
        a.g * b.g, 
        a.b * b.b, 
        a.a * b.a
    };
}

SAPI Color softColorDiv(Color a, Color b) {
    return (Color) { 
        a.r / b.r, 
        a.g / b.g, 
        a.b / b.b, 
        a.a / b.a
    };
}

SAPI Color softColorLerp(Color start, Color end, f32 t) {
    return (Color) { 
        softLerpI(start.r, end.r, t), 
        softLerpI(start.g, end.g, t), 
        softLerpI(start.b, end.b, t), 
        softLerpI(start.a, end.a, t)
    };
}

// ------------------------------------------------------
#pragma endregion
// ------------------------------------------------------

// ------------------------------------------------------
#pragma region SOFT_API_FUNC_IMAGE
// ------------------------------------------------------

SAPI Image softLoadImage(const string path) {
    Image result = { 0 };

    stbi_uc* data = stbi_load(
        path, 
        &result.size.x, 
        &result.size.y, 
        &result.channels, 
        STBI_rgb_alpha
    );

    if(!data) {
        softLogError("Image loading failure: %s", stbi_failure_reason());
        return result;
    }

    result.data = (PixelBuffer)calloc(result.size.x * result.size.y, sizeof(Pixel));

    for(i32 index_source = 0, index_result = 0; index_source < result.size.x * result.size.y * result.channels * sizeof(stbi_uc) && index_result < result.size.x * result.size.y * sizeof(Pixel); index_result++) {
        Color pixel_color = { 0 };
        pixel_color.r = data[index_source++];
        pixel_color.g = data[index_source++];
        pixel_color.b = data[index_source++];
        pixel_color.a = data[index_source++];

        result.data[index_result] = softColorToPixel(pixel_color);
    }

    stbi_image_free(data);

    softLogInfo("Image loaded successfully:");
    softLogInfo("   > resolution: %ix%ipx", result.size.x, result.size.y);
    softLogInfo("   > channels: %i", result.channels);
    softLogInfo("   > size: %i bytes", result.size.x * result.size.y * sizeof(Pixel));

    return result;
}

SAPI void softUnloadImage(Image* image) {
    if(!image->data) {
        softLogWarning("Trying to unload invalid image data.");
        return;
    }

    free(image->data);
    softLogInfo("Image unloaded successfully.");
}

// ------------------------------------------------------
#pragma endregion
// ------------------------------------------------------
