#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <getopt.h>
#include <numeric>
#include <iomanip>
#include <sstream>
#include <random>

#include "yahtzee.hpp"

using namespace std;

// compilation command: g++ -std=c++11 -g -Wno-c++11-extensions -Wno-switch-enum -o yahtzee yahtzee.cpp

// have to fix error checking for when getting scores

void Yahtzee::get_args(int argc, char* argv[]) {
    opterr = false;
    int choice;
    int index = 0;
    option long_options[] = {
        { "player1", required_argument, nullptr, '1'},
        { "player2", required_argument, nullptr, '2'},
        { "help", no_argument, nullptr, 'h'},
        { nullptr, 0, nullptr, '\0'}
    };  // long_options[]

    while ((choice = getopt_long(argc, argv, "1:2:h", long_options, &index)) != -1) {
        switch(choice){
            case '1':
                p1_name = optarg;
                break;

            case '2':
                p2_name = optarg;
                break;

            case 'h':
                cout << "Usage: " << argv[0] << " -1 [Name] | -2 [Name] | -h\n";
                cout << "This is a fun version of the classic game of Yahtzee!\n";

            default:
                cerr << "Error: invalid option";
                exit(1);
        } // switch choice
    } // while
} // get_args()

Yahtzee::Yahtzee(int argc, char* argv[]) : rng(random_device{}()), dice_dist(1, 6){
    get_args(argc, argv);
    p1_score.resize(13, -1);
    p2_score.resize(13, -1);
} // Yahtzee()

int Yahtzee::roll_dice() {
    return dice_dist(rng);
} // roll_dice()

void Yahtzee::print_score() {
    const int COL_WIDTH = 20;
    const int ROW_WIDTH = 5;

    cout << "Scoreboard:\n";
    cout << left << setw(ROW_WIDTH) << "Row"  
              << setw(COL_WIDTH) << "Category"
              << setw(COL_WIDTH + 5) << p1_name
              << setw(COL_WIDTH) << p2_name << "\n";
    cout << string(ROW_WIDTH + 2 * COL_WIDTH + 5 + COL_WIDTH, '-') << "\n";

    for (size_t i = 0; i < NUM_ROUNDS; ++i) {
        cout << left << setw(ROW_WIDTH) << i  
                  << setw(COL_WIDTH) << to_string(static_cast<Category>(i))
                  << setw(COL_WIDTH + 5) << (p1_score[i] == -1 ? "X" : to_string(p1_score[i]))  
                  << setw(COL_WIDTH) << (p2_score[i] == -1 ? "X" : to_string(p2_score[i]))
                  << "\n";
    }

    cout << string(ROW_WIDTH + 2 * COL_WIDTH + 5 + COL_WIDTH, '-') << "\n";
} // print_score()

int Yahtzee::get_score(int player, int category, vector<int>& dice) {
    int score = 0;
    vector<int> counts(6, 0);
    for(size_t i = 0; i < dice.size(); ++i) {
        ++counts[dice[i] - 1];
    }
    switch(category) {
        case static_cast<int>(Category::ONES):
            score = count(dice.begin(), dice.end(), 1);
            break;
        case static_cast<int>(Category::TWOS):
            score = count(dice.begin(), dice.end(), 2) * 2;
            break;
        case static_cast<int>(Category::THREES):
            score = count(dice.begin(), dice.end(), 3) * 3;
            break;
        case static_cast<int>(Category::FOURS):
            score = count(dice.begin(), dice.end(), 4) * 4;
            break;
        case static_cast<int>(Category::FIVES):
            score = count(dice.begin(), dice.end(), 5) * 5;
            break;
        case static_cast<int>(Category::SIXES):
            score = count(dice.begin(), dice.end(), 6) * 6;
            break;
        case static_cast<int>(Category::THREE_OF_A_KIND):
            for(size_t i = 0; i < counts.size(); ++i) {
                if(counts[i] >= 3) {
                    score = accumulate(dice.begin(), dice.end(), 0);
                    break;
                }
            }
            break;
        case static_cast<int>(Category::FOUR_OF_A_KIND):
            for(size_t i = 0; i < counts.size(); ++i) {
                if(counts[i] >= 4) {
                    score = accumulate(dice.begin(), dice.end(), 0);
                    break;
                }
            }
            break;
        case static_cast<int>(Category::FULL_HOUSE): {
            bool two = false, three = false;
            for(size_t i = 0; i < counts.size(); ++i) {
                if(counts[i] == 2) {
                    two = true;
                } else if(counts[i] == 3) {
                    three = true;
                }
            }
            if(two && three) {
                score = 25;
            }
            break;
        }
        case static_cast<int>(Category::SMALL_STRAIGHT): {
            int consecutive = 0;
            for(size_t i = 0; i < counts.size(); ++i) {
                if(counts[i] > 0) {
                    ++consecutive;
                } else {
                    consecutive = 0;
                }
                if(consecutive == 4) {
                    score = 30;
                    break;
                }
            }
            break;
        }
        case static_cast<int>(Category::LARGE_STRAIGHT):
            if(!(counts[0] == 0 && counts[5] == 0)) {
                score = 40;
            }
            break;
        case static_cast<int>(Category::YAHTZEE):
            for(size_t i = 0; i < counts.size(); ++i) {
                if(counts[i] == 5) {
                    score = 50;
                    break;
                }
            }
            break;
        case static_cast<int>(Category::CHANCE):
            score = accumulate(dice.begin(), dice.end(), 0);
            break;
        default:
            std::cerr << "Error: invalid category";
            exit(1);
    }
    return score;
} // get_score()

