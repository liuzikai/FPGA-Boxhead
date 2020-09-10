#ifndef RESOURCE_H_
#define RESOURCE_H_

static int ZOMBIE_WALK_WIDTH[8] = {  // [direction]
    26, 33, 27, 33, 26, 33, 27, 33
};
static int ZOMBIE_WALK_HEIGHT[8] = {  // [direction]
    41, 43, 39, 39, 36, 39, 39, 43
};
static int ZOMBIE_WALK_FLIP_X[8] = {  // [direction]
    0, 0, 0, 0, 0, 1, 1, 1
};
#define ZOMBIE_WALK_FRAME_COUNT 8
static int ZOMBIE_WALK_OFFSET[8][8] = {  // [direction][frame_index]
    {0x00000, 0x0042A, 0x00854, 0x00C7E, 0x010A8, 0x014D2, 0x018FC, 0x01D26},
    {0x02150, 0x026DB, 0x02C66, 0x031F1, 0x0377C, 0x03D07, 0x04292, 0x0481D},
    {0x04DA8, 0x051C5, 0x055E2, 0x059FF, 0x05E1C, 0x06239, 0x06656, 0x06A73},
    {0x06E90, 0x07397, 0x0789E, 0x07DA5, 0x082AC, 0x087B3, 0x08CBA, 0x091C1},
    {0x096C8, 0x09A70, 0x09E18, 0x0A1C0, 0x0A568, 0x0A910, 0x0ACB8, 0x0B060},
    {0x06E90, 0x07397, 0x0789E, 0x07DA5, 0x082AC, 0x087B3, 0x08CBA, 0x091C1},
    {0x04DA8, 0x051C5, 0x055E2, 0x059FF, 0x05E1C, 0x06239, 0x06656, 0x06A73},
    {0x02150, 0x026DB, 0x02C66, 0x031F1, 0x0377C, 0x03D07, 0x04292, 0x0481D}
};


