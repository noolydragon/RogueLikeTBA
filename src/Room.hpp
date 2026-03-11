#pragma once

#include <vector>
#include "Door.hpp"
#include "Dice.hpp"

class Entity;
class Player;
class Enemy;
class Room {
public:
    Player* GetPlayer();
    void Load(std::string _path);
    void Draw();
    void Update();
    bool IsRoomClear();
    char GetLocation(Vec2 _pos);
    void ClearLocation(Vec2 _pos);
    void OpenDoor(Vec2 _pos);
    void BeginCombat(Vec2 _pos);
    void Trap(Vec2 _pos);
    void Chest(Vec2 _pos);
    void Accesion(Vec2 _pos);
private:
    std::vector<Entity*> m_entities;
    Player* m_player = nullptr;
    std::vector<std::vector<char>> m_map;
    std::vector<Door> m_doors;
    int m_roomcount = 0;
    int m_weapon;
    std::vector<Die> m_die = { {2}};
    std::vector<Die> s_die = { {6}};
    std::vector<Die> g_die = { {20}};
    std::vector<Die> G_die = { {100}};
    std::vector<Die> r_die = { {8}};
    std::vector<Die> d_die = { {4}};
    std::vector<Die> l_die = { {12}};
    RollStats playerRoll;
};