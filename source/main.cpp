#include "natives.h"

// Definições para evitar erros de tipos do sistema
typedef unsigned long size_t;
#define NULL 0

extern "C" {
    __attribute__((visibility("default"))) int module_start(size_t argc, const void* args);
    __attribute__((visibility("default"))) int module_stop(size_t argc, const void* args);
}

bool verificado = false;

void ProcessarAssalto() {
    Player p = PLAYER::PLAYER_ID(); 
    Ped pP = PLAYER::PLAYER_PED_ID(); 
    Vector3 pos = ENTITY::GET_ENTITY_COORDS(pP, true);

    // Texto verde de confirmação (só aparece uma vez)
    if (!verificado) {
        HUD::_SET_TEXT_COLOR(0, 255, 0, 255);
        HUD::_SET_TEXT_SCALE(0.40f, 0.40f);
        HUD::_DISPLAY_TEXT("ASSALTO PRO 2026: CONECTADO", 0.5f, 0.5f);
        verificado = true;
    }

    // God Mode constante
    ENTITY::SET_ENTITY_HEALTH(pP, 200, 0);

    // Sistema de Máscara (v1.32)
    if (PED::IS_PED_WEARING_ANY_MASK(pP)) {
        PLAYER::SET_PLAYER_IGNORE_LOW_HONOR_COMPLAINTS(p, true);
        PLAYER::SET_PLAYER_WANTED_LEVEL(p, 0, false);
    }
}

extern "C" int module_start(size_t argc, const void* args) {
    for(int i = 0; i < 450; i++) { WAIT(100); } // Espera 45s
    while (true) {
        ProcessarAssalto();
        WAIT(0);
    }
    return 0;
}

extern "C" int module_stop(size_t argc, const void* args) { return 0; }
