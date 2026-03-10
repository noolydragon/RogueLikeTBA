#pragma once

#include "Entity.hpp"
#include "Dice.hpp"

class Player : public Entity
{
public:
    void Start(Vec2 _pos);
    void Update();
    bool IsAlive() { return m_Phealth > 0; }
    int GetHP() { return m_Phealth; }
    int GetXP() { return m_currentXP; }
    int GetGold() { return m_goldAmount; }
    int PrintStats();
    void MoveToStart(Vec2 _pos);
    void TakeDamage(int _amount) { m_Phealth -= _amount; }
    void AddGold(int _amount) { m_goldAmount += _amount; } 
    void SetXP(int _amount);
    void AddHP(int _amount);
private:
    int m_keyCount = 0;
    int m_Phealth = 20;
    int m_maxHP = 20;
    int m_currentLvl = 1;
    int m_currentXP = 0;
    int m_requiredXP = 20;
    int m_goldAmount = 0;
    char m_weapon;
};