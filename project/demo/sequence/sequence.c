#include "sequence.h"

#include "framework/sequencemanager/sequencemanager.h"
#include "framework/scenemanager/scenemanager.h"
#include "framework/musicmanager/musicmanager.h"
#include "framework/musicmanager/synth.h"
#include "framework/scenemanager/sceneoperations.h"
#include "framework/sequencemanager/timer.h"
#include "framework/util.h"
#include "demo/music/song.h"
#include "demo/textures/texture.h"


#define TITLESLEN 27000
#define S0LEN 36000 + TITLESLEN
#define S1LEN 29000 + S0LEN

/*
#define TITLESLEN 0
#define S0LEN 0
#define S1LEN 0
*/

#define NOISESIZE 512


// array of all curves
struct S_bezier *paths[E_CURVES_COUNT],*lookAtPaths[E_LOOKAT_COUNT];
// scene rotations
GLfloat rotations[E_ROT_COUNT][3];
// noises 
struct S_texture *noises[E_NOISES_COUNT];


/**
 * follow hand on piano according to song.
 * outside sequencemanager.c because it is specific to one demo
 * @param event event entry
 */
static void followHand(struct S_sequenceEvent *event){
	struct S_sceneNode *node = ((struct S_sceneNode **)event->param1)[event->param2];
	struct S_song *song = (struct S_song *)event->param3;
	GLuint trackID = event->param4;
	GLuint middleNote = event->param5;
	GLfloat oneNoteAngleStep = 1; // deg
	GLint playedNote = song->tracks[trackID].LastPlayedNote;
	// + noise
	GLfloat moveX = (0.5 - noises[E_NOISES_HANDS]->tex[(node->noisePos + 1 * 512) * 4]) * 0.3;
	GLfloat moveY = (0.5 - noises[E_NOISES_HANDS]->tex[(node->noisePos + 2 * 512) * 4]) * 0.3;
	GLfloat moveZ = (0.5 - noises[E_NOISES_HANDS]->tex[(node->noisePos + 3 * 512) * 4]) * 0.3;
	
	if (playedNote == -1){
		rotateNodeDeg(node, moveX, moveY, moveZ);
                ++node->noisePos;
		node->noisePos %= NOISESIZE;
		return;
	}
	// distance from middle note to played note
	GLint delta = middleNote - playedNote;

	myMemcpy(node->modelview, node->noRotModelview, 16 * sizeof(GLfloat));
	rotateNodeDeg(node, 0, 0, delta * oneNoteAngleStep);

}

/**
 * add noise element to target body part
 * outside sequencemanager.c because it is specific to one demos
 * @param event event entry
 */
static void noiseMovement(struct S_sequenceEvent *event){
	struct S_sceneNode *node = ((struct S_sceneNode **)event->param1)[event->param2];
	GLfloat oneStep = event->param3; // deg
	struct S_texture *moveNoise = noises[event->param4];
	GLfloat moveX = (0.5 - moveNoise->tex[(node->noisePos + 1 * 512) * 4]) * oneStep;
	GLfloat moveY = (0.5 - moveNoise->tex[(node->noisePos + 2 * 512) * 4]) * oneStep;
	GLfloat moveZ = (0.5 - moveNoise->tex[(node->noisePos + 3 * 512) * 4]) * oneStep;

	myMemcpy(node->modelview, node->noRotModelview, 16 * sizeof(GLfloat));
	if(event->param5 == -1){
		rotateNodeDeg(node, 1 - moveX, 1 - moveY, 1 -moveZ);
	}
	else{
		rotateNodeDeg(node, moveX, moveY, moveZ);
	}

        ++node->noisePos;
        node->noisePos %= NOISESIZE;
}

// events
struct S_sequenceEvent seq[] = {
	{setBgColor         ,     0,     0,                    0,                 0,                               0,     0,     0, 0}, // set bg to black
	{switchScene        ,     0,     0, (uintptr_t)beginning,          E_B_ROOT,                               0,     0,     0, 0}, // switch scene to beginning
	{changeLookat       ,     0,     0, (uintptr_t)beginning,         E_B_CAM_1,                 E_LOOKAT_TITLES,     0,     0, 0}, // look at titles
	{activateCamera     ,     0,     0, (uintptr_t)beginning,          E_B_ROOT,                       E_B_CAM_1,     0,     0, 0}, // activate camera
//	{beginSong          ,     0,     0,    (uintptr_t)&scale,                 0,                               0,     0,     0, 0}, // start song
//	{changePath         ,     0,     0, (uintptr_t)beginning,   E_B_CAM_1_CURVE,             E_CURVES_TEST_CURVE,     0,     0, 0}, // change path to test curve

