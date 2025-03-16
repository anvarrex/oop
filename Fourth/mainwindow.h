#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include <QPainter>
#include <QMouseEvent>
#include <QColorDialog>

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
    virtual void move(int mouseX, int mouseY, QPoint lastMousePos, QRect frameGeom) {};

    virtual bool contains(int px, int py) = 0;
    virtual void setColor(QColor color){
        this->color = color;
    }
    virtual void resize(int mouseX, int mouseY, QPoint lastMousePoint, QRect frameGeom, bool isStartLine){};
    virtual bool isOnEdge(int px, int py) {return false;}
    virtual bool isStartLine(int px, int py){return false;}

    virtual bool intersectsWithFrame(const QRect frameRect) = 0;

    //virtual bool canMove(QRect frameGeom, int deltaX, int deltaY){return false;}

};


class Circle: public Shape{
private:
    int rad = 20;

public:
    Circle(int ValueX, int ValueY, QColor color, QRect frameGeom): Shape(ValueX, ValueY, color){
        if (x - rad < frameGeom.x()) x = rad+frameGeom.x();
        if (y - rad < frameGeom.y()) y = rad+frameGeom.y();
        if (x + rad > frameGeom.width()+frameGeom.x()) x = frameGeom.x() + frameGeom.width() - rad;
        if (y + rad > frameGeom.height()+frameGeom.y()) y = frameGeom.y() + frameGeom.height() - rad;
    }
    void draw(QPainter &painter) override;
    bool contains(int px, int py) override;
    void drawSelected(QPainter &painter) override;

    virtual void move(int mouseX, int mouseY, QPoint lastMousePos, QRect frameGeom) override{
        int dx = mouseX - lastMousePos.x();
        int dy = mouseY - lastMousePos.y();

        x += dx;
        y += dy;

        //Ограничение в пределах `QFrame`
        if (x - rad < frameGeom.x()) x = rad+frameGeom.x();
        if (y - rad < frameGeom.y()) y = rad+frameGeom.y();
        if (x + rad > frameGeom.right()) x = frameGeom.right() - rad;
        if (y + rad > frameGeom.bottom()) y = frameGeom.bottom() - rad;
    }
    bool intersectsWithFrame(const QRect frameRect) override {
        // Проверяем пересечение круга с прямоугольником
        QRect circleRect(x - rad, y - rad, 2 * rad, 2 * rad);
        return !frameRect.contains(circleRect);
    }

    // bool canMove(QRect frameGeom, int deltaX, int deltaY) override{
    //         int newX = x + rad + deltaX;
    //         int newY = y + rad + deltaY;

    //         if ((newX - rad < frameGeom.x()) || (newY - rad < frameGeom.y()) || (newX + rad > frameGeom.width()+frameGeom.x()) || (newY + rad > frameGeom.height()+frameGeom.y())){
    //             return false;
    //         }
    //         else {
    //             return true;
    //         }
    //     }



    bool isOnEdge(int px, int py) override {

        int distance = std::hypot(px - x, py - y);
        if (abs(distance - rad) < 5){
            qDebug() << "Edge";
            return true;
        }
        else {
            return false;
        } // Проверяем, близка ли точка к границе
    }

    void resize(int mouseX, int mouseY, QPoint lastMousePos, QRect frameGeom, bool isStartLine) override {
        int dx = mouseX - lastMousePos.x();
        int dy = mouseY - lastMousePos.y();
        rad += dx;
        rad += dy;
        if (rad < 15) rad = 15;

        if (x - rad < frameGeom.x()) rad = x - frameGeom.x();
        if (y - rad < frameGeom.y()) rad = y - frameGeom.y();
        if (x + rad > frameGeom.width()+frameGeom.x()) rad = frameGeom.x() + frameGeom.width() - x;
        if (y + rad > frameGeom.height()+frameGeom.y()) rad = frameGeom.y() + frameGeom.height() - y;
    }
};

