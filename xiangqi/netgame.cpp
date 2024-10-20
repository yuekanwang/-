#include "Netgame.h"
#include<QDebug>
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

void NetGame::afterConnection()
{
    if(Socket)return;//如果已经有客户端连接了，则返回结束

    Socket=Server->nextPendingConnection();//返回一个客户端（Socket）

    qDebug()<<"123";
    /* 给对方发送数据 */
    char buf[2];
    buf[0]=1;
    buf[0]=0;
    Socket->write(buf,2);

}
void NetGame::slotRect()
{
    QByteArray ba=Socket->readAll();//字节数组接收存放

    char cmd=ba[0];
    if(cmd==1)
    {
        chat data=ba[1];
        init(data==1);
    }

}
