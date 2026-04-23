#ifndef LYNX_SPRITES_H
#define LYNX_SPRITES_H


// Sprite data
extern unsigned char frame_2p_spr[];
extern unsigned char frame_4p_spr[];

extern unsigned char ship_2p_v2_spr[];
extern unsigned char ship_2p_v3_spr[];
extern unsigned char ship_2p_v4_spr[];
extern unsigned char ship_2p_v5_spr[];
extern unsigned char ship_2p_h2_spr[];
extern unsigned char ship_2p_h3_spr[];
extern unsigned char ship_2p_h4_spr[];
extern unsigned char ship_2p_h5_spr[];

extern unsigned char ship_4p_v2_spr[];
extern unsigned char ship_4p_v3_spr[];
extern unsigned char ship_4p_v4_spr[];
extern unsigned char ship_4p_v5_spr[];
extern unsigned char ship_4p_h2_spr[];
extern unsigned char ship_4p_h3_spr[];
extern unsigned char ship_4p_h4_spr[];
extern unsigned char ship_4p_h5_spr[];

extern unsigned char attack_2p_1_spr[];
extern unsigned char attack_2p_2_spr[];
extern unsigned char attack_2p_3_spr[];
extern unsigned char attack_2p_4_spr[];

extern unsigned char attack_4p_1_spr[];
extern unsigned char attack_4p_2_spr[];
extern unsigned char attack_4p_3_spr[];
extern unsigned char attack_4p_4_spr[];

extern unsigned char cursor_2p_1_spr[];
extern unsigned char cursor_2p_2_spr[];
extern unsigned char cursor_4p_1_spr[];
extern unsigned char cursor_4p_2_spr[];

extern unsigned char hit_2p_1_spr[];
extern unsigned char hit_2p_2_spr[];
extern unsigned char hit_4p_1_spr[];
extern unsigned char hit_4p_2_spr[];

extern unsigned char miss_2p_spr[];
extern unsigned char miss_4p_spr[];

extern unsigned char player_spr[];
extern unsigned char clock_spr[];
extern unsigned char mark_spr[];
extern unsigned char text_cursor_spr[];

// Sprite data arrays
extern unsigned char *ship_2p_v_data[4];
extern unsigned char *ship_2p_h_data[4];
extern unsigned char *ship_4p_v_data[4];
extern unsigned char *ship_4p_h_data[4];
extern unsigned char *attack_2p_data[6];
extern unsigned char *attack_4p_data[6];
extern unsigned char *hit_2p_data[2];
extern unsigned char *hit_4p_data[2];
extern unsigned char *cursor_2p_data[3];
extern unsigned char *cursor_4p_data[3];

// Sprite SCB structs
extern SCB_REHV_PAL frame_sprite;
extern SCB_REHV_PAL ship_sprite;
extern SCB_REHV_PAL attack_sprite;
extern SCB_REHV_PAL hit_sprite;
extern SCB_REHV_PAL miss_sprite;
extern SCB_REHV_PAL cursor_sprite;
extern SCB_REHV_PAL player_sprite;
extern SCB_REHV_PAL clock_sprite;
extern SCB_REHV_PAL mark_sprite;
extern SCB_REHV_PAL text_cursor_sprite;

#endif