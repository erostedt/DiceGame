#include <string>
#include <vector>

class Player {

public:
    std::string name;
    int dice;

    Player(std::string name, int dice){
        this->name = name;
        this->dice = dice;
    }

    std::vector<int> roll(){
        std::vector<int> dice_rolls(dice);
        for(int d = 0; d < dice; d++){
            dice_rolls[d] = (std::rand() % 6) + 1;
        }
        return dice_rolls;
    }

    int end_game_roll(){
        return (std::rand() % 6) + 1;
    }
};


