#include "soft.h"

int main(int argc, char** argv) {
    softInit(1024, 768, softTextFormat("Soft %s", SOFT_VERSION));

    Image image = softLoadImage("../example/res/turtle.png");

    while(!softWindowShoulClose()) {
        softClearBufferColor(WHITE);
        
        softDrawImageEx(
            &image, 
            softGetWindowCenter(),
            softVectorDivFactor(image.size, 2.0f), 
            FLIP_DEFAULT,
            WHITE
        );

        softBlit();
    }

    softUnloadImage(&image);
    softClose();

    return 0;
}