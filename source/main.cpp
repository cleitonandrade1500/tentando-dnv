#include "natives.h"

// --- VARIÁVEIS GLOBAIS ---
bool assaltoAtivo = false;
Vector3 localCrime = {0.0f, 0.0f, 0.0f, 0.0f};
Ped meuGuarda = 0;
int tempoSegurando = 0;

// --- FUNÇÃO: GERENCIAR GUARDA-COSTA (GANGUE) ---
void GerenciarGuardaCosta() {
    Player player = PLAYER::PLAYER_ID();
    Ped pPed = PLAYER::PLAYER_PED_ID();
    Vector3 pPos = ENTITY::GET_ENTITY_COORDS(pPed, true);

    // SEGURANÇA: Remove o guarda se entrar em missão ou cutscene
    if (MISC::GET_MISSION_FLAG() || PLAYER::IS_PLAYER_IN_CUTSCENE(player)) {
        if (ENTITY::DOES_ENTITY_EXIST(meuGuarda)) {
            ENTITY::SET_ENTITY_AS_MISSION_ENTITY(meuGuarda, true, true);
            ENTITY::DELETE_ENTITY(&meuGuarda);
            meuGuarda = 0;
        }
        return;
    }

    // COMANDO: Segurar L1 + R1 por 5 segundos
    if (PAD::IS_CONTROL_PRESSED(0, 0xDB60C503) && PAD::IS_CONTROL_PRESSED(0, 0x3C0A40F2)) {
        tempoSegurando++;
        if (tempoSegurando < 500) HUD::_DISPLAY_TEXT("CHAMANDO REFORCOS DA GANGUE...", 0.5f, 0.85f);

        if (tempoSegurando >= 500) {
            if (ENTITY::DOES_ENTITY_EXIST(meuGuarda)) ENTITY::DELETE_ENTITY(&meuGuarda);

            // Sorteia: John, Sadie, Charles ou Bill
            Hash modelos[] = { 0x106561CA, 0xFE6FBA34, 0x76B13D5E, 0x1A083765 };
            Hash escolhido = modelos[MISC::GET_RANDOM_INT_IN_RANGE(0, 4)];

            STREAMING::REQUEST_MODEL(escolhido, false);
            while (!STREAMING::HAS_MODEL_LOADED(escolhido)) { WAIT(10); }

            meuGuarda = PED::CREATE_PED(escolhido, pPos.x + 2, pPos.y, pPos.z, 0.0f, false, false, false, false);
            PED::SET_PED_AS_GROUP_MEMBER(meuGuarda, PLAYER::GET_PLAYER_GROUP(player));
            PED::SET_PED_CONFIG_FLAG(meuGuarda, 279, true); // Modo Seguir/Proteger
            ENTITY::SET_ENTITY_HEALTH(meuGuarda, 5000, 0); 
            WEAPON::GIVE_WEAPON_TO_PED(meuGuarda, 0x64356159, 500, true, true); // Schofield

            tempoSegurando = 0;
            HUD::_DISPLAY_TEXT("MEMBRO DA GANGUE CHEGOU!", 0.5f, 0.85f);
            WAIT(2000);
        }
    } else { tempoSegurando = 0; }
}

// --- FUNÇÃO PRINCIPAL DO MOD ---
void ProcessarAssalto() {
    Player player = PLAYER::PLAYER_ID();
    Ped pPed = PLAYER::PLAYER_PED_ID();
    Vector3 pPos = ENTITY::GET_ENTITY_COORDS(pPed, true);

    HUD::_SET_TEXT_COLOR(255, 255, 255, 255);
    HUD::_SET_TEXT_SCALE(0.35f, 0.35f);
    HUD::_DISPLAY_TEXT("ROUBAR ON", 0.5f, 0.90f);

    GerenciarGuardaCosta();
    PLAYER::SET_PLAYER_IGNORE_LOW_HONOR_COMPLAINTS(player, PED::IS_PED_WEARING_ANY_MASK(pPed));

    // 1. MIRAR NO BANQUEIRO (Valentine, Rhodes, St Denis, Blackwater)
    Entity alvo;
    if (!assaltoAtivo && PLAYER::GET_ENTITY_PLAYER_IS_FREE_AIMING_AT(player, &alvo)) {
        Hash m = ENTITY::GET_ENTITY_MODEL(alvo);
        if (m == 0x2C047466 || m == 0x959E7979 || m == 0x19273D6B) {
            assaltoAtivo = true;
            TASK::TASK_HANDS_UP(alvo, -1, pPed, -1, false);

            Hash pts[] = { 0x7C030E57, 0x5D35D3D1, 0x242D7D7D, 0x4894379D, 0x5395A642 };
            for(int i=0; i<5; i++) {
                Object d = OBJECT::GET_CLOSEST_OBJECT_OF_TYPE(pPos.x, pPos.y, pPos.z, 20.0f, pts[i], false, false, true);
                if (ENTITY::DOES_ENTITY_EXIST(d)) OBJECT::SET_DOOR_STATE(pts[i], pPos.x, pPos.y, pPos.z, 3, 0.0f);
            }
            PLAYER::SET_PLAYER_WANTED_LEVEL(player, 4, false);
            localCrime = pPos;
        }
    }

    // 2. EXPLODIR COFRE (R2)
    Hash cofres[] = { 0x1873C856, 0x5395A642, 0x0E63B489 };
    for(int i=0; i<3; i++) {
        Object cf = OBJECT::GET_CLOSEST_OBJECT_OF_TYPE(pPos.x, pPos.y, pPos.z, 4.5f, cofres[i], false, false, true);
        if (ENTITY::DOES_ENTITY_EXIST(cf) && PAD::IS_CONTROL_JUST_PRESSED(0, 0x07CEABE4)) {
            FIRE::ADD_EXPLOSION(pPos.x, pPos.y, pPos.z, 2, 2.0f, true, false, 2.0f);
            ENTITY::DELETE_ENTITY(&cf);
            CASH::MONEY_ADD_CASH(cofres[i] == 0x5395A642 ? 500000 : 150000);
        }
    }

    if (assaltoAtivo && MISC::GET_DISTANCE_BETWEEN_COORDS(pPos.x, pPos.y, pPos.z, localCrime.x, localCrime.y, localCrime.z, true) > 120.0f) {
        assaltoAtivo = false;
    }
}

extern "C" int _start() { while (true) { ProcessarAssalto(); WAIT(0); } return 0; }
