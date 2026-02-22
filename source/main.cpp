#include "natives.h"

bool roubar_on = true;
Vector3 localDoCrime = {0.0f, 0.0f, 0.0f};

void SpawnPoliciaComBlip(Vector3 pos) {
    Hash copHash = MISC::GET_HASH_KEY("S_M_M_VALCOP_01");
    STREAMING::REQUEST_MODEL(copHash, false);
    if (STREAMING::HAS_MODEL_LOADED(copHash)) {
        for(int i=0; i<3; i++) {
            Ped cop = PED::CREATE_PED(copHash, pos.x + (i*4), pos.y + 12, pos.z, 0.0f, false, false, false, false);
            PED::SET_PED_RELATIONSHIP_GROUP_HASH(cop, MISC::GET_HASH_KEY("REL_COP"));
            TASK::TASK_COMBAT_PED(cop, PLAYER::PLAYER_PED_ID(), 0, 16);
            int blip = MAP::SET_BLIP_FOR_ENTITY(cop);
            HUD::SET_BLIP_SPRITE(blip, 0x1B1B1B1B);
            HUD::SET_BLIP_COLOUR(blip, 1);
        }
    }
}

void ModAssaltoSincronizado() {
    if (!roubar_on) return;
    Player player = PLAYER::PLAYER_ID();
    Ped playerPed = PLAYER::PLAYER_PED_ID();
    Vector3 pPos = ENTITY::GET_ENTITY_COORDS(playerPed, true);

    Entity target;
    if (PLAYER::GET_ENTITY_PLAYER_IS_FREE_AIMING_AT(player, &target)) {
        Hash model = ENTITY::GET_ENTITY_MODEL(target);
        // Banqueiros Valentine/St Denis
        if (model == 0x2C047466 || model == 0x959E7979) {
            TASK::TASK_HANDS_UP(target, -1, playerPed, -1, false);
            Hash vDoor = MISC::GET_HASH_KEY("p_door_vault01x");
            Object door = OBJECT::GET_CLOSEST_OBJECT_OF_TYPE(pPos.x, pPos.y, pPos.z, 25.0f, vDoor, 0, 0, 1);
            if (ENTITY::DOES_ENTITY_EXIST(door)) {
                Vector3 dPos = ENTITY::GET_ENTITY_COORDS(door, true);
                OBJECT::DOOR_CONTROL(vDoor, dPos.x, dPos.y, dPos.z, false, 1.0f, 0.0f, 0.0f);
                PLAYER::SET_PLAYER_WANTED_LEVEL(player, 4, false);
                localDoCrime = pPos;
                SpawnPoliciaComBlip(pPos);
            }
        }
    }

    Object safe = OBJECT::GET_CLOSEST_OBJECT_OF_TYPE(pPos.x, pPos.y, pPos.z, 2.5f, MISC::GET_HASH_KEY("p_bank_safe_01"), 0, 0, 1);
    if (ENTITY::DOES_ENTITY_EXIST(safe) && PAD::IS_CONTROL_PRESSED(0, 0x5B1736F5) && PAD::IS_CONTROL_JUST_PRESSED(0, 0xD9D0E1C0)) {
        Vector3 sPos = ENTITY::GET_ENTITY_COORDS(safe, true);
        FIRE::ADD_EXPLOSION(sPos.x, sPos.y, sPos.z, 2, 1.5f, true, false, 1.0f);
        ENTITY::SET_ENTITY_AS_MISSION_ENTITY(safe, true, true);
        ENTITY::SET_ENTITY_AS_NO_LONGER_NEEDED(&safe);
        CASH::MONEY_ADD_CASH(150000); 
    }

    if (PLAYER::GET_PLAYER_WANTED_LEVEL(player) > 0 && localDoCrime.x != 0.0f) {
        float dist = MISC::GET_DISTANCE_BETWEEN_COORDS(pPos.x, pPos.y, pPos.z, localDoCrime.x, localDoCrime.y, localDoCrime.z, true);
        if (dist > 80.0f) {
            PLAYER::CLEAR_PLAYER_WANTED_LEVEL(player);
            localDoCrime = {0.0f, 0.0f, 0.0f};
        }
    }
}

extern "C" int _start() { while (true) { ModAssaltoSincronizado(); WAIT(0); } return 0; }
