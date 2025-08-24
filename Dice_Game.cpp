#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <string>
#include <random>
#include <thread>
#include <chrono>

//================================================================
// Player Class
// Manages the data for a single player.
//================================================================
class Player {
private:
    std::string name;
    int position;

public:
    // Constructor to initialize a player with a name and starting position 0
    Player(const std::string& playerName) : name(playerName), position(0) {}

    // Getter for the player's name
    std::string getName() const {
        return name;
    }

    // Getter for the player's position
    int getPosition() const {
        return position;
    }

    // Setter to update the player's position
    void setPosition(int newPosition) {
        position = newPosition;
    }
};

//================================================================
// Dice Class
// Handles the logic for rolling a die.
//================================================================
class Dice {
private:
    std::mt19937 gen; // Mersenne Twister random number generator
    std::uniform_int_distribution<> distrib;

public:
    // Constructor sets up the random number generator
    Dice() : distrib(1, 6) {
        std::random_device rd;
        gen.seed(rd());
    }

    // Rolls the dice and returns the result
    int roll() {
        return distrib(gen);
    }
};

//================================================================
// Board Class
// Manages the game board, including special squares.
//================================================================
class Board {
private:
    std::map<int, int> specialSquares;
    const int finalSquare = 100;

public:
    // Constructor initializes the boosts and setbacks
    Board() {
        specialSquares = {
            // Boosts (Ladders)
            {4, 14}, {9, 31}, {20, 38}, {28, 84}, {40, 59}, {51, 67}, {63, 81}, {71, 91},
            // Setbacks (Snakes)
            {17, 7}, {54, 34}, {62, 19}, {64, 60}, {87, 24}, {93, 73}, {95, 75}, {98, 79}
        };
    }

    int getFinalSquare() const {
        return finalSquare;
    }

    // Checks if a square is special and returns the destination, otherwise returns the original position
    int getSpecialSquareTarget(int position) const {
        if (specialSquares.count(position)) {
            return specialSquares.at(position);
        }
        return position; // Not a special square, return original position
    }
};

//================================================================
// Game Class
// Orchestrates the entire game flow.
//================================================================
class Game {
private:
    std::vector<Player> players;
    std::queue<int> playerTurnQueue;
    Board board;
    Dice dice;
    int winnerIndex = -1;

    // Utility function to pause the game
    void pause(int milliseconds) {
        std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
    }

    // Sets up the game by getting player count and names
    void setup() {
        std::cout << "🎲 Welcome to Dice Race to 100! 🎲\n";
        std::cout << "====================================\n";

        int num_players;
        std::cout << "Enter the number of players (2-4): ";
        std::cin >> num_players;
        if (num_players < 2 || num_players > 4) {
            std::cout << "Invalid number of players. Please restart. Exiting.\n";
            exit(1); // Exit the program
        }

        for (int i = 0; i < num_players; ++i) {
            std::string name;
            std::cout << "Enter name for Player " << i + 1 << ": ";
            std::cin >> name;
            players.emplace_back(name); // Create Player object in place
            playerTurnQueue.push(i);
        }

        std::cout << "\nGame starting... Press Enter to roll the dice.\n";
        std::cin.ignore();
    }

    // Manages a single turn for a player
    void takeTurn() {
        int currentPlayerIndex = playerTurnQueue.front();
        Player& currentPlayer = players[currentPlayerIndex]; // Use a reference for easier access

        std::cout << "------------------------------------\n";
        std::cout << "It's " << currentPlayer.getName() << "'s turn. (Position: " << currentPlayer.getPosition() << ")\n";
        std::cout << "Press Enter to roll the dice...";
        std::cin.get();

        int roll = dice.roll();
        std::cout << "\n" << currentPlayer.getName() << " rolled a " << roll << "!\n";
        pause(500);

        int potentialPosition = currentPlayer.getPosition() + roll;

        if (potentialPosition > board.getFinalSquare()) {
            std::cout << "Oh no! A roll of " << roll << " overshoots 100. Turn skipped.\n";
        } else if (potentialPosition == board.getFinalSquare()) {
            currentPlayer.setPosition(potentialPosition);
            winnerIndex = currentPlayerIndex; // We have a winner!
        } else {
            currentPlayer.setPosition(potentialPosition);
            std::cout << "You move to position " << currentPlayer.getPosition() << ".\n";
            pause(500);

            int specialTarget = board.getSpecialSquareTarget(currentPlayer.getPosition());
            if (specialTarget != currentPlayer.getPosition()) {
                int startPos = currentPlayer.getPosition();
                if (specialTarget > startPos) {
                    std::cout << "🎉 Wow! You landed on a boost! You jump from " << startPos << " to " << specialTarget << "!\n";
                } else {
                    std::cout << "🐍 Ouch! You landed on a setback! You slide from " << startPos << " to " << specialTarget << ".\n";
                }
                currentPlayer.setPosition(specialTarget);
                pause(1000);
            }
        }
    }

    // Prints the current standings of all players
    void displayStandings() const {
        std::cout << "\nCurrent Standings:\n";
        for (const auto& player : players) {
            std::cout << "  - " << player.getName() << ": " << player.getPosition() << "\n";
        }
    }
    
    // Announces the winner
    void displayWinner() const {
        if (winnerIndex != -1) {
            std::cout << "\n====================================\n";
            std::cout << "🏆 CONGRATULATIONS " << players[winnerIndex].getName() << "! YOU WIN! 🏆\n";
            std::cout << "====================================\n";
            std::cout << "Thank you for playing!\n";
        }
    }

public:
    // Public method to start and run the game
    void run() {
        setup();
        
        while (winnerIndex == -1) {
            takeTurn();
            displayStandings();

            // Cycle the player queue for the next turn
            playerTurnQueue.push(playerTurnQueue.front());
            playerTurnQueue.pop();

            pause(1000);
        }

        displayWinner();
    }
};


//================================================================
// Main Function
// Creates a Game object and runs it.
//================================================================
int main() {
    Game diceRace;
    diceRace.run();
    return 0;
}
