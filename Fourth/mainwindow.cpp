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

    if (event->modifiers() & Qt::ControlModifier && event->key() == Qt::Key_A) {
        storage.selectAll();
        update();
    }

    if (event->modifiers() & Qt::ControlModifier) {
        if (event->key() == Qt::Key_C){
            storage.copyShapes();
            storage.resetOffset();
            QMessageBox::information(this, "Информация", "Объекты скопированы!");
        }
        if (event->key() == Qt::Key_V){
            storage.pasteShapes();

        }
        update();
    }

    if (event->key() == Qt::Key_Control){
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

void MainWindow::setCircle(){
    selected_shape = "Circle";
}

void MainWindow::setSqaure(){
    selected_shape = "Square";
}

void MainWindow::setEllipse(){
    selected_shape = "Ellipse";
}

void MainWindow::setRectangle(){
    selected_shape = "Rectangle";
}

void MainWindow::setLine(){
    selected_shape = "Line";
}

void MainWindow::setTriangle(){
    selected_shape = "Triangle";
}

Circle::Circle(int ValueX, int ValueY, QColor color, QRect frameGeom): Shape(ValueX, ValueY, color){
    if (x - rad < frameGeom.x()) x = rad+frameGeom.x();
    if (y - rad < frameGeom.y()) y = rad+frameGeom.y();
    if (x + rad > frameGeom.width()+frameGeom.x()) x = frameGeom.x() + frameGeom.width() - rad;
    if (y + rad > frameGeom.height()+frameGeom.y()) y = frameGeom.y() + frameGeom.height() - rad;
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


bool Circle::intersectsWithFrame(const QRect frameRect){
    QRect circleRect(x - rad, y - rad, 2 * rad, 2 * rad);
    return !frameRect.contains(circleRect);
}

bool Circle::isOnEdge(int px, int py){
    int distance = std::hypot(px - x, py - y);
    if (abs(distance - rad) < 5){
        qDebug() << "Edge";
        return true;
    }
    else {
        return false;
    } // Проверяем, близка ли точка к границе
}

void Circle::resize(int mouseX, int mouseY, QPoint lastMousePos, QRect frameGeom, bool isStartLine){
    int dx = mouseX - lastMousePos.x();
    int dy = mouseY - lastMousePos.y();
    rad += dx;
    rad += dy;
    if (rad < 15) rad = 15;

    QRect boundingBox = getBoundingBox();

    if (!frameGeom.contains(boundingBox)) {
        rad += -dx;
        rad += -dy;
    }
}

Shape *Circle::copy(){
    return new Circle(*this);
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

vector<Shape *> MyStorage::GetSelectedShapes(){
    return selected_shapes;
}

vector<Shape *> MyStorage::GetShapes(){
    return shapes;
}

void MyStorage::moveShape(int mouseX, int mouseY, QPoint lastMousePos, QRect frameGeom)
{
    for (Shape* selected_shape: selected_shapes)
    {
        selected_shape->move(mouseX, mouseY, lastMousePos, frameGeom);
    }
}

void MyStorage::resizeShape(int x, int y, QPoint lastMousePos, QRect frameGeom, bool isStartLine){
    for (Shape* selected_shape: selected_shapes){
        selected_shape->resize(x, y, lastMousePos, frameGeom, isStartLine);
    }
}

bool MyStorage::isStartLine(int px, int py, Shape *line){
    return line->isStartLine(px, py);
}

void MyStorage::selectAll(){
    selected_shapes = shapes;
}

void MyStorage::copyShapes(){
    clipboard.clear();
    for (Shape* selected_shape: selected_shapes){
        clipboard.push_back(selected_shape);
    }
}

void MyStorage::pasteShapes(){
    for (Shape* shape: clipboard){
        Shape* newShape = shape->copy();

        newShape->setOffset(offset);

        shapes.push_back(newShape);
        selected_shapes.push_back(newShape);
    }
    offset+=10;
}

void MyStorage::resetOffset(){
    offset = 10;
}


Rectangle::Rectangle(int ValueX, int ValueY, QColor color, QRect frameGeom): Shape(ValueX, ValueY, color), width(60), height(40){
    if (x < frameGeom.x()) x = frameGeom.x();
    if (y < frameGeom.y()) y = frameGeom.y();
    if (x + width > frameGeom.x()+frameGeom.width()) x = frameGeom.x() + frameGeom.width() - width;
    if (y + height > frameGeom.y()+frameGeom.height()) y =frameGeom.y() + frameGeom.height() - height;
}

void Rectangle::draw(QPainter &painter) {
    painter.setBrush(color);
    painter.drawRect(x, y, width, height);
}

bool Rectangle::contains(int px, int py) {
    return (px >= x && px <= x + width && py >= y && py <= y + height);
}

void Rectangle::drawSelected(QPainter &painter)
{
    Shape::drawSelected(painter);
    painter.setBrush(color);
    painter.drawRect(x, y, width, height);
}

bool Rectangle::intersectsWithFrame(const QRect frameRect) {
    QRect shapeRect(x, y, width, height);
    return !frameRect.contains(shapeRect);
}

bool Rectangle::isOnEdge(int px, int py){
    int edgeThreshold = 5;  // Толщина зоны для изменения размеров
    bool left = abs(px - x) < edgeThreshold;
    bool right = abs(px - (x + width)) < edgeThreshold;
    bool top = abs(py - y) < edgeThreshold;
    bool bottom = abs(py - (y + height)) < edgeThreshold;
    return left || right || top || bottom;
}

void Rectangle::resize(int mouseX, int mouseY, QPoint lastMousePos, QRect frameGeom, bool isStartLine) {
    int dx = mouseX - lastMousePos.x();
    int dy = mouseY - lastMousePos.y();
    width += dx;
    height += dy;
    width = max(60, width);
    height = max(40, height);

    QRect boundingBox = getBoundingBox();

    if (!frameGeom.contains(boundingBox)) {
        width += -dx;
        height += -dy;
    }
}

Shape *Rectangle::copy(){
    return new Rectangle(*this);
}

Ellipse::Ellipse(int ValueX, int ValueY, QColor color, QRect frameGeom) : Shape(ValueX, ValueY, color), widthR(30), heightR(20){

    // Проверяем, что эллипс не выходит за пределы области
    if (x - widthR < frameGeom.x()) x = frameGeom.x() + widthR;
    if (y - heightR < frameGeom.y()) y = frameGeom.y() + heightR;
    if (x + widthR > frameGeom.x() + frameGeom.width()) x = frameGeom.x() + frameGeom.width() - widthR;
    if (y + heightR > frameGeom.y() + frameGeom.height()) y = frameGeom.y() + frameGeom.height() - heightR;

}

bool Ellipse::intersectsWithFrame(const QRect frameRect) {
    // Проверяем пересечение эллипса с фреймом
    QRect ellipseRect(x - widthR, y - heightR, 2 * widthR, 2 * heightR);
    return !frameRect.contains(ellipseRect);
}

void Ellipse::draw(QPainter &painter) {
    painter.setBrush(color);
    painter.drawEllipse(QPoint(x, y), widthR, heightR);
}

bool Ellipse::contains(int px, int py) {
    double dx = px - x;  // Смещение по X от центра
    double dy = py - y;  // Смещение по Y от центра

    double a = widthR;   // Полуось по X (горизонтальный радиус)
    double b = heightR;  // Полуось по Y (вертикальный радиус)

    return (dx * dx) / (a * a) + (dy * dy) / (b * b) <= 1.0;
}

void Ellipse::drawSelected(QPainter &painter)
{
    Shape::drawSelected(painter);
    painter.setBrush(color);
    painter.drawEllipse(QPoint(x, y), widthR, heightR);
}


bool Ellipse::isOnEdge(int px, int py) {
    double centerX = x;
    double centerY = y;

    double normX = (px - centerX) / (widthR);  // X-координата относительно центра
    double normY = (py - centerY) / (heightR); // Y-координата относительно центра

    // Проверка, лежит ли точка на границе эллипса (погрешность 0.3)
    double equation = normX * normX + normY * normY;
    return fabs(equation - 1.0) < 0.3;
}

void Ellipse::resize(int mouseX, int mouseY, QPoint lastMousePos, QRect frameGeom, bool isStartLine) {
    // Смещение мыши
    int dx = mouseX - lastMousePos.x();
    int dy = mouseY - lastMousePos.y();

    // Изменение размеров эллипса
    widthR += dx;
    heightR += dy;

    // Ограничение минимального размера
    widthR = std::max(30, widthR);
    heightR = std::max(20, heightR);

    QRect boundingBox = getBoundingBox();

    if (!frameGeom.contains(boundingBox)) {
        heightR += -dx;
        widthR += -dy;
    }

}

Shape *Ellipse::copy() {
    return new Ellipse(*this);
}

Triangle::Triangle(int ValueX, int ValueY, QColor color, QRect frameGeom): Shape(ValueX, ValueY, color), size(30){
    if (x - size < frameGeom.x()) x = size+frameGeom.x();
    if (y - size < frameGeom.y()) y = size+frameGeom.y();
    if (x + size > frameGeom.width()+frameGeom.x()) x = frameGeom.x() + frameGeom.width() - size;
    if (y + size > frameGeom.height()+frameGeom.y()) y = frameGeom.y() + frameGeom.height() - size;
}

void Triangle::draw(QPainter &painter) {
    painter.setBrush(color);
    QPolygon polygon;
    polygon << QPoint(x, y - size)
            << QPoint(x - size, y + size)
            << QPoint(x + size, y + size);
    painter.drawPolygon(polygon);
}

bool Triangle::intersectsWithFrame(const QRect frameRect) {
    // Проверяем пересечение треугольника с фреймом
    QPolygon trianglePolygon;
    trianglePolygon << QPoint(x, y - size)
                    << QPoint(x - size, y + size)
                    << QPoint(x + size, y + size);
    return !frameRect.contains(trianglePolygon.boundingRect());
}

bool Triangle::contains(int px, int py) {
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

bool Triangle::isOnEdge(int px, int py) {
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

void Triangle::resize(int mouseX, int mouseY, QPoint lastMousePos, QRect frameGeom, bool isStartLine) {
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

    QRect boundingBox = getBoundingBox();

    if (!frameGeom.contains(boundingBox)) {
        size += -delta;
    }
}

Shape *Triangle::copy() {
    return new Triangle(*this);
}

Square::Square(int ValueX, int ValueY, QColor color, QRect frameGeom): Shape(ValueX, ValueY, color), width(40), height(40){
    if (x < frameGeom.x()) x = frameGeom.x();
    if (y < frameGeom.y()) y = frameGeom.y();
    if (x + width > frameGeom.x()+frameGeom.width()) x = frameGeom.x() + frameGeom.width() - width;
    if (y + height > frameGeom.y()+frameGeom.height()) y = frameGeom.y() + frameGeom.height() - height;
}

void Square::draw(QPainter &painter) {
    painter.setBrush(color);
    painter.drawRect(x, y, width, height);
}

bool Square::contains(int px, int py) {
    return (px >= x && px <= x + width && py >= y && py <= y + height);
}

bool Square::intersectsWithFrame(const QRect frameRect) {
    // Проверяем пересечение квадрата с фреймом
    QRect shapeRect(x, y, width, height);
    return !frameRect.contains(shapeRect);
}

void Square::drawSelected(QPainter &painter)
{
    Shape::drawSelected(painter);
    painter.setBrush(color);
    painter.drawRect(x, y, width, height);
}

bool Square::isOnEdge(int px, int py){
    int edgeThreshold = 5;  // Толщина зоны для изменения размеров
    bool left = abs(px - x) < edgeThreshold;
    bool right = abs(px - (x + width)) < edgeThreshold;
    bool top = abs(py - y) < edgeThreshold;
    bool bottom = abs(py - (y + height)) < edgeThreshold;
    return left || right || top || bottom;
}


void Square::resize(int mouseX, int mouseY, QPoint lastMousePos, QRect frameGeom, bool isStartLine) {
    int dx = mouseX - lastMousePos.x();
    int dy = mouseY - lastMousePos.y();

    int delta;

    if (dy < 0 || dx < 0){
        delta = min(dx, dy);
    }
    else {
        delta = max(dx, dy);
    }


    width += delta;
    height += delta;

    int minSizeSquare = 40;

    width = max(minSizeSquare, width);
    height = max(minSizeSquare, height);

    QRect boundingBox = getBoundingBox();

    if (!frameGeom.contains(boundingBox)) {
        width += -delta;
        height += -delta;
    }
}

Shape *Square::copy() {
    return new Square(*this);
}


Line::Line(int xStart, int yStart, int xEnd, int yEnd, QColor color): Shape(xStart, yStart, color), xEnd(xEnd), yEnd(yEnd) {}

void Line::draw(QPainter &painter) {
    QPen pen;
    pen.setWidth(3);
    pen.setColor(color);
    painter.setPen(pen);
    painter.drawLine(x, y, xEnd, yEnd);
    painter.setPen(QPen(Qt::black, 0.5));
}

bool Line::intersectsWithFrame(const QRect frameRect) {
    // Проверяем пересечение линии с фреймом
    return !frameRect.contains(x, y) || !frameRect.contains(xEnd, yEnd);
}


bool Line::contains(int px, int py) {
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

void Line::drawSelected(QPainter &painter)
{
    Shape::drawSelected(painter);
    painter.drawLine(x, y, xEnd, yEnd);
}

bool Line::isOnEdge(int px, int py){
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

bool Line::isStartLine(int px, int py) {
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

void Line::resize(int mouseX, int mouseY, QPoint lastMousePos, QRect frameGeom, bool isStartLine) {
    int dx = mouseX - lastMousePos.x();
    int dy = mouseY - lastMousePos.y();

    qDebug() << "исСтартЛайн" <<isStartLine;
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

Shape *Line::copy() {
    return new Line(*this);
}

void Line::setOffset(int offset){
    x+=offset;
    y+=offset;
    xEnd+=offset;
    yEnd+=offset;
}

void Shape::drawSelected(QPainter &painter)
{
    qDebug() << color;
    QPen pen(Qt::DashLine);
    pen.setColor(Qt::black);
    qDebug() << "ЭЭ";
    pen.setWidth(2);
    painter.setPen(pen);
}

void Shape::move(int mouseX, int mouseY, QPoint lastMousePos, QRect frameGeom) {
    int dx = mouseX - lastMousePos.x();
    int dy = mouseY - lastMousePos.y();

    QRect boundingBox = getBoundingBox();

    QRect newBoundingBox = boundingBox.translated(dx, dy);

    if (frameGeom.contains(newBoundingBox)) {
        x += dx;
        y += dy;
    }
}

void Shape::setColor(QColor color)
{
    this->color = color;
}

void Shape::setOffset(int offset)
{
    x+=offset;
    y+=offset;
}
