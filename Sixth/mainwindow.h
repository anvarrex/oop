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

class Shape;

class MementoShapes{
private:
    friend class Circle;
    friend class Square;
    friend class Triangle;
    friend class Rectangle;
    friend class Ellipse;

    map<string, int> state;

    void setState(const string& key, int value){
        state[key] = value;
    }

    int getState(const string& key){
        auto it = state.find(key);
        return it != state.end() ? it->second: 0;
    }

public:

    MementoShapes(){
        qDebug() << "MementoShapes()";
    }

    virtual ~MementoShapes(){
        qDebug() << "~MementoShapes()";
    };
};

class MementoGroup: public MementoShapes{
private:
    vector<MementoShapes*> childMementos;
public:



    void addChildMemento(MementoShapes* memento){
        childMementos.push_back(memento);
    }

    const vector<MementoShapes*>& getChildMementos(){
        return childMementos;
    }

    ~MementoGroup(){
        for (auto m: childMementos){
            delete m;
        }
    }


};


class MementoStorage{
private:

    friend class MyStorage;

    vector<Shape*> state_shapes;
    vector<Shape*> state_selected_shapes;

    void setState(vector<Shape*> shapes, vector<Shape*> selected_shapes){
        state_shapes = shapes;
        state_selected_shapes = selected_shapes;
        qDebug() << state_shapes;
        qDebug() << state_selected_shapes;
    }

    void getState(vector<Shape*> &shapes, vector<Shape*> &selected_shapes){
        shapes = state_shapes;
        selected_shapes = state_selected_shapes;
    }

    MementoStorage(){
        qDebug() << "Memento()";
    }
public:
    virtual ~MementoStorage(){};
};

class Params {
private:
    Params(): selected_shape('C'), selected_color("yellow"){
        qDebug() << "Params()";
    }

    Params(const Params& other) = delete;

    Params& operator=(const Params&) = delete;

    virtual ~Params(){
        qDebug() << "~Params()";
    }
public:

    bool ctrl = false;

    QColor selected_color;

    char selected_shape;

    QRect frameGeom;

    static Params& get(){
        static Params Params;
        return Params;
    }
};

class Shape{

protected:
    int x, y;
    QColor color;
    bool isSelected = false;

public:
    Shape(): x(0), y(0), color("blue"){}
    Shape(int x, int y): x(x), y(y), color(Params::get().selected_color){};

    virtual void draw(QPainter &painter){

        painter.setBrush(color);

        if (!isSelected){
            drawSpecific(&painter);
            return;
        }

        QPen pen(Qt::DashLine);
        pen.setColor(Qt::black);
        pen.setWidth(2);

        painter.setPen(pen);
        drawSpecific(&painter);
        painter.setPen(QPen(Qt::black, 0.5));

    };

    virtual void drawSpecific(QPainter *painter){};

    virtual void select(bool Select) {
        isSelected = Select;
    }

    virtual bool move(int &dx, int &dy);

    virtual bool resize(int dx, int dy) = 0;

    virtual bool contains(int px, int py) = 0;

    virtual void setColor(QColor color);

    virtual bool isOnEdge(int px, int py) {return false;}

    virtual bool intersectsWithFrame(const QRect frameRect) = 0;

    virtual QRect getBoundingBox() const = 0;

    virtual MementoShapes* createMemento() = 0;

    virtual void updateFromMemento(MementoShapes* memento) = 0;

    virtual QPoint getCoors(){
        return QPoint(x, y);
    }

    virtual ~Shape(){
        qDebug() << "~Shape()";
    };
};


class Circle: public Shape{
private:
    int rad = 20;

public:

    Circle(int ValueX, int ValueY);

    void drawSpecific(QPainter *painter) override {
        painter->drawEllipse(x - rad, y - rad, 2 * rad, 2 * rad);
    }

    bool contains(int px, int py) override;

    bool intersectsWithFrame(const QRect frameRect) override;

    bool isOnEdge(int px, int py) override;

    bool resize(int dx, int dy) override;

    QRect getBoundingBox() const override {
        return QRect(x - rad, y - rad, 2 * rad, 2 * rad);
    }

    MementoShapes* createMemento() override{
        MementoShapes *memento = new MementoShapes();
        memento->setState("x", x);
        memento->setState("y", y);
        memento->setState("rad", rad);
        memento->setState("color",  color.rgb());
        return memento;
    }

