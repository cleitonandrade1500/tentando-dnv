#include "natives.h"

// Configurações solicitadas
bool roubar_on = true; 

void ModLoop() {
    if (!roubar_on) return;

    Player player = PLAYER::PLAYER_ID();
    Ped playerPed = PLAYER::PLAYER_PED_ID();
    Entity target;

    // Se estiver mirando com arma
    if (PLAYER::GET_ENTITY_PLAYER_IS_FREE_AIMING_AT(player, &target)) {
        if (ENTITY::IS_ENTITY_A_PED(target) && !ENTITY::IS_ENTITY_DEAD(target)) {
            
            Hash model = ENTITY::GET_ENTITY_MODEL(target);
            
            // Filtro: Banqueiros e Gerentes (CUSA03140 1.32)
            if (model == 0x2C047466 || model == 0x959E7979 || model == 0x1E7B92C) {
                
                // NPC se rende
                TASK::TASK_HANDS_UP(target, -1, playerPed, -1, false);

                // Abre o cofre mais próximo (p_door_vault01x)
                Hash vaultHash = 0x12345678; // Hash simplificado para 1.32
                Object vault = OBJECT::GET_CLOSEST_OBJECT_OF_TYPE(ENTITY::GET_ENTITY_COORDS(target, 1).x, ENTITY::GET_ENTITY_COORDS(target, 1).y, ENTITY::GET_ENTITY_COORDS(target, 1).z, 20.0, vaultHash, 0, 0, 1);
                
                if (ENTITY::DOES_ENTITY_EXIST(vault)) {
                    OBJECT::DOOR_CONTROL(vaultHash, ENTITY::GET_ENTITY_COORDS(vault, 1).x, ENTITY::GET_ENTITY_COORDS(vault, 1).y, ENTITY::GET_ENTITY_COORDS(vault, 1).z, false, 1.0, 0, 0);
                    
                    // Injeta $1.000,00 (100.000 cents)
                    CASH::MONEY_ADD_CASH(100000);
                    
                    // Impede spam (espera 5 segundos para o próximo)
                    WAIT(5000); 
                }
            }
        }
    }
}

// Ponto de entrada do Mod no PS4
extern "C" int _start() {
    while (true) {
        ModLoop();
        WAIT(0);
    }
    return 0;
}
