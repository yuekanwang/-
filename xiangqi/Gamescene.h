#ifndef GAMESCENE_H
#define GAMESCENE_H

#include <QWidget>

namespace Ui {
class Gamescene;
}

class Gamescene : public QWidget
{
    Q_OBJECT

public:
    explicit Gamescene(QWidget *parent = nullptr);
    ~Gamescene();
    void paintEvent(QPaintEvent *event);

    int r;//棋子半径
    int offset;//距离界面的边距
    int d;//间距
    int selectid;


private:
    Ui::Gamescene *ui;
};

#endif // GAMESCENE_H
