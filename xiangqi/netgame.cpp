#include "Netgame.h"
#include<QDebug>
#include<QMessageBox>
NetGame::NetGame(bool isServer)
{
    Server=NULL;
    Socket=NULL;

    if(isServer)
    {
        Server=new QTcpServer(this);//new 一个服务器
        Server->listen(QHostAddress::Any,9999);
        //QHostAddresss保存IPv4或IPv6地址,此处表示服务器将监听所有网络接口
        //9999表示自己服务器的端口

        connect(Server,&QTcpServer::newConnection,this,&NetGame::afterConnection);

        qDebug()<<"服务器";
    }
    else
    {
        Socket=new QTcpSocket(this);//与上面同理
        Socket->connectToHost(QHostAddress("127.0.0.1"),9999);//连接到服务器
        /*
        127.0.0.1 是主机环回地址。主机环回是指地址为 127.0.0.1 的任何数
        据包都不应该离开计算机（主机），发送它——而不是被发送到本地网络
        或互联网，它只是被自己“环回”，并且发送数据包的计算机成为接收者。
        */
        qDebug()<<"客户端";
        connect(Socket,&QTcpSocket::readyRead,this,&NetGame::slotRect);

    }
}



NetGame::~NetGame()
{

}



void NetGame::mousePressEvent(QMouseEvent *ev)
{
    Gamescene::mousePressEvent(ev);//首先调用父类点击函数
    int row,col;
    /* 然后发送报文给对方 */
    char buf[3];
    buf[0]=2;//表示选择位置报文
    buf[1]=row;
    buf[2]=col;
    Socket->write(buf,3);//无论是服务器还是客户端，都用Socket发送报文出去

}

void NetGame::afterConnection()
{
    if(Socket)return;//如果已经有客户端连接了，则返回结束

    Socket=Server->nextPendingConnection();//返回一个客户端（Socket）

    connect(Socket,&QTcpSocket::readyRead,this,&NetGame::slotRect);
    //站在服务器角度，通过Socket接收数据。

    qDebug()<<"123";

    /* 给服务器对方发送选方数据 */
    char buf[2];
    QMessageBox::StandardButton ret;
    ret=QMessageBox::question(NULL,"选红黑方","是否为服务器端选择红方（客户端玩家自动选择对应颜色）。");
    if(ret==QMessageBox::No)
    {
        buf[0]=1;
        buf[1]=0;//输入0，自己选黑方，对方选红方
    }
    else
    {
        buf[0]=1;
        buf[1]=1;//输入1，自己选红方，对方选黑方
    }


    Socket->write(buf,2);
    if(buf[1]==1)//棋盘翻转
    {
        for(int i=0;i<32;i++)
        {
            stone[i].col=8-stone[i].col;
            stone[i].row=9-stone[i].row;
        }
    }
}


void NetGame::slotRect()//接收
{
    QByteArray ba=Socket->readAll();//字节数组接收报文存放

    char cmd=ba[0];
    if(cmd==1)
    {
        char data=ba[1];
        qDebug()<<data;
        if(data==0)//棋盘翻转
        {
            for(int i=0;i<32;i++)
            {
                stone[i].col=8-stone[i].col;
                stone[i].row=9-stone[i].row;
            }
        }
    }

    else if(cmd==2)
    {
        int row=ba[1];
        int col=ba[2];

    }

}
