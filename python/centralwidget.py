from PySide6.QtWidgets import QWidget, QTabWidget, QVBoxLayout, QTableView
from PySide6.QtGui import QStandardItemModel, QStandardItem

class CentralWidget(QWidget):
    def __init__(self, parent=None):
        super().__init__(parent)
        layout = QVBoxLayout(self)
        tabs = QTabWidget()

        for i in range(2):
            table = QTableView()
            model = QStandardItemModel(5, 3)
            for row in range(5):
                for col in range(3):
                    item = QStandardItem(f"Item {i}-{row},{col}")
                    model.setItem(row, col, item)
            table.setModel(model)
            tabs.addTab(table, f"Table {i+1}")

        layout.addWidget(tabs)
        self.setLayout(layout)
