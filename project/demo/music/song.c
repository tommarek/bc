#include "song.h"

struct S_song elise;
struct S_song scale;

/**
 * initialise all songs
 */
void initSongs(){
	initElise(&elise);
	initScale(&scale);
}