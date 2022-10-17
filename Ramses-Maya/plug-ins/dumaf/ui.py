# -*- coding: utf-8 -*-
"""User interface tools"""

import sys
import os
from PySide2.QtWidgets import ( # pylint: disable=import-error disable=no-name-in-module
    QApplication,
    QDialog,
    QVBoxLayout,
    QLabel,
    QTextEdit,
    QPushButton
)
from PySide2.QtGui import ( # pylint: disable=no-name-in-module,import-error
    QIcon,
    QDesktopServices,
)
from PySide2.QtCore import ( # pylint: disable=no-name-in-module
    Slot,
    QUrl
)
from dumaf.utils import getModulePath
from dumaf.rendering import get_ortho_cameras, get_persp_cameras, get_renderable_cameras
from dumaf.paths import baseName

ICON_PATH = getModulePath() + "/icons/"

def getMayaWindow():
    """Returns the Maya QMainWindow"""
    app = QApplication.instance() #get the qApp instance if it exists.
    if not app:
        app = QApplication(sys.argv)

    try:
        mayaWin = next(w for w in app.topLevelWidgets() if w.objectName()=='MayaWindow')
        return mayaWin
    except: # pylint: disable=bare-except
        return None

def update_cam_combobox(combobox):
    """Sets the list of cameras in the scene in a QComboBox"""
    combobox.clear()

    renderableCameras = get_renderable_cameras()
    perspCameras =  get_persp_cameras()
    orthoCameras = get_ortho_cameras()

    numRenderCam = len(renderableCameras)
    if numRenderCam > 0:
        for camera in renderableCameras:
            cameraName = baseName(camera)
            combobox.addItem( cameraName, camera)
        combobox.insertSeparator( numRenderCam )

    numPerspCam = len( perspCameras )
    if numPerspCam > 0:
        for camera in perspCameras:
            cameraName = baseName(camera)
            combobox.addItem( cameraName, camera)
        combobox.insertSeparator( numRenderCam+numPerspCam+1 )

    for camera in orthoCameras:
        cameraName = baseName(camera)
        combobox.addItem( cameraName, camera)

def icon(name):
    """Gets QIcon for an icon from its name (without extension)"""
    if os.path.isfile( ICON_PATH + name + ".png" ):
        return QIcon( ICON_PATH + name + ".png" )
    else:
        return QIcon( ICON_PATH + name + ".svg" )

class UpdateDialog( QDialog ):
    """The dialog to show details about an update"""

    def __init__(self, updateInfo, toolName, toolVersion, parent=None):
        if not parent:
            parent=getMayaWindow()
        super(UpdateDialog, self).__init__(parent)
        self.__setupUi(updateInfo, toolName, toolVersion)

    def __setupUi(self, updateInfo, toolName, toolVersion):
        self.setModal(True)

        mainLayout = QVBoxLayout()
        mainLayout.setSpacing(3)
        self.setLayout(mainLayout)

        if updateInfo.get("update", False):
            self.setWindowTitle("New " + toolName + " available!" )

            latestVersionLabel = QLabel("version: " + updateInfo.get("version") )
            mainLayout.addWidget(latestVersionLabel)

            descriptionEdit = QTextEdit(updateInfo.get("description"))
            descriptionEdit.setReadOnly(True)
            mainLayout.addWidget(descriptionEdit)

            currentVersionLabel = QLabel("Current version: " + toolVersion )
            currentVersionLabel.setEnabled(False)
            mainLayout.addWidget(currentVersionLabel)

            self.__downloadURL = updateInfo.get("downloadURL", "")
            if self.__downloadURL != "":
                self.__ui_downloadButton = QPushButton("Download")
                self.__ui_downloadButton.setIcon(icon("download"))
                mainLayout.addWidget(self.__ui_downloadButton)
                self.__ui_downloadButton.clicked.connect(self.download)

            self.__changelogURL = updateInfo.get("changelogURL", "")
            if self.__changelogURL != "":
                self.__ui_changelogButton = QPushButton("Changelog")
                self.__ui_changelogButton.setIcon(icon("changelog"))
                mainLayout.addWidget(self.__ui_changelogButton)
                self.__ui_changelogButton.clicked.connect(self.changelog)

            self.__donateURL = updateInfo.get("donateURL", "")
            if self.__donateURL != "":
                self.__ui_donateButton = QPushButton("I ♥ " + toolName)
                self.__ui_donateButton.setIcon(icon("donate"))
                mainLayout.addWidget(self.__ui_donateButton)
                self.__ui_donateButton.clicked.connect(self.donate)

            self.__ui_okButton = QPushButton("Close")
            self.__ui_okButton.setIcon(icon("close"))
            mainLayout.addWidget(self.__ui_okButton)
            self.__ui_okButton.clicked.connect(self.reject)
        elif updateInfo.get("accepted", False):
            self.setWindowTitle( "Update" )

            versionLabel = QLabel("I'm already up-to-date!" )
            mainLayout.addWidget(versionLabel)

            self.__ui_okButton = QPushButton("Close")
            self.__ui_okButton.setIcon(icon("close"))
            mainLayout.addWidget(self.__ui_okButton)
            self.__ui_okButton.clicked.connect(self.reject)
        elif updateInfo.get("success", False):
            self.setWindowTitle( "Server error" )
            label = QLabel("Sorry, the server could not get update information." )
            mainLayout.addWidget(label)

            descriptionEdit = QTextEdit(updateInfo.get("description", ""))
            descriptionEdit.setReadOnly(True)
            mainLayout.addWidget(descriptionEdit)

            self.__ui_okButton = QPushButton("Close")
            self.__ui_okButton.setIcon(icon("close"))
            mainLayout.addWidget(self.__ui_okButton)
            self.__ui_okButton.clicked.connect(self.reject)
        else:
            self.setWindowTitle( "Server error" )
            label = QLabel("Sorry, there was a server error." )
            mainLayout.addWidget(label)

            self.__ui_okButton = QPushButton("Close")
            self.__ui_okButton.setIcon(icon("close"))
            mainLayout.addWidget(self.__ui_okButton)
            self.__ui_okButton.clicked.connect(self.reject)

    @Slot()
    def download(self):
        """Opens the download URL"""
        QDesktopServices.openUrl ( QUrl( self.__downloadURL ) )
        self.close()

    @Slot()
    def changelog(self):
        """Opens the changelog URL"""
        QDesktopServices.openUrl ( QUrl( self.__changelogURL ) )
        self.close()

    @Slot()
    def donate(self):
        """Opens the donate URL"""
        QDesktopServices.openUrl ( QUrl( self.__donateURL ) )
        self.close()