    void updateFromMemento(MementoShapes* memento) override {
        x = memento->getState("x");
        y = memento->getState("y");
        color = memento->getState("color");
        rad = memento->getState("rad");
    }

    ~Circle(){
        qDebug() << "~Circle()";
    }
};

class Rectangle: public Shape {
private:
    int width, height;
public:
    Rectangle(int ValueX, int ValueY);

    void drawSpecific(QPainter *painter) override {
        painter->drawRect(x, y, width, height);
    };

    bool contains(int px, int py) override;

    bool intersectsWithFrame(const QRect frameRect) override;

    bool isOnEdge(int px, int py) override;

    bool resize(int dx, int dy) override;


    QRect getBoundingBox() const override {
        return QRect(x, y, width, height);
    }

    MementoShapes* createMemento() override{
        MementoShapes *memento = new MementoShapes();
        memento->setState("x", x);
        memento->setState("y", y);
        memento->setState("width", width);
        memento->setState("height",  height);
        memento->setState("color", color.rgb());
        return memento;
    }

    void updateFromMemento(MementoShapes* memento) override {
        x = memento->getState("x");
        y = memento->getState("y");
        color = memento->getState("color");
        width = memento->getState("width");
        height = memento->getState("height");
    }

    ~Rectangle(){
        qDebug() << "~Rectangle()";
    }

};

class Ellipse : public Shape {
private:
    int widthR, heightR;

public:
    Ellipse(int ValueX, int ValueY);

    bool intersectsWithFrame(const QRect frameRect) override;

    void drawSpecific(QPainter *painter) override {
        painter->drawEllipse(QPoint(x, y), widthR, heightR);
    };

    bool contains(int px, int py) override;


    bool isOnEdge(int px, int py) override;

    bool resize(int dx, int dy) override;


    QRect getBoundingBox() const override {
        return QRect(x - widthR, y - heightR, 2 * widthR, 2 * heightR);
    }

    MementoShapes* createMemento() override{
        MementoShapes *memento = new MementoShapes();
        memento->setState("x", x);
        memento->setState("y", y);
        memento->setState("widthR", widthR);
        memento->setState("heightR",  heightR);
        memento->setState("color", color.rgb());
        return memento;
    }

    void updateFromMemento(MementoShapes* memento) override {
        x = memento->getState("x");
        y = memento->getState("y");
        color = memento->getState("color");
        widthR = memento->getState("widthR");
        heightR = memento->getState("heightR");
    }

    ~Ellipse(){
        qDebug() << "~Ellipse()";
    }
};

class Triangle: public Shape {
private:
    int size;
public:
    Triangle(int ValueX, int ValueY);

    void drawSpecific(QPainter *painter) override {
        QPolygon polygon;
        polygon << QPoint(x, y - size)
                << QPoint(x - size, y + size)
                << QPoint(x + size, y + size);
        painter->drawPolygon(polygon);
    };

    bool intersectsWithFrame(const QRect frameRect) override;

    bool contains(int px, int py) override;


    bool isOnEdge(int px, int py) override;

    bool resize(int dx, int dy) override;

    QRect getBoundingBox() const override {
        QPolygon trianglePolygon;
        trianglePolygon << QPoint(x, y - size)
                        << QPoint(x - size, y + size)
                        << QPoint(x + size, y + size);
        return trianglePolygon.boundingRect();
    }

    MementoShapes* createMemento() override{
        MementoShapes *memento = new MementoShapes();
        memento->setState("x", x);
        memento->setState("y", y);
        memento->setState("size", size);
        memento->setState("color", color.rgb());
        return memento;
    }


    void updateFromMemento(MementoShapes* memento) override {
        x = memento->getState("x");
        y = memento->getState("y");
        color = memento->getState("color");
        size = memento->getState("size");
    }

    ~Triangle(){
        qDebug() << "~Triangle()";
    }

};

class Square: public Shape {
private:
    int size;
public:
    Square(int ValueX, int ValueY);

    void drawSpecific(QPainter *painter) override {
        painter->drawRect(x, y, size, size);
    };

    bool contains(int px, int py) override;

    bool intersectsWithFrame(const QRect frameRect) override;

