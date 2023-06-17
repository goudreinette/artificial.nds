#include <nds.h>
#include <stdio.h> // NOLINT(modernize-deprecated-headers)
#include <string>
#include <vector>
#include <filesystem.h>
#include <nf_lib.h>
#include <random>



/*
|--------------------------------------------------------------------------
| Utils
|--------------------------------------------------------------------------
*/
namespace utils {
    int rrandom(int range) {
        return rand() % range;
    }
}


/*
|--------------------------------------------------------------------------
| Drawing functions
|--------------------------------------------------------------------------
*/
namespace draw {
    /**
     * If true, the backbuffer will be sent to the screen very often,
     * which will slow down rendering and let you see every step
     */
    const bool SLOWMO = true;


    void draw_pixel(int x, int y) {
        NF_Draw16bitsImage(0, 2, x, y, true); // Ball (magenta is transparent)
    }


    void draw_line(int x1, int y1, int x2, int y2) {
        int dx = abs(x2 - x1);
        int dy = abs(y2 - y1);
        int sx = (x1 < x2) ? 1 : -1;
        int sy = (y1 < y2) ? 1 : -1;
        int err = dx - dy;

        while (true) {
            draw_pixel(x1, y1);

            if (x1 == x2 && y1 == y2) {
                break;
            }

            int e2 = 2 * err;
            if (e2 > -dy) {
                err -= dy;
                x1 += sx;
            }
            if (e2 < dx) {
                err += dx;
                y1 += sy;
            }

            if (SLOWMO) NF_Flip16bitsBackBuffer(0);
        }
    }

    void draw_rectangle(int x1, int y1, int x2, int y2) {
        for (int x = x1; x <= x2; x++) {
            draw_pixel(x, y1); // Top side
            draw_pixel(x, y2); // Bottom side

            if (SLOWMO) NF_Flip16bitsBackBuffer(0);
        }


        for (int y = y1; y <= y2; y++) {
            draw_pixel(x1, y); // Left side
            draw_pixel(x2, y); // Right side

            if (SLOWMO) NF_Flip16bitsBackBuffer(0);
        }


    }

    void draw_filled_rectangle(int x1, int y1, int x2, int y2) {
        for (int y = y1; y <= y2; y++) {
            for (int x = x1; x <= x2; x++) {
                draw_pixel(x, y);
            }

            if (SLOWMO) NF_Flip16bitsBackBuffer(0);
        }
    }

    void draw_circle(int center_x, int center_y, int radius) {
        int x = 0;
        int y = radius;
        int d = 3 - 2 * radius;

        while (x <= y) {
            draw_pixel(center_x + x, center_y + y); // Octant 1
            draw_pixel(center_x - x, center_y + y); // Octant 2
            draw_pixel(center_x + x, center_y - y); // Octant 8
            draw_pixel(center_x - x, center_y - y); // Octant 7
            draw_pixel(center_x + y, center_y + x); // Octant 3
            draw_pixel(center_x - y, center_y + x); // Octant 4
            draw_pixel(center_x + y, center_y - x); // Octant 6
            draw_pixel(center_x - y, center_y - x); // Octant 5

            if (SLOWMO) NF_Flip16bitsBackBuffer(0);

            if (d < 0) {
                d += 4 * x + 6;
            } else {
                d += 4 * (x - y) + 10;
                y--;
            }
            x++;
        }
    }

    void draw_filled_circle(int center_x, int center_y, int radius) {
        int x = 0;
        int y = radius;
        int d = 1 - radius;

        while (x <= y) {
            for (int i = center_x - x; i <= center_x + x; i++) {
                draw_pixel(i, center_y + y); // Upper half
                draw_pixel(i, center_y - y); // Lower half
            }

            for (int i = center_x - y; i <= center_x + y; i++) {
                draw_pixel(i, center_y + x); // Right half
                draw_pixel(i, center_y - x); // Left half
            }

            if (SLOWMO) NF_Flip16bitsBackBuffer(0);

            x++;

            if (d < 0) {
                d += 2 * x + 1;
            } else {
                y--;
                d += 2 * (x - y) + 1;
            }
        }
    }

    struct Point {
        int x;
        int y;
    };
}



