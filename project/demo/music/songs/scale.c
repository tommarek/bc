#include "scale.h"

#include "framework/util.h"

void initScale(struct S_song *s){

	s->instrumetsCount = E_INST_SCALE_COUNT;
	s->patternsCount = E_PATT_SCALE_COUNT;
	s->tracksCount = E_TRACK_SCALE_COUNT;

	s->instruments = checkMalloc(sizeof(struct S_songInstrument) * s->instrumetsCount);
	s->patterns = checkMalloc(sizeof(struct S_songPattern) * s->patternsCount);
	s->tracks = checkMalloc(sizeof(struct S_songTrack) * s->tracksCount);

	//// bpm
	s->BPM = 20;

	////instruments
	// piano for main melody
	s->instruments[E_INST_SCALE_PIANO].waveCallback = pianoCallback;
	s->instruments[E_INST_SCALE_PIANO].a = 100;
	s->instruments[E_INST_SCALE_PIANO].d = 12;
	s->instruments[E_INST_SCALE_PIANO].s = 10000;
	s->instruments[E_INST_SCALE_PIANO].sAmplitude = 0.8;
	s->instruments[E_INST_SCALE_PIANO].r = 30000;
	s->instruments[E_INST_SCALE_PIANO].mod = 100;
	s->instruments[E_INST_SCALE_PIANO].sweep = 0;
	s->instruments[E_INST_SCALE_PIANO].LRchannelsWeight = 0.50;
	s->instruments[E_INST_SCALE_PIANO].pola = 0;
	// boom callback
	s->instruments[E_INST_SCALE_BOOM].waveCallback = sinCallback;
	s->instruments[E_INST_SCALE_BOOM].a = 10;
	s->instruments[E_INST_SCALE_BOOM].d = 12;
	s->instruments[E_INST_SCALE_BOOM].s = 10000;
	s->instruments[E_INST_SCALE_BOOM].sAmplitude = 2.0;
	s->instruments[E_INST_SCALE_BOOM].r = 5000;
	s->instruments[E_INST_SCALE_BOOM].mod = 100;
	s->instruments[E_INST_SCALE_BOOM].sweep = 0;
	s->instruments[E_INST_SCALE_BOOM].LRchannelsWeight = 0.50;
	s->instruments[E_INST_SCALE_BOOM].pola = 0;

	// pattern 1
	s->patterns[E_PATT_SCALE_MELODY_0].notes[ 0] = E_N_c3;
	s->patterns[E_PATT_SCALE_MELODY_0].notes[ 1] = E_N_d3;
	s->patterns[E_PATT_SCALE_MELODY_0].notes[ 2] = E_N_e3;
	s->patterns[E_PATT_SCALE_MELODY_0].notes[ 3] = E_N_f3;
	s->patterns[E_PATT_SCALE_MELODY_0].notes[ 4] = E_N_g3;
	s->patterns[E_PATT_SCALE_MELODY_0].notes[ 5] = E_N_a3;
	s->patterns[E_PATT_SCALE_MELODY_0].notes[ 6] = E_N_b3;
	s->patterns[E_PATT_SCALE_MELODY_0].notes[ 7] = E_N_c4;
	s->patterns[E_PATT_SCALE_MELODY_0].notes[ 8] = E_N_d4;
	s->patterns[E_PATT_SCALE_MELODY_0].notes[ 9] = E_N_e4;
	s->patterns[E_PATT_SCALE_MELODY_0].notes[10] = E_N_f4;
	s->patterns[E_PATT_SCALE_MELODY_0].notes[11] = E_N_g4;
	s->patterns[E_PATT_SCALE_MELODY_0].notes[12] = E_N_a4;
	s->patterns[E_PATT_SCALE_MELODY_0].notes[13] = E_N_b4;
	s->patterns[E_PATT_SCALE_MELODY_0].notes[14] = E_N_c5;
	s->patterns[E_PATT_SCALE_MELODY_0].notes[15] = E_N_EMPTY;

	s->patterns[E_PATT_SCALE_MELODY_1].notes[ 0] = E_N_c5;
	s->patterns[E_PATT_SCALE_MELODY_1].notes[ 1] = E_N_b4;
	s->patterns[E_PATT_SCALE_MELODY_1].notes[ 2] = E_N_a4;
	s->patterns[E_PATT_SCALE_MELODY_1].notes[ 3] = E_N_g4;
	s->patterns[E_PATT_SCALE_MELODY_1].notes[ 4] = E_N_f4;
	s->patterns[E_PATT_SCALE_MELODY_1].notes[ 5] = E_N_e4;
	s->patterns[E_PATT_SCALE_MELODY_1].notes[ 6] = E_N_d4;
	s->patterns[E_PATT_SCALE_MELODY_1].notes[ 7] = E_N_c4;
	s->patterns[E_PATT_SCALE_MELODY_1].notes[ 8] = E_N_b3;
	s->patterns[E_PATT_SCALE_MELODY_1].notes[ 9] = E_N_a3;
	s->patterns[E_PATT_SCALE_MELODY_1].notes[10] = E_N_g3;
	s->patterns[E_PATT_SCALE_MELODY_1].notes[11] = E_N_f3;
	s->patterns[E_PATT_SCALE_MELODY_1].notes[12] = E_N_e3;
	s->patterns[E_PATT_SCALE_MELODY_1].notes[13] = E_N_d3;
	s->patterns[E_PATT_SCALE_MELODY_1].notes[14] = E_N_c3;
	s->patterns[E_PATT_SCALE_MELODY_1].notes[15] = E_N_EMPTY;

	s->patterns[E_PATT_SCALE_BOOM_0].notes[ 0] = E_N_EMPTY;
	s->patterns[E_PATT_SCALE_BOOM_0].notes[ 1] = E_N_EMPTY;
	s->patterns[E_PATT_SCALE_BOOM_0].notes[ 2] = E_N_EMPTY;
	s->patterns[E_PATT_SCALE_BOOM_0].notes[ 3] = E_N_EMPTY;
	s->patterns[E_PATT_SCALE_BOOM_0].notes[ 4] = E_N_EMPTY;
	s->patterns[E_PATT_SCALE_BOOM_0].notes[ 5] = E_N_EMPTY;
	s->patterns[E_PATT_SCALE_BOOM_0].notes[ 6] = E_N_EMPTY;
	s->patterns[E_PATT_SCALE_BOOM_0].notes[ 7] = E_N_EMPTY;
	s->patterns[E_PATT_SCALE_BOOM_0].notes[ 8] = E_N_EMPTY;
	s->patterns[E_PATT_SCALE_BOOM_0].notes[ 9] = E_N_EMPTY;
	s->patterns[E_PATT_SCALE_BOOM_0].notes[10] = E_N_EMPTY;
	s->patterns[E_PATT_SCALE_BOOM_0].notes[11] = E_N_EMPTY;
	s->patterns[E_PATT_SCALE_BOOM_0].notes[12] = E_N_EMPTY;
	s->patterns[E_PATT_SCALE_BOOM_0].notes[13] = E_N_EMPTY;
	s->patterns[E_PATT_SCALE_BOOM_0].notes[14] = E_N_EMPTY;
	s->patterns[E_PATT_SCALE_BOOM_0].notes[15] = E_N_EMPTY;

	s->patterns[E_PATT_SCALE_BOOM_1].notes[ 0] = E_N_EMPTY;
	s->patterns[E_PATT_SCALE_BOOM_1].notes[ 1] = E_N_EMPTY;
	s->patterns[E_PATT_SCALE_BOOM_1].notes[ 2] = E_N_EMPTY;
	s->patterns[E_PATT_SCALE_BOOM_1].notes[ 3] = E_N_EMPTY;
	s->patterns[E_PATT_SCALE_BOOM_1].notes[ 4] = E_N_EMPTY;
	s->patterns[E_PATT_SCALE_BOOM_1].notes[ 5] = E_N_EMPTY;
	s->patterns[E_PATT_SCALE_BOOM_1].notes[ 6] = E_N_EMPTY;
	s->patterns[E_PATT_SCALE_BOOM_1].notes[ 7] = E_N_EMPTY;
	s->patterns[E_PATT_SCALE_BOOM_1].notes[ 8] = E_N_EMPTY;
	s->patterns[E_PATT_SCALE_BOOM_1].notes[ 9] = E_N_EMPTY;
	s->patterns[E_PATT_SCALE_BOOM_1].notes[10] = E_N_EMPTY;
	s->patterns[E_PATT_SCALE_BOOM_1].notes[11] = E_N_EMPTY;
	s->patterns[E_PATT_SCALE_BOOM_1].notes[12] = E_N_EMPTY;
	s->patterns[E_PATT_SCALE_BOOM_1].notes[13] = E_N_EMPTY;
	s->patterns[E_PATT_SCALE_BOOM_1].notes[14] = E_N_EMPTY;
	s->patterns[E_PATT_SCALE_BOOM_1].notes[15] = E_N_c2;



	s->tracks[E_TRACK_SCALE_MELODY].instrumentID = E_INST_SCALE_PIANO;
	s->tracks[E_TRACK_SCALE_MELODY].patternsCount = 3;
	s->tracks[E_TRACK_SCALE_MELODY].patterns[ 0] = E_PATT_SCALE_MELODY_0;
	s->tracks[E_TRACK_SCALE_MELODY].patterns[ 1] = E_PATT_SCALE_MELODY_1;
	s->tracks[E_TRACK_SCALE_MELODY].patterns[ 2] = END;

	s->tracks[E_TRACK_SCALE_BOOM].instrumentID = E_INST_SCALE_BOOM;
	s->tracks[E_TRACK_SCALE_BOOM].patternsCount = 3;
	s->tracks[E_TRACK_SCALE_BOOM].patterns[ 0] = E_PATT_SCALE_BOOM_0;
	s->tracks[E_TRACK_SCALE_BOOM].patterns[ 1] = E_PATT_SCALE_BOOM_1;
	s->tracks[E_TRACK_SCALE_BOOM].patterns[ 2] = END;
}