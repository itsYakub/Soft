#include "soft.h"

int main(int argc, char** argv) {
    softInit(1024, 768, softTextFormat("Soft %s", SOFT_VERSION));

    while(!softWindowShoulClose()) {
        softClearBufferColor(WHITE);
        softDrawCircle((Circle) { softGetWindowCenter(), 100 }, RED);

        softBlit();
    }

    softClose();

    return 0;
}