	{changePath         ,     0,     0, (uintptr_t)beginning, E_B_TITLE_2_CURVE,    E_CURVES_TITLES_DREAMS_START,     0,     0, 0}, // chnge position of in your dreams to start
 	{changePath         ,     0,     0, (uintptr_t)beginning,   E_B_CAM_1_CURVE,        E_CURVES_TITLES_BEG_NAME,     0,  2000, 0}, // change path from beg to name
	{changePath         ,  2000,  2000, (uintptr_t)beginning,   E_B_CAM_1_CURVE,            E_CURVES_TITLES_NAME,  2000,  6000, 0}, // change path along name
	{changePath         ,  6000,  6000, (uintptr_t)beginning,   E_B_CAM_1_CURVE,     E_CURVES_TITLES_NAME_SCHOOL,  6000,  8000, 0}, // change path from name to school
	{changePath         ,  8000,  8000, (uintptr_t)beginning,   E_B_CAM_1_CURVE,          E_CURVES_TITLES_SCHOOL,  8000, 13000, 0}, // change path along school
	{changePath         , 13000, 13000, (uintptr_t)beginning,   E_B_CAM_1_CURVE, E_CURVES_TITLES_SCHOOL_PRESENTS, 13000, 15000, 0}, // change path from school to presents
	{changePath         , 15000, 15000, (uintptr_t)beginning,   E_B_CAM_1_CURVE,        E_CURVES_TITLES_PRESENTS, 15000, 16500, 0}, // change path along presents
	{changePath         , 16500, 16500, (uintptr_t)beginning,   E_B_CAM_1_CURVE,  E_CURVES_TITLES_PRESENTS_TITLE, 16500, 20000, 0}, // change path from presents to title
	{changePath         , 20000, 20000, (uintptr_t)beginning,   E_B_CAM_1_CURVE,           E_CURVES_TITLES_TITLE, 20000, 22000, 0}, // change path along title
	{changePath         , 22000, 22000, (uintptr_t)beginning, E_B_TITLE_2_CURVE,     E_CURVES_TITLES_DREAMS_PATH, 22000, 23300, 0}, // change path from beg to name
//`	{endSong            , 27000, 27000,                    0,                 0,                               0,     0,     0, 0}, // stop song



	//scene 0
        {setInvisible   ,      TITLESLEN + 0,      TITLESLEN + 0, (uintptr_t)scene0,               E_S0_THEEND,                                     0,                          0,                     0, 0}, // se the end as invisible
	{setBgColor     ,      TITLESLEN + 0,      TITLESLEN + 0,                 1,                         1,                                     1,                          1,                     0, 0}, // set bg to white
	{switchScene    ,      TITLESLEN + 0,      TITLESLEN + 0, (uintptr_t)scene0,                 E_S0_ROOT,                                     0,              TITLESLEN + 0,         TITLESLEN + 0, 0}, // switch scene to s1
	{beginSong      ,      TITLESLEN + 0,      TITLESLEN + 0, (uintptr_t)&elise,                         0,                                     0,              TITLESLEN + 0,         TITLESLEN + 0, 0}, // start fur elise
	{activateCamera ,      TITLESLEN + 0,      TITLESLEN + 0, (uintptr_t)scene0,                 E_S0_ROOT,                            E_S0_CAM_1,              TITLESLEN + 0,         TITLESLEN + 0, 0}, // activate camera
	{changeLookat   ,      TITLESLEN + 0,      TITLESLEN + 0, (uintptr_t)scene0,                E_S0_CAM_1,                         E_LOOKAT_HEAD,              TITLESLEN + 0,         TITLESLEN + 0, 0}, // look at titles

	{changePath     ,      TITLESLEN + 0,      TITLESLEN + 0, (uintptr_t)scene0,          E_S0_CAM_1_CURVE,                   E_CURVES_TEST_CURVE,              TITLESLEN + 0,         TITLESLEN + 0, 0}, // change path to test curve

	{changePath     ,      TITLESLEN + 0,      TITLESLEN + 0, (uintptr_t)scene0,          E_S0_CAM_1_CURVE,                         E_CURVES_S0_0,              TITLESLEN + 0,     TITLESLEN + 24000, 0}, // change path from beg to behind puppets bac
	{changePath     ,      TITLESLEN + 0,      TITLESLEN + 0, (uintptr_t)scene0,    E_S0_PUPPET_MOUTH_PATH,         E_CURVES_FACIAL_MOUTH_NEUTRAL,              TITLESLEN + 0,         TITLESLEN + 0, 0}, // change path from beg to behind puppets back

