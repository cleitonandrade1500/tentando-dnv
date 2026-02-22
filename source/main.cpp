#include "natives.h"

// Definições de sistema para evitar erros de compilação
typedef unsigned long size_t;
#define NULL 0

extern "C" {
    __attribute__((visibility("default"))) int module_start(size_t argc, const void* args);
    __attribute__((visibility("default"))) int module_stop(size_t argc, const void* args);
}

// Variáveis Globais
Ped meuGuarda = 0;
int tempoGuarda = 0;

void Aguardar(int ms) { WAIT(ms); }

void ProcessarAssalto() {
    // RESOLUÇÃO DOS ERROS DA IMAGEM:
    // O compilador agora reconhece Player, Ped e os Namespaces
    Player p = PLAYER::PLAYER_ID(); 
    Ped pP = PLAYER::PLAYER_PED_ID(); 
    Vector3 pos = ENTITY::GET_ENTITY_COORDS(pP, true);

    // 1. GOD MODE (VIDA INFINITA)
    ENTITY::SET_ENTITY_HEALTH(pP, 200, 0);

    // 2. SISTEMA DE MÁSCARA E POLÍCIA (Anti-Detecção)
    if (PED::IS_PED_WEARING_ANY_MASK(pP)) {
        PLAYER::SET_PLAYER_IGNORE_LOW_HONOR_COMPLAINTS(p, true);
        PLAYER::SET_PLAYER_WANTED_LEVEL(p, 0, false);
    }

    // 3. SUPORTE DA GANGUE (L1 + CIMA)
    if (PAD::IS_CONTROL_PRESSED(0, 0xF7D06352) && PAD::IS_CONTROL_PRESSED(0, 0x911CB91D)) {
        tempoGuarda++;
        if (tempoGuarda >= 150) {
            Hash sel = 0x106561CA; // Marston
            STREAMING::REQUEST_MODEL(sel, false);
            if (STREAMING::HAS_MODEL_LOADED(sel)) {
                if (ENTITY::DOES_ENTITY_EXIST(meuGuarda)) ENTITY::DELETE_ENTITY(&meuGuarda);
                meuGuarda = PED::CREATE_PED(sel, pos.x + 2.0f, pos.y, pos.z, 0.0f, false, false, false, false);
                PED::SET_PED_AS_GROUP_MEMBER(meuGuarda, PLAYER::GET_PLAYER_GROUP(p));
                tempoGuarda = 0;
            }
        }
    } else tempoGuarda = 0;

    // 4. ABERTURA DE PORTAS AO MIRAR
    if (PLAYER::GET_ENTITY_PLAYER_IS_FREE_AIMING_AT(p, NULL)) {
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
    // Delay de 45 segundos para o Havana Mod Menu estabilizar
    for(int i = 0; i < 450; i++) { Aguardar(100); }
    while (true) {
        ProcessarAssalto();
        Aguardar(0); 
    }
    return 0;
}

extern "C" int module_stop(size_t argc, const void* args) { return 0; }
