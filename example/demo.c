#include "soft.h"

int main(int argc, char** argv) {
    softSetConfigFlags(FLAG_WINDOW_VSYNC);
    softInit(1024, 768, softTextFormat("Soft %s", SOFT_VERSION));

    Image image = softLoadImage("../example/res/turtle.png");
    float time = 0.0f;

    while(!softWindowShoulClose()) {
        time += softDeltaTime();

        softClearBufferColor(WHITE);

        if((int) time % 2 == 0) {
            softDrawImageExtanded(
                &image, 
                softGetWindowCenter(),
                softVectorDivFactor(image.size, 2.0f), 
                FLIP_DEFAULT,
                WHITE
            );
        } else{
            softDrawImageExtanded(
                &image, 
                softGetWindowCenter(),
                softVectorDivFactor(image.size, 2.0f), 
                FLIP_H,
                WHITE
            );
        }

        softBlit();
    }

    softUnloadImage(&image);
    softClose();

    return 0;
}
