# -*- coding: utf-8 -*-

from PySide2.QtWidgets import ( # pylint: disable=no-name-in-module disable=import-error
    QMainWindow,
    QVBoxLayout,
    QProgressBar,
    QWidget,
    QPushButton,
)

from PySide2.QtCore import ( # pylint: disable=no-name-in-module disable=import-error
    Slot,
)

from .ui import getMayaWindow

class ProgressDialog( QMainWindow ):

    def __init__(self, parent = None):
        if parent is None:
            parent = getMayaWindow()
        super(ProgressDialog, self).__init__( parent )
        self.__canceled = False
        self.__setupUi()
        self.__connectEvents()

    def __setupUi(self):
        self.setWindowTitle( "Ramses is Working..." )
        self.setMinimumWidth( 300 )
        self.setMaximumHeight(50)

        mainLayout = QVBoxLayout()
        mainLayout.setContentsMargins(6,6,6,6)
        mainLayout.setSpacing(3)

        self.progressBar = QProgressBar()
        self.progressBar.setTextVisible(False)

        mainLayout.addWidget(self.progressBar)

        mainWidget = QWidget()
        mainWidget.setLayout( mainLayout )
        self.setCentralWidget( mainWidget )
        
    def __connectEvents(self):
        pass

    def setText(self, text):
        self.progressBar.setFormat(text + '... ( %p% )')
        self.progressBar.setTextVisible(True)

    def setValue(self, value):
        self.progressBar.setValue(value)

    def increment(self):
        self.progressBar.setValue( self.progressBar.value() + 1 )

    def setMaximum(self, max):
        self.progressBar.setMaximum(max)

if __name__ == '__main__':
    progressDialog = ProgressDialog()
    progressDialog.setText("test")
    progressDialog.setMaximum(10)
    progressDialog.increment()
    progressDialog.show()
    
