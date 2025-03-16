#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->circle, &QAction::triggered, this, &MainWindow::setCircle);
    connect(ui->square, &QAction::triggered, this, &MainWindow::setSqaure);
    connect(ui->ellipse, &QAction::triggered, this, &MainWindow::setEllipse);
    connect(ui->line, &QAction::triggered, this, &MainWindow::setLine);
    connect(ui->triangle, &QAction::triggered, this, &MainWindow::setTriangle);
    connect(ui->rectangle, &QAction::triggered, this, &MainWindow::setRectangle);
    connect(ui->chooseColor, &QAction::triggered, this, &MainWindow::openColorDialog);
}


int counter = 0;


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setClipRect(ui->frame->geometry());
    painter.setPen(QPen(Qt::black, 0.5));
    storage.drawShapes(painter);
    storage.drawSelectedShapes(painter);

    if (isDrawingLine) {
        painter.setPen(QPen(Qt::black, 2, Qt::DashLine));
        painter.drawLine(lineStart, lineEndTemporary);
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event){
    qDebug() << "Маус пресс ивент";
    if (event->button() == Qt::LeftButton){

        QRect frameGeom = ui->frame->geometry();
        QPoint PosInFrame = event->pos() - frameGeom.topLeft();
        MousePosCirclePress = event->pos();
        lastMousePos = event->pos();

        for (Shape* shape: storage.GetShapes()){
            if (shape->contains(event->pos().x(), event->pos().y())){
                shapesInPoint.push_back(shape);
            }
        }

        int corner = 10;

        bool topLeft = (abs(PosInFrame.x()) < corner && abs(PosInFrame.y()) < corner);
        bool topRight = (abs(PosInFrame.x()-frameGeom.width()) < corner && abs(PosInFrame.y()) < corner);
        bool bottomLeft = (abs(PosInFrame.x()) < corner && abs(PosInFrame.y() - frameGeom.height()) < corner);
        bool bottomRight = (abs(PosInFrame.x()-frameGeom.width()) < corner && abs(PosInFrame.y() - frameGeom.height()) < corner);

        // selecting = true;


        if (topLeft || topRight || bottomLeft || bottomRight) {
            isResizing = true;
            lastMousePos = event->pos();
            return;
        }


        if (shapesInPoint.size()!=0)
        {
            for (Shape* shapeInPoint: shapesInPoint){
                for (Shape* selected_shape: storage.GetSelectedShapes()){
                    if (shapeInPoint == selected_shape){
                        if (shapeInPoint->isOnEdge(event->pos().x(), event->pos().y())){
                            if (dynamic_cast<Line*> (shapeInPoint)){
                                isStartLine = storage.isStartLine(event->pos().x(), event->pos().y(), shapeInPoint);
                                isResizingShape = true;
                                return;
                            }
                            isResizingShape = true;
                            return;
                        }
                    }
                }
            }
            for (Shape* shapeInPoint: shapesInPoint){
                for (Shape* selected_shape: storage.GetSelectedShapes()){
                    if (shapeInPoint == selected_shape){
                        isMoving = true;
                        return;
                    }
                }
            }
        }

        if (selected_shape == "Line" && shapesInPoint.size()==0 && frameGeom.contains(event->pos()) ) {
            if (!isDrawingLine) {
                lineStart = event->pos();
                isDrawingLine = true;
                }
            }
        }
    }


void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    qDebug() << isMoving;
    qDebug() << "Маус мув ивент";


    if (isResizingShape){
        storage.resizeShape(event->pos().x(), event->pos().y(), lastMousePos, ui->frame->geometry(), isStartLine);
        lastMousePos = event->pos();
        update();
    }

    if (isDrawingLine) {
        lineEndTemporary = event->pos();
        update();
    }

    if (isMoving) {

        //int deltaX = event->pos().x() - lastMousePos.x();
        //int deltaY = event->pos().y() - lastMousePos.y();

        //if (storage.canMove(ui->frame->geometry(), deltaX, deltaY)){
        storage.moveShape(event->pos().x(), event->pos().y(), lastMousePos, ui->frame->geometry());
        lastMousePos = event->pos();
        update();
        //}
    }

    if (isResizing){
        // selecting = false;

        // int dx = event->pos().x() - lastMousePos.x();
        // int dy = event->pos().y() - lastMousePos.y();

        // int newWidth = qMax(100, ui->frame->width() + dx);
        // int newHeight = qMax(80, ui->frame->height() + dy);
        // ui->frame->resize(newWidth, newHeight);
        // lastMousePos = event->pos();
        selecting = false;

        int dx = event->pos().x() - lastMousePos.x();
        int dy = event->pos().y() - lastMousePos.y();

        // Предварительно вычисляем новые размеры фрейма
        int newWidth = qMax(100, ui->frame->width() + dx);
        int newHeight = qMax(80, ui->frame->height() + dy);

        // Создаем временный прямоугольник для проверки пересечения с фигурами
        QRect newFrameRect = ui->frame->geometry();
        newFrameRect.setWidth(newWidth);
        newFrameRect.setHeight(newHeight);

        // Проверяем, пересекается ли новый фрейм с какой-либо фигурой
        bool intersects = false;
        for (Shape* shape : storage.GetShapes()) {
            if (shape->intersectsWithFrame(newFrameRect)) {
                intersects = true;
                break;
            }
        }

        // Если пересечения нет, изменяем размер фрейма
        if (!intersects) {
            ui->frame->resize(newWidth, newHeight);
            lastMousePos = event->pos();
        }
    }


    }




