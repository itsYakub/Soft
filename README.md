<div align="center">

# Soft

**Simple 2D Real-Time Graphics Library**

</div>

## Introduction
This project was created purely for learning purposes. It allows me to get the deeper knowledge of how to graphics programming work under the hood. The big source of information was [raylib](https://github.com/raysan5/raylib), a simple and easy-to-use library to enjoy videogames programming. All of the external sources I've used are mentioned as the coude blocks in the `soft.c` implementation file. My main inspirations were:
- ["olive.c" by Tsoding.](https://www.youtube.com/playlist?list=PLpM-Dvs8t0Va-Gb0Dp4d9t8yvNFHaKH6N)
- ["Programming a first person shooter from scratch like it's 1995" by Jdh.](https://www.youtube.com/watch?v=fSjc8vLMg8c)

## Example

```c
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
```

## Setup

### 0. Prerequesites:
- **[Git](https://git-scm.com/);**
- **[CMake](https://cmake.org/);**

### 1. Dependencies:
*(NOTE: All of the external dependencies should be fetched by CMake using FetchContent)*
- **[SDL2](https://www.libsdl.org/);**

### 2. Setup:
- Clone this repository:
    ```console
    $ git clone https://github.com/itsYakub/Soft.git --branch=master --depth=1 
    $ cd Soft/
    ```

- Create a binary directory:
    ```console
    $ mkdir bin
    $ cd bin/
    ```

- Compile the project using **CMake**:
    ```console
    $ cmake ..
    $ cmake --build .
    ```

- Run the executable:
    ```console
    $ ./example/demo.out
    ```

## Credits
- **[Differential Line Algorithm's `Wikipedia` page](https://en.wikipedia.org/wiki/Digital_differential_analyzer_(graphics_algorithm));**
- **[Filled circle drawing algorithm on `Stack Overflow`](https://stackoverflow.com/questions/1201200/fast-algorithm-for-drawing-filled-circles/14976268#14976268);**
- **[Raylib's `TextFormat` solution](https://github.com/raysan5/raylib/blob/master/src/rtext.c#L1408);**
- **[Bresenham's algorithms](https://zingl.github.io/bresenham.html);**
- **[olive.c](https://github.com/tsoding/olive.c)**
## Licence
This project is under the **[MIT Licence]()**.