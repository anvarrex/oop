#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);

    connect(ui->circle, &QAction::triggered, this, &MainWindow::setCircle);
    connect(ui->square, &QAction::triggered, this, &MainWindow::setSqaure);
    connect(ui->ellipse, &QAction::triggered, this, &MainWindow::setEllipse);
    connect(ui->triangle, &QAction::triggered, this, &MainWindow::setTriangle);
    connect(ui->rectangle, &QAction::triggered, this, &MainWindow::setRectangle);
    connect(ui->chooseColor, &QAction::triggered, this, &MainWindow::openColorDialog);
    connect(ui->group, &QAction::triggered, this, &MainWindow::group);
    connect(ui->ungroup, &QAction::triggered, this, &MainWindow::ungroup);
    connect(ui->connectshapes, &QAction::triggered, this, &MainWindow::connectShapes);


    connect(ui->saveas, &QAction::triggered, this, &MainWindow::saveas);
    connect(ui->open, &QAction::triggered, this, &MainWindow::open);

    connect(ui->treeWidget, &QTreeWidget::itemClicked, this, &MainWindow::selectFromTree);

    Params::get().frameGeom = ui->frame->geometry();
    factory = new MyShapeFactory;
    FactoryManager::get().setFactory(factory);

    // ui->treeWidget->setHeaderHidden(true);

    ui->treeWidget->installEventFilter(this);
    tree = new Tree(ui->treeWidget);
    StorageManager::get().storage->addObserver(tree);

}


int counter = 0;



MainWindow::~MainWindow()
{
    while (!(CmdManager::get().history).empty()){
        delete (CmdManager::get().history).back();
        (CmdManager::get().history).pop_back();
    }

    delete factory;

    delete tree;

    delete ui;
}

bool MainWindow::eventFilter(QObject* obj, QEvent* event) {
    if (obj == ui->treeWidget && event->type() == QEvent::FocusOut) {
        qDebug() << "treeWidget потерял фокус!";
        old_selected_shapeTree = nullptr;
    }
    return QMainWindow::eventFilter(obj, event);
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setClipRect(ui->frame->geometry());
    painter.setPen(QPen(Qt::black, 0.5));
    dynamic_cast<MyStorage*>(StorageManager::get().storage)->drawShapes(painter);
}

void MainWindow::mousePressEvent(QMouseEvent *event){

    qDebug() << "Маус пресс ивент";

    if (event->button() == Qt::LeftButton){

        QPoint PosInFrame = event->pos() - Params::get().frameGeom.topLeft();
        MousePressPos = event->pos();
        lastMousePos = event->pos();

        for (Shape* shape: dynamic_cast<MyStorage*>(StorageManager::get().storage)->GetShapes()){
            if (shape->contains(event->pos().x(), event->pos().y())){
                shapesInPoint.push_back(shape);
            }
        }

        int corner = 10;

        bool topLeft = (abs(PosInFrame.x()) < corner && abs(PosInFrame.y()) < corner);
        bool topRight = (abs(PosInFrame.x() - Params::get().frameGeom.width()) < corner && abs(PosInFrame.y()) < corner);
        bool bottomLeft = (abs(PosInFrame.x()) < corner && abs(PosInFrame.y() - Params::get().frameGeom.height()) < corner);
        bool bottomRight = (abs(PosInFrame.x() - Params::get().frameGeom.width()) < corner && abs(PosInFrame.y() - Params::get().frameGeom.height()) < corner);


        if (topLeft || topRight || bottomLeft || bottomRight) {
            isResizingFrame = true;
            lastMousePos = event->pos();
            return;
        }

        if (shapesInPoint.size()!=0)
        {
            for (Shape* shapeInPoint: shapesInPoint){
                for (Shape* selected_shape: dynamic_cast<MyStorage*>(StorageManager::get().storage)->GetSelectedShapes()){
                    if (shapeInPoint == selected_shape){
                        if (shapeInPoint->isOnEdge(event->pos().x(), event->pos().y())){
                            isResizingShape = true;
                            return;
                        }
                        isMovingShape = true;
                        return;
                    }
                }
            }
        }
    }
}


