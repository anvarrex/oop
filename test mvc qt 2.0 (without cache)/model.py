from PyQt5 import QtCore, QtGui
from PyQt5.QtCore import Qt, pyqtSignal
import h5py
import numpy as np

class HDF5TableModel(QtCore.QAbstractTableModel):
    # Сигнал, который отправляется при изменении данных
    dataChangedSignal = pyqtSignal(int)

    def __init__(self, h5file_path, dataset_name):
        super().__init__()
        # Открываем файл HDF5 в режиме чтения/записи
        self.h5file = h5py.File(h5file_path, 'a')  
        self.dataset = self.h5file[dataset_name]  # Создаем ссылку на данные в датасете (изменения будут происходить там напрямую)
        self.updateData()  # Обновляем данные (пересчитываем столбцы)

    def rowCount(self, parent=None):
        # Возвращаем количество строк в наборе данных
        return self.dataset.shape[0]
    
    def columnCount(self, parent=None):
        # Возвращаем количество столбцов в наборе данных
        return self.dataset.shape[1]
    
    def data(self, index, role=Qt.DisplayRole):
        # Функция для отображения данных в ячейке таблицы в зависимости от роли
        if role == Qt.DisplayRole or role == Qt.EditRole:
            return float(self.dataset[index.row(), index.column()])  # Возвращаем значение ячейки как число

        if role == Qt.BackgroundRole and index.column() == self.columnCount()-1:
            # Устанавливаем цвет фона для последнего столбца в зависимости от значения
            value = float(self.dataset[index.row(), index.column()])
            return QtGui.QBrush(QtGui.QColor(144, 238, 144)) if value >= 0 else QtGui.QBrush(QtGui.QColor(255, 182, 193))

        return None
    
    def setData(self, index, value, role=Qt.EditRole):
        # Функция для изменения данных в ячейке таблицы
        if role == Qt.EditRole and index.column() < self.columnCount()-2:
            self.dataset[index.row(), index.column()] = float(value)  # Обновляем данные в наборе данных
            self.updateData()  # Пересчитываем связанные столбцы
            self.dataChangedSignal.emit(index.column())  # Отправляем сигнал о изменении данных
            self.h5file.flush()  # Принудительно записываем изменения в файл
            return True
        return False
    
    def updateData(self):
        # Функция для пересчета значений в вычисляемых столбцах
        rows = self.rowCount()
        cols = self.columnCount()
        
        # Вычисляем произведение для предпоследнего столбца (столбец с индексом cols-2)
        product = np.ones(rows)
        for col in range(cols-2):
            product *= self.dataset[:, col]  # Перемножаем все значения в строках

        self.dataset[:, cols-2] = product  # Обновляем данные в предпоследнем столбце
        
        # Вычисляем накопленную сумму для последнего столбца (столбец с индексом cols-1)
        self.dataset[:, cols-1] = np.cumsum(self.dataset[:, cols-2])  # Накопленная сумма
        
        self.dataChangedSignal.emit(cols-2)  # Сигнал об изменении данных в предпоследнем столбце
        self.dataChangedSignal.emit(cols-1)  # Сигнал об изменении данных в последнем столбце

        self.h5file.flush()  # Принудительно записываем изменения в файл
    
    def flags(self, index):
        # Функция для задания флагов для каждого элемента таблицы
        return Qt.ItemIsSelectable | Qt.ItemIsEnabled | Qt.ItemIsEditable  # Указываем, что элемент можно выбирать и редактировать
    
    def headerData(self, section, orientation, role=Qt.DisplayRole):
        # Функция для отображения данных в заголовках таблицы
        if role == Qt.DisplayRole:
            if orientation == Qt.Horizontal:
                if section == self.columnCount()-1:
                    return "Накопленные значения"  # Заголовок для последнего столбца
                elif section == self.columnCount()-2:
                    return "Пересчет значений"  # Заголовок для предпоследнего столбца
                return f"Столбец {section+1}"  # Заголовок для остальных столбцов
            else:
                return str(section+1)  # Нумерация строк
        return None
    
    def generateArr(self, col):
        # Функция для заполнения столбца случайными значениями
        if col == 0:
            # Заполняем первый столбец случайными целыми числами от 1 до 5
            self.dataset[:, col] = np.random.randint(1, 5, size=self.rowCount())
        else:
            # Заполняем остальные столбцы случайными числами с плавающей точкой в диапазоне [-5, 5]
            self.dataset[:, col] = np.round(np.random.uniform(-5, 5, size=self.rowCount()), 2)
        
        self.updateData()  # Пересчитываем данные после генерации
        self.dataChangedSignal.emit(col)  # Сигнал о изменении данных в столбце
        self.h5file.flush()  # Принудительно записываем изменения в файл

    def changeData(self, data):
        # Функция для замены данных в наборе данных
        self.beginResetModel()  # Оповещаем модель, что данные изменятся

        self.dataset = data.astype(float)  # Обновляем данные в модели

        del self.h5file["my_dataset"]  # Удаляем старый набор данных из файла

        # Создаем новый набор данных в файле
        self.h5file.create_dataset("my_dataset", data=self.dataset)

        self.dataset = self.h5file["my_dataset"]  # Создаем новую ссылку на измененный датасет

        self.updateData()  # Пересчитываем вычисляемые данные

        self.endResetModel()  # Завершаем обновление модели
        self.h5file.flush()  # Принудительно записываем изменения в файл

    def getData(self):
        # Возвращаем текущие данные
        return self.dataset
    
    def close(self):
        # Функция для закрытия файла HDF5
        self.h5file.close()


        