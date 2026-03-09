#include "Player.hpp"
#include "Room.hpp"
#include "fogpi/io.hpp"
#include "Dice.hpp"
#include "LootTable.hpp"
#include "Enemy.hpp"

void Player::Start(Vec2 _pos) {
    m_character = 'P';
    m_position = _pos;
    m_weapon = 'p';
}

void Player::Update() {
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

    if (room->GetLocation(tryPos) == 'C'){
         std::srand(static_cast<unsigned int>(std::time(0))); 

        LootTable chestLoot;
        chestLoot.addEntry({"Short Sword", 101, 's'}, 4);
        chestLoot.addEntry({"Health Potion", 107, 'h'}, 8);
        chestLoot.addEntry({"Gun", 103, 'g'}, 0.5);
        chestLoot.addEntry({"Dagger", 104, 'd'}, 5);
        chestLoot.addEntry({"Rapier", 105, 'r'}, 4);
        chestLoot.addEntry({"Long Sword", 106, 'l'}, 3);
        chestLoot.addEntry({"Great Sword", 102, 'G'}, 2);
        chestLoot.addEntry({"5 Gold", 108, '5'}, 2);
        chestLoot.addEntry({"10 Gold", 109, '1'}, 2);
        chestLoot.addEntry({"15 Gold", 110, '0'}, 2);

        Item droppedItem = chestLoot.chooseRandomItem();
        std::cout << "You found: " << droppedItem.name << std::endl;
        if(droppedItem.itemId < 107){
            m_weapon = droppedItem.itemChar;
        }
        if(droppedItem.itemId > 107){
            m_goldAmount;
        }
    }
    if (room->GetLocation(tryPos) == 'G'|| room->GetLocation(tryPos) == 'M'){
        room->BeginCombat(tryPos);
    }
}