#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
#include <stdio.h>
#include <nds.h>
#include <nf_lib.h>
#include "drawing_functions.h"
#include <filesystem.h>
#include <vector>

std::vector<Point> points;


void imgdraw_test_perlin() {

    // Prepare a NitroFS initialization screen
    NF_Set2D(0, 0);
    NF_Set2D(1, 0);
    consoleDemoInit();
    printf("\n NitroFS init. Please wait.\n\n");
    printf(" Iniciando NitroFS,\n por favor, espere.\n\n");
    swiWaitForVBlank();

    // Initialize NitroFS and set it as the root folder of the filesystem
    nitroFSInit(nullptr);
    NF_SetRootFolder("NITROFS");

    // Initialize 2D engine in both screens and use mode 5
    NF_Set2D(0, 5);

    // Initialize bitmap backgrounds system
    NF_InitBitmapBgSys(0, 1);

    // Initialize storage buffers
    NF_Init16bitsBgBuffers();

    // Initialize backbuffers
    NF_Init16bitsBackBuffer(0);

    // Enable backbuffers
    NF_Enable16bitsBackBuffer(0);

    // Load bitmap files from NitroFS
    NF_Load16bitsBg("bg/clear", 0);
    NF_Load16bitsImage("graphicsparts/circlesmall", 1, 16, 16);
    NF_Load16bitsImage("graphicsparts/pixel", 2, 16, 16);

    // Load image to VRAM of both screens
    NF_Copy16bitsBuffer(0, 1, 0);

    // Variables
    int t = 0;
    int x = SCREEN_WIDTH / 2;
    int y = SCREEN_HEIGHT / 2;


    while (true)
    {
        t++;

        scanKeys();
        if (keysDown() & KEY_SELECT) {
            // On touch, clear the screen
            NF_Copy16bitsBuffer(0, 1, 0); // Background
            // read touch
        }

//        if (keysDown() & KEY_START) {
//            break;
//        }

        touchPosition touch;
        touchRead(&touch);

        x = touch.px;
        y = touch.py;


        // Move ball
//        x += utils::rrandom(5) - 2;
//        y += utils::rrandom(5) - 2;
//        generate_random_polygon(points, x, y, 50, 50, 7);
//        draw_polygon(points);

//        draw_pixel(x, y);
//        draw_line(10, 10, x, y);
        draw_filled_circle(x, y, 10);
//        draw_filled_rectangle(x, y, x + 20, y + 30);
        draw_circle(x, y, 10);
        printf("x: %d, y: %d\n", x, y);
        draw_rectangle(x, y, x + 20, y + 30);

        // Redraw top screen
//        NF_Draw16bitsImage(0, 1, x, y, true); // Ball (magenta is transparent)
//        draw_pixel(x, y); // Ball (magenta is transparent)

        // Wait for the screen refresh
        swiWaitForVBlank();

        // Send backbuffers to the screen
        NF_Flip16bitsBackBuffer(0);
    }
}



#pragma clang diagnostic pop