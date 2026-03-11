#pragma once

#include "Entity.hpp"
#include "Room.hpp"
#include "Player.hpp"

class Enemy : public Entity
{
public:
    virtual void Start(Vec2 _pos, int _health) { m_position = _pos; }
    virtual void Update() = 0;
    virtual int GetHP() { return m_EHealth; }
    virtual int GetXP() { return m_XPworth; }
    virtual int GetGold() { return m_EGold; }
    virtual void TakeDamage(int _amount) = 0;
    virtual char Draw() = 0;
protected:
    int m_EHealth;
    int m_EGold;
    int m_XPworth;
};

                //Public enemy number one lol
class Goblin : public Enemy {
public:
    void Start(Vec2 _pos, int _health) override;
    void Update() override;
    char Draw() override;
    int GetGold() override;
    int GetXP() override;
    void TakeDamage(int _amount) override;
};

class Sloblin : public Enemy {
public:
    void Start(Vec2 _pos, int _health) override;
    void Update() override;
    char Draw() override;
    int GetGold() override;
    int GetXP() override;
    void TakeDamage(int _amount)override;
};

class Ogre : public Enemy {
public:
    void Start(Vec2 _pos, int _health) override;
    void Update() override;
    char Draw() override;
    int GetGold() override;
    int GetXP() override;
    void TakeDamage(int _amount) override;
};
