// Comando global para ativar/desativar
bool roubar_on = false;

void ProcessBankRobbery() {
    if (!roubar_on) return;

    // Obtém o NPC que o jogador está mirando
    Entity target;
    if (PLAYER::GET_ENTITY_PLAYER_IS_FREE_AIMING_AT(PLAYER::PLAYER_ID(), &target)) {
        if (ENTITY::IS_ENTITY_A_PED(target)) {
            Ped npc = (Ped)target;
            // Verifica se é um banqueiro (Model Hash ou Check de Localização)
            // Aqui usamos uma lógica simplificada para qualquer NPC em bancos
            
            if (PED::IS_PED_MODEL(npc, MISC::GET_HASH_KEY("U_M_M_BANKWALKER_01")) || 
                PED::IS_PED_MODEL(npc, MISC::GET_HASH_KEY("S_M_M_BANKCLERK_01"))) {
                
                // Animação de render do banqueiro
                TASK::TASK_HANDS_UP(npc, -1, PLAYER::PLAYER_PED_ID(), -1, false);
                
                // Lógica para abrir cofres próximos
                Object vaultDoor = OBJECT::GET_CLOSEST_OBJECT_OF_TYPE(ENTITY::GET_ENTITY_COORDS(npc, true).x, ENTITY::GET_ENTITY_COORDS(npc, true).y, ENTITY::GET_ENTITY_COORDS(npc, true).z, 10.0f, MISC::GET_HASH_KEY("p_door_vault01x"), false, false, true);
                
                if (ENTITY::DOES_ENTITY_EXIST(vaultDoor)) {
                    OBJECT::DOOR_CONTROL(MISC::GET_HASH_KEY("p_door_vault01x"), ENTITY::GET_ENTITY_COORDS(vaultDoor, true).x, ENTITY::GET_ENTITY_COORDS(vaultDoor, true).y, ENTITY::GET_ENTITY_COORDS(vaultDoor, true).z, false, 1.0f, 0.0f, 0);
                    // Adiciona dinheiro diretamente ao inventário (ex: $500)
                    CASH::INVENTORY_ADD_CASH_INVENTORY_ITEM(50000, 1); 
                }
            }
        }
    }
}