    bool isOnEdge(int px, int py) override;

    bool resize(int dx, int dy) override;

    QRect getBoundingBox() const override{
        return QRect(x, y, size, size);
    }

    MementoShapes* createMemento() override{
        MementoShapes *memento = new MementoShapes();
        memento->setState("x", x);
        memento->setState("y", y);
        memento->setState("size", size);
        memento->setState("color", color.rgb());
        return memento;
    }

    void updateFromMemento(MementoShapes* memento) override {
        x = memento->getState("x");
        y = memento->getState("y");
        color = memento->getState("color");
        size = memento->getState("size");
    }

    ~Square(){
        qDebug() << "~Sqaure()";
    }
};

class Group: public Shape {
private:
    vector <Shape*> _shapes;

public:
    Group(){
        qDebug()<<"Group()\n";
    }

    void addShapes(Shape* shape){
        _shapes.push_back(shape);
    }

    vector<Shape*> getShapes(){
        return _shapes;
    }

    void draw(QPainter &painter) override
    {
        for (Shape* shape: _shapes){
            shape->draw(painter);
        }

        if (isSelected){
            QPen pen;
            pen.setColor("blue");
            pen.setWidth(2);
            painter.setPen(pen);

            painter.setBrush(Qt::NoBrush);

            painter.drawRect(getBoundingBox());
            painter.setPen(QPen(Qt::black, 0.5));
        }
    };

    bool move(int &dx, int &dy) override {
        if (_shapes.empty()) {
            return false; // Нельзя перемещать пустую группу
        }

        QRect boundingBox = getBoundingBox();
        QRect newBoundingBox = boundingBox.translated(dx, dy);
        const QRect& frame = Params::get().frameGeom;

        // Если полностью внутри фрейма - перемещаем все фигуры
        if (frame.contains(newBoundingBox)) {
            for (Shape* shape : _shapes) {
                int shapeDx = dx;
                int shapeDy = dy;
                shape->move(shapeDx, shapeDy);
            }
            return true;
        }

        // Корректировка перемещения при выходе за границы
        int maxDx = dx;
        int maxDy = dy;

        if (newBoundingBox.left() < frame.left()) {
            maxDx = frame.left() - boundingBox.left();
        }
        else if (newBoundingBox.right() > frame.right()) {
            maxDx = frame.right() - boundingBox.right();
        }

        if (newBoundingBox.top() < frame.top()) {
            maxDy = frame.top() - boundingBox.top();
        }
        else if (newBoundingBox.bottom() > frame.bottom()) {
            maxDy = frame.bottom() - boundingBox.bottom();
        }

        // Если перемещение невозможно
        if (maxDx == 0 && maxDy == 0) {
            return false;
        }

        // Применяем скорректированное перемещение
        dx = maxDx;
        dy = maxDy;

        for (Shape* shape : _shapes) {
            int shapeDx = dx;
            int shapeDy = dy;
            shape->move(shapeDx, shapeDy);
        }

        return true;
    }

    bool contains(int px, int py) override {
        for (Shape* shape: _shapes){
            if(shape->contains(px, py)){return true;}
        }
        return false;
    };

    void setColor(QColor color) override {
        for (Shape* shape: _shapes){
            shape->setColor(color);
        }

    };

    bool resize(int dx, int dy) override {

        if (_shapes.empty()) return false;

        bool success = true;

        vector<MementoShapes*> mementos;

        for (Shape* shape : _shapes) {
            mementos.push_back(shape->createMemento());
        }

        for (Shape* shape : _shapes) {
            success = success && shape->resize(dx, dy);
            if (!success) break;
        }

        if (!success) {
            for (size_t i = 0; i < mementos.size(); ++i) {
                _shapes[i]->updateFromMemento(mementos[i]);
            }
        }

        for (MementoShapes* m : mementos) {
            delete m;
        }

        return success;

    };

    void select(bool fl) override {
        for (Shape* shape:_shapes){
            shape->select(fl);
        }
        isSelected = fl;
    }

    virtual bool isOnEdge(int px, int py) override {
        for (Shape* shape: _shapes){
            if (shape->isOnEdge(px, py)){
                return true;
            };
        }
        return false;
    }

    virtual bool intersectsWithFrame(const QRect frameRect) override {return false;};