class Rectangle: public Shape {
private:
    int width, height;
public:
    Rectangle(int ValueX, int ValueY, QColor color, QRect frameGeom): Shape(ValueX, ValueY, color), width(60), height(40){
        if (x < frameGeom.x()) x = frameGeom.x();
        if (y < frameGeom.y()) y = frameGeom.y();
        if (x + width > frameGeom.x()+frameGeom.width()) x = frameGeom.x() + frameGeom.width() - width;
        if (y + height > frameGeom.y()+frameGeom.height()) y =frameGeom.y() + frameGeom.height() - height;
    }
    void draw(QPainter &painter) override {
        painter.setBrush(color);
        painter.drawRect(x, y, width, height);
    }
    bool contains(int px, int py) override {
        return (px >= x && px <= x + width && py >= y && py <= y + height);
    }
    void drawSelected(QPainter &painter) override;

    bool intersectsWithFrame(const QRect frameRect) override {
        // Проверяем пересечение прямоугольника с фреймом
        QRect shapeRect(x, y, width, height);
        return !frameRect.contains(shapeRect);
    }

    bool isOnEdge(int px, int py) override{
        int edgeThreshold = 5;  // Толщина зоны для изменения размеров
        bool left = abs(px - x) < edgeThreshold;
        bool right = abs(px - (x + width)) < edgeThreshold;
        bool top = abs(py - y) < edgeThreshold;
        bool bottom = abs(py - (y + height)) < edgeThreshold;
        return left || right || top || bottom;
    }

    void move(int mouseX, int mouseY, QPoint lastMousePos, QRect frameGeom) override {
        int dx = mouseX - lastMousePos.x();
        int dy = mouseY - lastMousePos.y();

        x += dx;
        y += dy;

        if (x < frameGeom.x()) x = frameGeom.x();
        if (y < frameGeom.y()) y = frameGeom.y();
        if (x + width > frameGeom.x()+frameGeom.width()) x = frameGeom.x() + frameGeom.width() - width;
        if (y + height > frameGeom.y()+frameGeom.height()) y =frameGeom.y() + frameGeom.height() - height;
    }

    // bool canMove(QRect frameGeom, int deltaX, int deltaY) override {
    //     return (x < frameGeom.x()) || (y < frameGeom.y()) || (x + width > frameGeom.x()+frameGeom.width()) || (y + height > frameGeom.y()+frameGeom.height());
    // }

    void resize(int mouseX, int mouseY, QPoint lastMousePos, QRect frameGeom, bool isStartLine) override {
        int dx = mouseX - lastMousePos.x();
        int dy = mouseY - lastMousePos.y();
        width += dx;
        height += dy;
        width = max(60, width);
        height = max(40, height);

        if (x + width > frameGeom.x()+frameGeom.width()) width = frameGeom.x() + frameGeom.width() - x;
        if (y + height > frameGeom.y()+frameGeom.height()) height =frameGeom.y() + frameGeom.height() - y;
    }
};

class Ellipse : public Shape {
private:
    int widthR, heightR;

public:
    Ellipse(int ValueX, int ValueY, QColor color, QRect frameGeom) : Shape(ValueX, ValueY, color), widthR(30), heightR(20){

        // Проверяем, что эллипс не выходит за пределы области
        if (x - widthR < frameGeom.x()) x = frameGeom.x() + widthR;
        if (y - heightR < frameGeom.y()) y = frameGeom.y() + heightR;
        if (x + widthR > frameGeom.x() + frameGeom.width()) x = frameGeom.x() + frameGeom.width() - widthR;
        if (y + heightR > frameGeom.y() + frameGeom.height()) y = frameGeom.y() + frameGeom.height() - heightR;

    }

    bool intersectsWithFrame(const QRect frameRect) override {
        // Проверяем пересечение эллипса с фреймом
        QRect ellipseRect(x - widthR, y - heightR, 2 * widthR, 2 * heightR);
        return !frameRect.contains(ellipseRect);
    }


    void draw(QPainter &painter) override {
        painter.setBrush(color);
        painter.drawEllipse(QPoint(x, y), widthR, heightR);
    }

