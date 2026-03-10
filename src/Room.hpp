#pragma once

#include <vector>
#include "Door.hpp"

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
private:
    std::vector<Entity*> m_entities;
    Player* m_player = nullptr;
    std::vector<std::vector<char>> m_map;
    std::vector<Door> m_doors;
    int m_roomcount = 0;
};