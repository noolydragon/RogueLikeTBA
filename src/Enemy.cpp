#include "Enemy.hpp"
#include "Player.hpp"
#include "Room.hpp"
#include "fogpi/io.hpp"

void Goblin::Start(Vec2 _pos, int _health) {
    m_position = _pos;
    m_character = 'G';
    m_EHealth = _health;
    m_EGold = 5;
    m_XPworth = 5;
}

void Goblin::Update() {
    if (room == nullptr || room->GetPlayer() == nullptr) return;

    Vec2 playerPos = room->GetPlayer()->GetPosition();
    Vec2 nextPos = m_position;

    if (playerPos.x > m_position.x)     nextPos.x++;
    else if (playerPos.x < m_position.x)nextPos.x--;

    if (playerPos.y > m_position.y)     nextPos.y++;
    else if (playerPos.y < m_position.y)nextPos.y--;

    if (room->GetLocation(nextPos) == ' ') {
        m_position = nextPos;
    }
    else if (room->GetLocation(nextPos) == 'P') {
        room->BeginCombat(m_position);
    }
}

char Goblin::Draw() { return 'G'; }
int Goblin::GetGold() { return m_EGold; }
int Goblin::GetXP() { return m_XPworth; }
void Goblin::TakeDamage(int _amount) { m_EHealth -= _amount; }

void Sloblin::Start(Vec2 _pos, int _health) {
    m_position = _pos;
    m_character = 'M';
    m_EHealth = _health;
    m_EGold = 15;
    m_XPworth = 15;
}

void Sloblin::Update() {
    if (rand() % 2 == 0) return;

    if (room == nullptr || room->GetPlayer() == nullptr) return;

    Vec2 playerPos = room->GetPlayer()->GetPosition();
    Vec2 nextPos = m_position;

    if (playerPos.x > m_position.x)     nextPos.x++;
    else if (playerPos.x < m_position.x)nextPos.x--;

    if (playerPos.y > m_position.y)     nextPos.y++;
    else if (playerPos.y < m_position.y)nextPos.y--;

    if (room->GetLocation(nextPos) == ' ') {
        m_position = nextPos;
    }
    else if (room->GetLocation(nextPos) == 'P') {
        room->BeginCombat(m_position);
    }
}

char Sloblin::Draw() { return 'M'; }
int Sloblin::GetGold() { return m_EGold; }
int Sloblin::GetXP() { return m_XPworth; }
void Sloblin::TakeDamage(int _amount) { m_EHealth -= _amount; }

void Ogre::Start(Vec2 _pos, int _health) {
    m_position = _pos;
    m_character = 'O';
    m_EHealth = _health;
    m_EGold = 25;
    m_XPworth = 25;
}

void Ogre::Update() {
    if (rand() % 2 == 0) return;

    if (room == nullptr || room->GetPlayer() == nullptr) return;

    Vec2 playerPos = room->GetPlayer()->GetPosition();
    Vec2 nextPos = m_position;

    if (playerPos.x > m_position.x)     nextPos.x++;
    else if (playerPos.x < m_position.x)nextPos.x--;

    if (playerPos.y > m_position.y)     nextPos.y++;
    else if (playerPos.y < m_position.y)nextPos.y--;

    if (room->GetLocation(nextPos) == ' ') {
        m_position = nextPos;
    }
    else if (room->GetLocation(nextPos) == 'P') {
        room->BeginCombat(m_position);
    }
}

char Ogre::Draw() { return 'O'; }
int Ogre::GetGold() { return m_EGold; }
int Ogre::GetXP() { return m_XPworth; }
void Ogre::TakeDamage(int _amount) { m_EHealth -= _amount; }