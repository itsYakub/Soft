#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "SDL.h"
#include "SDL_events.h"
#include "SDL_mouse.h"
#include "SDL_stdinc.h"
#include "SDL_video.h"

#define VERSION "1.0"
#define VERSION_MAJOR 1
#define VERSION_MINOR 0

#define RED 0xFF0000FF
#define GREEN 0x00FF00FF
#define BLUE 0x0000FFFF
#define YELLOW 0xFFFF00FF
#define PINK 0xFF00FFFF
#define WHITE 0xFFFFFFFF
#define BLACK 0xFF000000

typedef uint32_t Pixel;
typedef struct { int x; int y; } iVec2;
typedef struct { int x; int y; int w; int h; } Rect;
typedef struct { int x; int y; int r; } Circle;
typedef struct { iVec2 a; iVec2 b; } Line;

struct {
    struct {
        SDL_Window* window;

        char* title;

        iVec2 display_size;
        iVec2 screen_size;

        bool quit;
        bool cursor_on_screen;
    } Window;

    struct {
        Pixel* render_data;
        SDL_Renderer* renderer;
        SDL_Texture* render_texture;
    } Render;

    struct {
        struct {
            iVec2 mouse_position;
            iVec2 previous_mouse_position;

            iVec2 offset;
            iVec2 scale;
        } Mouse;

    } Input;

} CORE;

int InitWindow(int width, int height, const char* title);
void CloseWindow();

void PollEvents();

iVec2 GetMousePosition();
iVec2 GetPreviousMousePosition();
iVec2 GetMouseDelta();

iVec2 GetWindowSize();
iVec2 GetDisplaySize();

void ClearBuffer();
void ClearColor(Pixel color);
void SetPixelColor(int x, int y, Pixel color);
void Blit();

void DrawRectangle(Rect rect, Pixel color);
void DrawRectangleLines(Rect rect, Pixel color);
void DrawLine(Line line, Pixel color);
void DrawCircle(Circle circle, Pixel color);
void DrawCircleLines(Circle circle, Pixel color);

void SetWindowTitle(const char* title);
bool WindowShoulClose();
void CloseWindowCallback();

void LogInfo(const char *restrict txt, ...);
void LogWarning(const char *restrict txt, ...);
void LogError(const char *restrict txt, ...);

int main(int argc, char** argv) {
    InitWindow(1024, 768, "Hello SDL2!");

    Rect rect = { 100, 100, 100, 100 };
    Circle circle = { 150, 300, 50 };

    while(!WindowShoulClose()) {
        PollEvents();

        ClearColor(WHITE);

        DrawRectangle(rect, RED);
        DrawRectangleLines((Rect) { rect.x + 150, rect.y, rect.w, rect.h }, RED);

        DrawCircle(circle, RED);
        DrawCircleLines((Circle) { circle.x + 150, circle.y, circle.r }, RED);

        Blit();
    }

    CloseWindow();

    return 0;
}

