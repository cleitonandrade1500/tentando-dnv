// Definição do comando requisitado
bool roubar_on = true; 

void CheckBankerAim() {
    if (roubar_on) {
        Player player = PLAYER::PLAYER_ID();
        Ped playerPed = PLAYER::PLAYER_PED_ID();
        Entity targetEntity;

        if (PLAYER::GET_ENTITY_PLAYER_IS_FREE_AIMING_AT(player, &targetEntity)) {
            if (ENTITY::IS_ENTITY_A_PED(targetEntity)) {
                // IDs de Banqueiros e Gerentes
                Hash model = ENTITY::GET_ENTITY_MODEL(targetEntity);
                if (model == MISC::GET_HASH_KEY("U_M_M_BANKWALKER_01") || model == 0x2C047466) {
                    
                    // Ação: Abrir Cofre e Dar Dinheiro
                    TASK::TASK_HANDS_UP(targetEntity, -1, playerPed, -1, false);
                    ENTITY::SET_ENTITY_AS_MISSION_ENTITY(targetEntity, true, true);
                    
                    // Comando para abrir cofres próximos (Vaults)
                    Object vault = OBJECT::GET_CLOSEST_OBJECT_OF_TYPE(ENTITY::GET_ENTITY_COORDS(playerPed, 1).x, ENTITY::GET_ENTITY_COORDS(playerPed, 1).y, ENTITY::GET_ENTITY_COORDS(playerPed, 1).z, 20.0, 0x12345678, false, false, true);
                    OBJECT::DOOR_CONTROL(0x12345678, ENTITY::GET_ENTITY_COORDS(vault, 1).x, ENTITY::GET_ENTITY_COORDS(vault, 1).y, ENTITY::GET_ENTITY_COORDS(vault, 1).z, false, 1.0, 0.0, 0);
                    
                    // Adiciona $1.000 direto na carteira
                    CASH::INVENTORY_ADD_CASH_INVENTORY_ITEM(100000, 1); 
                }
            }
        }
    }
}
