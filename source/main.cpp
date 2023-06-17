#include <nds.h>
#include <stdio.h> // NOLINT(modernize-deprecated-headers)
#include <string>
#include <vector>
#include <filesystem.h>
#include <nf_lib.h>
#include <random>
#include <algorithm>
#include <iostream>
#include "effolkronium/random.hpp"


/*
|--------------------------------------------------------------------------
| Utils
|--------------------------------------------------------------------------
*/
// get base random alias which is auto seeded and has static API and internal state
using Random = effolkronium::random_static;

namespace utils {

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

    enum GRAPHICS_PARTS_IDS {
        PIXEL_BLACK = 3,
        PIXEL_PINK = 4,
        PIXEL_PURPLE = 5,
        PIXEL_LILAC = 6,
        PIXEL_CYAN = 7,
        PIXEL_OLIVE = 8,
        CIRCLE_SMALL = 9
    };

    // Global drawing state
    bool is_blackandwhite = false;
    int current_color = 0;
    int frames_per_color = 500;
    int frame = 0;

    void cycle_color() {
        if (frame >= frames_per_color) {
            frame = 0;
            current_color = (current_color + 1) % 5;
        } else {
            frame++;
        }
    }

    void draw_pixel(int x, int y) {
        if (is_blackandwhite) {
            NF_Draw16bitsImage(0, PIXEL_BLACK, x, y, true);
        } else {
            cycle_color();
            int color = 4 + current_color;
            NF_Draw16bitsImage(0, color, x, y, true);
        }
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

    void draw_triangle(int x1, int y1, int x2, int y2, int x3, int y3) {
        draw_line(x1, y1, x2, y2);
        draw_line(x2, y2, x3, y3);
        draw_line(x3, y3, x1, y1);
    }

    void draw_triangle_filled(int x1, int y1, int x2, int y2, int x3, int y3) {
        int min_x = std::min(x1, std::min(x2, x3));
        int max_x = std::max(x1, std::max(x2, x3));
        int min_y = std::min(y1, std::min(y2, y3));
        int max_y = std::max(y1, std::max(y2, y3));

        for (int y = min_y; y <= max_y; y++) {
            for (int x = min_x; x <= max_x; x++) {
                if (x1 == x2 && x2 == x3) {
                    if (x == x1) {
                        draw_pixel(x, y);
                    }
                } else if (y1 == y2 && y2 == y3) {
                    if (y == y1) {
                        draw_pixel(x, y);
                    }
                } else {
                    int x1_x2 = x1 - x2;
                    int x2_x3 = x2 - x3;
                    int x3_x1 = x3 - x1;
                    int y1_y2 = y1 - y2;
                    int y2_y3 = y2 - y3;
                    int y3_y1 = y3 - y1;

                    int x_x1 = x - x1;
                    int x_x2 = x - x2;
                    int x_x3 = x - x3;
                    int y_y1 = y - y1;
                    int y_y2 = y - y2;
                    int y_y3 = y - y3;

                    int a = x1_x2 * y_y1 - y1_y2 * x_x1;
                    int b = x2_x3 * y_y2 - y2_y3 * x_x2;
                    int c = x3_x1 * y_y3 - y3_y1 * x_x3;

                    if ((a >= 0 && b >= 0 && c >= 0) || (a <= 0 && b <= 0 && c <= 0)) {
                        draw_pixel(x, y);
                    }
                }
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
    /**
     * Different types of instructions
     */
    class Instruction {
        public:
            bool is_blackandwhite = false;
            int frames_per_color = 500;
            virtual bool do_step() = 0; // Ret
            virtual std::string print() = 0; // turns 'true' when done
    };


    class LineSweep: public Instruction {
    public:
        int x1;
        int y1;
        int count;

        int step_i = 0;
        int current_x;
        int current_y;
        int last_x;
        int last_y;

        bool do_step() override {
            if (step_i++ > count) return true;

            if (!current_x) current_x = x1;
            if (!current_y) current_y = y1;

            // Set global draw parameters
            draw::is_blackandwhite = this->is_blackandwhite;
            draw::frames_per_color = this->frames_per_color;

            // Draw line
            current_x = Random::get(-parameters.scale, parameters.scale);
            current_y = Random::get(-parameters.scale, parameters.scale);

            draw::draw_line(last_x, last_y, current_x, current_y);

            last_x = current_x;
            last_y = current_y;

            return false;
        }

        std::string print() override {
            return "LineSweep x1:" + std::to_string(x1) + " y1:" + std::to_string(y1) + " no:" + std::to_string(count);
        }
    };

    class Burst: public Instruction {
    public:
        int center_x;
        int center_y;
        int radius;
        int count;
        bool filled;
        bool has_border;

        int step_i = 0;

        bool do_step() override {
            if (step_i++ > count) return true;

            // Set global draw parameters
            draw::is_blackandwhite = this->is_blackandwhite;
            draw::frames_per_color = this->frames_per_color;

            int x2 = center_x + Random::get(-100, 100);
            int y2 = center_y + Random::get(-100, 100);

            int x3 = center_x + Random::get(-100, 100);
            int y3 = center_y + Random::get(-100, 100);

            // Draw shape
            if (filled) {
                draw::draw_triangle_filled(center_x, center_y, x2, y2, x3, y3);
            } else {
                draw::draw_triangle(center_x, center_y, x2, y2, x3, y3);
            }
//            if (filled) {
//                if (utils::rrandom(100) > 50) {
//                    draw::draw_filled_rectangle(x - radius, y - radius, x + radius, y + radius);
//                } else {
//                    draw::draw_filled_circle(center_x, center_y, radius);
//                }
//            } else {
//                if (utils::rrandom(100) > 50) {
//                    draw::draw_rectangle(center_x - radius, center_y - radius, center_x + radius, center_y + radius);
//                } else {
//                    draw::draw_circle(center_x, center_y, radius);
//                }
//            }

            return false;
        }

        std::string print() {
            return "Burst x:" + std::to_string(center_x) + " y:" + std::to_string(center_y) + " r:" + std::to_string(radius) + " no:" + std::to_string(count);
        }
    };

    class Squiggle: public Instruction {
    public:
        int start_x;
        int start_y;
        int x_step;
        int y_step;
        int max_steps;
        int radius;

        int current_x = 0;
        int current_y = 0;
        int step_i = 0;

        bool do_step() override {
            if (step_i++ > max_steps) return true;

            if (!current_x) current_x = start_x;
            if (!current_y) current_y = start_y;

            // Invert direction
            if (current_x > SCREEN_WIDTH + radius) x_step = -x_step;
            if (current_x < 0 - radius) x_step = -x_step;
            if (current_y > SCREEN_HEIGHT + radius) y_step = -y_step;
            if (current_y < 0 - radius) y_step = -y_step;

            // Set global draw parameters
            draw::is_blackandwhite = this->is_blackandwhite;
            draw::frames_per_color = this->frames_per_color;

            draw::draw_filled_circle(current_x, current_y, radius);
//            NF_Flip16bitsBackBuffer(0); // needed to show the pixel

            current_x += x_step + Random::get(5, 10);
            current_y += y_step + Random::get(5, 10);

            return false;
        }

        std::string print() override {
            return "Squiggle x:" + std::to_string(start_x) + " y:" + std::to_string(start_y) + " steps:" + std::to_string(max_steps);
        }
    };

    class Confetti: public Instruction {
    public:
        int count;
        int scale;

        int step_i = 0;

        bool do_step() override {
            if (step_i++ > count) return true;

            // Set global draw parameters
            draw::is_blackandwhite = this->is_blackandwhite;
            draw::frames_per_color = this->frames_per_color;

            int x = Random::get(0, SCREEN_WIDTH);
            int y = Random::get(0, SCREEN_HEIGHT);
            int spikyness = Random::get(0, parameters.spikyness);
            if (spikyness < 64) {
                draw::draw_filled_circle(x, y, scale);
            } else if (spikyness < 84) {
                draw::draw_filled_rectangle(x, y, x+scale*2, y+scale*2);
            }


            //            NF_Flip16bitsBackBuffer(0); // needed to show the pixel


            return false;
        }

        std::string print() override {
            return "Confetti count:" + std::to_string(count);
        }
    };




    auto instructions = std::vector<Instruction*>();

    unsigned int instructions_index = 0;


    /**
     * Generates the instructions for the current parameters
     */
    void generate() {
        instructions.erase(instructions.begin(), instructions.end());

        for (int i = 0; i < parameters.repetitions / 5; i++) {
            int t = Random::get(0, 10);
            Instruction* instruction;

            if (t > 5) {
                if (Random::get(0, parameters.spikyness) > Random::get(0, parameters.wiggliness)) {
                    auto lineSweep = new LineSweep();
                    lineSweep->count = Random::get(0, 50);
                    lineSweep->x1 = Random::get(0, SCREEN_WIDTH);
                    lineSweep->y1 = Random::get(0, SCREEN_HEIGHT);
                    instruction = lineSweep;
                } else {
                    auto confetti = new Confetti();
                    confetti->count = Random::get(0, 100);
                    confetti->scale = Random::get(1, parameters.scale / 25);
                    instruction = confetti;
                }
            } else {
                if (Random::get(0, parameters.spikyness) > Random::get(0, parameters.wiggliness)) {
                    auto burst = new Burst();
                    burst->center_x = Random::get(0, SCREEN_WIDTH);
                    burst->center_y = Random::get(0, SCREEN_HEIGHT);
                    burst->radius = Random::get(0, parameters.scale) + 1;
                    burst->count = Random::get(0, 100);
                    burst->filled = Random::get(50, 100);
                    instruction = burst;
                } else {
                    auto squiggle = new Squiggle();
                    squiggle->x_step = Random::get(-5, 5);
                    squiggle->y_step = Random::get(-5, 5);
                    squiggle->max_steps = Random::get(0, 150);
                    squiggle->radius = Random::get(1, parameters.scale / 5);
                    squiggle->start_x = Random::get(0, SCREEN_WIDTH);
                    squiggle->start_y = Random::get(0, SCREEN_HEIGHT);
                    instruction = squiggle;
                }
            }

            // Valid for all instructions
            instruction->is_blackandwhite = Random::get(0, 90) > parameters.colourfulness;
            instruction->frames_per_color = Random::get(1, 200);
            instructions.push_back(instruction);
        }
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
    int done_countdown = 0;

    enum TEXT_COLORS {
        WHITE,
        OLIVE,
        PINK,
    };

public:
    void setup() override {
        NF_LoadTiledBg("bg/instructionsbackground", "instructionsbackground", 256, 256);

        // Load text font files from NitroFS
        NF_LoadTextFont("fnt/default", "down", 256, 256, 0);


        // Load the images used for drawing
        NF_Load16bitsImage("graphicsparts/circlesmall", draw::CIRCLE_SMALL, 16, 16);
        NF_Load16bitsImage("graphicsparts/pixel", draw::PIXEL_BLACK, 16, 16);
        NF_Load16bitsImage("graphicsparts/pixel1", draw::PIXEL_PINK, 16, 16);
        NF_Load16bitsImage("graphicsparts/pixel2", draw::PIXEL_PURPLE, 16, 16);
        NF_Load16bitsImage("graphicsparts/pixel3", draw::PIXEL_LILAC, 16, 16);
        NF_Load16bitsImage("graphicsparts/pixel4", draw::PIXEL_CYAN, 16, 16);
        NF_Load16bitsImage("graphicsparts/pixel5", draw::PIXEL_OLIVE, 16, 16);

        // White background
        NF_Load16bitsBg("bg/clear", 1);

        // Load back button
        NF_LoadSpriteGfx("sprite/backbutton", gui_sprites::BACK_BUTTON, 16, 16);
        NF_VramSpriteGfx(1, gui_sprites::BACK_BUTTON, gui_sprites::BACK_BUTTON, false);

    }

    void print_parameter_values() {
        // Print the values of all parameters
        NF_WriteText(1, 0, 1, 2, std::to_string(parameters.scale).c_str());
        NF_WriteText(1, 0, 1, 3, std::to_string(parameters.repetitions).c_str());
        NF_WriteText(1, 0, 1, 4, std::to_string(parameters.spikyness).c_str());
        NF_WriteText(1, 0, 1, 5, std::to_string(parameters.colourfulness).c_str());
        NF_WriteText(1, 0, 1, 6, std::to_string(parameters.wiggliness).c_str());
    }

    void print_instructions() {
        // Loop over the instructions and write text to the text layer

        int limit = std::min<int>(instructions::instructions.size(), 19);
        for (int i = 0; i < limit; i++) {
            int offset = std::max<int>(instructions::instructions_index-19, 0);
            int i_offsetted = i + offset;
            std::string instruction = instructions::instructions[i_offsetted]->print();
            if (i_offsetted < instructions::instructions_index) {
                NF_SetTextColor(1, 0, OLIVE); // Set the text color to purple (defined in setup
            } else if (i_offsetted == instructions::instructions_index) {
                NF_SetTextColor(1, 0, PINK);// Text with white color
            } else {
                NF_SetTextColor(1, 0, WHITE);// Text with white color
            }
            NF_WriteText(1, 0, 1, 4 + i, instruction.c_str()); // Text with default color
        }
    }

    void print_render_status() {
        // Set text color
        bool done = instructions::instructions_index == instructions::instructions.size();
        if (done) {
            NF_SetTextColor(1, 0, PINK);// Text with white color
        } else {
            NF_SetTextColor(1, 0, WHITE);// Text with white color
        }

        // Print the rendering status
        std::string status_text = std::to_string(instructions::instructions_index) + "/" + std::to_string(instructions::instructions.size());
        if (done) {
            status_text = "Done!";
        }

        NF_WriteText(1, 0, 25, 1, status_text.c_str());
    }

    void enter() override {
        NF_CreateSprite(1, gui_sprites::BACK_BUTTON, gui_sprites::BACK_BUTTON, gui_sprites::PALETTE_NUM, 3, 3);

        // Load the background
        NF_CreateTiledBg(1, 1, "instructionsbackground");

        //  Create the text layers
        NF_CreateTextLayer16(1, 0, 0, "down");

        // Define text colors
        NF_DefineTextColor(1, 0, WHITE, 31, 31, 31); // White
        NF_DefineTextColor(1, 0, OLIVE, 0, 13, 10); // Olive Green
        NF_DefineTextColor(1, 0, PINK, 31, 14, 20); // Pink

        restart();

        // Update text layers
        NF_UpdateTextLayers();
    }

    void restart() {
        // Generate new instructions and reset
        instructions::generate();
        instructions::instructions_index = 0;
        done_countdown = 0;

        // Clear the screen
        NF_Copy16bitsBuffer(0, 1, 1);
        NF_Flip16bitsBackBuffer(0);

        // Clear the text layer
        NF_ClearTextLayer(1, 0);
    }

    NEXT_ACTION update() override {
        touchPosition touch;
        touchRead(&touch);

        bool inside_back_button = gui_sprites::touch_inside_sprite(touch, 3, 3, 16, 16);
        if (inside_back_button) {
            return BACK;
        }


        // Print the instructions and which have been done already
        print_render_status();
        print_instructions();
        NF_UpdateTextLayers();


        // Done countdown
        if (instructions::instructions_index == instructions::instructions.size()) {
            done_countdown--;
            if (done_countdown <= 0) {
                restart();
            }
        } else {
            // Actual executing / drawing code
            bool is_done = instructions::instructions[instructions::instructions_index]->do_step();
            NF_Flip16bitsBackBuffer(0);
            if (is_done) {
                instructions::instructions_index++;
            }
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
