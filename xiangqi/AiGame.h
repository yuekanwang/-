#ifndef AIGAME_H
#define AIGAME_H

#include"Step.h"
#include"Gamescene.h"

class AiGame : public Gamescene
{
public:
    int lastrow;
    int lastcol;

    virtual void StoneMove(int row,int col);//移动函数
    Step AiMove();
    void getAllcanMove(QVector<Step> stones);
    void fakeMove(Step stone);
    void unfakeMove(Step stone);
    int getScore();
    void saveStone(int id,int killid,int row,int col,QVector<Step> stone);
};

#endif // AIGAME_H
