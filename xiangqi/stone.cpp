#include"Stone.h"

struct Pos
{
    int _row;
    int _col;
    Stone::TY _ty;
};
Stone::Stone()
{

}

Stone::~Stone()
{

}

//列举一方中16个棋子的初始状态
Pos pos[16]={
    {0,0,Stone::CHE},
    {0,1,Stone::MA},
    {0,3,Stone::XIANG},
    {0,4,Stone::SHI},
    {0,5,Stone::JIANG},
    {0,6,Stone::SHI},
    {0,7,Stone::XIANG},
    {0,8,Stone::MA},
    {0,9,Stone::CHE},

    {3,2,Stone::PAO},
    {3,8,Stone::PAO},

    {4,0,Stone::BING},
    {4,0,Stone::BING},
    {4,0,Stone::BING},
    {4,0,Stone::BING},
    {4,0,Stone::BING},
};

void Stone::initialize(int id)//根据棋子的id，给每个棋子赋予他的初始状态
{
    if(id<16)
    {
        row=pos[id]._row;
        col=pos[id]._col;
        ty=pos[id]._ty;
        red=true;
    }
    else
    {
        row=9-pos[id-16]._row;
        col=9-pos[id-16]._col;
        ty=pos[id]._ty;
        red=true;
    }
    death=false;
}

QString Stone::getText(bool isRed)
{
    QString a;
    if(isRed){
        switch (ty) {
        case CHE:
            return "俥";
        case MA:
            return "傌";
        case PAO:
            return "炮";
        case BING:
            return "兵";
        case JIANG:
            return "帥";
        case SHI:
            return "仕";
        case XIANG:
            return "相";
        default:
            return "null";
        }
    }
    else{

        switch (ty) {
        case CHE:
            return "車";
        case MA:
            return "馬";
        case PAO:
            return "砲";
        case BING:
            return "卒";
        case JIANG:
            return "將";
        case SHI:
            return "士";
        case XIANG:
            return "象";
        default:
            return "null";
        }
    }
    return "ERROR";

    return a;
}

