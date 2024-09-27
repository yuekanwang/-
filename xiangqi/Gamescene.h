#ifndef GAMESCENE_H
#define GAMESCENE_H
#include <QMouseEvent>
#include <QWidget>
#include "Stone.h"
namespace Ui {
class Gamescene;
}

class Gamescene : public QWidget
{
    Q_OBJECT

public:
    explicit Gamescene(QWidget *parent = nullptr);
    ~Gamescene();
    void paintEvent(QPaintEvent *event);

    //象棋的棋盘的坐标转换成界面坐标
    QPoint center(int row, int col);
    QPoint center(int id);

    //绘画单个具体的棋子
    void drawStone(QPainter&painter,int id);

    //界面坐标转换成棋盘的行列值[获取鼠标点击的像素坐标，是位于棋盘的哪一个行列值]
    bool getRowCol(QPoint pt, int& row, int& col);

    //鼠标点击事件
    virtual void mousePressEvent(QMouseEvent *event);

    //象棋移动的规则[将  士  象  马  车  炮  兵]
    bool canMove(int moveId, int killId, int row, int col);
    bool canMoveJIANG(int moveId, int killId, int row, int col);
    bool canMoveSHI(int moveId, int killId, int row, int col);
    bool canMoveXIANG(int moveId, int killId, int row, int col);
    bool canMoveMA(int moveId, int killId, int row, int col);
    bool canMoveCHE(int moveId, int killId, int row, int col);
    bool canMovePAO(int moveId, int killId, int row, int col);
    bool canMoveBING(int moveId, int killId, int row, int col);

    Stone stone[32];
    int r;//棋子半径
    int offset;//距离界面的边距
    int d;//间距
    int selectid;//选中棋子0，否则1；
    int clicked;//所选棋子的id
    bool redtrue;//红旗先下标志

private:
    Ui::Gamescene *ui;
    bool isdead(int id);
    int setStoneId(int row,int col);
    //车 炮 的功能辅助函数   判断两个点是否在一个直线上面,且返回直线之间的棋子个数
    int getStoneCountAtLine(int row1,int col1,int row2,int col2);
};

#endif // GAMESCENE_H
