from PyQt5 import QtCore, QtGui
from PyQt5.QtCore import Qt, pyqtSignal

import numpy as np


class TableModel(QtCore.QAbstractTableModel):

    dataChangedSignal = pyqtSignal(int)

    def __init__(self):
        super().__init__()
        self._data = np.random.randint(1, 5, (6, 4))
        self._data = self._data.astype(float)
        self.updateData()

    def rowCount(self, parent=None):
        return self._data.shape[0]

    def columnCount(self, parent=None):
        return self._data.shape[1]

    def data(self, index, role=Qt.DisplayRole):
        if role == Qt.DisplayRole or role == Qt.EditRole:
            return float(self._data[index.row(), index.column()])

        if role == Qt.BackgroundRole and index.column() == self._data.shape[1]-1:
            if float(self._data[index.row(), index.column()]) >= 0:
                return QtGui.QBrush(QtGui.QColor(144, 238, 144))
            else:
                return QtGui.QBrush(QtGui.QColor(255, 182, 193))
        return None

    def setData(self, index, value, role=Qt.EditRole):
        if role == Qt.EditRole:
            if index.column() != self._data.shape[1]-1 or index.column() != self._data.shape[1]-2:
                self._data[index.row(), index.column()] = float(value)
                self.updateData()
                self.dataChangedSignal.emit(index.column())
                return True
        return False

    def flags(self, index):
            return Qt.ItemIsSelectable | Qt.ItemIsEnabled | Qt.ItemIsEditable

    def headerData(self, section, orientation, role = Qt.DisplayRole):
        if role == Qt.DisplayRole:
            if orientation == Qt.Horizontal:
                if section == self._data.shape[1]-1:
                    return "Накопленные значения"
                elif section == self._data.shape[1]-2:
                    return "Пересчет значений"
                return "Столбец " + str(section+1)


            elif orientation == Qt.Vertical:
                return str(section+1)
        return None

    def updateData(self):
        rows = self._data.shape[0]
        cols = self._data.shape[1]

        product = np.ones(rows)

        for col in range(cols-2):
            product *= self._data[:, col]

        self._data[:, cols-2] = product
        self.dataChangedSignal.emit(cols-2)
        self._data[:, cols-1] = np.cumsum(self._data[:, cols-2])
        self.dataChangedSignal.emit(cols-1)


    def changeData(self, data):

        self.beginResetModel()

        self._data = data

        self._data = self._data.astype(float)

        self.updateData()

        self.endResetModel()
        

    def generateArr(self, col):

        if col == 0:
            self._data[:, col] = np.random.randint(1, 5, size = self._data.shape[0])

        else:
            self._data[:, col] = np.round(np.random.random(size = self._data.shape[0]), 2)

        self._data = self._data.astype(float)

        self.updateData()

        self.dataChangedSignal.emit(col)

    def getData(self):
        return self._data








