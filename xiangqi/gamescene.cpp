#include "Gamescene.h"
#include "ui_gamescene.h"

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

}
