#include "Netgame.h"
#include "ui_gamescene.h"
#include <QPushButton>
#include<QDebug>
#include<QMessageBox>

NetGame::NetGame(bool isServer,QWidget* parent):Gamescene(parent)
{
    Server=NULL;
    Socket=NULL;

    if(isServer)
    {
        this->setWindowTitle("中国象棋对战（服务器端）");
        Server=new QTcpServer(this);//new 一个服务器
        Server->listen(QHostAddress::Any,9999);
        //QHostAddresss保存IPv4或IPv6地址,此处表示服务器将监听所有网络接口
        //9999表示自己服务器的端口

        connect(Server,&QTcpServer::newConnection,this,&NetGame::afterConnection);
        connect(ui->pushButton_3,&QPushButton::clicked,[=](){
            char buf[3];
            buf[0]=3;//表示选择位置报文
            Socket->write(buf,1);
        });//悔棋按钮连接再添加功能,传报文去Socket
    }
    else
    {
        this->setWindowTitle("中国象棋对战（客户端）");
        Socket=new QTcpSocket(this);//与上面同理
        Socket->connectToHost(QHostAddress("127.0.0.1"),9999);//连接到服务器
        /*
        127.0.0.1 是主机环回地址。主机环回是指地址为 127.0.0.1 的任何数
        据包都不应该离开计算机（主机），发送它——而不是被发送到本地网络
        或互联网，它只是被自己“环回”，并且发送数据包的计算机成为接收者。
        */
        connect(Socket,&QTcpSocket::readyRead,this,&NetGame::slotRect);
        connect(ui->pushButton_3,&QPushButton::clicked,[=](){
            char buf[3];
            buf[0]=3;//表示选择位置报文
            Socket->write(buf,1);
        });//悔棋按钮连接再添加功能,传报文去Socket
    }
}



NetGame::~NetGame()
{

}



void NetGame::mousePressEvent(QMouseEvent *ev)
{
    if(token!=redtrue)
        return;

    QPoint pt = ev->pos();
    //将pt转化成棋盘的像行列值
    int row,col,id;
    //判断这个行列值上面有没有棋子
    //点击棋盘外面就不做处理
    if(!getRowCol(pt, row, col))
        return;


    StoneMove(row,col);

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
        token=false;
    }
    else
    {
        buf[0]=1;
        buf[1]=1;//输入1，自己选红方，对方选黑方

        token=true;
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
    if(cmd==1)//接收选棋子颜色的信息
    {
        char data=ba[1];
        token=false;
        qDebug()<<data;
        if(data==0)//棋盘翻转
        {
            token=true;
            for(int i=0;i<32;i++)
            {
            stone[i].col=8-stone[i].col;
            stone[i].row=9-stone[i].row;
            }
        }
    }

    else if(cmd==2)//接收对战信息
    {
        int row=ba[1];
        int col=ba[2];


        StoneMove(9-row,8-col);
    }

    else if(cmd==3)
    {
        Regretstone();
    }
}



bool NetGame::canMove(int moveId, int killId, int row, int col)
{
    //1.确定是选择其它棋子还是走棋
    //2.是否需要使用到canMoveXXX()来做限制
    //3.罗列出所有情况，和需要的得到的结果值 ==>  然后进行中间的逻辑层判断

    //tm这段注释别删，后面可能要用到
    if(stone[moveId].red==stone[killId].red)
    {
        if(killId==-1)
        {
            switch(stone[moveId].ty)//根据选中的棋子，来选择对应的走法
            {
            case Stone::JIANG:
                return canMoveJIANG(moveId, killId, row, col);
            case Stone::SHI:
                return canMoveSHI(moveId, killId, row, col);
            case Stone::XIANG:
                return canMoveXIANG(moveId, killId, row, col);
            case Stone::MA:
                return canMoveMA(moveId, killId, row, col);
            case Stone::CHE:
                return canMoveCHE(moveId, killId, row, col);
            case Stone::PAO:
                return canMovePAO(moveId, killId, row, col);
            case Stone::BING:
                return canMoveBING(moveId, killId, row, col);

            }
        }
        selectid=killId;
        update();
        return false;
    }

    else
    {
        switch(stone[moveId].ty)//根据选中的棋子，来选择对应的走法
        {
        case Stone::JIANG:
            return canMoveJIANG(moveId, killId, row, col);
        case Stone::SHI:
            return canMoveSHI(moveId, killId, row, col);
        case Stone::XIANG:
            return canMoveXIANG(moveId, killId, row, col);
        case Stone::MA:
            return canMoveMA(moveId, killId, row, col);
        case Stone::CHE:
            return canMoveCHE(moveId, killId, row, col);
        case Stone::PAO:
            return canMovePAO(moveId, killId, row, col);
        case Stone::BING:
            return canMoveBING(moveId, killId, row, col);

        }
        return true;
    }
}

/*根据token的选边，选用不同的棋子可走的策略
    比如选了服务器选了红方（toekn==true），那么服务器端就有自己棋子的走法
    于是客户端（token==false），接收服务器发送的报文后，对自己的棋子有相应的走法。

    不然，服务器的红方走的兵往前走,在客户端方接收的报文后，客户端红兵由于被视为往后走，就走不来了。


 */

