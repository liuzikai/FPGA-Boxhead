#include "resource.h"

// Up as 0 degree, cw

const unsigned int zombie_offset[8] = {
    0x8090, 0x5070, 0x2BE0, 0x0000, 0xA178, 0x0000, 0x2BE0, 0x5070
};

const unsigned int zombie_width[8] = {
    27, 35, 30, 36, 26, 36, 30, 35
};

const unsigned int zombie_height[8] = {
    39, 44, 39, 39, 36, 39, 39, 44
};

const unsigned int zombie_flip_x[8] = {
    0, 1, 1, 1, 0, 0, 0, 0
};
