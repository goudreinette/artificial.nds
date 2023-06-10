#include <stdio.h>
#include <nds.h>
#include <nf_lib.h>
#include <filesystem.h>


void imgdraw_test() {

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
    NF_Set2D(1, 5);

    // Initialize bitmap backgrounds system
    NF_InitBitmapBgSys(0, 1);
    NF_InitBitmapBgSys(1, 1);

    // Initialize storage buffers
    NF_Init16bitsBgBuffers();

    // Initialize backbuffers
    NF_Init16bitsBackBuffer(0);
    NF_Init16bitsBackBuffer(1);

    // Enable backbuffers
    NF_Enable16bitsBackBuffer(0);
    NF_Enable16bitsBackBuffer(1);

    // Load bitmap files from NitroFS
    NF_Load16bitsBg("bmp/bitmap16", 0);
    NF_Load16bitsImage("bmp/blueball", 1, 64, 64);

    // Load image to VRAM of both screens
    NF_Copy16bitsBuffer(0, 1, 0);
    NF_Copy16bitsBuffer(1, 1, 0);

    // Variables
    s16 x = 0;
    s16 y = 0;
    s8 ix = 1;
    s8 iy = 1;

    while (true)
    {
        // On touch, clear the screen
        scanKeys();
        if (keysDown() & KEY_TOUCH) {
            NF_Copy16bitsBuffer(0, 1, 0); // Background
        }


        // Move ball
        x = utils::rrandom(256);
        y = utils::rrandom(192);

        // Redraw top screen
        NF_Draw16bitsImage(0, 1, x, y, true); // Ball (magenta is transparent)

        // Redraw bottom screen
        NF_Copy16bitsBuffer(1, 1, 0); // Background
        NF_Draw16bitsImage(1, 1, x, y, true); // Ball (magenta is transparent)

        // Wait for the screen refresh
        swiWaitForVBlank();

        // Send backbuffers to the screen
        NF_Flip16bitsBackBuffer(0);
        NF_Flip16bitsBackBuffer(1);
    }

}