    bool contains(int px, int py) override {
        double dx = px - x;  // Смещение по X от центра
        double dy = py - y;  // Смещение по Y от центра

        double a = widthR;   // Полуось по X (горизонтальный радиус)
        double b = heightR;  // Полуось по Y (вертикальный радиус)

        return (dx * dx) / (a * a) + (dy * dy) / (b * b) <= 1.0;
    }

    void drawSelected(QPainter &painter) override;

    void move(int mouseX, int mouseY, QPoint lastMousePos, QRect frameGeom) override {
        int dx = mouseX - lastMousePos.x();
        int dy = mouseY - lastMousePos.y();

        x += dx;
        y += dy;

        // Проверяем, что эллипс не выходит за пределы области
        if (x - widthR < frameGeom.x()) x = frameGeom.x() + widthR;
        if (y - heightR < frameGeom.y()) y = frameGeom.y() + heightR;
        if (x + widthR > frameGeom.x() + frameGeom.width()) x = frameGeom.x() + frameGeom.width() - widthR;
        if (y + heightR > frameGeom.y() + frameGeom.height()) y = frameGeom.y() + frameGeom.height() - heightR;
    }

    // bool canMove(QRect frameGeom, int deltaX, int deltaY) override {
    //     return (x - widthR < frameGeom.x()) || (y - heightR < frameGeom.y()) || (x + widthR > frameGeom.x() + frameGeom.width()) || (y + heightR > frameGeom.y() + frameGeom.height());
    // }

    bool isOnEdge(int px, int py) override {
        double centerX = x;
        double centerY = y;

        double normX = (px - centerX) / (widthR);  // X-координата относительно центра
        double normY = (py - centerY) / (heightR); // Y-координата относительно центра

        // Проверка, лежит ли точка на границе эллипса (погрешность 0.3)
        double equation = normX * normX + normY * normY;
        return fabs(equation - 1.0) < 0.3;
    }

    void resize(int mouseX, int mouseY, QPoint lastMousePos, QRect frameGeom, bool isStartLine) override {
        // Смещение мыши
        int dx = mouseX - lastMousePos.x();
        int dy = mouseY - lastMousePos.y();

        // Изменение размеров эллипса
        widthR += dx;
        heightR += dy;

        // Ограничение минимального размера
        widthR = std::max(30, widthR);
        heightR = std::max(20, heightR);

        if (x - widthR < frameGeom.x()) widthR = x - frameGeom.x();
        if (y - heightR < frameGeom.y()) heightR= y - frameGeom.y();
        if (x + widthR > frameGeom.x() + frameGeom.width()) widthR = frameGeom.x() + frameGeom.width() - x;
        if (y + heightR > frameGeom.y() + frameGeom.height()) heightR = frameGeom.y() + frameGeom.height() - y;

    }
};

class Triangle: public Shape {
private:
    int size;
public:
    Triangle(int ValueX, int ValueY, QColor color, QRect frameGeom): Shape(ValueX, ValueY, color), size(30){
        if (x - size < frameGeom.x()) x = size+frameGeom.x();
        if (y - size < frameGeom.y()) y = size+frameGeom.y();
        if (x + size > frameGeom.width()+frameGeom.x()) x = frameGeom.x() + frameGeom.width() - size;
        if (y + size > frameGeom.height()+frameGeom.y()) y = frameGeom.y() + frameGeom.height() - size;
    }
    void draw(QPainter &painter) override {
        painter.setBrush(color);
        QPolygon polygon;
        polygon << QPoint(x, y - size)
                << QPoint(x - size, y + size)
                << QPoint(x + size, y + size);
        painter.drawPolygon(polygon);
    }

    bool intersectsWithFrame(const QRect frameRect) override {
        // Проверяем пересечение треугольника с фреймом
        QPolygon trianglePolygon;
        trianglePolygon << QPoint(x, y - size)
                        << QPoint(x - size, y + size)
                        << QPoint(x + size, y + size);
        return !frameRect.contains(trianglePolygon.boundingRect());
    }

