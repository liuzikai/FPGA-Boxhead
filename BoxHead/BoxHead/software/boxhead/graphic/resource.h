#ifndef RESOURCE_H_
#define RESOURCE_H_

static int ZOMBIE_WALK_WIDTH[8] = {  // [direction]
    22, 28, 23, 28, 22, 28, 23, 28
};
static int ZOMBIE_WALK_HEIGHT[8] = {  // [direction]
    34, 36, 33, 33, 30, 33, 33, 36
};
static int ZOMBIE_WALK_FLIP_X[8] = {  // [direction]
    0, 0, 0, 0, 0, 1, 1, 1
};
#define ZOMBIE_WALK_FRAME_COUNT 8
static int ZOMBIE_WALK_OFFSET[8][8] = {  // [direction][frame_index]
    {0x00000, 0x002EC, 0x005D8, 0x008C4, 0x00BB0, 0x00E9C, 0x01188, 0x01474},
    {0x01760, 0x01B50, 0x01F40, 0x02330, 0x02720, 0x02B10, 0x02F00, 0x032F0},
    {0x036E0, 0x039D7, 0x03CCE, 0x03FC5, 0x042BC, 0x045B3, 0x048AA, 0x04BA1},
    {0x04E98, 0x05234, 0x055D0, 0x0596C, 0x05D08, 0x060A4, 0x06440, 0x067DC},
    {0x06B78, 0x06E0C, 0x070A0, 0x07334, 0x075C8, 0x0785C, 0x07AF0, 0x07D84},
    {0x04E98, 0x05234, 0x055D0, 0x0596C, 0x05D08, 0x060A4, 0x06440, 0x067DC},
    {0x036E0, 0x039D7, 0x03CCE, 0x03FC5, 0x042BC, 0x045B3, 0x048AA, 0x04BA1},
    {0x01760, 0x01B50, 0x01F40, 0x02330, 0x02720, 0x02B10, 0x02F00, 0x032F0}
};


static int ZOMBIE_HIT_FRONT_WIDTH[8] = {  // [direction]
    22, 28, 22, 27, 21, 27, 22, 28
};
static int ZOMBIE_HIT_FRONT_HEIGHT[8] = {  // [direction]
    29, 32, 32, 34, 32, 34, 32, 32
};
static int ZOMBIE_HIT_FRONT_FLIP_X[8] = {  // [direction]
    0, 0, 0, 0, 0, 1, 1, 1
};
#define ZOMBIE_HIT_FRONT_FRAME_COUNT 11
static int ZOMBIE_HIT_FRONT_OFFSET[8][11] = {  // [direction][frame_index]
    {0x08018, 0x08018, 0x08018, 0x08018, 0x08296, 0x08296, 0x08296, 0x08296, 0x08296, 0x08296, 0x08296},
    {0x08514, 0x08514, 0x08514, 0x08514, 0x08894, 0x08894, 0x08894, 0x08894, 0x08894, 0x08894, 0x08894},
    {0x08C14, 0x08C14, 0x08C14, 0x08C14, 0x08ED4, 0x08ED4, 0x08ED4, 0x08ED4, 0x08ED4, 0x08ED4, 0x08ED4},
    {0x09194, 0x09194, 0x09194, 0x09194, 0x0952A, 0x0952A, 0x0952A, 0x0952A, 0x0952A, 0x0952A, 0x0952A},
    {0x098C0, 0x098C0, 0x098C0, 0x098C0, 0x09B60, 0x09B60, 0x09B60, 0x09B60, 0x09B60, 0x09B60, 0x09B60},
    {0x09194, 0x09194, 0x09194, 0x09194, 0x0952A, 0x0952A, 0x0952A, 0x0952A, 0x0952A, 0x0952A, 0x0952A},
    {0x08C14, 0x08C14, 0x08C14, 0x08C14, 0x08ED4, 0x08ED4, 0x08ED4, 0x08ED4, 0x08ED4, 0x08ED4, 0x08ED4},
    {0x08514, 0x08514, 0x08514, 0x08514, 0x08894, 0x08894, 0x08894, 0x08894, 0x08894, 0x08894, 0x08894}
};