bool NetGame::canMoveJIANG(int moveId, int killId, int row, int col)
{
    // 检查将军是否面对面
    if (face())
    {
        // 如果两个将军之间没有棋子，那么将军的走法与车一致
        if (canMoveCHE(moveId, killId, row, col))
            return true;
    }

    /* 在服务器是选红方的情况下*/
    if(token==false)
    {
        //将军不能超出营地
        if(stone[moveId].red)//红方
        {

            if(row > 2 || col < 3 )
                return false;
            if(col>5)
                return false;
        }
        else//黑方
        {
            if(row<7||col<3)
                return false;
            if(col>5)
                return false;
        }
    }
    else
    {
        //将军不能超出营地
        if(!stone[moveId].red)//红方
        {

            if(row > 2 || col < 3 )
                return false;
            if(col>5)
                return false;
        }
        else//黑方
        {
            if(row<7||col<3)
                return false;
            if(col>5)
                return false;
        }
    }

    //计算将军行与列分别走的步数，并加起来只能等于一
    int dr=abs(stone[moveId].row-row);
    int dc=abs(stone[moveId].col-col);

    int d=dr*10+dc;
    //为何要dr*10呢？假设填了(dr==1||dc==1),那么当dr=2，dc=1的时候，也能走
    //所以要让他们相加为d，行走了一步，算10；列走了一步，算1.
    if(d==1||d==10)
        return true;

    return false;

}


 bool NetGame::canMoveSHI(int moveId, int killId, int row, int col)
{
    if(token==false)
    {
        //对于士行走范围的限制
        if(stone[moveId].red)//红方
        {

            if(row > 2 || col < 3 )
                return false;
            if(col>5)
                return false;
        }
        else//黑方
        {
            if(row<7||col<3)
                return false;
            if(col>5)
                return false;
        }
    }
    else
    {
        //对于士行走范围的限制
        if(!stone[moveId].red)//红方
        {

            if(row > 2 || col < 3 )
                return false;
            if(col>5)
                return false;
        }
        else//黑方
        {
            if(row<7||col<3)
                return false;
            if(col>5)
                return false;
        }
    }
    int dr=abs(stone[moveId].row-row);//行走的步数

    int dc=abs(stone[moveId].col-col);//列走的步数
    int d=dr*10+dc;
    if(d==11)//用将的走法原理类似
        return true;

    return false;
}



bool NetGame::canMoveXIANG(int moveId, int killId, int row, int col)
{
    if(token==false)
    {
        if(stone[moveId].red)
        {
            if(row>4)
                return false;
        }
        else
        {
            if(row<5)
                return false;
        }
    }
    else
    {
        if(!stone[moveId].red)
        {
            if(row>4)
                return false;
        }
        else
        {
            if(row<5)
                return false;
        }
    }


    int dr=abs(stone[moveId].row-row);//行走的步数
    int dc=abs(stone[moveId].col-col);//列走的步数

    if(dr==2&&dc==2)
    {
        //计算象脚的位置
        int kr=(stone[moveId].row+row)/2;
        int kc=(stone[moveId].col+col)/2;

        if(getStoneId(kr,kc)!=-1)//如果象脚有棋子，那么不允许移动
            return false;

        return true;
    }

    return false;

}

bool NetGame::canMoveBING(int moveId, int killId, int row, int col)
{
    int r = stone[moveId].row;
    int c = stone[moveId].col;

    if(token==false)
    {
        // 兵在红方
        if(stone[moveId].red)
        {
            // 兵未过河
            if(r <= 4)
            {
                // 只能向前走一步
                if(row == r + 1 && col == c)
                    return true;
            }
            else // 兵已过河
            {
                // 可以向前、左、右走一步
                if((row == r + 1 && col == c) || (row == r && col == c + 1) || (row == r && col == c - 1))
                    return true;
            }
        }
        else // 兵在黑方
        {
            // 兵未过河
            if(r >= 5)
            {
                // 只能向前走一步
                if(row == r - 1 && col == c)
                    return true;
            }
            else // 兵已过河
            {
                // 可以向前、左、右走一步
                if((row == r - 1 && col == c) || (row == r && col == c + 1) || (row == r && col == c - 1))
                    return true;
            }
        }
    }
    else
    {
        // 兵在红方
        if(!stone[moveId].red)
        {
            // 兵未过河
            if(r <= 4)
            {
                // 只能向前走一步
                if(row == r + 1 && col == c)
                    return true;
            }
            else // 兵已过河
            {
                // 可以向前、左、右走一步
                if((row == r + 1 && col == c) || (row == r && col == c + 1) || (row == r && col == c - 1))
                    return true;
            }
        }
        else // 兵在黑方
        {
            // 兵未过河
            if(r >= 5)
            {
                // 只能向前走一步
                if(row == r - 1 && col == c)
                    return true;
            }
            else // 兵已过河
            {
                // 可以向前、左、右走一步
                if((row == r - 1 && col == c) || (row == r && col == c + 1) || (row == r && col == c - 1))
                    return true;
            }
        }
    }

    return false;
}