void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    isMoving = false;
    isResizingShape = false;
    qDebug() << "Маус релиз ивент";
    isResizing = false;
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

    if (isDrawingLine) {
        storage.AddObject(new Line(lineStart.x(), lineStart.y(), event->pos().x(), event->pos().y(), selected_color));
        isDrawingLine = false;
        update();
    }


    if (MousePosCirclePress == event->pos()){
        if (ui->frame->geometry().contains(event->pos())) {
            if (shapesInPoint.size()!=0){
                storage.selectShapes(shapesInPoint, ctrl);
            }
            else{
                qDebug() << ++counter ;
                if (selected_shape == "Circle"){
                    storage.AddObject(new Circle(event->pos().x(), event->pos().y(), selected_color, frameGeom));
                }
                else if (selected_shape == "Ellipse"){
                    storage.AddObject(new Ellipse(event->pos().x(), event->pos().y(), selected_color, frameGeom));
                }
                else if (selected_shape == "Rectangle"){
                    storage.AddObject(new Rectangle(event->pos().x(), event->pos().y(), selected_color, frameGeom));
                }
                else if (selected_shape == "Triangle"){
                    storage.AddObject(new Triangle(event->pos().x(), event->pos().y(), selected_color, frameGeom));
                }
                else if (selected_shape == "Square"){
                    storage.AddObject(new Square(event->pos().x(), event->pos().y(), selected_color, frameGeom));
                }
            }
            update();
        }
    }
    shapesInPoint.clear();
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Backspace || event->key() == Qt::Key_Delete) {
        qDebug() << "Del нажата!";
        storage.deleteShapes();
        update();
    }

    else if (event->modifiers() & Qt::ControlModifier && event->key() == Qt::Key_A) {
        storage.selectAll();
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

void MainWindow::openColorDialog()
{
    QColor color = QColorDialog::getColor(selected_color, this);
    if (color.isValid()) {
        selected_color = color;
        storage.setColor(color);
    }
}

void Circle::draw(QPainter &painter)
{
    painter.setBrush(color);
    painter.drawEllipse(x - rad, y - rad, 2 * rad, 2 * rad);
}

bool Circle::contains(int px, int py)
{
    int dx = x - px;
    int dy = y - py;
    return (dx * dx + dy * dy) <= (rad * rad);
}

void Circle::drawSelected(QPainter &painter)
{
    Shape::drawSelected(painter);
    painter.setBrush(color);
    painter.drawEllipse(x - rad, y - rad, 2 * rad, 2 * rad);
}

void MyStorage::AddObject(Shape *shape)
{
    shapes.push_back(shape);
    selected_shapes.clear();
}

void MyStorage::drawShapes(QPainter &painter)
{
    for (Shape* shape: shapes){
        shape->draw(painter);
    }
}



void MyStorage::selectShapes(vector <Shape*> shapesInPoint, bool ctrl)
{
    if (!ctrl)
    {
        if (selected_shapes == shapesInPoint){
            selected_shapes.clear();
        }
        else {
            selected_shapes = shapesInPoint;
        }
    }
    else {
        for (Shape* shapeInPoint: shapesInPoint){
            if (find(selected_shapes.begin(), selected_shapes.end(), shapeInPoint) == selected_shapes.end()){
                selected_shapes.push_back(shapeInPoint);
            }
            else {
                selected_shapes.erase(remove(selected_shapes.begin(), selected_shapes.end(), shapeInPoint), selected_shapes.end());
            }
        }
    }
    qDebug() << shapesInPoint;
    qDebug() << selected_shapes;
}


void MyStorage::drawSelectedShapes(QPainter &painter)
{
    for (Shape* shape: selected_shapes){
        shape->drawSelected(painter);
    }
}


void MyStorage::deleteShapes()
{
    for (Shape* selected_shape: selected_shapes){
        shapes.erase(remove(shapes.begin(), shapes.end(), selected_shape), shapes.end());
    }
    selected_shapes.clear();
}

void MyStorage::setColor(QColor color)
{
    for (Shape * selected_shape: selected_shapes)
    {
        selected_shape->setColor(color);
    }
}


void Rectangle::drawSelected(QPainter &painter)
{
    Shape::drawSelected(painter);
    painter.setBrush(color);
    painter.drawRect(x, y, width, height);
}

void Ellipse::drawSelected(QPainter &painter)
{
    Shape::drawSelected(painter);
    painter.setBrush(color);
    painter.drawEllipse(QPoint(x, y), widthR, heightR);
}

void Triangle::drawSelected(QPainter &painter)
{
    Shape::drawSelected(painter);
    QPolygon polygon;
    polygon << QPoint(x, y - size)
            << QPoint(x - size, y + size)
            << QPoint(x + size, y + size);
    painter.setBrush(color);
    painter.drawPolygon(polygon);
}

void Square::drawSelected(QPainter &painter)
{
    Shape::drawSelected(painter);
    painter.setBrush(color);
    painter.drawRect(x, y, width, height);
}


void Line::drawSelected(QPainter &painter)
{
    Shape::drawSelected(painter);
    painter.drawLine(x, y, xEnd, yEnd);
}
