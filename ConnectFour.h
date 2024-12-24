#ifndef CONNECTFOUR_H
#define CONNECTFOUR_H

#include <vector>
#include <array>

using namespace std;

class ConnectFour{
    public:
        const char computerToken = 'O';
        const char playerToken = 'X';
        int bestMove = -1;
        bool isPlayerTurn = true;
        static const int HEIGHT = 6;
        static const int WIDTH = 7;
        int currDepth = 10;
        int moveOrder[7] = {3, 2, 4, 1, 5, 0, 6};
        static uint64_t mask;
        static uint64_t currPlayer;
        static uint64_t noBottomRow;
        vector<vector<char>> board;
        static int maxDepth;
        static int moves;
        static bool is_win;

        bool isTie();
        bool isWin();
        int getAImove();
        void printBoard();
        void makeMove(int col);
        void undoMove(int col);
        bool isLegalMove(int col);
        int negaMax(int depth, bool isMaximising, int alpha, int beta);

        ConnectFour(){
            moves = 0;
            maxDepth = 10;
            currPlayer = 0;
            is_win = false;
            mask = 0b0000'0000'0000'0000'0000'0100'0000'1000'0001'0000'0010'0000'0100'0000'1000'0001;
            noBottomRow = 0b0000'0000'0000'0001'1111'1011'1111'0111'1110'1111'1101'1111'1011'1111'0111'1110;
            for (int row = 0; row < 6; row++) {
                board.push_back({}); //an empty row
                for (int col = 0; col < 7; col++) {
                    board[row].push_back('.');
                }
            }
        }

        ~ConnectFour(){};
};

#endif