#include "Gamescene.h"
#include "ui_gamescene.h"
#include <QDebug>
#include<QPainter>
#include<QPen>
#include <QColor>
#include <algorithm>
#include<math.h>

Gamescene::Gamescene(QWidget *parent)//构造函数（初始化游戏）
    : QWidget(parent)
    , ui(new Ui::Gamescene)
{
    ui->setupUi(this);
    this->setWindowTitle("中国象棋对战");//窗口名字设置为“中国象棋对战”
    this->setFixedSize(995,678);//固定好窗口的分辨率大小

    for(int i=0;i<32;i++)
    {
        stone[i].initialize(i);//(initialize)初始化棋子
    }

    selectid=-1;//初始化（未选择）
    redtrue=true;//初始化（红方先行）
}

Gamescene::~Gamescene()
{
    delete ui;
}

int Gamescene::getStoneId(int row,int col)
{
    for(int i=0;i<32;i++)
    {
        if(row==stone[i].row&&col==stone[i].col)
            return i;
    }
    return -1;
}

bool Gamescene::isdead(int id)
{
    if(id==-1)return true;
    else
        return stone[id].death;
}

int Gamescene::getStoneCountAtLine(int row1,int col1,int row2,int col2)
{
    int ret=0;//记录直线上的棋子数量(不包括终点和起点）

    //若是两个点一致或是两个点不在同一直线上，则返回-1结束
    if((row1==row2&&col1==col2)||(row1!=row2&&col1!=col2))
        return -1;

    if(row1==row2)
    {
        for(int col=std::min(col1,col2)+1;col<std::max(col1,col2);col++)
            if(getStoneId(row1,col)!=-1)ret++;
    }
    else if(col1==col2)
    {
        for(int row=std::min(row1,row2)+1;row<std::max(row1,row2);row++)
            if(getStoneId(row,col1)!=-1)ret++;
    }
    return ret;
}


void Gamescene::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    QPixmap pixmap(":/mainscene/Gamesceneprint.jpg");
    painter.drawPixmap(this->rect(),pixmap);//贴上深沉的背景

    QPen pen=QPen(QBrush(Qt::SolidPattern),4);
    painter.setPen(pen);//画笔变粗

    offset=60;//距离界面的边距
    d=(45/30)*offset;//间距为50px
    r=d/2;//棋子半径为d/2

    //***绘画棋盘***//
    //绘画10条横线
    int gz=100;//棋盘向右移动100
    for(int i = 0; i < 10; i++)
    {
        painter.drawLine(QPoint(offset+gz, offset+i*d), QPoint(offset+8*d+gz, offset+i*d));
    }

    //***绘画竖线***

    //左右两条边线
    painter.drawLine(QPoint(offset+gz,offset),QPoint(offset+gz,offset+9*d));
    painter.drawLine(QPoint(offset+gz+8*d,offset),QPoint(offset+gz+8*d,offset+9*d));

    //中间的线
    for(int i=0;i<8;i++)
    {
        painter.drawLine(QPoint(offset+gz+i*d, offset), QPoint(offset+i*d+gz, offset+4*d));
    }//上半图

    for(int i=0;i<8;i++)
    {
        painter.drawLine(QPoint(offset+gz+i*d, offset+5*d), QPoint(offset+i*d+gz, offset+9*d));
    }//下半图

    //***将军家的斜线***
    painter.drawLine(QPoint(offset+gz+3*d, offset), QPoint(offset+5*d+gz, offset+2*d));
    painter.drawLine(QPoint(offset+gz+5*d, offset), QPoint(offset+3*d+gz, offset+2*d));

    painter.drawLine(QPoint(offset+gz+3*d, offset+9*d), QPoint(offset+5*d+gz, offset+7*d));
    painter.drawLine(QPoint(offset+gz+5*d, offset+9*d), QPoint(offset+3*d+gz, offset+7*d));

    //**"楚河汉界"**先设方形，然后在方形里面写字
    QRect rect1(offset+d+gz,offset+4*d+10,d,d);
    QRect rect2(offset+2*d+gz,offset+4*d+10,d,d);

    QRect rect3(-(offset+6*d+gz),-(offset+5*d)+10,d,d);
    QRect rect4(-(offset+7*d+gz),-(offset+5*d)+10,d,d);

    painter.setFont(QFont("隶书",r,800));
    painter.drawText(rect1,"楚",QTextOption(Qt::AlignHCenter));
    painter.drawText(rect2,"河",QTextOption(Qt::AlignHCenter));
    painter.save();
    painter.translate(0,0);
    painter.rotate(180);
    painter.drawText(rect3,"汉",QTextOption(Qt::AlignHCenter));
    painter.drawText(rect4,"界",QTextOption(Qt::AlignHCenter));
    painter.restore();

    for(int i=0;i<32;i++)
    {
        drawStone(painter,i);
    }
}



QPoint Gamescene::center(int row, int col)//象棋的棋盘的坐标转换成界面坐标
{
    QPoint p(col*d+offset+gz,row*d+offset);
    return p;
}



QPoint Gamescene::center(int id)
{
    QPoint p(stone[id].col*d+offset+gz,stone[id].row*d+offset);
    return p;
}



