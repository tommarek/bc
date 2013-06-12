#include "mesh.h"

#include <GL/gl.h>
#include <GL/glu.h>

// models
#include "models/piano_body_generated.h"
#include "models/piano_strings_generated.h"
#include "models/ground_generated.h"
#include "models/piano_white_key_generated.h"
#include "models/piano_black_key_generated.h"
#include "models/piano_bench_generated.h"
#include "models/puppet_head_generated.h"
#include "models/puppet_hair_generated.h"
#include "models/puppet_upper_body_generated.h"
#include "models/puppet_lower_body_generated.h"
#include "models/puppet_arm_generated.h"
#include "models/puppet_leg_generated.h"
#include "models/puppet_eye_generated.h"
#include "models/puppet_eyebrow_generated.h"
#include "models/puppet_mouth_generated.h"
#include "models/puppet_hawk_generated.h"
#include "models/a_generated.h"
#include "models/b_generated.h"
#include "models/c_generated.h"
#include "models/d_generated.h"
#include "models/e_generated.h"
#include "models/f_generated.h"
#include "models/g_generated.h"
#include "models/h_generated.h"
#include "models/i_generated.h"
#include "models/j_generated.h"
#include "models/k_generated.h"
#include "models/l_generated.h"
#include "models/m_generated.h"
#include "models/n_generated.h"
#include "models/o_generated.h"
#include "models/p_generated.h"
#include "models/r_generated.h"
#include "models/s_generated.h"
#include "models/t_generated.h"
#include "models/u_generated.h"
#include "models/v_generated.h"
#include "models/w_generated.h"
#include "models/y_generated.h"
#include "models/z_generated.h"
#include "models/carka_generated.h"
#include "models/hacek_generated.h"
#include "models/puppet_tie_generated.h"

#include "framework/util.h"

/**
 * ground
 */
struct S_3DmodelUnpacked *createGround(){
	struct S_3DmodelUnpacked *ground;
	ground = UNPACK_DEMO_MESHES_MODELS_GROUND();

	calculateNormals(ground);
	calculateUVcoords(ground);
	createBuffers(ground);

	return ground;
}

/**
 * white key
 */
struct S_3DmodelUnpacked *createPianoWhiteKey(){
	struct S_3DmodelUnpacked *key;
	key = UNPACK_DEMO_MESHES_MODELS_PIANO_WHITE_KEY();

	calculateNormals(key);
	calculateUVcoords(key);
	createBuffers(key);

	return key;
}

/**
 * black key
 */
struct S_3DmodelUnpacked *createPianoBlackKey(){
	struct S_3DmodelUnpacked *key;
	key = UNPACK_DEMO_MESHES_MODELS_PIANO_BLACK_KEY();

	calculateNormals(key);
	calculateUVcoords(key);
	createBuffers(key);

	return key;
}

/**
 * piano body
 */
struct S_3DmodelUnpacked *createPianoBody(){
	struct S_3DmodelUnpacked *body;
	body = UNPACK_DEMO_MESHES_MODELS_PIANO_BODY();

	calculateNormals(body);
	calculateUVcoords(body);
	createBuffers(body);

	return body;
}

/**
 * piano bench
 */
struct S_3DmodelUnpacked *createPianoBench(){
	struct S_3DmodelUnpacked *bench;
	bench = UNPACK_DEMO_MESHES_MODELS_PIANO_BENCH();

	calculateNormals(bench);
	calculateUVcoords(bench);
	createBuffers(bench);

	return bench;
}
/**
 * piano strings
 */
struct S_3DmodelUnpacked *createPianoStrings(){
	struct S_3DmodelUnpacked *strings;
	strings = UNPACK_DEMO_MESHES_MODELS_PIANO_STRINGS();

	catmullClark(strings);

	calculateNormals(strings);
	calculateUVcoords(strings);
	createBuffers(strings);

	return strings;
}

/**
 * puppet head
 */
struct S_3DmodelUnpacked *createPuppetHead(){
	struct S_3DmodelUnpacked *head;
	head = UNPACK_DEMO_MESHES_MODELS_PUPPET_HEAD();

