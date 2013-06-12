#ifndef _SEQUENCE_H_
#define _SEQUENCE_H_

#include "framework/sequencemanager/sequencemanager.h"

enum E_CURVES{
        E_CURVES_TITLES_BEG_NAME = 0, // path from first point to name
        E_CURVES_TITLES_NAME, // path along name
        E_CURVES_TITLES_NAME_SCHOOL, // path from name to school
        E_CURVES_TITLES_SCHOOL, // path along school
        E_CURVES_TITLES_SCHOOL_PRESENTS, // path from school to presents
        E_CURVES_TITLES_PRESENTS, // path along presents
        E_CURVES_TITLES_PRESENTS_TITLE, // path from psents to title
        E_CURVES_TITLES_TITLE, // path (bump) along title
        E_CURVES_TITLES_DREAMS_START, // initial position of IN YOUR DREAMS
        E_CURVES_TITLES_DREAMS_PATH, // flight path of IN YOUR DREAMS

        E_CURVES_FACIAL_MOUTH_NEUTRAL, // path of mouth node in neutral
        E_CURVES_FACIAL_MOUTH_NEUTRAL_SMILE, // path of mouth node from neutral to smile
        E_CURVES_FACIAL_MOUTH_NEUTRAL_SAD, // path of mouth node from neutral to sad
        E_CURVES_FACIAL_MOUTH_SMILE_NEUTRAL, // path of mouth node from smile to neutral
        E_CURVES_FACIAL_MOUTH_SAD_NEUTRAL, // path of mouth node from sad to neutral

        E_CURVES_S0_0, 
        E_CURVES_S0_1,
        E_CURVES_S0_2,

        E_CURVES_S1_0,

        E_CURVES_TEST_CURVE, // testCurve

        E_CURVES_COUNT,
};

enum E_ROT{
        E_ROT_FACIAL_SMILE_25 = 0,
        E_ROT_FACIAL_SMILE_35,
        E_ROT_FACIAL_SAD_25,

        E_ROT_COUNT,
};

enum E_LOOKAT{
        E_LOOKAT_CENTER = 0,
        E_LOOKAT_TITLES,
        E_LOOKAT_HEAD,
        E_LOOKAT_HEAD_END,

        E_LOOKAT_COUNT,
};

enum E_NOISES{
        E_NOISES_HEAD = 0,
        E_NOISES_HANDS,
        E_NOISES_EYEBROWS,

        E_NOISES_COUNT,
};


extern struct S_bezier *paths[E_CURVES_COUNT],*lookAtPaths[E_LOOKAT_COUNT];
extern GLfloat rotations[E_ROT_COUNT][3];
extern struct S_sequenceEvent seq[];

void initCurves();
void initRotations();
void initNoise();


#endif