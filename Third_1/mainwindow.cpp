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

        int newWidth = qMax(100, ui->frame->width() + dx);
        int newHeight = qMax(80, ui->frame->height() + dy);
        ui->frame->resize(newWidth, newHeight);
        lastMousePos = event->pos();
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



void Circle::draw(QPainter &painter, QRect restriction)
{
    painter.setBrush(Qt::blue);
    painter.setClipRect(restriction);
    painter.drawEllipse(x - rad, y - rad, 2 * rad, 2 * rad);

}

bool Circle::contains(int px, int py)
{
    int dx = x - px;
    int dy = y - py;
    return (dx * dx + dy * dy) <= (rad * rad);
}

void Circle::select(QPainter &painter)
{
    painter.setBrush(Qt::darkBlue);
    painter.drawEllipse(x - rad, y - rad, 2 * rad, 2 * rad);
}

void MyStorage::AddObject(int x, int y)
{
    circles.push_back(new Circle(x, y));
    selected_circles.clear();
}

void MyStorage::drawCircles(QPainter &painter, QRect restriction)
{
    for (int i = 0; i<circles.size();i++){
        circles[i]->draw(painter, restriction);
    }
}

bool MyStorage::isCircle(int x, int y, bool ctrl)
{
    bool in = false;
    vector<Circle*> circlesInPoint;
    vector<Circle*> unselectedCircles;
    for (int i = 0; i<circles.size();i++){
        if (circles[i]->contains(x, y)){
            circlesInPoint.push_back(circles[i]);
            if (find(selected_circles.begin(), selected_circles.end(), circles[i]) == selected_circles.end())
            {
                if (ctrl == false){
                    selected_circles.clear();
                    selected_circles.push_back(circles[i]);
                }
                else {
                    selected_circles.push_back(circles[i]);
                }

            }

            else {
                selected_circles.erase(remove(selected_circles.begin(), selected_circles.end(), circles[i]), selected_circles.end());
                unselectedCircles.push_back(circles[i]);

            }
            in = true;
        }
    }

    qDebug() << "Selected метода isCircle" <<selected_circles;
    qDebug() << "InPoint метода isCircle" << circlesInPoint;
    if (circlesInPoint.size() > 1){
        if (circlesInPoint != unselectedCircles){
            for (int i = 0; i<circlesInPoint.size(); i++) {
                if (find(selected_circles.begin(), selected_circles.end(), circlesInPoint[i]) == selected_circles.end())
                {
                    selected_circles.push_back(circlesInPoint[i]);

                }
            }
        }
    }
    circlesInPoint.clear();
    unselectedCircles.clear();

    return in;
}

void MyStorage::selectCircles(QPainter &painter)
{
    for (int i = 0; i<selected_circles.size();i++) {
        qDebug() << "метод selectCircles" << selected_circles;
        selected_circles[i]->select(painter);
    }
}

void MyStorage::deleteCircles()
{
    for (int i = 0; i<selected_circles.size();i++) {
        circles.erase(remove(circles.begin(), circles.end(), selected_circles[i]), circles.end());
    }
    selected_circles.clear();
}

void MyStorage::selectCirclesInRect(QRect Rect)
{
    qDebug() << "метод selectCirclesInRect";
    for (int i = 0; i<circles.size();i++) {
        QRect circleRect(circles[i]->GetX() - 20, circles[i]->GetY() - 20, 40, 40);
        if (Rect.intersects(circleRect)) {
            if (find(selected_circles.begin(), selected_circles.end(), circles[i]) == selected_circles.end()){
                qDebug() << "метод selectcirclesinrect добавлен";
                selected_circles.push_back(circles[i]);
            }
        } else {
            selected_circles.erase(remove(selected_circles.begin(), selected_circles.end(), circles[i]), selected_circles.end());
        }
    }
}
