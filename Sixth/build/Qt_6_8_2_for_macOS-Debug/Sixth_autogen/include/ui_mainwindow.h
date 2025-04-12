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
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *circle;
    QAction *square;
    QAction *ellipse;
    QAction *rectangle;
    QAction *triangle;
    QAction *empty;
    QAction *chooseColor;
    QAction *group;
    QAction *ungroup;
    QWidget *centralwidget;
    QFrame *frame;
    QTextEdit *Commands;
    QLabel *CmdStory;
    QPushButton *clearHistory;
    QMenuBar *menubar;
    QMenu *menu;
    QMenu *menu_2;
    QMenu *menu_3;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 681);
        circle = new QAction(MainWindow);
        circle->setObjectName("circle");
        square = new QAction(MainWindow);
        square->setObjectName("square");
        ellipse = new QAction(MainWindow);
        ellipse->setObjectName("ellipse");
        rectangle = new QAction(MainWindow);
        rectangle->setObjectName("rectangle");
        triangle = new QAction(MainWindow);
        triangle->setObjectName("triangle");
        empty = new QAction(MainWindow);
        empty->setObjectName("empty");
        empty->setCheckable(false);
        chooseColor = new QAction(MainWindow);
        chooseColor->setObjectName("chooseColor");
        group = new QAction(MainWindow);
        group->setObjectName("group");
        ungroup = new QAction(MainWindow);
        ungroup->setObjectName("ungroup");
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        frame = new QFrame(centralwidget);
        frame->setObjectName("frame");
        frame->setGeometry(QRect(20, 20, 751, 491));
        frame->setFrameShape(QFrame::Shape::StyledPanel);
        frame->setFrameShadow(QFrame::Shadow::Raised);
        Commands = new QTextEdit(centralwidget);
        Commands->setObjectName("Commands");
        Commands->setEnabled(true);
        Commands->setGeometry(QRect(20, 540, 751, 74));
        Commands->setReadOnly(true);
        CmdStory = new QLabel(centralwidget);
        CmdStory->setObjectName("CmdStory");
        CmdStory->setGeometry(QRect(20, 520, 121, 16));
        clearHistory = new QPushButton(centralwidget);
        clearHistory->setObjectName("clearHistory");
        clearHistory->setGeometry(QRect(20, 620, 131, 32));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 37));
        menu = new QMenu(menubar);
        menu->setObjectName("menu");
        menu_2 = new QMenu(menubar);
        menu_2->setObjectName("menu_2");
        menu_3 = new QMenu(menubar);
        menu_3->setObjectName("menu_3");
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menu->menuAction());
        menubar->addAction(menu_2->menuAction());
        menubar->addAction(menu_3->menuAction());
        menu->addAction(circle);
        menu->addAction(square);
        menu->addAction(ellipse);
        menu->addAction(rectangle);
        menu->addAction(triangle);
        menu->addAction(empty);
        menu_2->addAction(chooseColor);
        menu_3->addSeparator();
        menu_3->addSeparator();
        menu_3->addAction(group);
        menu_3->addAction(ungroup);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        circle->setText(QCoreApplication::translate("MainWindow", "\320\232\321\200\321\203\320\263", nullptr));
        square->setText(QCoreApplication::translate("MainWindow", "\320\232\320\262\320\260\320\264\321\200\320\260\321\202", nullptr));
        ellipse->setText(QCoreApplication::translate("MainWindow", "\320\255\320\273\320\273\320\270\320\277\321\201", nullptr));
        rectangle->setText(QCoreApplication::translate("MainWindow", "\320\237\321\200\321\217\320\274\320\276\321\203\320\263\320\276\320\273\321\214\320\275\320\270\320\272", nullptr));
        triangle->setText(QCoreApplication::translate("MainWindow", "\320\242\321\200\320\265\321\203\320\263\320\276\320\273\321\214\320\275\320\270\320\272", nullptr));
        empty->setText(QCoreApplication::translate("MainWindow", ":)", nullptr));
        chooseColor->setText(QCoreApplication::translate("MainWindow", "\320\222\321\213\320\261\321\200\320\260\321\202\321\214 \321\206\320\262\320\265\321\202", nullptr));
        group->setText(QCoreApplication::translate("MainWindow", "\320\223\321\200\321\203\320\277\320\277\320\270\321\200\320\276\320\262\320\260\321\202\321\214", nullptr));
        ungroup->setText(QCoreApplication::translate("MainWindow", "\320\240\320\260\320\267\320\263\321\200\321\203\320\277\320\277\320\270\321\200\320\276\320\262\320\260\321\202\321\214", nullptr));
        CmdStory->setText(QCoreApplication::translate("MainWindow", "\320\230\321\201\321\202\320\276\321\200\320\270\321\217 \320\272\320\276\320\274\320\260\320\275\320\264:", nullptr));
        clearHistory->setText(QCoreApplication::translate("MainWindow", "\320\236\321\207\320\270\321\201\321\202\320\270\321\202\321\214 \320\270\321\201\321\202\320\276\321\200\320\270\321\216", nullptr));
        menu->setTitle(QCoreApplication::translate("MainWindow", "\320\244\320\270\320\263\321\203\321\200\321\213", nullptr));
        menu_2->setTitle(QCoreApplication::translate("MainWindow", "\320\246\320\262\320\265\321\202", nullptr));
        menu_3->setTitle(QCoreApplication::translate("MainWindow", "\320\223\321\200\321\203\320\277\320\277\320\270\321\200\320\276\320\262\320\272\320\260", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
