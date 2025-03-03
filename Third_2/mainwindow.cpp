#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(&model, &Model::dataUpdated, this, &MainWindow::UpdateDataForm);

    connect(ui->spinBoxA->findChild<QLineEdit *>(), &QLineEdit::returnPressed, this, &MainWindow::on_spinBoxA_EnterPressed);
    connect(ui->spinBoxB->findChild<QLineEdit *>(), &QLineEdit::returnPressed, this, &MainWindow::on_spinBoxB_EnterPressed);
    connect(ui->spinBoxC->findChild<QLineEdit *>(), &QLineEdit::returnPressed, this, &MainWindow::on_spinBoxC_EnterPressed);

    connect(ui->spinBoxA->findChild<QLineEdit *>(), &QLineEdit::editingFinished, this, &MainWindow::on_spinBoxA_editingFinished);
    connect(ui->spinBoxB->findChild<QLineEdit *>(), &QLineEdit::editingFinished, this, &MainWindow::on_spinBoxB_editingFinished);
    connect(ui->spinBoxC->findChild<QLineEdit *>(), &QLineEdit::editingFinished, this, &MainWindow::on_spinBoxC_editingFinished);


    ui->trackBarA->setRange(0, 100);
    ui->trackBarB->setRange(0, 100);
    ui->trackBarC->setRange(0, 100);

    ui->lineEditA->setText(QString::number(model.getValueA()));
    ui->lineEditB->setText(QString::number(model.getValueB()));
    ui->lineEditC->setText(QString::number(model.getValueC()));

    ui->lcdNumberA->display(model.getValueA());
    ui->lcdNumberB->display(model.getValueB());
    ui->lcdNumberC->display(model.getValueC());

    ui->spinBoxA->setValue(model.getValueA());
    ui->spinBoxB->setValue(model.getValueB());
    ui->spinBoxC->setValue(model.getValueC());

    ui->progressBarA->setValue(model.getValueA());
    ui->progressBarB->setValue(model.getValueB());
    ui->progressBarC->setValue(model.getValueC());

    ui->trackBarA->setValue(model.getValueA());
    ui->trackBarB->setValue(model.getValueB());
    ui->trackBarC->setValue(model.getValueC());
}

void MainWindow::UpdateDataForm()
{
    ui->lineEditA->setText(QString::number(model.getValueA()));
    ui->lineEditB->setText(QString::number(model.getValueB()));
    ui->lineEditC->setText(QString::number(model.getValueC()));

    ui->lcdNumberA->display(model.getValueA());
    ui->lcdNumberB->display(model.getValueB());
    ui->lcdNumberC->display(model.getValueC());

    ui->spinBoxA->setValue(model.getValueA());
    ui->spinBoxB->setValue(model.getValueB());
    ui->spinBoxC->setValue(model.getValueC());

    ui->progressBarA->setValue(model.getValueA());
    ui->progressBarB->setValue(model.getValueB());
    ui->progressBarC->setValue(model.getValueC());

    ui->trackBarA->setValue(model.getValueA());
    ui->trackBarB->setValue(model.getValueB());
    ui->trackBarC->setValue(model.getValueC());
}



MainWindow::~MainWindow()
{
    delete ui;
}

void Model::SetValueA(int ValueA)
{
    if (ValueA!=a) {
        a = qBound(0, ValueA, 100);

        if (a > b)
        {
            b = qMin(100, a+1);
            if (b>c){
                c = qMin(100, b+1);
            }

        }
        emit dataUpdated();
        qDebug() << "Отправилось уведолмение от SetValueA";
        SaveSetting();
    }

    // if (oldA!=a || oldB!=b || oldC!=c)
    // {
    //     qDebug() << "Отправилось уведолмение от SetValueA";
    //     emit dataUpdated();
    //     oldA = a;
    //     oldB = b;
    //     oldC = c;
    // }
}