    QRect getBoundingBox() const override {

        int minX = INT_MAX;
        int minY = INT_MAX;
        int maxX = INT_MIN;
        int maxY = INT_MIN;

        for (Shape* shape: _shapes){
            QRect BoundingBoxShape = shape->getBoundingBox();
            minX = min(BoundingBoxShape.x(), minX);
            minY = min(BoundingBoxShape.y(), minY);
            maxX = max(BoundingBoxShape.x()+BoundingBoxShape.width(), maxX);
            maxY = max(BoundingBoxShape.y()+BoundingBoxShape.height(), maxY);
        }

        return QRect(minX, minY, maxX-minX, maxY-minY);
    };

    MementoShapes* createMemento() override{
        MementoGroup* groupMemento = new MementoGroup();

        for (Shape* shape:_shapes){
            groupMemento->addChildMemento(shape->createMemento());
        }

        return groupMemento;
    };

    virtual void updateFromMemento(MementoShapes* memento) override {
        MementoGroup* groupMemento = dynamic_cast<MementoGroup*>(memento);
        const vector<MementoShapes*>& childMementos = groupMemento->getChildMementos();

        for (size_t i = 0; i < _shapes.size(); ++i) {
            _shapes[i]->updateFromMemento(childMementos[i]);
        }
    }

    ~Group(){
        qDebug() << "~Group()";
    }
};

class ShapeFactory{
public:
    virtual Shape* createShape(int x, int y) = 0;
    virtual ~ShapeFactory(){};
};

class MyShapeFactory: public ShapeFactory{

public:
    MyShapeFactory(){
        qDebug() << "MyShapeFactory()";
    }

    Shape* createShape(int x, int y) override {
        Shape *shape = nullptr;
        char code = Params::get().selected_shape;
        switch (code){
            case 'C':
                shape = new Circle(x, y);
                break;
            case 'R':
                shape = new Rectangle(x, y);
                break;
            case 'E':
                shape = new Ellipse(x, y);
                break;
            case 'S':
                shape = new Square(x, y);
                break;
            case 'T':
                shape = new Triangle(x, y);
                break;
            default:;
        }
        return shape;
    }

    ~MyShapeFactory(){
        qDebug() << "~MyShapeFactory()";
    }
};

class FactoryManager {
private:
    FactoryManager(){
        qDebug() << "FactoryManager()";
    }

    FactoryManager(const FactoryManager& other) = delete;

    FactoryManager& operator=(const FactoryManager&) = delete;

    virtual ~FactoryManager(){
        qDebug() << "~FactoryManager()";
    }
public:

    MyShapeFactory factory;

    static FactoryManager& get(){
        static FactoryManager factorymanager;
        return factorymanager;
    }
};


class Command;

class MyStorage{

private:

    vector<Shape*> shapes;
    vector<Shape*> selected_shapes;

public:
    MyStorage(){
        qDebug() <<  "MyStorage()";
    }

    void AddObj(Shape* shape);

    void DelObj(Shape* shape) {
        shapes.erase(find(selected_shapes.begin(), selected_shapes.end(), shape), selected_shapes.end());
        delete shape;
    };

    void drawShapes(QPainter &painter);

    void selectShapes(vector<Shape*> shapesInPoint);

    void unselectAllShapes(){
        for (Shape* shape: shapes){
            shape->select(false);
        }
        selected_shapes.clear();
    }

    void deleteShapes();


    vector <Shape*> GetSelectedShapes();

    vector <Shape*> GetShapes();

    void executeCommand(Command* newcommand);

    MementoStorage* createMemento(){
        MementoStorage* memento = new MementoStorage();
        memento->setState(shapes, selected_shapes);
        return memento;
    }

    void UpdateFromMemento(MementoStorage* memento){
        unselectAllShapes();
        memento->getState(shapes, selected_shapes);
        for (Shape* shape: selected_shapes){
            shape->select(true);
        }
    }

