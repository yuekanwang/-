#ifndef STONE_H
#define STONE_H
#include <QString>
class Stone
{
public:
    Stone();
    ~Stone();
    QString getText(bool isRed);//根据enum TY的类型决定棋子上面的汉字
    void initialize(int id);//棋子初始化

    enum TY{JIANG, SHI, XIANG, MA, CHE, PAO, BING};
    int row;//棋子所在的行
    int col;//列
    int id;//棋子id
    bool death;//是否死亡
    bool red;//棋子颜色
    TY ty;//棋子的类型

};

#endif // STONE_H
