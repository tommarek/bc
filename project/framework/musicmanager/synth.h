#ifndef _SYNTH_H_
#define _SYNTH_H_

#include <GL/gl.h>

#include "framework/musicmanager/musicmanager.h"

// special playable notes
#define END -2
#define LOOP -3
#define ECHON -4
#define ECHOFF -5
#define STOP -6
#define VOL -7
#define SLIDE -8
#define MUTEON -9
#define MUTEOFF -10
// end special playable notes

#define SAMPLERATE 44100
#define MAXPLAYINGNOTES 32


// all playable notes names (lower case notes are sharp, high are flat)
enum E_NOTES{
	E_N_c1 = 0, E_N_C1 , E_N_d1 , E_N_D1 , E_N_e1 , E_N_f1 , E_N_F1 , E_N_g1 , E_N_G1 , E_N_a1 , E_N_A1 , E_N_b1 ,
        E_N_c2    , E_N_C2 , E_N_d2 , E_N_D2 , E_N_e2 , E_N_f2 , E_N_F2 , E_N_g2 , E_N_G2 , E_N_a2 , E_N_A2 , E_N_b2 ,
	E_N_c3    , E_N_C3 , E_N_d3 , E_N_D3 , E_N_e3 , E_N_f3 , E_N_F3 , E_N_g3 , E_N_G3 , E_N_a3 , E_N_A3 , E_N_b3 ,
	E_N_c4    , E_N_C4 , E_N_d4 , E_N_D4 , E_N_e4 , E_N_f4 , E_N_F4 , E_N_g4 , E_N_G4 , E_N_a4 , E_N_A4 , E_N_b4 ,
	E_N_c5    , E_N_C5 , E_N_d5 , E_N_D5 , E_N_e5 , E_N_f5 , E_N_F5 , E_N_g5 , E_N_G5 , E_N_a5 , E_N_A5 , E_N_b5 ,
	E_N_c6    , E_N_C6 , E_N_d6 , E_N_D6 , E_N_e6 , E_N_f6 , E_N_F6 , E_N_g6 , E_N_G6 , E_N_a6 , E_N_A6 , E_N_b6 ,
        // Special "notes"
        E_N_ECHON, E_N_ECHOFF, E_N_STOP, E_N_VOL, E_N_SLIDE, E_N_EMPTY,

};


//instruments callbacks
GLfloat sinCallback(GLfloat t);
GLfloat pianoCallback(GLfloat t);
GLfloat squareCallback(GLfloat t);
GLfloat sawCallback(GLfloat t);
GLfloat whiteNoiseCallback(GLfloat t);

void synthInit(struct S_song *s);

void synthLoad(struct S_song *song);
void synthPlay(struct S_song *s, GLshort *dest, GLint len);

void synthSetVolume(struct S_song *s, GLint instr, GLfloat newVolume);
void synthSetNote(struct S_song *s, GLint instr, GLint note);
void synthMute(struct S_song *s, GLint instr, GLint nmute, GLint delay);
void synthTranspose(struct S_song *s, GLint instr, GLint transp);

#endif
