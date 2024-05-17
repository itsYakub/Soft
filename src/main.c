#include "soft.h"

int main(int argc, char** argv) {
    softSetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_HIGHDPI | FLAG_WINDOW_VSYNC);
    softInit(1024, 768, softTextFormat("Soft %s", SOFT_VERSION));

    iVec2 position = softGetWindowCenter();

    while(!softWindowShoulClose()) {
        if(softMouseButtonDown(BUTTON_LEFT)) {
            position = softGetMousePosition();
        }

        softClearBufferColor(WHITE);

        softDrawCircle((Circle) { position, 100 }, RED);

        softBlit();
        softPollEvents();
    }

    softClose();

    return 0;
}
