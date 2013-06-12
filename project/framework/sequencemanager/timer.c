/**
\file
 Time operations
*/
#include "timer.h"

#include <stdio.h>

#include "framework/window.h"

struct S_timer timer;

/**
 * initialise timer
 * @param timer pointer to timer struct
 */
void initTimer(struct S_timer *t){
	t->start = SDL_GetTicks();

	t->frames = 0;
	t->oldTime = 0;
}

/**
 * count frames per second
 * @param timer pointer to timer struct
 */
void getFPS(struct S_timer *t){
	t->frames++;

	if (t->time - t->oldTime >= 1000){
		t->fps = t->frames / ((t->time - t->oldTime) / 1000);

		// display fps in title
		char title[64];
		sprintf(title, "Bug's life - FPS : %d", t->fps);
		setCaption(title);

		t->oldTime = t->time;
		t->frames = 0;
	}
}

/**
 * update timer
 * @param timer pointer to timer struct
 */
void updateTimer(struct S_timer *t){
	SDL_Delay(1);
	t->time = SDL_GetTicks() - t->start;

	getFPS(t);
}

/**
 * get actual time
 * @param timer pointer to timer struct
 */
GLint getTime(struct S_timer *t){
	return t->time;
}