	catmullClark(head);

	calculateNormals(head);
	calculateUVcoords(head);
	createBuffers(head);

	return head;
}

/**
 * puppet hair
 */
struct S_3DmodelUnpacked *createPuppetHair(){
	struct S_3DmodelUnpacked *hair;
	hair = UNPACK_DEMO_MESHES_MODELS_PUPPET_HAIR();

	catmullClark(hair);

	calculateNormals(hair);
	calculateUVcoords(hair);
	createBuffers(hair);

	return hair;
}

/**
 * puppet tie
 */
struct S_3DmodelUnpacked *createPuppetTie(){
	struct S_3DmodelUnpacked *tie;
	tie = UNPACK_DEMO_MESHES_MODELS_PUPPET_TIE();

	catmullClark(tie);

	calculateNormals(tie);
	calculateUVcoords(tie);
	createBuffers(tie);

	return tie;
}

/**
 * puppet upper body
 */
struct S_3DmodelUnpacked *createPuppetUpperBody(){
	struct S_3DmodelUnpacked *body;
	body = UNPACK_DEMO_MESHES_MODELS_PUPPET_UPPER_BODY();

	catmullClark(body);

	calculateNormals(body);
	calculateUVcoords(body);
	createBuffers(body);

	return body;
}

/**
 * puppet lower body
 */
struct S_3DmodelUnpacked *createPuppetLowerBody(){
	struct S_3DmodelUnpacked *body;
	body = UNPACK_DEMO_MESHES_MODELS_PUPPET_LOWER_BODY();

	catmullClark(body);

	calculateNormals(body);
	calculateUVcoords(body);
	createBuffers(body);

	return body;
}

/**
 * puppet left arm
 */
struct S_3DmodelUnpacked *createPuppetArm(){
	struct S_3DmodelUnpacked *arm;
	arm = UNPACK_DEMO_MESHES_MODELS_PUPPET_ARM();

	catmullClark(arm);

	calculateNormals(arm);
	calculateUVcoords(arm);
	createBuffers(arm);

	return arm;
}

/**
 * puppet left arm
 */
struct S_3DmodelUnpacked *createPuppetLeg(){
	struct S_3DmodelUnpacked *leg;
	leg = UNPACK_DEMO_MESHES_MODELS_PUPPET_LEG();

	catmullClark(leg);

	calculateNormals(leg);
	calculateUVcoords(leg);
	createBuffers(leg);

	return leg;
}

/**
 * puppet eye
 */
struct S_3DmodelUnpacked *createPuppetEye(){
	struct S_3DmodelUnpacked *eye;
	eye = UNPACK_DEMO_MESHES_MODELS_PUPPET_EYE();

	catmullClark(eye);
	catmullClark(eye);

	calculateNormals(eye);
	calculateUVcoords(eye);
	createBuffers(eye);

	return eye;
}

/**
 * puppet eyebrows
 */
struct S_3DmodelUnpacked *createPuppetBrow(){
	struct S_3DmodelUnpacked *brow;
	brow = UNPACK_DEMO_MESHES_MODELS_PUPPET_EYEBROW();

	catmullClark(brow);
	catmullClark(brow);

	calculateNormals(brow);
	calculateUVcoords(brow);
	createBuffers(brow);

	return brow;
}

/**
 * puppet mouth
 */
struct S_3DmodelUnpacked *createPuppetMouth(){
	struct S_3DmodelUnpacked *mouth;
	mouth = UNPACK_DEMO_MESHES_MODELS_PUPPET_MOUTH();

	catmullClark(mouth);
	catmullClark(mouth);

	calculateNormals(mouth);
	calculateUVcoords(mouth);
	createBuffers(mouth);

	return mouth;
}

/**
 * puppet hawk
 */
struct S_3DmodelUnpacked *createPuppetHawk(){
	struct S_3DmodelUnpacked *hawk;
	hawk = UNPACK_DEMO_MESHES_MODELS_PUPPET_HAWK();

	catmullClark(hawk);
	catmullClark(hawk);

