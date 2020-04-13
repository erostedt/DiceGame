#include "Player.cpp"
#include <iostream>

struct Fighters{
    /*
    * Fighter object which holds winner or loser when a bluff occurs.
    */
    int winner;
    int loser;
};

// Normal game

bool valid_move(int new_val, int new_dice, int old_val, int old_dice){
    /*
    * Functions for determining if a move was valid according to the game rules.
    * @param new_val Proposed new value.
    * @param new_dice Proposed new amount of dice.
    * @param old_val Old value.
    * @param old_dice Old number of dice.
    * @return True if move is legal, false else.
    */
    if (new_val > 6 || new_val < 1 || new_dice < 0) {
        return false;
    }

    if ((new_val > old_val && new_dice >= old_dice) || (new_dice > old_dice)){
        return true;
    }
    return false;
}

void get_standing(std::vector<Player> players) {
    /*
    * Prints the current game standing.
    * @param players Vector of players.
    */
    for(Player p: players){
        std::cout << p.name + " has " + std::to_string(p.dice) + " dice left" << '\n';
    }
}

void get_roll(std::vector<int> rolls){
    /*
    * Prints the rolls for a given vector of rolls.
    * @param rolls Vector of rolls.
    */
    std::string p_roll = "";
    for(int roll: rolls){
        p_roll = p_roll + std::to_string(roll) + " ";
    }
    std::cout << p_roll << '\n';
}

Fighters get_winner_and_loser(int num_players, int bluffer, int new_val, int new_dice, std::vector< std::vector<int> > rolls){
    /*
    * Finds winner and loser when a bluff occurs.
    * @param num_players Number of players left in the game.
    * @param bluffer Player index of the player who bluffed.
    * @param new_val Proposed value.
    * @param new_dice Proposed amount of dice.
    * @param rolls All rolls for all players.
    * @return fighters Fighter object with the winner and loser.
    */
    Fighters fighters; 
     int dice_count = 0;
     for(std::vector<int> player_roll: rolls){
         for(int die: player_roll){
             if (die == new_val){
                 dice_count++;
             }
         }
     }
     if (dice_count < new_dice){
         fighters.winner = bluffer;
         fighters.loser = (bluffer > 0) ? bluffer - 1 : num_players;
     }
     else{
         fighters.winner = (bluffer > 0) ? bluffer - 1 : num_players;
         fighters.loser = bluffer;
     }
     std::cout << "Number of " + std::to_string(new_val) + "'s' was: " + std::to_string(dice_count) << '\n';
     return fighters;
 }

 // End game

 bool end_game_valid_move(int new_total_sum, int old_total_sum){
     if(new_total_sum > old_total_sum && new_total_sum < 13){
         return true;
     }
     return false;
 }

 Fighters end_game_get_winner_and_loser(int bluffer, int new_val, std::vector<int> rolls){
     int total_sum = 0;
     Fighters fighters;
     for (int roll: rolls){
        total_sum += roll;
     }
     if (total_sum < new_val){
        fighters.winner = bluffer;
        fighters.loser = (bluffer + 1) % 2;
     }
     else{
        fighters.loser = bluffer;
        fighters.winner = (bluffer + 1) % 2;
     }

     std::cout << "Guessed total sum was: " + std::to_string(new_val) + ", and actual total sum was: " + std::to_string(total_sum) << '\n';
     return fighters;
 }

