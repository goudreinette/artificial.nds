namespace backgrounds {


    void setup_all() {
        // Load background files from NitroFS
        NF_LoadTiledBg("bg/splash", "splash", 256, 256);
        NF_LoadTiledBg("bg/background", "background", 256, 256);

        // Create top screen background
        NF_CreateTiledBg(0, 1, "splash");

        // Create bottom screen background
        NF_CreateTiledBg(1, 1, "background");
    }
}