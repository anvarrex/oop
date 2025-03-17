#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include <QPainter>
#include <QMouseEvent>
#include <QColorDialog>
#include <QMessageBox>

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
    virtual void drawSelected(QPainter &painter);
    void move(int mouseX, int mouseY, QPoint lastMousePos, QRect frameGeom);

    virtual bool contains(int px, int py) = 0;
    virtual void setColor(QColor color);
    virtual void resize(int mouseX, int mouseY, QPoint lastMousePoint, QRect frameGeom, bool isStartLine) = 0;
    virtual bool isOnEdge(int px, int py) {return false;}
    virtual bool isStartLine(int px, int py){return false;}

    virtual bool intersectsWithFrame(const QRect frameRect) = 0;

    virtual QRect getBoundingBox() const = 0;


    virtual Shape* copy() = 0;
    virtual void setOffset(int offset);
};


class Circle: public Shape{
private:
    int rad = 20;

public:
    Circle(int ValueX, int ValueY, QColor color, QRect frameGeom);
    void draw(QPainter &painter) override;
    bool contains(int px, int py) override;
    void drawSelected(QPainter &painter) override;


    bool intersectsWithFrame(const QRect frameRect) override;

    bool isOnEdge(int px, int py) override;

    void resize(int mouseX, int mouseY, QPoint lastMousePos, QRect frameGeom, bool isStartLine) override;

    Shape* copy() override;

    virtual QRect getBoundingBox() const override {
        return QRect(x - rad, y - rad, 2 * rad, 2 * rad);
    }
};

class Rectangle: public Shape {
private:
    int width, height;
public:
    Rectangle(int ValueX, int ValueY, QColor color, QRect frameGeom);
    void draw(QPainter &painter) override;
    bool contains(int px, int py) override;
    void drawSelected(QPainter &painter) override;

    bool intersectsWithFrame(const QRect frameRect) override;

    bool isOnEdge(int px, int py) override;

    void resize(int mouseX, int mouseY, QPoint lastMousePos, QRect frameGeom, bool isStartLine) override;

    Shape* copy() override;

    QRect getBoundingBox() const override {
        return QRect(x, y, width, height);
    }
};

class Ellipse : public Shape {
private:
    int widthR, heightR;

public:
    Ellipse(int ValueX, int ValueY, QColor color, QRect frameGeom);

    bool intersectsWithFrame(const QRect frameRect) override;

    void draw(QPainter &painter) override;

    bool contains(int px, int py) override;

    void drawSelected(QPainter &painter) override;

    bool isOnEdge(int px, int py) override;

    void resize(int mouseX, int mouseY, QPoint lastMousePos, QRect frameGeom, bool isStartLine) override;

    Shape* copy() override;

    QRect getBoundingBox() const override {
        return QRect(x - widthR, y - heightR, 2 * widthR, 2 * heightR);
    }
};

class Triangle: public Shape {
private:
    int size;
public:
    Triangle(int ValueX, int ValueY, QColor color, QRect frameGeom);
    void draw(QPainter &painter) override;

    bool intersectsWithFrame(const QRect frameRect) override;

    bool contains(int px, int py) override;
    void drawSelected(QPainter &painter) override;

    bool isOnEdge(int px, int py) override;

    void resize(int mouseX, int mouseY, QPoint lastMousePos, QRect frameGeom, bool isStartLine) override;

    QRect getBoundingBox() const override {
        QPolygon trianglePolygon;
        trianglePolygon << QPoint(x, y - size)
                        << QPoint(x - size, y + size)
                        << QPoint(x + size, y + size);
        return trianglePolygon.boundingRect();
    }

    Shape* copy() override;
};

class Square: public Shape {
private:
    int width, height;
public:
    Square(int ValueX, int ValueY, QColor color, QRect frameGeom);
    void draw(QPainter &painter) override;

    bool contains(int px, int py) override;

    bool intersectsWithFrame(const QRect frameRect) override;

    void drawSelected(QPainter &painter) override;
    bool isOnEdge(int px, int py) override;

    void resize(int mouseX, int mouseY, QPoint lastMousePos, QRect frameGeom, bool isStartLine) override;

    QRect getBoundingBox() const override{
        return QRect(x, y, width, height);
    }

    Shape* copy() override;
};

class Line : public Shape {
private:
    int xEnd, yEnd;
public:
    Line(int xStart, int yStart, int xEnd, int yEnd, QColor color);

    void draw(QPainter &painter) override;

    bool intersectsWithFrame(const QRect frameRect) override;
    bool contains(int px, int py) override;
    void drawSelected(QPainter &painter) override;
    bool isOnEdge(int px, int py) override;
    bool isStartLine(int px, int py) override;
    void resize(int mouseX, int mouseY, QPoint lastMousePos, QRect frameGeom, bool isStartLine) override;

    Shape* copy() override;

    void setOffset(int offset) override;

    QRect getBoundingBox() const override {
        return QRect(std::min(x, xEnd), std::min(y, yEnd), std::abs(xEnd - x), std::abs(yEnd - y));
    }
};

class MyStorage{
private:
    vector<Shape*> shapes;
    vector<Shape*> selected_shapes;
    vector<Shape*> clipboard;
    int offset = 10;
public:

    void AddObject(Shape *shape);

    void drawShapes(QPainter &painter);

    void selectShapes(vector <Shape*> shapesInPoint, bool ctrl);

    void drawSelectedShapes(QPainter &painter);

    void deleteShapes();

    void setColor(QColor color);

    vector <Shape*> GetSelectedShapes();

    vector <Shape*> GetShapes();

    void moveShape(int mouseX, int mouseY, QPoint lastMousePos, QRect frameGeom);

    void resizeShape(int x, int y, QPoint lastMousePos, QRect frameGeom, bool isStartLine);

    bool isStartLine(int px, int py, Shape* line);

    void selectAll();

    void copyShapes();

    void pasteShapes();

    void resetOffset();

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
    void setCircle();
    void setSqaure();
    void setEllipse();
    void setRectangle();
    void setLine();
    void setTriangle();



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