	calculateNormals(hawk);
	calculateUVcoords(hawk);
	createBuffers(hawk);

	return hawk;
}


/**
 * initialise whole alphabet
 */
void createAlphabet(){
	struct S_3DmodelUnpacked *tmp;
	tmp = UNPACK_DEMO_MESHES_MODELS_A();
	calculateNormals(tmp);
	calculateUVcoords(tmp);
	createBuffers(tmp);
	meshes[E_MESH_ALPHABET_A] = tmp;

	tmp = UNPACK_DEMO_MESHES_MODELS_B();
	calculateNormals(tmp);
	calculateUVcoords(tmp);
	createBuffers(tmp);
	meshes[E_MESH_ALPHABET_B] = tmp;

	tmp = UNPACK_DEMO_MESHES_MODELS_C();
	calculateNormals(tmp);
	calculateUVcoords(tmp);
	createBuffers(tmp);
	meshes[E_MESH_ALPHABET_C] = tmp;

	tmp = UNPACK_DEMO_MESHES_MODELS_D();
	calculateNormals(tmp);
	calculateUVcoords(tmp);
	createBuffers(tmp);
	meshes[E_MESH_ALPHABET_D] = tmp;

	tmp = UNPACK_DEMO_MESHES_MODELS_E();
	calculateNormals(tmp);
	calculateUVcoords(tmp);
	createBuffers(tmp);
	meshes[E_MESH_ALPHABET_E] = tmp;

	tmp = UNPACK_DEMO_MESHES_MODELS_F();
	calculateNormals(tmp);
	calculateUVcoords(tmp);
	createBuffers(tmp);
	meshes[E_MESH_ALPHABET_F] = tmp;

	tmp = UNPACK_DEMO_MESHES_MODELS_G();
	calculateNormals(tmp);
	calculateUVcoords(tmp);
	createBuffers(tmp);
	meshes[E_MESH_ALPHABET_G] = tmp;

	tmp = UNPACK_DEMO_MESHES_MODELS_H();
	calculateNormals(tmp);
	calculateUVcoords(tmp);
	createBuffers(tmp);
	meshes[E_MESH_ALPHABET_H] = tmp;

	tmp = UNPACK_DEMO_MESHES_MODELS_I();
	calculateNormals(tmp);
	calculateUVcoords(tmp);
	createBuffers(tmp);
	meshes[E_MESH_ALPHABET_I] = tmp;

	tmp = UNPACK_DEMO_MESHES_MODELS_J();
	calculateNormals(tmp);
	calculateUVcoords(tmp);
	createBuffers(tmp);
	meshes[E_MESH_ALPHABET_J] = tmp;

	tmp = UNPACK_DEMO_MESHES_MODELS_K();
	calculateNormals(tmp);
	calculateUVcoords(tmp);
	createBuffers(tmp);
	meshes[E_MESH_ALPHABET_K] = tmp;

	tmp = UNPACK_DEMO_MESHES_MODELS_L();
	calculateNormals(tmp);
	calculateUVcoords(tmp);
	createBuffers(tmp);
	meshes[E_MESH_ALPHABET_L] = tmp;

	tmp = UNPACK_DEMO_MESHES_MODELS_M();
	calculateNormals(tmp);
	calculateUVcoords(tmp);
	createBuffers(tmp);
	meshes[E_MESH_ALPHABET_M] = tmp;

	tmp = UNPACK_DEMO_MESHES_MODELS_N();
	calculateNormals(tmp);
	calculateUVcoords(tmp);
	createBuffers(tmp);
	meshes[E_MESH_ALPHABET_N] = tmp;

	tmp = UNPACK_DEMO_MESHES_MODELS_O();
	calculateNormals(tmp);
	calculateUVcoords(tmp);
	createBuffers(tmp);
	meshes[E_MESH_ALPHABET_O] = tmp;

	tmp = UNPACK_DEMO_MESHES_MODELS_P();
	calculateNormals(tmp);
	calculateUVcoords(tmp);
	createBuffers(tmp);
	meshes[E_MESH_ALPHABET_P] = tmp;

	tmp = UNPACK_DEMO_MESHES_MODELS_R();
	calculateNormals(tmp);
	calculateUVcoords(tmp);
	createBuffers(tmp);
	meshes[E_MESH_ALPHABET_R] = tmp;

	tmp = UNPACK_DEMO_MESHES_MODELS_S();
	calculateNormals(tmp);
	calculateUVcoords(tmp);
	createBuffers(tmp);
	meshes[E_MESH_ALPHABET_S] = tmp;

	tmp = UNPACK_DEMO_MESHES_MODELS_T();
	calculateNormals(tmp);
	calculateUVcoords(tmp);
	createBuffers(tmp);
	meshes[E_MESH_ALPHABET_T] = tmp;

	tmp = UNPACK_DEMO_MESHES_MODELS_U();
	calculateNormals(tmp);
	calculateUVcoords(tmp);
	createBuffers(tmp);
	meshes[E_MESH_ALPHABET_U] = tmp;

	tmp = UNPACK_DEMO_MESHES_MODELS_V();
	calculateNormals(tmp);
	calculateUVcoords(tmp);
	createBuffers(tmp);
	meshes[E_MESH_ALPHABET_V] = tmp;

	tmp = UNPACK_DEMO_MESHES_MODELS_W();
	calculateNormals(tmp);
	calculateUVcoords(tmp);
	createBuffers(tmp);
	meshes[E_MESH_ALPHABET_W] = tmp;

	tmp = UNPACK_DEMO_MESHES_MODELS_Y();
	calculateNormals(tmp);
	calculateUVcoords(tmp);
	createBuffers(tmp);
	meshes[E_MESH_ALPHABET_Y] = tmp;

	tmp = UNPACK_DEMO_MESHES_MODELS_Z();
	calculateNormals(tmp);
	calculateUVcoords(tmp);
	createBuffers(tmp);
	meshes[E_MESH_ALPHABET_Z] = tmp;

	tmp = UNPACK_DEMO_MESHES_MODELS_CARKA();
	calculateNormals(tmp);
	calculateUVcoords(tmp);
	createBuffers(tmp);
	meshes[E_MESH_ALPHABET_CARKA] = tmp;

	tmp = UNPACK_DEMO_MESHES_MODELS_HACEK();
	calculateNormals(tmp);
	calculateUVcoords(tmp);
	createBuffers(tmp);
	meshes[E_MESH_ALPHABET_HACEK] = tmp;
}


