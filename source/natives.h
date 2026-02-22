#ifndef NATIVES_H
#define NATIVES_H

typedef int Player;
typedef int Entity;
typedef int Ped;
typedef int Object;
typedef int Blip;
typedef unsigned int Hash;

struct Vector3 { 
    float x, y, z; 
    float padding; // Necessário para o alinhamento de memória no PS4
};

namespace PLAYER {
    extern Player PLAYER_ID();
    extern Ped PLAYER_PED_ID();
    extern bool GET_ENTITY_PLAYER_IS_FREE_AIMING_AT(Player player, Entity* entity);
    extern void SET_PLAYER_WANTED_LEVEL(Player player, int level, bool p2);
    extern int GET_PLAYER_WANTED_LEVEL(Player player);
    extern void CLEAR_PLAYER_WANTED_LEVEL(Player player);
}

namespace ENTITY {
    extern Hash GET_ENTITY_MODEL(Entity entity);
    extern Vector3 GET_ENTITY_COORDS(Entity entity, bool alive);
    extern bool DOES_ENTITY_EXIST(Entity entity);
    extern void SET_ENTITY_AS_MISSION_ENTITY(Entity entity, bool p1, bool p2);
    extern void SET_ENTITY_AS_NO_LONGER_NEEDED(Entity* entity);
    extern void DELETE_ENTITY(Entity* entity); // Essencial para o cofre
    extern void SET_ENTITY_PROOFS(Entity entity, bool p1, bool p2, bool p3, bool p4, bool p5, bool p6, bool p7, bool p8);
}

namespace PED {
    extern Ped CREATE_PED(Hash model, float x, float y, float z, float heading, bool p5, bool p6, bool p7, bool p8);
    extern void SET_PED_RELATIONSHIP_GROUP_HASH(Ped ped, Hash group);
}

namespace STREAMING {
    extern void REQUEST_MODEL(Hash model, bool p1);
    extern bool HAS_MODEL_LOADED(Hash model);
    extern void SET_MODEL_AS_NO_LONGER_NEEDED(Hash model);
}

namespace TASK {
    extern void TASK_HANDS_UP(Ped ped, int duration, Ped facingPed, int p3, bool p4);
    extern void TASK_COMBAT_PED(Ped ped, Ped target, int p2, int p3);
}

namespace OBJECT {
    extern Object GET_CLOSEST_OBJECT_OF_TYPE(float x, float y, float z, float radius, Hash model, bool p5, bool p6, bool p7);
    // Substitui DOOR_CONTROL por SET_DOOR_STATE para funcionar no PS4
    extern void SET_DOOR_STATE(Hash doorModel, float x, float y, float z, int state, float angle);
}

namespace HUD {
    extern void SET_BLIP_SPRITE(Blip blip, Hash spriteHash);
    extern void SET_BLIP_COLOUR(Blip blip, int colour);
}

namespace MAP {
    extern Blip SET_BLIP_FOR_ENTITY(Entity entity);
}

namespace CASH { 
    extern void MONEY_ADD_CASH(int amount); 
}

namespace MISC { 
    extern Hash GET_HASH_KEY(const char* str); 
    extern float GET_DISTANCE_BETWEEN_COORDS(float x1, float y1, float z1, float x2, float y2, float z2, bool useZ);
}

namespace PAD { 
    extern bool IS_CONTROL_PRESSED(int p0, Hash control); 
    extern bool IS_CONTROL_JUST_PRESSED(int p0, Hash control); 
}

namespace FIRE { 
    extern void ADD_EXPLOSION(float x, float y, float z, int type, float damage, bool audible, bool invisible, float shake); 
}

extern "C" void WAIT(int ms);

#endif
