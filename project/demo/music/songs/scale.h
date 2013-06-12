#ifndef _SCALE_H_
#define _SCALE_H_

#include "framework/musicmanager/musicmanager.h"
#include "framework/musicmanager/synth.h"

enum E_INSTRUMENTS_SCALE{
        E_INST_SCALE_PIANO = 0,
        E_INST_SCALE_BOOM,

        E_INST_SCALE_COUNT,
};

enum E_PATTERNS_SCALE{
        E_PATT_SCALE_MELODY_0 = 0,
        E_PATT_SCALE_BOOM_0,
        E_PATT_SCALE_MELODY_1,
        E_PATT_SCALE_BOOM_1,
        
        E_PATT_SCALE_COUNT,
};

enum E_TRACKS_SCALE{
        E_TRACK_SCALE_BOOM = 0,
        E_TRACK_SCALE_MELODY,

        E_TRACK_SCALE_COUNT,
};

void initScale(struct S_song *s);

#endif