#include "natives.h"

// Definições de sistema para PS4
typedef unsigned long size_t;
#define NULL 0

// Variáveis de controle
bool assaltoAtivo = false;
Ped meuGuarda = 0;
int tempoGuarda = 0;

void Aguardar(int ms) { WAIT(ms); }

void ProcessarAssalto() {
    Player p = PLAYER::PLAYER_ID(); 
    Ped pP = PLAYER::PLAYER_PED_ID(); 
    Vector3 pos = ENTITY::GET_ENTITY_COORDS(pP, true);

    // 1. SUPORTE DA GANGUE (L1 + D-Pad Cima)
    // Segure por 2 segundos para spawnar o John Marston
    if (PAD::IS_CONTROL_PRESSED(0, 0xF7D06352) && PAD::IS_CONTROL_PRESSED(0, 0x911CB91D)) {
        tempoGuarda++;
        if (tempoGuarda >= 200) {
            Hash sel = 0x106561CA; // John Marston
            STREAMING::REQUEST_MODEL(sel, false);
            if (STREAMING::HAS_MODEL_LOADED(sel)) {
                if (ENTITY::DOES_ENTITY_EXIST(meuGuarda)) ENTITY::DELETE_ENTITY(&meuGuarda);
                meuGuarda = PED::CREATE_PED(sel, pos.x + 2.0f, pos.y, pos.z, 0.0f, false, false, false, false);
                PED::SET_PED_AS_GROUP_MEMBER(meuGuarda, PLAYER::GET_PLAYER_GROUP(p));
                tempoGuarda = 0;
            }
        }
    } else tempoGuarda = 0;

    // 2. ABERTURA DE PORTAS (Ao Mirar)
    // Se você estiver mirando com arma, as portas dos cofres abrem em 30m
    if (PLAYER::GET_ENTITY_PLAYER_IS_FREE_AIMING_AT(p, NULL)) {
        Hash portas[] = { 0x7C030E57, 0x5D35D3D1, 0x242D7D7D, 0x4894379D, 0x5395A642 };
        for(int i = 0; i < 5; i++) {
            Object portaObj = OBJECT::GET_CLOSEST_OBJECT_OF_TYPE(pos.x, pos.y, pos.z, 30.0f, portas[i], false, false, true);
            if (ENTITY::DOES_ENTITY_EXIST(portaObj)) {
                OBJECT::SET_DOOR_STATE(portas[i], pos.x, pos.y, pos.z, 3, 0.0f);
            }
        }
    }

    // 3. ROUBO DE COFRES (Pressionar TRIÂNGULO perto do cofre)
    Hash cofres[] = { 0x1873C856, 0x5395A642, 0x0E63B489 };
    for(int i = 0; i < 3; i++) {
        Object cf = OBJECT::GET_CLOSEST_OBJECT_OF_TYPE(pos.x, pos.y, pos.z, 3.0f, cofres[i], false, false, true);
        if (ENTITY::DOES_ENTITY_EXIST(cf) && PAD::IS_CONTROL_JUST_PRESSED(0, 0x07CEABE4)) {
            FIRE::ADD_EXPLOSION(pos.x, pos.y, pos.z, 2, 1.0f, true, false, 1.0f);
            CASH::MONEY_ADD_CASH(150000); // Adiciona $1500.00
            ENTITY::DELETE_ENTITY(&cf);
        }
    }
}

// Entry Point Exportado para o GoldHEN
extern "C" {
    __attribute__((visibility("default"))) int _main(size_t argc, const void* args) {
        // Aguarda 30 segundos para o Havana carregar
        for(int i = 0; i < 300; i++) { Aguardar(100); }

        while (true) {
            ProcessarAssalto();
            Aguardar(0);
        }
        return 0;
    }
    
    // Nomes padrão que o GoldHEN busca
    __attribute__((visibility("default"))) int module_start(size_t argc, const void* args) { return _main(argc, args); }
    __attribute__((visibility("default"))) int module_stop(size_t argc, const void* args) { return 0; }
}
