#ifndef STEP_H
#define STEP_H

#include"Gamescene.h"
class Step
{
public:
    Step();

    int _id;
    int col;//要去的行和列
    int row;
    int _killid;
    int lastcol;//原来的行和列
    int lastrow;

    //void getLastrowcol(int id);
};

#endif // STEP_H
