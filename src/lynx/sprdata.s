    .export _frame_2p_spr, _frame_4p_spr

    .export _ship_2p_v2_spr, _ship_2p_v3_spr, _ship_2p_v4_spr, _ship_2p_v5_spr
    .export _ship_4p_v2_spr, _ship_4p_v3_spr, _ship_4p_v4_spr, _ship_4p_v5_spr
    .export _ship_2p_h2_spr, _ship_2p_h3_spr, _ship_2p_h4_spr, _ship_2p_h5_spr
    .export _ship_4p_h2_spr, _ship_4p_h3_spr, _ship_4p_h4_spr, _ship_4p_h5_spr

    .export _attack_2p_1_spr, _attack_2p_2_spr, _attack_2p_3_spr, _attack_2p_4_spr
;    .export _attack_4p_1_spr, _attack_4p_2_spr, _attack_4p_3_spr, _attck_4p_4_spr

    .export _cursor_2p_1_spr
    .export _cursor_4p_1_spr

    .export _hit_2p_1_spr, _hit_2p_2_spr
    .export _hit_4p_1_spr;, _hit_4p_2_spr
    .export _miss_2p_spr, _miss_4p_spr

    .export _player_spr, _clock_spr, _mark_spr, _text_cursor_spr, _connection_spr


    .rodata

_frame_2p_spr:
    .incbin "./sprdata/2p_frame.spr"
_frame_4p_spr:
    .incbin "./sprdata/4p_frame.spr"

_ship_2p_v2_spr:
    .incbin "./sprdata/2p_2_ship.spr"
_ship_2p_v3_spr:
    .incbin "./sprdata/2p_3_ship.spr"
_ship_2p_v4_spr:
    .incbin "./sprdata/2p_4_ship.spr"
_ship_2p_v5_spr:
    .incbin "./sprdata/2p_5_ship.spr"
_ship_2p_h2_spr:
    .incbin "./sprdata/2p_2_ship90.spr"
_ship_2p_h3_spr:
    .incbin "./sprdata/2p_3_ship90.spr"
_ship_2p_h4_spr:
    .incbin "./sprdata/2p_4_ship90.spr"
_ship_2p_h5_spr:
    .incbin "./sprdata/2p_5_ship90.spr"

_ship_4p_v2_spr:
    .incbin "./sprdata/4p_2_ship.spr"
_ship_4p_v3_spr:
    .incbin "./sprdata/4p_3_ship.spr"
_ship_4p_v4_spr:
    .incbin "./sprdata/4p_4_ship.spr"
_ship_4p_v5_spr:
    .incbin "./sprdata/4p_5_ship.spr"
_ship_4p_h2_spr:
    .incbin "./sprdata/4p_2_ship90.spr"
_ship_4p_h3_spr:
    .incbin "./sprdata/4p_3_ship90.spr"
_ship_4p_h4_spr:
    .incbin "./sprdata/4p_4_ship90.spr"
_ship_4p_h5_spr:
    .incbin "./sprdata/4p_5_ship90.spr"

_attack_2p_1_spr:
    .incbin "./sprdata/2p_attack1.spr"
_attack_2p_2_spr:
    .incbin "./sprdata/2p_attack2.spr"
_attack_2p_3_spr:
    .incbin "./sprdata/2p_attack3.spr"
_attack_2p_4_spr:
    .incbin "./sprdata/2p_attack4.spr"

;_attack_4p_1_spr:
;    .incbin "./sprdata/4p_attack1.spr"
;_attack_4p_2_spr:
;    .incbin "./sprdata/4p_attack2.spr"
;_attack_4p_3_spr:
;    .incbin "./sprdata/4p_attack3.spr"
;_attack_4p_4_spr:
;    .incbin "./sprdata/4p_attack4.spr"

_cursor_2p_1_spr:
    .incbin "./sprdata/2p_cursor1.spr"
_cursor_4p_1_spr:
    .incbin "./sprdata/4p_cursor1.spr"

_hit_2p_1_spr:
    .incbin "./sprdata/2p_hit1.spr"
_hit_2p_2_spr:
    .incbin "./sprdata/2p_hit2.spr"
_hit_4p_1_spr:
    .incbin "./sprdata/4p_hit1.spr"
;_hit_4p_2_spr:
;    .incbin "./sprdata/4p_hit2.spr"

_miss_2p_spr:
    .incbin "./sprdata/2p_miss.spr"
_miss_4p_spr:
    .incbin "./sprdata/4p_miss.spr"

_player_spr:
    .incbin "./sprdata/player.spr"
_clock_spr:
    .incbin "./sprdata/clock.spr"
_mark_spr:
    .incbin "./sprdata/mark.spr"
_text_cursor_spr:
    .incbin "./sprdata/text_cursor.spr"
_connection_spr:
    .incbin "./sprdata/connection.spr"