	{noiseMovement  ,      TITLESLEN + 0, TITLESLEN + 80000, (uintptr_t)scene0,          E_S0_PUPPET_HEAD,                                    10,              E_NOISES_HEAD,                     0, 0},
	{noiseMovement  ,      TITLESLEN + 0, TITLESLEN + 80000, (uintptr_t)scene0,  E_S0_PUPPET_LEFT_EYEBROW,                                    70,          E_NOISES_EYEBROWS,                     0, 0},
	{noiseMovement  ,      TITLESLEN + 0, TITLESLEN + 80000, (uintptr_t)scene0, E_S0_PUPPET_RIGHT_EYEBROW,                                    70,          E_NOISES_EYEBROWS,                    -1, 0},
	{noiseMovement  ,      TITLESLEN + 0, TITLESLEN + 80000, (uintptr_t)scene0,      E_S0_PUPPET_LEFT_EYE,                                    50,              E_NOISES_HEAD,                     0, 0},
	{noiseMovement  ,      TITLESLEN + 0, TITLESLEN + 80000, (uintptr_t)scene0,     E_S0_PUPPET_RIGHT_EYE,                                    50,              E_NOISES_HEAD,                     0, 0},
	{noiseMovement  ,      TITLESLEN + 0, TITLESLEN + 80000, (uintptr_t)scene0,    E_S0_PUPPET_UPPER_BODY,                                    15,              E_NOISES_HEAD,                     0, 0},
	{followHand     ,      TITLESLEN + 0, TITLESLEN + 80000, (uintptr_t)scene0,      E_S0_PUPPET_LEFT_ARM,                     (uintptr_t)&elise,  E_TRACK_ELISE_MELODY_LEFT,                E_N_b2, 0}, // left hand following song
	{followHand     ,      TITLESLEN + 0, TITLESLEN + 80000, (uintptr_t)scene0,     E_S0_PUPPET_RIGHT_ARM,                     (uintptr_t)&elise, E_TRACK_ELISE_MELODY_RIGHT,                E_N_e4, 0}, // left hand following song

	{changePath     ,  TITLESLEN + 20000,  TITLESLEN + 20000, (uintptr_t)scene0,    E_S0_PUPPET_MOUTH_PATH,     E_CURVES_FACIAL_MOUTH_NEUTRAL_SAD,          TITLESLEN + 20000,     TITLESLEN + 23000, 0}, // smile path
  	{rotate         ,  TITLESLEN + 20000,  TITLESLEN + 23000, (uintptr_t)scene0,         E_S0_PUPPET_MOUTH,                   E_ROT_FACIAL_SAD_25,          TITLESLEN + 20000,     TITLESLEN + 23000, 0}, // smile rot
	{saveRot        ,  TITLESLEN + 23000,  TITLESLEN + 23000, (uintptr_t)scene0,         E_S0_PUPPET_MOUTH,                                     0,                          0,                     0, 0}, // save mouth rotation
	
	{changePath     ,  TITLESLEN + 24000,  TITLESLEN + 35000, (uintptr_t)scene0,          E_S0_CAM_1_CURVE,                         E_CURVES_S0_1,          TITLESLEN + 24000,     TITLESLEN + 35000, 0}, // change path from beg to behind puppets back
	{changeFOV      ,  TITLESLEN + 35000,  TITLESLEN + 36000, (uintptr_t)scene0,                E_S0_CAM_1,                                    50,                          0,     TITLESLEN + 35000,     TITLESLEN + 36000},
	
	// scene 1
	{setBgColor     ,     S0LEN + 0,      S0LEN + 0,                 0,                         0,                                     0,                          0,                     0, 0}, // set bg to white
	{switchScene    ,     S0LEN + 0,      S0LEN + 0, (uintptr_t)scene1,                 E_S1_ROOT,                                     0,                  S0LEN + 0,             S0LEN + 0, 0}, // switch scene to s1
	{activateCamera ,     S0LEN + 0,      S0LEN + 0, (uintptr_t)scene1,                 E_S1_ROOT,                            E_S1_CAM_1,                  S0LEN + 0,             S0LEN + 0, 0}, // activate camera
	{changeLookat   ,     S0LEN + 0,      S0LEN + 0, (uintptr_t)scene1,                E_S1_CAM_1,                         E_LOOKAT_HEAD,                  S0LEN + 0,             S0LEN + 0, 0}, // look at titles

	{changePath     ,     S0LEN + 0,      S0LEN + 0, (uintptr_t)scene1,    E_S1_PUPPET_MOUTH_PATH,         E_CURVES_FACIAL_MOUTH_NEUTRAL,                  S0LEN + 0,             S0LEN + 0, 0}, // change path from beg to behind puppets back
	//{changePath     ,     S0LEN + 0,      S0LEN + 0, (uintptr_t)scene1,          E_S1_CAM_1_CURVE,                   E_CURVES_TEST_CURVE,                  S0LEN + 0,             S0LEN + 0, 0}, // change path to test curve
	{changePath     ,     S0LEN + 0,      S0LEN + 0, (uintptr_t)scene1,          E_S1_CAM_1_CURVE,                         E_CURVES_S1_0,                  S0LEN + 0,         S0LEN + 28000, 0}, // change path to test curve

