#include "natives.h"

// Correção de tipos para o compilador Clang no PS4
typedef unsigned long size_t;
#define NULL 0

// Variáveis de Controle
bool assaltoAtivo = false;
Vector3 localCrime = {0.0f, 0.0f, 0.0f, 0.0f};
Ped meuGuarda = 0;
int tempoGuarda = 0;

// Sistema de Yield (Espera)
void Aguardar(int ms) { WAIT(ms); }

void GerenciarGuarda() {
    Player p = PLAYER::PLAYER_ID(); 
    Ped pP = PLAYER::PLAYER_PED_ID(); 
    Vector3 pos = ENTITY::GET_ENTITY_COORDS(pP, true);

    // NOVO ATALHO: Segurar L1 (0xF7D06352) + Direcional Cima (0x911CB91D)
    // Isso evita conflito com o R1 + Quadrado do Havana
    if (PAD::IS_CONTROL_PRESSED(0, 0xF7D06352) && PAD::IS_CONTROL_PRESSED(0, 0x911CB91D)) {
        tempoGuarda++;
        if (tempoGuarda >= 250) { // ~2.5 segundos
            Hash sel = 0x106561CA; // John Marston
            STREAMING::REQUEST_MODEL(sel, false);
            while (!STREAMING::HAS_MODEL_LOADED(sel)) { Aguardar(10); }
            
            if (ENTITY::DOES_ENTITY_EXIST(meuGuarda)) ENTITY::DELETE_ENTITY(&meuGuarda);
            meuGuarda = PED::CREATE_PED(sel, pos.x + 2.0f, pos.y, pos.z, 0.0f, false, false, false, false);
            PED::SET_PED_AS_GROUP_MEMBER(meuGuarda, PLAYER::GET_PLAYER_GROUP(p));
            PED::SET_PED_CONFIG_FLAG(meuGuarda, 279, true);
            
            HUD::_DISPLAY_TEXT("GANGUE CHEGOU!", 0.5f, 0.85f);
            tempoGuarda = 0;
        }
    } else tempoGuarda = 0;
}

void ProcessarAssalto() {
    Player p = PLAYER::PLAYER_ID(); 
    Ped pP = PLAYER::PLAYER_PED_ID(); 
    Vector3 pos = ENTITY::GET_ENTITY_COORDS(pP, true);

    // Texto de Status (Posicionado para não bater no menu do Havana)
    HUD::_SET_TEXT_COLOR(255, 255, 255, 200);
    HUD::_SET_TEXT_SCALE(0.28f, 0.28f);
    HUD::_DISPLAY_TEXT("ASSALTO PRO 1.32 ATIVO", 0.85f, 0.95f);
    
    GerenciarGuarda();

    // 1. Abertura Automática de Portas (Raio 50m)
    Entity alvo;
    if (!assaltoAtivo && PLAYER::GET_ENTITY_PLAYER_IS_FREE_AIMING_AT(p, &alvo)) {
        Hash m = ENTITY::GET_ENTITY_MODEL(alvo);
        if (m == 0x2C047466 || m == 0x959E7979) { 
            assaltoAtivo = true;
            localCrime = pos;
            PLAYER::SET_PLAYER_WANTED_LEVEL(p, 4, false);
            
            // Portas dos Bancos de Valentine, Rhodes e Saint Denis
            Hash pts[] = { 0x7C030E57, 0x5D35D3D1, 0x242D7D7D, 0x4894379D, 0x5395A642 };
            for(int i=0; i<5; i++) {
                Object d = OBJECT::GET_CLOSEST_OBJECT_OF_TYPE(pos.x, pos.y, pos.z, 50.0f, pts[i], false, false, true);
                if (ENTITY::DOES_ENTITY_EXIST(d)) {
                    OBJECT::SET_DOOR_STATE(pts[i], pos.x, pos.y, pos.z, 3, 0.0f);
                }
            }
            HUD::_DISPLAY_TEXT("BANCO INVADIDO!", 0.5f, 0.80f);
        }
    }

    // 2. Explodir Cofres e Roubar (Perto do cofre)
    Hash cofres[] = { 0x1873C856, 0x5395A642, 0x0E63B489 };
    for(int i=0; i<3; i++) {
        Object cf = OBJECT::GET_CLOSEST_OBJECT_OF_TYPE(pos.x, pos.y, pos.z, 3.5f, cofres[i], false, false, true);
        if (ENTITY::DOES_ENTITY_EXIST(cf) && PAD::IS_CONTROL_JUST_PRESSED(0, 0x07CEABE4)) {
            FIRE::ADD_EXPLOSION(pos.x, pos.y, pos.z, 2, 1.0f, true, false, 1.0f);
            ENTITY::DELETE_ENTITY(&cf);
            CASH::MONEY_ADD_CASH(MISC::GET_RANDOM_INT_IN_RANGE(20000, 400000));
            HUD::_DISPLAY_TEXT("SAQUE CONCLUIDO!", 0.5f, 0.85f);
        }
    }

    if (assaltoAtivo && MISC::GET_DISTANCE_BETWEEN_COORDS(pos.x, pos.y, pos.z, localCrime.x, localCrime.y, localCrime.z, true) > 150.0f) {
        assaltoAtivo = false;
    }
}

// --- ENTRY POINT COMPATÍVEL COM HAVANA & GOLDHEN 2.4b ---
extern "C" int module_start(size_t argc, const void* args) {
    // ESPERA DE SEGURANÇA (40 segundos)
    // Permite que o Havana se instale primeiro para evitar Kernel Panic
    for(int i = 0; i < 400; i++) {
        Aguardar(100); 
    }

    while (true) {
        // Só roda se o jogador não estiver em tela de carregamento
        if (!PLAYER::IS_PLAYER_IN_CUTSCENE(PLAYER::PLAYER_ID())) {
            ProcessarAssalto();
        }
        Aguardar(0); 
    }
    return 0;
}

extern "C" int module_stop(size_t argc, const void* args) {
    return 0;
}
