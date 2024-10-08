#include "Gamescene.h"
#include "ui_gamescene.h"
#include <QDebug>
#include<QPainter>
#include<QPen>
#include <QColor>
#include <algorithm>

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
}



void Gamescene::mousePressEvent(QMouseEvent *event)
{
QPoint pt = event->pos();
int row, col;
if(getRowCol(pt, row, col))
{
    int clickedId = getStoneId(row, col);
    if(selectid == -1) // 没有选择棋子
    {
        if(clickedId != -1 && stone[clickedId].red == redtrue) // 选择自己的棋子
        {
            selectid = clickedId;
            qDebug() << "Selected stone id:" << selectid;
        }
    }
    else // 已经选择了一个棋子
    {
        if(clickedId != -1 && stone[clickedId].red == redtrue) // 选择自己的棋子
        {
            selectid = clickedId;
            qDebug() << selectid;
        }
        else // 移动棋子或吃子
        {
            if(clickedId == -1 || stone[clickedId].red != redtrue) // 空位置或对方棋子
            {
                // 移动棋子逻辑
                stone[selectid].row = row;
                stone[selectid].col = col;
                if(clickedId != -1) // 吃子逻辑
                {
                    stone[clickedId].death = true;
                    qDebug() << clickedId;
                }
                selectid = -1; // 取消选择
                redtrue = !redtrue; // 轮到对方
                update(); // 刷新界面
            }
        }
    }
}
else
{
    selectid = -1; // 点击空白区域，取消选择
    qDebug() << "没选中";
}
}

