#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QMouseEvent>
#include <QColorDialog>
#include <QMessageBox>
#include <stack>

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
    QRect* pframeGeom;

public:
    Shape(): x(0), y(0), color("blue"), pframeGeom(nullptr){}
    Shape(int x, int y, QColor color, QRect* pframeGeom): x(x), y(y), color(color), pframeGeom(pframeGeom){};

    virtual void draw(QPainter &painter) = 0;
    virtual void drawSelected(QPainter &painter);
    void move(int dx, int dy);

    virtual bool contains(int px, int py) = 0;

    virtual void setColor(QColor color);

    virtual void resize(int dx, int dy) = 0;

    virtual bool isOnEdge(int px, int py) {return false;}

    virtual bool intersectsWithFrame(const QRect frameRect) = 0;

    virtual QRect getBoundingBox() const = 0;

    virtual ~Shape(){
        qDebug() << "~Shape()";
        pframeGeom = nullptr;
    };
};


class Circle: public Shape{
private:
    int rad = 20;

public:
    Circle(int ValueX, int ValueY, QColor color, QRect* pframeGeom);
    void draw(QPainter &painter) override;
    bool contains(int px, int py) override;
    void drawSelected(QPainter &painter) override;


    bool intersectsWithFrame(const QRect frameRect) override;

    bool isOnEdge(int px, int py) override;

    void resize(int dx, int dy) override;


    QRect getBoundingBox() const override {
        return QRect(x - rad, y - rad, 2 * rad, 2 * rad);
    }
    ~Circle(){
        qDebug() << "~Circle()";
    }
};

class Rectangle: public Shape {
private:
    int width, height;
public:
    Rectangle(int ValueX, int ValueY, QColor color, QRect* pframeGeom);
    void draw(QPainter &painter) override;
    bool contains(int px, int py) override;
    void drawSelected(QPainter &painter) override;

    bool intersectsWithFrame(const QRect frameRect) override;

    bool isOnEdge(int px, int py) override;

    void resize(int dx, int dy) override;


    QRect getBoundingBox() const override {
        return QRect(x, y, width, height);
    }
    ~Rectangle(){
        qDebug() << "~Rectangle()";
    }
};

class Ellipse : public Shape {
private:
    int widthR, heightR;

public:
    Ellipse(int ValueX, int ValueY, QColor color, QRect* pframeGeom);

    bool intersectsWithFrame(const QRect frameRect) override;

    void draw(QPainter &painter) override;

    bool contains(int px, int py) override;

    void drawSelected(QPainter &painter) override;

    bool isOnEdge(int px, int py) override;

    void resize(int dx, int dy) override;


    QRect getBoundingBox() const override {
        return QRect(x - widthR, y - heightR, 2 * widthR, 2 * heightR);
    }

    ~Ellipse(){
        qDebug() << "~Ellipse()";
    }
};

class Triangle: public Shape {
private:
    int size;
public:
    Triangle(int ValueX, int ValueY, QColor color, QRect* pframeGeom);
    void draw(QPainter &painter) override;

    bool intersectsWithFrame(const QRect frameRect) override;

    bool contains(int px, int py) override;
    void drawSelected(QPainter &painter) override;

    bool isOnEdge(int px, int py) override;

    void resize(int dx, int dy) override;

    QRect getBoundingBox() const override {
        QPolygon trianglePolygon;
        trianglePolygon << QPoint(x, y - size)
                        << QPoint(x - size, y + size)
                        << QPoint(x + size, y + size);
        return trianglePolygon.boundingRect();
    }

    ~Triangle(){
        qDebug() << "~Triangle()";
    }

};

class Square: public Shape {
private:
    int width, height;
public:
    Square(int ValueX, int ValueY, QColor color, QRect* pframeGeom);
    void draw(QPainter &painter) override;

    bool contains(int px, int py) override;

    bool intersectsWithFrame(const QRect frameRect) override;

    void drawSelected(QPainter &painter) override;
    bool isOnEdge(int px, int py) override;

    void resize(int dx, int dy) override;

    QRect getBoundingBox() const override{
        return QRect(x, y, width, height);
    }

    ~Square(){
        qDebug() << "~Sqaure()";
    }
};



class Group: public Shape{
private:
    vector <Shape*> _shapes;
public:
    Group(){
        qDebug()<<"Group()\n";
    }

    void addShapes(Shape* shape){
        _shapes.push_back(shape);
    }

    void draw(QPainter &painter){
        for (Shape* shape: _shapes){
            shape->draw(painter);
        }
    };

    void drawSelected(QPainter &painter){
        for (Shape* shape: _shapes){
            shape->drawSelected(painter);
        }
        painter.drawRect(getBoundingBox());

    };

    void move(int dx, int dy){
        for (Shape* shape: _shapes){
            shape->move(dx, dy);
        }
    };

    bool contains(int px, int py){
        for (Shape* shape: _shapes){
            if(shape->contains(px, py)){return true;}
        }
        return false;
    };

    void setColor(QColor color){
        for (Shape* shape: _shapes){
            shape->setColor(color);
        }

    };

