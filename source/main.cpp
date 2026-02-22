#include "natives.h"

bool assaltoAtivo = false;
Vector3 localCrime = {0.0f, 0.0f, 0.0f, 0.0f};

void ProcessarMod() {
    Player player = PLAYER::PLAYER_ID();
    Ped pPed = PLAYER::PLAYER_PED_ID();
    Vector3 pPos = ENTITY::GET_ENTITY_COORDS(pPed, true);

    // Texto na tela
    HUD::_SET_TEXT_COLOR(255, 255, 255, 255);
    HUD::_SET_TEXT_SCALE(0.35f, 0.35f);
    HUD::_DISPLAY_TEXT("ROUBAR ON", 0.5f, 0.90f);

    // Proteção de Honra com Máscara
    PLAYER::SET_PLAYER_IGNORE_LOW_HONOR_COMPLAINTS(player, PED::IS_PED_WEARING_ANY_MASK(pPed));

    Entity alvo;
    if (!assaltoAtivo && PLAYER::GET_ENTITY_PLAYER_IS_FREE_AIMING_AT(player, &alvo)) {
        Hash m = ENTITY::GET_ENTITY_MODEL(alvo);
        if (m == 0x2C047466 || m == 0x959E7979) { // Banqueiros
            assaltoAtivo = true;
            TASK::TASK_HANDS_UP(alvo, -1, pPed, -1, false);
            
            Hash porta = MISC::GET_HASH_KEY("p_door_vault01x");
            Object d = OBJECT::GET_CLOSEST_OBJECT_OF_TYPE(pPos.x, pPos.y, pPos.z, 20.0f, porta, false, false, true);
            if (ENTITY::DOES_ENTITY_EXIST(d)) {
                OBJECT::SET_DOOR_STATE(porta, pPos.x, pPos.y, pPos.z, 3, 0.0f);
            }
            PLAYER::SET_PLAYER_WANTED_LEVEL(player, 4, false);
            localCrime = pPos;
        }
    }

    // Abrir Cofre com R2 (Ataque)
    Object cofre = OBJECT::GET_CLOSEST_OBJECT_OF_TYPE(pPos.x, pPos.y, pPos.z, 3.0f, MISC::GET_HASH_KEY("p_bank_safe_01"), false, false, true);
    if (ENTITY::DOES_ENTITY_EXIST(cofre) && PAD::IS_CONTROL_JUST_PRESSED(0, 0x07CEABE4)) {
        FIRE::ADD_EXPLOSION(pPos.x, pPos.y, pPos.z, 2, 1.0f, true, false, 1.0f);
        ENTITY::DELETE_ENTITY(&cofre);
        CASH::MONEY_ADD_CASH(150000);
    }

    if (assaltoAtivo && MISC::GET_DISTANCE_BETWEEN_COORDS(pPos.x, pPos.y, pPos.z, localCrime.x, localCrime.y, localCrime.z, true) > 100.0f) {
        assaltoAtivo = false;
    }
}

extern "C" int _start() {
    while (true) { ProcessarMod(); WAIT(0); }
    return 0;
}
