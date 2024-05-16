#include "SDL_pixels.h"
#include "SDL_version.h"
#ifndef SOFT_IMPLEMENTATION

// ------------------------------------------------------
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

// ------------------------------------------------------
#pragma endregion
// ------------------------------------------------------

// ------------------------------------------------------
#pragma region SOFT_INTERNAL_MACROS
// ------------------------------------------------------

#define SOFT_CHARBUF_SIZE_MAX 256

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
            iVec2 position_current;
            iVec2 position_previous;
            iVec2 wheel_move;

            iVec2 offset;
            iVec2 scale;
        } Mouse;

    } Input;

} CORE;

internal void softSetPixel(int x, int y, Pixel pixel) {
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

    CORE.Render.data[y * CORE.Window.display_size.x + x] = pixel;
}

// ------------------------------------------------------
#pragma endregion
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

    CORE.Window.window = SDL_CreateWindow(
        title, 
        SDL_WINDOWPOS_CENTERED, 
        SDL_WINDOWPOS_CENTERED, 
        width, 
        height, 
        SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI
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

    CORE.Render.renderer = SDL_CreateRenderer(
        CORE.Window.window, 
        -1, 
        0
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

SAPI void softPollEvents() {
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

            case SDL_KEYDOWN:

                break;

            case SDL_KEYUP:

                break;

            case SDL_MOUSEMOTION:
                CORE.Input.Mouse.position_previous = CORE.Input.Mouse.position_current;

                CORE.Input.Mouse.position_current.x = event.motion.x;
                CORE.Input.Mouse.position_current.y = event.motion.y;
                
                break;

            case SDL_MOUSEWHEEL:
                CORE.Input.Mouse.wheel_move.x = event.wheel.x;
                CORE.Input.Mouse.wheel_move.y = event.wheel.y;
                
                break;

            case SDL_MOUSEBUTTONDOWN:

                break;

            case SDL_MOUSEBUTTONUP:

                break;
        }
    }
}

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

SAPI bool softKeyPressed(KeyCode key) {
    // TODO(yakub): Implement the input functions for the Keyboard
    // You'll probably need to convert the Soft KeyCode's to the SDL_Scancode's for this to work
    
    return false;
}

SAPI bool softKeyReleased(KeyCode key) {
    // TODO(yakub): Implement the input functions for the Keyboard
    // You'll probably need to convert the Soft KeyCode's to the SDL_Scancode's for this to work
    
    return false;
}

SAPI bool softKeyDown(KeyCode key) {
    // TODO(yakub): Implement the input functions for the Keyboard
    // You'll probably need to convert the Soft KeyCode's to the SDL_Scancode's for this to work
    
    return false;
}

SAPI bool softKeyUp(KeyCode key) {
    // TODO(yakub): Implement the input functions for the Keyboard
    // You'll probably need to convert the Soft KeyCode's to the SDL_Scancode's for this to work
    
    return false;
}

SAPI iVec2 softGetWindowSize(){
    return CORE.Window.screen_size;
}

SAPI iVec2 softGetDisplaySize(){
    return CORE.Window.display_size;
}

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

#endif // SOFT_IMPLEMENTATION