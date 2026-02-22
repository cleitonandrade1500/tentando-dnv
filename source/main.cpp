#include "natives.h"

bool roubar_on = true; // Comando solicitado

void LoopAssalto() {
    if (!roubar_on) return;

    Player player = PLAYER::PLAYER_ID();
    Entity target;

    // Mira no NPC = Inicia Assalto
    if (PLAYER::GET_ENTITY_PLAYER_IS_FREE_AIMING_AT(player, &target)) {
        if (ENTITY::IS_ENTITY_A_PED(target)) {
            
            Hash model = ENTITY::GET_ENTITY_MODEL(target);
            // Hashes de Banqueiros v1.32
            if (model == 0x2C047466 || model == 0x959E7979) {
                
                // Abre cofre próximo
                Hash vault = 0x12345678; 
                OBJECT::DOOR_CONTROL(vault, 0, 0, 0, false, 1.0, 0, 0);
                
                // Dá $1.000,00
                CASH::MONEY_ADD_CASH(100000);
            }
        }
    }
}

extern "C" int _start() {
    while (true) {
        LoopAssalto();
        // Função de espera do sistema
        for(int i=0; i<100000; i++) { __asm__("nop"); } 
    }
    return 0;
}
