/********************************************************************************
** Form generated from reading UI file 'gamescene.ui'
**
** Created by: Qt User Interface Compiler version 6.5.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GAMESCENE_H
#define UI_GAMESCENE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Gamescene
{
public:
    QPushButton *pushButton;

    void setupUi(QWidget *Gamescene)
    {
        if (Gamescene->objectName().isEmpty())
            Gamescene->setObjectName("Gamescene");
        Gamescene->resize(1265, 712);
        Gamescene->setStyleSheet(QString::fromUtf8(""));
        pushButton = new QPushButton(Gamescene);
        pushButton->setObjectName("pushButton");
        pushButton->setGeometry(QRect(730, 50, 161, 81));
        pushButton->setStyleSheet(QString::fromUtf8(""));

        retranslateUi(Gamescene);

        QMetaObject::connectSlotsByName(Gamescene);
    } // setupUi

    void retranslateUi(QWidget *Gamescene)
    {
        Gamescene->setWindowTitle(QCoreApplication::translate("Gamescene", "Form", nullptr));
        pushButton->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class Gamescene: public Ui_Gamescene {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GAMESCENE_H
