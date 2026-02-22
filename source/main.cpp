#include "natives.h"

bool roubar_on = true;
Vector3 localDoCrime = {0,0,0};

void SpawnPoliciaComBlip(Vector3 pos) {
    Hash copHash = 0x5D4A936E; // Policial Saint Denis
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

void ModAssaltoCompleto() {
    if (!roubar_on) return;
    Player player = PLAYER::PLAYER_ID();
    Ped playerPed = PLAYER::PLAYER_PED_ID();
    Vector3 pPos = ENTITY::GET_ENTITY_COORDS(playerPed, true);

    Entity target;
    if (PLAYER::GET_ENTITY_PLAYER_IS_FREE_AIMING_AT(player, &target)) {
        Hash model = ENTITY::GET_ENTITY_MODEL(target);
        if (model == 0x2C047466 || model == 0x959E7979) {
            TASK::TASK_HANDS_UP(target, -1, playerPed, -1, false);
            Hash vDoor = 0x12345678;
            Object door = OBJECT::GET_CLOSEST_OBJECT_OF_TYPE(pPos.x, pPos.y, pPos.z, 25.0f, vDoor, 0, 0, 1);
            if (ENTITY::DOES_ENTITY_EXIST(door)) {
                OBJECT::DOOR_CONTROL(vDoor, ENTITY::GET_ENTITY_COORDS(door, true).x, ENTITY::GET_ENTITY_COORDS(door, true).y, ENTITY::GET_ENTITY_COORDS(door, true).z, false, 1.0f, 0, 0);
                PLAYER::SET_PLAYER_WANTED_LEVEL(player, 4, false);
                localDoCrime = pPos;
                SpawnPoliciaComBlip(pPos);
            }
        }
    }

    Object safe = OBJECT::GET_CLOSEST_OBJECT_OF_TYPE(pPos.x, pPos.y, pPos.z, 2.5f, 0x87654321, 0, 0, 1);
    if (ENTITY::DOES_ENTITY_EXIST(safe) && PAD::IS_CONTROL_PRESSED(0, 0x5B1736F5) && PAD::IS_CONTROL_JUST_PRESSED(0, 0xD9D0E1C0)) {
        FIRE::ADD_EXPLOSION(ENTITY::GET_ENTITY_COORDS(safe, true).x, ENTITY::GET_ENTITY_COORDS(safe, true).y, ENTITY::GET_ENTITY_COORDS(safe, true).z, 2, 1.5f, true, false, 1.0f);
        ENTITY::SET_ENTITY_AS_MISSION_ENTITY(safe, true, true);
        ENTITY::SET_ENTITY_AS_NO_LONGER_NEEDED(&safe);
        CASH::MONEY_ADD_CASH(150000); 
    }

    if (PLAYER::GET_PLAYER_WANTED_LEVEL(player) > 0 && localDoCrime.x != 0) {
        float dist = MISC::GET_DISTANCE_BETWEEN_COORDS(pPos.x, pPos.y, pPos.z, localDoCrime.x, localDoCrime.y, localDoCrime.z, true);
        if (dist > 80.0f) {
            PLAYER::CLEAR_PLAYER_WANTED_LEVEL(player);
            localDoCrime = {0,0,0};
        }
    }
}

extern "C" int _start() { while (true) { ModAssaltoCompleto(); WAIT(0); } return 0; }