void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    qDebug() << isMovingShape;
    qDebug() << "Маус мув ивент";


    if (isResizingShape){

        int dx = event->pos().x() - lastMousePos.x();
        int dy = event->pos().y() - lastMousePos.y();

        qDebug() << dx << " " << dy;

        if (dx != 0 || dy !=0){
            Command *resizecommand = new ResizeCommand(dx, dy);

            dynamic_cast<MyStorage*>(StorageManager::get().storage)->executeCommand(resizecommand);

            updateHistory();

            delete resizecommand;
        }



        lastMousePos = event->pos();

        update();
    }

    if (isMovingShape) {

        int dx = event->pos().x() - lastMousePos.x();
        int dy = event->pos().y() - lastMousePos.y();

        if (dx != 0 || dy !=0){
            Command *movecommand = new MoveCommand(dx, dy);




            dynamic_cast<MyStorage*>(StorageManager::get().storage)->executeCommand(movecommand);

            updateHistory();

            delete movecommand;
        }

        lastMousePos = event->pos();

        update();

    }

    if (isResizingFrame){

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
        for (Shape* shape : dynamic_cast<MyStorage*>(StorageManager::get().storage)->GetShapes()) {
            if (shape->intersectsWithFrame(newFrameRect)) {
                intersects = true;
                break;
            }
        }

        // Если пересечения нет, изменяем размер фрейма
        if (!intersects) {
            ui->frame->resize(newWidth, newHeight);
            Params::get().frameGeom = ui->frame->geometry();
            lastMousePos = event->pos();
        }
    }


}




void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    qDebug() << "Маус релиз ивент";

    isMovingShape = false;
    isResizingShape = false;
    isResizingFrame = false;

    update();

    QPoint PosInFrame = event->pos() - Params::get().frameGeom.topLeft();

    int corner = 10;

    bool topLeft = (abs(PosInFrame.x()) < corner && abs(PosInFrame.y()) < corner);
    bool topRight = (abs(PosInFrame.x()- Params::get().frameGeom.width()) < corner && abs(PosInFrame.y()) < corner);
    bool bottomLeft = (abs(PosInFrame.x()) < corner && abs(PosInFrame.y() - Params::get().frameGeom.height()) < corner);
    bool bottomRight = (abs(PosInFrame.x()- Params::get().frameGeom.width()) < corner && abs(PosInFrame.y() -  Params::get().frameGeom.height()) < corner);


    if (topLeft || topRight || bottomLeft || bottomRight) {
        return;
    }


    if (MousePressPos == event->pos()){
        if (ui->frame->geometry().contains(event->pos())) {
            if (shapesInPoint.size()!=0){
                Command* command = new SelectCommand(shapesInPoint);
                command->execute(shapesInPoint[0]);
                CmdManager::get().history.push_back(command);
                updateHistory();
            }
            else{
                qDebug() << ++counter ;
                Command* newcommand = new AddObjCommand();
                newcommand->execute(FactoryManager::get().factory->createShape(event->pos().x(), event->pos().y()));
                CmdManager::get().history.push_back(newcommand);
                qDebug()<< "stack++";
                updateHistory();

            }
            update();
        }
    }
    shapesInPoint.clear();
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    Command *command = CmdManager::get().commands[event->key()];

    if (command!=nullptr){

        for (Shape* shape: dynamic_cast<MyStorage*>(StorageManager::get().storage)->GetShapes()){
            shape->setMoved(false);
        }

        dynamic_cast<MyStorage*>(StorageManager::get().storage)->executeCommand(command);

        updateHistory();

        update();
    }

    else if(event->key() == Qt::Key_Z && !CmdManager::get().history.empty()){
        for (Shape* shape: dynamic_cast<MyStorage*>(StorageManager::get().storage)->GetShapes()){
            shape->setMoved(false);
        }
        Command *lastcommand = CmdManager::get().history.back();
        lastcommand->unexecute();
        delete lastcommand;
        CmdManager::get().history.pop_back();
        updateHistory();
        update();
    }

    else if (event->key() == Qt::Key_Backspace || event->key() == Qt::Key_Delete) {
        qDebug() << "Del нажата!";



        vector <Shape*> shapesToDelete;

        shapesToDelete = dynamic_cast<MyStorage*>(StorageManager::get().storage)->GetSelectedShapes();


        if (shapesToDelete.size()!=0){
            Command* command = new DeleteCommand(shapesToDelete);
            command->execute(shapesToDelete[0]);
            CmdManager::get().history.push_back(command);
            updateHistory();
            update();
        }

    }

    else if (event->modifiers() & Qt::ControlModifier && event->key() == Qt::Key_A) {
        Command *newcommand = new SelectCommand(dynamic_cast<MyStorage*>(StorageManager::get().storage)->GetShapes());
        CmdManager::get().history.push_back(newcommand);
        updateHistory();
        update();
    }


    else if (event->modifiers() & Qt::ControlModifier && event->key() == Qt::Key_Z) {

    }

    else if (event->key() == Qt::Key_Control){
        qDebug() << "Ctrl нажата!";
        Params::get().ctrl = true;
    }

    QMainWindow::keyPressEvent(event);
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Control){
        qDebug() << "Ctrl отпущена!";
        Params::get().ctrl = false;
    }
}

