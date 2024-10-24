#ifndef NETGAME_H
#define NETGAME_H

#include "Gamescene.h"
#include <QTcpServer>
#include <QTcpSocket>


/*要传递的信息（组织报文结构）
 1.执红方还是黑方，这个信息有服务器发出，客户端接收
（第一个字节固定是1，第二个字节是1，或者0，1表示接收方走红旗，0表示走黑棋）
 2.鼠标点击信息
（第一个字节固定是2，第二个字节是row，第三个字节是co1）
3.悔棋信息
（第一个字节固定是3）
*/
class NetGame:public Gamescene
{
public:

    NetGame(bool isServer,QWidget* parent = nullptr);
    ~NetGame();

    void mousePressEvent(QMouseEvent *ev);//重载鼠标点击函数

    QTcpServer * Server;//服务器端
    QTcpSocket * Socket;//数据接收或发送器

    void afterConnection();//连接后
    void slotRect();
    /* 这些都是要重载的函数*/
    bool canMove(int moveId, int killId, int row, int col);//重载他的原因是，刷新他里面调用的函数
    bool canMoveJIANG(int moveId, int killId, int row, int col);
    bool canMoveSHI(int moveId, int killId, int row, int col);
    bool canMoveXIANG(int moveId, int killId, int row, int col);
    bool canMoveBING(int moveId, int killId, int row, int col);

private:
    bool token;//联机令牌，true是红方，fasle是黑方，有了这个令牌，就不能控制别人的棋子

};


#endif // NETGAME_H