	{noiseMovement  ,     S0LEN + 0,  S0LEN + 29000, (uintptr_t)scene1,          E_S1_PUPPET_HEAD,                                    10,              E_NOISES_HEAD,                     0, 0},
	{noiseMovement  ,     S0LEN + 0,  S0LEN + 29000, (uintptr_t)scene1,  E_S1_PUPPET_LEFT_EYEBROW,                                    70,          E_NOISES_EYEBROWS,                     0, 0},
	{noiseMovement  ,     S0LEN + 0,  S0LEN + 29000, (uintptr_t)scene1, E_S1_PUPPET_RIGHT_EYEBROW,                                    70,          E_NOISES_EYEBROWS,                    -1, 0},
	{noiseMovement  ,     S0LEN + 0,  S0LEN + 29000, (uintptr_t)scene1,      E_S1_PUPPET_LEFT_EYE,                                    80,              E_NOISES_HEAD,                     0, 0},
	{noiseMovement  ,     S0LEN + 0,  S0LEN + 29000, (uintptr_t)scene1,     E_S1_PUPPET_RIGHT_EYE,                                    80,              E_NOISES_HEAD,                     0, 0},
	{noiseMovement  ,     S0LEN + 0,  S0LEN + 29000, (uintptr_t)scene1,    E_S1_PUPPET_UPPER_BODY,                                    15,              E_NOISES_HEAD,                     0, 0},
	{followHand     ,     S0LEN + 0,  S0LEN + 29000, (uintptr_t)scene1,      E_S1_PUPPET_LEFT_ARM,                     (uintptr_t)&elise,  E_TRACK_ELISE_MELODY_LEFT,                E_N_b2, 0}, // left hand following song
	{followHand     ,     S0LEN + 0,  S0LEN + 29000, (uintptr_t)scene1,     E_S1_PUPPET_RIGHT_ARM,                     (uintptr_t)&elise, E_TRACK_ELISE_MELODY_RIGHT,                E_N_e4, 0}, // left hand following song

	{changePath     ,     S0LEN + 0,      S0LEN + 0, (uintptr_t)scene1,    E_S1_PUPPET_MOUTH_PATH,     E_CURVES_FACIAL_MOUTH_SAD_NEUTRAL,                  S0LEN + 0,          S0LEN + 1000, 0}, // change path from beg to behind puppets back
  	{rotate         ,     S0LEN + 0,   S0LEN + 1000, (uintptr_t)scene1,         E_S1_PUPPET_MOUTH,                 E_ROT_FACIAL_SMILE_35,                  S0LEN + 0,          S0LEN + 1000, 0}, // smile rot
	{saveRot        ,  S0LEN + 1000,   S0LEN + 1000, (uintptr_t)scene1,         E_S1_PUPPET_MOUTH,                                     0,                          0,                     0, 0}, // save mouth rotation

	
	// back to scene 0

/*
        // debug
        {setBgColor     , S1LEN + 0, S1LEN + 0,                 1,                         1,                                     1,                          1,                     0, 0}, // set bg to white
	{switchScene    , S1LEN + 0, S1LEN + 0, (uintptr_t)scene0,                 E_S0_ROOT,                                     0,              S1LEN + 0,         S1LEN + 0, 0}, // switch scene to s1
	{activateCamera , S1LEN + 0, S1LEN + 0, (uintptr_t)scene0,                 E_S0_ROOT,                            E_S0_CAM_1,              S1LEN + 0,         S1LEN + 0, 0}, // activate camera
	{changeLookat   , S1LEN + 0, S1LEN + 0, (uintptr_t)scene0,                E_S0_CAM_1,                         E_LOOKAT_HEAD,              S1LEN + 0,         S1LEN + 0, 0}, // look at titles
        {noiseMovement  , S1LEN + 0, S1LEN + 80000, (uintptr_t)scene0,          E_S0_PUPPET_HEAD,                                    10,              E_NOISES_HEAD,                     0, 0},
	{noiseMovement  , S1LEN + 0, S1LEN + 80000, (uintptr_t)scene0,  E_S0_PUPPET_LEFT_EYEBROW,                                    70,          E_NOISES_EYEBROWS,                     0, 0},
	{noiseMovement  , S1LEN + 0, S1LEN + 80000, (uintptr_t)scene0, E_S0_PUPPET_RIGHT_EYEBROW,                                    70,          E_NOISES_EYEBROWS,                    -1, 0},
	{noiseMovement  , S1LEN + 0, S1LEN + 80000, (uintptr_t)scene0,      E_S0_PUPPET_LEFT_EYE,                                    50,              E_NOISES_HEAD,                     0, 0},
	{noiseMovement  , S1LEN + 0, S1LEN + 80000, (uintptr_t)scene0,     E_S0_PUPPET_RIGHT_EYE,                                    50,              E_NOISES_HEAD,                     0, 0},
	{noiseMovement  , S1LEN + 0, S1LEN + 80000, (uintptr_t)scene0,    E_S0_PUPPET_UPPER_BODY,                                    15,              E_NOISES_HEAD,                     0, 0},
	{followHand     , S1LEN + 0, S1LEN + 80000, (uintptr_t)scene0,      E_S0_PUPPET_LEFT_ARM,                     (uintptr_t)&elise,  E_TRACK_ELISE_MELODY_LEFT,                E_N_b2, 0}, // left hand following song
	{followHand     , S1LEN + 0, S1LEN + 80000, (uintptr_t)scene0,     E_S0_PUPPET_RIGHT_ARM,                     (uintptr_t)&elise, E_TRACK_ELISE_MELODY_RIGHT,                E_N_e4, 0}, // left hand following song
       	{changePath     , S1LEN + 0, S1LEN + 0, (uintptr_t)scene0,    E_S0_PUPPET_MOUTH_PATH,         E_CURVES_FACIAL_MOUTH_NEUTRAL,              S1LEN + 0,         S1LEN + 0, 0}, // change path from beg to behind puppets back
	{changePath     , S1LEN + 0, S1LEN + 0, (uintptr_t)scene0,          E_S0_CAM_1_CURVE,                   E_CURVES_TEST_CURVE,              S1LEN + 0,         S1LEN + 0, 0}, // change path to test curve
*/

