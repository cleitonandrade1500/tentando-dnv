#ifndef NATIVES_H
#define NATIVES_H

typedef int Player;
typedef int Entity;
typedef int Ped;
typedef int Object;
typedef unsigned int Hash;

struct Vector3 { float x, y, z; };

namespace PLAYER {
    extern Player PLAYER_ID();
    extern Ped PLAYER_PED_ID();
    extern bool GET_ENTITY_PLAYER_IS_FREE_AIMING_AT(Player player, Entity* entity);
    extern void CLEAR_PLAYER_WANTED_LEVEL(Player player);
}

namespace ENTITY {
    extern bool IS_ENTITY_A_PED(Entity entity);
    extern bool IS_ENTITY_DEAD(Entity entity);
    extern Hash GET_ENTITY_MODEL(Entity entity);
    extern Vector3 GET_ENTITY_COORDS(Entity entity, bool alive);
    extern bool DOES_ENTITY_EXIST(Entity entity);
    extern void SET_ENTITY_AS_MISSION_ENTITY(Entity entity, bool p1, bool p2);
    extern void SET_ENTITY_AS_NO_LONGER_NEEDED(Entity* entity);
}

namespace TASK {
    extern void TASK_HANDS_UP(Ped ped, int duration, Ped facingPed, int p3, bool p4);
}

namespace OBJECT {
    extern Object GET_CLOSEST_OBJECT_OF_TYPE(float x, float y, float z, float radius, Hash modelHash, bool isMission, bool p6, bool p7);
    extern void DOOR_CONTROL(Hash doorHash, float x, float y, float z, bool locked, float xRot, float yRot, float zRot);
}

namespace CASH {
    extern void MONEY_ADD_CASH(int amount);
}

namespace MISC {
    extern Hash GET_HASH_KEY(const char* string);
}

namespace PAD {
    extern bool IS_CONTROL_PRESSED(int padIndex, Hash control);
    extern bool IS_CONTROL_JUST_PRESSED(int padIndex, Hash control);
}

namespace FIRE {
    extern void ADD_EXPLOSION(float x, float y, float z, int explosionType, float damageScale, bool isAudible, bool isInvisible, float cameraShake);
}

extern "C" void WAIT(int ms);

#endif
