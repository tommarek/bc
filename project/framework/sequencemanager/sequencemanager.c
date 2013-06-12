/**
\file
 All sequence manager operations
*/
#include "sequencemanager.h"

#include "framework/sequencemanager/sequencemanager.h"
#include "framework/sequencemanager/timer.h"
#include "framework/musicmanager/musicmanager.h"
#include "framework/scenemanager/sceneoperations.h"
#include "framework/draw.h"
#include "framework/util.h"
#include "framework/window.h"
#include "demo/sequence/sequence.h"

// list of unfinished events
#define TODOLISTSIZE 64
static int todoCount;
static int todoList[TODOLISTSIZE];
// id of event
static int nextEvent;
// when == True intro is finished
int end;


// currently active scene (root node)
struct S_sceneNode *currentRoot;

/**
 * change path to another
 * @param event event entry
 */
void changePath(struct S_sequenceEvent *event){
	struct S_sceneNode *curveNode = ((struct S_sceneNode **)event->param1)[event->param2];
	struct S_bezier *newCurve = paths[event->param3];
	GLuint startTime = event->param4;
	GLuint stopTime = event->param5;

	((struct S_curveContent *)curveNode->content)->curve = newCurve;
	((struct S_curveContent *)curveNode->content)->startTime = startTime;
	((struct S_curveContent *)curveNode->content)->stopTime = stopTime;
}

/**
 * change lookat path to another
 * @param event event entry
 */
void changeLookat(struct S_sequenceEvent *event){
	struct S_sceneNode *cameraNode = ((struct S_sceneNode **)event->param1)[event->param2];
	struct S_bezier *newCurve = lookAtPaths[event->param3];
	GLuint startTime = event->param4;
	GLuint stopTime = event->param5;

	((struct S_cameraContent *)cameraNode->content)->lookAtCurve = newCurve;
	((struct S_cameraContent *)cameraNode->content)->startTime = startTime;
	((struct S_cameraContent *)cameraNode->content)->stopTime = stopTime;
}

/**
 * change field of view of camera
 * @param event event entry
 */
void changeFOV(struct S_sequenceEvent *event){
	struct S_sceneNode *cameraNode = ((struct S_sceneNode **)event->param1)[event->param2];
	GLuint startFOV = event->param3;
	GLuint stopFOV = event->param4;
	GLuint startTime = event->param5;
	GLuint stopTime = event->param6;

	GLint realTime = getTime(&timer), len, deltaFOV;
	GLfloat tmp, pos;

	if (realTime < startTime || realTime > stopTime)
		return;
	else{
		len = stopTime - startTime;
		pos = realTime - startTime;
		tmp = pos / len;

		deltaFOV = stopFOV - startFOV;
		((struct S_cameraContent *)cameraNode->content)->fov = startFOV + deltaFOV * tmp;
	}
}

/**
 * Activate camera in scene
 * @param event event entry
 */
void activateCamera(struct S_sequenceEvent *event){
	struct S_sceneNode *root = ((struct S_sceneNode **)event->param1)[event->param2];
	struct S_sceneNode *camera = ((struct S_sceneNode **)event->param1)[event->param3];
	((struct S_rootContent *)root->content)->activeCamera = camera;
}

/**
 * change scene to another (or initialises scene)
 * @param event event entry
 */
void switchScene(struct S_sequenceEvent *event){
	currentRoot = ((struct S_sceneNode **)event->param1)[event->param2];
}

/**
 * Set object visible
 * @param event event entry
 */
void setVisible(struct S_sequenceEvent *event){
	struct S_sceneNode *node = ((struct S_sceneNode **)event->param1)[event->param2];
        node->visible = True;
}
/**
 * Set object invisible
 * @param event event entry
 */
void setInvisible(struct S_sequenceEvent *event){
	struct S_sceneNode *node = ((struct S_sceneNode **)event->param1)[event->param2];
        node->visible = False;
}
/**
 * switch to another song
 * @param event event entry
 */
void beginSong(struct S_sequenceEvent *event){
	struct S_song *song = (struct S_song *)event->param1;
	startSong(song);
}

/**
 * switch to another song
 * @param event event entry
 */
void switchSong(struct S_sequenceEvent *event){
	stopSong();
	startSong((struct S_song *)event->param1);
}

/**
 * switch to another song
 * @param event event entry
 */
void endSong(){
	stopSong();
}

/**
 * end demo
 * @param event event entry
 */
void endDemo(){
	end = True;
}

/**
 *  rotate node in time using lerp
 * @param event event entry
 */
void rotate(struct S_sequenceEvent *event){
	struct S_sceneNode *node = ((struct S_sceneNode **)event->param1)[event->param2];
	GLuint rot = event->param3; // whole angle to rotate
	GLuint startTime = event->param4;
	GLuint stopTime = event->param5;
	GLint realTime = getTime(&timer), len;
	GLfloat tmp, pos;

	if (realTime < startTime || realTime > stopTime)
		return;
	else{
		len = stopTime - startTime;
		pos = realTime - startTime;
		tmp = pos / len;
		myMemcpy(node->modelview, node->noRotModelview, 16 * sizeof(GLfloat));
		rotateNodeDeg(node, rotations[rot][0] * tmp, rotations[rot][1] * tmp, rotations[rot][2] * tmp);
	}
}

/**
 * save rotation as initial
 * @param event event entry
 */
void saveRot(struct S_sequenceEvent *event){
	struct S_sceneNode *node = ((struct S_sceneNode **)event->param1)[event->param2];
	storeNoRotMatrix(node);
}

/**
 * end demo
 * @param event event entry
 */
void setBgColor(struct S_sequenceEvent *event){
	glClearColor(event->param1, event->param2, event->param3, event->param4);
}

/**
 * Internal function for doActions(). Add action to todo list.
 * @param ID determines ID of action to be added
 */
static void addToTodoList(GLuint ID){
	todoList[todoCount] = ID;
	todoCount += 1;
}

/**
 * Internal function for doActions(). Remove action from todo list.
 */
static void removeFromTodoList(GLuint todoID){
	todoList[todoID] = todoList[todoCount - 1];
	todoCount -= 1;
}



/**
 * Scene manager utility for doing actions such as changing paths and setting time, changing song etc.
 * @return 1 when ended by time else 0
 */
static int doActions(){
	GLint t = getTime(&timer);
	for(int i = 0; i < todoCount; ++i){
		if(seq[todoList[i]].stopTime <= t){
			removeFromTodoList(i);
			i--;
			continue;
		}
		seq[todoList[i]].callback(&seq[todoList[i]]);
	}
	while(seq[nextEvent].startTime <= t){
		addToTodoList(nextEvent);
		seq[nextEvent].callback(&seq[nextEvent]);
		if(end) return True;
		nextEvent += 1;
	}
	return False;
}

/**
 * draw loop
 */
void sequenceManagerLoop(){
	while(!end){
		end = doActions();

		drawGLScene(currentRoot);

		if (isUserExit()) break;
		SDL_GL_SwapBuffers();
		updateTimer(&timer);
	}
	return;
}



/**
 * initialise sequence manager. everything is set here.
 */
void initSequenceManager(){
	initCurves();
	initRotations();
	initNoise();

	// initialise
	todoCount = 0;
	nextEvent = 0;
	end = False;
}

