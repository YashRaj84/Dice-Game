// DiceGame_Compatible.cpp

// Includes all standard C++ libraries.
#include <bits/stdc++.h>
// This header is needed for the Sleep() function on Windows.
#include <windows.h>

// Allows us to use names for objects and variables from the standard library.
using namespace std;

int main() {
    // --- 1. GAME SETUP ---
    cout << "?? Welcome to Dice Race to 100! ??\n";
    cout << "====================================\n";

    // Seed the random number generator. This is a classic way to ensure
    // you get different random numbers each time you run the program.
    srand(time(0));

    // Get the number of players
    int num_players;
    cout << "Enter the number of players (2-4): ";
    cin >> num_players;
    if (num_players < 2 || num_players > 4) {
        cout << "Invalid number of players. Please restart. Exiting.\n";
        return 1;
    }

    // Data Structures Initialization
    queue<int> player_queue;
    vector<string> player_names(num_players);
    vector<int> player_positions(num_players, 0);

    // Get player names and populate the queue
    for (int i = 0; i < num_players; ++i) {
        cout << "Enter name for Player " << i + 1 << ": ";
        cin >> player_names[i];
        player_queue.push(i);
    }

    // Create the map for special squares (Boosts & Setbacks)
    map<int, int> board_specials;
    // Boosts (Ladders)
    board_specials[4] = 14;
    board_specials[9] = 31;
    board_specials[20] = 38;
    board_specials[28] = 84;
    board_specials[40] = 59;
    board_specials[51] = 67;
    board_specials[63] = 81;
    board_specials[71] = 91;
    // Setbacks (Snakes)
    board_specials[17] = 7;
    board_specials[54] = 34;
    board_specials[62] = 19;
    board_specials[64] = 60;
    board_specials[87] = 24;
    board_specials[93] = 73;
    board_specials[95] = 75;
    board_specials[98] = 79;
    
    cout << "\nGame starting... Press Enter to roll the dice.\n";
    cin.ignore();

    // --- 2. GAME LOOP ---
    int winner = -1;
    while (winner == -1) {
        int current_player_index = player_queue.front();

        cout << "------------------------------------\n";
        cout << "It's " << player_names[current_player_index] << "'s turn. (Position: " << player_positions[current_player_index] << ")\n";
        cout << "Press Enter to roll the dice...";
        cin.get();

        // Roll the dice using the classic rand() function
        int roll = rand() % 6 + 1; // Generates a number from 1 to 6
        cout << "\n" << player_names[current_player_index] << " rolled a " << roll << "!\n";
        Sleep(500); // Pause for 500 milliseconds

        int potential_position = player_positions[current_player_index] + roll;

        if (potential_position > 100) {
            cout << "Oh no! A roll of " << roll << " overshoots 100. Turn skipped.\n";
        } else if (potential_position == 100) {
            player_positions[current_player_index] = 100;
            winner = current_player_index;
        } else {
            player_positions[current_player_index] = potential_position;
            cout << "You move to position " << player_positions[current_player_index] << ".\n";
            Sleep(500);

            if (board_specials.count(player_positions[current_player_index])) {
                int start_pos = player_positions[current_player_index];
                int end_pos = board_specials[start_pos];
                
                if (end_pos > start_pos) {
                    cout << "?? Wow! You landed on a boost! You jump from " << start_pos << " to " << end_pos << "!\n";
                } else {
                    cout << "?? Ouch! You landed on a setback! You slide from " << start_pos << " to " << end_pos << ".\n";
                }
                player_positions[current_player_index] = end_pos;
                Sleep(1000);
            }
        }
        
        cout << "\nCurrent Standings:\n";
        for (int i = 0; i < num_players; ++i) {
            cout << "  - " << player_names[i] << ": " << player_positions[i] << "\n";
        }

        player_queue.pop();
        player_queue.push(current_player_index);
        
        Sleep(1000);
    }

    // --- 3. GAME END ---
    cout << "\n====================================\n";
    cout << "?? CONGRATULATIONS " << player_names[winner] << "! YOU WIN! ??\n";
    cout << "====================================\n";
    cout << "Thank you for playing!\n";

    return 0;
}
