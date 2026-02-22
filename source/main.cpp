#include "natives.h"

bool assaltoAtivo = false;
Vector3 localCrime = {0.0f, 0.0f, 0.0f, 0.0f};
Ped meuGuarda = 0;
int tempoGuarda = 0;
int tempoRelatorio = 0;
int totalRoubadoSessao = 0;

void GerenciarGuarda() {
    Player p = PLAYER::PLAYER_ID(); Ped pP = PLAYER::PLAYER_PED_ID(); Vector3 pos = ENTITY::GET_ENTITY_COORDS(pP, true);
    if (MISC::GET_MISSION_FLAG() || PLAYER::IS_PLAYER_IN_CUTSCENE(p)) {
        if (ENTITY::DOES_ENTITY_EXIST(meuGuarda)) { ENTITY::DELETE_ENTITY(&meuGuarda); meuGuarda = 0; }
        return;
    }
    if (PAD::IS_CONTROL_PRESSED(0, 0xDB60C503) && PAD::IS_CONTROL_PRESSED(0, 0x3C0A40F2)) { // L1 + R1
        tempoGuarda++;
        if (tempoGuarda >= 500) {
            Hash mods[] = { 0x106561CA, 0xFE6FBA34, 0x76B13D5E, 0x1A083765 };
            Hash sel = mods[MISC::GET_RANDOM_INT_IN_RANGE(0, 4)];
            STREAMING::REQUEST_MODEL(sel, false);
            while (!STREAMING::HAS_MODEL_LOADED(sel)) { WAIT(10); }
            if (ENTITY::DOES_ENTITY_EXIST(meuGuarda)) ENTITY::DELETE_ENTITY(&meuGuarda);
            meuGuarda = PED::CREATE_PED(sel, pos.x + 2.0f, pos.y, pos.z, 0.0f, false, false, false, false);
            PED::SET_PED_AS_GROUP_MEMBER(meuGuarda, PLAYER::GET_PLAYER_GROUP(p));
            PED::SET_PED_CONFIG_FLAG(meuGuarda, 279, true);
            ENTITY::SET_ENTITY_HEALTH(meuGuarda, 5000, 0);
            WEAPON::GIVE_WEAPON_TO_PED(meuGuarda, 0x64356159, 500, true, true);
            tempoGuarda = 0; HUD::_DISPLAY_TEXT("GANGUE CHEGOU!", 0.5f, 0.85f);
        }
    } else tempoGuarda = 0;
}

void ProcessarAssalto() {
    Player p = PLAYER::PLAYER_ID(); Ped pP = PLAYER::PLAYER_PED_ID(); Vector3 pos = ENTITY::GET_ENTITY_COORDS(pP, true);
    HUD::_SET_TEXT_COLOR(255, 255, 255, 255); HUD::_SET_TEXT_SCALE(0.30f, 0.30f); HUD::_DISPLAY_TEXT("ROUBAR ON", 0.5f, 0.94f);
    
    // RELATÓRIO: Segurar Direcional Cima (0xE123C0DD) por 5 segundos
    if (PAD::IS_CONTROL_PRESSED(0, 0xE123C0DD)) {
        tempoRelatorio++;
        if (tempoRelatorio >= 500) {
            HUD::_DISPLAY_TEXT("RELATORIO DO DIA: VER CARTEIRA", 0.5f, 0.85f);
            tempoRelatorio = 0; WAIT(2000);
        }
    } else tempoRelatorio = 0;

    GerenciarGuarda();
    PLAYER::SET_PLAYER_IGNORE_LOW_HONOR_COMPLAINTS(p, PED::IS_PED_WEARING_ANY_MASK(pP));

    Entity alvo;
    if (!assaltoAtivo && PLAYER::GET_ENTITY_PLAYER_IS_FREE_AIMING_AT(p, &alvo)) {
        Hash m = ENTITY::GET_ENTITY_MODEL(alvo);
        if (m == 0x2C047466 || m == 0x959E7979 || m == 0x19273D6B) {
            assaltoAtivo = true; TASK::TASK_HANDS_UP(alvo, -1, pP, -1, false);
            Hash pts[] = { 0x7C030E57, 0x5D35D3D1, 0x242D7D7D, 0x4894379D, 0x5395A642 };
            for(int i=0; i<5; i++) {
                Object d = OBJECT::GET_CLOSEST_OBJECT_OF_TYPE(pos.x, pos.y, pos.z, 20.0f, pts[i], false, false, true);
                if (ENTITY::DOES_ENTITY_EXIST(d)) OBJECT::SET_DOOR_STATE(pts[i], pos.x, pos.y, pos.z, 3, 0.0f);
            }
            PLAYER::SET_PLAYER_WANTED_LEVEL(p, 4, false); localCrime = pos;
        }
    }

    Hash cfs[] = { 0x1873C856, 0x5395A642, 0x0E63B489 };
    for(int i=0; i<3; i++) {
        Object cf = OBJECT::GET_CLOSEST_OBJECT_OF_TYPE(pos.x, pos.y, pos.z, 4.5f, cfs[i], false, false, true);
        if (ENTITY::DOES_ENTITY_EXIST(cf) && PAD::IS_CONTROL_JUST_PRESSED(0, 0x07CEABE4)) {
            FIRE::ADD_EXPLOSION(pos.x, pos.y, pos.z, 2, 2.0f, true, false, 2.0f);
            ENTITY::DELETE_ENTITY(&cf);
            
            // Dinheiro Aleatório Surpresa
            int saque = MISC::GET_RANDOM_INT_IN_RANGE(10000, 300000);
            if (ENTITY::DOES_ENTITY_EXIST(meuGuarda)) saque += MISC::GET_RANDOM_INT_IN_RANGE(5000, 50000);
            CASH::MONEY_ADD_CASH(saque);
            HUD::_DISPLAY_TEXT("SAQUE CONCLUIDO!", 0.5f, 0.85f);
        }
    }
    if (assaltoAtivo && MISC::GET_DISTANCE_BETWEEN_COORDS(pos.x, pos.y, pos.z, localCrime.x, localCrime.y, localCrime.z, true) > 120.0f) assaltoAtivo = false;
}
extern "C" int _start() { while (true) { ProcessarAssalto(); WAIT(0); } return 0; }