void Yahtzee::play_turn(int player) {
    vector<int> dice(5);
    for(size_t i = 0; i < NUM_ROLLS; ++i) {
        cout << "Roll " << i + 1 << "\n";
        if(i == 0){
            for(size_t j = 0; j < 5; ++j) {
                dice[j] = roll_dice();
            }
        }
        sort(dice.begin(), dice.end());
        for(size_t j = 0; j < 5; ++j) {
            cout << dice[j] << " ";
        }
        cout << "\n";
        if(i < NUM_ROLLS - 1) {
            cout << "Enter dice to reroll (1-5) separated by spaces: ";
            cin.ignore();
            string input;
            getline(cin, input);

            stringstream ss(input);
            int rerollIndex;
            while (ss >> rerollIndex) { 
                if (rerollIndex >= 1 && rerollIndex <= 5) {
                    dice[rerollIndex - 1] = roll_dice();
                }
            }
        }
    }
    cout << "Enter Category (0-12): ";
    int category;
    cin >> category;
    bool updated = false;
    while(!updated) {
        if(player == 1){
            if(p1_score[category] != -1) {
                cout << "Category already used\n";
            } else {
                p1_score[category] = get_score(player, category, dice);
                updated = true;
            }
        }
        else {
            if(p2_score[category] != -1) {
                cout << "Category already used\n";
            } else {
                p2_score[category] = get_score(player, category, dice);
                updated = true;
            }
        }
    }
} // play_turn()

void Yahtzee::play_game() {
    for(size_t i = 0; i < NUM_ROUNDS; ++i) {
        print_score();
        cout << "Round " << i + 1 << "\n";
        cout << p1_name << "'s turn\n";
        play_turn(1);
        cout << p2_name << "'s turn\n";
        play_turn(2);
    }
    print_score();
    int p1_total = accumulate(p1_score.begin(), p1_score.end(), 0), p2_total = accumulate(p2_score.begin(), p2_score.end(), 0);
    if(accumulate(p1_score.begin(), p1_score.begin() + 5, 0) >= BONUS_THRESHOLD) {
        cout << p1_name << " gets 35 bonus points!\n";
        p1_total += BONUS_SCORE;
    }
    if(accumulate(p2_score.begin(), p2_score.begin() + 5, 0) >= BONUS_THRESHOLD) {
        cout << p2_name << " gets 35 bonus points!\n";
        p2_total += BONUS_SCORE;
    }
    cout << "Final Score:\n";
    cout << p1_name << ": " << p1_total << "\n";
    cout << p2_name << ": " << p2_total << "\n";
    if(p1_total > p2_total) {
        cout << p1_name << " wins!\n";
    } else if(p2_total > p1_total) {
        cout << p2_name << " wins!\n";
    } else {
        cout << "It's a tie!\n";
    }
} // play_game()

int main(int argc, char* argv[]){
    std::ios_base::sync_with_stdio(false);

    Yahtzee yahtzee = Yahtzee(argc, argv);
    yahtzee.play_game();

    return 0;
} // main()