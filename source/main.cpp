#include "natives.h"

// Definição manual de tipos para evitar erros de compilação no Ubuntu/PS4
typedef unsigned long size_t;
#define NULL 0

// Variáveis de Controle do Mod
bool assaltoAtivo = false;
Vector3 localCrime = {0.0f, 0.0f, 0.0f, 0.0f};
Ped meuGuarda = 0;
int tempoGuarda = 0;

// Sistema de espera (Yield)
void Aguardar(int ms) { WAIT(ms); }

void GerenciarGuarda() {
    Player p = PLAYER::PLAYER_ID(); 
    Ped pP = PLAYER::PLAYER_PED_ID(); 
    Vector3 pos = ENTITY::GET_ENTITY_COORDS(pP, true);

    // Atalho: Segurar L3 + R3 (ou botões equivalentes)
    if (PAD::IS_CONTROL_PRESSED(0, 0xDB60C503) && PAD::IS_CONTROL_PRESSED(0, 0x3C0A40F2)) {
        tempoGuarda++;
        if (tempoGuarda >= 300) { 
            Hash sel = 0x106561CA; // John Marston
            STREAMING::REQUEST_MODEL(sel, false);
            while (!STREAMING::HAS_MODEL_LOADED(sel)) { Aguardar(10); }
            
            if (ENTITY::DOES_ENTITY_EXIST(meuGuarda)) ENTITY::DELETE_ENTITY(&meuGuarda);
            meuGuarda = PED::CREATE_PED(sel, pos.x + 2.0f, pos.y, pos.z, 0.0f, false, false, false, false);
            PED::SET_PED_AS_GROUP_MEMBER(meuGuarda, PLAYER::GET_PLAYER_GROUP(p));
            PED::SET_PED_CONFIG_FLAG(meuGuarda, 279, true); // Defender jogador
            
            HUD::_DISPLAY_TEXT("GANGUE CHEGOU!", 0.5f, 0.85f);
            tempoGuarda = 0;
        }
    } else tempoGuarda = 0;
}

void ProcessarAssalto() {
    Player p = PLAYER::PLAYER_ID(); 
    Ped pP = PLAYER::PLAYER_PED_ID(); 
    Vector3 pos = ENTITY::GET_ENTITY_COORDS(pP, true);

    // Texto de Status no Rodapé
    HUD::_SET_TEXT_COLOR(255, 255, 255, 255);
    HUD::_SET_TEXT_SCALE(0.30f, 0.30f);
    HUD::_DISPLAY_TEXT("ASSALTO PRO 1.32 ON", 0.5f, 0.94f);
    
    GerenciarGuarda();

    // 1. Abertura de Portas ao Mirar no Caixa (Raio 50m)
    Entity alvo;
    if (!assaltoAtivo && PLAYER::GET_ENTITY_PLAYER_IS_FREE_AIMING_AT(p, &alvo)) {
        Hash m = ENTITY::GET_ENTITY_MODEL(alvo);
        if (m == 0x2C047466 || m == 0x959E7979) { 
            assaltoAtivo = true;
            localCrime = pos;
            PLAYER::SET_PLAYER_WANTED_LEVEL(p, 4, false);
            
            // Hashes das Portas de Valentine, Rhodes e Saint Denis
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

    // 2. Explodir e Saquear Cofres (Botão Triângulo/E próximo ao cofre)
    Hash cofres[] = { 0x1873C856, 0x5395A642, 0x0E63B489 };
    for(int i=0; i<3; i++) {
        Object cf = OBJECT::GET_CLOSEST_OBJECT_OF_TYPE(pos.x, pos.y, pos.z, 3.5f, cofres[i], false, false, true);
        if (ENTITY::DOES_ENTITY_EXIST(cf) && PAD::IS_CONTROL_JUST_PRESSED(0, 0x07CEABE4)) {
            FIRE::ADD_EXPLOSION(pos.x, pos.y, pos.z, 2, 1.0f, true, false, 1.0f);
            ENTITY::DELETE_ENTITY(&cf);
            CASH::MONEY_ADD_CASH(MISC::GET_RANDOM_INT_IN_RANGE(15000, 350000));
            HUD::_DISPLAY_TEXT("SAQUE CONCLUIDO!", 0.5f, 0.85f);
        }
    }

    // Resetar estado se fugir do local
    if (assaltoAtivo && MISC::GET_DISTANCE_BETWEEN_COORDS(pos.x, pos.y, pos.z, localCrime.x, localCrime.y, localCrime.z, true) > 150.0f) {
        assaltoAtivo = false;
    }
}

// --- ENTRY POINTS PARA GOLDHEN ---
extern "C" int module_start(size_t argc, const void* args) {
    // Loop principal (o WAIT(0) evita o crash no PS4)
    while (true) {
        ProcessarAssalto();
        Aguardar(0); 
    }
    return 0;
}

extern "C" int module_stop(size_t argc, const void* args) {
    return 0;
}