void MainWindow::updateHistory()
{
    ui->Commands->setText(CmdManager::get().showHistory());
}

void MainWindow::connectShapes()
{
    MyStorage* mystorage = dynamic_cast<MyStorage*>(StorageManager::get().storage);
    if (mystorage->GetSelectedShapes().size() == 2){
        Shape *a = mystorage->GetSelectedShapes()[0];
        Shape *b = mystorage->GetSelectedShapes()[1];
        Command *command = new createArrowCommand(a, b);
        command->execute(a);
        CmdManager::get().history.push_back(command);
        updateHistory();
    }
    else{
        QMessageBox::warning(this, "Внимание", "Выберите две объекта!");
    }
    update();

}

void MainWindow::openColorDialog()
{
    QColor color = QColorDialog::getColor(Params::get().selected_color, this);

    if (color.isValid()) {
        Params::get().selected_color = color;

        Command* command = new SetColorCommand(color);

        dynamic_cast<MyStorage*>(StorageManager::get().storage)->executeCommand(command);

        updateHistory();

        delete command;
    }
}

void MainWindow::setCircle(){
    Params::get().selected_shape = 'C';
}

void MainWindow::setSqaure(){
    Params::get().selected_shape = 'S';
}

void MainWindow::setEllipse(){
    Params::get().selected_shape = 'E';
}

void MainWindow::setRectangle(){
    Params::get().selected_shape = 'R';
}


void MainWindow::setTriangle(){
    Params::get().selected_shape = 'T';
}

void MainWindow::group(){
    if (dynamic_cast<MyStorage*>(StorageManager::get().storage)->GetSelectedShapes().size()<2){
        QMessageBox::warning(this, "Внимание", "Выберите хотя бы 2 фигуры!");
        return;
    }
    Shape* shape = nullptr;
    Command* newcommand = new GroupCommand();
    newcommand->execute(shape);
    CmdManager::get().history.push_back(newcommand);
    QMessageBox::information(this, "Заголовок", "Группировка выполнена!");
    updateHistory();
    update();
}

void MainWindow::ungroup(){

    bool isGroup = true;
    if(dynamic_cast<MyStorage*>(StorageManager::get().storage)->GetSelectedShapes().size()!=1){
        QMessageBox::warning(this, "Внимание", "Выберите одну группу!");
        return;
    }
    Command* newcommand = new UnGroupCommand();
    newcommand->execute(dynamic_cast<MyStorage*>(StorageManager::get().storage)->GetSelectedShapes()[0]);
    CmdManager::get().history.push_back(newcommand);
    QMessageBox::information(this, "Заголовок", "Разгруппировка выполнена!");
    updateHistory();

    update();
}

