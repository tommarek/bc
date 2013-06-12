#ifndef MATERIAL_H
#define MATERIAL_H

#include "framework/materialmanager/materialmanager.h"

enum E_Material {
        E_MAT_PIANO_BODY = 0,
        E_MAT_PIANO_STRINGS,
        E_MAT_PIANO_WHITE_KEY,
        E_MAT_PIANO_BLACK_KEY,

        E_MAT_PUPPET_WOOD,
        E_MAT_PUPPET_HAIR,
        E_MAT_PUPPET_WIG,
        E_MAT_PUPPET_EYE,

        E_MAT_TEXT,

        E_MAT_GROUND,

        E_MAT_COUNT,
};

void initMaterials();
#endif