#ifndef STONE_H
#define STONE_H
#include <QString>
class Stone
{
public:
    Stone();
    ~Stone();
    QString getText();//根据enum TY得类型决定棋子上面的汉字
    void initialize();//棋子初始化

    enum TY{将，士，象，马，车，炮，兵};
    int row;//棋子所在的行
    int col;//列
    int id;//棋子id
    bool death;//是否死亡
    bool red;//棋子颜色
    TY ty;//棋子的类型

};

#endif // STONE_H
