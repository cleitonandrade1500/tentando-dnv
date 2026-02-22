#include "natives.h"
#include "constants.h"

// Configuração solicitada
bool roubar_on = true;

void ProcessBankRobbery() {
    // Verifica se o comando está ativo
    if (!roubar_on) return;

    Player player = PLAYER::PLAYER_ID();
    Ped playerPed = PLAYER::PLAYER_PED_ID();
    Entity targetEntity;

    // 1. Verifica se o jogador está mirando em alguém
    if (PLAYER::GET_ENTITY_PLAYER_IS_FREE_AIMING_AT(player, &targetEntity)) {
        
        // 2. Verifica se o alvo é um NPC (Ped)
        if (ENTITY::IS_ENTITY_A_PED(targetEntity) && !ENTITY::IS_ENTITY_DEAD(targetEntity)) {
            
            Hash model = ENTITY::GET_ENTITY_MODEL(targetEntity);
            
            // 3. IDs dos Banqueiros (CUSA03140 1.32)
            // Modelos: Bank Walker, Bank Clerk e Gerentes
            if (model == MISC::GET_HASH_KEY("U_M_M_BANKWALKER_01") || 
                model == MISC::GET_HASH_KEY("S_M_M_BANKCLERK_01") ||
                model == 0x2C047466) { // Hash direto do gerente

                // Ação do NPC: Levantar as mãos com medo
                TASK::TASK_HANDS_UP(targetEntity, -1, playerPed, -1, false);

                // 4. Lógica do Cofre (Vault)
                // Procura a porta do cofre mais próxima em um raio de 20 metros
                Hash vaultDoorHash = MISC::GET_HASH_KEY("p_door_vault01x");
                Object vaultDoor = OBJECT::GET_CLOSEST_OBJECT_OF_TYPE(
                    ENTITY::GET_ENTITY_COORDS(targetEntity, true).x, 
                    ENTITY::GET_ENTITY_COORDS(targetEntity, true).y, 
                    ENTITY::GET_ENTITY_COORDS(targetEntity, true).z, 
                    20.0f, vaultDoorHash, false, false, true
                );

                if (ENTITY::DOES_ENTITY_EXIST(vaultDoor)) {
                    // Comando para destravar e abrir a porta
                    OBJECT::DOOR_CONTROL(vaultDoorHash, 
                        ENTITY::GET_ENTITY_COORDS(vaultDoor, true).x, 
                        ENTITY::GET_ENTITY_COORDS(vaultDoor, true).y, 
                        ENTITY::GET_ENTITY_COORDS(vaultDoor, true).z, 
                        false, 1.0f, 0.0f, 0
                    );

                    // 5. Entrega de Dinheiro (Injetar na carteira)
                    // No RDR2 o valor é em cents (100000 = $1.000,00)
                    CASH::MONEY_ADD_CASH(100000); 
                    
                    // Feedback visual (Opcional: remove o alvo para não repetir o script no mesmo frame)
                    ENTITY::SET_ENTITY_AS_NO_LONGER_NEEDED(&targetEntity);
                }
            }
        }
    }
}

// Loop principal do Mod Havana
void MainLoop() {
    while (true) {
        ProcessBankRobbery();
        WAIT(0); // Essencial para não travar o PS4
    }
}
