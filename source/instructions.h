#include <stdio.h> // NOLINT(modernize-deprecated-headers)
#include <string>
#include <vector>
#include "nds.h"
#include "nf_lib.h"
#include <random>
#include "utils.h"

namespace instructions {
    std::vector<std::string> instructions = {};


    /**
     * Generates the instructions for the current parameters
     */
    void generate() {
        std::vector<std::string> code = {};

        // circles
        for (int c = 0; c < utils::rrandom(5); c++) {
            code.push_back(
                    "(circle " + std::to_string(utils::rrandom(SCREEN_WIDTH)) + " " + std::to_string(utils::rrandom(SCREEN_HEIGHT)) +
                    " " + std::to_string(utils::rrandom(SCREEN_WIDTH)) + " " + std::to_string(utils::rrandom(10)) + ")");
        }

        // lines
        for (int l = 0; l < utils::rrandom(5); l++) {
            code.push_back(
                    "(line " + std::to_string(utils::rrandom(SCREEN_WIDTH)) + " " + std::to_string(utils::rrandom(SCREEN_HEIGHT)) +
                    " " + std::to_string(utils::rrandom(SCREEN_WIDTH)) + " " + std::to_string(utils::rrandom(SCREEN_HEIGHT)) + ")");
        }

        // // boxes
        // for (int b = 0; b < rrandom(5); b++) {
        //     code.push_back("(box " + std::to_string(rrandom(SCREEN_WIDTH)) + " " + std::to_string(rrandom(SCREEN_HEIGHT)) + " " + std::to_string(rrandom(SCREEN_WIDTH)) + " " + std::to_string(rrandom(SCREEN_HEIGHT)) + ")");
        // }

        // // boxes filled
        // for (int b = 0; b < rrandom(5); b++) {
        //     code.push_back("(box-filled " + std::to_string(rrandom(SCREEN_WIDTH)) + " " + std::to_string(rrandom(SCREEN_HEIGHT)) + " " + std::to_string(rrandom(SCREEN_WIDTH)) + " " + std::to_string(rrandom(SCREEN_HEIGHT)) + ")");
        // }

        // // boxes filled gradient
        // for (int b = 0; b < rrandom(5); b++) {
        //     code.push_back("(box-filled-gradient " + std::to_string(rrandom(SCREEN_WIDTH)) + " " + std::to_string(rrandom(SCREEN_HEIGHT)) + " " + std::to_string(rrandom(SCREEN_WIDTH)) + " " + std::to_string(rrandom(SCREEN_HEIGHT)) + " " + std::to_string(rrandom(SCREEN_WIDTH)) + " " + std::to_string(rrandom(SCREEN_HEIGHT)) + " " + std::to_string(rrandom(SCREEN_WIDTH)) + " " + std::to_string(rrandom(SCREEN_HEIGHT)) + ")");
        // }

        // // triangles
        // for (int t = 0; t < rrandom(5); t++) {
        //     code.push_back("(triangle " + std::to_string(rrandom(SCREEN_WIDTH)) + " " + std::to_string(rrandom(SCREEN_HEIGHT)) + " " + std::to_string(rrandom(SCREEN_WIDTH)) + " " + std::to_string(rrandom(SCREEN_HEIGHT)) + " " + std::to_string(rrandom(SCREEN_WIDTH)) + " " + std::to_string(rrandom(SCREEN_HEIGHT)) + ")");
        // }

        // // triangles filled
        // for (int t = 0; t < rrandom(5); t++) {
        //     code.push_back("(triangle-filled " + std::to_string(rrandom(SCREEN_WIDTH)) + " " + std::to_string(rrandom(SCREEN_HEIGHT)) + " " + std::to_string(rrandom(SCREEN_WIDTH)) + " " + std::to_string(rrandom(SCREEN_HEIGHT)) + " " + std::to_string(rrandom(SCREEN_WIDTH)) + ")");
        // }

        // // triangles filled gradient
        // for (int t = 0; t < rrandom(5); t++) {
        //     code.push_back("(triangle-filled-gradient " + std::to_string(rrandom(SCREEN_WIDTH)) + " " + std::to_string(rrandom(SCREEN_HEIGHT)) + " " + std::to_string(rrandom(SCREEN_WIDTH)) + ")");
        // }


        instructions = code;
    }
}
