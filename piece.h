#ifndef _PIECE_H_
#define _PIECE_H_

#include <random>

const int PieceSize = 4;

extern const int GameWinHeight;
extern const int GameWinWidth;

class Piece{
    public:
        Piece();
        ~Piece();

        void initBox();

        int getHeight();
        int getWidth();
        bool isBoxExist(int x,int y);
        void setBoxExist(int i,int j,bool flag);
        int getOccupyHeight();
        int getOccupyWidth();
        void setOccupyHeight(int height);
        void setOccupyWidth(int width);

        int getHeadX();
        int getHeadY();
        void setHeadX(int x);
        void setHeadY(int y);

    private:
        bool box[PieceSize][PieceSize];
        int height_;
        int width_;
        int height_occupy_;
        int width_occupy_;
        int head_x_;
        int head_y_;
};

#endif
