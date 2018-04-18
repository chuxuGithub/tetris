#include "./piece.h"
#include "./tetris.h"
#include <ctime>

Piece::Piece(){
    height_ = PieceSize;
    width_ = PieceSize;

    for(int i=0;i<PieceSize;i++)
        for(int j=0;j<PieceSize;j++)
            box[i][j] = false;

    initBox();
}
Piece::~Piece(){

}

void Piece::initBox(){
    std::random_device rd;
    std::uniform_int_distribution<unsigned> u(0,6);
    std::default_random_engine e(rd());

    switch(u(e)){
        case 0:
            height_occupy_ = 1;
            width_occupy_ = 4;
            box[0][0] = true;
            box[0][1] = true;
            box[0][2] = true;
            box[0][3] = true;
            break;
        case 1:
            height_occupy_ = 2;
            width_occupy_ = 3;
            box[0][0] = true;
            box[1][0] = true;
            box[1][1] = true;
            box[1][2] = true;
            break;
        case 2:
            height_occupy_ = 2;
            width_occupy_ = 3;
            box[0][2] = true;
            box[1][0] = true;
            box[1][1] = true;
            box[1][2] = true;
            break;
        case 3:
            height_occupy_ = 2;
            width_occupy_ = 3;
            box[0][1] = true;
            box[0][2] = true;
            box[1][0] = true;
            box[1][1] = true;
            break;
        case 4:
            height_occupy_ = 2;
            width_occupy_ = 3;
            box[0][0] = true;
            box[0][1] = true;
            box[1][1] = true;
            box[1][2] = true;
            break;
        case 5:
            height_occupy_ = 2;
            width_occupy_ = 2;
            box[0][0] = true;
            box[0][1] = true;
            box[1][0] = true;
            box[1][1] = true;
            break;
        case 6:
            height_occupy_ = 2;
            width_occupy_ = 3;
            box[0][1] = true;
            box[1][0] = true;
            box[1][1] = true;
            box[1][2] = true;
            break;
    }

    head_x_ = GameWinWidth/2;
    head_y_ = 1;
}

int Piece::getHeight(){
    return PieceSize;
}

int Piece::getWidth(){
    return PieceSize;
}

bool Piece::isBoxExist(int x,int y){
    return box[x][y];
}

void Piece::setBoxExist(int i,int j,bool flag){
    box[i][j] = flag;
}

int Piece::getOccupyHeight(){
    return height_occupy_;
}

int Piece::getOccupyWidth(){
    return width_occupy_;
}

void Piece::setOccupyHeight(int height){
    height_occupy_ = height;
}

void Piece::setOccupyWidth(int width){
    width_occupy_ = width;
}

int Piece::getHeadX(){
    return head_x_;
}

int Piece::getHeadY(){
    return head_y_;
}

void Piece::setHeadX(int x){
    head_x_ = x;
}

void Piece::setHeadY(int y){
    head_y_ = y;
}
