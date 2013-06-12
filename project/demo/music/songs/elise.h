#ifndef _ELISE_H_
#define _ELISE_H_

#include "framework/musicmanager/musicmanager.h"
#include "framework/musicmanager/synth.h"

enum E_INSTRUMENTS_ELISE{
        E_INST_ELISE_PIANO = 0,

        E_INST_ELISE_COUNT,
};

enum E_PATTERNS_ELISE{
        // beginning
        E_PATT_ELISE_RIGHT_0 = 0,
        E_PATT_ELISE_RIGHT_1,
        E_PATT_ELISE_RIGHT_2,
        E_PATT_ELISE_RIGHT_3,
        E_PATT_ELISE_RIGHT_4,
        E_PATT_ELISE_RIGHT_5,
        E_PATT_ELISE_LEFT_0,
        E_PATT_ELISE_LEFT_1,
        E_PATT_ELISE_LEFT_2,
        E_PATT_ELISE_LEFT_3,
        E_PATT_ELISE_LEFT_4,
        E_PATT_ELISE_LEFT_5,
        // end beginning

        E_PATT_ELISE_RIGHT_6,
        E_PATT_ELISE_RIGHT_7,
        E_PATT_ELISE_RIGHT_8,
        E_PATT_ELISE_RIGHT_9,
        E_PATT_ELISE_RIGHT_10,
        E_PATT_ELISE_RIGHT_11,
        E_PATT_ELISE_RIGHT_12,
        E_PATT_ELISE_RIGHT_13,
        E_PATT_ELISE_RIGHT_14,
        E_PATT_ELISE_RIGHT_15,
        E_PATT_ELISE_RIGHT_16,
        E_PATT_ELISE_RIGHT_17,
        E_PATT_ELISE_LEFT_6,
        E_PATT_ELISE_LEFT_7,
        E_PATT_ELISE_LEFT_8,
        E_PATT_ELISE_LEFT_9,
        E_PATT_ELISE_LEFT_10,
        E_PATT_ELISE_LEFT_11,
        E_PATT_ELISE_LEFT_12,
        E_PATT_ELISE_LEFT_13,
        E_PATT_ELISE_LEFT_14,
        E_PATT_ELISE_LEFT_15,
        E_PATT_ELISE_LEFT_16,
        E_PATT_ELISE_LEFT_17,

        E_PATT_ELISE_COUNT,
};

enum E_TRACKS_ELISE{
        E_TRACK_ELISE_MELODY_LEFT = 0,
        E_TRACK_ELISE_MELODY_RIGHT,

        E_TRACK_ELISE_COUNT,
};

void initElise(struct S_song *s);


#endif