#include "soft.h"

int main(int argc, char** argv) {
    softSetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_VSYNC);
    softInit(1024, 768, softTextFormat("Soft %s", SOFT_VERSION));

    Image image = softLoadImage("../example/res/turtle.png");

    while(!softWindowShoulClose()) {
        softClearBufferColor(WHITE);

        softDrawImageExtanded(
            &image, 
            softGetWindowCenter(),
            softVectorDivFactor(image.size, 2.0f), 
            WHITE
        );

        softBlit();
    }

    softUnloadImage(&image);
    softClose();

    return 0;
}
