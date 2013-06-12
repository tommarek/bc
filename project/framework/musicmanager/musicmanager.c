/**
\file
 All music manager operations
*/
#include "musicmanager.h"

#include <math.h>

#include "framework/musicmanager/synth.h"
#include "demo/music/song.h"

/**
 *
 */
static void playerCallback(void *udata, Uint8 *stream, int len){
	// len / 4 because uint8 is 8 bit and short is 16 bit and there are  two channels
	synthPlay(udata, (GLshort *)stream, len / 4);
}

/**
 * start target song
 * @param s target song
 */
void startSong(struct S_song *s){
	SDL_AudioSpec info;
	info.freq = SAMPLERATE;
	info.format = AUDIO_S16SYS;
	info.channels = 2;
	info.samples = 4096;
	info.callback = playerCallback;
	info.userdata = s;
	SDL_OpenAudio(&info,NULL);

	synthInit(s);
	synthLoad(s);
	SDL_PauseAudio(0);
}

/**
 * stop playing song
 */
void stopSong(){
	SDL_CloseAudio();
}

/**
 * initialise music manager
 */
void initMusicManager(){
    initSongs();
}