#pragma once

#include <vector>
#include "Door.hpp"

class Entity;
class Player;
class Enemy;
class Monster;

class Room {
public:
    void Load(std::string _path);
    void Draw();
    void Update();
    char GetLocation(Vec2 _pos);
    void ClearLocation(Vec2 _pos);
    void OpenDoor(Vec2 _pos);
    void BeginCombat(Vec2 _pos);
private:
    std::vector<Entity*> m_entities;
    Player* m_player = nullptr;
    Enemy* m_enemy = nullptr;
    std::vector<Monster*> m_monsters;
    std::vector<std::vector<char>> m_map;
    std::vector<Door> m_doors;
};