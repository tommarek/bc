#include "scene.h"

#include <assert.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "framework/meshmanager/meshmanager.h"
#include "framework/materialmanager/materialmanager.h"
#include "framework/scenemanager/sceneoperations.h"
#include "framework/util.h"

struct S_sceneNode *scene0[E_S0_COUNT];
struct S_sceneNode *scene1[E_S1_COUNT];
struct S_sceneNode *beginning[E_B_COUNT];

/**
 * init begginning scene
 */
static void initSceneBeginning(){
	GLfloat up[3];

	// create root node
	beginning[E_B_ROOT] = createRoot();
	GLfloat LightAmbient[] = {0.0f, 0.0f, 0.0f, 1.0f};
	GLfloat LightDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
	GLfloat LightSpecular[] = {1.0f, 1.0f, 1.0f, 1.0f};
	GLfloat LightPosition[] = {15.0f, 15.0f, 15.0f, 1.0f};
	GLfloat LightTargetPoint[] = {0.0, 0.0, 0.0};
	GLfloat lightCutOff = 30.0;
	setLight(beginning[E_B_ROOT], LightAmbient, LightDiffuse, LightSpecular, LightPosition, LightTargetPoint, lightCutOff);

	// setup camera
	beginning[E_B_CAM_1_CURVE] = createCurve(beginning[E_B_ROOT], NULL, NULL, 0, 0);
	up[0] =   0;  up[1] =   0;  up[2] =   1;
	beginning[E_B_CAM_1] = createCamera(beginning[E_B_CAM_1_CURVE], 50, 1, 10000, up, NULL, 0, 0);


	// add models
	// body - dependent on root
	beginning[E_B_PIANO_BODY] = createMesh(beginning[E_B_ROOT], E_MESH_PIANO_BODY, E_MAT_PIANO_BODY);
	// bench - dependent on root
	beginning[E_B_PIANO_BENCH] = createMesh(beginning[E_B_ROOT], E_MESH_PIANO_BENCH, E_MAT_PIANO_BODY);
	// strings - dependent on body
	beginning[E_B_PIANO_STRINGS] = createMesh(beginning[E_B_PIANO_BODY], E_MESH_PIANO_STRINGS, E_MAT_PIANO_STRINGS);
	// keys - dependent on body
	for (int i = 0; i < 9; ++i){
		beginning[E_B_PIANO_C0 + 0 + i * 11] = createMesh(beginning[E_B_PIANO_BODY], E_MESH_PIANO_WHITE_KEY, E_MAT_PIANO_WHITE_KEY); // C sharp
		translateNode(beginning[E_B_PIANO_C0 + 0 + i * 11], -0.1875 * 0 + (-0.1875 * 7 * i), 0, 0);

		beginning[E_B_PIANO_C0 + 1 + i * 11] = createMesh(beginning[E_B_PIANO_BODY], E_MESH_PIANO_BLACK_KEY, E_MAT_PIANO_BLACK_KEY); // C flat
		translateNode(beginning[E_B_PIANO_C0 + 1 + i * 11], -0.1875 * 0 + (-0.1875 * 7 * i), 0, 0);

		beginning[E_B_PIANO_C0 + 2 + i * 11] = createMesh(beginning[E_B_PIANO_BODY], E_MESH_PIANO_WHITE_KEY, E_MAT_PIANO_WHITE_KEY); // D sharp
		translateNode(beginning[E_B_PIANO_C0 + 2 + i * 11], -0.1875 * 1 + (-0.1875 * 7 * i), 0, 0);

		beginning[E_B_PIANO_C0 + 3 + i * 11] = createMesh(beginning[E_B_PIANO_BODY], E_MESH_PIANO_BLACK_KEY, E_MAT_PIANO_BLACK_KEY); // D flat
		translateNode(beginning[E_B_PIANO_C0 + 3 + i * 11], -0.1875 * 1 + (-0.1875 * 7 * i), 0, 0);

		beginning[E_B_PIANO_C0 + 4 + i * 11] = createMesh(beginning[E_B_PIANO_BODY], E_MESH_PIANO_WHITE_KEY, E_MAT_PIANO_WHITE_KEY); // E sharp
		translateNode(beginning[E_B_PIANO_C0 + 4 + i * 11], -0.1875 * 2 + (-0.1875 * 7 * i), 0, 0);

		beginning[E_B_PIANO_C0 + 5 + i * 11] = createMesh(beginning[E_B_PIANO_BODY], E_MESH_PIANO_WHITE_KEY, E_MAT_PIANO_WHITE_KEY); // F sharp
		translateNode(beginning[E_B_PIANO_C0 + 5 + i * 11], -0.1875 * 3 + (-0.1875 * 7 * i), 0, 0);

		beginning[E_B_PIANO_C0 + 6 + i * 11] = createMesh(beginning[E_B_PIANO_BODY], E_MESH_PIANO_BLACK_KEY, E_MAT_PIANO_BLACK_KEY); // F flat
		translateNode(beginning[E_B_PIANO_C0 + 6 + i * 11], -0.1875 * 3 + (-0.1875 * 7 * i), 0, 0);

		beginning[E_B_PIANO_C0 + 7 + i * 11] = createMesh(beginning[E_B_PIANO_BODY], E_MESH_PIANO_WHITE_KEY, E_MAT_PIANO_WHITE_KEY); // G sharp
		translateNode(beginning[E_B_PIANO_C0 + 7 + i * 11], -0.1875 * 4 + (-0.1875 * 7 * i), 0, 0);

		beginning[E_B_PIANO_C0 + 8 + i * 11] = createMesh(beginning[E_B_PIANO_BODY], E_MESH_PIANO_BLACK_KEY, E_MAT_PIANO_BLACK_KEY); // G flat
		translateNode(beginning[E_B_PIANO_C0 + 8 + i * 11], -0.1875 * 4 + (-0.1875 * 7 * i), 0, 0);

		beginning[E_B_PIANO_C0 + 9 + i * 11] = createMesh(beginning[E_B_PIANO_BODY], E_MESH_PIANO_WHITE_KEY, E_MAT_PIANO_WHITE_KEY); // A sharp
		translateNode(beginning[E_B_PIANO_C0 + 9 + i * 11], -0.1875 * 5 + (-0.1875 * 7 * i), 0, 0);

		beginning[E_B_PIANO_C0 + 10 + i * 11] = createMesh(beginning[E_B_PIANO_BODY], E_MESH_PIANO_BLACK_KEY, E_MAT_PIANO_BLACK_KEY); // A flat
		translateNode(beginning[E_B_PIANO_C0 + 10 + i * 11], -0.1875 * 5 + (-0.1875 * 7 * i), 0, 0);

		beginning[E_B_PIANO_C0 + 11 + i * 11] = createMesh(beginning[E_B_PIANO_BODY], E_MESH_PIANO_WHITE_KEY, E_MAT_PIANO_WHITE_KEY); // B sharp
		translateNode(beginning[E_B_PIANO_C0 + 11 + i * 11], -0.1875 * 6 + (-0.1875 * 7 * i), 0, 0);
	}

	// ground - dependent on root
	beginning[E_B_GROUND] = createMesh(beginning[E_B_ROOT], E_MESH_GROUND, E_MAT_GROUND);

	// texts
	// author's name
	beginning[E_B_NAME] = createDummy(beginning[E_B_ROOT]);
	translateNode(beginning[E_B_NAME], -6, -600, 600);
	rotateNodeDeg(beginning[E_B_NAME], -45, 0, 0);
	beginning[E_B_NAME0] = createMesh(beginning[E_B_NAME], E_MESH_ALPHABET_T, E_MAT_TEXT); // T
	translateNode(beginning[E_B_NAME0], 1, 0, 0);
	beginning[E_B_NAME1] = createMesh(beginning[E_B_NAME0], E_MESH_ALPHABET_O, E_MAT_TEXT); // O
	translateNode(beginning[E_B_NAME1], 1, 0, 0);
	beginning[E_B_NAME2] = createMesh(beginning[E_B_NAME1], E_MESH_ALPHABET_M, E_MAT_TEXT); // M
	translateNode(beginning[E_B_NAME2], 1, 0, 0);
	beginning[E_B_NAME3] = createMesh(beginning[E_B_NAME2], E_MESH_ALPHABET_A, E_MAT_TEXT); // A
	beginning[E_B_NAME4] = createMesh(beginning[E_B_NAME3], E_MESH_ALPHABET_CARKA, E_MAT_TEXT); // '
	translateNode(beginning[E_B_NAME3], 1, 0, 0);
	beginning[E_B_NAME5] = createMesh(beginning[E_B_NAME4], E_MESH_ALPHABET_S, E_MAT_TEXT); // S
	beginning[E_B_NAME6] = createMesh(beginning[E_B_NAME5], E_MESH_ALPHABET_HACEK, E_MAT_TEXT); // ˇ
	translateNode(beginning[E_B_NAME5], 1, 0, 0);
	beginning[E_B_NAME7] = createMesh(beginning[E_B_NAME4], E_MESH_ALPHABET_M, E_MAT_TEXT); // M
	translateNode(beginning[E_B_NAME7], 3, 0, 0);
	beginning[E_B_NAME8] = createMesh(beginning[E_B_NAME7], E_MESH_ALPHABET_A, E_MAT_TEXT); // A
	translateNode(beginning[E_B_NAME8], 1, 0, 0);
	beginning[E_B_NAME9] = createMesh(beginning[E_B_NAME8], E_MESH_ALPHABET_R, E_MAT_TEXT); // R
	translateNode(beginning[E_B_NAME9], 1, 0, 0);
	beginning[E_B_NAME10] = createMesh(beginning[E_B_NAME9], E_MESH_ALPHABET_E, E_MAT_TEXT); // E
	translateNode(beginning[E_B_NAME10], 1, 0, 0);
	beginning[E_B_NAME11] = createMesh(beginning[E_B_NAME10], E_MESH_ALPHABET_K, E_MAT_TEXT); // K
	translateNode(beginning[E_B_NAME11], 1, 0, 0);

	// school
	beginning[E_B_SCHOOL] = createDummy(beginning[E_B_ROOT]);
	translateNode(beginning[E_B_SCHOOL], -14.9, -500, 500);
	rotateNodeDeg(beginning[E_B_SCHOOL], -45, 0, 0);
	beginning[E_B_SCHOOL0] = createMesh(beginning[E_B_SCHOOL], E_MESH_ALPHABET_B, E_MAT_TEXT); // B
	translateNode(beginning[E_B_SCHOOL0], 1, 0, 0);
	beginning[E_B_SCHOOL1] = createMesh(beginning[E_B_SCHOOL0], E_MESH_ALPHABET_R, E_MAT_TEXT); // R
	translateNode(beginning[E_B_SCHOOL1], 1, 0, 0);
	beginning[E_B_SCHOOL2] = createMesh(beginning[E_B_SCHOOL1], E_MESH_ALPHABET_N, E_MAT_TEXT); // N
	translateNode(beginning[E_B_SCHOOL2], 1, 0, 0);
	beginning[E_B_SCHOOL3] = createMesh(beginning[E_B_SCHOOL2], E_MESH_ALPHABET_O, E_MAT_TEXT); // O
	translateNode(beginning[E_B_SCHOOL3], 1, 0, 0);
	beginning[E_B_SCHOOL4] = createMesh(beginning[E_B_SCHOOL3], E_MESH_ALPHABET_U, E_MAT_TEXT); // U
	translateNode(beginning[E_B_SCHOOL4], 2, 0, 0);
	beginning[E_B_SCHOOL5] = createMesh(beginning[E_B_SCHOOL4], E_MESH_ALPHABET_N, E_MAT_TEXT); // N
	translateNode(beginning[E_B_SCHOOL5], 1, 0, 0);
	beginning[E_B_SCHOOL6] = createMesh(beginning[E_B_SCHOOL5], E_MESH_ALPHABET_I, E_MAT_TEXT); // I
	translateNode(beginning[E_B_SCHOOL6], 0.8, 0, 0);
	beginning[E_B_SCHOOL7] = createMesh(beginning[E_B_SCHOOL6], E_MESH_ALPHABET_V, E_MAT_TEXT); // V
	translateNode(beginning[E_B_SCHOOL7], 0.8, 0, 0);
	beginning[E_B_SCHOOL8] = createMesh(beginning[E_B_SCHOOL7], E_MESH_ALPHABET_E, E_MAT_TEXT); // E
	translateNode(beginning[E_B_SCHOOL8], 1, 0, 0);
	beginning[E_B_SCHOOL9] = createMesh(beginning[E_B_SCHOOL8], E_MESH_ALPHABET_R, E_MAT_TEXT); // R
	translateNode(beginning[E_B_SCHOOL9], 1, 0, 0);
	beginning[E_B_SCHOOL10] = createMesh(beginning[E_B_SCHOOL9], E_MESH_ALPHABET_S, E_MAT_TEXT); // S
	translateNode(beginning[E_B_SCHOOL10], 1, 0, 0);
	beginning[E_B_SCHOOL11] = createMesh(beginning[E_B_SCHOOL10], E_MESH_ALPHABET_I, E_MAT_TEXT); // I
	translateNode(beginning[E_B_SCHOOL11], 0.8, 0, 0);
	beginning[E_B_SCHOOL12] = createMesh(beginning[E_B_SCHOOL11], E_MESH_ALPHABET_T, E_MAT_TEXT); // T
	translateNode(beginning[E_B_SCHOOL12], 0.8, 0, 0);
	beginning[E_B_SCHOOL13] = createMesh(beginning[E_B_SCHOOL12], E_MESH_ALPHABET_Y, E_MAT_TEXT); // Y --
	translateNode(beginning[E_B_SCHOOL13], 1, 0, 0);
	beginning[E_B_SCHOOL14] = createMesh(beginning[E_B_SCHOOL13], E_MESH_ALPHABET_O, E_MAT_TEXT); // O
	translateNode(beginning[E_B_SCHOOL14], 2, 0, 0);
	beginning[E_B_SCHOOL15] = createMesh(beginning[E_B_SCHOOL14], E_MESH_ALPHABET_F, E_MAT_TEXT); // F
	translateNode(beginning[E_B_SCHOOL15], 1, 0, 0);
	beginning[E_B_SCHOOL16] = createMesh(beginning[E_B_SCHOOL15], E_MESH_ALPHABET_T, E_MAT_TEXT); // T
	translateNode(beginning[E_B_SCHOOL16], 2, 0, 0);
	beginning[E_B_SCHOOL17] = createMesh(beginning[E_B_SCHOOL16], E_MESH_ALPHABET_E, E_MAT_TEXT); // E
	translateNode(beginning[E_B_SCHOOL17], 1, 0, 0);
	beginning[E_B_SCHOOL18] = createMesh(beginning[E_B_SCHOOL17], E_MESH_ALPHABET_C, E_MAT_TEXT); // C
	translateNode(beginning[E_B_SCHOOL18], 1, 0, 0);
	beginning[E_B_SCHOOL19] = createMesh(beginning[E_B_SCHOOL18], E_MESH_ALPHABET_H, E_MAT_TEXT); // H
	translateNode(beginning[E_B_SCHOOL19], 1, 0, 0);
	beginning[E_B_SCHOOL20] = createMesh(beginning[E_B_SCHOOL19], E_MESH_ALPHABET_N, E_MAT_TEXT); // N
	translateNode(beginning[E_B_SCHOOL20], 1, 0, 0);
	beginning[E_B_SCHOOL21] = createMesh(beginning[E_B_SCHOOL20], E_MESH_ALPHABET_O, E_MAT_TEXT); // O
	translateNode(beginning[E_B_SCHOOL21], 1, 0, 0);
	beginning[E_B_SCHOOL22] = createMesh(beginning[E_B_SCHOOL21], E_MESH_ALPHABET_L, E_MAT_TEXT); // L
	translateNode(beginning[E_B_SCHOOL22], 1, 0, 0);
	beginning[E_B_SCHOOL23] = createMesh(beginning[E_B_SCHOOL22], E_MESH_ALPHABET_O, E_MAT_TEXT); // O
	translateNode(beginning[E_B_SCHOOL23], 1, 0, 0);
	beginning[E_B_SCHOOL24] = createMesh(beginning[E_B_SCHOOL23], E_MESH_ALPHABET_G, E_MAT_TEXT); // G
	translateNode(beginning[E_B_SCHOOL24], 1, 0, 0);
	beginning[E_B_SCHOOL25] = createMesh(beginning[E_B_SCHOOL24], E_MESH_ALPHABET_Y, E_MAT_TEXT); // Y
	translateNode(beginning[E_B_SCHOOL25], 1, 0, 0);

	// presents
	beginning[E_B_PRESENTS] = createDummy(beginning[E_B_ROOT]);
	translateNode(beginning[E_B_PRESENTS], -4.5, -400, 400);
	rotateNodeDeg(beginning[E_B_PRESENTS], -45, 0, 0);
	beginning[E_B_PRESENTS0] = createMesh(beginning[E_B_PRESENTS], E_MESH_ALPHABET_P, E_MAT_TEXT); // P
	translateNode(beginning[E_B_PRESENTS0], 1, 0, 0);
	beginning[E_B_PRESENTS1] = createMesh(beginning[E_B_PRESENTS0], E_MESH_ALPHABET_R, E_MAT_TEXT); // R
	translateNode(beginning[E_B_PRESENTS1], 1, 0, 0);
	beginning[E_B_PRESENTS2] = createMesh(beginning[E_B_PRESENTS1], E_MESH_ALPHABET_E, E_MAT_TEXT); // E
	translateNode(beginning[E_B_PRESENTS2], 1, 0, 0);
	beginning[E_B_PRESENTS3] = createMesh(beginning[E_B_PRESENTS2], E_MESH_ALPHABET_S, E_MAT_TEXT); // S
	translateNode(beginning[E_B_PRESENTS3], 1, 0, 0);
	beginning[E_B_PRESENTS4] = createMesh(beginning[E_B_PRESENTS3], E_MESH_ALPHABET_E, E_MAT_TEXT); // E
	translateNode(beginning[E_B_PRESENTS4], 1, 0, 0);
	beginning[E_B_PRESENTS5] = createMesh(beginning[E_B_PRESENTS4], E_MESH_ALPHABET_N, E_MAT_TEXT); // N
	translateNode(beginning[E_B_PRESENTS5], 1, 0, 0);
	beginning[E_B_PRESENTS6] = createMesh(beginning[E_B_PRESENTS5], E_MESH_ALPHABET_T, E_MAT_TEXT); // T
	translateNode(beginning[E_B_PRESENTS6], 1, 0, 0);
	beginning[E_B_PRESENTS7] = createMesh(beginning[E_B_PRESENTS6], E_MESH_ALPHABET_S, E_MAT_TEXT); // S
	translateNode(beginning[E_B_PRESENTS7], 1, 0, 0);


	// everything is possible
	beginning[E_B_TITLE_1] = createDummy(beginning[E_B_ROOT]);
	translateNode(beginning[E_B_TITLE_1], 2, 5, 13.5);
	rotateNodeDeg(beginning[E_B_TITLE_1], 0, 0, 140);
	beginning[E_B_TITLE_1_0] = createMesh(beginning[E_B_TITLE_1], E_MESH_ALPHABET_E, E_MAT_TEXT); // E
	translateNode(beginning[E_B_TITLE_1_0], 1, 0, 0);
	beginning[E_B_TITLE_1_1] = createMesh(beginning[E_B_TITLE_1_0], E_MESH_ALPHABET_V, E_MAT_TEXT); // V
	translateNode(beginning[E_B_TITLE_1_1], 1, 0, 0);
	beginning[E_B_TITLE_1_2] = createMesh(beginning[E_B_TITLE_1_1], E_MESH_ALPHABET_E, E_MAT_TEXT); // E
	translateNode(beginning[E_B_TITLE_1_2], 1, 0, 0);
	beginning[E_B_TITLE_1_3] = createMesh(beginning[E_B_TITLE_1_2], E_MESH_ALPHABET_R, E_MAT_TEXT); // R
	translateNode(beginning[E_B_TITLE_1_3], 1, 0, 0);
	beginning[E_B_TITLE_1_4] = createMesh(beginning[E_B_TITLE_1_3], E_MESH_ALPHABET_Y, E_MAT_TEXT); // Y
	translateNode(beginning[E_B_TITLE_1_4], 1, 0, 0);
	beginning[E_B_TITLE_1_5] = createMesh(beginning[E_B_TITLE_1_4], E_MESH_ALPHABET_T, E_MAT_TEXT); // T
	translateNode(beginning[E_B_TITLE_1_5], 1, 0, 0);
	beginning[E_B_TITLE_1_6] = createMesh(beginning[E_B_TITLE_1_5], E_MESH_ALPHABET_H, E_MAT_TEXT); // H
	translateNode(beginning[E_B_TITLE_1_6], 1, 0, 0);
	beginning[E_B_TITLE_1_7] = createMesh(beginning[E_B_TITLE_1_6], E_MESH_ALPHABET_I, E_MAT_TEXT); // I
	translateNode(beginning[E_B_TITLE_1_7], 1, 0, 0);
	beginning[E_B_TITLE_1_8] = createMesh(beginning[E_B_TITLE_1_7], E_MESH_ALPHABET_N, E_MAT_TEXT); // N
	translateNode(beginning[E_B_TITLE_1_8], 1, 0, 0);
	beginning[E_B_TITLE_1_9] = createMesh(beginning[E_B_TITLE_1_8], E_MESH_ALPHABET_G, E_MAT_TEXT); // G
	translateNode(beginning[E_B_TITLE_1_9], 1, 0, 0);
	beginning[E_B_TITLE_1_10] = createMesh(beginning[E_B_TITLE_1], E_MESH_ALPHABET_I, E_MAT_TEXT); // I
	translateNode(beginning[E_B_TITLE_1_10], 5, 0, -1.5);
	beginning[E_B_TITLE_1_11] = createMesh(beginning[E_B_TITLE_1_10], E_MESH_ALPHABET_S, E_MAT_TEXT); // S
	translateNode(beginning[E_B_TITLE_1_11], 1, 0, 0);
	beginning[E_B_TITLE_1_12] = createMesh(beginning[E_B_TITLE_1], E_MESH_ALPHABET_P, E_MAT_TEXT); // P
	translateNode(beginning[E_B_TITLE_1_12], 2, 0, -3);
	beginning[E_B_TITLE_1_13] = createMesh(beginning[E_B_TITLE_1_12], E_MESH_ALPHABET_O, E_MAT_TEXT); // O
	translateNode(beginning[E_B_TITLE_1_13], 1, 0, 0);
	beginning[E_B_TITLE_1_14] = createMesh(beginning[E_B_TITLE_1_13], E_MESH_ALPHABET_S, E_MAT_TEXT); // S
	translateNode(beginning[E_B_TITLE_1_14], 1, 0, 0);
	beginning[E_B_TITLE_1_15] = createMesh(beginning[E_B_TITLE_1_14], E_MESH_ALPHABET_S, E_MAT_TEXT); // S
	translateNode(beginning[E_B_TITLE_1_15], 1, 0, 0);
	beginning[E_B_TITLE_1_16] = createMesh(beginning[E_B_TITLE_1_15], E_MESH_ALPHABET_I, E_MAT_TEXT); // I
	translateNode(beginning[E_B_TITLE_1_16], 1, 0, 0);
	beginning[E_B_TITLE_1_17] = createMesh(beginning[E_B_TITLE_1_16], E_MESH_ALPHABET_B, E_MAT_TEXT); // B
	translateNode(beginning[E_B_TITLE_1_17], 1, 0, 0);
	beginning[E_B_TITLE_1_18] = createMesh(beginning[E_B_TITLE_1_17], E_MESH_ALPHABET_L, E_MAT_TEXT); // L
	translateNode(beginning[E_B_TITLE_1_18], 1, 0, 0);
	beginning[E_B_TITLE_1_19] = createMesh(beginning[E_B_TITLE_1_18], E_MESH_ALPHABET_E, E_MAT_TEXT); // L
	translateNode(beginning[E_B_TITLE_1_19], 1, 0, 0);

	beginning[E_B_TITLE_2_CURVE] = createCurve(beginning[E_B_ROOT], NULL, NULL, 0, 0);
	beginning[E_B_TITLE_2] = createDummy(beginning[E_B_TITLE_2_CURVE]);
	scaleNode(beginning[E_B_TITLE_2], 0.5, 0.5, 0.5);
	rotateNodeDeg(beginning[E_B_TITLE_2], 30, 30, 120);
	beginning[E_B_TITLE_2_0] = createMesh(beginning[E_B_TITLE_2], E_MESH_ALPHABET_I, E_MAT_TEXT); // I
	translateNode(beginning[E_B_TITLE_2_0], 1, 0, 0);
	beginning[E_B_TITLE_2_1] = createMesh(beginning[E_B_TITLE_2_0], E_MESH_ALPHABET_N, E_MAT_TEXT); // N
	translateNode(beginning[E_B_TITLE_2_1], 1, 0, 0);
	beginning[E_B_TITLE_2_2] = createMesh(beginning[E_B_TITLE_2_1], E_MESH_ALPHABET_Y, E_MAT_TEXT); // Y
	translateNode(beginning[E_B_TITLE_2_2], 2, 0, 0);
	beginning[E_B_TITLE_2_3] = createMesh(beginning[E_B_TITLE_2_2], E_MESH_ALPHABET_O, E_MAT_TEXT); // O
	translateNode(beginning[E_B_TITLE_2_3], 1, 0, 0);
	beginning[E_B_TITLE_2_4] = createMesh(beginning[E_B_TITLE_2_3], E_MESH_ALPHABET_U, E_MAT_TEXT); // U
	translateNode(beginning[E_B_TITLE_2_4], 1, 0, 0);
	beginning[E_B_TITLE_2_5] = createMesh(beginning[E_B_TITLE_2_4], E_MESH_ALPHABET_R, E_MAT_TEXT); // R
	translateNode(beginning[E_B_TITLE_2_5], 1, 0, 0);
	beginning[E_B_TITLE_2_6] = createMesh(beginning[E_B_TITLE_2], E_MESH_ALPHABET_D, E_MAT_TEXT); // D
	translateNode(beginning[E_B_TITLE_2_6], 2, 0, -1.5);
	beginning[E_B_TITLE_2_7] = createMesh(beginning[E_B_TITLE_2_6], E_MESH_ALPHABET_R, E_MAT_TEXT); // R
	translateNode(beginning[E_B_TITLE_2_7], 1, 0, 0);
	beginning[E_B_TITLE_2_8] = createMesh(beginning[E_B_TITLE_2_7], E_MESH_ALPHABET_E, E_MAT_TEXT); // E
	translateNode(beginning[E_B_TITLE_2_8], 1, 0, 0);
	beginning[E_B_TITLE_2_9] = createMesh(beginning[E_B_TITLE_2_8], E_MESH_ALPHABET_A, E_MAT_TEXT); // A
	translateNode(beginning[E_B_TITLE_2_9], 1, 0, 0);
	beginning[E_B_TITLE_2_10] = createMesh(beginning[E_B_TITLE_2_9], E_MESH_ALPHABET_M, E_MAT_TEXT); // M
	translateNode(beginning[E_B_TITLE_2_10], 1, 0, 0);
	beginning[E_B_TITLE_2_11] = createMesh(beginning[E_B_TITLE_2_10], E_MESH_ALPHABET_S, E_MAT_TEXT); // S
	translateNode(beginning[E_B_TITLE_2_11], 1, 0, 0);




}

