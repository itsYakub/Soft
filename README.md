<div align="center">

# Soft

**Simple 2D Real-Time Graphics Library**

</div>


## Example

```c
#include "soft.h"

int main(int argc, char** argv) {
    softInit(1024, 768, "Hello, soft!");

    while(!softWindowShoulClose()) {
        softClearBufferColor(WHITE);

        softDrawCircle((Circle) { softGetWindowCenter(), 100 }, RED);

        softBlit();
        softPollEvents();
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
- **[SDL2](https://www.libsdl.org/);**

### 2. Setup:
- Clone this repository:
    ```console
    $ git clone https://github.com/itsYakub/Soft.git --branch=master --depth=1 
    $ cd ./Soft
    ```

- Create a binary directory:
    ```console
    $ mkdir bin
    $ cd bin
    ```

- Compile the project using **CMake**:
    ```console
    $ cmake ..
    $ cmake --build .
    ```

- Run the executable:
    ```console
    $ ./demo
    ```

## Credits
- **[Differential Line Algorithm's `Wikipedia` page](https://en.wikipedia.org/wiki/Digital_differential_analyzer_(graphics_algorithm));**
- **[Filled circle drawing algorithm on `Stack Overflow`](https://stackoverflow.com/questions/1201200/fast-algorithm-for-drawing-filled-circles/14976268#14976268);**
- **[Raylib's `TextFormat` solution](https://github.com/raysan5/raylib/blob/master/src/rtext.c#L1408);**
- **[Bresenham's algorithms](https://zingl.github.io/bresenham.html);**

## Licence
This project is under the **[MIT Licence]()**.