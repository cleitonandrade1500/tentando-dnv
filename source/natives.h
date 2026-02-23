#ifndef NATIVES_H
#define NATIVES_H

// Tipos básicos para evitar dependências externas
typedef int Player; 
typedef int Entity; 
typedef int Ped; 
typedef int Object; 
typedef unsigned int Hash;
typedef unsigned long uint64_t;

struct Vector3 { float x, y, z; float padding; };

// Funções de comunicação com o PS4/GoldHEN
extern "C" {
    void nativeInit(uint64_t hash);
    void nativePush(uint64_t val);
    uint64_t* nativeCall();
}

// Template para funções que RETORNAM valor (Ex: Player ID)
template<typename T, typename... Args>
static T Invoke(uint64_t hash, Args... args) {
    nativeInit(hash);
    uint64_t vars[] = { (uint64_t)args... };
    for (int i = 0; i < sizeof...(args); i++) nativePush(vars[i]);
    uint64_t* res = nativeCall();
    return *(T*)res;
}

// Template para funções VOID (Ex: Dinheiro, Nível de Procurado)
// Isso resolve o erro de "indirection not permitted"
template<typename... Args>
static void InvokeVoid(uint64_t hash, Args... args) {
    nativeInit(hash);
    uint64_t vars[] = { (uint64_t)args... };
    for (int i = 0; i < sizeof...(args); i++) nativePush(vars[i]);
    nativeCall();
}

namespace PLAYER {
    inline Player PLAYER_ID() { return Invoke<Player>(0xEC6A0600); }
    inline Ped PLAYER_PED_ID() { return Invoke<Ped>(0x09697976); }
    inline void SET_PLAYER_WANTED_LEVEL(Player p, int l, bool p2) { InvokeVoid(0xB7A0914B, p, l, p2); }
}

namespace ENTITY {
    inline Vector3 GET_ENTITY_COORDS(Entity e, bool a) { return Invoke<Vector3>(0xA86D3333, e, a); }
    inline bool DOES_ENTITY_EXIST(Entity e) { return Invoke<bool>(0x367F4B72, e); }
    inline void DELETE_ENTITY(Entity* e) { InvokeVoid(0x37648392, e); }
}

namespace CASH {
    inline void MONEY_ADD_CASH(int amount) { InvokeVoid(0xBC235322, amount); }
}

extern "C" void WAIT(int ms);
#endif
