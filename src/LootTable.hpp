#include <iostream>
#include <vector>
#include <string>
#include <numeric>
#include <random>
#include <ctime>

// 1. Define the Item structure
struct Item {
    std::string name;
    int itemId;
    char itemChar;
    // Add other properties like rarity, value, etc.
};

// 2. Define the Weighted Entry structure
struct LootEntry {
    Item item;
    unsigned int weight;
    
};

// 3. Define the Loot Table class
class LootTable {
public:
    void addEntry(const Item& item, unsigned int weight) {
        entries.push_back({item, weight});
        totalWeight = std::accumulate(entries.begin(), entries.end(), 0, [](int sum, const LootEntry& entry){
            return sum + entry.weight;
        });
    }

    // Function to select a random item
    Item chooseRandomItem() const {
        // Use a more robust random number generation for actual games
        // std::srand(std::time(0)); 
        int roll = std::rand() % totalWeight; // Roll between 0 and total weight - 1
        
        int currentWeight = 0;
        for (const auto& entry : entries) {
            currentWeight += entry.weight;
            if (roll < currentWeight) {
                // In a real system, you'd also determine quantity here
                return entry.item;
            }
        }
        return {"Nothing", 0}; // Should not reach here if implemented correctly and no "null" item is added
    }

private:
    std::vector<LootEntry> entries;
    int totalWeight = 0;
};