void Model::SetValueB(int ValueB)
{
    if (ValueB!=b) {
        b = qBound(a, ValueB, c);
        emit dataUpdated();
        qDebug() << "Отправилось уведолмение от SetValueB";
        SaveSetting();
    }



    // if (oldA!=a || oldB!=b || oldC!=c)
    // {
    //     qDebug() << "Отправилось уведолмение от SetValueB";

    //     oldA = a;
    //     oldB = b;
    //     oldC = c;
    // }

}

void Model::SetValueC(int ValueC)
{
    if (ValueC!=c) {
        c = qBound(0, ValueC, 100);

        if (c<b)
        {
            b = c;
            if (b<a){
                a = b;
            }
        }
        emit dataUpdated();
        qDebug() << "Отправилось уведолмение от SetValueC";
    }


    // if (oldA!=a || oldB!=b || oldC!=c)
    // {
    //     qDebug() << "Отправилось уведолмение от SetValueC";
        // emit dataUpdated();
    //     oldA = a;
    //     oldB = b;
    //     oldC = c;
    // }
    SaveSetting();
}

void Model::SaveSetting()
{
    QSettings settings("OOP", "MVC");

    settings.setValue("A", a);
    settings.setValue("B", b);
    settings.setValue("C", c);
}

int Model::getValueA()
{
    return a;
}

int Model::getValueB()
{
    return b;
}

int Model::getValueC()
{
    return c;
}


void MainWindow::on_lineEditA_returnPressed()
{
    bool fl;
    int value = ui->lineEditA->text().toInt(&fl);
    if (fl)
    {
        model.SetValueA(value);
    }
    else
    {
        ui->lineEditA->setText(QString::number(model.getValueA()));
    }

}

void MainWindow::on_spinBoxA_EnterPressed()
{
    model.SetValueA(ui->spinBoxA->text().toInt());
}

void MainWindow::on_lineEditB_returnPressed()
{
    bool fl;
    int value = ui->lineEditB->text().toInt(&fl);
    if (fl)
    {
        model.SetValueB(value);
    }
    else
    {
        ui->lineEditB->setText(QString::number(model.getValueB()));
    }
}

void MainWindow::on_spinBoxB_EnterPressed()
{
    model.SetValueB(ui->spinBoxB->text().toInt());
}

void MainWindow::on_lineEditC_returnPressed()
{
    bool fl;
    int value = ui->lineEditC->text().toInt(&fl);
    if (fl)
    {
        model.SetValueC(value);
    }
    else
    {
        ui->lineEditC->setText(QString::number(model.getValueC()));
    }
}

void MainWindow::on_spinBoxC_EnterPressed()
{
    model.SetValueC(ui->spinBoxC->text().toInt());
}

void MainWindow::on_spinBoxA_editingFinished()
{
    qDebug() << "РасфокусСпинА";
    ui->spinBoxA->setValue(model.getValueA());
}

void MainWindow::on_spinBoxB_editingFinished()
{
    qDebug() << "РасфокусСпинB";
    ui->spinBoxB->setValue(model.getValueB());
}

void MainWindow::on_spinBoxC_editingFinished()
{
    qDebug() << "РасфокусСпинC";
    ui->spinBoxC->setValue(model.getValueC());
}

void MainWindow::on_trackBarA_sliderReleased()
{
    model.SetValueA(ui->trackBarA->value());
}

void MainWindow::on_trackBarB_sliderReleased()
{
    model.SetValueB(ui->trackBarB->value());
}

void MainWindow::on_trackBarC_sliderReleased()
{
    model.SetValueC(ui->trackBarC->value());
}


void MainWindow::on_lineEditA_editingFinished()
{
    qDebug() << "РасфокусА";
    ui->lineEditA->setText(QString::number(model.getValueA()));
}

void MainWindow::on_lineEditB_editingFinished()
{
    qDebug() << "РасфокусB";
    ui->lineEditB->setText(QString::number(model.getValueB()));
}


void MainWindow::on_lineEditC_editingFinished()
{
    qDebug() << "РасфокусC";
    ui->lineEditC->setText(QString::number(model.getValueC()));
}


