#ifndef _MUSICMANAGER_H_
#define _MUSICMANAGER_H_

#include <SDL/SDL.h>
#include <GL/gl.h>

// length of patterns
#define PTLEN 16


struct S_instrumentFreq{
        GLfloat freq;
        GLfloat volume;
};

struct S_songInstrument{
        GLfloat (*waveCallback)(GLfloat t);

        // ADSR enevelope info - length of each phase
        GLint a, d, s, r;
        // amplitude of sustain phase <0,1>
        GLfloat sAmplitude;

        GLint mod, sweep;
        GLfloat LRchannelsWeight, pola;

        // instrument value
        GLint len;
        GLfloat *instr;
};

struct S_songPattern{
        // notes to be played each pattern might have 16 notes
        GLint notes[PTLEN];
};

struct S_songTrack{
        GLint patternsCount;

        // string containing pattern
        GLint patterns[100];
        GLint instrumentID;

        GLint LastPlayedNote;

        //mute track
        GLint mute, nextmute, echoOn, transp;
        GLint trackPos, patternPos, offset;
        GLfloat *echo, freq, prev, vol, slide;
};

struct S_song{
        GLint BPM;

        GLint instrumetsCount;
        GLint patternsCount;
        GLint tracksCount;

        // input informations about song
        struct S_songInstrument *instruments;
        struct S_songPattern *patterns;
        struct S_songTrack *tracks;
};


void startSong(struct S_song *s);
void stopSong();

void initMusicManager();

#endif