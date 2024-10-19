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
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;

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
        pushButton_2 = new QPushButton(Gamescene);
        pushButton_2->setObjectName("pushButton_2");
        pushButton_2->setGeometry(QRect(909, 20, 71, 50));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(pushButton_2->sizePolicy().hasHeightForWidth());
        pushButton_2->setSizePolicy(sizePolicy);
        pushButton_2->setMinimumSize(QSize(50, 50));
        pushButton_2->setCursor(QCursor(Qt::PointingHandCursor));
        pushButton_2->setStyleSheet(QString::fromUtf8("QPushButton\n"
"{\n"
"	color:black;\n"
"\n"
"	font: 15pt \"\345\215\216\346\226\207\350\241\214\346\245\267\";\n"
"\n"
"	background-color:#47afff;\n"
"\n"
"	border-radius:10px;\n"
"}\n"
"\n"
"QPushButton:hover\n"
"{\n"
"	color:black;\n"
"\n"
"	font: 15pt \"\345\215\216\346\226\207\350\241\214\346\245\267\";\n"
"\n"
"	background-color:#99d8ff;\n"
"\n"
"   border-radius:10px;\n"
"}"));
        pushButton_2->setCheckable(true);
        pushButton_2->setChecked(true);
        pushButton_2->setAutoExclusive(true);
        pushButton_3 = new QPushButton(Gamescene);
        pushButton_3->setObjectName("pushButton_3");
        pushButton_3->setGeometry(QRect(770, 530, 91, 51));
        pushButton_3->setCursor(QCursor(Qt::PointingHandCursor));

        retranslateUi(Gamescene);

        QMetaObject::connectSlotsByName(Gamescene);
    } // setupUi

    void retranslateUi(QWidget *Gamescene)
    {
        Gamescene->setWindowTitle(QCoreApplication::translate("Gamescene", "Form", nullptr));
        pushButton->setText(QString());
        pushButton_2->setText(QCoreApplication::translate("Gamescene", "\351\237\263\344\271\220\346\211\223\345\274\200", nullptr));
        pushButton_3->setText(QCoreApplication::translate("Gamescene", "\346\202\224\346\243\213", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Gamescene: public Ui_Gamescene {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GAMESCENE_H
