#pragma once

#include "Entity.hpp"

class Player : public Entity
{
public:
    void Start(Vec2 _pos);
    void Update();
    int GetHP() { return m_Pshealth; }
    void TakeDamage(int _amount) { m_Pshealth -= _amount; }
private:
    int m_keyCount = 0;
    int m_Pshealth = 20;
    int m_health = 20;
    int m_goldAmount = 0;
    char m_weapon;
};