#include "./userOp.h"
#include "./piece.h"
#include <sys/types.h>
#include <unistd.h>
#include <ncurses.h>

void UserOp::order(int key,Piece* p){
    if(key == KEY_LEFT){
        moveLeft(p);
    }
    else if(key == KEY_RIGHT){
        moveRight(p);
    }
    else if(key == KEY_DOWN){
        moveDown(p);
    }
    else if(key == KEY_UP){
        rotate(p);
    }
}

void UserOp::moveLeft(Piece* p){
    int head_x = p->getHeadX()-1;
    p->setHeadX(head_x);
}

void UserOp::moveRight(Piece* p){
    int head_x = p->getHeadX()+1;
    p->setHeadX(head_x);
}

void UserOp::moveDown(Piece* p){
    int head_y = p->getHeadY()+1;
    p->setHeadY(head_y);
}

void UserOp::rotate(Piece* p){
    bool temp[PieceSize][PieceSize] = {false};

    for(int i=0;i<p->getOccupyHeight();i++)
        for(int j=0;j<p->getOccupyWidth();j++)
            temp[j][i]=p->isBoxExist(i,j);

    int tmp = p->getOccupyHeight();
    p->setOccupyHeight(p->getOccupyWidth());
    p->setOccupyWidth(tmp);

    for(int i=0;i<p->getOccupyHeight();i++)
        for(int j=0;j<p->getOccupyWidth();j++)
            p->setBoxExist(i,p->getOccupyWidth()-1-j,temp[i][j]);
}