static int ZOMBIE_DIE_WIDTH[8] = {  // [direction]
    22, 22, 41, 41, 23, 23, 41, 41
};
static int ZOMBIE_DIE_HEIGHT[8] = {  // [direction]
    39, 39, 36, 36, 43, 43, 36, 36
};
static int ZOMBIE_DIE_FLIP_X[8] = {  // [direction]
    0, 0, 0, 0, 0, 0, 1, 1
};
#define ZOMBIE_DIE_FRAME_COUNT 11
static int ZOMBIE_DIE_OFFSET[8][11] = {  // [direction][frame_index]
    {0x09E00, 0x0A15A, 0x0A4B4, 0x0A80E, 0x0AB68, 0x0AB68, 0x0AB68, 0x0AB68, 0x0AB68, 0x0AB68, 0x0AB68},
    {0x09E00, 0x0A15A, 0x0A4B4, 0x0A80E, 0x0AB68, 0x0AB68, 0x0AB68, 0x0AB68, 0x0AB68, 0x0AB68, 0x0AB68},
    {0x0AEC2, 0x0B486, 0x0BA4A, 0x0C00E, 0x0C5D2, 0x0C5D2, 0x0C5D2, 0x0C5D2, 0x0C5D2, 0x0C5D2, 0x0C5D2},
    {0x0AEC2, 0x0B486, 0x0BA4A, 0x0C00E, 0x0C5D2, 0x0C5D2, 0x0C5D2, 0x0C5D2, 0x0C5D2, 0x0C5D2, 0x0C5D2},
    {0x0CB96, 0x0CF73, 0x0D350, 0x0D72D, 0x0DB0A, 0x0DB0A, 0x0DB0A, 0x0DB0A, 0x0DB0A, 0x0DB0A, 0x0DB0A},
    {0x0CB96, 0x0CF73, 0x0D350, 0x0D72D, 0x0DB0A, 0x0DB0A, 0x0DB0A, 0x0DB0A, 0x0DB0A, 0x0DB0A, 0x0DB0A},
    {0x0AEC2, 0x0B486, 0x0BA4A, 0x0C00E, 0x0C5D2, 0x0C5D2, 0x0C5D2, 0x0C5D2, 0x0C5D2, 0x0C5D2, 0x0C5D2},
    {0x0AEC2, 0x0B486, 0x0BA4A, 0x0C00E, 0x0C5D2, 0x0C5D2, 0x0C5D2, 0x0C5D2, 0x0C5D2, 0x0C5D2, 0x0C5D2}
};


static int ZOMBIE_ATTACK_WIDTH[8] = {  // [direction]
    23, 38, 35, 38, 23, 38, 35, 38
};
static int ZOMBIE_ATTACK_HEIGHT[8] = {  // [direction]
    34, 36, 34, 40, 37, 40, 34, 36
};
static int ZOMBIE_ATTACK_FLIP_X[8] = {  // [direction]
    0, 0, 0, 0, 0, 1, 1, 1
};
#define ZOMBIE_ATTACK_FRAME_COUNT 8
static int ZOMBIE_ATTACK_OFFSET[8][8] = {  // [direction][frame_index]
    {0x0DEE7, 0x0DEE7, 0x0E1F5, 0x0E1F5, 0x0E503, 0x0E503, 0x0E811, 0x0E811},
    {0x0EB1F, 0x0EB1F, 0x0F077, 0x0F077, 0x0F5CF, 0x0F5CF, 0x0FB27, 0x0FB27},
    {0x1007F, 0x1007F, 0x10525, 0x10525, 0x109CB, 0x109CB, 0x10E71, 0x10E71},
    {0x11317, 0x11317, 0x11907, 0x11907, 0x11EF7, 0x11EF7, 0x124E7, 0x124E7},
    {0x12AD7, 0x12AD7, 0x12E2A, 0x12E2A, 0x1317D, 0x1317D, 0x134D0, 0x134D0},
    {0x11317, 0x11317, 0x11907, 0x11907, 0x11EF7, 0x11EF7, 0x124E7, 0x124E7},
    {0x1007F, 0x1007F, 0x10525, 0x10525, 0x109CB, 0x109CB, 0x10E71, 0x10E71},
    {0x0EB1F, 0x0EB1F, 0x0F077, 0x0F077, 0x0F5CF, 0x0F5CF, 0x0FB27, 0x0FB27}
};


