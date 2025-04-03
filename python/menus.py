from PySide6.QtGui import QAction

def create_menus(main_window):
    menubar = main_window.menuBar()

    file_menu = menubar.addMenu("File")
    file_menu.addAction(QAction("Open", main_window))
    file_menu.addAction(QAction("Save", main_window))
    file_menu.addSeparator()
    file_menu.addAction(QAction("Exit", main_window, triggered=main_window.close))

    view_menu = menubar.addMenu("View")
    view_menu.addAction(QAction("Toggle Plots", main_window))
    view_menu.addAction(QAction("Toggle Machine Status", main_window))

    help_menu = menubar.addMenu("Help")
    help_menu.addAction(QAction("About", main_window))