static int ZOMBIE_ATTACK_WIDTH[8] = {  // [direction]
    28, 46, 42, 46, 28, 46, 42, 46
};
static int ZOMBIE_ATTACK_HEIGHT[8] = {  // [direction]
    41, 43, 40, 48, 45, 48, 40, 43
};
static int ZOMBIE_ATTACK_FLIP_X[8] = {  // [direction]
    0, 0, 0, 0, 0, 1, 1, 1
};
#define ZOMBIE_ATTACK_FRAME_COUNT 24
static int ZOMBIE_ATTACK_OFFSET[8][24] = {  // [direction][frame_index]
    {0x0B408, 0x0B408, 0x0B408, 0x0B408, 0x0B408, 0x0B408, 0x0B884, 0x0B884, 0x0B884, 0x0B884, 0x0B884, 0x0B884, 0x0BD00, 0x0BD00, 0x0BD00, 0x0BD00, 0x0BD00, 0x0BD00, 0x0C17C, 0x0C17C, 0x0C17C, 0x0C17C, 0x0C17C, 0x0C17C},
    {0x0C5F8, 0x0C5F8, 0x0C5F8, 0x0C5F8, 0x0C5F8, 0x0C5F8, 0x0CDB2, 0x0CDB2, 0x0CDB2, 0x0CDB2, 0x0CDB2, 0x0CDB2, 0x0D56C, 0x0D56C, 0x0D56C, 0x0D56C, 0x0D56C, 0x0D56C, 0x0DD26, 0x0DD26, 0x0DD26, 0x0DD26, 0x0DD26, 0x0DD26},
    {0x0E4E0, 0x0E4E0, 0x0E4E0, 0x0E4E0, 0x0E4E0, 0x0E4E0, 0x0EB70, 0x0EB70, 0x0EB70, 0x0EB70, 0x0EB70, 0x0EB70, 0x0F200, 0x0F200, 0x0F200, 0x0F200, 0x0F200, 0x0F200, 0x0F890, 0x0F890, 0x0F890, 0x0F890, 0x0F890, 0x0F890},
    {0x0FF20, 0x0FF20, 0x0FF20, 0x0FF20, 0x0FF20, 0x0FF20, 0x107C0, 0x107C0, 0x107C0, 0x107C0, 0x107C0, 0x107C0, 0x11060, 0x11060, 0x11060, 0x11060, 0x11060, 0x11060, 0x11900, 0x11900, 0x11900, 0x11900, 0x11900, 0x11900},
    {0x121A0, 0x121A0, 0x121A0, 0x121A0, 0x121A0, 0x121A0, 0x1268C, 0x1268C, 0x1268C, 0x1268C, 0x1268C, 0x1268C, 0x12B78, 0x12B78, 0x12B78, 0x12B78, 0x12B78, 0x12B78, 0x13064, 0x13064, 0x13064, 0x13064, 0x13064, 0x13064},
    {0x0FF20, 0x0FF20, 0x0FF20, 0x0FF20, 0x0FF20, 0x0FF20, 0x107C0, 0x107C0, 0x107C0, 0x107C0, 0x107C0, 0x107C0, 0x11060, 0x11060, 0x11060, 0x11060, 0x11060, 0x11060, 0x11900, 0x11900, 0x11900, 0x11900, 0x11900, 0x11900},
    {0x0E4E0, 0x0E4E0, 0x0E4E0, 0x0E4E0, 0x0E4E0, 0x0E4E0, 0x0EB70, 0x0EB70, 0x0EB70, 0x0EB70, 0x0EB70, 0x0EB70, 0x0F200, 0x0F200, 0x0F200, 0x0F200, 0x0F200, 0x0F200, 0x0F890, 0x0F890, 0x0F890, 0x0F890, 0x0F890, 0x0F890},
    {0x0C5F8, 0x0C5F8, 0x0C5F8, 0x0C5F8, 0x0C5F8, 0x0C5F8, 0x0CDB2, 0x0CDB2, 0x0CDB2, 0x0CDB2, 0x0CDB2, 0x0CDB2, 0x0D56C, 0x0D56C, 0x0D56C, 0x0D56C, 0x0D56C, 0x0D56C, 0x0DD26, 0x0DD26, 0x0DD26, 0x0DD26, 0x0DD26, 0x0DD26}
};


static int PLAYER_WALK_WIDTH[8] = {  // [direction]
    26, 39, 37, 28, 26, 41, 36, 27
};
static int PLAYER_WALK_HEIGHT[8] = {  // [direction]
    39, 42, 39, 43, 42, 42, 39, 42
};
static int PLAYER_WALK_FLIP_X[8] = {  // [direction]
    0, 0, 0, 0, 0, 0, 0, 0
};
#define PLAYER_WALK_FRAME_COUNT 8
static int PLAYER_WALK_OFFSET[8][8] = {  // [direction][frame_index]
    {0x13550, 0x13946, 0x13D3C, 0x14132, 0x14528, 0x1491E, 0x14D14, 0x1510A},
    {0x15500, 0x15B66, 0x161CC, 0x16832, 0x16E98, 0x174FE, 0x17B64, 0x181CA},
    {0x18830, 0x18DD3, 0x19376, 0x19919, 0x19EBC, 0x1A45F, 0x1AA02, 0x1AFA5},
    {0x1B548, 0x1B9FC, 0x1BEB0, 0x1C364, 0x1C818, 0x1CCCC, 0x1D180, 0x1D634},
    {0x1DAE8, 0x1DF2C, 0x1E370, 0x1E7B4, 0x1EBF8, 0x1F03C, 0x1F480, 0x1F8C4},
    {0x1FD08, 0x203C2, 0x20A7C, 0x21136, 0x217F0, 0x21EAA, 0x22564, 0x22C1E},
    {0x232D8, 0x23854, 0x23DD0, 0x2434C, 0x248C8, 0x24E44, 0x253C0, 0x2593C},
    {0x25EB8, 0x26326, 0x26794, 0x26C02, 0x27070, 0x274DE, 0x2794C, 0x27DBA}
};


#endif  // RESOURCE_H_
