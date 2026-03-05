#pragma once

#include "Entity.hpp"

class Enemy : public Entity
{
public:
    void Start(Vec2 _pos);
    void Update();
    int GetHP() { return m_EHealth; }
    void TakeDamage(int _amount) { m_EHealth -= _amount; }
private:
    int m_EHealth = 10;
    int m_EStrength = 5;
    int m_ESpeed = 1;
};