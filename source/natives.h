#ifndef NATIVES_H
#define NATIVES_H

#include <stdint.h>

// Definições de Tipos Base do RDR2
typedef int Player; 
typedef int Entity; 
typedef int Ped; 
typedef int Object; 
typedef int Blip; 
typedef unsigned int Hash;

// Struct com padding de 16 bytes para compatibilidade PS4
struct Vector3 { 
    float x, y, z; 
    float padding; 
};

// Funções do Jogo (Devem ser resolvidas pelo Linker/Invoker do GoldHEN)
namespace PLAYER {
    extern "C" Player PLAYER_ID(); 
    extern "C" Ped PLAYER_PED_ID(); 
    extern "C" int GET_PLAYER_GROUP(Player player);
    extern "C" bool GET_ENTITY_PLAYER_IS_FREE_AIMING_AT(Player player, Entity* entity);
    extern "C" void SET_PLAYER_WANTED_LEVEL(Player player, int level, bool p2);
    extern "C" void SET_PLAYER_IGNORE_LOW_HONOR_COMPLAINTS(Player player, bool toggle);
    extern "C" bool IS_PLAYER_IN_CUTSCENE(Player player);
}

namespace ENTITY {
    extern "C" Hash GET_ENTITY_MODEL(Entity entity); 
    extern "C" Vector3 GET_ENTITY_COORDS(Entity entity, bool alive);
    extern "C" bool DOES_ENTITY_EXIST(Entity entity); 
    extern "C" void DELETE_ENTITY(Entity* entity);
    extern "C" void SET_ENTITY_HEALTH(Entity entity, int health, int p2);
    extern "C" void SET_ENTITY_AS_MISSION_ENTITY(Entity entity, bool p1, bool p2);
}

namespace PED {
    extern "C" Ped CREATE_PED(Hash model, float x, float y, float z, float heading, bool p5, bool p6, bool p7, bool p8);
    extern "C" bool IS_PED_WEARING_ANY_MASK(Ped ped); 
    extern "C" void SET_PED_AS_GROUP_MEMBER(Ped ped, int groupID);
    extern "C" void SET_PED_CONFIG_FLAG(Ped ped, int flag, bool value);
}

namespace TASK {
    extern "C" void TASK_HANDS_UP(Ped ped, int duration, Ped facingPed, int p3, bool p4);
    extern "C" void TASK_COMBAT_PED(Ped ped, Ped target, int p2, int p3);
}

namespace STREAMING {
    extern "C" void REQUEST_MODEL(Hash model, bool p1); 
    extern "C" bool HAS_MODEL_LOADED(Hash model);
    extern "C" void SET_MODEL_AS_NO_LONGER_NEEDED(Hash model);
}

namespace OBJECT {
    extern "C" Object GET_CLOSEST_OBJECT_OF_TYPE(float x, float y, float z, float radius, Hash model, bool p5, bool p6, bool p7);
    extern "C" void SET_DOOR_STATE(Hash doorModel, float x, float y, float z, int state, float angle);
}

namespace HUD {
    extern "C" void _DISPLAY_TEXT(const char* text, float x, float y);
    extern "C" void _SET_TEXT_COLOR(int r, int g, int b, int a);
    extern "C" void _SET_TEXT_SCALE(float scale, float size);
}

namespace MISC { 
    extern "C" Hash GET_HASH_KEY(const char* str); 
    extern "C" bool GET_MISSION_FLAG();
    extern "C" int GET_RANDOM_INT_IN_RANGE(int min, int max);
    extern "C" float GET_DISTANCE_BETWEEN_COORDS(float x1, float y1, float z1, float x2, float y2, float z2, bool useZ);
}

namespace PAD { 
    extern "C" bool IS_CONTROL_PRESSED(int p0, Hash control); 
    extern "C" bool IS_CONTROL_JUST_PRESSED(int p0, Hash control); 
}

namespace FIRE { 
    extern "C" void ADD_EXPLOSION(float x, float y, float z, int type, float damage, bool audible, bool invisible, float shake); 
}

namespace CASH { 
    extern "C" void MONEY_ADD_CASH(int amount); 
}

namespace WEAPON { 
    extern "C" void GIVE_WEAPON_TO_PED(Ped ped, Hash weaponHash, int ammoCount, bool p3, bool p4); 
}

// Definição global de espera (Yield) do sistema
extern "C" void WAIT(int ms);

#endif
