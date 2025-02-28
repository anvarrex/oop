#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}


int counter = 0;

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    storage.drawCircles(painter, ui->frame->geometry());
    storage.selectCircles(painter);

    if (selecting){
        painter.setPen(QPen(Qt::black, 2, Qt::DashLine));
        painter.setBrush(Qt::NoBrush);
        painter.drawRect(selectionRect.normalized());
    }

}

void MainWindow::mousePressEvent(QMouseEvent *event){
    qDebug() << "Маус пресс ивент";

    if (event->button() == Qt::LeftButton){

        QRect frameGeom = ui->frame->geometry();
        QPoint PosInFrame = event->pos() - frameGeom.topLeft();
        MousePosCirclePress = event->pos();
        int corner = 10;

        bool topLeft = (abs(PosInFrame.x()) < corner && abs(PosInFrame.y()) < corner);
        bool topRight = (abs(PosInFrame.x()-frameGeom.width()) < corner && abs(PosInFrame.y()) < corner);
        bool bottomLeft = (abs(PosInFrame.x()) < corner && abs(PosInFrame.y() - frameGeom.height()) < corner);
        bool bottomRight = (abs(PosInFrame.x()-frameGeom.width()) < corner && abs(PosInFrame.y() - frameGeom.height()) < corner);

        selecting = true;

        if (storage.isCircle(event->pos().x(), event->pos().y(), ctrl)){
            fl = true;
            selecting = false;
            update();

        }

        if (topLeft || topRight || bottomLeft || bottomRight) {
            isResizing = true;
            lastMousePos = event->pos();
            return;
        }

    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    qDebug() << "Маус мув ивент";
    if (selecting && ctrl == false && ui->frame->geometry().contains(MousePosCirclePress) && ui->frame->geometry().contains(event->pos())) {
        selectionRect.setTopLeft(MousePosCirclePress);
        selectionRect.setBottomRight(event->pos());
        storage.selectCirclesInRect(selectionRect);
        update();
    }


    if (isResizing){
        selecting = false;

        int dx = event->pos().x() - lastMousePos.x();
        int dy = event->pos().y() - lastMousePos.y();

        ui->frame->resize(ui->frame->width() + dx, ui->frame->height() + dy);
        lastMousePos = event->pos();
        return;
    }
}


void MainWindow::mouseReleaseEvent(QMouseEvent *event)

{
    qDebug() << "Маус релиз ивент";
    isResizing = false;
    selecting = false;
    update();

    QRect frameGeom = ui->frame->geometry();
    QPoint PosInFrame = event->pos() - frameGeom.topLeft();

    int corner = 10;

    bool topLeft = (abs(PosInFrame.x()) < corner && abs(PosInFrame.y()) < corner);
    bool topRight = (abs(PosInFrame.x()-frameGeom.width()) < corner && abs(PosInFrame.y()) < corner);
    bool bottomLeft = (abs(PosInFrame.x()) < corner && abs(PosInFrame.y() - frameGeom.height()) < corner);
    bool bottomRight = (abs(PosInFrame.x()-frameGeom.width()) < corner && abs(PosInFrame.y() - frameGeom.height()) < corner);


    if (topLeft || topRight || bottomLeft || bottomRight) {
        return;
    }

    if (MousePosCirclePress == event->pos()){
        if (ui->frame->geometry().contains(event->pos()) && !fl) {
            qDebug() << ++counter ;
            if (ctrl == false)
            {
                storage.AddObject(event->pos().x(), event->pos().y());
            }
            update();
        }
    }
    fl = false;
}


void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Backspace || event->key() == Qt::Key_Delete) {
        qDebug() << "Del нажата!";
        storage.deleteCircles();
        update();
    }

    else if (event->key() == Qt::Key_Control){
        qDebug() << "Ctrl нажата!";
        ctrl = true;
    }


    QMainWindow::keyPressEvent(event);
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Control){
        qDebug() << "Ctrl отпущена!";
        ctrl = false;
    }
}






