from PyQt5 import QtCore, QtGui
from PyQt5.QtCore import Qt, pyqtSignal
import numpy as np

# Создаем модель для таблицы, которая будет использоваться в QTableView
class TableModel(QtCore.QAbstractTableModel):

    # Определяем сигнал dataChangedSignal, который будет использоваться для оповещения о том,
    # что данные в таблице изменились. Он принимает индекс столбца, который был изменен.
    dataChangedSignal = pyqtSignal(int)

    # Инициализатор модели, где создается начальный массив данных (6 строк и 4 столбца).
    def __init__(self):
        super().__init__()
        self._data = np.random.randint(1, 5, (6, 4))  # Генерация случайных данных в диапазоне от 1 до 5
        self._data = self._data.astype(float)  # Преобразование данных в тип float
        self.updateData()  # Обновление данных (пересчитываем накопленные значения)

    # Метод возвращает количество строк в модели
    def rowCount(self, parent=None):
        return self._data.shape[0]  # Количество строк в массиве данных

    # Метод возвращает количество столбцов в модели
    def columnCount(self, parent=None):
        return self._data.shape[1]  # Количество столбцов в массиве данных

    # Метод для получения данных из модели. Используется для отображения данных в ячейке
    def data(self, index, role=Qt.DisplayRole):
        # Если запрос на отображение или редактирование данных, возвращаем значение из массива данных
        if role == Qt.DisplayRole or role == Qt.EditRole:
            return float(self._data[index.row(), index.column()])

        # Если запрашивается фон ячейки, то для последнего столбца меняем цвет в зависимости от значения
        if role == Qt.BackgroundRole and index.column() == self._data.shape[1]-1:
            if float(self._data[index.row(), index.column()]) >= 0:
                return QtGui.QBrush(QtGui.QColor(144, 238, 144))  # Зеленый цвет для положительных значений
            else:
                return QtGui.QBrush(QtGui.QColor(255, 182, 193))  # Розовый цвет для отрицательных значений

        return None

    # Метод для изменения данных в ячейке модели. Вызывается при редактировании данных
    def setData(self, index, value, role=Qt.EditRole):
        if role == Qt.EditRole:
            # Если редактируется не последний и не предпоследний столбец
            if index.column() != self._data.shape[1]-1 or index.column() != self._data.shape[1]-2:
                self._data[index.row(), index.column()] = float(value)  # Обновляем значение в массиве данных
                self.updateData()  # Обновляем вычисляемые данные (пересчитываем итоговые значения)
                self.dataChangedSignal.emit(index.column())  # Сигнализируем, что данные в столбце изменились
                return True
        return False

    # Метод для определения флагов ячеек (выделяем их как доступные для редактирования, выделения и т.д.)
    def flags(self, index):
        return Qt.ItemIsSelectable | Qt.ItemIsEnabled | Qt.ItemIsEditable

    # Метод для отображения заголовков столбцов и строк
    def headerData(self, section, orientation, role=Qt.DisplayRole):
        if role == Qt.DisplayRole:
            if orientation == Qt.Horizontal:  # Заголовки столбцов
                if section == self._data.shape[1]-1:
                    return "Накопленные значения"  # Последний столбец
                elif section == self._data.shape[1]-2:
                    return "Пересчет значений"  # Предпоследний столбец
                return "Столбец " + str(section + 1)  # Заголовок для остальных столбцов

            elif orientation == Qt.Vertical:  # Заголовки строк
                return str(section + 1)  # Нумерация строк
        return None

    # Метод для обновления данных (пересчитываем накопленные значения и пересчитанные данные)
    def updateData(self):
        rows = self._data.shape[0]  # Количество строк
        cols = self._data.shape[1]  # Количество столбцов

        # Массив для хранения произведений по строкам
        product = np.ones(rows)

        # Перемножаем все значения в строках для каждого столбца до предпоследнего
        for col in range(cols-2):
            product *= self._data[:, col]

        # Обновляем предпоследний столбец с результатами перемножения
        self._data[:, cols-2] = product
        self.dataChangedSignal.emit(cols-2)  # Сигнализируем, что данные в столбце пересчитаны

        # Обновляем последний столбец с накопленными значениями
        self._data[:, cols-1] = np.cumsum(self._data[:, cols-2])
        self.dataChangedSignal.emit(cols-1)  # Сигнализируем, что данные в последнем столбце обновлены

    # Метод для изменения данных в модели (принимает новый массив данных)
    def changeData(self, data):
        self.beginResetModel()  # Оповещаем модель о том, что данные будут изменены

        self._data = data  # Обновляем данные модели
        self._data = self._data.astype(float)  # Преобразуем в float
        self.updateData()  # Обновляем вычисляемые данные (например, пересчитываем итоговые значения)

        self.endResetModel()  # Завершаем изменение данных

    # Метод для генерации случайных данных в одном столбце
    def generateArr(self, col):
        if col == 0:  # Для первого столбца генерируем случайные целые числа от 1 до 5
            self._data[:, col] = np.random.randint(1, 5, size=self._data.shape[0])
        else:  # Для остальных столбцов генерируем случайные вещественные числа от -5 до 5
            self._data[:, col] = np.round(np.random.uniform(-5, 5, size=self._data.shape[0]), 2)

        self._data = self._data.astype(float)  # Преобразуем в float
        self.updateData()  # Обновляем вычисляемые данные

        # Сигнализируем, что данные в конкретном столбце были изменены
        self.dataChangedSignal.emit(col)

    # Метод для получения данных модели
    def getData(self):
        return self._data  # Возвращаем массив данных





        