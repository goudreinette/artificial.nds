#include <nds.h>


namespace utils {
    /**
     * Generates a random number between 0 and range
     * @param range
     * @return
     */
    int rrandom(int range) {
        return rand() % range;
    }
}

