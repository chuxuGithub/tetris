#ifndef _USEROP_H_
#define _USEROP_H_

class Piece;

extern const int PieceSize;

class UserOp{
    public:
        void order(int key,Piece* p);
        void moveLeft(Piece* p);
        void moveRight(Piece* p);
        void moveDown(Piece* p);
        void rotate(Piece* p);
};

#endif
