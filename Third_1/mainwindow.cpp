#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    storage.drawCircles(painter, ui->frame->geometry());
}

void MainWindow::mousePressEvent(QMouseEvent *event){

    if (event->button() == Qt::LeftButton){
        QRect frameGeom = ui->frame->geometry();
        QPoint PosInFrame = event->pos() - frameGeom.topLeft();

        int corner = 10;

        bool topLeft = (abs(PosInFrame.x()) < corner && abs(PosInFrame.y()) < corner);
        bool topRight = (abs(PosInFrame.x()-frameGeom.width()) < corner && abs(PosInFrame.y()) < corner);
        bool bottomLeft = (abs(PosInFrame.x()) < corner && abs(PosInFrame.y() - frameGeom.height()) < corner);
        bool bottomRight = (abs(PosInFrame.x()-frameGeom.width()) < corner && abs(PosInFrame.y() - frameGeom.height()) < corner);


        if (topLeft || topRight || bottomLeft || bottomRight) {
            isResizing = true;
            lastMousePos = event->pos();
            return;
        }

        if (frameGeom.contains(event->pos())) {
            storage.AddObject(event->pos().x(), event->pos().y());
            update();
        }


    }

}


void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    isResizing = false;
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (isResizing){
        int dx = event->pos().x() - lastMousePos.x();
        int dy = event->pos().y() - lastMousePos.y();

        ui->frame->resize(ui->frame->width() + dx, ui->frame->height() + dy);
        lastMousePos = event->pos();
    }
}