    bool contains(int px, int py) override {
        QPoint A(x, y - size);
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
    void move(int mouseX, int mouseY, QPoint lastMousePos, QRect frameGeom) override {
        int dx = mouseX - lastMousePos.x();
        int dy = mouseY - lastMousePos.y();

        x += dx;
        y += dy;

        if (x - size < frameGeom.x()) x = size+frameGeom.x();
        if (y - size < frameGeom.y()) y = size+frameGeom.y();
        if (x + size > frameGeom.width()+frameGeom.x()) x = frameGeom.x() + frameGeom.width() - size;
        if (y + size > frameGeom.height()+frameGeom.y()) y = frameGeom.y() + frameGeom.height() - size;
    }

    // bool canMove(QRect frameGeom, int deltaX, int deltaY) override {
    //     return (x - size < frameGeom.x()) || (y - size < frameGeom.y()) || (x + size > frameGeom.width()+frameGeom.x()) || (y + size > frameGeom.height()+frameGeom.y());
    // }

    bool isOnEdge(int px, int py) override {
        QPoint A(x, y - size);
        QPoint B(x - size, y + size);
        QPoint C(x + size, y + size);
        QPoint P(px, py);

        auto distance = [](QPoint p1, QPoint p2) {
            return sqrt(pow(p1.x() - p2.x(), 2) + pow(p1.y() - p2.y(), 2));
        };

        auto pointToLineDistance = [&](QPoint A, QPoint B, QPoint P) {
            double area = abs((B.x() - A.x()) * (A.y() - P.y()) - (A.x() - P.x()) * (B.y() - A.y()));
            double base = distance(A, B);
            return area / base;
        };

        int threshold = 5; // Точность попадания на край
        return (pointToLineDistance(A, B, P) < threshold ||
                pointToLineDistance(B, C, P) < threshold ||
                pointToLineDistance(C, A, P) < threshold);
    }

    void resize(int mouseX, int mouseY, QPoint lastMousePos, QRect frameGeom, bool isStartLine) override {
        int dx = mouseX - lastMousePos.x();
        int dy = mouseY - lastMousePos.y();

        int delta;
        if (dy < 0 || dx < 0){
            delta = std::min(dx, dy);
        }
        else {
            delta = std::max(dx, dy);
        }


        size += delta;
        size = std::max(30, size);

        if (x - size < frameGeom.x()) size = x - frameGeom.x();
        if (y - size < frameGeom.y()) size = y - frameGeom.y();
        if (x + size > frameGeom.width()+frameGeom.x()) size = frameGeom.x() + frameGeom.width() - x;
        if (y + size > frameGeom.height()+frameGeom.y()) size = frameGeom.y() + frameGeom.height() - y;
    }
};

class Square: public Shape {
private:
    int width, height;
public:
    Square(int ValueX, int ValueY, QColor color, QRect frameGeom): Shape(ValueX, ValueY, color), width(40), height(40){
        if (x < frameGeom.x()) x = frameGeom.x();
        if (y < frameGeom.y()) y = frameGeom.y();
        if (x + width > frameGeom.x()+frameGeom.width()) x = frameGeom.x() + frameGeom.width() - width;
        if (y + height > frameGeom.y()+frameGeom.height()) y = frameGeom.y() + frameGeom.height() - height;
    }
    void draw(QPainter &painter) override {
        painter.setBrush(color);
        painter.drawRect(x, y, width, height);
    }

    bool contains(int px, int py) override {
        return (px >= x && px <= x + width && py >= y && py <= y + height);
    }

    bool intersectsWithFrame(const QRect frameRect) override {
        // Проверяем пересечение квадрата с фреймом
        QRect shapeRect(x, y, width, height);
        return !frameRect.contains(shapeRect);
    }

    void drawSelected(QPainter &painter) override;
    bool isOnEdge(int px, int py) override{
        int edgeThreshold = 5;  // Толщина зоны для изменения размеров
        bool left = abs(px - x) < edgeThreshold;
        bool right = abs(px - (x + width)) < edgeThreshold;
        bool top = abs(py - y) < edgeThreshold;
        bool bottom = abs(py - (y + height)) < edgeThreshold;
        return left || right || top || bottom;
    }

