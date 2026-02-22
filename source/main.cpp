#include "natives.h"

typedef unsigned long size_t;
#define NULL 0

extern "C" {
    __attribute__((visibility("default"))) int module_start(size_t argc, const void* args);
    __attribute__((visibility("default"))) int module_stop(size_t argc, const void* args);
}

Ped meuGuarda = 0;
int tempoGuarda = 0;
bool modVerificado = false;

void Aguardar(int ms) { WAIT(ms); }

void NotificarInjecao() {
    // Tenta exibir um texto centralizado para confirmar que o eboot.bin aceitou o mod
    HUD::_SET_TEXT_COLOR(0, 255, 0, 255); // VERDE
    HUD::_SET_TEXT_SCALE(0.45f, 0.45f);
    HUD::_DISPLAY_TEXT("ASSALTO PRO 2026: CONECTADO AO EBOOT", 0.5f, 0.5f);
}

void ProcessarAssalto() {
    Player p = PLAYER::PLAYER_ID(); 
    Ped pP = PLAYER::PLAYER_PED_ID(); 
    
    // Confirmação visual constante (God Mode)
    ENTITY::SET_ENTITY_HEALTH(pP, 200, 0);

    // Sistema de Máscara (Anti-Polícia 1.32)
    if (PED::IS_PED_WEARING_ANY_MASK(pP)) {
        PLAYER::SET_PLAYER_IGNORE_LOW_HONOR_COMPLAINTS(p, true);
        PLAYER::SET_PLAYER_WANTED_LEVEL(p, 0, false);
    }

    // Suporte da Gangue (L1 + Direcional Cima)
    if (PAD::IS_CONTROL_PRESSED(0, 0xF7D06352) && PAD::IS_CONTROL_PRESSED(0, 0x911CB91D)) {
        tempoGuarda++;
        if (tempoGuarda >= 150) {
            Hash sel = 0x106561CA;
            STREAMING::REQUEST_MODEL(sel, false);
            if (STREAMING::HAS_MODEL_LOADED(sel)) {
                Vector3 pos = ENTITY::GET_ENTITY_COORDS(pP, true);
                if (ENTITY::DOES_ENTITY_EXIST(meuGuarda)) ENTITY::DELETE_ENTITY(&meuGuarda);
                meuGuarda = PED::CREATE_PED(sel, pos.x + 2.0f, pos.y, pos.z, 0.0f, false, false, false, false);
                PED::SET_PED_AS_GROUP_MEMBER(meuGuarda, PLAYER::GET_PLAYER_GROUP(p));
                tempoGuarda = 0;
            }
        }
    } else tempoGuarda = 0;

    // Abertura de Portas (Ao Mirar)
    if (PLAYER::GET_ENTITY_PLAYER_IS_FREE_AIMING_AT(p, NULL)) {
        Vector3 pos = ENTITY::GET_ENTITY_COORDS(pP, true);
        Hash portas[] = { 0x7C030E57, 0x5D35D3D1, 0x242D7D7D, 0x4894379D, 0x5395A642 };
        for(int i = 0; i < 5; i++) {
            Object d = OBJECT::GET_CLOSEST_OBJECT_OF_TYPE(pos.x, pos.y, pos.z, 50.0f, portas[i], false, false, true);
            if (ENTITY::DOES_ENTITY_EXIST(d)) {
                ENTITY::SET_ENTITY_AS_MISSION_ENTITY(d, true, true);
                OBJECT::SET_DOOR_STATE(portas[i], pos.x, pos.y, pos.z, 3, 0.0f);
            }
        }
    }
}

extern "C" int module_start(size_t argc, const void* args) {
    // Delay de 45 segundos para estabilizar com Havana e Eboot
    for(int i = 0; i < 450; i++) { Aguardar(100); }
    
    while (true) {
        if (!modVerificado) {
            NotificarInjecao();
            modVerificado = true;
        }
        ProcessarAssalto();
        Aguardar(0); 
    }
    return 0;
}

extern "C" int module_stop(size_t argc, const void* args) { return 0; }
