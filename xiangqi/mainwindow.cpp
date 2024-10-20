#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Netgame.h"
#include <QPainter>
#include <QMessageBox>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->action,&QAction::triggered,this,&MainWindow::close);
    this->setWindowTitle("中国象棋");
    this->setFixedSize(995,678);


}
void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);//创建画家，指定绘图设备
    QPixmap pix;//创建QPixmap对象
    pix.load(":/mainscene/mainprint(fangyuan).jpg");
    painter.drawPixmap(0,0,this->width(),this->height(),pix);
}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    this->hide();//隐藏开启页面
    Gamescene *gamescene =new Gamescene;//单机游戏画面
    gamescene->show();
}


void MainWindow::on_pushButton_2_clicked()
{
    QMessageBox::StandardButton ret;
    ret=QMessageBox::question(NULL," ","是否作为服务器启动");
    bool isServer=false;
    if(ret==QMessageBox::Yes)
    {
        isServer=true;
    }
    this->hide();
    NetGame *netgame=new NetGame(isServer);//联机游戏画面
    netgame->show();
}

