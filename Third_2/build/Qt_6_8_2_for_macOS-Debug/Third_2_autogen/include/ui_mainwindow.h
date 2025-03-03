/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLCDNumber>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QLineEdit *lineEditA;
    QSpinBox *spinBoxA;
    QProgressBar *progressBarA;
    QLCDNumber *lcdNumberA;
    QLineEdit *lineEditC;
    QLCDNumber *lcdNumberC;
    QSpinBox *spinBoxC;
    QProgressBar *progressBarC;
    QLineEdit *lineEditB;
    QLCDNumber *lcdNumberB;
    QSpinBox *spinBoxB;
    QProgressBar *progressBarB;
    QLabel *labelA;
    QLabel *labelB;
    QLabel *labelC;
    QLabel *label_less_1;
    QLabel *label_less_2;
    QSlider *trackBarA;
    QSlider *trackBarB;
    QSlider *trackBarC;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(619, 294);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        lineEditA = new QLineEdit(centralwidget);
        lineEditA->setObjectName("lineEditA");
        lineEditA->setGeometry(QRect(70, 90, 113, 21));
        spinBoxA = new QSpinBox(centralwidget);
        spinBoxA->setObjectName("spinBoxA");
        spinBoxA->setGeometry(QRect(70, 120, 111, 22));
        spinBoxA->setMaximum(100);
        spinBoxA->setValue(10);
        progressBarA = new QProgressBar(centralwidget);
        progressBarA->setObjectName("progressBarA");
        progressBarA->setGeometry(QRect(70, 170, 111, 23));
        progressBarA->setValue(10);
        lcdNumberA = new QLCDNumber(centralwidget);
        lcdNumberA->setObjectName("lcdNumberA");
        lcdNumberA->setGeometry(QRect(70, 200, 111, 41));
        lcdNumberA->setProperty("intValue", QVariant(10));
        lineEditC = new QLineEdit(centralwidget);
        lineEditC->setObjectName("lineEditC");
        lineEditC->setGeometry(QRect(440, 90, 113, 21));
        lcdNumberC = new QLCDNumber(centralwidget);
        lcdNumberC->setObjectName("lcdNumberC");
        lcdNumberC->setGeometry(QRect(440, 200, 111, 41));
        lcdNumberC->setProperty("intValue", QVariant(90));
        spinBoxC = new QSpinBox(centralwidget);
        spinBoxC->setObjectName("spinBoxC");
        spinBoxC->setGeometry(QRect(440, 120, 111, 22));
        spinBoxC->setMaximum(100);
        spinBoxC->setValue(90);
        progressBarC = new QProgressBar(centralwidget);
        progressBarC->setObjectName("progressBarC");
        progressBarC->setGeometry(QRect(440, 170, 111, 23));
        progressBarC->setValue(90);
        lineEditB = new QLineEdit(centralwidget);
        lineEditB->setObjectName("lineEditB");
        lineEditB->setGeometry(QRect(260, 90, 113, 21));
        lcdNumberB = new QLCDNumber(centralwidget);
        lcdNumberB->setObjectName("lcdNumberB");
        lcdNumberB->setGeometry(QRect(260, 200, 111, 41));
        lcdNumberB->setProperty("intValue", QVariant(50));
        spinBoxB = new QSpinBox(centralwidget);
        spinBoxB->setObjectName("spinBoxB");
        spinBoxB->setGeometry(QRect(260, 120, 111, 22));
        spinBoxB->setMaximum(100);
        spinBoxB->setValue(50);
        progressBarB = new QProgressBar(centralwidget);
        progressBarB->setObjectName("progressBarB");
        progressBarB->setGeometry(QRect(260, 170, 111, 23));
        progressBarB->setValue(50);
        labelA = new QLabel(centralwidget);
        labelA->setObjectName("labelA");
        labelA->setGeometry(QRect(110, 20, 31, 51));
        labelA->setTextFormat(Qt::TextFormat::AutoText);
        labelB = new QLabel(centralwidget);
        labelB->setObjectName("labelB");
        labelB->setGeometry(QRect(300, 20, 31, 51));
        labelB->setTextFormat(Qt::TextFormat::AutoText);
        labelC = new QLabel(centralwidget);
        labelC->setObjectName("labelC");
        labelC->setGeometry(QRect(480, 20, 31, 51));
        labelC->setTextFormat(Qt::TextFormat::AutoText);
        label_less_1 = new QLabel(centralwidget);
        label_less_1->setObjectName("label_less_1");
        label_less_1->setGeometry(QRect(200, 20, 51, 51));
        label_less_1->setTextFormat(Qt::TextFormat::AutoText);
        label_less_2 = new QLabel(centralwidget);
        label_less_2->setObjectName("label_less_2");
        label_less_2->setGeometry(QRect(380, 20, 51, 51));
        label_less_2->setTextFormat(Qt::TextFormat::AutoText);
        trackBarA = new QSlider(centralwidget);
        trackBarA->setObjectName("trackBarA");
        trackBarA->setGeometry(QRect(70, 150, 111, 25));
        trackBarA->setOrientation(Qt::Orientation::Horizontal);
        trackBarB = new QSlider(centralwidget);
        trackBarB->setObjectName("trackBarB");
        trackBarB->setGeometry(QRect(260, 150, 111, 25));
        trackBarB->setOrientation(Qt::Orientation::Horizontal);
        trackBarC = new QSlider(centralwidget);
        trackBarC->setObjectName("trackBarC");
        trackBarC->setGeometry(QRect(440, 150, 111, 25));
        trackBarC->setOrientation(Qt::Orientation::Horizontal);
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 619, 37));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MVC", nullptr));
        lineEditA->setText(QCoreApplication::translate("MainWindow", "10", nullptr));
        lineEditA->setPlaceholderText(QString());
        lineEditC->setText(QCoreApplication::translate("MainWindow", "90", nullptr));
        lineEditB->setText(QCoreApplication::translate("MainWindow", "50", nullptr));
        labelA->setText(QCoreApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:36pt;\">A</span></p></body></html>", nullptr));
        labelB->setText(QCoreApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:36pt;\">B</span></p></body></html>", nullptr));
        labelC->setText(QCoreApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:36pt;\">C</span></p></body></html>", nullptr));
        label_less_1->setText(QCoreApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:36pt;\">&lt;=</span></p></body></html>", nullptr));
        label_less_2->setText(QCoreApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:36pt;\">&lt;=</span></p></body></html>", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
