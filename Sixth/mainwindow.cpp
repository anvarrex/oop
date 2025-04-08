#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow), storage(&history){
    ui->setupUi(this);

    commands[Qt::Key_A] = new MoveCommand(-10, 0);
    commands[Qt::Key_W] = new MoveCommand(0, -10);
    commands[Qt::Key_S] = new MoveCommand(0, 10);
    commands[Qt::Key_D] = new MoveCommand(10, 0);

    connect(ui->circle, &QAction::triggered, this, &MainWindow::setCircle);
    connect(ui->square, &QAction::triggered, this, &MainWindow::setSqaure);
    connect(ui->ellipse, &QAction::triggered, this, &MainWindow::setEllipse);
    connect(ui->triangle, &QAction::triggered, this, &MainWindow::setTriangle);
    connect(ui->rectangle, &QAction::triggered, this, &MainWindow::setRectangle);
    connect(ui->chooseColor, &QAction::triggered, this, &MainWindow::openColorDialog);
    //connect(ui->group, &QAction::triggered, this, &MainWindow::Group);
    connect(ui->ungroup, &QAction::triggered, this, &MainWindow::openColorDialog);

    frameGeom = ui->frame->geometry();
}


int counter = 0;


MainWindow::~MainWindow()
{
    while (!history.empty()){
        delete history.top();
        history.pop();
    }

    qDebug() << "Delete prototypes:";
    delete commands[Qt::Key_A];
    delete commands[Qt::Key_W];
    delete commands[Qt::Key_S];
    delete commands[Qt::Key_D];

    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setClipRect(ui->frame->geometry());
    painter.setPen(QPen(Qt::black, 0.5));
    storage.drawShapes(painter);
    storage.drawSelectedShapes(painter);
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
    }
}