/*
|--------------------------------------------------------------------------
| Parameters
|--------------------------------------------------------------------------
*/
// All values are 0-128
struct Parameters {
    int scale = 64;
    int repetitions = 64;
    int spikyness = 64;
    int colourfulness = 64;
    int wiggliness = 64;
} parameters;


/*
|--------------------------------------------------------------------------
| Instructions
|--------------------------------------------------------------------------
*/
namespace instructions {
    std::vector<std::string> instructions = {};

    int instructions_length = 100;
    int instructions_index = 0;

    /**
     * Generates the instructions for the current parameters
     */
    void generate() {
        std::vector<std::string> code = {};

        // circles
        for (int c = 0; c < utils::rrandom(5); c++) {
            code.push_back(
                    "(circle " + std::to_string(utils::rrandom(SCREEN_WIDTH)) + " " +
                    std::to_string(utils::rrandom(SCREEN_HEIGHT)) +
                    " " + std::to_string(utils::rrandom(SCREEN_WIDTH)) + " " + std::to_string(utils::rrandom(10)) +
                    ")");
        }

        // lines
        for (int l = 0; l < utils::rrandom(5); l++) {
            code.push_back(
                    "(line " + std::to_string(utils::rrandom(SCREEN_WIDTH)) + " " +
                    std::to_string(utils::rrandom(SCREEN_HEIGHT)) +
                    " " + std::to_string(utils::rrandom(SCREEN_WIDTH)) + " " +
                    std::to_string(utils::rrandom(SCREEN_HEIGHT)) + ")");
        }

        instructions = code;
    }
}




/*
|--------------------------------------------------------------------------
| GUI Sprites  (all indexes)
|--------------------------------------------------------------------------
*/
namespace gui_sprites {
    const int PALETTE_NUM = 0;

    // This is a 'global' because sprite IDs need to be globally unique
    enum GUI_SPRITE_IDS {
        GENERATE_BUTTON,
        BACK_BUTTON,
        SLIDER_KNOB,
        ABOUT_BUTTON,
    };


    // This palette is shared between most sprites.
    // Exceptions are loaded in the individual scenes.
    void setup_main_palette() {
        /**
         * Load sprite palette from NitroFS
         */
        NF_LoadSpritePal("sprite/generatebutton", PALETTE_NUM);

        /**
         * Transfer the required sprite palette to VRAM
         */
        NF_VramSpritePal(1, gui_sprites::PALETTE_NUM, gui_sprites::PALETTE_NUM);
    }

    bool touch_inside_sprite(touchPosition touch, int x, int y, int width, int height) {
        return touch.px > x && touch.px < x + width && touch.py > y && touch.py < y + height;
    }
}


/*
|--------------------------------------------------------------------------
| Scenes
|--------------------------------------------------------------------------
*/

enum NEXT_ACTION {
    NONE,
    GENERATE,
    BACK,
    SHOW_ABOUT
};


// create an abstract class 'Scene' that will be the base for all scenes, with funtions 'enter', 'leave' and 'update'
class Scene {
public:
    virtual void setup() = 0;

    virtual void enter() = 0;

    virtual NEXT_ACTION update() = 0;

    virtual void leave() = 0;
};


// create a class for each scene, inheriting from the abstract class 'Scene'
class ParametersScene : public Scene {
private:
    int about_countdown = 60;

public:
    enum SLIDER_KNOBS {
        SCALE = 7,
        REPETITIONS = 8,
        SPIKYNESS = 9,
        COLOURFULNESS = 10,
        WIGGLINESS = 11
    };

    const int KNOB_Y_OFFSET = 32;
    const int KNOB_X_OFFSET = SCREEN_WIDTH / 2 - 20;