static int PLAYER_WALK_WIDTH[8] = {  // [direction]
    21, 32, 31, 23, 21, 34, 30, 22
};
static int PLAYER_WALK_HEIGHT[8] = {  // [direction]
    32, 35, 33, 36, 35, 35, 33, 35
};
static int PLAYER_WALK_FLIP_X[8] = {  // [direction]
    0, 0, 0, 0, 0, 0, 0, 0
};
#define PLAYER_WALK_FRAME_COUNT 8
static int PLAYER_WALK_OFFSET[8][8] = {  // [direction][frame_index]
    {0x13823, 0x13AC3, 0x13D63, 0x14003, 0x142A3, 0x14543, 0x147E3, 0x14A83},
    {0x14D23, 0x15183, 0x155E3, 0x15A43, 0x15EA3, 0x16303, 0x16763, 0x16BC3},
    {0x17023, 0x17422, 0x17821, 0x17C20, 0x1801F, 0x1841E, 0x1881D, 0x18C1C},
    {0x1901B, 0x19357, 0x19693, 0x199CF, 0x19D0B, 0x1A047, 0x1A383, 0x1A6BF},
    {0x1A9FB, 0x1ACDA, 0x1AFB9, 0x1B298, 0x1B577, 0x1B856, 0x1BB35, 0x1BE14},
    {0x1C0F3, 0x1C599, 0x1CA3F, 0x1CEE5, 0x1D38B, 0x1D831, 0x1DCD7, 0x1E17D},
    {0x1E623, 0x1EA01, 0x1EDDF, 0x1F1BD, 0x1F59B, 0x1F979, 0x1FD57, 0x20135},
    {0x20513, 0x20815, 0x20B17, 0x20E19, 0x2111B, 0x2141D, 0x2171F, 0x21A21}
};


static int PLAYER_HIT_FRONT_WIDTH[8] = {  // [direction]
    21, 28, 22, 27, 21, 27, 22, 28
};
static int PLAYER_HIT_FRONT_HEIGHT[8] = {  // [direction]
    29, 33, 33, 35, 32, 35, 33, 33
};
static int PLAYER_HIT_FRONT_FLIP_X[8] = {  // [direction]
    0, 0, 0, 0, 0, 1, 1, 1
};
#define PLAYER_HIT_FRONT_FRAME_COUNT 11
static int PLAYER_HIT_FRONT_OFFSET[8][11] = {  // [direction][frame_index]
    {0x21D23, 0x21D23, 0x21D23, 0x21D23, 0x21F84, 0x21F84, 0x21F84, 0x21F84, 0x21F84, 0x21F84, 0x21F84},
    {0x221E5, 0x221E5, 0x221E5, 0x221E5, 0x22581, 0x22581, 0x22581, 0x22581, 0x22581, 0x22581, 0x22581},
    {0x2291D, 0x2291D, 0x2291D, 0x2291D, 0x22BF3, 0x22BF3, 0x22BF3, 0x22BF3, 0x22BF3, 0x22BF3, 0x22BF3},
    {0x22EC9, 0x22EC9, 0x22EC9, 0x22EC9, 0x2327A, 0x2327A, 0x2327A, 0x2327A, 0x2327A, 0x2327A, 0x2327A},
    {0x2362B, 0x2362B, 0x2362B, 0x2362B, 0x238CB, 0x238CB, 0x238CB, 0x238CB, 0x238CB, 0x238CB, 0x238CB},
    {0x22EC9, 0x22EC9, 0x22EC9, 0x22EC9, 0x2327A, 0x2327A, 0x2327A, 0x2327A, 0x2327A, 0x2327A, 0x2327A},
    {0x2291D, 0x2291D, 0x2291D, 0x2291D, 0x22BF3, 0x22BF3, 0x22BF3, 0x22BF3, 0x22BF3, 0x22BF3, 0x22BF3},
    {0x221E5, 0x221E5, 0x221E5, 0x221E5, 0x22581, 0x22581, 0x22581, 0x22581, 0x22581, 0x22581, 0x22581}
};


