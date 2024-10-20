#ifndef NETGAME_H
#define NETGAME_H

#include "Gamescene.h"
#include <QTcpServer>
#include <QTcpSocket>


/*要传递的信息（组织报文结构）
 1.执红方还是黑方，这个信息有服务器发出，客户端接收
（第一个字节固定是1，第二个字节是1，或者0，1表示接收方走红旗，0表示走黑棋）
 2.鼠标点击信息
（第一个字节固定是2，第二个字节是row，第三个字节是co1，第四个字节是点击的棋子id）
*/
class NetGame:public Gamescene
{
public:
    NetGame(bool isServer);
    ~NetGame();

    QTcpServer * Server;//服务器端
    QTcpSocket * Socket;//客户端

    void afterConnection();//连接后
    void slotRect();

};

#endif // NETGAME_H
