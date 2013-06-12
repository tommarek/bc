/**
\file
 Framework main file
*/
#include <stdio.h>

#include "GL/gl.h"
#include "GL/glu.h"
#include "SDL/SDL.h"

#include "framework/meshmanager/meshmanager.h"
#include "framework/texturemanager/texturemanager.h"
#include "framework/materialmanager/materialmanager.h"
#include "framework/scenemanager/scenemanager.h"
#include "framework/sequencemanager/timer.h"
#include "framework/sequencemanager/sequencemanager.h"
#include "framework/extensions.h"
#include "framework/window.h"
#include "framework/util.h"
#include "framework/draw.h"

static void init(){
	initExtensions();
	initMeshManager();
	initTextureManager();
	initMaterialManager();
	initSceneManger();
	initMusicManager();
	initGL();
	initSequenceManager();
	initTimer(&timer);
}

int main(void){
	window.fs = False;
	createWindow("All dreams are reachable", 640, 640);

	init();
	
	sequenceManagerLoop();
	
	killGLWindow(); // after drawing, delete window
	stopSong();
	_exit(0); // exit program
}