    Group* MakeGroup(){
        // Создаем новую группу
        Group* newGroup = new Group();

        // Переносим фигуры в группу и удаляем из основного хранилища
        for (Shape* shape : selected_shapes) {
            // Добавляем фигуру в группу
            newGroup->addShapes(shape);

            // Удаляем фигуру из основного списка
            auto it = std::find(shapes.begin(), shapes.end(), shape);
            if (it != shapes.end()) {
                shapes.erase(it);
            }
        }

        // Очищаем выделение (так как фигуры теперь внутри группы)
        selected_shapes.clear();

        // Добавляем группу в хранилище
        shapes.push_back(newGroup);

        // Выделяем новую группу
        selected_shapes.push_back(newGroup);
        newGroup->select(true);

        return newGroup;
    }

    Group* UnGroup(Shape* group){
        Group* d = dynamic_cast<Group*>(group);
        if (d){
            auto it = std::find(shapes.begin(), shapes.end(), group);
            if (it != shapes.end()) {
                shapes.erase(it);
            }
            selected_shapes.clear();
            for (Shape* shape_in_group: d->getShapes()){
                shapes.push_back(shape_in_group);
                selected_shapes.push_back(shape_in_group);
            }

        }



        qDebug() << "Storage::Ungroup()";
        qDebug() << "shapes:" << shapes;
        qDebug() << "selected shapes:" << selected_shapes;
        return d;
    }

    virtual ~MyStorage(){
        qDebug() <<  "~MyStorage()";

        for (Shape* shape: shapes){
            delete shape;
        }
    }
};





class StorageManager {

private:
    StorageManager(){
        qDebug() << "StorageManager()";
    }

    StorageManager(const StorageManager& other) = delete;

    StorageManager& operator=(const StorageManager&) = delete;

    virtual ~StorageManager(){
        qDebug() << "~StorageManager()";
    }
public:

    MyStorage storage;

    static StorageManager& get(){
        static StorageManager storagemanager;
        return storagemanager;
    }
};





class Command{
protected:
    Shape* _shape = nullptr;
    bool fl = false;
public:
    virtual QString show() = 0;
    virtual bool execute(Shape* shape) = 0;
    virtual void unexecute() = 0;
    virtual Command* clone() = 0;
    virtual ~Command() {};
};


class MoveCommand: public Command{
private:

    int _dx, _dy;
    MementoShapes* state = nullptr;

public:

    QString show() override{
        return QString("Move: %1 dx = %2, dy = %3").arg(reinterpret_cast<quintptr>(_shape), 0, 16).arg(_dx).arg(_dy);
    }

    MoveCommand(int dx, int dy): _dx(dx), _dy(dy){
        qDebug() << "MoveCommand::MoveCommand(int dx, int dy)";
    }

    bool execute(Shape* shape) override {
        qDebug()<<"MoveCommand::execute(Shape* shape)";
        _shape = shape;
        state = shape->createMemento();
        fl = _shape->move(_dx, _dy);
        return fl;
    }

    void unexecute() override {
        qDebug()<<"MoveCommand::unexecute()";
        _shape->updateFromMemento(state);
    }

    Command* clone() override{
        qDebug()<<"MoveCommand::Command* clone()";
        return new MoveCommand(_dx, _dy);
    }


    ~MoveCommand(){
        qDebug()<<"~MoveCommand()";
        delete state;
    }
};

class ResizeCommand: public Command{
private:
    int _dx, _dy;
    MementoShapes* state = nullptr;

public:
    ResizeCommand(int dx, int dy): _dx(dx), _dy(dy){
        qDebug() << "ResizeCommand(int dx, int dy)";
    }

    QString show() override{
        return QString("Resize obj %1").arg(reinterpret_cast<quintptr>(_shape), 0, 16);
    }

    bool execute(Shape *shape) override {
        qDebug()<<"ResizeCommand::execute(Shape* shape)";
        _shape = shape;
        state = shape->createMemento();
        fl = _shape->resize(_dx, _dy);
        return fl;
    }

    void unexecute() override {
        qDebug()<<"ResizeCommand::unexecute()";
        _shape->updateFromMemento(state);
    }

    Command* clone() override{
        qDebug()<<"ResizeCommand::Command* clone()";
        return new ResizeCommand(_dx, _dy);
    }

    ~ResizeCommand(){
        qDebug() << "~ResizeCommand()";
        delete state;
    }

};



class AddObjCommand: public Command{
private:
    MementoStorage* state = nullptr;

public:

    AddObjCommand(){
        qDebug() << "AddObjectCommand(Shape* shape)";
    }

