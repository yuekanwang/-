#include "Gamescene.h"
#include "ui_gamescene.h"

#include <QDebug>
#include<QPainter>
#include<QPen>
#include <QColor>
Gamescene::Gamescene(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Gamescene)
{
    ui->setupUi(this);
    this->setWindowTitle("中国象棋对战");
    this->setFixedSize(995,678);

    for(int i=0;i<32;i++)
    {
        stone[i].initialize(i);//(initialize)初始化
    }

}

Gamescene::~Gamescene()
{
    delete ui;
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
    QRect rect3(offset+5*d+gz,offset+4*d+10,d,d);
    QRect rect4(offset+6*d+gz,offset+4*d+10,d,d);
    painter.setFont(QFont("隶书",r,800));
    painter.drawText(rect1,"楚",QTextOption(Qt::AlignHCenter));
    painter.drawText(rect2,"河",QTextOption(Qt::AlignHCenter));
    painter.drawText(rect3,"汉",QTextOption(Qt::AlignHCenter));
    painter.drawText(rect4,"界",QTextOption(Qt::AlignHCenter));

}
QPoint Gamescene::center(int row, int col)//象棋的棋盘的坐标转换成界面坐标
{
    QPoint p(col*d+offset+gz,row*d+offset+gz);
    return p;
}

QPoint Gamescene::center(int id)
{
    QPoint p(stone[id].col*d+offset+gz,stone[id].row*d+offset+gz);
    return p;
}

void Gamescene::drawStone(QPainter&painter,int id)
{
    if(stone[id].death)//判断棋子有无死
        return ;


    QPen pen=QPen(QBrush(Qt::SolidPattern),4);
    painter.setPen(pen);//画笔变粗
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
        painter.setPen(QColor(255, 0, 0));
    else
        painter.setPen(QColor(0, 0, 0));

    painter.drawText(QRect(center(id),d,d), stone[id].getText(stone[id].death), QTextOption(Qt::AlignCenter));  //绘画圆形里面的汉字

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


void Gamescene::mousePressEvent(QMouseEvent *)
{
    qDebug()<<"123";
}

