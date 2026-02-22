#include "natives.h"

bool assaltoIniciado = false;
Vector3 localDoCrime = {0.0f, 0.0f, 0.0f, 0.0f};

// Função para criar polícia no PS4
void SpawnPolicia(Vector3 pos) {
    Hash copHash = MISC::GET_HASH_KEY("S_M_M_VALCOP_01");
    STREAMING::REQUEST_MODEL(copHash, false);
    
    int timeout = 0;
    while (!STREAMING::HAS_MODEL_LOADED(copHash) && timeout < 50) {
        WAIT(10);
        timeout++;
    }

    if (STREAMING::HAS_MODEL_LOADED(copHash)) {
        for(int i = 0; i < 3; i++) {
            Ped cop = PED::CREATE_PED(copHash, pos.x + (i * 3), pos.y + 10, pos.z, 0.0f, false, false, false, false);
            PED::SET_PED_RELATIONSHIP_GROUP_HASH(cop, MISC::GET_HASH_KEY("REL_COP"));
            TASK::TASK_COMBAT_PED(cop, PLAYER::PLAYER_PED_ID(), 0, 16);
            
            // Blip da Lei no PS4
            int blip = MAP::SET_BLIP_FOR_ENTITY(cop);
            HUD::SET_BLIP_SPRITE(blip, MISC::GET_HASH_KEY("blip_ambient_law"));
            HUD::SET_BLIP_COLOUR(blip, 1); // Vermelho
        }
        STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(copHash);
    }
}

void ModAssaltoSincronizado() {
    Player player = PLAYER::PLAYER_ID();
    Ped playerPed = PLAYER::PLAYER_PED_ID();
    Vector3 pPos = ENTITY::GET_ENTITY_COORDS(playerPed, true);

    Entity target;
    // 1. Lógica de render o banqueiro e abrir grades/portas
    if (!assaltoIniciado && PLAYER::GET_ENTITY_PLAYER_IS_FREE_AIMING_AT(player, &target)) {
        Hash model = ENTITY::GET_ENTITY_MODEL(target);
        
        // Banqueiros (Valentine, Rhodes, Saint Denis)
        if (model == 0x2C047466 || model == 0x959E7979 || model == 0x19273D6B) {
            assaltoIniciado = true;
            TASK::TASK_HANDS_UP(target, -1, playerPed, -1, false);

            // Tenta abrir diferentes modelos de portas de banco (Valentine e Rhodes)
            Hash portas[] = { 
                MISC::GET_HASH_KEY("p_door_vault01x"), 
                MISC::GET_HASH_KEY("p_rhodes_bank_door_01x"),
                MISC::GET_HASH_KEY("p_stdenis_bank_door_01x") 
            };

            for(int i = 0; i < 3; i++) {
                Object door = OBJECT::GET_CLOSEST_OBJECT_OF_TYPE(pPos.x, pPos.y, pPos.z, 15.0f, portas[i], false, false, true);
                if (ENTITY::DOES_ENTITY_EXIST(door)) {
                    Vector3 dPos = ENTITY::GET_ENTITY_COORDS(door, true);
                    // No PS4: Estado 3 destranca e abre a porta
                    OBJECT::SET_DOOR_STATE(portas[i], dPos.x, dPos.y, dPos.z, 3, 0.0f);
                    ENTITY::SET_ENTITY_AS_MISSION_ENTITY(door, true, true);
                }
            }

            PLAYER::SET_PLAYER_WANTED_LEVEL(player, 4, false);
            localDoCrime = pPos;
            SpawnPolicia(pPos);
        }
    }

    // 2. Lógica do Cofre (Explosão e Dinheiro)
    // Procurar tanto o cofre de Valentine quanto o de Rhodes
    Hash safeHash = MISC::GET_HASH_KEY("p_bank_safe_01");
    Object safe = OBJECT::GET_CLOSEST_OBJECT_OF_TYPE(pPos.x, pPos.y, pPos.z, 3.0f, safeHash, false, false, true);

    if (ENTITY::DOES_ENTITY_EXIST(safe)) {
        // Se apertar R2 (Ataque) perto do cofre
        if (PAD::IS_CONTROL_JUST_PRESSED(0, 0x07CEABE4)) { 
            Vector3 sPos = ENTITY::GET_ENTITY_COORDS(safe, true);
            FIRE::ADD_EXPLOSION(sPos.x, sPos.y, sPos.z, 2, 1.0f, true, false, 1.0f);
            
            // Para o cofre "abrir", nós o removemos e adicionamos o dinheiro
            ENTITY::SET_ENTITY_AS_MISSION_ENTITY(safe, true, true);
            ENTITY::DELETE_ENTITY(&safe); 
            
            CASH::MONEY_ADD_CASH(150000); // Adiciona $1.500,00
        }
    }

    // 3. Sistema de Fuga (Reseta o assalto ao se afastar)
    if (assaltoIniciado) {
        float dist = MISC::GET_DISTANCE_BETWEEN_COORDS(pPos.x, pPos.y, pPos.z, localDoCrime.x, localDoCrime.y, localDoCrime.z, true);
        if (dist > 120.0f) {
            assaltoIniciado = false;
            localDoCrime = {0.0f, 0.0f, 0.0f, 0.0f};
        }
    }
}

extern "C" int _start() { 
    while (true) { 
        ModAssaltoSincronizado(); 
        WAIT(10); // Essencial para estabilidade no PS4
    } 
    return 0; 
}
