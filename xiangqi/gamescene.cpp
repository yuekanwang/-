#include "Gamescene.h"
#include "ui_gamescene.h"
#include <QMouseEvent>
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


}

Gamescene::~Gamescene()
{
    delete ui;
}

void Gamescene::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    QPixmap pixmap(":/mainscene/Gamesceneprint.jpg");
    painter.drawPixmap(this->rect(),pixmap);//贴上深沉的背景
    QPen pen=QPen(QBrush(Qt::SolidPattern),4);
    painter.setPen(pen);

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

    //绘画竖线

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


}
