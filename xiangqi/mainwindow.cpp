#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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