namespace backgrounds {


    void setup_all() {
        // Load background files from NitroFS
        NF_LoadTiledBg("bg/background", "background", 256, 256);
        NF_LoadTiledBg("bg/instructionsbackground", "instructionsbackground", 256, 256);

        // Create bottom screen backgrounds
        NF_CreateTiledBg(1, 1, "background");
    }
}