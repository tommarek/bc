#ifndef WINDOW_H
#define WINDOW_H

#include <GL/gl.h>
#include <GL/glu.h>

#include <SDL/SDL.h>

struct S_glWindow{
	SDL_Surface *screen;
	SDL_Event event;

        GLboolean fs;
        GLuint width, height;
        GLuint depth;
};

extern struct S_glWindow window;

void createWindow(const char* title, GLuint width, GLuint height);
GLvoid setCaption(const char *title);
GLvoid killGLWindow(GLvoid);
GLboolean isUserExit();


#endif // WINDOW_H