/**
 * initialise all meshes
 */
void initMeshes(){
	// piano
	meshes[E_MESH_PIANO_BODY] = createPianoBody();
	meshes[E_MESH_PIANO_BENCH] = createPianoBench();
	meshes[E_MESH_PIANO_STRINGS] = createPianoStrings();
	meshes[E_MESH_PIANO_WHITE_KEY] = createPianoWhiteKey();
	meshes[E_MESH_PIANO_BLACK_KEY] = createPianoBlackKey();

	// puppet
	meshes[E_MESH_PUPPET_HEAD] = createPuppetHead();
	meshes[E_MESH_PUPPET_HAIR] = createPuppetHair();
	meshes[E_MESH_PUPPET_UPPER_BODY] = createPuppetUpperBody();
	meshes[E_MESH_PUPPET_LOWER_BODY] = createPuppetLowerBody();
	meshes[E_MESH_PUPPET_ARM] = createPuppetArm();
	meshes[E_MESH_PUPPET_LEG] = createPuppetLeg();
	meshes[E_MESH_PUPPET_EYE] = createPuppetEye();
	meshes[E_MESH_PUPPET_EYEBROW] = createPuppetBrow();
	meshes[E_MESH_PUPPET_MOUTH] = createPuppetMouth();
	meshes[E_MESH_PUPPET_HAWK] = createPuppetHawk();
	meshes[E_MESH_PUPPET_TIE] = createPuppetTie();

	// ground
	meshes[E_MESH_GROUND] = createGround();

	// alphabet
	createAlphabet();
}