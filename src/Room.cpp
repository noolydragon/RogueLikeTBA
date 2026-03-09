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
                if (m_player == nullptr)
                    m_player = new Player();
                
                m_player->Start(Vec2(x,y));
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
                g->Start(Vec2(x,y));
                m_entities.push_back(g);
                m_map[y][x] = ' ';
            }
            else if (m_map[y][x] == 'M')
            {
                Sloblin* s = new Sloblin();
                s->Start(Vec2(x,y));
                m_entities.push_back(s);
                m_map[y][x] = ' ';
            }
        }
    }
}

void Room::Update()
{
    for (Entity* e : m_entities) {
        e->Update();
    }
    Draw();
    if (m_player != nullptr)
    {
        m_player->room = this;
        m_player->Update();
    }
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
    return m_map[_pos.y][_pos.x];
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
    std::string sti = std::to_string(1 + (rand() % (9-1 +1)));
    Load("assets/level_" + sti + ".map" );
    m_roomcount ++;
    if(m_roomcount == 10){
        Load("assets/level_10.map");
    }
    
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
        printf("\nPlayer HP: %d | Enemy HP: %d", m_player->GetHP(), targetEnemy->GetHP());
        request_char("\nEnter roll to roll for attack");

        
        std::vector<Die> playerDice = { {20} }; // Change based on weapon
        
        RollStats playerRoll = RollDice(playerDice);
        
        printf("You rolled a total of %d\n", playerRoll.total);
        targetEnemy->TakeDamage(playerRoll.total);
        
        if (playerRoll.critCount > 0) {
            printf("CRITICAL HIT! %d\n", playerRoll.critCount);
        }

        if (m_player->GetHP() <= 0) {
            ClearLocation(_pos);
            printf("You have been vanquished!");

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

        std::vector<Die> enemyDice = { {10} };
        RollStats enemyRoll = RollDice(enemyDice);
        
        printf("Enemy attacks for %d damage!\n", enemyRoll.total);
        m_player->TakeDamage(enemyRoll.total);
    }
    printf("You gained %d gold and now have %d total gold, %d health %d XP\n", gainedGold, m_player->GetGold(), m_player->GetHP(), m_player->GetXP());
}

void Room::Trap(Vec2 _pos){
    m_player->TakeDamage(5);
    printf("you have taken %d health becuse of a trap\n", m_player->GetHP());
}

