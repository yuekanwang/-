#include "AiGame.h"
#include"Step.h"
void AiGame::StoneMove(int row,int col)
{
    Gamescene::StoneMove(row,col);

    if(!this->redtrue)
    {
        AiMove();
    }
}

Step AiGame::AiMove()
{
    //获取所有的可行步骤
    QVector<Step> steps;
    getAllcanMove(steps);

    //尝试走一下
    int maxScore=-100000;
    Step ret;
    for(QVector<Step>::iterator it = steps.begin();it!=steps.end();it++)
    {
        Step steps = *it;
        // int killid = this->getStoneId(stone.row,stone.col);
        // if(canMove(stone.id,killid,stone.row,stone.col))
        // {
        //     this->stone[stone.id].row = stone.row;
        //     this->stone[stone.id].col = stone.col;
        //     this->stone[stone.id].death = false;
        //     if(killid!=-1)
        //     {
        //         this->stone[killid].death = true;
        //     }
        //     break;
        // }
        fakeMove(steps);
        int Score=getScore();
        unfakeMove(steps);

        if(Score>maxScore)
        {
            maxScore=Score;
            ret=steps;
        }
    }

    return ret;
}

void AiGame::getAllcanMove(QVector<Step> steps)
{
    //获取所有的可行步骤
    steps.clear();
    for(int i=16;i<32;i++)
    {
        if(this->stone[i].death) continue;
        lastrow = this->stone[i].row;
        lastcol = this->stone[i].col;

        for(int row=0;row<=9;row++)
        {
            for(int col=0;col<=8;col++)
            {
                int killid = this->getStoneId(row,col);
                if(killid==-1)//没子
                {
                    if(canMove(i,killid,row,col))
                    {
                        saveStone(i,killid,row,col,steps);
                    }
                }
                else
                {
                    if(canMove(i,killid,row,col))
                    {

                        saveStone(i,killid,row,col,steps);

                    }
                }
            }
        }

    }
}

void AiGame::saveStone(int id,int killid,int row,int col,QVector<Step> steps)
{
    Step _stone;
    _stone._id=id;
    _stone._killid=killid;
    _stone.row=row;
    _stone.col=col;
    _stone.lastrow=lastrow;
    _stone.lastcol=lastcol;
    steps.push_back(_stone);
}


void AiGame::fakeMove(Step _stone)
{
    if(_stone._killid!=-1)
    {
        this->stone[_stone._killid].death = true;
    }
    this->stone[_stone._id].row = _stone.row;
    this->stone[_stone._id].col = _stone.col;

}

void AiGame::unfakeMove(Step _stone)
{
    if(_stone._killid!=-1)
    {
        this->stone[_stone._killid].death = false;
    }
    this->stone[_stone._id].row = _stone.lastrow;
    this->stone[_stone._id].col = _stone.lastcol;
}
int AiGame::getScore()
{
    return 1;
}