    void setup() override {
        // Load bitmap background from NitroFS for the top screen
        NF_Load16bitsBg("bg/splash", 0);

        // Bottom screen background
        NF_LoadTiledBg("bg/paramsbackground", "paramsbackground", 256, 256);
        // Slider knob
        NF_LoadSpriteGfx("sprite/sliderknob", gui_sprites::SLIDER_KNOB, 16, 16);
        NF_LoadSpritePal("sprite/sliderknob", gui_sprites::SLIDER_KNOB);
        NF_VramSpritePal(1, gui_sprites::SLIDER_KNOB, gui_sprites::SLIDER_KNOB);
        NF_VramSpriteGfx(1, gui_sprites::SLIDER_KNOB, gui_sprites::SLIDER_KNOB, false);
        // About button
        NF_LoadSpritePal("sprite/aboutbutton", gui_sprites::ABOUT_BUTTON);
        NF_VramSpritePal(1, gui_sprites::ABOUT_BUTTON, gui_sprites::ABOUT_BUTTON);
        NF_LoadSpriteGfx("sprite/aboutbutton", gui_sprites::ABOUT_BUTTON, 64, 32);
        NF_VramSpriteGfx(1, gui_sprites::ABOUT_BUTTON, gui_sprites::ABOUT_BUTTON, false);

        // Generate button
        NF_LoadSpriteGfx("sprite/generatebutton", gui_sprites::GENERATE_BUTTON, 32, 16);
        NF_VramSpriteGfx(1, gui_sprites::GENERATE_BUTTON, gui_sprites::GENERATE_BUTTON, false);
    }

    void enter() override {
        NF_CreateSprite(1, gui_sprites::GENERATE_BUTTON, gui_sprites::GENERATE_BUTTON, gui_sprites::PALETTE_NUM,
                        SCREEN_WIDTH - 32 - 3, 3);

        NF_CreateSprite(1, gui_sprites::ABOUT_BUTTON, gui_sprites::ABOUT_BUTTON, gui_sprites::ABOUT_BUTTON,
                        3, 3);

        // Load splash image to VRAM of top screen
        NF_Copy16bitsBuffer(0, 1, 0);
        NF_Flip16bitsBackBuffer(0);

        // Create bottom screen background
        NF_CreateTiledBg(1, 1, "paramsbackground");

        // All sliders
        NF_CreateSprite(1, SCALE, gui_sprites::SLIDER_KNOB, gui_sprites::SLIDER_KNOB, KNOB_X_OFFSET, 35);
        NF_CreateSprite(1, REPETITIONS, gui_sprites::SLIDER_KNOB, gui_sprites::SLIDER_KNOB, KNOB_X_OFFSET,
                        35 + KNOB_Y_OFFSET);
        NF_CreateSprite(1, SPIKYNESS, gui_sprites::SLIDER_KNOB, gui_sprites::SLIDER_KNOB, KNOB_X_OFFSET,
                        35 + KNOB_Y_OFFSET * 2);
        NF_CreateSprite(1, COLOURFULNESS, gui_sprites::SLIDER_KNOB, gui_sprites::SLIDER_KNOB, KNOB_X_OFFSET,
                        35 + KNOB_Y_OFFSET * 3);
        NF_CreateSprite(1, WIGGLINESS, gui_sprites::SLIDER_KNOB, gui_sprites::SLIDER_KNOB, KNOB_X_OFFSET,
                        35 + KNOB_Y_OFFSET * 4);

        about_countdown = 60; // reset
    }

    NEXT_ACTION update() override {
        touchPosition touch;
        touchRead(&touch);

        bool inside_run_button = gui_sprites::touch_inside_sprite(touch, SCREEN_WIDTH - 32 - 3, 3, 32, 16);
        if (inside_run_button) {
            return GENERATE;
        }

        if (about_countdown > 0) {
            about_countdown--;
        } else {
            bool inside_about_button = gui_sprites::touch_inside_sprite(touch, 3, 3, 43, 16);
            if (inside_about_button) {
                return SHOW_ABOUT;
            }
        }

        // Update slider knobs FIXME ugly code
        if (gui_sprites::touch_inside_sprite(touch, KNOB_X_OFFSET, 35, 128, 24)) {
            parameters.scale = touch.px - KNOB_X_OFFSET;
        }

        if (gui_sprites::touch_inside_sprite(touch, KNOB_X_OFFSET, 35 + KNOB_Y_OFFSET, 128, 24)) {
            parameters.repetitions = touch.px - KNOB_X_OFFSET;
        }

        if (gui_sprites::touch_inside_sprite(touch, KNOB_X_OFFSET, 35 + KNOB_Y_OFFSET * 2, 128, 24)) {
            parameters.spikyness = touch.px - KNOB_X_OFFSET;
        }

        if (gui_sprites::touch_inside_sprite(touch, KNOB_X_OFFSET, 35 + KNOB_Y_OFFSET * 3, 128, 24)) {
            parameters.colourfulness = touch.px - KNOB_X_OFFSET;
        }

        if (gui_sprites::touch_inside_sprite(touch, KNOB_X_OFFSET, 35 + KNOB_Y_OFFSET * 4, 128, 24)) {
            parameters.wiggliness = touch.px - KNOB_X_OFFSET;
        }

        NF_MoveSprite(1, SCALE, KNOB_X_OFFSET + parameters.scale - 8, 35);
        NF_MoveSprite(1, REPETITIONS, KNOB_X_OFFSET + parameters.repetitions - 8, 35 + KNOB_Y_OFFSET);
        NF_MoveSprite(1, SPIKYNESS, KNOB_X_OFFSET + parameters.spikyness - 8, 35 + KNOB_Y_OFFSET * 2);
        NF_MoveSprite(1, COLOURFULNESS, KNOB_X_OFFSET + parameters.colourfulness - 8, 35 + KNOB_Y_OFFSET * 3);
        NF_MoveSprite(1, WIGGLINESS, KNOB_X_OFFSET + parameters.wiggliness - 8, 35 + KNOB_Y_OFFSET * 4);

        return NONE;
    }