int main() {
    /*
    * Runs the game according to the game rules.
    */
    int num_players;
    int dice;
    std::cout << "How many players?" << '\n';
    std::cin >> num_players;

    std::cout << "How many dice per person?" << '\n';
    std::cin >> dice;

    std::vector<Player> players;

    for (int i = 0; i < num_players; i++) {
        std::cout << "Enter a name" << '\n';
        std::string name;
        std::cin >> name;
        players.push_back(Player(name, dice));
    }

    int curr_player_idx = 0;
    while (players.size() > 2) {
        // Roll
        std::vector< std::vector<int> > rolls(num_players);
        for (int p = 0; p < num_players; p++){
            rolls[p] = players[p].roll();

            std::string see_roll = "n";
            std::cout << players[p].name + ", do you want to see your roll (press y if so)?" << '\n';
            std::cin >> see_roll;
            if (see_roll == "y"){
                get_roll(rolls[p]);
            }
        }

        int new_val = 0;
        int new_dice = 1;
        int old_val = 0;
        int old_dice = 1;
        std::string bluff = "n";
        while (1) {
            // Play a round
            Player curr_player = players[curr_player_idx];

            if(new_val > 0){
                std::cout << curr_player.name + ", do you want to bluff (press y if so)?" << '\n';
                std::cin >> bluff;
                if (bluff == "y") {
                    break;
                }
            }

            while (!valid_move(new_val, new_dice, old_val, old_dice)){
                std::cout << curr_player.name + ", guess how many dice?" << '\n';
                std::cin >> new_dice;
                std::cout << "And what value?" << '\n';
                std::cin >> new_val;
            }
            std::cout << "Current guess is " + std::to_string(new_dice) + " " + std::to_string(new_val) + "'s" << '\n';

            old_val = new_val;
            old_dice = new_dice;

            curr_player_idx++;
            if (curr_player_idx == num_players){
                curr_player_idx = 0;
            }
        }
        Fighters fighters = get_winner_and_loser(num_players, curr_player_idx, new_val, new_dice, rolls);
        std::cout << players[fighters.winner].name  + " wins! " + players[fighters.loser].name + " loses a die"  << '\n';
        players[fighters.loser].dice--;
        if (players[fighters.loser].dice == 0){
            std::cout << players[fighters.loser].name + " is out!" << '\n';
            players.erase(players.begin() + fighters.loser); 
            num_players--;
        }
        curr_player_idx = fighters.winner;
        std::string see_standings;
        std::cout << "Wish to see standings? (press y if so)." << '\n';
        std::cin >> see_standings;
        if (see_standings == "y"){
            get_standing(players);
        }
    }
    // End game reached.
    while (num_players > 1) {
        std::vector<int> rolls;
        int roll = 0;
        std::string see_roll = "n";
        for (Player player: players){
            roll = player.end_game_roll();
            std::cout << player.name + ", want to see your roll? (press y if so)." << '\n';
            std::cin >> see_roll;
            if (see_roll == "y"){
                std::cout << roll << '\n';
            }
            rolls.push_back(roll);
        }
        int old_total_sum = 0;
        int new_total_sum = 0;
        std::string bluff = "n";
        while (new_total_sum < 12){
            Player curr_player = players[curr_player_idx];
            
            if(new_total_sum > 0){
                std::cout << curr_player.name + ", do you want to bluff? (press y if so)" << '\n';
                std::cin >> bluff;
                if (bluff == "y"){
                    break;
                }
            }

            while(!end_game_valid_move(new_total_sum, old_total_sum)){
                std::cout << curr_player.name + ", guess total sum of the dice." << '\n';
                std::cin >> new_total_sum;
            }
            old_total_sum = new_total_sum;
            curr_player_idx = (curr_player_idx+1) % 2;
        } 
        Fighters fighters = end_game_get_winner_and_loser(curr_player_idx, new_total_sum, rolls);
        std::cout << players[fighters.winner].name + " wins! " + players[fighters.loser].name + " loses." << '\n';
        players[fighters.loser].dice--;
        if (players[fighters.loser].dice == 0){
            std::cout << players[fighters.loser].name + " is out!" << '\n';
            players.erase(players.begin() + fighters.loser);
            num_players--;
        }
        else{
            curr_player_idx = fighters.winner;
            std::string see_standing = "n";
            std::cout << players[curr_player_idx].name + ", do you want to see current standing? (press y if so)." << '\n';
            std::cin >> see_standing;
            if (see_standing == "y"){
                get_standing(players);
            }
        }
    }
    std::cout << players[0].name + " wins the game!" << '\n';
}
