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
    storage.drawShapes(painter);
    storage.drawSelectedShapes(painter);

    // if (selecting){
    //     painter.setPen(QPen(Qt::black, 2, Qt::DashLine));
    //     painter.setBrush(Qt::NoBrush);
    //     painter.drawRect(selectionRect.normalized());
    // }
}

void MainWindow::mousePressEvent(QMouseEvent *event){
    qDebug() << "Маус пресс ивент";
    if (event->button() == Qt::LeftButton){

        QRect frameGeom = ui->frame->geometry();
        QPoint PosInFrame = event->pos() - frameGeom.topLeft();
        MousePosCirclePress = event->pos();
        lastMousePos = event->pos();
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

        if (storage.shapesInPoint(event->pos().x(), event->pos().y()).size()!=0)
        {
            for (Shape* shapeInPoint: storage.shapesInPoint(event->pos().x(), event->pos().y())){
                for (Shape* selected_shape: storage.GetSelectedShapes()){
                    if (shapeInPoint == selected_shape){
                        isMoving = true;
                    }
                }
            }

        //     shapesToMove = storage.shapesInPoint(event->pos().x(), event->pos().y());
        //     storage.selectShapes(storage.shapesInPoint(event->pos().x(), event->pos().y()), ctrl);
        //     fl = true;
        }


        if (selected_shape == "Line" && !fl) {
            if (!isDrawingLine) {
                lineStart = event->pos();
                isDrawingLine = true;
            } else {
                storage.AddObject(new Line(lineStart.x(), lineStart.y(), event->pos().x(), event->pos().y(), selected_color));
                isDrawingLine = false;
                update();
            }
        }
}
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    qDebug() << isMoving;
    // qDebug() << "Маус мув ивент";
    // if (selecting && ctrl == false && ui->frame->geometry().contains(MousePosCirclePress) && ui->frame->geometry().contains(event->pos())) {
    //     selectionRect.setTopLeft(MousePosCirclePress);
    //     selectionRect.setBottomRight(event->pos());
    //     storage.selectshapesInRect(selectionRect);
    //     update();
    // }
    if (isMoving) {
        qDebug() << "1ййййй";
        storage.moveShape(event->pos().x(), event->pos().y(), lastMousePos);
        lastMousePos = event->pos();
        qDebug() << "2ййййй";
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
    isMoving = false;
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
        if (ui->frame->geometry().contains(event->pos())) {
            if (storage.shapesInPoint(event->pos().x(), event->pos().y()).size()!=0){
                storage.selectShapes(storage.shapesInPoint(event->pos().x(), event->pos().y()), ctrl);
            }
            else{
                qDebug() << ++counter ;
                if (selected_shape == "Circle"){
                    storage.AddObject(new Circle(event->pos().x(), event->pos().y(), selected_color));
                }
                else if (selected_shape == "Ellipse"){
                    storage.AddObject(new Ellipse(event->pos().x(), event->pos().y(), selected_color));
                }
                else if (selected_shape == "Rectangle"){
                    storage.AddObject(new Rectangle(event->pos().x(), event->pos().y(), selected_color));
                }
                else if (selected_shape == "Triangle"){
                    storage.AddObject(new Triangle(event->pos().x(), event->pos().y(), selected_color));
                }
                else if (selected_shape == "Sqaure"){
                    storage.AddObject(new Square(event->pos().x(), event->pos().y(), selected_color));
                }
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
        storage.deleteShapes();
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
    QColor color = QColorDialog::getColor(selected_color, this, "Выберите цвет");
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

vector <Shape *> MyStorage::shapesInPoint(int x, int y)
{
    vector <Shape*> shapesInPoint;
    for (Shape* shape: shapes){
        if (shape->contains(x, y)){
            shapesInPoint.push_back(shape);
        }
    }
    return shapesInPoint;
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

// bool MyStorage::isShape(int x, int y, bool ctrl)
// {
//     bool in = false;
//     vector<Shape*> shapesInPoint;
//     vector<Shape*> unselectedShapes;
//     for (int i = 0; i<shapes.size();i++){
//         if (shapes[i]->contains(x, y)){
//             shapesInPoint.push_back(shapes[i]);
//             if (find(selected_shapes.begin(), selected_shapes.end(), shapes[i]) == selected_shapes.end())
//             {
//                 if (ctrl == false){
//                     selected_shapes.clear();
//                     selected_shapes.push_back(shapes[i]);
//                 }
//                 else {
//                     selected_shapes.push_back(shapes[i]);
//                 }

//             }

//             else {
//                 selected_shapes.erase(remove(selected_shapes.begin(), selected_shapes.end(), shapes[i]), selected_shapes.end());
//                 unselectedShapes.push_back(shapes[i]);

//             }
//             in = true;
//         }
//     }

//     qDebug() << "Selected метода isCircle" <<selected_shapes;
//     qDebug() << "InPoint метода isCircle" << shapesInPoint;
//     if (shapesInPoint.size() > 1){
//         if (shapesInPoint != unselectedshapes){
//             for (int i = 0; i<shapesInPoint.size(); i++) {
//                 if (find(selected_shapes.begin(), selected_shapes.end(), shapesInPoint[i]) == selected_shapes.end())
//                 {
//                     selected_shapes.push_back(shapesInPoint[i]);

//                 }
//             }
//         }
//     }
//     shapesInPoint.clear();
//     unselectedshapes.clear();

//     return in;
// }

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

// void MyStorage::selectshapesInRect(QRect Rect)
// {
//     qDebug() << "метод selectshapesInRect";
//     for (int i = 0; i<shapes.size();i++) {
//         QRect circleRect(shapes[i]->GetX() - 20, shapes[i]->GetY() - 20, 40, 40);
//         if (Rect.intersects(circleRect)) {
//             if (find(selected_shapes.begin(), selected_shapes.end(), shapes[i]) == selected_shapes.end()){
//                 qDebug() << "метод selectshapesinrect добавлен";
//                 selected_shapes.push_back(shapes[i]);
//             }
//         } else {
//             selected_shapes.erase(remove(selected_shapes.begin(), selected_shapes.end(), shapes[i]), selected_shapes.end());
//         }
//     }
// }

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
    painter.drawEllipse(x, y, width, height);
}

void Triangle::drawSelected(QPainter &painter)
{
    Shape::drawSelected(painter);
    QPolygon polygon;
    polygon << QPoint(x, y - 20)
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
