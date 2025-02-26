#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QMouseEvent>

using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE


class Circle{
private:
    int x, y;
    const int rad = 20;

public:
    Circle(int ValueX, int ValueY):x(ValueX), y(ValueY){}
    void draw(QPainter &painter, QRect restriction){
        painter.setBrush(Qt::blue);
        painter.setClipRect(restriction);
        painter.drawEllipse(x - rad, y - rad, 2 * rad, 2 * rad);
    };
};


class MyStorage{
private:
    vector<Circle*> circles;
    // int Rad = 10;
public:

    void AddObject(int x, int y){
        circles.push_back(new Circle(x, y));
    }

    void drawCircles(QPainter &painter, QRect restriction){
        for (int i = 0; i<circles.size();i++){
            circles[i]->draw(painter, restriction);
        }
    }

    ~MyStorage(){
        for (int i = 0; i<circles.size();i++){
            delete circles[i];
            }
        }
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    Ui::MainWindow *ui;
    MyStorage storage;
    QPoint lastMousePos;
    bool isResizing = false;
};


#endif // MAINWINDOW_H
