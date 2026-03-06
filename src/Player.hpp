#pragma once

#include "Entity.hpp"

class Player : public Entity
{
public:
    void Start(Vec2 _pos);
    void Update();
    int GetHP() { return m_Phealth; }
    int GetXP() { return m_Pexperience; }
    int GetGold() { return m_goldAmount; }
    void TakeDamage(int _amount) { m_Phealth -= _amount; }
    void AddGold(int _amount) { m_goldAmount += _amount; } 
    void SetXP(int _amount) { m_Pexperience += _amount; }
private:
    int m_keyCount = 0;
    int m_Phealth = 20;
    int m_Pexperience = 0;
    int m_health = 20;
    int m_goldAmount = 0;
    char m_weapon;
};