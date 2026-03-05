#include "Enemy.hpp"
#include "Player.hpp"
#include "Room.hpp"
#include "fogpi/io.hpp"

void Enemy::Start(Vec2 _pos) {
    m_character = 'E';
    m_position = _pos;
}

void Enemy::Update() {
    
}