void MainWindow::saveas()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Сохранить файл", QDir::homePath(), "Text Files (*.txt)");
    if (fileName.isEmpty()) return;


    FILE* stream = fopen(fileName.toStdString().c_str(), "w");
    if (!stream) {
        QMessageBox::critical(this, "Ошибка", "Не удалось открыть файл");
        return;
    }
    else{
        dynamic_cast<MyStorage*>(StorageManager::get().storage)->saveShapes(stream);
    }
    fclose(stream);
}

void MainWindow::open()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Открыть файл", QDir::homePath(), "Text Files (*.txt)");
    if (fileName.isEmpty()) return;

    FILE* stream = fopen(fileName.toStdString().c_str(), "r");
    if (!stream) {
        QMessageBox::critical(this, "Ошибка", "Не удалось открыть файл");
        return;
    }
    else{
        dynamic_cast<MyStorage*>(StorageManager::get().storage)->loadShapes(stream);
    }
    fclose(stream);
}

void MainWindow::selectFromTree()
{
    Shape* _shape = tree->getSelectedShapes();
    qDebug() << _shape;

    Shape* root = findRootGroup(_shape, dynamic_cast<MyStorage*>(StorageManager::get().storage)->GetShapes());
    if (root) {
        _shape = root;
    }

    if (old_selected_shapeTree != _shape){
        if (_shape) {
            Command* newcommand = new SelectFromTreeCommand();
            newcommand->execute(_shape);
            CmdManager::get().history.push_back(newcommand);
            updateHistory();
        }
    }
    qDebug() << _shape;
    old_selected_shapeTree = _shape;
    update();
}

Shape *MainWindow::findRootGroup(Shape *shape, const std::vector<Shape *> &shapes)
{
    for (Shape* candidate : shapes) {
        Group* group = dynamic_cast<Group*>(candidate);
        if (!group) continue;

        for (Shape* member : group->getShapes()) {
            if (member == shape) {
                return group; // Прямо входит в эту группу
            }

            // Рекурсивно ищем в подгруппах
            Shape* found = findRootGroup(shape, group->getShapes());
            if (found) {
                return group; // Нашли во вложенной — значит верхняя группа это group
            }
        }
    }
    return nullptr;
}


Circle::Circle(int ValueX, int ValueY): Shape(ValueX, ValueY){
    if (x - rad < Params::get().frameGeom.x()) x = rad + Params::get().frameGeom.x();
    if (y - rad < Params::get().frameGeom.y()) y = rad + Params::get().frameGeom.y();
    if (x + rad > Params::get().frameGeom.width()+ Params::get().frameGeom.x()) x = Params::get().frameGeom.x() + Params::get().frameGeom.width() - rad;
    if (y + rad > Params::get().frameGeom.height()+ Params::get().frameGeom.y()) y = Params::get().frameGeom.y() + Params::get().frameGeom.height() - rad;
}


bool Circle::contains(int px, int py)
{
    int dx = x - px;
    int dy = y - py;
    return (dx * dx + dy * dy) <= (rad * rad);
}