	{setBgColor     ,      S1LEN + 0,       S1LEN + 0,                 1,                         1,                                     1,                          0,                     0, 0}, // set bg to white
        {switchScene    ,      S1LEN + 0,       S1LEN + 0, (uintptr_t)scene0,                 E_S0_ROOT,                                     0,                  S1LEN + 0,             S1LEN + 0, 0}, // switch scene to s1
	{changeFOV      ,      S1LEN + 0,    S1LEN + 1000, (uintptr_t)scene0,                E_S0_CAM_1,                                     0,                         30,             S1LEN + 0,     S1LEN + 1000},
        {changePath     ,   S1LEN + 1000,    S1LEN + 1000, (uintptr_t)scene0,          E_S0_CAM_1_CURVE,                         E_CURVES_S0_2,               S1LEN + 1000,         S1LEN + 15000, 0}, // change path to test curve

	{changePath     ,   S1LEN + 1000,    S1LEN + 1000, (uintptr_t)scene0,    E_S0_PUPPET_MOUTH_PATH,     E_CURVES_FACIAL_MOUTH_SAD_NEUTRAL,               S1LEN + 1000,          S1LEN + 3000, 0}, // change path from beg to behind puppets back
  	{rotate         ,   S1LEN + 1000,    S1LEN + 3000, (uintptr_t)scene0,         E_S0_PUPPET_MOUTH,                 E_ROT_FACIAL_SMILE_35,               S1LEN + 1000,          S1LEN + 3000, 0}, // smile rot
	{saveRot        ,   S1LEN + 3000,    S1LEN + 3000, (uintptr_t)scene0,         E_S0_PUPPET_MOUTH,                                     0,                          0,                     0, 0}, // save mouth rotation

	{changePath     ,   S1LEN + 3000,    S1LEN + 3000, (uintptr_t)scene0,    E_S0_PUPPET_MOUTH_PATH,   E_CURVES_FACIAL_MOUTH_NEUTRAL_SMILE,               S1LEN + 3000,          S1LEN + 5000, 0}, // change path from beg to behind puppets back
  	{rotate         ,   S1LEN + 3000,    S1LEN + 5000, (uintptr_t)scene0,         E_S0_PUPPET_MOUTH,                 E_ROT_FACIAL_SMILE_35,               S1LEN + 3000,          S1LEN + 5000, 0}, // smile rot
	{saveRot        ,   S1LEN + 5000,    S1LEN + 5000, (uintptr_t)scene0,         E_S0_PUPPET_MOUTH,                                     0,                          0,                     0, 0}, // save mouth rotation
        {changeLookat   ,   S1LEN + 5000,    S1LEN + 5000, (uintptr_t)scene0,                E_S0_CAM_1,                     E_LOOKAT_HEAD_END,               S1LEN + 5000,         S1LEN + 15000, 0},

        {setVisible     ,  S1LEN + 12000,   S1LEN + 12000, (uintptr_t)scene0,               E_S0_THEEND,                                     0,                          0,                     0, 0}, // se the end as invisible

	{endSong        ,  S1LEN + 15000,   S1LEN + 15000,                 0,                         0,                                     0,                          0,                     0, 0}, // stop fur elise
        {endDemo        ,  S1LEN + 18000,   S1LEN + 18000,                 0,                         0,                                     0,                          0,                     0, 0}, // end demo

};


/**
 * initialise all needed curves
 */
