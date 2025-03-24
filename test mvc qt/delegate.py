from PyQt5.QtCore import Qt
from PyQt5 import QtWidgets

class ComboBoxDelegate(QtWidgets.QStyledItemDelegate):

    def createEditor(self, parent, option, index):
        editor = QtWidgets.QComboBox(parent)
        editor.addItems(['1', '2', '3', '4', '5'])
        return editor

    def setEditorData(self, editor, index):
        value = int(index.model().data(index, Qt.EditRole))
        editor.setCurrentText(str(value))  

    def setModelData(self, editor, model, index):
        model.setData(index, editor.currentText(), Qt.EditRole)