    void leave() override {
        NF_DeleteTiledBg(1, 1);
//        NF_DeleteTiledBg(1, 1);
        NF_DeleteSprite(1, SCALE);
        NF_DeleteSprite(1, REPETITIONS);
        NF_DeleteSprite(1, SPIKYNESS);
        NF_DeleteSprite(1, COLOURFULNESS);
        NF_DeleteSprite(1, WIGGLINESS);

        NF_DeleteSprite(1, gui_sprites::GENERATE_BUTTON);
        NF_DeleteSprite(1, gui_sprites::ABOUT_BUTTON);
    }
};


class RunScene : public Scene {
    int done_countdown = 200;

public:
    void setup() override {
        NF_LoadTiledBg("bg/instructionsbackground", "instructionsbackground", 256, 256);

        // Load text font files from NitroFS
        NF_LoadTextFont16("fnt/font16", "down", 256, 256, 0);

        // Load the images used for drawing
        NF_Load16bitsImage("graphicsparts/circlesmall", 1, 16, 16);
        NF_Load16bitsImage("graphicsparts/pixel", 2, 16, 16);

        // White background
        NF_Load16bitsBg("bg/clear", 1);

        // Load back button
        NF_LoadSpriteGfx("sprite/backbutton", gui_sprites::BACK_BUTTON, 16, 16);
        NF_VramSpriteGfx(1, gui_sprites::BACK_BUTTON, gui_sprites::BACK_BUTTON, false);

    }

    void enter() override {
        NF_CreateSprite(1, gui_sprites::BACK_BUTTON, gui_sprites::BACK_BUTTON, gui_sprites::PALETTE_NUM, 3, 3);

        // Load the background and dump it to the screen
        NF_CreateTiledBg(1, 1, "instructionsbackground");
        NF_Copy16bitsBuffer(0, 1, 1);
        NF_Flip16bitsBackBuffer(0);

        instructions::generate();
        instructions::instructions_index = 0;
        done_countdown = 200;

        // Create a text layer
        NF_CreateTextLayer16(1, 0, 0, "down");
        // Define a color for the text font
        NF_DefineTextColor(1, 0, 1, 31, 31, 31); // White

        // Loop over the instructions and write text to the text layer
//        for (int i = 0; i < instructions::instructions.size(); i++) {
//            const char *instruction = instructions::instructions[i].c_str();
//            NF_WriteText16(1, 0, 1, 2 + i, instruction); // Text with default color
//        }
        // Print the values of all parameters
//        NF_WriteText16(1, 0, 1, 2, std::to_string(parameters.scale).c_str());
//        NF_WriteText16(1, 0, 1, 3, std::to_string(parameters.repetitions).c_str());
//        NF_WriteText16(1, 0, 1, 4, std::to_string(parameters.spikyness).c_str());
//        NF_WriteText16(1, 0, 1, 5, std::to_string(parameters.colourfulness).c_str());
//        NF_WriteText16(1, 0, 1, 6, std::to_string(parameters.wiggliness).c_str());

        // Update text layers
        NF_UpdateTextLayers();
    }

