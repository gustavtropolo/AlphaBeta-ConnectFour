#include "ConnectFour.h"
#include <iostream>
#include <vector>
#include <chrono>
#include <cstdint>
#include <unordered_set>

bool ConnectFour::isTie(){
    return ConnectFour::moves >= 42;
}

bool ConnectFour::isWin() {
    uint64_t temp = ConnectFour::currPlayer;
    temp = (temp << 2) & temp;
    temp = (temp << 1) & temp;
    bool vertWin = temp;

    temp = ConnectFour::currPlayer;
    temp = (temp << 14) & temp;
    temp = (temp << 7) & temp;
    bool horizWin = temp;

    temp = ConnectFour::currPlayer;
    temp = (temp << 16) & temp; //up to the left
    temp = (temp << 8) & temp;
    bool diagWin = temp;
    temp = ConnectFour::currPlayer;
    temp = (temp << 12) & temp; //down to the left
    temp = (temp << 6) & temp;
    diagWin = temp || diagWin;

    return vertWin || horizWin || diagWin;
}

bool ConnectFour::isLegalMove(int col) {
    return !(mask & (uint64_t(1) << ((col * 7) + 6)));
}

void ConnectFour::printBoard() {
    std::cout << " 1  2  3  4  5  6  7" << std::endl;
    for(int row = 0; row < HEIGHT; row++){
        for(int col = 0; col < WIDTH; col++){
            std::cout << " " << board[row][col] << " ";
        }
        std::cout << std::endl;
    }
}

void ConnectFour::makeMove(int col) { //returns currP but later will just update globally
    using C = ConnectFour; 
    uint64_t allFilled = ((C::mask & C::noBottomRow) >> 1);                     //get the filled tiles
    uint64_t otherPlayer = C::currPlayer ^ allFilled;                                     //get the other player
    C::mask = C::mask | (C::mask + (uint64_t(1) << (7 * (6 - col)))); //update the mask
    allFilled = ((C::mask & C::noBottomRow) >> 1);                              //update the filled tiles
    C::currPlayer = allFilled - otherPlayer;                                              //update the position 
    C::is_win = isWin();                                                                  //check for a win
    C::currPlayer = otherPlayer;                                                          //switch players
}

void ConnectFour::undoMove(int col) {
    using C = ConnectFour; 
    C::is_win = false;
    C::currPlayer = currPlayer ^ ((C::mask & C::noBottomRow) >> 1); //switch back to last player
    uint64_t newCol = (mask & (0b1111110 << (7 * (6 - col)))) >> 1; //gives our updated column
    mask = mask & ~(0b1111111 << (7 * (6 - col)));                  //remove the column we are replacing
    mask = mask | newCol;                                           //put the new column in
}

int ConnectFour::getAImove() {
    int bestScore = -999;
    int bestMove = -1;
    for (int col : moveOrder) {
        if (!isLegalMove(col)) {
            continue;
        }
        makeMove(col);
        int score = -negaMax(currDepth - 1, false, -999, 999);
        undoMove(col);

        std::cout << score << std::endl;

        if (score > bestScore) {
            bestScore = score;
            bestMove = col;
        }
    }
    return bestMove;
}

int ConnectFour::negaMax(int depth, bool isMaximising, int alpha, int beta) {
    if (depth == 0) {
        return 0;
    }
    if (ConnectFour::is_win) { //ask if the other player has won on their last move
        return -1000 + (currDepth - depth); // delay losing as much as possible
    }
    for (int col : moveOrder) {
        if (!isLegalMove(col)) {
            continue;
        }
        makeMove(col);
        int score = -negaMax(depth - 1, !isMaximising, -999, -alpha);
        undoMove(col);

        if (score > alpha) {
            alpha = score;
            if (alpha >= beta) {
                break;
            }
        }
    }
    return alpha;
}