static int PLAYER_DIE_WIDTH[8] = {  // [direction]
    22, 22, 42, 42, 23, 23, 42, 42
};
static int PLAYER_DIE_HEIGHT[8] = {  // [direction]
    40, 40, 36, 36, 44, 44, 36, 36
};
static int PLAYER_DIE_FLIP_X[8] = {  // [direction]
    0, 0, 0, 0, 0, 0, 1, 1
};
#define PLAYER_DIE_FRAME_COUNT 11
static int PLAYER_DIE_OFFSET[8][11] = {  // [direction][frame_index]
    {0x23B6B, 0x23EDB, 0x2424B, 0x245BB, 0x2492B, 0x2492B, 0x2492B, 0x2492B, 0x2492B, 0x2492B, 0x2492B},
    {0x23B6B, 0x23EDB, 0x2424B, 0x245BB, 0x2492B, 0x2492B, 0x2492B, 0x2492B, 0x2492B, 0x2492B, 0x2492B},
    {0x24C9B, 0x25283, 0x2586B, 0x25E53, 0x2643B, 0x2643B, 0x2643B, 0x2643B, 0x2643B, 0x2643B, 0x2643B},
    {0x24C9B, 0x25283, 0x2586B, 0x25E53, 0x2643B, 0x2643B, 0x2643B, 0x2643B, 0x2643B, 0x2643B, 0x2643B},
    {0x26A23, 0x26E17, 0x2720B, 0x275FF, 0x279F3, 0x279F3, 0x279F3, 0x279F3, 0x279F3, 0x279F3, 0x279F3},
    {0x26A23, 0x26E17, 0x2720B, 0x275FF, 0x279F3, 0x279F3, 0x279F3, 0x279F3, 0x279F3, 0x279F3, 0x279F3},
    {0x24C9B, 0x25283, 0x2586B, 0x25E53, 0x2643B, 0x2643B, 0x2643B, 0x2643B, 0x2643B, 0x2643B, 0x2643B},
    {0x24C9B, 0x25283, 0x2586B, 0x25E53, 0x2643B, 0x2643B, 0x2643B, 0x2643B, 0x2643B, 0x2643B, 0x2643B}
};


#define BLOOD_1_WIDTH 32
#define BLOOD_1_HEIGHT 28
#define BLOOD_1_OFFSET 0x27DE7

#define BLOOD_2_WIDTH 27
#define BLOOD_2_HEIGHT 25
#define BLOOD_2_OFFSET 0x28167

#define BLOOD_3_WIDTH 25
#define BLOOD_3_HEIGHT 25
#define BLOOD_3_OFFSET 0x2840A

#define REMAIN_BLOOD_WIDTH 25
#define REMAIN_BLOOD_HEIGHT 10
#define REMAIN_BLOOD_OFFSET 0x2867B

#define GUN_LINE_WIDTH 40
#define GUN_LINE_HEIGHT 32
#define GUN_LINE_OFFSET 0x28775

#define P1_WIDTH 15
#define P1_HEIGHT 13
#define P1_OFFSET 0x28C75

#define P2_WIDTH 15
#define P2_HEIGHT 13
#define P2_OFFSET 0x28D38

#define WALL_WIDTH 71
#define WALL_HEIGHT 17
#define WALL_OFFSET 0x28DFB

#endif  // RESOURCE_H_
