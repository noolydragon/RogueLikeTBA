#pragma once

#include "Entity.hpp"

class Enemies : public Entity
{
public:
    void Start(Vec2 _pos);
    void Update();
private:
    int m_health = 10;
};