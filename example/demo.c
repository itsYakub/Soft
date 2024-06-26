#include "soft.h"

int main(int argc, char** argv) {
    softInit(1024, 768, softTextFormat("Soft %s", SOFT_VERSION));

    while(!softWindowShoulClose()) {
        softClearBufferColor(WHITE);

        softDrawRectangle((Rect) { softVectorSub(softGetWindowCenter(), (iVec2) { 64, 64 }), { 128, 128 } }, RED);
        
        softBlit();
    }

    softUnloadResources();
    softClose();

    return 0;
}
