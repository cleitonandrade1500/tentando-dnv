#include "natives.h"

bool roubar_on = true;

void ExecutarMissaoAssalto() {
    if (!roubar_on) return;

    Player player = PLAYER::PLAYER_ID();
    Ped playerPed = PLAYER::PLAYER_PED_ID();
    Entity target;

    // 1. Mirar no Banqueiro abre o Vault
    if (PLAYER::GET_ENTITY_PLAYER_IS_FREE_AIMING_AT(player, &target)) {
        if (ENTITY::IS_ENTITY_A_PED(target) && !ENTITY::IS_ENTITY_DEAD(target)) {
            Hash model = ENTITY::GET_ENTITY_MODEL(target);
            if (model == 0x2C047466 || model == 0x959E7979 || model == 0x1E7B92C) {
                TASK::TASK_HANDS_UP(target, -1, playerPed, -1, false);
                Hash vaultDoor = 0x12345678; 
                Vector3 c = ENTITY::GET_ENTITY_COORDS(target, true);
                Object door = OBJECT::GET_CLOSEST_OBJECT_OF_TYPE(c.x, c.y, c.z, 20.0f, vaultDoor, 0, 0, 1);
                if (ENTITY::DOES_ENTITY_EXIST(door)) {
                    Vector3 dc = ENTITY::GET_ENTITY_COORDS(door, true);
                    OBJECT::DOOR_CONTROL(vaultDoor, dc.x, dc.y, dc.z, false, 1.0f, 0, 0);
                }
            }
        }
    }

    // 2. Explodir cofre com R1 + Quadrado
    Hash safeHash = 0x87654321;
    Vector3 pc = ENTITY::GET_ENTITY_COORDS(playerPed, true);
    Object safe = OBJECT::GET_CLOSEST_OBJECT_OF_TYPE(pc.x, pc.y, pc.z, 2.0f, safeHash, 0, 0, 1);

    if (ENTITY::DOES_ENTITY_EXIST(safe)) {
        if (PAD::IS_CONTROL_PRESSED(0, 0x5B1736F5) && PAD::IS_CONTROL_JUST_PRESSED(0, 0xD9D0E1C0)) {
            Vector3 sc = ENTITY::GET_ENTITY_COORDS(safe, true);
            FIRE::ADD_EXPLOSION(sc.x, sc.y, sc.z, 2, 1.0f, true, false, 1.0f);
            ENTITY::SET_ENTITY_AS_MISSION_ENTITY(safe, true, true);
            ENTITY::SET_ENTITY_AS_NO_LONGER_NEEDED(&safe); 
            CASH::MONEY_ADD_CASH(50000); 
            PLAYER::CLEAR_PLAYER_WANTED_LEVEL(player);
        }
    }
}

extern "C" int _start() {
    while (true) {
        ExecutarMissaoAssalto();
        WAIT(0);
    }
    return 0;
}
