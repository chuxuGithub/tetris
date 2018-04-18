#ifndef _TETRIS_H_
#define _TETRIS_H_

#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>
#include <memory>

const int GameWinHeight = 25;
const int GameWinWidth = 40;

const int HintWinHeight = 10;
const int HintWinWidth = 15;

class Piece;

class Tetris
{
    public:
       Tetris();
       ~Tetris();

       void initInterface();

       void start();

       void displayScore();
       void displayNextPiece();
       void generateNextPiece();
       bool isGameOver();
       bool isCrossBorderOrDoublication();

       void goNext();
       void judgeScore();
       bool judgeLine(int row);

    private:
       bool box_exist_[GameWinHeight][GameWinWidth];

       WINDOW* game_win_;
       WINDOW* hint_win_;
       WINDOW* score_win_;

       int score;

       bool game_over_;

       //std::shared_ptr<Piece> ppiece_cur;
      //std::shared_ptr<Piece> ppiece_next;
      Piece* ppiece_cur;
      Piece* ppiece_next;
};

#endif
