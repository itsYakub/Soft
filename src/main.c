#include "soft.h"

int main(int argc, char** argv) {
    softSetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_HIGHDPI);
    softInit(1024, 768, softTextFormat("Soft %s", SOFT_VERSION));

    while(!softWindowShoulClose()) {
        softClearBufferColor(WHITE);

        softDrawRectangleExtanded((Rect) { softGetWindowCenter(), { 100, 100 } }, (iVec2) { 50, 50 }, RED);
        softDrawRectangleExtanded((Rect) { softGetMousePosition(), { 100, 100 } }, (iVec2) { 50, 50 }, softColorFade(GREEN, 0.5f));

        softBlit();
        softPollEvents();
    }

    softClose();

    return 0;
}
