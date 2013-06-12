#ifndef _SONG_H_
#define _SONG_H_

#include "framework/musicmanager/musicmanager.h"

#include "songs/elise.h"
#include "songs/scale.h"

#include "instruments/piano_instrument.h"

extern struct S_song elise;
extern struct S_song scale;


void initSongs();

#endif