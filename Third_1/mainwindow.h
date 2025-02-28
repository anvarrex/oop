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
    bool contains(int px, int py){
        int dx = x - px;
        int dy = y - py;
        return (dx * dx + dy * dy) <= (rad * rad);
    }
    void select(QPainter &painter){
        painter.setBrush(Qt::darkBlue);
        painter.drawEllipse(x - rad, y - rad, 2 * rad, 2 * rad);
    }
    auto GetX(){
        return x;
    }
    auto GetY(){
        return y;
    }
};


class MyStorage{
private:
    vector<Circle*> circles;
    vector<Circle*> selected_circles;
    // int Rad = 10;
public:

    void AddObject(int x, int y){
        circles.push_back(new Circle(x, y));
        selected_circles.clear();
    }

    void drawCircles(QPainter &painter, QRect restriction){
        for (int i = 0; i<circles.size();i++){
            circles[i]->draw(painter, restriction);
        }
    }

    bool isCircle(int x, int y, bool ctrl){
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


    void selectCircles(QPainter &painter){
        for (int i = 0; i<selected_circles.size();i++) {
            qDebug() << "метод selectCircles" << selected_circles;
            selected_circles[i]->select(painter);
        }
    }

    void deleteCircles(){
        for (int i = 0; i<selected_circles.size();i++) {
            circles.erase(remove(circles.begin(), circles.end(), selected_circles[i]), circles.end());
        }
        selected_circles.clear();
    }

    void selectCirclesInRect(QRect Rect){
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