void Gamescene::drawStone(QPainter&painter,int id)
{
    if(stone[id].death)//判断棋子有无死
        return ;

    painter.setBrush(QBrush(QColor(255, 183, 58)));
    painter.setPen(QPen(Qt::black,4,Qt::SolidLine));
    painter.drawEllipse(center(id),r,r);//画棋子的圆形

    if(selectid==id)//判断是否有选择棋子
    {
        painter.setBrush(QBrush(QColor(64,64,196, 80)));
    }
    else
    {
        painter.setBrush(QBrush(QColor(64,64,196, 10)));
    }

    //画棋子颜色
    if(id < 16)
        painter.setPen(QPen(Qt::red,4,Qt::SolidLine));
    else
        painter.setPen(QPen(Qt::black,4,Qt::SolidLine));

    QRect rect(center(id).x()-r,center(id).y()-r,d,d);

    //设一个方，将其置于棋子的圆内，然后写字
    painter.setFont(QFont("楷体",r*1.3,2000));
    painter.drawText(rect,stone[id].getText(stone[id].red), QTextOption(Qt::AlignCenter));
    //绘画圆形里面的汉字

}



bool Gamescene::getRowCol(QPoint pt, int &row, int &col)
{
    for(row = 0; row <= 9; row++)
    {
        for(col = 0; col <= 8; col++)
        {
            QPoint temp = center(row, col);
            int x = temp.x()-pt.x();
            int y = temp.y()-pt.y();

            if(x*x+y*y < r*r)
                return true;
        }
    }
    return false;
}


//鼠标点击事件
void Gamescene::mousePressEvent(QMouseEvent *ev)
{
    QPoint pt = ev->pos();
    //将pt转化成棋盘的像行列值
    //判断这个行列值上面有没有棋子
    int row, col;

    //点击棋盘外面就不做处理
    if(!getRowCol(pt, row, col))
        return;

    clicked = -1;
    int i;

    //判断是哪一个棋子被选中，根据ID（这里的局部i）来记录下来
    for(i = 0; i <= 31; i++)
    {
        if(stone[i].row == row && stone[i].col == col && stone[i].death == false)
            break;
    }

    if(i < 32)
        clicked = i;  //选中的棋子的ID

    if(selectid == -1)//选中棋子
    {
        if(clicked != -1)
        {
            if(redtrue == stone[clicked].red)
                selectid = clicked;
        }
    }
    else if(selectid!=-1&&clicked!=-1&&stone[selectid].red==stone[clicked].red)//想换个棋子选
    {
        selectid = clicked;
    }
    else//走棋子
    {
        if(canMove(selectid,clicked, row, col ))
        {
            // 移动棋子的逻辑
            stone[selectid].row = row;
            stone[selectid].col = col;
            if(clicked != -1)// 取消选择
                stone[clicked].death = true;

            selectid = -1;
            redtrue = !redtrue;// 轮到对方

        }

    }
    update();// 刷新界面

}



//总的移动规则，选中准备下的棋子，被杀的棋子， 准备移动到的目的行列值
bool Gamescene::canMove(int moveId, int killId, int row, int col)//棋子走法
{
    //1.确定是选择其它棋子还是走棋
    //2.是否需要使用到canMoveXXX()来做限制
    //3.罗列出所有情况，和需要的得到的结果值 ==>  然后进行中间的逻辑层判断

    //tm这段注释别删，后面可能要用到
    // if(stone[moveId].red==stone[killId].red)
    // {
    //     if(killId==-1)
    //     {
    //         switch(stone[moveId].ty)//根据选中的棋子，来选择对应的走法
    //         {
    //         case Stone::JIANG:
    //             return canMoveJIANG(moveId, killId, row, col);
    //         case Stone::SHI:
    //             return canMoveSHI(moveId, killId, row, col);
    //         case Stone::XIANG:
    //             return canMoveXIANG(moveId, killId, row, col);
    //             // case Stone::MA:
    //             //     return canMoveMA(moveId, killId, row, col);
    //             // case Stone::CHE:
    //             //     return canMoveCHE(moveId, killId, row, col);
    //             // case Stone::PAO:
    //             //     return canMovePAO(moveId, killId, row, col);
    //             // case Stone::BING:
    //             //     return canMoveBING(moveId, killId, row, col);

    //         }
    //     }
    //     selectid=killId;
    //     update();
    //     return false;
    // }

    // else
    // {
        switch(stone[moveId].ty)//根据选中的棋子，来选择对应的走法
        {
        case Stone::JIANG:
            return canMoveJIANG(moveId, killId, row, col);
        case Stone::SHI:
            return canMoveSHI(moveId, killId, row, col);
        case Stone::XIANG:
            return canMoveXIANG(moveId, killId, row, col);
            // case Stone::MA:
            //     return canMoveMA(moveId, killId, row, col);
            // case Stone::CHE:
            //     return canMoveCHE(moveId, killId, row, col);
            // case Stone::PAO:
            //     return canMovePAO(moveId, killId, row, col);
            // case Stone::BING:
            //     return canMoveBING(moveId, killId, row, col);

        }
        return true;
    // }
}

bool Gamescene::canMoveJIANG(int moveId, int killId, int row, int col)
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
bool Gamescene::canMoveSHI(int moveId, int killId, int row, int col)
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
    int dr=abs(stone[moveId].row-row);//行走的步数

    int dc=abs(stone[moveId].col-col);//列走的步数
    int d=dr*10+dc;
    if(d==11)//用将的走法原理类似
        return true;

    return false;

}
 bool Gamescene::canMoveXIANG(int moveId, int killId, int row, int col)
{
     return 1;
}
// bool Gamescene::canMoveMA(int moveId, int killId, int row, int col);
// bool Gamescene::canMoveCHE(int moveId, int killId, int row, int col);
// bool Gamescene::canMovePAO(int moveId, int killId, int row, int col);
// bool Gamescene::canMoveBING(int moveId, int killId, int row, int col);