    void move(int mouseX, int mouseY, QPoint lastMousePos, QRect frameGeom) override {
        int dx = mouseX - lastMousePos.x();
        int dy = mouseY - lastMousePos.y();

        x += dx;
        y += dy;

        if (x < frameGeom.x()) x = frameGeom.x();
        if (y < frameGeom.y()) y = frameGeom.y();
        if (x + width > frameGeom.x()+frameGeom.width()) x = frameGeom.x() + frameGeom.width() - width;
        if (y + height > frameGeom.y()+frameGeom.height()) y =frameGeom.y() + frameGeom.height() - height;
    }

    // bool canMove(QRect frameGeom, int deltaX, int deltaY) override{
    //     return (x < frameGeom.x()) ||  (y < frameGeom.y()) || (x + width > frameGeom.x()+frameGeom.width()) || (y + height > frameGeom.y()+frameGeom.height());
    // }

    void resize(int mouseX, int mouseY, QPoint lastMousePos, QRect frameGeom, bool isStartLine) override {
        int dx = mouseX - lastMousePos.x();
        int dy = mouseY - lastMousePos.y();

        int delta;

        if (dy < 0 || dx < 0){
            delta = std::min(dx, dy);
        }
        else {
            delta = std::max(dx, dy);
        }


        width += delta;
        height += delta;

        int minSizeSquare = 40;

        width = std::max(minSizeSquare, width);
        height = std::max(minSizeSquare, height);

        if (x + width > frameGeom.x()+frameGeom.width()) width = frameGeom.x() + frameGeom.width() - x;
        if (y + height > frameGeom.y()+frameGeom.height()) height =frameGeom.y() + frameGeom.height() - y;
    }
};

class Line : public Shape {
private:
    int xEnd, yEnd;
public:
    Line(int xStart, int yStart, int xEnd, int yEnd, QColor color): Shape(xStart, yStart, color), xEnd(xEnd), yEnd(yEnd) {}

    void draw(QPainter &painter) override {
        QPen pen;
        pen.setWidth(3);
        pen.setColor(color);
        painter.setPen(pen);
        painter.drawLine(x, y, xEnd, yEnd);
        painter.setPen(QPen(Qt::black, 0.5));
    }

    bool intersectsWithFrame(const QRect frameRect) override {
        // Проверяем пересечение линии с фреймом
        return !frameRect.contains(x, y) || !frameRect.contains(xEnd, yEnd);
    }

    void move(int mouseX, int mouseY, QPoint lastMousePos, QRect geomFrame) override {
        int dx = mouseX - lastMousePos.x();
        int dy = mouseY - lastMousePos.y();

        // Проверяем, где будет новая начальная точка
        int newX = x + dx;
        int newY = y + dy;

        // Проверяем, где будет новая конечная точка
        int newXEnd = xEnd + dx;
        int newYEnd = yEnd + dy;

        // Проверяем, не выйдут ли обе точки за границы фрейма
        bool isOutOfBounds = false;

        if (newX < geomFrame.x() || newY < geomFrame.y() ||
            newX > geomFrame.x() + geomFrame.width() || newY > geomFrame.y() + geomFrame.height()) {
            isOutOfBounds = true;
        }

        if (newXEnd < geomFrame.x() || newYEnd < geomFrame.y() ||
            newXEnd > geomFrame.x() + geomFrame.width() || newYEnd > geomFrame.y() + geomFrame.height()) {
            isOutOfBounds = true;
        }

        // Если хотя бы одна точка вышла за границу, движение останавливается
        if (!isOutOfBounds) {
            x = newX;
            y = newY;
            xEnd = newXEnd;
            yEnd = newYEnd;
        }
    }

