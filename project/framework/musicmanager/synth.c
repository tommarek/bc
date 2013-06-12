/**
\file
 Software synthesizer
*/
#include "synth.h"

#include <math.h>

#include "framework/util.h"
#include "demo/music/song.h"

#define AMPLIFIER 2.0

GLint synthCounter,syna_row;

// next update in update if changed, new in nupdate
GLint update, nupdate;

// array containing all playing notes
struct S_playingNote{
        struct S_songInstrument *instrument;
        GLint offset; // offset in adsr envelope
        GLint freq; // note frequency
	GLfloat volume; // volume 0 - 1
};
struct S_playingNote playingNotes[MAXPLAYINGNOTES];
GLint playingNotesCount = 0;

GLint  echoLen, firstrow;

// values for all playable notes
static int noteFreq[]={
	0   , 0   , 0   , 0   ,     0   ,0   ,0   ,0   ,     0   ,0   ,0   ,0   ,
	0   , 0   , 0   , 0   ,     0   ,0   ,0   ,0   ,     0   ,0   ,0   ,0   ,
	0   , 0   , 0   , 0   ,     0   ,0   ,0   ,0   ,     0   ,0   ,0   ,0   ,
	0   , 0   , 0   , 0   ,     0   ,0   ,0   ,0   ,     0   ,0   ,0   ,0   ,
	0   , 0   , 0   , 0   ,     0   ,0   ,0   ,0   ,     0   ,0   ,0   ,0   ,
	1047, 1109, 1175, 1245,     1319,1397,1480,1568,     1661,1760,1864,1976,
	ECHON, ECHOFF, STOP, VOL, SLIDE, 0,
};

/**
 * Instrument callback. Generating piano-like signal
 * @param t time
 * @return piano-like with range between -1 and 1
 */
GLfloat pianoCallback(GLfloat t){
	GLint i = 0;
	GLfloat result = 0;
	t *= 2.0 * M_PI;
	while(piano_instrument[i].volume != 0){
		result += piano_instrument[i].volume * sinf(piano_instrument[i].freq * t);
		++i;
	}
	return result;
}

/**
 * Instrument callback. Generating sin signal
 * @param t time
 * @return sin signal with range between -1 and 1
 */
GLfloat sinCallback(GLfloat t){
	return sinf(t * 2 * M_PI);
}

/**
 * Instrument callback. Generating saw signal
 * @param t time
 * @return square signal with range between -1 and 1
 */
GLfloat squareCallback(GLfloat t){
	return (fmodf(t, 1) > 0.5) ? 1.0 : 0.8;
}

/**
 * Instrument callback. Generating saw signal
 * @param t time
 * @return saw signal with range between -1 and 1
 */
GLfloat sawCallback(GLfloat t){
	return (fmodf(t, 1) * 2 - 1);
}

/**
 * Instrument callback. Generating white noise
 * @param t time
 * @return white noise with range between -1 and 1
 */
GLfloat whiteNoiseCallback(GLfloat t){
	return (rand() / (GLfloat)RAND_MAX) * 2.0 - 1.0;
}

/**
 * initialise synth
 * @param frequency
 */
void synthInit(struct S_song *s){
	synthCounter = syna_row = 0;
	firstrow = 1;
	nupdate = -1;



	// Generate lower octave values from row 6
	for(int i = 4; i >=0; i--)
		for(int j = 0; j < 12; j++)
			noteFreq[i * 12 + j] = noteFreq[(i + 1) * 12 + j] / 2;

	for (int i = 0; i < s->tracksCount; ++i){
		s->tracks[i].offset = -1.0;
		s->tracks[i].freq = 0.0;
		s->tracks[i].trackPos = -1;
		s->tracks[i].patternPos = 0;
		s->tracks[i].echoOn = 0;
		s->tracks[i].vol = 1.0;
		s->tracks[i].prev = 0.0;
		s->tracks[i].slide = 0.0;
		s->tracks[i].transp = 0;
		s->tracks[i].freq = 0.0;
		s->tracks[i].mute = 0;
		s->tracks[i].nextmute = 0;
	}
}



/**
 * Internal function for synthLoad(). Makes ADSR envelope for instruments.
 * @param song which song
 * @param instrumentID id of instrument to make ADSR envelope
 */
static GLfloat synthADSR(struct S_songInstrument *instrument, GLint t){
	GLfloat delta;

	GLint a = instrument->a;
	GLint d = instrument->d;
	GLint s = instrument->s;
	GLfloat sAmplitude = instrument->sAmplitude;
	GLint r = instrument->r;

	// all phases must have at least one sample
	if (a == 0) a = 1;
	if (d == 0) d = 1;
	if (s == 0) s = 1;
	if (r == 0) r = 1;

	if (t <= a){
		delta = (1.0 / a) * t;
		return delta;
	}
	else if (t <= a + d){
		t = t - a;
		delta = 1.0 - sAmplitude;
		delta /= d;
		delta *= t;
		delta = 1.0 - delta;
		return delta;
	}
	else if(t <= a + d + s){
		return sAmplitude;
	}
	else if(t <= a + d + s + r){
		t = t - a - d - s;
		delta = sAmplitude / r;
		delta *= t;
		delta = sAmplitude - delta;
		return delta;
	}
	else{
		return 0;
	}
}

/**
 * use song and create all needed data
 * @param song Target song
 */