    virtual void resize(int dx, int dy){
        for (Shape* shape: _shapes){
            while (pframeGeom->contains(getBoundingBox())){
                shape->resize(dx, dy);
            }
        }
    };
    virtual bool isOnEdge(int px, int py) {
        for (Shape* shape: _shapes){
            if (shape->isOnEdge(px, py)){
                return true;
            };
        }
        return false;
    }
    virtual bool isStartLine(int px, int py){return false;}

    virtual bool intersectsWithFrame(const QRect frameRect){return false;};

    QRect getBoundingBox(){
        int minX = 99999;
        int minY = 99999;
        int maxX = -1;
        int maxY = -1;

        for (Shape* shape: _shapes){
            QRect BoundingBoxShape = shape->getBoundingBox();
            minX = min(BoundingBoxShape.x(), minX);
            minY = min(BoundingBoxShape.y(), minY);
            maxX = max(BoundingBoxShape.x()+BoundingBoxShape.width(), maxX);
            maxY = max(BoundingBoxShape.y()+BoundingBoxShape.height(), maxY);
        }

        return QRect(minX, minY, maxX, maxY);
    };

    ~Group(){
        qDebug() << "~Group()";
    }


};

class Command;

class MyStorage{
private:
    vector<Shape*> shapes;
    vector<Shape*> selected_shapes;
    vector<Shape*> clipboard;
    stack<Command*> *_phistory;

    int offset = 10;
public:
    MyStorage(stack<Command*> *phistory){
        _phistory = phistory;
    }

    void AddObject(Shape *shape);

    void drawShapes(QPainter &painter);

    void selectShapes(vector <Shape*> shapesInPoint, bool ctrl);

    void drawSelectedShapes(QPainter &painter);

    void deleteShapes();

    void setColor(QColor color);

    vector <Shape*> GetSelectedShapes();

    vector <Shape*> GetShapes();

    void moveShape(int dx, int dy, QRect frameGeom);

    void moveShapeFromKeyBoard(Command* newcommand);

    void resizeShape(int dx, int dy);

    bool isStartLine(int px, int py, Shape* line);

    void selectAll();

    ~MyStorage(){
        qDebug() <<  "~MyStorage()";
        _phistory = nullptr;
        for (Shape* shape: shapes){
            delete shape;
        }
    }
};


class Command{
public:
    virtual void execute(Shape* shape) = 0;
    virtual void unexecute() = 0;
    virtual Command* clone() = 0;
    virtual ~Command() {};
};



class MoveCommand: public Command{
private:
    int _dx, _dy;
    Shape* _shape;

public:
    MoveCommand(int dx, int dy): _dx(dx), _dy(dy), _shape(nullptr){
        qDebug() << "MoveCommand::MoveCommand(int dx, int dy)";
    }
    void execute(Shape* shape) override {
        qDebug()<<"MoveCommand::execute(Shape* shape)";
        _shape = shape;
        _shape->move(_dx, _dy);
    }

    void unexecute() override {
        qDebug()<<"MoveCommand::unexecute()";
        _shape->move(-_dx,-_dy);
    }

    Command* clone() override{
        qDebug()<<"MoveCommand::Command* clone()";
        return new MoveCommand(_dx, _dy);
    }

    ~MoveCommand(){
        qDebug()<<"~MoveCommand()";
    }
};

class AddObjectCommand: public Command{
private:
    Shape* _shape;
    vector<Shape*> *_pshapes;
    vector<Shape*> *_pselectedshapes;
public:
    AddObjectCommand(vector<Shape*> *pshapes, vector<Shape*> *pselectedshapes): _pshapes(pshapes), _pselectedshapes(pselectedshapes){
        qDebug() << "AddObjectCommand(Shape* shape)";
    }

    void execute(Shape* shape) override {
        qDebug()<<"AddObjectCommand::execute(Shape* shape)";
        _shape = shape;
        _pshapes->push_back(_shape);
    }

    void unexecute() override{
        qDebug()<<"AddObjectCommand::unexecute()";
        _pshapes->erase(remove(_pshapes->begin(), _pshapes->end(), _shape), _pshapes->end());
        _pselectedshapes->erase(remove(_pselectedshapes->begin(), _pselectedshapes->end(), _shape), _pselectedshapes->end());
        delete _shape;

    }

    virtual Command * clone() override {
        return new AddObjectCommand(_pshapes, _pselectedshapes);
    };

    ~AddObjectCommand(){
        qDebug()<< "~AddObjectCommand()";
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
    void setCircle();
    void setSqaure();
    void setEllipse();
    void setRectangle();
    void setLine();
    void setTriangle();
    void group();


private:
    Ui::MainWindow *ui;
    stack<Command*> history;
    QRect frameGeom;
    map<int, Command*> commands;
    MyStorage storage;
    QPoint lastMousePos;
    QPoint lineStart, lineEndTemporary;
    QColor selected_color = Qt::yellow;
    QPoint MousePosCirclePress;
    string selected_shape = "Circle";
    bool isStartLine = false;
    bool fl;

    bool isMoving = false;
    bool isResizing = false;

    bool ctrl = false;
    bool selecting = false;
    bool isResizingShape = false;
    vector <Shape*> shapesInPoint;
};

#endif // MAINWINDOW_H
