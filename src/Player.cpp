#include "Player.hpp"
#include "Room.hpp"
#include "fogpi/io.hpp"
#include "Dice.hpp"
#include "LootTable.hpp"
#include "Enemy.hpp"
#include "Room.hpp"

void Player::Start(Vec2 _pos) {
    m_character = 'P';
    m_position = _pos;
    m_weapon = 'p';
    m_currentLvl = 1;
    m_currentXP = 0;
    m_requiredXP = 20;
    MoveToStart(_pos);
}

void Player::MoveToStart(Vec2 _pos) {
    m_position = _pos;
}

void Player::Update(){
    //while(request_char("hit w to continue: ") != 'w') {}

    if(m_weapon == 'p'){
        Die die = { .sides = 2};
    }
    if(m_weapon == 's'){
        Die die = { .sides = 6};
    }
    if(m_weapon == 'g'){
        Die die = { .sides = 20};
    }
    if(m_weapon == 'G'){
        Die die = { .sides = 100};
    }
    if(m_weapon == 'r'){
        Die die = { .sides = 8};
    }
    if(m_weapon == 'd'){
        Die die = { .sides = 4};
    }
    if(m_weapon == 'l'){
        Die die = { .sides = 12};
    }

    char directionInput;

    do {
        directionInput = request_char("wasd and Enter to move");
    } while (directionInput != 'w' &&
             directionInput != 'a' &&
             directionInput != 's' &&
             directionInput != 'd');
    
    Vec2 direction(0.0f);

    switch (directionInput)
    {
    case 'w':
        direction = {0.0f, -1.0f};
        break;
    case 'a':
        direction = {-1.0f, 0.0f};
        break;
    case 's':
        direction = {0.0f, 1.0f};
        break;
    case 'd':
        direction = {1.0f, 0.0f};
        break;
    default:
        direction = {0.0f, 1.0f};
        break;
    }

    Vec2 tryPos = m_position + direction;

    if (room->GetLocation(tryPos) == 'K') {
        m_keyCount++;
        room->ClearLocation(tryPos);
    }

    if (room->GetLocation(tryPos) == ' ') {
        m_position = tryPos;
    }

    if (room->GetLocation(tryPos) == 'D') {
        room->OpenDoor(tryPos);
    }

    if (room->GetLocation(tryPos) == 'L' && (m_keyCount >= 1)){
        room->OpenDoor(tryPos);
        m_keyCount -= 1;
        printf("You used a key!, You have %d keys left\n", m_keyCount);
    }

    if (room->GetLocation(tryPos) == 'D' || room->GetLocation(tryPos) == 'L') {
        if (room->IsRoomClear()) {
            room->OpenDoor(tryPos);
        } else {
            printf("\nThe door is sealed by goblin magic.\n");
        }
    }

    if (room->GetLocation(tryPos) == 'C' && (m_keyCount >= 1)){
        room->Chest(tryPos);
        m_keyCount--;
        room->ClearLocation(tryPos);
    }
    if (room->GetLocation(tryPos) == 'G'|| room->GetLocation(tryPos) == 'M')
    {
        room->BeginCombat(tryPos);
    }

    if (room->GetLocation(tryPos) == 'T')
    {
        room->Trap(tryPos);
    }
}

int Player::PrintStats(){
    return printf("YOUR STATS:\nLevel: %d\nHealth: %d\nGold: %d\nXP: %d\n", m_currentLvl, m_Phealth, m_goldAmount, m_currentXP);
}

void Player::SetXP(int _amount) {
    m_currentXP += _amount;

    while (m_currentXP >= m_requiredXP){
        m_currentXP -= m_requiredXP;
        m_currentLvl ++;

        m_maxHP += 1; //Increases the health cap;
        m_Phealth = m_maxHP; //Level up heal just dropped;
        // Increase the threshold by a slight percentage
        m_requiredXP = int(m_requiredXP * 1.2f);
        printf("LEVEL UP!!! You are now level %d!\nMax HP raised to %d!\n", m_currentLvl, m_maxHP);
    }
}

void Player::AddHP(int _amount) {
    m_Phealth += _amount;

    //Health cap logic
    if (m_Phealth > m_maxHP) {
        m_Phealth = m_maxHP;
    }
}