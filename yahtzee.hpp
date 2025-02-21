#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <getopt.h>
#include <numeric>
#include <iomanip>
#include <sstream>
#include <random>

enum class Category : int {
    ONES = 0,
    TWOS = 1,
    THREES = 2,
    FOURS = 3,
    FIVES = 4,
    SIXES = 5,
    THREE_OF_A_KIND = 6,
    FOUR_OF_A_KIND = 7,
    FULL_HOUSE = 8,
    SMALL_STRAIGHT = 9,
    LARGE_STRAIGHT = 10,
    YAHTZEE = 11,
    CHANCE = 12
}; // Category

std::string to_string(Category c) {
    switch (static_cast<int>(c)) {
        case static_cast<int>(Category::ONES):
            return "Ones";
        case static_cast<int>(Category::TWOS):
            return "Twos";
        case static_cast<int>(Category::THREES):
            return "Threes";
        case static_cast<int>(Category::FOURS):
            return "Fours";
        case static_cast<int>(Category::FIVES):
            return "Fives";
        case static_cast<int>(Category::SIXES):
            return "Sixes";
        case static_cast<int>(Category::THREE_OF_A_KIND):
            return "Three of a Kind";
        case static_cast<int>(Category::FOUR_OF_A_KIND):
            return "Four of a Kind";
        case static_cast<int>(Category::FULL_HOUSE):
            return "Full House";
        case static_cast<int>(Category::SMALL_STRAIGHT):
            return "Small Straight";
        case static_cast<int>(Category::LARGE_STRAIGHT):
            return "Large Straight";
        case static_cast<int>(Category::YAHTZEE):
            return "Yahtzee";
        case static_cast<int>(Category::CHANCE):
            return "Chance";
        default:
            std::cerr << "Error: invalid category";
            exit(1);
    }
} // to_string()

class Yahtzee {
    std::string p1_name = "Player 1"; // default names
    std::string p2_name = "Player 2";
    std::vector<int> p1_score; // score vectors
    std::vector<int> p2_score;

    const int BONUS_THRESHOLD = 63;
    const int BONUS_SCORE = 35;
    const int NUM_ROLLS = 3;
    const int NUM_ROUNDS = 13;

    std::mt19937 rng; // random number generator
    std::uniform_int_distribution<int> dice_dist; // dice distribution
    int roll_dice();
    
    void print_score();
    void get_args(int argc, char* argv[]);
    void play_turn(int player);
    int get_score(int player, int category, std::vector<int>& dice);
public:
    Yahtzee(int argc, char* argv[]);
    void play_game();
}; // Yahtzee