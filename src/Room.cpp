#include "Room.hpp"
#include "Dice.hpp"
#include "Player.hpp"
#include "Enemy.hpp"
#include "LootTable.hpp"

#include <fstream>
#include <string>
#include <random>
#include <list>

Player* Room::GetPlayer()
{
    return m_player;
}

void Room::Load(std::string _path)
{
    for (Entity* e : m_entities) {
        delete e;
    }
    m_entities.clear();

    m_map.clear();
    m_doors.clear();

    std::ifstream file;
    file.open(_path);

    

    if (!file.is_open())
    {
        printf("file not found at: %s \n", _path.c_str());
        exit(1);
    }

    std::string word;
    int number;

    while (file >> word)
    {
        if (word == "level")
        {
            if (file >> number)
            {
                printf("open level: %i\n", number);
            }
        }

        if (word == "next_level")
        {
            if (file >> word)
            {
                m_doors.push_back(Door());
                m_doors[m_doors.size() - 1].path = word;
            }
        }

        if (word == "map")
        {
            m_map.push_back(std::vector<char>());
            while(file >> word)
            {
                if (word == "-2")
                {
                    break;
                }

                if (word == "-1")
                {
                    m_map.push_back(std::vector<char>());
                    continue;
                }

                if (word == "0")
                    m_map[m_map.size() - 1].push_back(' ');
                else
                    m_map[m_map.size() - 1].push_back(word[0]);
                
            }
        }
    }

    int doorCount = 0;
    for (int y = 0; y < m_map.size(); y++)
    {
        for (int x = 0; x < m_map[y].size(); x++)
        {
            if (m_map[y][x] == 'S')
            {
                if (m_player == nullptr){
                    m_player = new Player();
                    m_player->Start(Vec2(x,y));
                } else {
                    m_player->MoveToStart(Vec2(x,y));
                }
                m_map[y][x] = ' ';
            }

            if (m_map[y][x] == 'D' || m_map[y][x] == 'L')
            {
                if (m_doors.size() - 1 >= doorCount)
                {
                    m_doors[doorCount].pos.x = x;
                    m_doors[doorCount].pos.y = y;
                    doorCount++;
                }
            }
            //instantiate a Goblin where G appears on the map
            if (m_map[y][x] == 'G')
            {
                Goblin* g = new Goblin();
                g->room = this;
                int scaledHP = 5 + (m_roomcount);
                g->Start(Vec2(x,y), scaledHP);
                m_entities.push_back(g);
                m_map[y][x] = ' ';
            }
            else if (m_map[y][x] == 'M')
            {
                Sloblin* s = new Sloblin();
                s->room = this;
                int scaledHP = 15 + (m_roomcount);
                s->Start(Vec2(x,y), scaledHP);
                m_entities.push_back(s);
                m_map[y][x] = ' ';
            }
            else if (m_map[y][x] == 'O')
            {
                Ogre* o = new Ogre();
                o->room = this;
                int scaledHP = 25 + (m_roomcount);
                o->Start(Vec2(x,y), scaledHP);
                m_entities.push_back(o);
                m_map[y][x] = ' ';
            }

            if(m_map[y][x] == 'T'){
                m_map[y][x] = '.';
            }
        }
    }
}

void Room::Update()
{
    //Moves the player first
    if (m_player != nullptr)
    {
        if (!m_player->IsAlive()) {
            printf("GAME OVER\n");
            m_player->PrintStats();
            delete m_player;
            m_player = nullptr;
            request_char("Press any key to exit...");
            exit(0);
        }
        m_player->room = this;
        m_player->Update();
    }
    //Moves the enemy in response
    for (Entity* e : m_entities) {
        e->Update();
    }
    Draw();
}

void Room::Draw()
{
    for (int y = 0; y < m_map.size(); y++)
    {
        for (int x = 0; x < m_map[y].size(); x++)
        {
            printf("%c ", GetLocation(Vec2(x, y)));
        }
        printf("\n");
    }
}

char Room::GetLocation(Vec2 _pos)
{
    if (_pos.y >= m_map.size()|| _pos.y < 0)
        return ' ';
    
    if (_pos.x >= m_map[_pos.y].size()|| _pos.x < 0)
        return ' ';

    if (m_player != nullptr && m_player->GetPosition() == _pos)
    {
        return m_player->Draw();
    }

    for (Entity* e : m_entities)
    {
        if (e != nullptr && e->GetPosition() == _pos)
        {
            return e->Draw();
        }
    }

    char mapChar = m_map[_pos.y][_pos.x];
    
    if (mapChar == 'D' || mapChar == 'L' || mapChar == 'A')
    {
        if (!IsRoomClear())
        {
            return 'L';
        }
    }
    return mapChar;
}

void Room::ClearLocation(Vec2 _pos)
{
    if (_pos.y >= m_map.size())
        return;
    
    if (_pos.x >= m_map[_pos.y].size())
        return;
    
    m_map[_pos.y][_pos.x] = ' ';
}

void Room::OpenDoor(Vec2 _pos)
{
    //Locked door logic yay!
    if (!IsRoomClear()) {
        printf("\nYou can't leave, magic bars the doors.\n");
        return;
    }

    //Between room healing logic
    std::vector<Die> restDice = { {6} };
    RollStats healRoll = RollDice(restDice);
    m_player->AddHP(healRoll.total);
    printf("\nYou rested and recovered %d HP! Current HP: %d\n", healRoll.total, m_player->GetHP());
    request_char("Enter any symbol to continue...");

    //Idk logic...wait, level loding
    std::string sti = std::to_string(1 + (rand() % (9-1 +1)));
    Load("assets/level_" + sti + ".map" );
    m_roomcount ++;
    if(m_roomcount == 10){
        Load("assets/level_10.map");
    }
}

