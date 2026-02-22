#include "natives.h"

bool roubar_on = true;

void ExecutarMissaoAssalto() {
    if (!roubar_on) return;

    Player player = PLAYER::PLAYER_ID();
    Ped playerPed = PLAYER::PLAYER_PED_ID();
    Entity target;

    // 1. MIRAR NO BANQUEIRO: Ele abre a porta do cofre principal (Vault)
    if (PLAYER::GET_ENTITY_PLAYER_IS_FREE_AIMING_AT(player, &target)) {
        if (ENTITY::IS_ENTITY_A_PED(target) && !ENTITY::IS_ENTITY_DEAD(target)) {
            Hash model = ENTITY::GET_ENTITY_MODEL(target);
            
            // Filtro de Banqueiros (CUSA03140 1.32)
            if (model == 0x2C047466 || model == 0x959E7979 || model == 0x1E7B92C) {
                TASK::TASK_HANDS_UP(target, -1, playerPed, -1, false);

                // Abre a porta principal (Vault)
                Hash vaultDoor = 0x12345678; // Hash simplificado para a porta
                Object door = OBJECT::GET_CLOSEST_OBJECT_OF_TYPE(ENTITY::GET_ENTITY_COORDS(target, 1).x, ENTITY::GET_ENTITY_COORDS(target, 1).y, ENTITY::GET_ENTITY_COORDS(target, 1).z, 20.0f, vaultDoor, 0, 0, 1);
                
                if (ENTITY::DOES_ENTITY_EXIST(door)) {
                    OBJECT::DOOR_CONTROL(vaultDoor, ENTITY::GET_ENTITY_COORDS(door, 1).x, ENTITY::GET_ENTITY_COORDS(door, 1).y, ENTITY::GET_ENTITY_COORDS(door, 1).z, false, 1.0f, 0, 0);
                }
            }
        }
    }

    // 2. EXPLODIR COFRES PEQUENOS (Safes)
    Hash safeHash = 0x87654321; // Hash simplificado para o cofre pequeno
    Object safe = OBJECT::GET_CLOSEST_OBJECT_OF_TYPE(ENTITY::GET_ENTITY_COORDS(playerPed, 1).x, ENTITY::GET_ENTITY_COORDS(playerPed, 1).y, ENTITY::GET_ENTITY_COORDS(playerPed, 1).z, 2.0f, safeHash, 0, 0, 1);

    if (ENTITY::DOES_ENTITY_EXIST(safe)) {
        // Se o jogador pressionar R1 + Quadrado
        if (PAD::IS_CONTROL_PRESSED(0, 0x5B1736F5) && PAD::IS_CONTROL_JUST_PRESSED(0, 0xD9D0E1C0)) {
            
            // Explosão de dinamite
            FIRE::ADD_EXPLOSION(ENTITY::GET_ENTITY_COORDS(safe, 1).x, ENTITY::GET_ENTITY_COORDS(safe, 1).y, ENTITY::GET_ENTITY_COORDS(safe, 1).z, 2, 1.0f, true, false, 1.0f);
            
            // CORREÇÃO DO ERRO: Usando a Native correta para "limpar" o objeto
            ENTITY::SET_ENTITY_AS_MISSION_ENTITY(safe, true, true);
            ENTITY::SET_ENTITY_AS_NO_LONGER_NEEDED(&safe); 
            
            // Adiciona dinheiro
            CASH::MONEY_ADD_CASH(50000); 
            
            // Limpa o Wanted Level
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