/**
 * init 0th scene
 */
static void initScene0(){
	GLfloat up[3];

	// create root node
	scene0[E_S0_ROOT] = createRoot();
	GLfloat LightAmbient[] = {0.0f, 0.0f, 0.0f, 1.0f};
	GLfloat LightDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
	GLfloat LightSpecular[] = {1.0f, 1.0f, 1.0f, 1.0f};
	GLfloat LightPosition[] = {15.0f, 15.0f, 15.0f, 1.0f};
	GLfloat LightTargetPoint[] = {0.0, 0.0, 0.0};
	GLfloat lightCutOff = 180.0;
	setLight(scene0[E_S0_ROOT], LightAmbient, LightDiffuse, LightSpecular, LightPosition, LightTargetPoint, lightCutOff);

	// setup camera
	scene0[E_S0_CAM_1_CURVE] = createCurve(scene0[E_S0_ROOT], NULL, NULL, 0, 0);
	 up[0] =   0;  up[1] =   0;  up[2] =   1;
	scene0[E_S0_CAM_1] = createCamera(scene0[E_S0_CAM_1_CURVE], 50, 1, 10000, up, NULL, 0, 0);

	// add models
	// body - dependent on root
	scene0[E_S0_PIANO_BODY] = createMesh(scene0[E_S0_ROOT], E_MESH_PIANO_BODY, E_MAT_PIANO_WHITE_KEY);
	// bench - dependent on root
	scene0[E_S0_PIANO_BENCH] = createMesh(scene0[E_S0_ROOT], E_MESH_PIANO_BENCH, E_MAT_PIANO_WHITE_KEY);
	// strings - dependent on body
	scene0[E_S0_PIANO_STRINGS] = createMesh(scene0[E_S0_PIANO_BODY], E_MESH_PIANO_STRINGS, E_MAT_PIANO_STRINGS);
	// keys - dependent on body
	for (int i = 0; i < 9; ++i){
		scene0[E_S0_PIANO_C0 + 0 + i * 11] = createMesh(scene0[E_S0_PIANO_BODY], E_MESH_PIANO_WHITE_KEY, E_MAT_PIANO_WHITE_KEY); // C sharp
		translateNode(scene0[E_S0_PIANO_C0 + 0 + i * 11], -0.1875 * 0 + (-0.1875 * 7 * i), 0, 0);

		scene0[E_S0_PIANO_C0 + 1 + i * 11] = createMesh(scene0[E_S0_PIANO_BODY], E_MESH_PIANO_BLACK_KEY, E_MAT_PIANO_BLACK_KEY); // C flat
		translateNode(scene0[E_S0_PIANO_C0 + 1 + i * 11], -0.1875 * 0 + (-0.1875 * 7 * i), 0, 0);

		scene0[E_S0_PIANO_C0 + 2 + i * 11] = createMesh(scene0[E_S0_PIANO_BODY], E_MESH_PIANO_WHITE_KEY, E_MAT_PIANO_WHITE_KEY); // D sharp
		translateNode(scene0[E_S0_PIANO_C0 + 2 + i * 11], -0.1875 * 1 + (-0.1875 * 7 * i), 0, 0);

		scene0[E_S0_PIANO_C0 + 3 + i * 11] = createMesh(scene0[E_S0_PIANO_BODY], E_MESH_PIANO_BLACK_KEY, E_MAT_PIANO_BLACK_KEY); // D flat
		translateNode(scene0[E_S0_PIANO_C0 + 3 + i * 11], -0.1875 * 1 + (-0.1875 * 7 * i), 0, 0);

		scene0[E_S0_PIANO_C0 + 4 + i * 11] = createMesh(scene0[E_S0_PIANO_BODY], E_MESH_PIANO_WHITE_KEY, E_MAT_PIANO_WHITE_KEY); // E sharp
		translateNode(scene0[E_S0_PIANO_C0 + 4 + i * 11], -0.1875 * 2 + (-0.1875 * 7 * i), 0, 0);

		scene0[E_S0_PIANO_C0 + 5 + i * 11] = createMesh(scene0[E_S0_PIANO_BODY], E_MESH_PIANO_WHITE_KEY, E_MAT_PIANO_WHITE_KEY); // F sharp
		translateNode(scene0[E_S0_PIANO_C0 + 5 + i * 11], -0.1875 * 3 + (-0.1875 * 7 * i), 0, 0);

		scene0[E_S0_PIANO_C0 + 6 + i * 11] = createMesh(scene0[E_S0_PIANO_BODY], E_MESH_PIANO_BLACK_KEY, E_MAT_PIANO_BLACK_KEY); // F flat
		translateNode(scene0[E_S0_PIANO_C0 + 6 + i * 11], -0.1875 * 3 + (-0.1875 * 7 * i), 0, 0);

		scene0[E_S0_PIANO_C0 + 7 + i * 11] = createMesh(scene0[E_S0_PIANO_BODY], E_MESH_PIANO_WHITE_KEY, E_MAT_PIANO_WHITE_KEY); // G sharp
		translateNode(scene0[E_S0_PIANO_C0 + 7 + i * 11], -0.1875 * 4 + (-0.1875 * 7 * i), 0, 0);

		scene0[E_S0_PIANO_C0 + 8 + i * 11] = createMesh(scene0[E_S0_PIANO_BODY], E_MESH_PIANO_BLACK_KEY, E_MAT_PIANO_BLACK_KEY); // G flat
		translateNode(scene0[E_S0_PIANO_C0 + 8 + i * 11], -0.1875 * 4 + (-0.1875 * 7 * i), 0, 0);

		scene0[E_S0_PIANO_C0 + 9 + i * 11] = createMesh(scene0[E_S0_PIANO_BODY], E_MESH_PIANO_WHITE_KEY, E_MAT_PIANO_WHITE_KEY); // A sharp
		translateNode(scene0[E_S0_PIANO_C0 + 9 + i * 11], -0.1875 * 5 + (-0.1875 * 7 * i), 0, 0);

		scene0[E_S0_PIANO_C0 + 10 + i * 11] = createMesh(scene0[E_S0_PIANO_BODY], E_MESH_PIANO_BLACK_KEY, E_MAT_PIANO_BLACK_KEY); // A flat
		translateNode(scene0[E_S0_PIANO_C0 + 10 + i * 11], -0.1875 * 5 + (-0.1875 * 7 * i), 0, 0);

		scene0[E_S0_PIANO_C0 + 11 + i * 11] = createMesh(scene0[E_S0_PIANO_BODY], E_MESH_PIANO_WHITE_KEY, E_MAT_PIANO_WHITE_KEY); // B sharp
		translateNode(scene0[E_S0_PIANO_C0 + 11 + i * 11], -0.1875 * 6 + (-0.1875 * 7 * i), 0, 0);
	}

	// puppet
	scene0[E_S0_PUPPET_LOWER_BODY] = createMesh(scene0[E_S0_ROOT], E_MESH_PUPPET_LOWER_BODY, E_MAT_PUPPET_WOOD);
	translateNode(scene0[E_S0_PUPPET_LOWER_BODY], -0.6, 10.3, 3.5);
	storeNoRotMatrix(scene0[E_S0_PUPPET_LOWER_BODY]);

	scene0[E_S0_PUPPET_UPPER_BODY] = createMesh(scene0[E_S0_PUPPET_LOWER_BODY], E_MESH_PUPPET_UPPER_BODY, E_MAT_PUPPET_WOOD);
	translateNode(scene0[E_S0_PUPPET_UPPER_BODY], 0, 0, 1.3);
	storeNoRotMatrix(scene0[E_S0_PUPPET_UPPER_BODY]);

	scene0[E_S0_PUPPET_HEAD] = createMesh(scene0[E_S0_PUPPET_UPPER_BODY], E_MESH_PUPPET_HEAD, E_MAT_PUPPET_WOOD);
	translateNode(scene0[E_S0_PUPPET_HEAD], 0, 0, 2.2);
	storeNoRotMatrix(scene0[E_S0_PUPPET_HEAD]);

	scene0[E_S0_PUPPET_HAWK] = createMesh(scene0[E_S0_PUPPET_HEAD], E_MESH_PUPPET_HAWK, E_MAT_PUPPET_HAIR);
	translateNode(scene0[E_S0_PUPPET_HAWK], 0, -0.1, 1);

	scene0[E_S0_PUPPET_LEFT_ARM] = createMesh(scene0[E_S0_PUPPET_UPPER_BODY], E_MESH_PUPPET_ARM, E_MAT_PUPPET_WOOD);
	translateNode(scene0[E_S0_PUPPET_LEFT_ARM], 1.1, -0.2, 1.1);
	storeNoRotMatrix(scene0[E_S0_PUPPET_LEFT_ARM]);

	scene0[E_S0_PUPPET_RIGHT_ARM] = createMesh(scene0[E_S0_PUPPET_UPPER_BODY], E_MESH_PUPPET_ARM, E_MAT_PUPPET_WOOD);
	translateNode(scene0[E_S0_PUPPET_RIGHT_ARM], -1.1, -0.2, 1.1);
	storeNoRotMatrix(scene0[E_S0_PUPPET_RIGHT_ARM]);

	scene0[E_S0_PUPPET_LEFT_LEG] = createMesh(scene0[E_S0_ROOT], E_MESH_PUPPET_LEG, E_MAT_PUPPET_WOOD);
	scene0[E_S0_PUPPET_RIGHT_LEG] = createMesh(scene0[E_S0_PUPPET_LEFT_LEG], E_MESH_PUPPET_LEG, E_MAT_PUPPET_WOOD);
	translateNode(scene0[E_S0_PUPPET_RIGHT_LEG], -1, 0, 0);

	scene0[E_S0_PUPPET_LEFT_EYE] = createMesh(scene0[E_S0_PUPPET_HEAD], E_MESH_PUPPET_EYE, E_MAT_PUPPET_EYE);
	translateNode(scene0[E_S0_PUPPET_LEFT_EYE], 0.3, -0.65, 1.1);
	storeNoRotMatrix(scene0[E_S0_PUPPET_LEFT_EYE]);
	scene0[E_S0_PUPPET_LEFT_PUPIL] = createMesh(scene0[E_S0_PUPPET_LEFT_EYE], E_MESH_PUPPET_EYE, E_MAT_PUPPET_HAIR);
	translateNode(scene0[E_S0_PUPPET_LEFT_PUPIL], 0, -0.1, -0.05);
	scaleNode(scene0[E_S0_PUPPET_LEFT_PUPIL], 0.4, 0.4, 0.4);

	scene0[E_S0_PUPPET_RIGHT_EYE] = createMesh(scene0[E_S0_PUPPET_HEAD], E_MESH_PUPPET_EYE, E_MAT_PUPPET_EYE);
	translateNode(scene0[E_S0_PUPPET_RIGHT_EYE], -0.3, -0.65, 1.1);
	storeNoRotMatrix(scene0[E_S0_PUPPET_RIGHT_EYE]);
	scene0[E_S0_PUPPET_RIGHT_PUPIL] = createMesh(scene0[E_S0_PUPPET_RIGHT_EYE], E_MESH_PUPPET_EYE, E_MAT_PUPPET_HAIR);
	translateNode(scene0[E_S0_PUPPET_RIGHT_PUPIL], 0, -0.1, -0.05);
	scaleNode(scene0[E_S0_PUPPET_RIGHT_PUPIL], 0.4, 0.4, 0.4);

	scene0[E_S0_PUPPET_MOUTH_PATH] = createCurve(scene0[E_S0_PUPPET_HEAD], NULL, NULL, 0, 0);
	scene0[E_S0_PUPPET_MOUTH] = createMesh(scene0[E_S0_PUPPET_MOUTH_PATH], E_MESH_PUPPET_MOUTH, E_MAT_PUPPET_EYE);
	storeNoRotMatrix(scene0[E_S0_PUPPET_MOUTH]);

	scene0[E_S0_PUPPET_LEFT_EYEBROW] = createMesh(scene0[E_S0_PUPPET_HEAD], E_MESH_PUPPET_EYEBROW, E_MAT_PUPPET_HAIR);
	translateNode(scene0[E_S0_PUPPET_LEFT_EYEBROW], 0.3, -0.7, 1.35);
	storeNoRotMatrix(scene0[E_S0_PUPPET_LEFT_EYEBROW]);

	scene0[E_S0_PUPPET_RIGHT_EYEBROW] = createMesh(scene0[E_S0_PUPPET_HEAD], E_MESH_PUPPET_EYEBROW, E_MAT_PUPPET_HAIR);
	translateNode(scene0[E_S0_PUPPET_RIGHT_EYEBROW], -0.3, -0.7, 1.35);
	storeNoRotMatrix(scene0[E_S0_PUPPET_RIGHT_EYEBROW]);


        // the end
	scene0[E_S0_THEEND] = createDummy(scene0[E_S0_ROOT]);
	translateNode(scene0[E_S0_THEEND], 1, -20, 26);
	rotateNodeDeg(scene0[E_S0_THEEND], 180, 180, 0);
	rotateNodeDeg(scene0[E_S0_THEEND], 45, 0, 0);
	scene0[E_S0_THEEND0] = createMesh(scene0[E_S0_THEEND], E_MESH_ALPHABET_T, E_MAT_TEXT); // T
	translateNode(scene0[E_S0_THEEND0], 1, 0, 0);
	scene0[E_S0_THEEND1] = createMesh(scene0[E_S0_THEEND0], E_MESH_ALPHABET_H, E_MAT_TEXT); // H
	translateNode(scene0[E_S0_THEEND1], 1, 0, 0);
	scene0[E_S0_THEEND2] = createMesh(scene0[E_S0_THEEND1], E_MESH_ALPHABET_E, E_MAT_TEXT); // E
	translateNode(scene0[E_S0_THEEND2], 1, 0, 0);
	scene0[E_S0_THEEND3] = createMesh(scene0[E_S0_THEEND], E_MESH_ALPHABET_E, E_MAT_TEXT); // E
	translateNode(scene0[E_S0_THEEND3], 1, 0, -1.5);
	scene0[E_S0_THEEND4] = createMesh(scene0[E_S0_THEEND3], E_MESH_ALPHABET_N, E_MAT_TEXT); // N
	translateNode(scene0[E_S0_THEEND4], 1, 0, 0);
	scene0[E_S0_THEEND5] = createMesh(scene0[E_S0_THEEND4], E_MESH_ALPHABET_D, E_MAT_TEXT); // D
	translateNode(scene0[E_S0_THEEND5], 1, 0, 0);

	// ground - dependent on root
	scene0[E_S0_GROUND] = createMesh(scene0[E_S0_ROOT], E_MESH_GROUND, E_MAT_PIANO_WHITE_KEY);
}

