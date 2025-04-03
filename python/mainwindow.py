from PySide6.QtWidgets import QMainWindow, QStatusBar
from menus import create_menus
from docks import create_docks
from centralwidget import CentralWidget

class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("TAPIR - RTL Configuration Tool")
        self.resize(1200, 800)

        create_menus(self)
        self.setStatusBar(QStatusBar())
        self.setCentralWidget(CentralWidget(self))
        create_docks(self)