int InitWindow(int width, int height, const char* title) { 
    LogInfo("Initializing v.%s", VERSION);

    int init = SDL_Init(SDL_INIT_VIDEO);

    if(init != 0) {
        LogError("Couldn't initialize SDL. Returning...");
        
        return 1;
    }

    SDL_DisplayMode mode;
    SDL_GetCurrentDisplayMode(0, &mode);
    CORE.Window.display_size = (iVec2) { mode.w, mode.h };

    CORE.Window.window = SDL_CreateWindow(
        title, 
        SDL_WINDOWPOS_CENTERED, 
        SDL_WINDOWPOS_CENTERED, 
        width, 
        height, 
        SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI
    );

    if(!CORE.Window.window) {
        LogError("Couldn't create the Window. Returning...");
        
        return 1;
    }

    CORE.Render.renderer = SDL_CreateRenderer(
        CORE.Window.window, 
        -1, 
        0
    );

    if(!CORE.Render.renderer) {
        LogError("Couldn't create the Renderer. Returning...");

        SDL_DestroyWindow(CORE.Window.window);
        
        return 1;
    }

    CORE.Render.render_texture = SDL_CreateTexture(
        CORE.Render.renderer, 
        SDL_PIXELFORMAT_RGBA8888, 
        SDL_TEXTUREACCESS_STREAMING, 
        CORE.Window.display_size.x, 
        CORE.Window.display_size.y
    );

    if(!CORE.Render.render_texture) {
        LogError("Couldn't create the Render Texture. Returning...");

        SDL_DestroyWindow(CORE.Window.window);
        SDL_DestroyRenderer(CORE.Render.renderer);
        
        return 1;
    }

    CORE.Render.render_data = calloc(CORE.Window.display_size.x * CORE.Window.display_size.y, sizeof(Pixel));

    if(!CORE.Render.render_data) {
        LogError("Couldn't allocate the Render Data. Returning...");

        SDL_DestroyWindow(CORE.Window.window);
        SDL_DestroyRenderer(CORE.Render.renderer);
        SDL_DestroyTexture(CORE.Render.render_texture);
        
        return 1;
    }

    CORE.Window.screen_size = (iVec2) { width, height };
    CORE.Window.quit = false;

    CORE.Input.Mouse.offset = (iVec2) { 0 };
    CORE.Input.Mouse.scale = (iVec2) { 1, 1 };

    return 0;
}

void CloseWindow() {
    SDL_free(CORE.Render.render_data);
    SDL_DestroyTexture(CORE.Render.render_texture);
    SDL_DestroyRenderer(CORE.Render.renderer);
    SDL_DestroyWindow(CORE.Window.window);
    SDL_Quit();
}

void PollEvents() {
    SDL_Event event = { 0 };
    while(SDL_PollEvent(&event)) {
        switch(event.type) {
            case SDL_WINDOWEVENT:
                switch (event.window.event) {
                    case SDL_WINDOWEVENT_CLOSE:
                        CloseWindowCallback();
                        
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

            case SDL_MOUSEBUTTONDOWN:

                break;

            case SDL_MOUSEBUTTONUP:

                break;

            case SDL_MOUSEMOTION:
                CORE.Input.Mouse.previous_mouse_position = CORE.Input.Mouse.mouse_position;

                CORE.Input.Mouse.mouse_position.x = event.motion.x;
                CORE.Input.Mouse.mouse_position.y = event.motion.y;
                
                break;
        }
    }
}

iVec2 GetMousePosition() {
    return (iVec2) {
        (CORE.Input.Mouse.mouse_position.x + CORE.Input.Mouse.offset.x) * CORE.Input.Mouse.scale.x,
        (CORE.Input.Mouse.mouse_position.y + CORE.Input.Mouse.offset.y) * CORE.Input.Mouse.scale.y
    };
}

iVec2 GetPreviousMousePosition() {
    return (iVec2) {
        (CORE.Input.Mouse.previous_mouse_position.x + CORE.Input.Mouse.offset.x) * CORE.Input.Mouse.scale.x,
        (CORE.Input.Mouse.previous_mouse_position.y + CORE.Input.Mouse.offset.y) * CORE.Input.Mouse.scale.y
    };
}

iVec2 GetMouseDelta() {
    return (iVec2) {
        CORE.Input.Mouse.mouse_position.x - CORE.Input.Mouse.previous_mouse_position.x,
        CORE.Input.Mouse.mouse_position.y - CORE.Input.Mouse.previous_mouse_position.y
    };
}

iVec2 GetWindowSize(){
    return CORE.Window.screen_size;
}

iVec2 GetDisplaySize(){
    return CORE.Window.display_size;
}

void ClearBuffer() {
    SDL_memset(CORE.Render.render_data, 0, CORE.Window.display_size.x * CORE.Window.display_size.y * sizeof(Pixel));
}

void ClearColor(Pixel color) {
    SDL_memset(CORE.Render.render_data, color, CORE.Window.display_size.x * CORE.Window.display_size.y * sizeof(Pixel));
}

void SetPixelColor(int x, int y, Pixel color) {
    if(x < 0 || x >= CORE.Window.screen_size.x) { 
        return; 
    }

    if(y < 0 || y >= CORE.Window.screen_size.y) { 
        return; 
    }


    CORE.Render.render_data[y * CORE.Window.display_size.x + x] = color;
}

void Blit() {
    SDL_UpdateTexture(
        CORE.Render.render_texture,
        NULL,
        CORE.Render.render_data, 
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
        CORE.Render.render_texture, 
        NULL, 
        &destination_rect,
        0.0,
        NULL,
        SDL_FLIP_NONE
    );

    SDL_RenderPresent(CORE.Render.renderer);
}

void DrawRectangle(Rect rect, Pixel color) {
    for(int i = 0; i < rect.h; i++) {
        for(int j = 0; j < rect.w; j++) {
            SetPixelColor( rect.x + j, rect.y + i, color);
        }
    }
}

void DrawRectangleLines(Rect rect, Pixel color) {
    Line rect_lines[4]  = {
        { (iVec2) { rect.x, rect.y }, (iVec2) { rect.x + rect.w, rect.y } },
        { (iVec2) { rect.x, rect.y }, (iVec2) { rect.x, rect.y + rect.h} },
        { (iVec2) { rect.x + rect.w, rect.y }, (iVec2) { rect.x + rect.w, rect.y + rect.h } },
        { (iVec2) { rect.x, rect.y + rect.h }, (iVec2) { rect.x + rect.w, rect.y + rect.h } },
    };

    for(int i = 0; i < 4; i++) {
        DrawLine(rect_lines[i], color);
    }
}

void DrawLine(Line line, Pixel color) {
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
        SetPixelColor(x, y, color);
        x += dx;
        y += dy;
        i++;
    }
}