void initCurves(){
	// path curves
	// path from beg to name
	paths[E_CURVES_TITLES_BEG_NAME] = createBezier(3);
	bezierAddControlPoint(paths[E_CURVES_TITLES_BEG_NAME], 0, -800, 800);
	bezierAddControlPoint(paths[E_CURVES_TITLES_BEG_NAME], -6, -605, 605);
	// path along name
	paths[E_CURVES_TITLES_NAME] = createBezier(3);
	bezierAddControlPoint(paths[E_CURVES_TITLES_NAME], -6, -605, 605);
	bezierAddControlPoint(paths[E_CURVES_TITLES_NAME], 10, -605, 605);
	// path from name to school
	paths[E_CURVES_TITLES_NAME_SCHOOL] = createBezier(3);
	bezierAddControlPoint(paths[E_CURVES_TITLES_NAME_SCHOOL], 10, -605, 605);
	bezierAddControlPoint(paths[E_CURVES_TITLES_NAME_SCHOOL], -16, -505, 505);
	// path along school
	paths[E_CURVES_TITLES_SCHOOL] = createBezier(3);
	bezierAddControlPoint(paths[E_CURVES_TITLES_SCHOOL], -16, -505, 505);
	bezierAddControlPoint(paths[E_CURVES_TITLES_SCHOOL], 18, -505, 505);
	// path from school to presents
	paths[E_CURVES_TITLES_SCHOOL_PRESENTS] = createBezier(3);
	bezierAddControlPoint(paths[E_CURVES_TITLES_SCHOOL_PRESENTS], 18, -505, 505);
	bezierAddControlPoint(paths[E_CURVES_TITLES_SCHOOL_PRESENTS], -2, -405, 405);
	// path along presents
	paths[E_CURVES_TITLES_PRESENTS] = createBezier(3);
	bezierAddControlPoint(paths[E_CURVES_TITLES_PRESENTS], -2, -405, 405);
	bezierAddControlPoint(paths[E_CURVES_TITLES_PRESENTS], 4, -405, 405);
	// path from presents to title
	paths[E_CURVES_TITLES_PRESENTS_TITLE] = createBezier(6);
	bezierAddControlPoint(paths[E_CURVES_TITLES_PRESENTS_TITLE], 4, -405, 405);
	bezierAddControlPoint(paths[E_CURVES_TITLES_PRESENTS_TITLE], 200, -205, 205);
	bezierAddControlPoint(paths[E_CURVES_TITLES_PRESENTS_TITLE], 400, -100, 100);
	bezierAddControlPoint(paths[E_CURVES_TITLES_PRESENTS_TITLE], 2, 150, 150);
	bezierAddControlPoint(paths[E_CURVES_TITLES_PRESENTS_TITLE], 6, 17, 9);
	// initial position of IN YOUR DREAMS
	paths[E_CURVES_TITLES_DREAMS_START] = createBezier(2);
	bezierAddControlPoint(paths[E_CURVES_TITLES_DREAMS_START], -10000, -10000, -10000);
	// initial path of IN YOUR DREAMS
	paths[E_CURVES_TITLES_DREAMS_PATH] = createBezier(3);
	bezierAddControlPoint(paths[E_CURVES_TITLES_DREAMS_PATH], 100, 100, 9);
	bezierAddControlPoint(paths[E_CURVES_TITLES_DREAMS_PATH], -5, 10, 9);

	paths[E_CURVES_TITLES_TITLE] = createBezier(14);
	bezierAddControlPoint(paths[E_CURVES_TITLES_TITLE], 6, 17, 9);
	bezierAddControlPoint(paths[E_CURVES_TITLES_TITLE], 12, 34, 9);
	bezierAddControlPoint(paths[E_CURVES_TITLES_TITLE], -3, -8, 9);
	bezierAddControlPoint(paths[E_CURVES_TITLES_TITLE], 12, 34, 9);
	bezierAddControlPoint(paths[E_CURVES_TITLES_TITLE], -3, -8, 9);
	bezierAddControlPoint(paths[E_CURVES_TITLES_TITLE], 12, 34, 9);
	bezierAddControlPoint(paths[E_CURVES_TITLES_TITLE], -3, -8, 9);
	bezierAddControlPoint(paths[E_CURVES_TITLES_TITLE], 12, 34, 9);
	bezierAddControlPoint(paths[E_CURVES_TITLES_TITLE], 6, 17, 9);

	// movements for facial expressions
	// neutral
	paths[E_CURVES_FACIAL_MOUTH_NEUTRAL] = createBezier(3);
	bezierAddControlPoint(paths[E_CURVES_FACIAL_MOUTH_NEUTRAL], -0.01, -0.32, 0.4);
	bezierAddControlPoint(paths[E_CURVES_FACIAL_MOUTH_NEUTRAL], -0.01, -0.32, 0.4);
	// neutral -> smile
	paths[E_CURVES_FACIAL_MOUTH_NEUTRAL_SMILE] = createBezier(3);
	bezierAddControlPoint(paths[E_CURVES_FACIAL_MOUTH_NEUTRAL_SMILE], -0.01, -0.32, 0.4);
	bezierAddControlPoint(paths[E_CURVES_FACIAL_MOUTH_NEUTRAL_SMILE], -0.01, -0.35, 0.6);
	// neutral -> sad
	paths[E_CURVES_FACIAL_MOUTH_NEUTRAL_SAD] = createBezier(3);
	bezierAddControlPoint(paths[E_CURVES_FACIAL_MOUTH_NEUTRAL_SAD], -0.01, -0.32, 0.4);
	bezierAddControlPoint(paths[E_CURVES_FACIAL_MOUTH_NEUTRAL_SAD], -0.01, -0.35, 0.25);
	// smile -> neutral
	paths[E_CURVES_FACIAL_MOUTH_SMILE_NEUTRAL] = createBezier(3);
	bezierAddControlPoint(paths[E_CURVES_FACIAL_MOUTH_SMILE_NEUTRAL], -0.01, -0.32, 0.6);
	bezierAddControlPoint(paths[E_CURVES_FACIAL_MOUTH_SMILE_NEUTRAL], -0.01, -0.32, 0.4);
	// sad -> neutral
	paths[E_CURVES_FACIAL_MOUTH_SAD_NEUTRAL] = createBezier(3);
	bezierAddControlPoint(paths[E_CURVES_FACIAL_MOUTH_SAD_NEUTRAL], -0.01, -0.35, 0.25);
	bezierAddControlPoint(paths[E_CURVES_FACIAL_MOUTH_SAD_NEUTRAL], -0.01, -0.32, 0.4);

	// main scene curves
	paths[E_CURVES_S0_0] = createBezier(15);
	bezierAddControlPoint(paths[E_CURVES_S0_0], 1.2, 500, 8.5);
	bezierAddControlPoint(paths[E_CURVES_S0_0], 5.2, 30, 8.5);
	bezierAddControlPoint(paths[E_CURVES_S0_0], 8.2, 20, 8.5);
	bezierAddControlPoint(paths[E_CURVES_S0_0], 4.2, 10, 8.5);
	bezierAddControlPoint(paths[E_CURVES_S0_0], 4.2, -10, 8.5);
	bezierAddControlPoint(paths[E_CURVES_S0_0], -8, -25, 23);
	bezierAddControlPoint(paths[E_CURVES_S0_0], -8, -35, 23);
	bezierAddControlPoint(paths[E_CURVES_S0_0], -37, 18, 22);
	bezierAddControlPoint(paths[E_CURVES_S0_0], -1, 2.5, 8.5);
	bezierAddControlPoint(paths[E_CURVES_S0_0], -1, 1.5, 8.5);
	bezierAddControlPoint(paths[E_CURVES_S0_0], -1, 0.5, 8.5);
	bezierAddControlPoint(paths[E_CURVES_S0_0], -1, 2.5, 8.5);
	bezierAddControlPoint(paths[E_CURVES_S0_0], -8.0, 5.4, 8.5);
	bezierAddControlPoint(paths[E_CURVES_S0_0], 4.0, 5.4, 8.5);

	paths[E_CURVES_S0_1] = createBezier(6);
	bezierAddControlPoint(paths[E_CURVES_S0_1], 4.0, 5.4, 8.5);
	bezierAddControlPoint(paths[E_CURVES_S0_1], 8.0, 5.4, 4.5);
	bezierAddControlPoint(paths[E_CURVES_S0_1], -20, 40, 30);
	bezierAddControlPoint(paths[E_CURVES_S0_1], -9.0, 5.4, 4.5);
	bezierAddControlPoint(paths[E_CURVES_S0_1], 2.3, -18.8, 6.3);
	bezierAddControlPoint(paths[E_CURVES_S0_1], -1, 3.7, 6.3);

	paths[E_CURVES_S1_0] = createBezier(30);
	bezierAddControlPoint(paths[E_CURVES_S1_0], 0.2, -60.0, 8.5);
	bezierAddControlPoint(paths[E_CURVES_S1_0], 0.2, 10, 8.5);
	bezierAddControlPoint(paths[E_CURVES_S1_0], 0.2, 10, 8.5);
	bezierAddControlPoint(paths[E_CURVES_S1_0], 0.2, 14.0, 8.5);
	bezierAddControlPoint(paths[E_CURVES_S1_0], -8, 7.0, 8.5);
	bezierAddControlPoint(paths[E_CURVES_S1_0], -12, 7.0, 8.5);
	bezierAddControlPoint(paths[E_CURVES_S1_0], -12, 50.0, 8.5);
	bezierAddControlPoint(paths[E_CURVES_S1_0],  0, 6.0, 8.5);
	bezierAddControlPoint(paths[E_CURVES_S1_0], 24, 0.0, 8.5);
	bezierAddControlPoint(paths[E_CURVES_S1_0], 14, 0.0, 8.5);
	bezierAddControlPoint(paths[E_CURVES_S1_0], 5, 8.0, 8.5);
	bezierAddControlPoint(paths[E_CURVES_S1_0], 0, 8.0, 8.5);
	bezierAddControlPoint(paths[E_CURVES_S1_0], -5, 8.0, 8.5);
	bezierAddControlPoint(paths[E_CURVES_S1_0], -100, 14.0, 8.5);
	bezierAddControlPoint(paths[E_CURVES_S1_0], -24, -50.0, 6.5);
	bezierAddControlPoint(paths[E_CURVES_S1_0], -12, -50.0, 4.5);
	bezierAddControlPoint(paths[E_CURVES_S1_0], 14, -40.0, 4.5);
	bezierAddControlPoint(paths[E_CURVES_S1_0], 14, 20.0, 4.5);
	bezierAddControlPoint(paths[E_CURVES_S1_0], 14, 20.0, 6.5);
	bezierAddControlPoint(paths[E_CURVES_S1_0], 14, 20.0, 10.5);
	bezierAddControlPoint(paths[E_CURVES_S1_0], -14, 20.0, 8.5);
	bezierAddControlPoint(paths[E_CURVES_S1_0], -14, 18.0, 8.5);
	bezierAddControlPoint(paths[E_CURVES_S1_0], -30, 18.0, 10.5);
	bezierAddControlPoint(paths[E_CURVES_S1_0], -20, -20.0, 20);

	paths[E_CURVES_S0_2] = createBezier(30);
	bezierAddControlPoint(paths[E_CURVES_S0_2], -1, 3.7, 6.3);
	bezierAddControlPoint(paths[E_CURVES_S0_2], -1, -10.7, 6.3);
	bezierAddControlPoint(paths[E_CURVES_S0_2], -1, -10.7, 15.3);

	
	// lookat curves
	lookAtPaths[E_LOOKAT_CENTER] = createBezier(1);
	bezierAddControlPoint(lookAtPaths[E_LOOKAT_CENTER], 0, 0, 0);
	
	lookAtPaths[E_LOOKAT_HEAD] = createBezier(1);
	bezierAddControlPoint(lookAtPaths[E_LOOKAT_HEAD], -0.6, 10.9, 8.7);

	lookAtPaths[E_LOOKAT_HEAD_END] = createBezier(5);
	bezierAddControlPoint(lookAtPaths[E_LOOKAT_HEAD_END], -0.6, 10.9, 8.7);
	bezierAddControlPoint(lookAtPaths[E_LOOKAT_HEAD_END], -0.6, 10, 8.7);
	bezierAddControlPoint(lookAtPaths[E_LOOKAT_HEAD_END], -0.6, 8, 8.7);
	bezierAddControlPoint(lookAtPaths[E_LOOKAT_HEAD_END], -0.6, -20.7, 2);
	bezierAddControlPoint(lookAtPaths[E_LOOKAT_HEAD_END], -0.6, -200, 200);


	lookAtPaths[E_LOOKAT_TITLES] = createBezier(2);
	bezierAddControlPoint(lookAtPaths[E_LOOKAT_TITLES], -7, 0, 5);

	// test curve
	paths[E_CURVES_TEST_CURVE] = createBezier(2);
	bezierAddControlPoint(paths[E_CURVES_TEST_CURVE], -1, 3.7, 6.3);

}