    // bool canMove(QRect frameGeom, int deltaX, int deltaY) override {
    //     return x < frameGeom.x() || y < frameGeom.y() || x > frameGeom.left() || y > frameGeom.bottom() || xEnd < frameGeom.x() || yEnd < frameGeom.y() || xEnd > frameGeom.left() || yEnd > frameGeom.bottom();
    // }


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
    bool isOnEdge(int px, int py) override{
        // Длина линии
        double length = std::sqrt((xEnd - x) * (xEnd - x) + (yEnd - y) * (yEnd - y));

        // Вычисляем расстояния от курсора до начала и конца линии
        double distToStart = std::hypot(px- x, py - y);
        double distToEnd = std::hypot(px - xEnd, py - yEnd);


        return distToStart < length / 4 || distToEnd < length / 4;
        // if (distToStart < length / 4) {
        //     x += dx;
        //     y += dy;
        // } else (distToEnd < length / 4) {
        //     xEnd += dx;
        //     yEnd += dy;
        // }
    }
    bool isStartLine(int px, int py) override {
        double length = std::sqrt((xEnd - x) * (xEnd - x) + (yEnd - y) * (yEnd - y));

        // Вычисляем расстояния от курсора до начала и конца линии
        double distToStart = std::hypot(px- x, py - y);
        if (distToStart<length/4){
            return true;
        }
        else {
            return false;
        }
    }
    void resize(int mouseX, int mouseY, QPoint lastMousePos, QRect frameGeom, bool isStartLine) override {
        int dx = mouseX - lastMousePos.x();
        int dy = mouseY - lastMousePos.y();

        qDebug() << "исСтартЛайн" <<isStartLine;        // Изменение начала линии
        if (isStartLine) {
            // Проверяем, не выйдет ли начало линии за границу фрейма
            int newX = x + dx;
            int newY = y + dy;

            if (frameGeom.contains(newX, newY)) {
                x = newX;
                y = newY;
            }
        }
        // Изменение конца линии
        else  {
            // Проверяем, не выйдет ли конец линии за границу фрейма
            int newXEnd = xEnd + dx;
            int newYEnd = yEnd + dy;

            if (frameGeom.contains(newXEnd, newYEnd)) {
                xEnd = newXEnd;
                yEnd = newYEnd;
            }
        }
    }
};

class MyStorage{
private:
    vector<Shape*> shapes;
    vector<Shape*> selected_shapes;
public:

    void AddObject(Shape *shape);

    void drawShapes(QPainter &painter);

    void selectShapes(vector <Shape*> shapesInPoint, bool ctrl);

    void drawSelectedShapes(QPainter &painter);

    void deleteShapes();

    void setColor(QColor color);

    vector <Shape*> GetSelectedShapes(){
        return selected_shapes;
    }

    vector <Shape*> GetShapes(){
        return shapes;
    }

    void moveShape(int mouseX, int mouseY, QPoint lastMousePos, QRect frameGeom)
    {
        for (Shape* selected_shape: selected_shapes)
        {
            selected_shape->move(mouseX, mouseY, lastMousePos, frameGeom);
        }
    }

    void resizeShape(int x, int y, QPoint lastMousePos, QRect frameGeom, bool isStartLine){
        for (Shape* selected_shape: selected_shapes){
            selected_shape->resize(x, y, lastMousePos, frameGeom, isStartLine);
        }
    }

    bool isStartLine(int px, int py, Shape* line){
        return line->isStartLine(px, py);
    }

    void selectAll(){
        selected_shapes = shapes;
    }

    // bool canMove(QRect frameGeom, int deltaX, int deltaY){
    //     for (Shape* shape: shapes){
    //         qDebug() << "canMove" << shape->canMove(frameGeom, deltaX, deltaY);
    //         return shape->canMove(frameGeom, deltaX, deltaY);
    //     }
    //     return false;
    // }

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
        selected_shape = "Square";
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
    QPoint lineStart, lineEndTemporary;
    QColor selected_color = Qt::yellow;
    QPoint MousePosCirclePress;
    string selected_shape = "Circle";
    bool isStartLine = false;
    bool fl;
    bool isDrawingLine = false;
    bool isMoving = false;
    bool isResizing = false;
    bool ctrl = false;
    bool selecting = false;
    bool isResizingShape = false;
    vector <Shape*> shapesInPoint;
};

#endif // MAINWINDOW_H
