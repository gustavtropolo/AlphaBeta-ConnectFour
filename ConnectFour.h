#ifndef CONNECTFOUR_H
#define CONNECTFOUR_H

#include <vector>
#include <array>

class ConnectFour{
    public:
        //TranspositionTable transpositionTable;
        int nodeCount = 0;
        int maxDepth = 100;
        char computerToken = 'O';
        char playerToken = 'X';
        int bestMove = -1;
        bool isPlayerTurn = true;
        static const int HEIGHT = 6;
        static const int WIDTH = 7;
        int currDepth = 10;
        int moveOrder[7] = {3, 2, 4, 1, 5, 0, 6};
        int rowArray[7] = {HEIGHT-1, HEIGHT-1, HEIGHT-1, HEIGHT-1, HEIGHT-1, HEIGHT-1, HEIGHT-1};
        std::vector< std::vector <char> > board;
        std::vector<int> moves;
        u_int64_t mask;
        u_int64_t current_position;
        static u_int64_t bottomMask;
        static u_int64_t topMask;
        //int moves = 42;

        bool isTie();
        bool isWin(char token);
        bool isLegalMove(int col);
        void makeMove(int col, bool isMaximising);
        void printBoard();
        void takeTurnAI();
        int miniMax(int depth, bool isMaximising, int alpha, int beta);
        void undoMove(int col);
        void undoPlayerMove();
        void resetVarialbesForAI();
        int getAImove();
        int negaMax(int depth, bool isMaximising, int alpha, int beta);
        static void initTopAndBottomMask();
        bool canPlay(int col) const;
        void play(int col);
        u_int64_t getPosition(u_int64_t currentPlayer, u_int64_t mask);
        static bool isWin(u_int64_t pos);

        ConnectFour(){
            for(int row = 0; row < HEIGHT; row++){
                board.push_back({});
                for(int col = 0; col < WIDTH; col++){
                    board[row].push_back('.');
                }
            }
        }

        ~ConnectFour(){};
};

#endif