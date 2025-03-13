#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include <QPainter>
#include <QMouseEvent>
#include <QColorDialog>
#include <set>

using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class Shape{
protected:
    int x, y;
    QColor color;
public:
    Shape(int x, int y, QColor color): x(x), y(y), color(color){};
    virtual void draw(QPainter &painter) = 0;
    virtual void drawSelected(QPainter &painter){
        qDebug() << color;
        QPen pen(Qt::DashLine);
        pen.setColor(Qt::black);
        qDebug() << "ЭЭ";
        pen.setWidth(2);
        painter.setPen(pen);
    };
    virtual void move(int mouseX, int mouseY, QPoint lastMousePos)
    {
        int dx = mouseX - lastMousePos.x();
        int dy = mouseY - lastMousePos.y();

        x += dx;
        y += dy;
    }

    virtual bool contains(int px, int py) = 0;
    virtual void setColor(QColor color){
        this->color = color;
    }
};


class Circle: public Shape{
private:
    const int rad = 20;

public:
    Circle(int ValueX, int ValueY, QColor color): Shape(ValueX, ValueY, color){}
    void draw(QPainter &painter) override;
    bool contains(int px, int py) override;
    void drawSelected(QPainter &painter) override;
};

class Rectangle: public Shape {
private:
    int width, height;
public:
    Rectangle(int x, int y, QColor color) : Shape(x, y, color), width(60), height(40){}
    void draw(QPainter &painter) override {
        painter.setBrush(color);
        painter.drawRect(x, y, width, height);
    }
    bool contains(int px, int py) override {
        return (px >= x && px <= x + width && py >= y && py <= y + height);
    }
    void drawSelected(QPainter &painter) override;
};

class Ellipse : public Shape {
private:
    int width, height;

public:
    Ellipse(int x, int y, QColor color) : Shape(x, y, color), width(60), height(40){}

    void draw(QPainter &painter) override {
        painter.setBrush(color);
        painter.drawEllipse(x, y, width, height);
    }

    bool contains(int px, int py) override {
        int dx = px - (x + width / 2);
        int dy = py - (y + height / 2);
        return (dx * dx) / (width * width / 4) + (dy * dy) / (height * height / 4) <= 1;
    }
    void drawSelected(QPainter &painter) override;
};

class Triangle: public Shape {
private:
    int size;
public:
    Triangle(int x, int y, QColor color): Shape(x, y, color), size(30){}
    void draw(QPainter &painter) override {
        painter.setBrush(color);
        QPolygon polygon;
        polygon << QPoint(x, y - 20)
                << QPoint(x - size, y + size)
                << QPoint(x + size, y + size);
        painter.drawPolygon(polygon);
    }
    bool contains(int px, int py) override {
        QPoint A(x, y - 20);
        QPoint B(x - size, y + size);
        QPoint C(x + size, y + size);
        QPoint P(px, py);

        auto area = [](QPoint p1, QPoint p2, QPoint p3) {
            return abs((p1.x() * (p2.y() - p3.y()) +
                        p2.x() * (p3.y() - p1.y()) +
                        p3.x() * (p1.y() - p2.y())) / 2.0);
        };

        double totalArea = area(A, B, C);
        double area1 = area(P, B, C);
        double area2 = area(A, P, C);
        double area3 = area(A, B, P);

        return fabs(totalArea - (area1 + area2 + area3)) < 1e-6;
    }
    void drawSelected(QPainter &painter) override;
};

class Square: public Shape {
private:
    int width, height;
public:
    Square(int x, int y, QColor color): Shape(x, y, color), width(40), height(40){}
    void draw(QPainter &painter) override {
        painter.setBrush(color);
        painter.drawRect(x, y, width, height);
    }

    bool contains(int px, int py) override {
        return (px >= x && px <= x + width && py >= y && py <= y + height);
    }
    void drawSelected(QPainter &painter) override;
};

class Line : public Shape {
private:
    int xEnd, yEnd;
public:
    Line(int x, int y, int xEnd, int yEnd, QColor color): Shape(x, y, color), xEnd(xEnd), yEnd(yEnd) {}

    void draw(QPainter &painter) override {
        QPen pen;
        pen.setWidth(3);
        pen.setColor(color);
        painter.setPen(pen);
        painter.drawLine(x, y, xEnd, yEnd);
    }

    bool contains(int px, int py) override {
        int penWidth = 3;
        double epsilon = penWidth / 2.0 + 1.0;
        int dx = xEnd - x;
        int dy = yEnd - y;
        double length = std::sqrt(dx * dx + dy * dy);

        if (length == 0) {
            // Отрезок вырожден в точку, проверяем попадание в "круг" радиусом penWidth / 2
            return std::hypot(px - x, py - y) <= epsilon;
        }

        double ux = dx / length;
        double uy = dy / length;

        double perpX = -uy;
        double perpY = ux;

        double projLength = (px - x) * ux + (py - y) * uy;
        double perpDistance = std::abs((px - x) * perpX + (py - y) * perpY);


        return (0 <= projLength && projLength <= length) && (perpDistance <= epsilon);
    }
    void drawSelected(QPainter &painter) override;
};

class MyStorage{
private:
    vector<Shape*> shapes;
    vector<Shape*> selected_shapes;
public:

    void AddObject(Shape *shape);

    void drawShapes(QPainter &painter);

    vector <Shape*> shapesInPoint(int x, int y);

    void selectShapes(vector <Shape*> shapesInPoint, bool ctrl);

    void drawSelectedShapes(QPainter &painter);

    void deleteShapes();

    void setColor(QColor color);

    vector <Shape*> GetSelectedShapes(){
        return selected_shapes;
    }

    void moveShape(int mouseX, int mouseY, QPoint lastMousePos)
    {
        // for (Shape* selected_shape: selected_shapes){
        //     if (find(shapesToMove.begin(), shapesToMove.end(), selected_shape) == shapesToMove.end()){
        //         shapesToMove.push_back(selected_shape);
        //     }
        // }
        for (Shape* selected_shape: selected_shapes)
        {
            selected_shape->move(mouseX, mouseY, lastMousePos);
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
    void openColorDialog();
    void setCircle(){
        selected_shape = "Circle";
    }
    void setSqaure(){
        selected_shape = "Sqaure";
    }
    void setEllipse(){
        selected_shape = "Ellipse";
    }
    void setRectangle(){
        selected_shape = "Rectangle";
    }
    void setLine(){
        selected_shape = "Line";
    }
    void setTriangle(){
        selected_shape = "Triangle";
    }


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
    string selected_shape = "Circle";
    bool isDrawingLine = false;
    QPoint lineStart;
    QColor selected_color = Qt::yellow;
    bool isMoving = false;
    //vector <Shape*> shapesToMove;
};

#endif // MAINWINDOW_H
