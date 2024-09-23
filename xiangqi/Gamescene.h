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

private:
    Ui::Gamescene *ui;
};

#endif // GAMESCENE_H
