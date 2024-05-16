#include "soft.h"

int main(int argc, char** argv) {
    softInit(1024, 768, softTextFormat("Soft %s", SOFT_VERSION));

    while(!softWindowShoulClose()) {
        softPollEvents();

        softClearBufferColor(BLUE);

        softDrawCircle((Circle) { softGetMousePosition(), 50 }, RED);

        softDrawLine((Line) { { 0 }, softGetMousePosition() }, GREEN);
        softDrawLine((Line) { { 0, softGetWindowSize().y }, softGetMousePosition() }, GREEN);
        softDrawLine((Line) { { softGetWindowSize().x, 0 }, softGetMousePosition() }, GREEN);
        softDrawLine((Line) { softGetWindowSize(), softGetMousePosition() }, GREEN);

        softBlit();
    }

    softClose();

    return 0;
}
