#include "natives.h"

bool roubar_on = true;

void ExecutarMissaoAssalto() {
    if (!roubar_on) return;

    Player player = PLAYER::PLAYER_ID();
    Ped playerPed = PLAYER::PLAYER_PED_ID();
    Entity target;

    // 1. MIRAR NO BANQUEIRO: Ele abre a porta do cofre grande (Vault)
    if (PLAYER::GET_ENTITY_PLAYER_IS_FREE_AIMING_AT(player, &target)) {
        if (ENTITY::IS_ENTITY_A_PED(target) && !ENTITY::IS_ENTITY_DEAD(target)) {
            Hash model = ENTITY::GET_ENTITY_MODEL(target);
            
            // Filtro de Banqueiros de todas as cidades
            if (model == 0x2C047466 || model == 0x959E7979 || model == 0x1E7B92C) {
                TASK::TASK_HANDS_UP(target, -1, playerPed, -1, false);

                // Abre a porta principal do cofre (Vault)
                Hash vaultDoor = MISC::GET_HASH_KEY("p_door_vault01x");
                Object door = OBJECT::GET_CLOSEST_OBJECT_OF_TYPE(ENTITY::GET_ENTITY_COORDS(target, 1).x, ENTITY::GET_ENTITY_COORDS(target, 1).y, ENTITY::GET_ENTITY_COORDS(target, 1).z, 20.0f, vaultDoor, 0, 0, 1);
                
                if (ENTITY::DOES_ENTITY_EXIST(door)) {
                    OBJECT::DOOR_CONTROL(vaultDoor, ENTITY::GET_ENTITY_COORDS(door, 1).x, ENTITY::GET_ENTITY_COORDS(door, 1).y, ENTITY::GET_ENTITY_COORDS(door, 1).z, false, 1.0f, 0, 0);
                }
            }
        }
    }

    // 2. EXPLODIR COFRES PEQUENOS: Se você estiver perto de um cofre trancado e usar o comando
    // Procura por cofres de parede ou balcão (Safe)
    Hash safeHash = MISC::GET_HASH_KEY("p_bank_safe_01"); // Hash do cofre da missão
    Object safe = OBJECT::GET_CLOSEST_OBJECT_OF_TYPE(ENTITY::GET_ENTITY_COORDS(playerPed, 1).x, ENTITY::GET_ENTITY_COORDS(playerPed, 1).y, ENTITY::GET_ENTITY_COORDS(playerPed, 1).z, 2.0f, safeHash, 0, 0, 1);

    if (ENTITY::DOES_ENTITY_EXIST(safe)) {
        // Se o jogador pressionar R1 + Quadrado (Botão padrão do Havana/Plugin) perto do cofre
        if (PAD::IS_CONTROL_PRESSED(0, 0x5B1736F5) && PAD::IS_CONTROL_JUST_PRESSED(0, 0xD9D0E1C0)) {
            // Cria explosão de dinamite no cofre
            FIRE::ADD_EXPLOSION(ENTITY::GET_ENTITY_COORDS(safe, 1).x, ENTITY::GET_ENTITY_COORDS(safe, 1).y, ENTITY::GET_ENTITY_COORDS(safe, 1).z, 2, 1.0f, true, false, 1.0f);
            
            // Remove o cofre e adiciona o dinheiro (Simula a abertura)
            ENTITY::DELETE_ENTITY(&safe);
            CASH::MONEY_ADD_CASH(50000); // $500 por cofre explodido
            
            // 3. LIMPAR POLÍCIA: Remove o nível de procurado após a explosão
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
