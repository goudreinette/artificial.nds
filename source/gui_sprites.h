
namespace gui_sprites {
    const int PALETTE_NUM = 0;

    enum GUI_SPRITE_IDS {
        GENERATE_BUTTON,
        BACK_BUTTON,
        SLIDER_TRACK,
        SLIDER_HANDLE,
        PARAMSTEXT,
        RUNNINGTEXT
    };


    void setup_all() {
        // Load sprite files from NitroFS
        NF_LoadSpriteGfx("sprite/generatebutton", GENERATE_BUTTON, 32, 16);
        NF_LoadSpriteGfx("sprite/backbutton", BACK_BUTTON, 16, 16);
        NF_LoadSpriteGfx("sprite/slidertrack", SLIDER_TRACK, 64, 32);
        NF_LoadSpritePal("sprite/slidertrack", SLIDER_TRACK);
        NF_LoadSpritePal("sprite/generatebutton", PALETTE_NUM);

        // Transfer the required sprites to VRAM
        NF_VramSpriteGfx(1, GENERATE_BUTTON, GENERATE_BUTTON, false); // Ball: Keep all frames in VRAM
        NF_VramSpriteGfx(1, BACK_BUTTON, BACK_BUTTON, false); // Ball: Keep all frames in VRAM
        NF_VramSpriteGfx(1, SLIDER_TRACK, SLIDER_TRACK, false); // Ball: Keep all frames in VRAM
        NF_VramSpritePal(1, PALETTE_NUM, PALETTE_NUM);
        NF_VramSpritePal(1, SLIDER_TRACK, SLIDER_TRACK);

        // Create the sprites
        NF_CreateSprite(1, GENERATE_BUTTON, GENERATE_BUTTON, PALETTE_NUM, SCREEN_WIDTH - 32 - 3, 3);
        NF_CreateSprite(1, BACK_BUTTON, BACK_BUTTON, PALETTE_NUM, 3, 3);
        NF_CreateSprite(1, SLIDER_TRACK, SLIDER_TRACK, SLIDER_TRACK, SCREEN_WIDTH / 2, 66);
        NF_SpriteRotScale(1, SLIDER_TRACK, 256, 512, 256);
    }


    bool touch_inside_sprite(touchPosition touch, int x, int y, int width, int height) {
        return touch.px > x && touch.px < x + width && touch.py > y && touch.py < y + height;
    }


}