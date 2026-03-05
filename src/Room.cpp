#include "Room.hpp"
#include "Dice.hpp"
#include "Player.hpp"
#include "Enemy.hpp"

#include <fstream>
#include <string>

void Room::Load(std::string _path)
{
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
            //instantiate an enemy where E appears on the map
            if (m_map[y][x] == 'E')
            {
                if (m_enemy == nullptr)
                    m_enemy = new Enemy();

                m_enemy->Start(Vec2(x,y));
                m_map[y][x] = ' ';
            }
        }
    }
}

void Room::Update()
{
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
    if (_pos.y >= m_map.size())
        return ' ';
    
    if (_pos.x >= m_map[_pos.y].size())
        return ' ';

    if (m_player != nullptr)
        if (m_player->GetPosition() == _pos)
            return m_player->Draw();
    
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
    for(int i = 0; i < m_doors.size(); i++)
    {
        if (m_doors[i].pos == _pos)
        {
            Load(m_doors[i].path.c_str());
        }
    }
}

void Room::BeginCombat(Vec2 _pos){
    if (m_player == nullptr || m_enemy == nullptr) return;

    printf("Combat Started");

    while (m_enemy->GetHP() > 0 && m_player->GetHP() > 0) {
        printf("\nPlayer HP: %d | Enemy HP: %d", m_player->GetHP(), m_enemy->GetHP());
        printf("\nPress enter to roll for an attack! ");
        getchar();

        std::vector<Die> playerDice = {{20}};

        RollStats playerRoll = RollDice(playerDice);

        printf("You rolled a %d!\n", playerRoll.total);

        if (m_enemy->GetHP() <= 0) {
            printf("\nVictory! The enemy has been defeated.\n");
            ClearLocation(_pos);
            break;
        }

        printf("Enemy turn\n");
        int enemyRoll = Dice::RollDice(1, 20);
        if (enemyRoll > 12) {
            int eDamage = Dice::RollDice(1, 4);
            m_player->TakeDamage(eDamage);
            printf("You took %d damage!\n", eDamage);
        }else {
            printf("The enemy missed!\n");
        }

        if (m_player->GetHP() <= 0) {
            printf("\nYOU DIED");
            exit(0);
        }
    }
    printf("COMBAT ENDED");
}