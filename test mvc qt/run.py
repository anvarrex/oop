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
    def __init__(self):
        super(MyWindow, self).__init__()

        self.setWindowTitle("Тестовое задание")
        self.setMinimumSize(1100, 700)
        self.initUI();
    

    def initUI(self):

        layout1 = QtWidgets.QVBoxLayout()
        layout2 = QtWidgets.QHBoxLayout()

        self.model = TableModel();

        self.model.dataChangedSignal.connect(self.updateGraph)

        self.table = QTableView()

        self.table.setModel(self.model)

        self.selected_columns = []

        self.table.selectionModel().selectionChanged.connect(self.selectionChanged)

        self.setStyleToTable()

        self.showGraphFlag = False

       


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

        
        layout2.addWidget(changeArrBtn)
        layout2.addWidget(generateArrBtn)
        layout2.addWidget(saveArrBtn)
        layout2.addWidget(loadArrBtn)
        layout2.addWidget(showSciPyBtn)




        self.graph = pg.PlotWidget(self)
        self.graph.setBackground("w")
        self.graph.showGrid(x=True, y=True)
        self.graph.setLabel("left", "Значение Y")
        self.graph.setLabel("bottom", "Значение X")

        

        layout1.addWidget(self.table)
        layout1.addWidget(self.graph)
        layout1.addLayout(layout2)
        layout1.addWidget(showGraphBtn)


        container = QtWidgets.QWidget(self)
        container.setLayout(layout1)


        self.setCentralWidget(container)



    def showSciPy(self):
        self.graph.clear()
        x = np.linspace(0, 2 * np.pi, 100)
        y = np.sin(x)
        self.graph.plot(x, y, pen = "b")
        result = optimize.minimize(lambda x: np.sin(x), 0)
        QMessageBox.information(self, "", f"Минимум синуса: {result.x}\nЗначение функции на минимуме: {result.fun}")


    def saveArr(self):
        options = QFileDialog.Options()
        file_path, _ = QFileDialog.getSaveFileName(self, "Сохранить в HDF5", "", "HDF5 Files (*.h5)", options=options)

        if not file_path:
            return

        try:
            with h5py.File(file_path, "w") as f:
                data = self.model.getData()
                dataset = f.create_dataset("my_dataset", data=data)
                dataset.attrs["description"] = "Массив numpy"
            
            QMessageBox.information(self, "Сохранение", "Файл успешно сохранён!")

        except Exception as e:
            QMessageBox.critical(self, "Ошибка", f"Не удалось сохранить файл:\n{str(e)}")



    def loadArr(self):
        options = QFileDialog.Options()
        file_path, _ = QFileDialog.getOpenFileName(self, "Сохранить в HDF5", "", "HDF5 Files (*.h5)", options=options)

        if not file_path:
            return


        try:
            with h5py.File(file_path, "r") as f:
                dataset = f["my_dataset"]
                self.model.changeData(dataset[:])

        except Exception as e:
            QMessageBox.critical(self, "Ошибка", f"Не удалось открыть файл:\n{str(e)}")

        self.setStyleToTable()
        self.graph.clear()



    def selectionChanged(self):
        self.selected_columns = self.table.selectionModel().selectedColumns() 
        print(self.selected_columns)


    def updateGraph(self, col):
        if self.showGraphFlag:
            if self.col1 == col or self.col2 == col:
                data = self.model.getData()
                x = data[:, self.col1]
                print(x)
                y = data[:, self.col2]
                print(y)
                self.graph.clear()
                self.graph.plot(x, y, pen = "b")


    def showGraph(self):
        if len(self.selected_columns) == 2:
            data = self.model.getData()
            x = data[:, self.selected_columns[0].column()]
            self.col1 = self.selected_columns[0].column()
            print(x)
            y = data[:, self.selected_columns[1].column()]
            self.col2 = self.selected_columns[1].column()
            print(y)
            self.graph.clear()
            self.graph.plot(x, y, pen = "b")
            self.showGraphFlag = True
            QMessageBox.information(self, "График зависимости", f"Показан график зависимости Столбца {self.col2+1} от Столбца {self.col1+1}")
        else:
            QMessageBox.warning(self, "Предупреждение", "Выберите 2 столбца!")


    def changeArr(self):
        rows, ok1 = QInputDialog.getInt(self, "Изменение размера", "Введите количество строк (от 1 до 20):", self.model.rowCount(), 1, 20)
        if ok1:
            cols, ok2 = QInputDialog.getInt(self, "Изменение размера", "Введите количество столбцов (минимум 4):", self.model.columnCount(), 4, 10)

        if (rows!=self.model.rowCount() or cols!=self.model.columnCount()) and ok1 and ok2:
            data = np.random.randint(1, 5, (rows, cols))
            self.model.changeData(data)
            self.graph.clear()

        self.setStyleToTable()
        


    def setStyleToTable(self):
        self.table.horizontalHeader().setDefaultSectionSize(100)
        self.table.setColumnWidth(self.model.columnCount()-1, 180)
        self.table.setColumnWidth(self.model.columnCount()-2, 150)
        self.table.setItemDelegateForColumn(0, ComboBoxDelegate())


        font = QtGui.QFont("Arial", 14)
        font.setBold(True)

        self.table.setFont(font)

    def generateArr(self):
        generateCol, ok = QInputDialog.getInt(self, "Заполнение случайными величинами", "Выберите столбец для заполнения случайными величинами:", 1, 1, self.model.columnCount()-2)
        if ok:
            self.model.generateArr(generateCol-1)


    
def application():
    app = QApplication(sys.argv)
    window = MyWindow()

    window.show()
    sys.exit(app.exec_())


if __name__ == "__main__":
    application()