/**
 * init first scene
 */
static void initScene1(){
	GLfloat up[3];

	// create root node
	scene1[E_S1_ROOT] = createRoot();
	GLfloat LightAmbient[] = {0.0f, 0.0f, 0.0f, 1.0f};
	GLfloat LightDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
	GLfloat LightSpecular[] = {1.0f, 1.0f, 1.0f, 1.0f};
	GLfloat LightPosition[] = {15.0f, 15.0f, 15.0f, 1.0f};
	GLfloat LightTargetPoint[] = {0.0, 0.0, 0.0};
	GLfloat lightCutOff = 30.0;
	setLight(scene1[E_S1_ROOT], LightAmbient, LightDiffuse, LightSpecular, LightPosition, LightTargetPoint, lightCutOff);

	// setup camera
	scene1[E_S1_CAM_1_CURVE] = createCurve(scene1[E_S1_ROOT], NULL, NULL, 0, 0);
	 up[0] =   0;  up[1] =   0;  up[2] =   1;
	scene1[E_S1_CAM_1] = createCamera(scene1[E_S1_CAM_1_CURVE], 50, 1, 10000, up, NULL, 0, 0);

	// add models
	// body - dependent on root
	scene1[E_S1_PIANO_BODY] = createMesh(scene1[E_S1_ROOT], E_MESH_PIANO_BODY, E_MAT_PIANO_BODY);
	// bench - dependent on root
	scene1[E_S1_PIANO_BENCH] = createMesh(scene1[E_S1_ROOT], E_MESH_PIANO_BENCH, E_MAT_PIANO_BODY);
	// strings - dependent on body
	scene1[E_S1_PIANO_STRINGS] = createMesh(scene1[E_S1_PIANO_BODY], E_MESH_PIANO_STRINGS, E_MAT_PIANO_STRINGS);
	// keys - dependent on body
	for (int i = 0; i < 9; ++i){
		scene1[E_S1_PIANO_C0 + 0 + i * 11] = createMesh(scene1[E_S1_PIANO_BODY], E_MESH_PIANO_WHITE_KEY, E_MAT_PIANO_WHITE_KEY); // C sharp
		translateNode(scene1[E_S1_PIANO_C0 + 0 + i * 11], -0.1875 * 0 + (-0.1875 * 7 * i), 0, 0);

		scene1[E_S1_PIANO_C0 + 1 + i * 11] = createMesh(scene1[E_S1_PIANO_BODY], E_MESH_PIANO_BLACK_KEY, E_MAT_PIANO_BLACK_KEY); // C flat
		translateNode(scene1[E_S1_PIANO_C0 + 1 + i * 11], -0.1875 * 0 + (-0.1875 * 7 * i), 0, 0);

		scene1[E_S1_PIANO_C0 + 2 + i * 11] = createMesh(scene1[E_S1_PIANO_BODY], E_MESH_PIANO_WHITE_KEY, E_MAT_PIANO_WHITE_KEY); // D sharp
		translateNode(scene1[E_S1_PIANO_C0 + 2 + i * 11], -0.1875 * 1 + (-0.1875 * 7 * i), 0, 0);

		scene1[E_S1_PIANO_C0 + 3 + i * 11] = createMesh(scene1[E_S1_PIANO_BODY], E_MESH_PIANO_BLACK_KEY, E_MAT_PIANO_BLACK_KEY); // D flat
		translateNode(scene1[E_S1_PIANO_C0 + 3 + i * 11], -0.1875 * 1 + (-0.1875 * 7 * i), 0, 0);

		scene1[E_S1_PIANO_C0 + 4 + i * 11] = createMesh(scene1[E_S1_PIANO_BODY], E_MESH_PIANO_WHITE_KEY, E_MAT_PIANO_WHITE_KEY); // E sharp
		translateNode(scene1[E_S1_PIANO_C0 + 4 + i * 11], -0.1875 * 2 + (-0.1875 * 7 * i), 0, 0);

		scene1[E_S1_PIANO_C0 + 5 + i * 11] = createMesh(scene1[E_S1_PIANO_BODY], E_MESH_PIANO_WHITE_KEY, E_MAT_PIANO_WHITE_KEY); // F sharp
		translateNode(scene1[E_S1_PIANO_C0 + 5 + i * 11], -0.1875 * 3 + (-0.1875 * 7 * i), 0, 0);

		scene1[E_S1_PIANO_C0 + 6 + i * 11] = createMesh(scene1[E_S1_PIANO_BODY], E_MESH_PIANO_BLACK_KEY, E_MAT_PIANO_BLACK_KEY); // F flat
		translateNode(scene1[E_S1_PIANO_C0 + 6 + i * 11], -0.1875 * 3 + (-0.1875 * 7 * i), 0, 0);

		scene1[E_S1_PIANO_C0 + 7 + i * 11] = createMesh(scene1[E_S1_PIANO_BODY], E_MESH_PIANO_WHITE_KEY, E_MAT_PIANO_WHITE_KEY); // G sharp
		translateNode(scene1[E_S1_PIANO_C0 + 7 + i * 11], -0.1875 * 4 + (-0.1875 * 7 * i), 0, 0);

		scene1[E_S1_PIANO_C0 + 8 + i * 11] = createMesh(scene1[E_S1_PIANO_BODY], E_MESH_PIANO_BLACK_KEY, E_MAT_PIANO_BLACK_KEY); // G flat
		translateNode(scene1[E_S1_PIANO_C0 + 8 + i * 11], -0.1875 * 4 + (-0.1875 * 7 * i), 0, 0);

		scene1[E_S1_PIANO_C0 + 9 + i * 11] = createMesh(scene1[E_S1_PIANO_BODY], E_MESH_PIANO_WHITE_KEY, E_MAT_PIANO_WHITE_KEY); // A sharp
		translateNode(scene1[E_S1_PIANO_C0 + 9 + i * 11], -0.1875 * 5 + (-0.1875 * 7 * i), 0, 0);

		scene1[E_S1_PIANO_C0 + 10 + i * 11] = createMesh(scene1[E_S1_PIANO_BODY], E_MESH_PIANO_BLACK_KEY, E_MAT_PIANO_BLACK_KEY); // A flat
		translateNode(scene1[E_S1_PIANO_C0 + 10 + i * 11], -0.1875 * 5 + (-0.1875 * 7 * i), 0, 0);

		scene1[E_S1_PIANO_C0 + 11 + i * 11] = createMesh(scene1[E_S1_PIANO_BODY], E_MESH_PIANO_WHITE_KEY, E_MAT_PIANO_WHITE_KEY); // B sharp
		translateNode(scene1[E_S1_PIANO_C0 + 11 + i * 11], -0.1875 * 6 + (-0.1875 * 7 * i), 0, 0);
	}

	// puppet
	scene1[E_S1_PUPPET_LOWER_BODY] = createMesh(scene1[E_S1_ROOT], E_MESH_PUPPET_LOWER_BODY, E_MAT_PUPPET_WOOD);
	translateNode(scene1[E_S1_PUPPET_LOWER_BODY], -0.6, 10.3, 3.5);
	storeNoRotMatrix(scene1[E_S1_PUPPET_LOWER_BODY]);

	scene1[E_S1_PUPPET_UPPER_BODY] = createMesh(scene1[E_S1_PUPPET_LOWER_BODY], E_MESH_PUPPET_UPPER_BODY, E_MAT_PUPPET_WOOD);
	translateNode(scene1[E_S1_PUPPET_UPPER_BODY], 0, 0, 1.3);
	storeNoRotMatrix(scene1[E_S1_PUPPET_UPPER_BODY]);

	scene1[E_S1_PUPPET_TIE] = createMesh(scene1[E_S1_PUPPET_UPPER_BODY], E_MESH_PUPPET_TIE, E_MAT_PIANO_WHITE_KEY);
	translateNode(scene1[E_S1_PUPPET_TIE], 0, -0.3, 2.0);
	scaleNode(scene1[E_S1_PUPPET_TIE], 0.6, 0.6, 0.6);
	storeNoRotMatrix(scene1[E_S1_PUPPET_TIE]);

	scene1[E_S1_PUPPET_HEAD] = createMesh(scene1[E_S1_PUPPET_UPPER_BODY], E_MESH_PUPPET_HEAD, E_MAT_PUPPET_WOOD);
	translateNode(scene1[E_S1_PUPPET_HEAD], 0, 0, 2.2);
	storeNoRotMatrix(scene1[E_S1_PUPPET_HEAD]);

	scene1[E_S1_PUPPET_HAIR] = createMesh(scene1[E_S1_PUPPET_HEAD], E_MESH_PUPPET_HAIR, E_MAT_PUPPET_HAIR);
	translateNode(scene1[E_S1_PUPPET_HAIR], 0, -0.1, 0.2);
	scaleNode(scene1[E_S1_PUPPET_HAIR], 0.9, 0.9, 0.9);

	scene1[E_S1_PUPPET_LEFT_ARM] = createMesh(scene1[E_S1_PUPPET_UPPER_BODY], E_MESH_PUPPET_ARM, E_MAT_PUPPET_WOOD);
	translateNode(scene1[E_S1_PUPPET_LEFT_ARM], 1.1, -0.2, 1.1);
	storeNoRotMatrix(scene1[E_S1_PUPPET_LEFT_ARM]);

	scene1[E_S1_PUPPET_RIGHT_ARM] = createMesh(scene1[E_S1_PUPPET_UPPER_BODY], E_MESH_PUPPET_ARM, E_MAT_PUPPET_WOOD);
	translateNode(scene1[E_S1_PUPPET_RIGHT_ARM], -1.1, -0.2, 1.1);
	storeNoRotMatrix(scene1[E_S1_PUPPET_RIGHT_ARM]);

	scene1[E_S1_PUPPET_LEFT_LEG] = createMesh(scene1[E_S1_ROOT], E_MESH_PUPPET_LEG, E_MAT_PUPPET_WOOD);
	scene1[E_S1_PUPPET_RIGHT_LEG] = createMesh(scene1[E_S1_PUPPET_LEFT_LEG], E_MESH_PUPPET_LEG, E_MAT_PUPPET_WOOD);
	translateNode(scene1[E_S1_PUPPET_RIGHT_LEG], -1, 0, 0);

	scene1[E_S1_PUPPET_LEFT_EYE] = createMesh(scene1[E_S1_PUPPET_HEAD], E_MESH_PUPPET_EYE, E_MAT_PUPPET_EYE);
	translateNode(scene1[E_S1_PUPPET_LEFT_EYE], 0.3, -0.65, 1.1);
	storeNoRotMatrix(scene1[E_S1_PUPPET_LEFT_EYE]);
	scene1[E_S1_PUPPET_LEFT_PUPIL] = createMesh(scene1[E_S1_PUPPET_LEFT_EYE], E_MESH_PUPPET_EYE, E_MAT_PUPPET_HAIR);
	translateNode(scene1[E_S1_PUPPET_LEFT_PUPIL], 0, -0.1, -0.05);
	scaleNode(scene1[E_S1_PUPPET_LEFT_PUPIL], 0.4, 0.4, 0.4);

	scene1[E_S1_PUPPET_RIGHT_EYE] = createMesh(scene1[E_S1_PUPPET_HEAD], E_MESH_PUPPET_EYE, E_MAT_PUPPET_EYE);
	translateNode(scene1[E_S1_PUPPET_RIGHT_EYE], -0.3, -0.65, 1.1);
	storeNoRotMatrix(scene1[E_S1_PUPPET_RIGHT_EYE]);
	scene1[E_S1_PUPPET_RIGHT_PUPIL] = createMesh(scene1[E_S1_PUPPET_RIGHT_EYE], E_MESH_PUPPET_EYE, E_MAT_PUPPET_HAIR);
	translateNode(scene1[E_S1_PUPPET_RIGHT_PUPIL], 0, -0.1, -0.05);
	scaleNode(scene1[E_S1_PUPPET_RIGHT_PUPIL], 0.4, 0.4, 0.4);

	scene1[E_S1_PUPPET_MOUTH_PATH] = createCurve(scene1[E_S1_PUPPET_HEAD], NULL, NULL, 0, 0);
	scene1[E_S1_PUPPET_MOUTH] = createMesh(scene1[E_S1_PUPPET_MOUTH_PATH], E_MESH_PUPPET_MOUTH, E_MAT_PUPPET_EYE);
	storeNoRotMatrix(scene1[E_S1_PUPPET_MOUTH]);

	scene1[E_S1_PUPPET_LEFT_EYEBROW] = createMesh(scene1[E_S1_PUPPET_HEAD], E_MESH_PUPPET_EYEBROW, E_MAT_PUPPET_HAIR);
	translateNode(scene1[E_S1_PUPPET_LEFT_EYEBROW], 0.3, -0.7, 1.33);
	storeNoRotMatrix(scene1[E_S1_PUPPET_LEFT_EYEBROW]);

	scene1[E_S1_PUPPET_RIGHT_EYEBROW] = createMesh(scene1[E_S1_PUPPET_HEAD], E_MESH_PUPPET_EYEBROW, E_MAT_PUPPET_HAIR);
	translateNode(scene1[E_S1_PUPPET_RIGHT_EYEBROW], -0.3, -0.7, 1.33);
	storeNoRotMatrix(scene1[E_S1_PUPPET_RIGHT_EYEBROW]);

	// ground - dependent on root
	scene1[E_S1_GROUND] = createMesh(scene1[E_S1_ROOT], E_MESH_GROUND, E_MAT_GROUND);
}

/**
 * initialise scenes
 */
void initScenes(){
	initSceneBeginning();
	initScene0();
	initScene1();
    
}