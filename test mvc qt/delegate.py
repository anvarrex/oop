from PyQt5.QtCore import Qt
from PyQt5 import QtWidgets

# Создаем класс ComboBoxDelegate, который будет служить для создания кастомного редактора
# для ячеек в таблице QTableView (вместо стандартного текстового редактора будет использован ComboBox).
class ComboBoxDelegate(QtWidgets.QStyledItemDelegate):

    # Метод createEditor вызывается для создания редактора для ячейки таблицы.
    def createEditor(self, parent, option, index):
        # Создаем экземпляр QComboBox, который будет использоваться для редактирования данных ячейки.
        editor = QtWidgets.QComboBox(parent)
        
        # Добавляем элементы в выпадающий список. Здесь — это числа от 1 до 5.
        editor.addItems(['1', '2', '3', '4', '5'])
        
        # Возвращаем созданный редактор (QComboBox), который будет использован в ячейке.
        return editor

    # Метод setEditorData используется для инициализации данных в редакторе.
    def setEditorData(self, editor, index):
        # Получаем данные из модели, которые соответствуют выбранной ячейке.
        # Используется Qt.EditRole для получения текущих данных, отображаемых в ячейке.
        value = int(index.model().data(index, Qt.EditRole))
        
        # Устанавливаем выбранное значение в редактор (QComboBox) как текущий текст.
        # Сначала преобразуем значение в строку, чтобы оно могло быть выбрано в ComboBox.
        editor.setCurrentText(str(value))  

    # Метод setModelData используется для обновления данных модели после редактирования в редакторе.
    def setModelData(self, editor, model, index):
        # Получаем выбранный пользователем элемент из QComboBox.
        # Устанавливаем его значение в модель для соответствующей ячейки.
        # Используем Qt.EditRole, чтобы обновить данные в модели.
        model.setData(index, editor.currentText(), Qt.EditRole)


