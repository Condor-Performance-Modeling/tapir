from PySide6.QtWidgets import QDockWidget, QTextEdit
from PySide6.QtCore import Qt

def create_docks(main_window):
    docks = {
        "Goal Seeking": Qt.LeftDockWidgetArea,
        "Project": Qt.LeftDockWidgetArea,
        "Plots": Qt.RightDockWidgetArea,
        "Machine Status": Qt.RightDockWidgetArea,
    }

    for name, area in docks.items():
        dock = QDockWidget(name, main_window)
        dock.setAllowedAreas(area)
        dock.setWidget(QTextEdit(f"{name} content (placeholder)"))
        main_window.addDockWidget(area, dock)
