

namespace view {
    enum View {
        PARAMETERS,
        INSTRUCTIONS,
        RUNNING
    };

    View current_view = PARAMETERS;


    void run() {
        view::current_view = view::RUNNING;

        NF_HideBg(0, 1);
        NF_ClearTextLayer16(1, 0); // Clear the text layer

        instructions::generate();
        text_console::write_instructions_to_text_layer(instructions::instructions);

        // Update text layers
        NF_UpdateTextLayers();
    }


    void go_to_parameters_screen() {
        view::current_view = view::PARAMETERS;

        NF_ShowBg(0, 1);
        NF_ClearTextLayer16(1, 0); // Clear the text layer

        // Write the parameters
        NF_WriteText16(1, 0, 1, 2, "Lines"); // Text with default color
        NF_WriteText16(1, 0, 1, 4, "Circles"); // Text with default color
        NF_WriteText16(1, 0, 1, 6, "Polygons"); // Text with default color
        NF_WriteText16(1, 0, 1, 8, "Colorfulness"); // Text with default color
        NF_WriteText16(1, 0, 1, 10, "Gradientness"); // Text with default color

        // Update text layers
        NF_UpdateTextLayers();
    }
}