bool Circle::intersectsWithFrame(const QRect frameRect){
    QRect circleRect = getBoundingBox();
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

bool Circle::resize(int dx, int dy){

    int currrad = rad;

    rad += dx;
    rad += dy;

    if (rad < 15){
        rad = 15;
    };


    QRect boundingBox = getBoundingBox();

    if (!Params::get().frameGeom.contains(boundingBox)) {
        // Автоматическое уменьшение радиуса до максимально возможного
        int maxLeft   = x - Params::get().frameGeom.left();
        int maxTop    = y - Params::get().frameGeom.top();
        int maxRight  = Params::get().frameGeom.x()+Params::get().frameGeom.width() - x;
        int maxBottom = Params::get().frameGeom.y()+Params::get().frameGeom.height()- y;

        int maxRad = std::min({maxLeft, maxTop, maxRight, maxBottom});

        // Сохраняем минимально допустимый радиус
        maxRad = std::max(15, maxRad);

        rad = maxRad;


    }

    return currrad != rad;
}


void MyStorage::AddObj(Shape *shape)
{
    shapes.push_back(shape);
    unselectAllShapes();
    notifyEveryone();
}

void MyStorage::drawShapes(QPainter &painter)
{
    for (Shape* shape: shapes){
        shape->draw(painter);
    }

}



void MyStorage::selectShapes(vector<Shape*> shapesInPoint)
{
    if (!Params::get().ctrl)
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

    qDebug() << "MyStorage::selectShapes";
    qDebug() << shapesInPoint;
    qDebug() << selected_shapes;

    for (Shape* shape: shapes){
        shape->select(false);
    }

    for (Shape* shape: selected_shapes){
        shape->select(true);
    }

    notifyEveryone();

}


void MyStorage::deleteShapes()
{
    for (Shape* selected_shape: selected_shapes){
        Arrow *d = dynamic_cast<Arrow*>(selected_shape);
        if (d){
            d->unsubscribe();
        }
        shapes.erase(remove(shapes.begin(), shapes.end(), selected_shape), shapes.end());
        Shape* arrow = selected_shape->get_Arrow();
        if (arrow!=nullptr){
            shapes.erase(remove(shapes.begin(), shapes.end(), arrow), shapes.end());
        }
    }

    selected_shapes.clear();
    notifyEveryone();
}

vector<Shape *> MyStorage::GetSelectedShapes(){
    return selected_shapes;
}

vector<Shape *> MyStorage::GetShapes(){
    return shapes;
}

void MyStorage::executeCommand(Command *command)
{
    bool fl = false;

    for (Shape* shape: shapes){
        shape->setMoved(false);
    }

    if (selected_shapes.size() > 1){
        for (Shape* selected_shape: selected_shapes){
            selected_shape->setMoved(true);
        }
    }


    Command* commandsequence = new CommandsSequence();
    for (Shape* selected_shape: selected_shapes){
        Command* newcommand = command->clone();
        fl = newcommand->execute(selected_shape);
        if (fl){
            qDebug() << "stack++";
            dynamic_cast<CommandsSequence*>(commandsequence)->addCommand(newcommand);

        }
        else {
            delete newcommand;
        }
    }
    if (fl){
        CmdManager::get().history.push_back(commandsequence);
    }
    else{
        delete commandsequence;
    }

}



Rectangle::Rectangle(int ValueX, int ValueY): Shape(ValueX, ValueY), width(60), height(40){
    if (x < Params::get().frameGeom.x()) x = Params::get().frameGeom.x();
    if (y < Params::get().frameGeom.y()) y = Params::get().frameGeom.y();
    if (x + width > Params::get().frameGeom.x()+Params::get().frameGeom.width()) x = Params::get().frameGeom.x() + Params::get().frameGeom.width() - width;
    if (y + height > Params::get().frameGeom.y()+Params::get().frameGeom.height()) y =Params::get().frameGeom.y() + Params::get().frameGeom.height() - height;
}


bool Rectangle::contains(int px, int py) {
    return (px >= x && px <= x + width && py >= y && py <= y + height);
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

bool Rectangle::resize(int dx, int dy) {

    int currWidth = width;
    int currHeight = height;

    // Изменяем размеры
    width += dx;
    height += dy;

    // Учитываем минимальные размеры
    width = std::max(60, width);
    height = std::max(40, height);

    // Получаем текущую границу прямоугольника
    QRect boundingBox = getBoundingBox();
    QRect frame = Params::get().frameGeom;

    // Проверяем, если выходит за пределы фрейма, откатываем до максимального возможного
    if (!frame.contains(boundingBox)) {
        // Ограничиваем размеры прямоугольника
        if (boundingBox.right() > frame.right()) {
            width -= (boundingBox.right() - frame.right());
        }
        if (boundingBox.bottom() > frame.bottom()) {
            height -= (boundingBox.bottom() - frame.bottom());
        }
    }

    return currWidth != width || currHeight != height;
}


Ellipse::Ellipse(int ValueX, int ValueY) : Shape(ValueX, ValueY), widthR(30), heightR(20){

    // Проверяем, что эллипс не выходит за пределы области
    if (x - widthR < Params::get().frameGeom.x()) x = Params::get().frameGeom.x() + widthR;
    if (y - heightR < Params::get().frameGeom.y()) y = Params::get().frameGeom.y() + heightR;
    if (x + widthR > Params::get().frameGeom.x() + Params::get().frameGeom.width()) x = Params::get().frameGeom.x() + Params::get().frameGeom.width() - widthR;
    if (y + heightR > Params::get().frameGeom.y() + Params::get().frameGeom.height()) y = Params::get().frameGeom.y() + Params::get().frameGeom.height() - heightR;

}

bool Ellipse::intersectsWithFrame(const QRect frameRect) {
    // Проверяем пересечение эллипса с фреймом
    QRect ellipseRect(x - widthR, y - heightR, 2 * widthR, 2 * heightR);
    return !frameRect.contains(ellipseRect);
}



bool Ellipse::contains(int px, int py) {
    double dx = px - x;  // Смещение по X от центра
    double dy = py - y;  // Смещение по Y от центра

    double a = widthR;   // Полуось по X (горизонтальный радиус)
    double b = heightR;  // Полуось по Y (вертикальный радиус)

    return (dx * dx) / (a * a) + (dy * dy) / (b * b) <= 1.0;
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

bool Ellipse::resize(int dx, int dy) {

    int currWidthR = widthR;
    int currHeightR = heightR;

    // Увеличиваем размеры
    widthR += dx;
    heightR += dy;

    // Минимальные размеры
    widthR = std::max(30, widthR);
    heightR = std::max(20, heightR);

    // Получаем ограничивающий прямоугольник эллипса
    QRect boundingBox = getBoundingBox();
    QRect frame = Params::get().frameGeom;

    // Если эллипс выходит за пределы фрейма, откатываем изменения
    if (!frame.contains(boundingBox)) {
        // Откатываем размеры до максимально возможных
        if (boundingBox.left() < frame.left()) {
            widthR = x - frame.x();
        }
        if (boundingBox.top() < frame.top()) {
            heightR = y - frame.y();
        }

        if (boundingBox.right() > frame.right()) {
            widthR = frame.x()+frame.width() - x;
        }
        if (boundingBox.bottom() > frame.bottom()) {
            heightR = frame.y()+frame.height() - y;
        }
    }

    // Возвращаем, если размеры изменились
    return currWidthR != widthR || currHeightR != heightR;

}


Triangle::Triangle(int ValueX, int ValueY): Shape(ValueX, ValueY), size(30){
    if (x - size < Params::get().frameGeom.x()) x = size+Params::get().frameGeom.x();
    if (y - size < Params::get().frameGeom.y()) y = size+Params::get().frameGeom.y();
    if (x + size > Params::get().frameGeom.width()+Params::get().frameGeom.x()) x = Params::get().frameGeom.x() + Params::get().frameGeom.width() - size;
    if (y + size > Params::get().frameGeom.height()+Params::get().frameGeom.y()) y = Params::get().frameGeom.y() + Params::get().frameGeom.height() - size;
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

bool Triangle::resize(int dx, int dy) {

    int currSize = size;

    int delta;
    if (dx < 0 || dy < 0) {
        delta = std::min(dx, dy);
    } else {
        delta = std::max(dx, dy);
    }

    size += delta;

    // Учитываем минимальный размер
    if (size < 30)
        size = 30;

    QRect boundingBox = getBoundingBox();
    QRect frame = Params::get().frameGeom;

    if (!frame.contains(boundingBox)) {
        // Вычислим максимально возможный size при текущем положении
        int maxLeft   = x - frame.left();
        int maxTop    = y - frame.top();
        int maxRight  = frame.x() + frame.width() - x;
        int maxBottom = frame.y() + frame.height() - y;

        // Треугольник вверх — size влияет на высоту вверх и ширину в стороны
        int maxHorizontal = std::min(maxLeft, maxRight);
        int maxVertical = std::min(maxTop, maxBottom);

        int maxAllowedSize = std::min(maxHorizontal, maxVertical);

        size = std::max(30, maxAllowedSize);
    }

    return size != currSize;
}



Square::Square(int ValueX, int ValueY): Shape(ValueX, ValueY), size(40) {
    if (x < Params::get().frameGeom.x()) x = Params::get().frameGeom.x();
    if (y < Params::get().frameGeom.y()) y = Params::get().frameGeom.y();
    if (x + size > Params::get().frameGeom.x()+Params::get().frameGeom.width()) x = Params::get().frameGeom.x() + Params::get().frameGeom.width() - size;
    if (y + size > Params::get().frameGeom.y()+Params::get().frameGeom.height()) y = Params::get().frameGeom.y() + Params::get().frameGeom.height() - size;
}


bool Square::contains(int px, int py) {
    return (px >= x && px <= x + size && py >= y && py <= y + size);
}

bool Square::intersectsWithFrame(const QRect frameRect) {
    // Проверяем пересечение квадрата с фреймом
    QRect shapeRect(x, y, size, size);
    return !frameRect.contains(shapeRect);
}

bool Square::isOnEdge(int px, int py){
    int edgeThreshold = 5;  // Толщина зоны для изменения размеров
    bool left = abs(px - x) < edgeThreshold;
    bool right = abs(px - (x + size)) < edgeThreshold;
    bool top = abs(py - y) < edgeThreshold;
    bool bottom = abs(py - (y + size)) < edgeThreshold;
    return left || right || top || bottom;
}


bool Square::resize(int dx, int dy) {

    int currsize = size;

    // Вычисляем, на сколько нужно изменить размер
    int delta;
    if (dy < 0 || dx < 0) {
        delta = std::min(dx, dy);
    } else {
        delta = std::max(dx, dy);
    }

    // Изменяем размер квадрата
    size += delta;

    // Учитываем минимальный размер
    if (size < 40) {
        size = 40;
    }

    // Получаем boundingBox квадрата
    QRect boundingBox = getBoundingBox();
    QRect frame = Params::get().frameGeom;

    // Проверяем, если размер выходит за пределы фрейма
    if (!frame.contains(boundingBox)) {
        // Ограничиваем размер, если выходит за пределы фрейма
        int maxWidth = frame.x()+frame.width() - x;
        int maxHeight = frame.y()+frame.height() - y;

        // Устанавливаем максимальный размер, если выходит за границы
        size = std::min(maxWidth, maxHeight);

        // Если размер меньше минимального, устанавливаем минимальный
        size = std::max(40, size);
        return false;
    }

    return currsize != size;
}

bool Shape::move(int &dx, int &dy) {

    QRect boundingBox = getBoundingBox();

    QRect newboundingBox = boundingBox.translated(dx, dy);

    if (Params::get().frameGeom.contains(newboundingBox)) {
        x += dx;
        y += dy;

        notifyEveryone();
        return true;
    }

    else {

        int maxDx = dx;
        int maxDy = dy;


        if (newboundingBox.left() < Params::get().frameGeom.left()) {
            maxDx = Params::get().frameGeom.left() - boundingBox.left();
        } else if (newboundingBox.right() > Params::get().frameGeom.right()) {
            maxDx = Params::get().frameGeom.right() - boundingBox.right();
        }


        if (newboundingBox.top() < Params::get().frameGeom.top()) {
            maxDy = Params::get().frameGeom.top() - boundingBox.top();
        } else if (newboundingBox.bottom() > Params::get().frameGeom.bottom()) {
            maxDy = Params::get().frameGeom.bottom() - boundingBox.bottom();
        }

        x += maxDx;
        y += maxDy;

        dx = maxDx;
        dy = maxDy;

        if (maxDx!=0 or maxDy!=0){
            return true;
        }

        notifyEveryone();
        return false;
    }
}

void Shape::setColor(QColor color)
{
    this->color = color;
}


void MainWindow::on_clearHistory_clicked()
{
    CmdManager::get().clearHistory();
    update();
    updateHistory();
}

