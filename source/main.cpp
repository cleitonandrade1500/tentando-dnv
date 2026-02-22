#include "natives.h"

// --- VARIÁVEIS GLOBAIS ---
bool assaltoAtivo = false;
Vector3 localCrime = {0.0f, 0.0f, 0.0f, 0.0f};

// --- FUNÇÕES DE AUXÍLIO ---

void ExibirStatusHUD() {
    // Texto fixo na parte inferior da tela
    HUD::_SET_TEXT_COLOR(255, 255, 255, 255); // Branco
    HUD::_SET_TEXT_SCALE(0.35f, 0.35f);
    // x=0.5 (centro), y=0.90 (baixo)
    HUD::_DISPLAY_TEXT("ROUBAR ON", 0.5f, 0.90f);
}

void GerenciarMascaraEHonra() {
    Player player = PLAYER::PLAYER_ID();
    Ped pPed = PLAYER::PLAYER_PED_ID();
    
    // Se estiver de máscara, o jogo ignora perda de honra por crimes menores
    if (PED::IS_PED_WEARING_ANY_MASK(pPed)) {
        PLAYER::SET_PLAYER_IGNORE_LOW_HONOR_COMPLAINTS(player, true);
    } else {
        PLAYER::SET_PLAYER_IGNORE_LOW_HONOR_COMPLAINTS(player, false);
    }
}

void SpawnPoliciaPS4(Vector3 pos) {
    Hash copHash = MISC::GET_HASH_KEY("S_M_M_VALCOP_01");
    STREAMING::REQUEST_MODEL(copHash, false);
    
    int timeout = 0;
    while (!STREAMING::HAS_MODEL_LOADED(copHash) && timeout < 50) { 
        WAIT(10); 
        timeout++; 
    }

    if (STREAMING::HAS_MODEL_LOADED(copHash)) {
        for(int i = 0; i < 3; i++) {
            Ped cop = PED::CREATE_PED(copHash, pos.x + (i * 2), pos.y + 8, pos.z, 0.0f, false, false, false, false);
            PED::SET_PED_RELATIONSHIP_GROUP_HASH(cop, MISC::GET_HASH_KEY("REL_COP"));
            TASK::TASK_COMBAT_PED(cop, PLAYER::PLAYER_PED_ID(), 0, 16);
            
            Blip b = MAP::SET_BLIP_FOR_ENTITY(cop);
            HUD::SET_BLIP_SPRITE(b, MISC::GET_HASH_KEY("blip_ambient_law"));
            HUD::SET_BLIP_COLOUR(b, 1); // Vermelho
        }
        STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(copHash);
    }
}

void ModAssaltoSincronizado() {
    Player player = PLAYER::PLAYER_ID();
    Ped pPed = PLAYER::PLAYER_PED_ID();
    Vector3 pPos = ENTITY::GET_ENTITY_COORDS(pPed, true);

    // Desenha o texto "ROUBAR ON" e checa a máscara todo frame
    ExibirStatusHUD();
    GerenciarMascaraEHonra();

    Entity alvo;
    // 1. MIRAR NO BANQUEIRO
    if (!assaltoAtivo && PLAYER::GET_ENTITY_PLAYER_IS_FREE_AIMING_AT(player, &alvo)) {
        Hash m = ENTITY::GET_ENTITY_MODEL(alvo);
        // Banqueiros: Valentine (0x2C047466), Rhodes (0x959E7979), St Denis (0x19273D6B)
        if (m == 0x2C047466 || m == 0x959E7979 || m == 0x19273D6B) {
            assaltoAtivo = true;
            TASK::TASK_HANDS_UP(alvo, -1, pPed, -1, false);

            // Hashes das Grades/Portas dos Bancos
            Hash portas[] = { 
                MISC::GET_HASH_KEY("p_door_vault01x"), 
                MISC::GET_HASH_KEY("p_rhodes_bank_door_01x"), 
                0x242D7D7D // Grade genérica
            }; 

            for(int i = 0; i < 3; i++) {
                Object d = OBJECT::GET_CLOSEST_OBJECT_OF_TYPE(pPos.x, pPos.y, pPos.z, 20.0f, portas[i], false, false, true);
                if (ENTITY::DOES_ENTITY_EXIST(d)) {
                    Vector3 dPos = ENTITY::GET_ENTITY_COORDS(d, true);
                    // Estado 3 abre a porta no PS4
                    OBJECT::SET_DOOR_STATE(portas[i], dPos.x, dPos.y, dPos.z, 3, 0.0f);
                    ENTITY::SET_ENTITY_AS_MISSION_ENTITY(d, true, true);
                }
            }
            PLAYER::SET_PLAYER_WANTED_LEVEL(player, 4, false);
            localCrime = pPos;
            SpawnPoliciaPS4(pPos);
        }
    }

    // 2. EXPLODIR COFRE (Perto do cofre + R2/Ataque)
    Hash hCofre = MISC::GET_HASH_KEY("p_bank_safe_01");
    Object cofre = OBJECT::GET_CLOSEST_OBJECT_OF_TYPE(pPos.x, pPos.y, pPos.z, 3.5f, hCofre, false, false, true);
    
    if (ENTITY::DOES_ENTITY_EXIST(cofre)) {
        if (PAD::IS_CONTROL_JUST_PRESSED(0, 0x07CEABE4)) { // Botão R2 no PS4
            Vector3 sPos = ENTITY::GET_ENTITY_COORDS(cofre, true);
            FIRE::ADD_EXPLOSION(sPos.x, sPos.y, sPos.z, 2, 1.0f, true, false, 1.0f);
            
            // Simula destruição removendo o objeto físico
            ENTITY::SET_ENTITY_AS_MISSION_ENTITY(cofre, true, true);
            ENTITY::DELETE_ENTITY(&cofre); 
            CASH::MONEY_ADD_CASH(150000); // R$ 1.500,00 (em centavos)
        }
    }

    // 3. RESETAR AO FUGIR
    if (assaltoAtivo) {
        float dist = MISC::GET_DISTANCE_BETWEEN_COORDS(pPos.x, pPos.y, pPos.z, localCrime.x, localCrime.y, localCrime.z, true);
        if (dist > 120.0f) {
            assaltoAtivo = false;
        }
    }
}

// LOOP PRINCIPAL GOLDHEN
extern "C" int _start() { 
    while (true) { 
        ModAssaltoSincronizado(); 
        WAIT(0); // WAIT(0) é essencial para o texto HUD não piscar
    } 
    return 0; 
}
