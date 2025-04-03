#! /usr/bin/env python3

import sys
import signal
from PySide6.QtWidgets import QApplication
from mainwindow import MainWindow

def main():
    app = QApplication(sys.argv)

    # Allow Ctrl+C to terminate the app
    signal.signal(signal.SIGINT, signal.SIG_DFL)

    window = MainWindow()
    window.show()
    sys.exit(app.exec())

if __name__ == "__main__":
    main()