/**
 * initialise all needed rotations
 */
void initRotations(){
	rotations[E_ROT_FACIAL_SMILE_35][0] = 35;
	rotations[E_ROT_FACIAL_SMILE_35][1] = 0;
	rotations[E_ROT_FACIAL_SMILE_35][2] = 0;
	
	rotations[E_ROT_FACIAL_SMILE_25][0] = 25;
	rotations[E_ROT_FACIAL_SMILE_25][1] = 0;
	rotations[E_ROT_FACIAL_SMILE_25][2] = 0;

	rotations[E_ROT_FACIAL_SAD_25][0] = -25;
	rotations[E_ROT_FACIAL_SAD_25][1] = 0;
	rotations[E_ROT_FACIAL_SAD_25][2] = 0;
}


/**
 * initialise noises
 */
void initNoise(){
	noises[E_NOISES_HANDS] = checkMalloc(sizeof(struct S_texture));
	noises[E_NOISES_HANDS]->textureWidth = NOISESIZE;
	noises[E_NOISES_HANDS]->textureHeight = NOISESIZE;
	generateMultioctaveValueNoise(noises[E_NOISES_HANDS], 20, 1);

	noises[E_NOISES_HEAD] = checkMalloc(sizeof(struct S_texture));
	noises[E_NOISES_HEAD]->textureWidth = NOISESIZE;
	noises[E_NOISES_HEAD]->textureHeight = NOISESIZE;
	generateMultioctaveValueNoise(noises[E_NOISES_HEAD], 4, 1);

	noises[E_NOISES_EYEBROWS] = checkMalloc(sizeof(struct S_texture));
	noises[E_NOISES_EYEBROWS]->textureWidth = NOISESIZE;
	noises[E_NOISES_EYEBROWS]->textureHeight = NOISESIZE;
	generateMultioctaveValueNoise(noises[E_NOISES_EYEBROWS], 2, 1);
}
