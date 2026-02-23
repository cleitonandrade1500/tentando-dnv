#include "natives.h"

// Variáveis Globais
Ped meuGuarda = 0;
int tempoGuarda = 0;

void LoopAssalto() {
    Player p = PLAYER::PLAYER_ID(); 
    Ped pP = PLAYER::PLAYER_PED_ID(); 
    Vector3 pos = ENTITY::GET_ENTITY_COORDS(pP, true);

    // 1. SUPORTE (L1 + SETA CIMA)
    // Hashes de controles para v1.32
    if (Invoke<bool>(0x58044322, 0, 0xF7D06352) && Invoke<bool>(0x58044322, 0, 0x911CB91D)) {
        tempoGuarda++;
        if (tempoGuarda >= 50) { // Reduzi o tempo para ser mais rápido
            Hash marston = 0x106561CA;
            STREAMING::REQUEST_MODEL(marston, false);
            if (STREAMING::HAS_MODEL_LOADED(marston)) {
                meuGuarda = Invoke<Ped>(0xD4335322, marston, pos.x + 2.0f, pos.y, pos.z, 0.0f, false, false, false, false);
                tempoGuarda = 0;
            }
        }
    }

    // 2. DINHEIRO (Pressione R3 + L3 para ganhar 5000)
    if (Invoke<bool>(0x58044322, 0, 0x62800322) && Invoke<bool>(0x58044322, 0, 0x62800323)) {
        CASH::MONEY_ADD_CASH(500000); // 5000 cents = $50.00
        WAIT(500); 
    }
}

// FUNÇÃO DE ENTRADA DO GOLDHEN 2.4b
extern "C" int module_start(size_t argc, const void* args) {
    // Delay de segurança para o RDR2 v1.32 carregar os sistemas
    for(int i = 0; i < 300; i++) { WAIT(100); } 

    // O loop principal que faz o mod ter efeito
    while (true) {
        LoopAssalto();
        WAIT(0); // Devolve o processamento ao PS4 (Evita Crash)
    }
    return 0;
}

extern "C" int module_stop(size_t argc, const void* args) { return 0; }
