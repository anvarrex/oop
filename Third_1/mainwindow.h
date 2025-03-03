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
    void draw(QPainter &painter);
    bool contains(int px, int py);
    void draw_selected(QPainter &painter);

    int GetX(){
        return x;
    }
    int GetY(){
        return y;
    }
};


class MyStorage{
private:
    vector<Circle*> circles;
    vector<Circle*> selected_circles;
public:

    void AddObject(int x, int y);

    void drawCircles(QPainter &painter);

    bool isCircle(int x, int y, bool ctrl);

    void selectCircles(QPainter &painter);

    void deleteCircles();

    void selectCirclesInRect(QRect Rect);

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
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private:
    Ui::MainWindow *ui;
    MyStorage storage;
    QPoint lastMousePos;
    bool isResizing = false;
    bool ctrl = false;
    bool selecting = false;
    QRect selectionRect;
    QPoint MousePosCirclePress;
    bool fl;
};

#endif // MAINWINDOW_H