void DrawCircle(Circle circle, Pixel color) {
    // Source: https://stackoverflow.com/questions/1201200/fast-algorithm-for-drawing-filled-circles/14976268#14976268

    int x = circle.r;
    int y = 0;
    int xChange = 1 - (circle.r << 1);
    int yChange = 0;
    int radiusError = 0;

    while (x >= y)
    {
        for (int i = circle.x - x; i <= circle.x + x; i++)
        {
            SetPixelColor(i, circle.y + y, color);
            SetPixelColor(i, circle.y - y, color);
        }
        for (int i = circle.x - y; i <= circle.x + y; i++)
        {
            SetPixelColor(i, circle.y + x, color);
            SetPixelColor(i, circle.y - x, color);
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

void DrawCircleLines(Circle circle, Pixel color) {
    // Source: https://zingl.github.io/bresenham.html

    int r = circle.r;
    int x = r * -1;
    int y = 0; 
    int err = 2 - 2 * r; 

    do {
        SetPixelColor(circle.x - x, circle.y + y, color);
        SetPixelColor(circle.x - y, circle.y - x, color);
        SetPixelColor(circle.x + x, circle.y - y, color);
        SetPixelColor(circle.x + y, circle.y + x, color);

        r = err;

        if (r <= y) {
            err += ++y*2+1;
        }

        if (r > x || err > y) {
            err += ++x*2+1;
        }
    } while (x < 0);
}

void SetWindowTitle(const char* title) {
    SDL_SetWindowTitle(CORE.Window.window, title);
    strcpy(CORE.Window.title, title);
}

bool WindowShoulClose() {
    return CORE.Window.quit;
}

void CloseWindowCallback() {
    CORE.Window.quit = true;
}

void LogInfo(const char *restrict txt, ...) {
    char buf[256];

    va_list list;
    va_start(list, txt);

        vsnprintf(buf, sizeof(buf), txt, list);

    va_end(list);

    fprintf(stdout, "[INFO] %s\n", buf);
}

void LogWarning(const char *restrict txt, ...) {
    char buf[256];

    va_list list;
    va_start(list, txt);

        vsnprintf(buf, sizeof(buf), txt, list);

    va_end(list);

    fprintf(stdout, "[WARN] %s\n", buf);
}

void LogError(const char *restrict txt, ...) {
    char buf[256];

    va_list list;
    va_start(list, txt);

        vsnprintf(buf, sizeof(buf), txt, list);

    va_end(list);

    fprintf(stderr, "[ERR] %s\n", buf);
}
