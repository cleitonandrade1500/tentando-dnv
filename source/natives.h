#ifndef NATIVES_H
#define NATIVES_H

typedef int Player; 
typedef int Entity; 
typedef int Ped; 
typedef int Object; 
typedef unsigned int Hash;

struct Vector3 { 
    float x, y, z; 
    float padding; 
};

namespace PLAYER {
    extern "C" Player PLAYER_ID(); 
    extern "C" Ped PLAYER_PED_ID(); 
    extern "C" void SET_PLAYER_WANTED_LEVEL(Player player, int level, bool p2);
    extern "C" void SET_PLAYER_IGNORE_LOW_HONOR_COMPLAINTS(Player player, bool toggle);
}

namespace ENTITY {
    extern "C" Vector3 GET_ENTITY_COORDS(Entity entity, bool alive);
    extern "C" bool DOES_ENTITY_EXIST(Entity entity); 
    extern "C" void DELETE_ENTITY(Entity* entity);
    extern "C" void SET_ENTITY_HEALTH(Entity entity, int health, int p2);
    extern "C" void SET_ENTITY_AS_MISSION_ENTITY(Entity entity, bool p1, bool p2);
    extern "C" Hash GET_ENTITY_MODEL(Entity entity);
}

namespace PED {
    extern "C" bool IS_PED_WEARING_ANY_MASK(Ped ped); 
    extern "C" void SET_PED_AS_GROUP_MEMBER(Ped ped, int groupID);
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

namespace PAD { 
    extern "C" bool IS_CONTROL_PRESSED(int p0, Hash control); 
}

extern "C" void WAIT(int ms);

#endif
