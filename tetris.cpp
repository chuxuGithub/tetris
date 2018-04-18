#include "./tetris.h"
#include "./piece.h"
#include <sys/epoll.h>
#include "./userOp.h"

Tetris::Tetris():score(0),game_over_(false){
    //ppiece_cur = std::make_shared<Piece>();
    //ppiece_next = std::make_shared<Piece>();
    ppiece_cur = new Piece();
    ppiece_next = new Piece();

    for(int i=0; i<GameWinHeight;i++)
        for(int j=0; j<GameWinWidth;j++)
        {
            if(0==i || GameWinHeight-1==i || 0==j || GameWinWidth-1==j)
                box_exist_[i][j] = true;
            else
                box_exist_[i][j] = false;
        }
    initInterface();

    displayScore();
    displayNextPiece();
}

WINDOW* create_newwin(int height,int width,int starty,int startx){
    WINDOW* local_win = newwin(height,width,starty,startx);
    box(local_win,0,0);
    wrefresh(local_win);
    return local_win;
}

void destory_win(WINDOW* local_win){
    wborder(local_win,' ',' ',' ',' ',' ',' ',' ',' ');
    wrefresh(local_win);
    delwin(local_win);
}

void Tetris::initInterface(){
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr,TRUE);

    refresh();

    game_win_ = create_newwin(GameWinHeight,GameWinWidth,0,0);
    wborder(game_win_,'*','*','*','*','*','*','*','*');
    wrefresh(game_win_);

    hint_win_ = create_newwin(HintWinHeight,HintWinWidth,0,GameWinWidth+10);
    mvprintw(0,GameWinWidth+10+2,"%s","Next");
    refresh();

    score_win_ = create_newwin(HintWinHeight,HintWinWidth,15,GameWinWidth+10);
    mvprintw(15,GameWinWidth+10+2,"%s","Score");
    refresh();


}

void Tetris::start(){
    int epoll_fd;
    struct epoll_event ev,events[1];

    epoll_fd = epoll_create(2);

    ev.data.fd = 0;
    ev.events = EPOLLIN;

    epoll_ctl(epoll_fd,EPOLL_CTL_ADD,0,&ev);

    int nfds;
    while(1){
        if(game_over_){
            break;
        }

        if((nfds=epoll_wait(epoll_fd,events,1,500)) == 0){
            int head_y = ppiece_cur->getHeadY()+1;
            ppiece_cur->setHeadY(head_y);

            if(isCrossBorderOrDoublication()){
                ppiece_cur->setHeadY(head_y-1);
                for(int i=0;i<ppiece_cur->getOccupyHeight();i++)
                    for(int j=0;j<ppiece_cur->getOccupyWidth();j++)
                        if(ppiece_cur->isBoxExist(i,j))
                            box_exist_[ppiece_cur->getHeadY()+i][ppiece_cur->getHeadX()+j] = true;
                goNext();
            }
            else{
                for(int i=ppiece_next->getOccupyHeight()-1;i>=0;i--)
                    for(int j=0;j<ppiece_cur->getOccupyWidth();j++){
                        if(ppiece_cur->isBoxExist(i,j)){
                            mvwaddch(game_win_,ppiece_cur->getHeadY()-1+i,ppiece_cur->getHeadX()+j,' ');
                            mvwaddch(game_win_,ppiece_cur->getHeadY()+i,ppiece_cur->getHeadX()+j,'#');
                        }
                    }
                wrefresh(game_win_);
            }
        }
        else{
            int key;
            //while((key=getch()) == -1);
            key = getch();
            Piece piece_tmp(*ppiece_cur);

            UserOp user_op;
            user_op.order(key,ppiece_cur);

            if(isCrossBorderOrDoublication()){
                *ppiece_cur = piece_tmp;

                if(key == KEY_DOWN){
                    for(int i=0;i<ppiece_cur->getOccupyHeight();i++)
                        for(int j=0;j<ppiece_cur->getOccupyWidth();j++)
                            if(ppiece_cur->isBoxExist(i,j))
                                box_exist_[ppiece_cur->getHeadY()+i][ppiece_cur->getHeadX()+j] = true;
                    goNext();
                }
            }
            else{
                for(int i=0;i<piece_tmp.getOccupyHeight();i++)
                    for(int j=0;j<piece_tmp.getOccupyWidth();j++)
                        if(piece_tmp.isBoxExist(i,j)){
                            mvwaddch(game_win_,piece_tmp.getHeadY()+i,piece_tmp.getHeadX()+j,' ');
                        }
                wrefresh(game_win_);

                for(int i=0;i<ppiece_cur->getOccupyHeight();i++)
                    for(int j=0;j<ppiece_cur->getOccupyWidth();j++)
                        if(ppiece_cur->isBoxExist(i,j)){
                            mvwaddch(game_win_,ppiece_cur->getHeadY()+i,ppiece_cur->getHeadX()+j,'#');
                        }

                wrefresh(game_win_);
            }
        }
    }
}