bool Room::IsRoomClear() {
    for (Entity* e : m_entities) {
        if (e == nullptr) continue;

        Enemy* isEnemy = dynamic_cast<Enemy*>(e);

        if (isEnemy != nullptr) {
            return false;
        }
    }
    return true;
}

void Room::BeginCombat(Vec2 _pos)
{
    char promptAnswer;
    Enemy* targetEnemy = nullptr;

    for (int i = 0; i < m_entities.size(); i++) {
        if(m_entities[i]->GetPosition() == _pos) {
            targetEnemy = dynamic_cast<Enemy*>(m_entities[i]);
            break;
        }
    }
    if (m_player == nullptr || targetEnemy == nullptr) return;
    int gainedGold = targetEnemy->GetGold();

    printf("\nCOMBAT STARTED with a %c\n", targetEnemy->Draw());


    while (targetEnemy->GetHP() > 0 && m_player->GetHP() > 0)
        {
        printf("\nPlayer HP: %d | Enemy HP: %d\n", m_player->GetHP(), targetEnemy->GetHP());
        request_char("\nEnter roll to roll for attack");

        if(m_weapon == 101){
            playerRoll = RollDice(s_die);
            printf("short sword\n");
        }
        else if(m_weapon == 102){
            playerRoll = RollDice(g_die);
            printf("Great sword\n");
        }
        else if(m_weapon == 103){
            playerRoll = RollDice(G_die);
            printf("Gun\n");
        }
        else if(m_weapon == 105){
            playerRoll = RollDice(r_die);
            printf("rapier\n");
        }
        else if(m_weapon == 104){
            playerRoll = RollDice(d_die);
            printf("dagger\n");
        }
        else if(m_weapon == 106){
            playerRoll = RollDice(l_die);
            printf("long sword\n");
        }
        else{
            playerRoll = RollDice(d_die);
            printf("pan\n");
        }
        
        
        printf("You rolled a total of %d\n", playerRoll.total);
        targetEnemy->TakeDamage(playerRoll.total);
        
        if (playerRoll.critCount > 0) {
            printf("CRITICAL HIT! %d\n", playerRoll.critCount);
        }

        if (targetEnemy->GetHP() <= 0) {
            ClearLocation(_pos);
            printf("The enemy has been slain!\n");
            m_player->SetXP(targetEnemy->GetXP());

            printf("Would you like to loot?\n");
            do{
            promptAnswer = request_char("\nY or N");
            } while (promptAnswer != 'y' &&
                     promptAnswer != 'n');
            if (promptAnswer == 'y'){
                m_player->AddGold(gainedGold);
            }else break;

            for (auto it = m_entities.begin(); it != m_entities.end(); ++it) {
                if (*it == targetEnemy) {
                    delete *it;
                    m_entities.erase(it);
                    break;
                }
            }
            break;
        }

        if(targetEnemy->Draw() == 'G'){
            enemyRoll = RollDice(e_die);
        }
        else if(targetEnemy->Draw() == 'M'){
            enemyRoll = RollDice(m_die);
        }
        else if(targetEnemy->Draw() == 'O'){
            enemyRoll = RollDice(o_die);
        }
        
        printf("Enemy attacks for %d damage!\n", enemyRoll.total);
        m_player->TakeDamage(enemyRoll.total);
        if (m_player->IsAlive() == false) {
            printf("\nThat was fatal, you died...\n");
            break;
        }
    }
    if (m_player->IsAlive()) {
        printf("\nYou gained %d gold\n", gainedGold);
        m_player->PrintStats();
    } else {
        printf("\nYou lost!\n");
        m_player->PrintStats();
    }
    
}

void Room::Trap(Vec2 _pos){
    if(m_roomcount == 10){
        m_player->TakeDamage(2);
        printf("you are at %d health becuse of a trap\n", m_player->GetHP());\
        if(m_player->GetHP() <= 0){
            printf("you died");
            exit(0);
        }
    }
}
void Room::Accesion(Vec2 _pos){
    printf("\nyou won\n");
    m_player->PrintStats();
    exit(0);
}
void Room::Chest(Vec2 _pos){
    std::srand(static_cast<unsigned int>(std::time(0))); 

    LootTable chestLoot;
    chestLoot.addEntry({"Short Sword", 101, 's'}, 4.5);
    chestLoot.addEntry({"Health Potion", 107, 'h'}, 0);
    chestLoot.addEntry({"Gun", 103, 'g'}, 1);
    chestLoot.addEntry({"Dagger", 104, 'd'}, 5);
    chestLoot.addEntry({"Rapier", 105, 'r'}, 4);
    chestLoot.addEntry({"Long Sword", 106, 'l'}, 3);
    chestLoot.addEntry({"Great Sword", 102, 'G'}, 2);
    chestLoot.addEntry({"5 Gold", 108, '5'}, 5);
    chestLoot.addEntry({"10 Gold", 109, '1'}, 6);
    chestLoot.addEntry({"15 Gold", 110, '0'}, 7);

    Item droppedItem = chestLoot.chooseRandomItem();
    std::cout << "You found: " << droppedItem.name << std::endl;
    if(droppedItem.itemId < 107){
        m_weapon = droppedItem.itemId;
    }
    if(droppedItem.itemId > 107){
        if(droppedItem.itemChar == '5'){
            m_player->AddGold(5);
        }
        else if(droppedItem.itemChar == '1'){
            m_player->AddGold(10);
        }
        else if(droppedItem.itemChar == '0'){
            m_player->AddGold(15);
        }

    }
}


