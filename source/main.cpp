#include <nds.h>
#include <stdio.h> // NOLINT(modernize-deprecated-headers)
#include <string>
#include <vector>
#include <filesystem.h>
#include <nf_lib.h>
#include <random>


// My files
#include "text_console.h"
#include "instructions.h"
#include "view.h"
#include "gui_sprites.h"
#include "backgrounds.h"
#include "imgdraw_test.h"




void initialize_top_screen() {
    // Initialize bitmap backgrounds system
    NF_InitBitmapBgSys(0, 1);

    // Initialize storage buffers
    NF_Init16bitsBgBuffers();

    // Initialize backbuffers
    NF_Init16bitsBackBuffer(0);

    // Enable backbuffers
    NF_Enable16bitsBackBuffer(0);

    // Load bitmap files from NitroFS
    NF_Load16bitsBg("bg/splash", 0);
    NF_Load16bitsBg("bg/clear", 1);

    // Load image to VRAM of both screens
    NF_Copy16bitsBuffer(0, 1, 0);

    NF_Flip16bitsBackBuffer(0);
}



void initialize_bottom_screen() {
    // Initialize tiled backgrounds system
    NF_InitTiledBgBuffers();    // Initialize storage buffers
    NF_InitTiledBgSys(1);       // Bottom screen

    // Initialize sprite system
    NF_InitSpriteBuffers();     // Initialize storage buffers
    NF_InitSpriteSys(1);        // Bottom screen
}



void initialize_screens() {
    // Initialize 2D engine in both screens and use mode 0
    NF_Set2D(0, 5);
    NF_Set2D(1, 5);

    consoleDemoInit();
    printf("\n NitroFS init. Please wait.\n\n");
    swiWaitForVBlank();

    // Initialize NitroFS and set it as the root folder of the filesystem
    nitroFSInit(NULL);
    NF_SetRootFolder("NITROFS");

    initialize_top_screen();
    initialize_bottom_screen();
}



int main(int argc, char **argv) {
    initialize_screens();

    gui_sprites::setup_all();
    backgrounds::setup_all();
    text_console::setup();

    // Start 
    view::go_to_parameters_screen();

    while (true) {
        scanKeys(); // Read keypad
        u16 keys = keysHeld(); // Keys currently pressed

        touchPosition touch;
        touchRead(&touch);

        bool inside_back_button = gui_sprites::touch_inside_sprite(touch, 3, 3, 16, 16);
        bool inside_run_button = gui_sprites::touch_inside_sprite(touch, SCREEN_WIDTH - 32 - 3, 3, 32, 16);

        if (inside_run_button) {
            view::run();
        }

        if (inside_back_button) {
            view::go_to_parameters_screen();
        }


        // Update the screen        
        // NF_ShowSprite(1, GENERATE_BUTTON, current_screen == PARAMETERS);
        NF_ShowSprite(1, gui_sprites::BACK_BUTTON, view::current_view == view::RUNNING);
        NF_ShowSprite(1, gui_sprites::SLIDER_TRACK, view::current_view == view::PARAMETERS);

        // Update OAM array
        NF_SpriteOamSet(0);
        NF_SpriteOamSet(1);

        // Wait for the screen refresh
        swiWaitForVBlank();

        // Update OAM
        oamUpdate(&oamMain);
        oamUpdate(&oamSub);
    }

    return 0;
}






