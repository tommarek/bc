/**
\file
 Windows are created here
*/
#include "window.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL/SDL.h>

#include "framework/scenemanager/bezier.h"
#include "framework/sequencemanager/timer.h"
#include "framework/util.h"
#include "demo/sequence/sequence.h"

struct S_glWindow window;

/**
 * Check for exit events or if debug left and right 
 */
GLboolean isUserExit(){
	SDL_PollEvent(&(window.event));

	switch(window.event.type){
		case SDL_KEYDOWN:
			switch(window.event.key.keysym.sym){
#ifndef NDEBUG
				case SDLK_LEFT:
					paths[E_CURVES_TEST_CURVE]->controlPoints[0] += 1.1;
					return False;
					break;
				case SDLK_RIGHT:
					paths[E_CURVES_TEST_CURVE]->controlPoints[0] -= 1.1;
					return False;
					break;
				case SDLK_UP:
					paths[E_CURVES_TEST_CURVE]->controlPoints[1] -= 1.0;
					return False;
					break;
				case SDLK_DOWN:
					paths[E_CURVES_TEST_CURVE]->controlPoints[1] += 1.1;
					return False;
					break;
				case SDLK_PAGEUP:
					paths[E_CURVES_TEST_CURVE]->controlPoints[2] += 1.1;
					return False;
					break;
				case SDLK_PAGEDOWN:
					paths[E_CURVES_TEST_CURVE]->controlPoints[2] -= 1.1;
					return False;
					break;
				case SDLK_SPACE:
					print("pos = [%f, %f, %f]\n", paths[E_CURVES_TEST_CURVE]->controlPoints[0], paths[E_CURVES_TEST_CURVE]->controlPoints[1], paths[E_CURVES_TEST_CURVE]->controlPoints[2]);
					return False;
					break;
				case SDLK_RETURN:
					print("time = %d\n", getTime(&timer));
					return False;
					break;

#endif
				case SDLK_ESCAPE:
					return True;
					break;
				default:
					break;
			}
	}



	if (window.event.type == SDL_QUIT){
		return True;
	}
	return False;
}

/**
 * Cloase window
 */
GLvoid killGLWindow(){
	SDL_Quit();
}

/**
 * Rename window
 * @param title new title
 */
GLvoid setCaption(const char *title){
	SDL_WM_SetCaption(title, 0);
}

/**
 * Create new window
 * @param title window title
 * @param width window width
 * @param height window height
 */
void createWindow(const char* title, GLuint width, GLuint height){
	window.width = width;
	window.height = height;

	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_AUDIO);
	if (window.fs == True){
		window.screen = SDL_SetVideoMode(width, height, 0, SDL_OPENGL | SDL_FULLSCREEN | SDL_DOUBLEBUF);
		SDL_ShowCursor(0);
	}
	else{
		window.screen = SDL_SetVideoMode(width, height, 0, SDL_OPENGL | SDL_DOUBLEBUF);
		SDL_WM_SetCaption(title, 0);
	}
}


