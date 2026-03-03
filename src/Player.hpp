#pragma once

#include "Entity.hpp"

class Player : public Entity
{
public:
    void Start(Vec2 _pos);
    void Update();
private:
    int m_keyCount = 0;
    int m_health = 20;
    int m_goldAmount = 0;
    char m_weapon;
};