    QString show() override{
        return QString("Shape: %1 with (x, y) = (%2, %3) added").arg(reinterpret_cast<quintptr>(_shape), 0, 16).arg(_shape->getCoors().x()).arg(_shape->getCoors().y());
    }

    bool execute(Shape* shape) override {
        qDebug()<<"AddObjectCommand::execute(Shape* shape)";
        _shape = shape;
        state = StorageManager::get().storage.createMemento();
        StorageManager::get().storage.AddObj(_shape);
        return true;
    }

    void unexecute() override{
        qDebug()<<"AddObjectCommand::unexecute()";
        StorageManager::get().storage.UpdateFromMemento(state);
        delete _shape;
    }

    Command * clone() override {
        return new AddObjCommand();
    };

    ~AddObjCommand(){
        qDebug()<< "~AddObjCommand()";
        delete state;
    }
};

class SelectCommand: public Command{
private:
    MementoStorage* state = nullptr;
    vector <Shape*> _shapesInPoint;

public:
    SelectCommand(vector<Shape*> shapesInPoint){
        qDebug() << "SelectCommand()";
        _shapesInPoint = shapesInPoint;
    }

    QString show() override {
        QString result = "Shape(s): ";
        for (Shape* shape: _shapesInPoint){
            result += QString("%1 ").arg(reinterpret_cast<quintptr>(shape), 0, 16);
        }
        result+="selection changed";
        return result;
    }

    bool execute(Shape* shape) override{
        state = StorageManager::get().storage.createMemento();
        StorageManager::get().storage.selectShapes(_shapesInPoint);
        return true;
    }

    void unexecute() override{
        qDebug()<<"SelectCommand::unexecute()";
        StorageManager::get().storage.UpdateFromMemento(state);
    }

    Command * clone() override {
        return new SelectCommand(_shapesInPoint);
    };

    ~SelectCommand(){
        qDebug()<< "~SelectCommand()";
        delete state;
    }
};

class GroupCommand: public Command{
private:
    MementoStorage* state = nullptr;
    Group* group = nullptr;
public:
    GroupCommand(){
        qDebug() << "GroupCommand()";
    }

    QString show() override {
        return QString("Group %1 created").arg(reinterpret_cast<quintptr>(group), 0, 16);
    }

    bool execute(Shape* shape) override{
        qDebug() << "GroupCommand::execute()";
        state = StorageManager::get().storage.createMemento();
        group = StorageManager::get().storage.MakeGroup();
        return true;
    }

    void unexecute() override{
        qDebug()<<"GroupCommand::execute()";
        StorageManager::get().storage.UpdateFromMemento(state);
        delete group;
    }

    Command * clone() override {
        return new GroupCommand();
    };

    ~GroupCommand(){
        qDebug()<< "~GroupCommand()";
        delete state;
    }
};

class UnGroupCommand: public Command{
private:
    MementoStorage* state = nullptr;
    Group* group = nullptr;
public:
    UnGroupCommand(){
        qDebug() << "UnroupCommand()";
    }

    QString show() override {
        return QString("%1 ungrouped").arg(reinterpret_cast<quintptr>(group), 0, 16);
    }

    bool execute(Shape* shape) override{
        qDebug() << "UnGroupCommand::execute()";
        state = StorageManager::get().storage.createMemento();
        group = StorageManager::get().storage.UnGroup(shape);
        return true;
    }

    void unexecute() override{
        qDebug()<<"GroupCommand::execute()";
        StorageManager::get().storage.UpdateFromMemento(state);
    }

    Command * clone() override {
        return new UnGroupCommand();
    };

    ~UnGroupCommand(){
        qDebug()<< "~UnGroupCommand()";
        delete state;
    }
};

class DeleteCommand: public Command{
private:
    vector<Shape*> _shapesToDelete;
    MementoStorage* state = nullptr;
public:
    DeleteCommand(vector<Shape*> shapesToDelete){
        qDebug() << "DeleteCommand()";
        _shapesToDelete = shapesToDelete;
    }

    QString show() override {
        QString result = "Shape(s): ";
        for (Shape* shape: _shapesToDelete){
            result += QString("%1 ").arg(reinterpret_cast<quintptr>(shape), 0, 16);
        }
        result+="deleted";
        return result;
    }


