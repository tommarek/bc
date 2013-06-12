#ifndef _SEQUENCEMANAGER_H_
#define _SEQUENCEMANAGER_H_

#include <GL/gl.h>

#include "framework/musicmanager/musicmanager.h"
#include "demo/scene/scene.h"

struct S_sequenceEvent;

typedef void (*eventCallback)(struct S_sequenceEvent *);

struct S_sequenceEvent{
        eventCallback callback;

        GLuint startTime; // in 1/1000 s (ms)
        GLuint stopTime;

        // params
        uintptr_t param1;
        uintptr_t param2;
        uintptr_t param3;
        uintptr_t param4;
        uintptr_t param5;
        uintptr_t param6;
};


void changePath(struct S_sequenceEvent *event);
void changeLookat(struct S_sequenceEvent *event);
void activateCamera(struct S_sequenceEvent *event);
void switchScene(struct S_sequenceEvent *event);
void setInvisible(struct S_sequenceEvent *event);
void setVisible(struct S_sequenceEvent *event);
void beginSong(struct S_sequenceEvent *event);
void switchSong(struct S_sequenceEvent *event);
void endSong();
void endDemo();
void rotate(struct S_sequenceEvent *event);
void saveRot(struct S_sequenceEvent *event);
void setBgColor(struct S_sequenceEvent *event);
void changeFOV(struct S_sequenceEvent *event);

void sequenceManagerLoop();
void initSequenceManager();
#endif