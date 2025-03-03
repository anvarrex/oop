#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QRandomGenerator>
#include <QSettings>
#include <QKeyEvent>
#include <QSpinBox>
#include <QLineEdit>
#include <QString>

using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class Model: public QObject
{
    Q_OBJECT

private:
    int a, b, c;
public:
    Model()
    {
        QSettings settings("OOP", "MVC");
        a = settings.value("A", 10).toInt();
        b = settings.value("B", 50).toInt();
        c = settings.value("C", 90).toInt();

    }
    void SetValueA(int ValueA);
    void SetValueB(int ValueB);
    void SetValueC(int ValueC);
    void SaveSetting();
    int getValueA();
    int getValueB();
    int getValueC();

signals:
    void dataUpdated();
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    Model model;

protected:
    void UpdateDataForm();
    void LoadSettings();
    void SaveSettings();
    void on_spinBoxA_EnterPressed();
    void on_spinBoxB_EnterPressed();
    void on_spinBoxC_EnterPressed();
    void on_spinBoxA_editingFinished();
    void on_spinBoxB_editingFinished();
    void on_spinBoxC_editingFinished();

private slots:
    void on_lineEditA_returnPressed();
    void on_lineEditB_returnPressed();
    void on_lineEditC_returnPressed();
    void on_trackBarA_sliderReleased();
    void on_trackBarB_sliderReleased();
    void on_trackBarC_sliderReleased();
    void on_lineEditA_editingFinished();
    void on_lineEditB_editingFinished();
    void on_lineEditC_editingFinished();
};

#endif // MAINWINDOW_H