void Tetris::displayScore(){
    mvwprintw(score_win_,HintWinHeight/2,HintWinWidth/2-2,"%d",score);
    wrefresh(score_win_);
}

void Tetris::displayNextPiece(){
    for(int i=1;i<HintWinHeight-1;i++)
        for(int j=1;j<HintWinWidth-1;j++){
            mvwaddch(hint_win_,i,j,' ');
            wrefresh(hint_win_);
        }

    for(int i=0;i<ppiece_next->getOccupyHeight();i++)
        for(int j=0;j<ppiece_next->getOccupyWidth();j++)
            if(ppiece_next->isBoxExist(i,j)){
                mvwaddch(hint_win_,(HintWinHeight-ppiece_next->getOccupyHeight())/2+i,(HintWinWidth-ppiece_next->getOccupyHeight())/2+j,'#');
                wrefresh(hint_win_);
            }
}

void Tetris::generateNextPiece(){
    free(ppiece_cur);
    ppiece_cur = ppiece_next;
    //ppiece_next = std::make_shared<Piece>();
    ppiece_next = new Piece();
}

bool Tetris::isGameOver(){
    if(isCrossBorderOrDoublication()){
        return true;
    }
    return false;
}

bool Tetris::isCrossBorderOrDoublication(){
    for(int i=0;i<ppiece_cur->getOccupyHeight();i++)
        for(int j=0;j<ppiece_cur->getOccupyWidth();j++){
            if(ppiece_cur->isBoxExist(i,j)){
                if(ppiece_cur->getHeadY()+i > GameWinHeight-2)
                    return true;
                if(ppiece_cur->getHeadX()+j < 1 || ppiece_cur->getHeadY()>GameWinWidth-2)
                    return true;
                if(box_exist_[ppiece_cur->getHeadY()+i][ppiece_cur->getHeadX()+j])
                    return true;
            }
        }
    return false;
}

void Tetris::goNext(){
    score += 10;

    judgeScore();
    displayScore();
    generateNextPiece();
    displayNextPiece();

    if(isGameOver()){
        game_over_ = true;
    }
}

void Tetris::judgeScore(){
    bool line_full;
    int line_count=0;

    for(int i=1;i<GameWinHeight-1;i++){
        line_full = true;
        for(int j=0;j<GameWinWidth-1;j++){
            if(!box_exist_[i][j]){
                line_full = false;
                break;
            }
        }

        if(line_full){
            line_count++;
            score+=50;
            for(int j=1;j<GameWinWidth-1;j++){
                box_exist_[i][j] = false;
            }
        }
    }

    if(line_count != 0){
        for(int i=GameWinHeight-2;i>=2;i--){
            int s=i;
            if(!judgeLine(s)){
                while(s>1 && !judgeLine(--s));
                for(int j=1;j<GameWinWidth-1;j++){
                    box_exist_[i][j] = box_exist_[s][j];
                    box_exist_[s][j] = false;
            }
        }
    }
    }

    for(int i=1;i<GameWinHeight-1;i++)
        for(int j=1;j<GameWinWidth-1;j++){
            if(box_exist_[i][j]){
                mvwaddch(game_win_,i,j,'#');
            }
            else{
                mvwaddch(game_win_,i,j,' ');
            }
        }
    wrefresh(game_win_);
}

bool Tetris::judgeLine(int row){
    for(int j=1;j<GameWinWidth-1;j++)
        if(box_exist_[row][j])
            return true;

    return false;
}

Tetris::~Tetris(){
    destory_win(game_win_);
    destory_win(hint_win_);
    destory_win(score_win_);
    system("clear");

    int row,col;
    getmaxyx(stdscr,row,col);
    mvprintw(row/2-4,col/2,"%s","GAME OVER!\n");
    mvprintw(row/2+1,col/2-9,"%s","Wait 3s to return the terminal!\n");
    refresh();

    sleep(3);
    endwin();
}

