#include "natives.h"

Ped meuGuarda = 0;
int tempoGuarda = 0;

void LoopAssalto() {
    Player p = PLAYER::PLAYER_ID(); 
    Ped pP = PLAYER::PLAYER_PED_ID(); 
    Vector3 pos = ENTITY::GET_ENTITY_COORDS(pP, true);

    // SUPORTE (L1 + SETA CIMA)
    // Usando Invoke<bool> para leitura direta de botões (PAD)
    if (Invoke<bool>(0x58044322, 0, 0xF7D06352) && Invoke<bool>(0x58044322, 0, 0x911CB91D)) {
        tempoGuarda++;
        if (tempoGuarda >= 50) {
            Hash marston = 0x106561CA;
            STREAMING::REQUEST_MODEL(marston, false);
            if (STREAMING::HAS_MODEL_LOADED(marston)) {
                if (ENTITY::DOES_ENTITY_EXIST(meuGuarda)) ENTITY::DELETE_ENTITY(&meuGuarda);
                meuGuarda = Invoke<Ped>(0xD4335322, marston, pos.x + 2.0f, pos.y, pos.z, 0.0f, false, false, false, false);
                tempoGuarda = 0;
            }
        }
    } else { tempoGuarda = 0; }
}

// Função de entrada do SPRX para o GoldHEN
extern "C" int module_start(size_t argc, const void* args) {
    for(int i = 0; i < 250; i++) { WAIT(100); } 
    while (true) {
        LoopAssalto();
        WAIT(0);
    }
    return 0;
}

extern "C" int module_stop(size_t argc, const void* args) { return 0; }
