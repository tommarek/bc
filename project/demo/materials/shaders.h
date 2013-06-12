#ifndef SHADERS_H
#define SHADERS_H

#include "framework/util.h"
#include "framework/materialmanager/materialmanager.h"

enum E_Shader {
        E_SH_NOTEX_SHADOW = 0,
        E_SH_NOTEX_NOSHADOW,

        E_SH_SHADOW,

        E_SH_COUNT,
};

// vertex shaders
extern const char* vPhong;
extern const char* vShadow;
extern const char* vPhongSphere;
extern const char* vPhongBump;

// fragment shaders
extern const char* fShadow;
extern const char* fPhongNoTexShadow;
extern const char* fPhongNoTexNoShadow;
extern const char* fPhong1tex;
extern const char* fPhongBump;
extern const char* fPhongBumpNoTex;

void initShaders();

#endif