    bool execute(Shape* shape) override {
        state = StorageManager::get().storage.createMemento();
        StorageManager::get().storage.deleteShapes();
        return true;
    }

    void unexecute() override{
        qDebug() << "DeleteCommand::unexecute()";
        StorageManager::get().storage.UpdateFromMemento(state);
    }

    Command* clone() override{
        return new DeleteCommand(_shapesToDelete);
    }


    ~DeleteCommand(){
        qDebug() << "~DeleteCommand()";
        delete state;
    }
};



class SetColorCommand: public Command{
private:
    QColor _newcolor;
    MementoShapes* state = nullptr;
public:
    SetColorCommand(QColor newcolor): _newcolor(newcolor){
        qDebug() << "SetColorCommand(QColor currcolor, QColor newcolor)";
    }


    QString show() override{
        return QString("SetColor %1 %2").arg(reinterpret_cast<quintptr>(_shape), 0, 16).arg(_newcolor.rgb());
    }



    bool execute(Shape* shape) override {
        qDebug() << "SetColorCommand::execute(Shape* shape)";
        _shape = shape;
        state = shape->createMemento();
        _shape->setColor(_newcolor);
        return true;
    }

    void unexecute() override {
        qDebug() << "SetColorCommand::unexecute()";
        _shape->updateFromMemento(state);
    }



    Command* clone() override {
        return new SetColorCommand(_newcolor);
    }

    ~SetColorCommand(){
        qDebug() << "~SetColorCommand()";
        delete state;
    }
};




class CommandsSequence: public Command{
private:
    vector<Command*> commandSequence;
public:
    QString show(){
        QString result;
        for (Command* cmd: commandSequence){
            result += cmd->show() + ' ';
        }
        return result;
    };

    void addCommand(Command* command){
        commandSequence.push_back(command);
    }

    bool execute(Shape* shape){

        for (Command* cmd: commandSequence){
            cmd->execute(shape);
        }
        return true;
    };
    void unexecute(){
        qDebug() << "CommandSequence" << commandSequence;
        for (Command* cmd: commandSequence){
            cmd->unexecute();
        }
    };
    Command* clone() {
        return new CommandsSequence;
    }
    ~CommandsSequence(){
        qDebug() << "~CommandsSequence()";
    };
};

class CmdManager {
private:

    CmdManager(){

        qDebug() << "CmdManager()";

        commands[Qt::Key_A] = new MoveCommand(-10, 0);
        commands[Qt::Key_W] = new MoveCommand(0, -10);
        commands[Qt::Key_S] = new MoveCommand(0, 10);
        commands[Qt::Key_D] = new MoveCommand(10, 0);
        commands[Qt::Key_Equal] = new ResizeCommand(5, 5);
        commands[Qt::Key_Minus] = new ResizeCommand(-5, -5);

    }

    CmdManager(const CmdManager& other) = delete;

    CmdManager& operator=(const CmdManager&) = delete;

    virtual ~CmdManager() {
        qDebug() << "~CmdManager()";
        delete commands[Qt::Key_A];
        delete commands[Qt::Key_W];
        delete commands[Qt::Key_S];
        delete commands[Qt::Key_D];
        delete commands[Qt::Key_Minus];
        delete commands[Qt::Key_Equal];
    }

public:

    vector<Command*> history;

    QString showHistory(){

        QString result;

        for (Command* command: history){
            result += command->show() + '\n';
        }

        return result;
    }

    void clearHistory(){

        for (int i = history.size()-1; i >= 0; --i){
            history[i]->unexecute();
            delete history[i];
        }

        history.clear();

    }

    map<int, Command*> commands;

    static CmdManager& get(){
        static CmdManager cmdmanager;
        return cmdmanager;
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
    void updateHistory();
    void openColorDialog();
    void setCircle();
    void setSqaure();
    void setEllipse();
    void setRectangle();
    void setLine();
    void setTriangle();
    void group();
    void ungroup();
private slots:
    void on_clearHistory_clicked();

private:

    Ui::MainWindow *ui;

    vector <Shape*> shapesInPoint;

    QPoint lastMousePos;

    QPoint MousePressPos;

    bool isMovingShape = false;
    bool isResizingShape = false;


    bool isResizingFrame = false;



};

#endif // MAINWINDOW_H
