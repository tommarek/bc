#ifndef TIMER_H
#define TIMER_H

#include <GL/gl.h>
#include <GL/glu.h>

struct S_timer{
        GLint start;
        GLint time;

        GLint fps;
	GLint frames;
        GLint oldTime;
};

extern struct S_timer timer;

void initTimer(struct S_timer *t);
void updateTimer(struct S_timer *t);
void getFPS(struct S_timer *t);
GLint getTime(struct S_timer *t);

#endif