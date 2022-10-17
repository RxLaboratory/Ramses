# -*- coding: utf-8 -*-

from PySide2.QtWidgets import ( # pylint: disable=no-name-in-module
    QDialog,
    QHBoxLayout,
    QVBoxLayout,
    QFormLayout,
    QWidget,
    QComboBox,
    QLineEdit,
    QPushButton,
    QLabel,
    QFileDialog,
)
from PySide2.QtCore import ( # pylint: disable=no-name-in-module
    Slot,
)
import maya.cmds as cmds # pylint: disable=import-error
from ramses_maya.ui_dialog import Dialog
from ramses_maya.ui_object_combobox import RamObjectBox


import ramses as ram
ramses = ram.Ramses.instance()

class PublishTemplateDialog( Dialog ):

    def __init__(self, parent=None):
        super(PublishTemplateDialog,self).__init__(parent)
        self.__setupUi()
        self.__loadProjects()
        self.__connectEvents()

    def __setupUi(self):
        self.setWindowTitle( "Save as Template" )

        self.setMinimumWidth(400)

        mainLayout = QVBoxLayout()
        mainLayout.setSpacing(3)
        self.main_layout.addLayout(mainLayout)

        topLayout = QFormLayout()
        topLayout.setFieldGrowthPolicy( QFormLayout.AllNonFixedFieldsGrow )
        topLayout.setSpacing(3)

        self.projectBox = RamObjectBox()
        topLayout.addRow( "Project:", self.projectBox )

        self.stepBox = RamObjectBox()
        topLayout.addRow( "Step:", self.stepBox )

        self.nameEdit = QLineEdit()
        self.nameEdit.setPlaceholderText("Template")
        topLayout.addRow("Name:", self.nameEdit)

        self.extensionBox = QComboBox()
        self.extensionBox.addItem("Maya Binary (.mb)", "mb")
        self.extensionBox.addItem("Maya ASCII (.ma)", "ma")
        topLayout.addRow("File Type:", self.extensionBox)

        locationWidget = QWidget()
        locationLayout = QHBoxLayout()
        locationLayout.setSpacing(3)
        locationLayout.setContentsMargins(0,0,0,0)
        locationWidget.setLayout(locationLayout)

        self.locationEdit = QLineEdit()
        self.locationEdit.setEnabled(False)
        self.locationEdit.setPlaceholderText("Location...")
        locationLayout.addWidget( self.locationEdit )

        self.browseButton = QPushButton("Browse...")
        self.browseButton.setVisible( False )
        locationLayout.addWidget( self.browseButton )

        topLayout.addRow("Location:",locationWidget)

        self.fileNameLabel = QLabel()
        topLayout.addRow("Filename:", self.fileNameLabel)

        mainLayout.addLayout( topLayout )

        buttonsLayout = QHBoxLayout()
        buttonsLayout.setSpacing(2)

        self._publishButton = QPushButton("Save Template")
        buttonsLayout.addWidget( self._publishButton )
        self._cancelButton = QPushButton("Cancel")
        buttonsLayout.addWidget( self._cancelButton )

        mainLayout.addLayout( buttonsLayout )

    def __connectEvents(self):
        self.browseButton.clicked.connect( self.browse )
        self.projectBox.currentTextChanged.connect( self.__loadSteps )
        self.stepBox.currentTextChanged.connect( self.__buildPath )
        self._publishButton.clicked.connect( self.accept )
        self._cancelButton.clicked.connect( self.reject )
        self.extensionBox.currentIndexChanged.connect( self.__buildFileName )
        self.nameEdit.textEdited.connect( self.__buildPath )

    def __loadProjects(self):
        # Load projects
        projects = ramses.projects()
        self.projectBox.clear()
        if len(projects) == 0:
            self.setOffline()
            self.__loadSteps( )
            return
        for project in ramses.projects():
            self.projectBox.addItem(str(project), project)
        self.__loadSteps( )

    @Slot()
    def __loadSteps(self):
        project = self.projectBox.getObject()
        if project is not None:
            self.stepBox.clear()
            for step in project.steps():
                self.stepBox.addItem(str(step), step)
        self.__buildPath()

    @Slot()
    def __buildPath(self):
        self._publishButton.setEnabled(False)
        self.locationEdit.setText("")
        self.fileNameLabel.setText("")
        project = self.projectBox.getObject()
        if project is None:
            self.locationEdit.setPlaceholderText("Sorry, Invalid project...")
            return
        pShortName = project.shortName()
        step = self.stepBox.getObject()
        if step is None:
            self.locationEdit.setPlaceholderText("Sorry, Invalid step...")
            return
        sShortName = step.shortName()
        self.locationEdit.setPlaceholderText("Location")
        # Build the subfolder name
        nm = ram.RamFileInfo()
        nm.project = pShortName
        nm.step = sShortName
        nm.ramType = ram.ItemType.GENERAL
        nm.shortName = self.nameEdit.text()
        if nm.shortName == "":
            nm.shortName = "Template"

        self.locationEdit.setText( step.templatesFolderPath() + "/" + nm.fileName() )
        self._publishButton.setEnabled(True)
        # build file name
        self.__buildFileName()

    @Slot()
    def __buildFileName(self):
        nm = ram.RamFileInfo()

        p = project = self.projectBox.getObject()
        s = self.stepBox.getObject()
        if not p:
            return ""
        if not s:
            return ""
        nm.project = p.shortName()
        nm.step = s.shortName()
        nm.extension = self.extensionBox.currentData()
        nm.ramType = ram.ItemType.GENERAL
        nm.shortName = self.nameEdit.text()
        if nm.shortName == "":
            nm.shortName = "Template"
        
        self.fileNameLabel.setText( nm.fileName() )

    @Slot()
    def browse(self):
        path = QFileDialog.getExistingDirectory(
            self,
            "Select Templates Directory",
            ramses.folderPath(),
            QFileDialog.ShowDirsOnly
            )
        self.locationEdit.setText("")
        # Try to extract info from the path
        if path != "":
            nm = ram.RamFileInfo()
            nm.setFilePath( path )
            project = nm.project
            step = nm.step
            if step == "" or project == "":
                cmds.confirmDialog(
                title="Invalid Ramses project or step",
                message="Sorry, this folder does not belong to a valid step in this project, I can't export the template there.",
                button=["OK"],
                icon="warning"
                )
            if project != "":
                self.projectBox.setEditText( project )
            if step != "":
                self.stepBox.setEditText( step )
            self.__buildPath() 

    def setProject(self, project):
        for i in range(self.projectBox.count()):
            if self.projectBox.itemData(i) == project.shortName():
                self.projectBox.setCurrentIndex(i)
                return
        n = project.name()
        if n == "":
            n = project.shortName()
        self.projectBox.addItem(n, project.shortName())
        self.projectBox.setCurrentIndex( self.projectBox.count() - 1)

    def setStep(self, step):
        for i in range( self.stepBox.count() ):
            if self.stepBox.itemData(i) == step.shortName():
                self.stepBox.setCurrentIndex(i)
                return
        n = step.name()
        if n == "":
            n = step.shortName()
        self.stepBox.addItem(n, step.shortName())
        self.stepBox.setCurrentIndex( self.stepBox.count() - 1)
        
    def setOffline(self, offline=True):
        self.locationEdit.setEnabled(offline)
        self.browseButton.setVisible(offline)

    def getFolder(self):
        return self.locationEdit.text()

    def getFile(self):
        return self.fileNameLabel.text()

if __name__ == '__main__':
    publishTemplateDialog = PublishTemplateDialog()
    ok = publishTemplateDialog.exec_()
    if ok == 1:
        print(publishTemplateDialog.getFile())
        print(publishTemplateDialog.getFolder())