void synthLoad(struct S_song *song){
	// BPM
	update = ((SAMPLERATE * 60) / song->BPM) / (PTLEN / 4); // samplerate / bps , (PTLEN / 4) - number of quarter notes in one pattern

	// instruments
	for (int i = 0 ; i < song->instrumetsCount; ++i){
		// get length of instruments note and malloc space for it
		song->instruments[i].len = song->instruments[i].a + song->instruments[i].d + song->instruments[i].s + song->instruments[i].r;
	}
}

/**
 * plays fragment to destination
 * @param dest destination buffer
 * @param len size of buffer
 */
void synthPlay(struct S_song *s, GLshort *dest, GLint len){
	GLint note, n;

	// check if bpm changed
	if (nupdate != -1){
		update = nupdate;
		nupdate = -1;
	}

	// go through len
	for (n = 0; n < len; ++n, ++synthCounter){
		// only update on right time
		if (synthCounter % update == 0){
			if (firstrow)
				firstrow = 0;
			else
				++synthCounter;

			// go through all patterns
			for (int i = 0; i < s->tracksCount; ++i){
				// muting if set
				s->tracks[i].mute = s->tracks[i].nextmute;

				// if track info is set to end, skip track
				if (s->tracks[i].trackPos == END)
					continue;

				// position +1
				s->tracks[i].patternPos += 1;

				// if trackPosition == -1 or note == END
				if (s->tracks[i].trackPos == -1 ||  s->tracks[i].patternPos >= PTLEN){
					// inc position
					++(s->tracks[i].trackPos);

					// if loop or end occured
					if (s->tracks[i].patterns[s->tracks[i].trackPos] == LOOP)
						s->tracks[i].trackPos = 0;
					else if (s->tracks[i].patterns[s->tracks[i].trackPos] == END)
						s->tracks[i].trackPos = END;

					// set pattern position to 0
					s->tracks[i].patternPos = 0;
				}

				if (s->tracks[i].trackPos != END){
					// get note
					note = s->patterns[s->tracks[i].patterns[s->tracks[i].trackPos]].notes[s->tracks[i].patternPos];
					s->tracks[i].LastPlayedNote = -1;
					// decide what to do with note
					switch(note){
						case E_N_EMPTY	: break;

						case E_N_STOP	: // not implemented
								  break;

						case E_N_ECHON	: // not implemented
								  break;

						case E_N_ECHOFF	: // not implemented
								  break;

						case E_N_VOL	: s->tracks[i].patternPos += 1;
								  s->tracks[i].vol = note / 100.0;
								  break;

						case E_N_SLIDE	:// not implemented
								  break;

						default		: if (s->tracks[i].mute) break;
								  note += s->tracks[i].transp;
								  if (note < E_N_c1) note = E_N_c1;
								  if (note > E_N_b6) note = E_N_b6;
								  // save note for sequence manager
								  s->tracks[i].LastPlayedNote = note;
								  // pick note freq in table of notes
								  playingNotes[playingNotesCount].freq = noteFreq[note];
								  playingNotes[playingNotesCount].offset = 0;
								  playingNotes[playingNotesCount].instrument = &(s->instruments[s->tracks[i].instrumentID]);
								  playingNotes[playingNotesCount].volume = s->tracks[i].vol;
								  playingNotesCount++;
					}

				}
			}
		}
		// sum instruments to output
		GLfloat left = 0.0, right = 0.0;
		for (int i = 0; i < playingNotesCount; ++i){
			double smp = (playingNotes[i].instrument->waveCallback)(((GLfloat)playingNotes[i].offset * playingNotes[i].freq) / SAMPLERATE)
				* synthADSR(playingNotes[i].instrument, playingNotes[i].offset)
				* playingNotes[i].volume * AMPLIFIER ;


			// output channels
			right += playingNotes[i].instrument->LRchannelsWeight * smp;
			left += (1 - playingNotes[i].instrument->LRchannelsWeight) * smp;

			if (++playingNotes[i].offset >= playingNotes[i].instrument->len){
				playingNotes[i] = playingNotes[playingNotesCount - 1];
				--playingNotesCount;
				--i;
			}
		}

		if (right < -3.0) right = -3.0;
		else if (right > 3.0) right = 3.0;
		dest[n * 2 + 0] = (GLshort)(right * 10922.0);

		if (left < -3.0) left = -3.0;
		else if (left > 3.0) left = 3.0;
		dest[n * 2 + 1] = (GLshort)(left * 10922.0);
	}

}

/**
 * set volume for one channel in target song
 * @param s target song
 * @param ch target channel
 * @param newVolume new volume between 0 and 1
 */
void synthSetVolume(struct S_song *s, GLint ch, GLfloat newVolume){
	if (ch < s->tracksCount)
		s->tracks[ch].vol = newVolume;
}

/**
 * set bpm in song
 * @param s target song
 * @param bpm new bpm
 */
void synthSetBpm(struct S_song *s, GLint bpm){
	s->BPM = bpm;
	nupdate = 6 * SAMPLERATE / (bpm * 10 / 25);
}

/**
 * Mute instrument
 * @param s target song
 * @param ch target channel
 * @param nmute nmute = 0 normal playing, nmute = 1 mute
 * @param delay delay = 0 mute at once, delay = 1 mute on next line
 */
void synthMute(struct S_song *s, GLint ch, GLint nmute, GLint delay){
	if (ch < s->tracksCount){
		s->tracks[ch].nextmute = nmute;
		if (!delay)
			s->tracks[ch].mute = nmute;
	}
}

/**
 * Transpose instrument.
 * @param s target song
 * @param ch target channel
 * @param transp negative values transpose down, positive up
 */
void synthTranspose(struct S_song *s, GLint ch, GLint transp){
	if (ch < s->tracksCount){
		s->tracks[ch].transp = transp;
	}
}
