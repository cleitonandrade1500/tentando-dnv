#ifndef NATIVES_H
#define NATIVES_H

// Definições de tipos básicas para RDR2 PS4
typedef int Player;
typedef int Entity;
typedef int Ped;
typedef int Object;
typedef unsigned int Hash;

// 1. Natives de Jogador e Mira
namespace PLAYER {
    extern Player PLAYER_ID();
    extern Ped PLAYER_PED_ID();
    extern bool GET_ENTITY_PLAYER_IS_FREE_AIMING_AT(Player player, Entity* entity);
}

// 2. Natives de Entidades (Banqueiros)
namespace ENTITY {
    extern bool IS_ENTITY_A_PED(Entity entity);
    extern bool IS_ENTITY_DEAD(Entity entity);
    extern Hash GET_ENTITY_MODEL(Entity entity);
    extern void SET_ENTITY_AS_NO_LONGER_NEEDED(Entity* entity);
    extern struct Vector3 GET_ENTITY_COORDS(Entity entity, bool alive);
}

// 3. Natives de Ações (Render-se)
namespace TASK {
    extern void TASK_HANDS_UP(Ped ped, int duration, Ped facingPed, int p3, bool p4);
}

// 4. Natives de Objetos (Cofres)
namespace OBJECT {
    extern Object GET_CLOSEST_OBJECT_OF_TYPE(float x, float y, float z, float radius, Hash modelHash, bool isMission, bool p6, bool p7);
    extern void DOOR_CONTROL(Hash doorHash, float x, float y, float z, bool locked, float xRot, float yRot, float zRot);
}

// 5. Natives de Dinheiro e Utilidades
namespace CASH {
    extern void MONEY_ADD_CASH(int amount); // Adiciona dinheiro direto
}

namespace MISC {
    extern Hash GET_HASH_KEY(const char* string);
}

// Função de sincronização do PS4
extern "C" void WAIT(int ms);

// Estrutura de coordenadas
struct Vector3 {
    float x, y, z;
};

#endif
