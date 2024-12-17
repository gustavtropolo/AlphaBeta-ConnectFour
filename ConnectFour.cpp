#include "ConnectFour.h"
#include <iostream>
#include <vector>
#include <chrono>
#include <cstdint>
#include <unordered_set>

// void ConnectFour::initTopAndBottomMask() {
//     uint64_t number = 0;
//     for (int i = 0; i <= 42; i += 7) {
//         number |= (uint64_t(1) << i);  // Set the ith bit to 1
//     }
//     bottomMask = number;

//     number = 0;
//     for (int i = 6; i <= 48; i += 7) {
//         number |= (uint64_t(1) << i);  // Set the ith bit to 1
//     }
//     topMask = number;
// }

// bool ConnectFour::canPlay(int col) const {
//     return mask & topMask == 0; //check that the top is empty
// }

// void ConnectFour::play(int col) {
//     mask |= mask + bottomMask;
//     current_position ^= mask; //switch to the other players filled cells
//     moves--;
// }

// u_int64_t ConnectFour::getPosition(u_int64_t currentPlayer, u_int64_t mask) {
//     return currentPlayer + mask + bottomMask;
// }

// bool ConnectFour::isWin(u_int64_t pos) {
//     //check rows
//     u_int64_t m = pos & (pos << (HEIGHT + 1));
//     if (m & (m << 2 * (HEIGHT + 1))) {
//         return true;
//     }
//     // check columns
//     m = pos & (pos << 1);
//     if (m & (m << 2)) {
//         return true;
//     }
//     //check up right diagonals
//     m = pos & (pos << HEIGHT + 2);
//     if (m & (m << 2 * (HEIGHT + 2))) {
//         return true;
//     }
//     //check down left diagonals
//     m = pos & (pos << HEIGHT);
//     if (m & (m << 2 * HEIGHT)) {
//         return true;
//     }

//     return false;
// }

// void translateToArrayBoard(isPlayersTurn) {
//     u_int64_t opponentsPieces = mask - currentPlayer;
//     if (isPlayersTurn) {  //the currentplayer's pieces are the players
        
//     }

// }

bool ConnectFour::isTie(){
    return false;
}

bool ConnectFour::isWin(char token){
    int counter = 0;
    //checks rows for a win
    for(int row = 0; row < HEIGHT; row++){
    for(int col = 0; col < WIDTH; col++){
        if(board.at(row).at(col) == token){
        counter++;
        if(counter >= 4){
            return true;
        }
        }else{
        counter = 0;
        }
    }
    counter = 0;
    }
    counter = 0;
    //checks cols for a win
    for(int col = 0; col < WIDTH; col++){
    for(int row = 0; row < HEIGHT; row++){
        if(board.at(row).at(col) == token){
        counter++;
        if(counter >= 4){
            return true;
        }
        }else{
        counter = 0;
        }
    }
    counter = 0;
    }
    counter = 0;
    //checks diagonals for a win (down and to the right)
    int row = HEIGHT-4, x = -1, y = -1;
    for(int col = 0; col <= 3; col++){
    if(row < 0){
        row = 0;
    }
    while(row >= 0){
        x = col;
        y = row;
        while(x < WIDTH && y < HEIGHT){
        if(board[y][x] == token){
            counter++;
            if(counter >= 4){
            return true;
            }
        }else{
            counter = 0;
        }
        x++;
        y++;
        }
        counter = 0;
        row--;
    }
    counter = 0;
    }
    counter = 0;
    //checks diagonals for a win (down and to the left)
    row = HEIGHT-4;
    for(int col = WIDTH-1; col >=3; col--){
    if(row < 0){
        row = 0;
    }
    while(row >= 0){
        x = col;
        y = row;
        while(x >= 0 && y < HEIGHT){
        if(board[y][x] == token){
            counter++;
            if(counter >= 4){
            return true;
            }
        }else{
            counter = 0;
        }
        x--;
        y++;
        }
        counter = 0;
        row--;
    }
    counter = 0;
    }
    return false;
}

bool ConnectFour::isLegalMove(int col){
    if(col > 6 || col < 0){
    return false;
    }else if(rowArray[col] < 0){
    return false;
    }
    return true;
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

void ConnectFour::undoPlayerMove(){
    board[rowArray[moves[moves.size()-1]]+1] [moves[moves.size()-1]] = '.';
    rowArray[moves[moves.size()-1]] += 1;
    moves.pop_back();
    std::cout << std::endl;
    board[rowArray[moves[moves.size()-1]]+1] [moves[moves.size()-1]] = '.';
    rowArray[moves[moves.size()-1]] += 1;
    moves.pop_back();
    std::cout << std::endl;
    this->printBoard();
    std::cout << "Move undone." << std::endl;
}

int ConnectFour::getAImove() {
    int bestScore = -999;
    int bestMove = -1;
    nodeCount = 0;
    for (int col : moveOrder) {
        if (board[0][col] != '.') {
            continue;
        }
        makeMove(col, true);
        if (isWin('O')) {
            //undoMove(col);
            //return col; //return this winning move
        }
        int score = -negaMax(currDepth - 1, false, -999, 999);
        nodeCount++;
        undoMove(col);

        std::cout << score << std::endl;

        if (score > bestScore) {
            bestScore = score;
            bestMove = col;
            if (score > 0) {
            maxDepth = 999 - score;
            break;
            }
        }
    }
    std::cout << "Node count: " << nodeCount << std::endl;
    // std::cout << "Current depth: " << currDepth << std::endl;
    return bestMove;
}

int ConnectFour::negaMax(int depth, bool isMaximising, int alpha, int beta) {
    if (depth == 0) {
        return 0;
    }
    if (isMaximising ? isWin('X') : isWin('O')) { //ask if the other player has won on their last move
        return -1000 + (currDepth - depth); // delay losing as much as possible
    }
    for (int col : moveOrder) {
        if (board[0][col] != '.') {
            continue;
        }
        makeMove(col, isMaximising);
        int score = -negaMax(depth - 1, !isMaximising, -999, -alpha);
        nodeCount++;
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

void ConnectFour::makeMove(int col, bool isMaximising) {
    board[rowArray[col]][col] = isMaximising? 'O' : 'X';
    rowArray[col]--;
}

void ConnectFour::undoMove(int col) {
    board[rowArray[col] + 1][col] = '.';
    rowArray[col]++;
}