from PyQt5 import QtWidgets, QtGui, QtCore
from PyQt5.QtWidgets import QApplication, QMainWindow, QTableView, QInputDialog, QMessageBox, QFileDialog
from PyQt5.QtCore import QItemSelection, QItemSelectionModel
from scipy import optimize


import numpy as np
import pyqtgraph as pg
import h5py


import sys
import os

from model import TableModel
from delegate import ComboBoxDelegate


class MyWindow(QMainWindow):
    # Инициализация главного окна
    def __init__(self):
        super(MyWindow, self).__init__()

        self.setWindowTitle("Тестовое задание")  # Устанавливаем название окна
        self.setMinimumSize(1100, 700)  # Устанавливаем минимальный размер окна
        self.initUI()  # Инициализируем интерфейс

    def initUI(self):
        layout1 = QtWidgets.QVBoxLayout()  # Создаем вертикальный layout для основного окна
        layout2 = QtWidgets.QHBoxLayout()  # Создаем горизонтальный layout для кнопок

        self.model = TableModel()  # Создаем объект модели данных для таблицы

        self.model.dataChangedSignal.connect(self.updateGraph)  # Соединяем сигнал обновления данных с функцией обновления графика

        self.table = QTableView()  # Создаем таблицу для отображения данных
        self.table.setModel(self.model)  # Устанавливаем модель данных для таблицы

        self.selected_columns = []  # Список выбранных столбцов

        self.table.selectionModel().selectionChanged.connect(self.selectionChanged)  # Обработчик изменения выбора в таблице

        self.setStyleToTable()  # Применяем стили к таблице

        self.showGraphFlag = False  # Флаг для отображения графика

        # Кнопки управления
        changeArrBtn = QtWidgets.QPushButton()
        changeArrBtn.setText("Изменить размер таблицы (массива)")
        changeArrBtn.clicked.connect(self.changeArr)

        generateArrBtn = QtWidgets.QPushButton()
        generateArrBtn.setText("Заполнить случайными числами")
        generateArrBtn.clicked.connect(self.generateArr)

        saveArrBtn = QtWidgets.QPushButton()
        saveArrBtn.setText("Сохранить таблицу (массив)")
        saveArrBtn.clicked.connect(self.saveArr)

        loadArrBtn = QtWidgets.QPushButton()
        loadArrBtn.setText("Загрузить таблицу (массив) из файла")
        loadArrBtn.clicked.connect(self.loadArr)

        showGraphBtn = QtWidgets.QPushButton()
        showGraphBtn.setText("Показать график зависимости между выбранными столбцами (2-ого от 1-ого)")
        showGraphBtn.clicked.connect(self.showGraph)

        showSciPyBtn = QtWidgets.QPushButton()
        showSciPyBtn.setText("SciPy")
        showSciPyBtn.clicked.connect(self.showSciPy)

        # Добавляем кнопки в layout2
        layout2.addWidget(changeArrBtn)
        layout2.addWidget(generateArrBtn)
        layout2.addWidget(saveArrBtn)
        layout2.addWidget(loadArrBtn)
        layout2.addWidget(showSciPyBtn)

        # Настройка графика
        self.graph = pg.PlotWidget(self)
        self.graph.setBackground("w")
        self.graph.showGrid(x=True, y=True)
        self.graph.setLabel("left", "Значение Y")
        self.graph.setLabel("bottom", "Значение X")

        # Добавляем таблицу, график и кнопки в основной layout
        layout1.addWidget(self.table)
        layout1.addWidget(self.graph)
        layout1.addLayout(layout2)
        layout1.addWidget(showGraphBtn)

        # Создаем контейнер для виджетов
        container = QtWidgets.QWidget(self)
        container.setLayout(layout1)

        self.setCentralWidget(container)  # Устанавливаем контейнер как центральный виджет

    # Функция для отображения графика синуса с использованием SciPy
    def showSciPy(self):
        self.graph.clear()  # Очищаем текущий график
        x = np.linspace(0, 2 * np.pi, 100)  # Генерируем данные для оси X
        y = np.sin(x)  # Вычисляем значения синуса для оси Y
        self.graph.plot(x, y, pen="b")  # Отображаем график синуса
        result = optimize.minimize(lambda x: np.sin(x), 0)  # Используем SciPy для минимизации функции синуса
        QMessageBox.information(self, "", f"Минимум синуса: {result.x}\nЗначение функции на минимуме: {result.fun}")  # Показываем результаты

    # Функция для сохранения массива в HDF5 файл
    def saveArr(self):
        options = QFileDialog.Options()  # Открываем диалог для сохранения файла
        file_path, _ = QFileDialog.getSaveFileName(self, "Сохранить в HDF5", "", "HDF5 Files (*.h5)", options=options)

        if not file_path:
            return  # Если путь не был выбран, выходим из функции

        try:
            with h5py.File(file_path, "w") as f:  # Открываем файл для записи
                data = self.model.getData()  # Получаем данные из модели
                dataset = f.create_dataset("my_dataset", data=data)  # Создаем набор данных в файле
                dataset.attrs["description"] = "Массив numpy"  # Добавляем описание
            QMessageBox.information(self, "Сохранение", "Файл успешно сохранён!")  # Информируем пользователя о сохранении
        except Exception as e:
            QMessageBox.critical(self, "Ошибка", f"Не удалось сохранить файл:\n{str(e)}")  # Обрабатываем ошибки сохранения

    # Функция для загрузки массива из HDF5 файла
    def loadArr(self):
        options = QFileDialog.Options()  # Открываем диалог для открытия файла
        file_path, _ = QFileDialog.getOpenFileName(self, "Открыть HDF5", "", "HDF5 Files (*.h5)", options=options)

        if not file_path:
            return  # Если путь не был выбран, выходим из функции

        try:
            with h5py.File(file_path, "r") as f:  # Открываем файл для чтения
                dataset = f["my_dataset"]  # Читаем набор данных
                self.model.changeData(dataset[:])  # Обновляем модель новыми данными
        except Exception as e:
            QMessageBox.critical(self, "Ошибка", f"Не удалось открыть файл:\n{str(e)}")  # Обрабатываем ошибки открытия файла

        self.setStyleToTable()  # Применяем стили к таблице
        self.graph.clear()  # Очищаем график
        self.showGraphFlag = False

    # Обработчик изменения выделения в таблице
    def selectionChanged(self):
        self.selected_columns = self.table.selectionModel().selectedColumns()  # Получаем выбранные столбцы
        print(self.selected_columns)  # Печатаем информацию о выбранных столбцах

    # Обновление графика, если изменились данные
    def updateGraph(self, col):
        if self.showGraphFlag:  # Если график должен отображаться
            if self.col1 == col or self.col2 == col:  # Если изменился один из выбранных столбцов
                data = self.model.getData()
                x = data[:, self.col1]  # Получаем данные для оси X
                print(x)
                y = data[:, self.col2]  # Получаем данные для оси Y
                print(y)
                self.graph.clear()  # Очищаем график
                self.graph.plot(x, y, pen="b")  # Строим новый график

    # Функция для отображения графика зависимости между двумя выбранными столбцами
    def showGraph(self):
        if len(self.selected_columns) == 2:  # Проверяем, что выбраны два столбца
            data = self.model.getData()
            x = data[:, self.selected_columns[0].column()]  # Получаем данные для оси X
            self.col1 = self.selected_columns[0].column()  # Сохраняем индекс первого столбца
            print(x)
            y = data[:, self.selected_columns[1].column()]  # Получаем данные для оси Y
            self.col2 = self.selected_columns[1].column()  # Сохраняем индекс второго столбца
            print(y)
            self.graph.clear()  # Очищаем график
            self.graph.plot(x, y, pen="b")  # Строим новый график
            self.showGraphFlag = True  # Устанавливаем флаг для отображения графика
            QMessageBox.information(self, "График зависимости", f"Показан график зависимости Столбца {self.col2+1} от Столбца {self.col1+1}")
        else:
            QMessageBox.warning(self, "Предупреждение", "Выберите 2 столбца!")  # Если не выбраны два столбца, показываем предупреждение

    # Функция для изменения размера таблицы (массива)
    def changeArr(self):
        rows, ok1 = QInputDialog.getInt(self, "Изменение размера", "Введите количество строк (от 1 до 20):", self.model.rowCount(), 1, 20)
        if ok1:
            cols, ok2 = QInputDialog.getInt(self, "Изменение размера", "Введите количество столбцов (минимум 4):", self.model.columnCount(), 4, 10)

        if (rows != self.model.rowCount() or cols != self.model.columnCount()) and ok1 and ok2:
            data = np.random.randint(1, 5, (rows, cols))  # Генерируем новые данные
            self.model.changeData(data)  # Обновляем модель
            self.graph.clear()  # Очищаем график

        self.setStyleToTable()  # Применяем стили к таблице

    # Функция для настройки стилей таблицы
    def setStyleToTable(self):
        self.table.horizontalHeader().setDefaultSectionSize(100)  # Устанавливаем размер секций заголовков
        self.table.setColumnWidth(self.model.columnCount() - 1, 180)  # Устанавливаем ширину последнего столбца
        self.table.setColumnWidth(self.model.columnCount() - 2, 150)  # Устанавливаем ширину предпоследнего столбца
        self.table.setItemDelegateForColumn(0, ComboBoxDelegate())  # Устанавливаем делегат для первого столбца

        font = QtGui.QFont("Arial", 14)  # Устанавливаем шрифт
        font.setBold(True)  # Делаем шрифт жирным

        self.table.setFont(font)  # Применяем шрифт к таблице

    # Функция для заполнения столбца случайными величинами
    def generateArr(self):
        generateCol, ok = QInputDialog.getInt(self, "Заполнение случайными величинами", "Выберите столбец для заполнения случайными величинами:", 1, 1, self.model.columnCount() - 2)
        if ok:
            self.model.generateArr(generateCol - 1)  # Заполняем выбранный столбец случайными значениями

# Функция для запуска приложения
def application():
    app = QApplication(sys.argv)  # Создаем приложение
    window = MyWindow()  # Создаем главное окно
    window.show()  # Отображаем окно
    sys.exit(app.exec_())  # Запускаем приложение

# Основной блок для запуска приложения
if __name__ == "__main__":
    application()



    