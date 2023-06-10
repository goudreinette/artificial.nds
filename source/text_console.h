

namespace text_console {
    void setup() {
        // Load text font files from NitroFS
        NF_LoadTextFont16("fnt/font16", "down", 256, 256, 0);

        // Create a text layer
        NF_CreateTextLayer16(1, 0, 0, "down");

        // Define a color for the text font
        NF_DefineTextColor(1, 0, 1, 31, 31, 31); // White
    }

    void write_instructions_to_text_layer(std::vector<std::string> instructions) {
        // loop over the instructions and write text
        for (int i = 0; i < instructions.size(); i++) {
            const char *instruction = instructions[i].c_str();
            NF_WriteText16(1, 0, 1, 2 + i, instruction); // Text with default color
        }
    }
}