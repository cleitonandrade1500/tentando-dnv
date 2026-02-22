#include "natives.h"

typedef unsigned long size_t;
#define NULL 0

extern "C" {
    __attribute__((visibility("default"))) int module_start(size_t argc, const void* args);
    __attribute__((visibility("default"))) int module_stop(size_t argc, const void* args);
}

Ped meuGuarda = 0;
int tempoGuarda = 0;
bool assaltoAtivo = false;
Vector3 localCrime = {0.0f, 0.0f, 0.0f, 0.0f};

void ProcessarAssalto() {
    Player p = PLAYER::PLAYER_ID(); 
    Ped pP = PLAYER::PLAYER_PED_ID(); 
    Vector3 pos = ENTITY::GET_ENTITY_COORDS(pP, true);

    // 1. SUPORTE MARSTON (L1 + Cima)
    if (PAD::IS_CONTROL_PRESSED(0, 0xF7D06352) && PAD::IS_CONTROL_PRESSED(0, 0x911CB91D)) {
        tempoGuarda++;
        if (tempoGuarda >= 150) {
            Hash sel = 0x106561CA;
            STREAMING::REQUEST_MODEL(sel, false);
            if (STREAMING::HAS_MODEL_LOADED(sel)) {
                if (ENTITY::DOES_ENTITY_EXIST(meuGuarda)) ENTITY::DELETE_ENTITY(&meuGuarda);
                meuGuarda = PED::CREATE_PED(sel, pos.x + 2.0f, pos.y, pos.z, 0.0f, false, false, false, false);
                PED::SET_PED_AS_GROUP_MEMBER(meuGuarda, PLAYER::GET_PLAYER_GROUP(p));
                tempoGuarda = 0;
            }
        }
    } else tempoGuarda = 0;

    // 2. ABERTURA DE PORTAS E ROUBO
    Entity alvo;
    if (PLAYER::GET_ENTITY_PLAYER_IS_FREE_AIMING_AT(p, &alvo)) {
        Hash m = ENTITY::GET_ENTITY_MODEL(alvo);
        if (m == 0x2C047466 || m == 0x959E7979) { // Caixas registradoras
            assaltoAtivo = true; localCrime = pos;
            PLAYER::SET_PLAYER_WANTED_LEVEL(p, 4, false);
            Hash portas[] = { 0x7C030E57, 0x5D35D3D1, 0x242D7D7D, 0x4894379D, 0x5395A642 };
            for(int i = 0; i < 5; i++) {
                Object d = OBJECT::GET_CLOSEST_OBJECT_OF_TYPE(pos.x, pos.y, pos.z, 50.0f, portas[i], false, false, true);
                if (ENTITY::DOES_ENTITY_EXIST(d)) OBJECT::SET_DOOR_STATE(portas[i], pos.x, pos.y, pos.z, 3, 0.0f);
            }
        }
    }

    // 3. EXPLODIR COFRE E GANHAR DINHEIRO
    Hash cofres[] = { 0x1873C856, 0x5395A642, 0x0E63B489 };
    for(int i = 0; i < 3; i++) {
        Object cf = OBJECT::GET_CLOSEST_OBJECT_OF_TYPE(pos.x, pos.y, pos.z, 3.5f, cofres[i], false, false, true);
        if (ENTITY::DOES_ENTITY_EXIST(cf) && PAD::IS_CONTROL_JUST_PRESSED(0, 0x07CEABE4)) {
            FIRE::ADD_EXPLOSION(pos.x, pos.y, pos.z, 2, 1.0f, true, false, 1.0f);
            ENTITY::DELETE_ENTITY(&cf);
            CASH::MONEY_ADD_CASH(MISC::GET_RANDOM_INT_IN_RANGE(20000, 400000));
        }
    }
}

extern "C" int module_start(size_t argc, const void* args) {
    for(int i = 0; i < 450; i++) { WAIT(100); } // 45s Delay
    while (true) { ProcessarAssalto(); WAIT(0); }
    return 0;
}
extern "C" int module_stop(size_t argc, const void* args) { return 0; }
