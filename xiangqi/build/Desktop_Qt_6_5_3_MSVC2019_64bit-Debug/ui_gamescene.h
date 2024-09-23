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
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Gamescene
{
public:

    void setupUi(QWidget *Gamescene)
    {
        if (Gamescene->objectName().isEmpty())
            Gamescene->setObjectName("Gamescene");
        Gamescene->resize(995, 678);
        Gamescene->setStyleSheet(QString::fromUtf8(""));

        retranslateUi(Gamescene);

        QMetaObject::connectSlotsByName(Gamescene);
    } // setupUi

    void retranslateUi(QWidget *Gamescene)
    {
        Gamescene->setWindowTitle(QCoreApplication::translate("Gamescene", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Gamescene: public Ui_Gamescene {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GAMESCENE_H