void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    qDebug() << isMoving;
    qDebug() << "Маус мув ивент";


    if (isResizingShape){

        int dx = event->pos().x() - lastMousePos.x();
        int dy = event->pos().y() - lastMousePos.y();

        storage.resizeShape(dx, dy);
        lastMousePos = event->pos();

        update();
    }

    if (isMoving) {

        int dx = event->pos().x() - lastMousePos.x();
        int dy = event->pos().y() - lastMousePos.y();


        storage.moveShape(dx, dy, ui->frame->geometry());
        lastMousePos = event->pos();

        update();

    }

    if (isResizing){

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
            frameGeom = ui->frame->geometry();
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

    QPoint PosInFrame = event->pos() - frameGeom.topLeft();

    int corner = 10;

    bool topLeft = (abs(PosInFrame.x()) < corner && abs(PosInFrame.y()) < corner);
    bool topRight = (abs(PosInFrame.x()- frameGeom.width()) < corner && abs(PosInFrame.y()) < corner);
    bool bottomLeft = (abs(PosInFrame.x()) < corner && abs(PosInFrame.y() - frameGeom.height()) < corner);
    bool bottomRight = (abs(PosInFrame.x()- frameGeom.width()) < corner && abs(PosInFrame.y() -  frameGeom.height()) < corner);


    if (topLeft || topRight || bottomLeft || bottomRight) {
        return;
    }


    if (MousePosCirclePress == event->pos()){
        if (ui->frame->geometry().contains(event->pos())) {
            if (shapesInPoint.size()!=0){
                storage.selectShapes(shapesInPoint, ctrl);
            }
            else{
                qDebug() << ++counter ;
                if (selected_shape == "Circle"){
                    storage.AddObject(new Circle(event->pos().x(), event->pos().y(), selected_color, &frameGeom));
                }
                else if (selected_shape == "Ellipse"){
                    storage.AddObject(new Ellipse(event->pos().x(), event->pos().y(), selected_color, &frameGeom));
                }
                else if (selected_shape == "Rectangle"){
                    storage.AddObject(new Rectangle(event->pos().x(), event->pos().y(), selected_color,&frameGeom));
                }
                else if (selected_shape == "Triangle"){
                    storage.AddObject(new Triangle(event->pos().x(), event->pos().y(), selected_color, &frameGeom));
                }
                else if (selected_shape == "Square"){
                    storage.AddObject(new Square(event->pos().x(), event->pos().y(), selected_color, &frameGeom));
                }
            }
            update();
        }
    }
    shapesInPoint.clear();
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    Command *movecommand = commands[event->key()];

    if (movecommand!=nullptr){
        storage.moveShapeFromKeyBoard(movecommand);
        update();
    }

    else if(event->key() == Qt::Key_Z && !history.empty()){
        Command *lastcommand = history.top();
        lastcommand->unexecute();
        delete lastcommand;
        history.pop();
        update();
    }

    else if (event->key() == Qt::Key_Backspace || event->key() == Qt::Key_Delete) {
        qDebug() << "Del нажата!";
        storage.deleteShapes();
        update();
    }

    else if (event->modifiers() & Qt::ControlModifier && event->key() == Qt::Key_A) {
        storage.selectAll();
        update();
    }


    else if (event->modifiers() & Qt::ControlModifier && event->key() == Qt::Key_Z) {

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


void MainWindow::setTriangle(){
    selected_shape = "Triangle";
}

void MainWindow::group()
{

}

Circle::Circle(int ValueX, int ValueY, QColor color, QRect* pframeGeom): Shape(ValueX, ValueY, color, pframeGeom){
    if (x - rad < pframeGeom->x()) x = rad + pframeGeom->x();
    if (y - rad < pframeGeom->y()) y = rad+ pframeGeom->y();
    if (x + rad > pframeGeom->width()+ pframeGeom->x()) x = pframeGeom->x() + pframeGeom->width() - rad;
    if (y + rad > pframeGeom->height()+ pframeGeom->y()) y = pframeGeom->y() + pframeGeom->height() - rad;
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

void Circle::resize(int dx, int dy){
    rad += dx;
    rad += dy;
    if (rad < 15) rad = 15;

    QRect boundingBox = getBoundingBox();

    if (!pframeGeom->contains(boundingBox)) {
        rad += -dx;
        rad += -dy;
    }
}


void MyStorage::AddObject(Shape *shape)
{
    Command *addobjectcommand = new AddObjectCommand(&shapes, &selected_shapes);
    addobjectcommand->execute(shape);
    _phistory->push(addobjectcommand);
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

void MyStorage::moveShape(int dx, int dy, QRect frameGeom)
{
    for (Shape* selected_shape: selected_shapes){
        Command* newcommand = new MoveCommand(dx, dy);
        newcommand->execute(selected_shape);
        _phistory->push(newcommand);
    }

}

void MyStorage::moveShapeFromKeyBoard(Command *movecommand)
{
    for (Shape* selected_shape: selected_shapes){
        Command *newcommand = movecommand->clone();
        newcommand->execute(selected_shape);
        _phistory->push(newcommand);
    }
}

void MyStorage::resizeShape(int dx, int dy){
    for (Shape* selected_shape: selected_shapes){
        selected_shape->resize(dx, dy);
    }
}


void MyStorage::selectAll(){
    selected_shapes = shapes;
}


Rectangle::Rectangle(int ValueX, int ValueY, QColor color, QRect* pframeGeom): Shape(ValueX, ValueY, color, pframeGeom), width(60), height(40){
    if (x < pframeGeom->x()) x = pframeGeom->x();
    if (y < pframeGeom->y()) y = pframeGeom->y();
    if (x + width > pframeGeom->x()+pframeGeom->width()) x = pframeGeom->x() + pframeGeom->width() - width;
    if (y + height > pframeGeom->y()+pframeGeom->height()) y =pframeGeom->y() + pframeGeom->height() - height;
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

void Rectangle::resize(int dx, int dy) {
    width += dx;
    height += dy;
    width = max(60, width);
    height = max(40, height);

    QRect boundingBox = getBoundingBox();

    if (!pframeGeom->contains(boundingBox)) {
        width += -dx;
        height += -dy;
    }
}


Ellipse::Ellipse(int ValueX, int ValueY, QColor color, QRect* pframeGeom) : Shape(ValueX, ValueY, color, pframeGeom), widthR(30), heightR(20){

    // Проверяем, что эллипс не выходит за пределы области
    if (x - widthR < pframeGeom->x()) x = pframeGeom->x() + widthR;
    if (y - heightR < pframeGeom->y()) y = pframeGeom->y() + heightR;
    if (x + widthR > pframeGeom->x() + pframeGeom->width()) x = pframeGeom->x() + pframeGeom->width() - widthR;
    if (y + heightR > pframeGeom->y() + pframeGeom->height()) y = pframeGeom->y() + pframeGeom->height() - heightR;

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

void Ellipse::resize(int dx, int dy) {

    // Изменение размеров эллипса
    widthR += dx;
    heightR += dy;

    // Ограничение минимального размера
    widthR = std::max(30, widthR);
    heightR = std::max(20, heightR);

    QRect boundingBox = getBoundingBox();

    if (!pframeGeom->contains(boundingBox)) {
        heightR += -dy;
        widthR += -dx;
    }

}


Triangle::Triangle(int ValueX, int ValueY, QColor color, QRect* pframeGeom): Shape(ValueX, ValueY, color, pframeGeom), size(30){
    if (x - size < pframeGeom->x()) x = size+pframeGeom->x();
    if (y - size < pframeGeom->y()) y = size+pframeGeom->y();
    if (x + size > pframeGeom->width()+pframeGeom->x()) x = pframeGeom->x() + pframeGeom->width() - size;
    if (y + size > pframeGeom->height()+pframeGeom->y()) y = pframeGeom->y() + pframeGeom->height() - size;
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

void Triangle::resize(int dx, int dy) {

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

    if (!pframeGeom->contains(boundingBox)) {
        size += -delta;
    }
}



Square::Square(int ValueX, int ValueY, QColor color, QRect* pframeGeom): Shape(ValueX, ValueY, color, pframeGeom), width(40), height(40){
    if (x < pframeGeom->x()) x = pframeGeom->x();
    if (y < pframeGeom->y()) y = pframeGeom->y();
    if (x + width > pframeGeom->x()+pframeGeom->width()) x = pframeGeom->x() + pframeGeom->width() - width;
    if (y + height > pframeGeom->y()+pframeGeom->height()) y = pframeGeom->y() + pframeGeom->height() - height;
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


void Square::resize(int dx, int dy) {

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

    if (!pframeGeom->contains(boundingBox)) {
        width += -delta;
        height += -delta;
    }
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

void Shape::move(int dx, int dy) {

    QRect boundingBox = getBoundingBox();

    QRect newboundingBox = boundingBox.translated(dx, dy);

    if (pframeGeom->contains(newboundingBox)) {
        x += dx;
        y += dy;
    }
}

void Shape::setColor(QColor color)
{
    this->color = color;
}