    NEXT_ACTION update() override {
        touchPosition touch;
        touchRead(&touch);

        bool inside_back_button = gui_sprites::touch_inside_sprite(touch, 3, 3, 16, 16);
        if (inside_back_button) {
            return BACK;
        }

        // Debug information
        NF_WriteText16(1, 0, 1, 7, std::to_string(instructions::instructions_index).c_str());
//        NF_WriteText16(1, 0, 1, 8, std::to_string(done_countdown).c_str());
        NF_UpdateTextLayers();


        // Done countdown
        if (instructions::instructions_index == instructions::instructions_length) {
            done_countdown--;
            if (done_countdown == 0) {
                return BACK;
            }
        } else {
            // Actual executing / drawing code
            int x = utils::rrandom(256);
            int y = utils::rrandom(192);
            draw::draw_filled_circle(x, y, 10);
            instructions::instructions_index++;
        }


        return NONE;
    }

    void leave() override {
        NF_DeleteTiledBg(1, 1);
        NF_DeleteTextLayer(1, 0);
        NF_DeleteSprite(1, gui_sprites::BACK_BUTTON);
    }
};

class AboutScene : public Scene {
private:
    int back_countdown = 60;

public:
    void setup() {
        NF_LoadTiledBg("bg/aboutpage", "aboutpage", 256, 256);
    }

    void enter() override {
        NF_CreateTiledBg(1, 1, "aboutpage");
        NF_CreateSprite(1, gui_sprites::BACK_BUTTON, gui_sprites::BACK_BUTTON, gui_sprites::PALETTE_NUM, 3, 3);

        back_countdown = 60; // reset
    }

    NEXT_ACTION update() override {
        touchPosition touch;
        touchRead(&touch);

        if (back_countdown > 0) {
            back_countdown--;
        }

        // if within back button
        bool inside_back_button = gui_sprites::touch_inside_sprite(touch, 3, 3, 16, 16);
        if (inside_back_button && back_countdown == 0) {
            return BACK;
        }

        return NONE;
    }

    void leave() override {
        NF_DeleteTiledBg(1, 1);
        NF_DeleteSprite(1, gui_sprites::BACK_BUTTON);
    }
};


/*
|--------------------------------------------------------------------------
| Screens
|--------------------------------------------------------------------------
*/
void initialize_top_screen() {
    // Initialize bitmap backgrounds system
    NF_InitBitmapBgSys(0, 1);
    // Initialize storage buffers
    NF_Init16bitsBgBuffers();
    // Initialize backbuffers
    NF_Init16bitsBackBuffer(0);
    // Enable backbuffers
    NF_Enable16bitsBackBuffer(0);
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
    printf("\n Loading artificial... \n\n");
    swiWaitForVBlank();

    // Initialize NitroFS and set it as the root folder of the filesystem
    nitroFSInit(NULL);
    NF_SetRootFolder("NITROFS");

    initialize_top_screen();
    initialize_bottom_screen();
}


int main(int argc, char **argv) {
    initialize_screens();
    gui_sprites::setup_main_palette();

    // Initialize scenes
    ParametersScene parameters_scene;
    RunScene run_scene;
    AboutScene about_scene;

    parameters_scene.setup();
    run_scene.setup();
    about_scene.setup();

    Scene *current_scene = &parameters_scene;

    // Go to the parameters scene
    parameters_scene.enter();


    // Start
    bool start_pressed = false;

    while (!start_pressed) {
        scanKeys(); // Read keypad
        u16 keys = keysHeld(); // Keys currently pressed

        // if start pressed, exit
        if (keys & KEY_START) {
            start_pressed = true;
        }

        // Update the current scene and handle transitions / check if we should leave
        NEXT_ACTION nextAction = current_scene->update();
        if (nextAction == BACK) {
            current_scene->leave();
            current_scene = &parameters_scene;
            current_scene->enter();
        } else if (nextAction == GENERATE) {
            current_scene->leave();
            current_scene = &run_scene;
            current_scene->enter();
        } else if (nextAction == SHOW_ABOUT) {
            current_scene->leave();
            current_scene = &about_scene;
            current_scene->enter();
        }



        // Update OAM array every frame, regardless of the scene
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
