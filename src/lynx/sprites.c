#include <lynx.h>
#include <tgi.h>

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

//#include "../standard_lib.h"

#include "sprites.h"


// Sprite data arrays
unsigned char *ship_2p_v_data[4] = {&ship_2p_v2_spr[0], &ship_2p_v3_spr[0], &ship_2p_v4_spr[0], &ship_2p_v5_spr[0]};
unsigned char *ship_2p_h_data[4] = {&ship_2p_h2_spr[0], &ship_2p_h3_spr[0], &ship_2p_h4_spr[0], &ship_2p_h5_spr[0]};
unsigned char *ship_4p_v_data[4] = {&ship_4p_v2_spr[0], &ship_4p_v3_spr[0], &ship_4p_v4_spr[0], &ship_4p_v5_spr[0]};
unsigned char *ship_4p_h_data[4] = {&ship_4p_h2_spr[0], &ship_4p_h3_spr[0], &ship_4p_h4_spr[0], &ship_4p_h5_spr[0]};
unsigned char *attack_2p_data[6] = {&attack_2p_1_spr[0], &attack_2p_2_spr[0], &attack_2p_3_spr[0], &attack_2p_4_spr[0], &attack_2p_4_spr[0], &hit_2p_1_spr[0]};
unsigned char *attack_4p_data[6] = {&attack_2p_2_spr[0], &attack_2p_2_spr[0], &attack_2p_3_spr[0], &attack_2p_4_spr[0], &attack_2p_4_spr[0], &hit_4p_1_spr[0]};
unsigned char *hit_2p_data[2] = {&hit_2p_1_spr[0], &hit_2p_2_spr[0]};
unsigned char *hit_4p_data[2] = {&hit_4p_1_spr[0], &hit_4p_1_spr[0]};
unsigned char *cursor_2p_data[3] = {&cursor_2p_1_spr[0], &cursor_2p_2_spr[0], &cursor_2p_2_spr[0]};
unsigned char *cursor_4p_data[3] = {&cursor_4p_1_spr[0], &cursor_4p_2_spr[0], &cursor_4p_2_spr[0]};


SCB_REHV_PAL frame_sprite = {
    BPP_4 | TYPE_NORMAL,
    REHV,
    0x01,
    0,
    NULL,
    0, 0,
    0x0100, 0x0100,
    { 0xEC, 0x5A, 0x0F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF }
};

SCB_REHV_PAL ship_sprite = {
    BPP_4 | TYPE_NORMAL,
    REHV,
    0x01,
    0,
    NULL,
    0, 0,
    0x0100, 0x0100,
    { 0x11, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }
};

SCB_REHV_PAL attack_sprite = {
    BPP_4 | TYPE_NORMAL,
    REHV,
    0x01,
    0,
    NULL,
    0, 0,
    0x0100, 0x0100,
    { 0x15, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }
};

SCB_REHV_PAL hit_sprite = {
    BPP_4 | TYPE_NORMAL,
    REHV,
    0x01,
    0,
    NULL,
    0, 0,
    0x0100, 0x0100,
    { 0x29, 0x03, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF }
};

SCB_REHV_PAL miss_sprite = {
    BPP_4 | TYPE_NORMAL,
    REHV,
    0x01,
    0,
    NULL,
    0, 0,
    0x0100, 0x0100,
    { 0xC0, 0x03, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF }
};

SCB_REHV_PAL cursor_sprite = {
    BPP_4 | TYPE_NORMAL,
    REHV,
    0x01,
    0,
    NULL,
    0, 0,
    0x0100, 0x0100,
    { 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }
};

SCB_REHV_PAL player_sprite = {
    BPP_1 | TYPE_NORMAL,
    REHV,
    0x01,
    0,
    (unsigned char *) &player_spr,
    0, 0,
    0x0100, 0x0100,
    { 0x01, 0x23, 0x45, 0x00, 0x00, 0x00, 0x00, 0x00 }
};

SCB_REHV_PAL clock_sprite = {
    BPP_4 | TYPE_NORMAL,
    REHV,
    0x01,
    0,
    (unsigned char *) &clock_spr,
    0, 0,
    0x0100, 0x0100,
    { 0x01, 0x23, 0x45, 0x00, 0x00, 0x00, 0x00, 0x00 }
};

SCB_REHV_PAL mark_sprite = {
    BPP_4 | TYPE_NORMAL,
    REHV,
    0x01,
    0,
    (unsigned char *) &mark_spr,
    0, 0,
    0x0100, 0x0100,
    { 0x01, 0x23, 0x45, 0x00, 0x00, 0x00, 0x00, 0x00 }
};

SCB_REHV_PAL text_cursor_sprite = {
    BPP_1 | TYPE_NORMAL,
    REHV,
    0x01,
    0,
    (unsigned char*) &text_cursor_spr,
    0, 0,
    0x0100, 0x0100,
    { 0x01, 0x23, 0x45, 0x00, 0x00, 0x00, 0x00, 0x00 }
};