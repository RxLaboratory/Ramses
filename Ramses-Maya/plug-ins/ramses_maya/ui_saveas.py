# -*- coding: utf-8 -*-
"""Save As/Create scene Dialog"""

from PySide2.QtWidgets import ( # pylint: disable=no-name-in-module
    QDialog,
    QHBoxLayout,
    QRadioButton,
    QVBoxLayout,
    QFormLayout,
    QWidget,
    QComboBox,
    QLineEdit,
    QPushButton,
    QLabel,
)
from PySide2.QtCore import ( # pylint: disable=no-name-in-module
    Slot,
)

import ramses as ram
from ramses_maya.ui_object_combobox import RamObjectBox
from ramses_maya.ui_dialog import Dialog

RAMSES = ram.Ramses.instance()

class SaveAsDialog( Dialog ):
    """Save As/Create scene Dialog"""

    def __init__(self, parent=None):
        super(SaveAsDialog, self).__init__(parent)

        self.__setup_ui()
        self._loadProjects()
        self._connectEvents()

    def __setup_ui(self):
        self.setWindowTitle( "Save Scene As..." )

        self.setMinimumWidth(400)

        main_layout = QVBoxLayout()
        main_layout.setSpacing(3)
        self.main_layout.addLayout(main_layout)

        top_layout = QFormLayout()
        top_layout.setFieldGrowthPolicy( QFormLayout.AllNonFixedFieldsGrow )
        top_layout.setSpacing(3)

        self.project_box = RamObjectBox()
        top_layout.addRow( "Project:", self.project_box )

        typeWidget = QWidget()
        typeLayout = QVBoxLayout()
        typeLayout.setContentsMargins(0,0,0,0)
        typeLayout.setSpacing(3)
        self.assetButton = QRadioButton("Asset")
        self.assetButton.setChecked(True)
        typeLayout.addWidget(self.assetButton)
        self.shotButton = QRadioButton("Shot")
        typeLayout.addWidget(self.shotButton)
        self.otherButton = QRadioButton("Other")
        typeLayout.addWidget(self.otherButton)
        typeWidget.setLayout(typeLayout)
        top_layout.addRow("Type:", typeWidget)

        self.stepBox = RamObjectBox()
        top_layout.addRow( "Step:", self.stepBox )

        self.groupBox = RamObjectBox()
        self.groupLabel = QLabel("Asset Group:")
        top_layout.addRow( self.groupLabel, self.groupBox )

        self.itemBox = RamObjectBox()
        self.itemLabel = QLabel("Item:")
        top_layout.addRow( self.itemLabel, self.itemBox )

        self.resourceEdit = QLineEdit()
        top_layout.addRow( "Resource:", self.resourceEdit)

        self.extensionBox = QComboBox()
        self.extensionBox.addItem("Maya Binary (.mb)", "mb")
        self.extensionBox.addItem("Maya ASCII (.ma)", "ma")
        top_layout.addRow("File Type:", self.extensionBox)

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

        top_layout.addRow("Location:",locationWidget)

        self.fileNameLabel = QLabel()
        top_layout.addRow("Filename:", self.fileNameLabel)

        main_layout.addLayout(top_layout)

        buttonsLayout = QHBoxLayout()
        buttonsLayout.setSpacing(2)
        self._saveButton = QPushButton("Save")
        buttonsLayout.addWidget( self._saveButton )
        self._cancelButton = QPushButton("Cancel")
        buttonsLayout.addWidget( self._cancelButton )
        main_layout.addLayout( buttonsLayout )

    def _connectEvents(self):
        self._saveButton.clicked.connect( self.accept )
        self._cancelButton.clicked.connect( self.reject )
        self.project_box.currentTextChanged.connect( self._loadSteps )
        self.stepBox.currentIndexChanged.connect( self._loadGroups )
        self.assetButton.clicked.connect( self._typeChanged )
        self.shotButton.clicked.connect( self._typeChanged )
        self.otherButton.clicked.connect( self._typeChanged )
        self.groupBox.currentIndexChanged.connect( self._loadItems )
        self.itemBox.currentTextChanged.connect( self._buildPath )
        self.itemBox.currentIndexChanged.connect( self._buildPath )
        self.itemBox.editTextChanged.connect( self._buildPath )
        self.resourceEdit.textChanged.connect( self._buildPath )
        self.extensionBox.currentIndexChanged.connect( self._buildPath )

    def _loadProjects(self):
        # Load projects
        projects = RAMSES.projects()
        self.project_box.clear()
        if len(projects) == 0:
            self.setOffline()
            self._loadSteps( )
            return
        for project in RAMSES.projects():
            self.project_box.addItem(str(project), project)
        # No selection, to make things faster and load steps & items only once needed
        self.project_box.setCurrentIndex(-1)
        self._loadSteps( )

    @Slot()
    def _loadSteps(self):
        self.stepBox.clear()
        project = self.getProject()
        if not project:
            return

        steps = []
        if self.assetButton.isChecked():
            steps = project.steps( ram.StepType.ASSET_PRODUCTION )
        elif self.shotButton.isChecked():
            steps = project.steps( ram.StepType.SHOT_PRODUCTION )
        else:
            steps = project.steps( )
        for step in steps:
            self.stepBox.addItem(str(step), step)
        # No selection, to make things faster and load steps & items only once needed
        self.stepBox.setCurrentIndex(-1)

        self._loadGroups()

    def _buildPath(self):
        self.locationEdit.setText('')
        self.fileNameLabel.setText('')
        self._saveButton.setEnabled(False)

        project = self.getProject()
        if not project:
            self.locationEdit.setPlaceholderText('Sorry, invalid project...')
            return

        step = self.getStep()
        if not step:
            self.locationEdit.setPlaceholderText('Sorry, invalid step...')
            return
        
        if self.assetButton.isChecked():

            # Get the group
            assetGroup = self.groupBox.currentText()
            if assetGroup == '':
                self.locationEdit.setText('Sorry, invalid asset group...')
                return

            # Let's get the step/asset location
            asset = self.getItem()
            if not asset:
                self.locationEdit.setPlaceholderText("Sorry, invalid asset...")
                return               

            assetsPath = project.assetsPath( assetGroup )

            nm = ram.RamFileInfo()
            nm.project = project.shortName()
            nm.ramType = ram.ItemType.ASSET
            nm.shortName = asset.shortName()
            assetFolderName = nm.fileName()

            nm.step = step.shortName()
            assetStepFolderName = nm.fileName()

            # The folder
            assetFolder = ram.RamFileManager.buildPath((
                assetsPath,
                assetFolderName,
                assetStepFolderName
            ))

            self.locationEdit.setText(assetFolder)

            # The filename
            nm.extension = self.extensionBox.currentData()
            nm.resource = self.resourceEdit.text()
            assetFileName = nm.fileName()

            self.fileNameLabel.setText(assetFileName)
                
        elif self.shotButton.isChecked():

            shot = self.getItem()
            if not shot:
                self.locationEdit.setPlaceholderText("Sorry, invalid shot...")
                return

            shotsPath = project.shotsPath()

            nm = ram.RamFileInfo()
            nm.project = project.shortName()
            nm.ramType = ram.ItemType.SHOT
            nm.shortName = shot.shortName()
            shotFolderName = nm.fileName()

            nm.step = step.shortName()
            shotStepFolderName = nm.fileName()

            shotFolder = ram.RamFileManager.buildPath((
                shotsPath,
                shotFolderName,
                shotStepFolderName
            ))

            self.locationEdit.setText(shotFolder)

            # The filename
            nm.extension = self.extensionBox.currentData()
            nm.resource = self.resourceEdit.text()
            shotFileName = nm.fileName()

            self.fileNameLabel.setText(shotFileName)
        
        else:

            itemShortName = self.itemBox.currentText()
            item = self.getItem()
            if item: itemShortName = item.shortName()

            stepPath = step.folderPath()

            self.locationEdit.setText(stepPath)

            # The filename
            nm = ram.RamFileInfo()
            nm.project = project.shortName()
            nm.ramType = ram.ItemType.SHOT
            nm.step = step.shortName()
            nm.shortName = itemShortName
            nm.extension = self.extensionBox.currentData()
            nm.resource = self.resourceEdit.text()
            itemFileName = nm.fileName()

            self.fileNameLabel.setText( itemFileName )

        self._saveButton.setEnabled(True)

    @Slot()
    def _typeChanged(self):
        if self.assetButton.isChecked():
            self.groupBox.show()
            self.groupLabel.show()
            self.groupLabel.setText("Asset Group:")
        elif self.shotButton.isChecked():
            self.groupBox.show()
            self.groupLabel.show()
            self.groupLabel.setText("Sequence:")
        else:
            self.groupBox.hide()
            self.groupLabel.hide()

        self._loadSteps()

    @Slot()
    def _loadGroups(self):
        self.itemBox.clear()
        self.groupBox.clear()

        project = self.getProject()
        if not project: return
        
        step = self.getStep()
        if not step: return

        if self.assetButton.isChecked():
            # Load asset groups
            for ag in project.assetGroups():
                self.groupBox.addItem(ag.name(), ag)
        elif self.shotButton.isChecked():
            # Load sequences
            for seq in project.sequences():
                self.groupBox.addItem(seq.name(), seq)

        # No selection, to make things faster and load steps & items only once needed
        self.groupBox.setCurrentIndex(-1)
        self._loadItems()

        self._buildPath()

    @Slot()
    def _loadItems(self):
        self.itemBox.clear()

        project = self.getProject()
        if not project:
            return

        if self.assetButton.isChecked():
            ag = self.groupBox.currentData()
            for asset in project.assets( ag ):
                self.itemBox.addItem(str(asset), asset)
        elif self.shotButton.isChecked():
            seq = self.groupBox.currentData()
            for shot in project.shots( sequence=seq ):
                self.itemBox.addItem(str(shot), shot)
        # No selection, to make things faster and load steps & items only once needed
        self.itemBox.setCurrentIndex(-1)

    def setOffline(self): # TODO
        pass

    def setProject(self, project):
        for i in range(self.project_box.count()):
            if self.project_box.itemData(i) == project:
                self.project_box.setCurrentIndex(i)
                return

        self.project_box.addItem(str(project), project)
        self.project_box.setCurrentIndex( self.project_box.count() - 1)

    def setStep(self, step):
        for i in range( self.stepBox.count() ):
            if self.stepBox.itemData(i) == step:
                self.stepBox.setCurrentIndex(i)
                return

        self.stepBox.addItem(str(step), step)
        self.stepBox.setCurrentIndex( self.stepBox.count() - 1)

    def setItem(self, item):
        if item.itemType() == ram.ItemType.ASSET:
            self.assetButton.setChecked(True)
        elif item.itemType() == ram.ItemType.SHOT:
            self.shotButton.setChecked(True)
        else:
            self.otherButton.setChecked(True)

        self._typeChanged()

        for i in range( self.itemBox.count() ):
            if self.itemBox.itemData(i) == item:
                self.itemBox.setCurrentIndex(i)
                return

        self.itemBox.addItem(str(item), item)
        self.itemBox.setCurrentIndex( self.itemBox.count() - 1)

    def setItemShortName(self, itemShortName):
        for i in range( self.itemBox.count() ):
            item = self.itemBox.itemData(i)
            if not item: continue
            if item.shortName() == itemShortName:
                self.itemBox.setCurrentIndex(i)
                return

        self.itemBox.addItem(itemShortName)
        self.itemBox.setCurrentIndex( self.itemBox.count() - 1)

    def getFilePath(self):
        path = self.locationEdit.text()
        if path == '':
            return ''
        fileName = self.fileNameLabel.text()
        return ram.RamFileManager.buildPath((
            path,
            fileName
        ))

    def getExtension(self):
        return self.extensionBox.currentData()

    def getProject(self):
        """Returns the selected project"""
        p = self.project_box.currentData()
        return p

    def getStep(self):
        s = self.stepBox.currentData()
        if not s:
            project = self.getProject()
            if not project: return None
            sShortName = self.stepBox.currentText()
            s = project.step( sShortName )
        return s

    def getItem(self):
        i = self.itemBox.currentData()
        return i

if __name__ == '__main__':
    dialog = SaveAsDialog()
    ok = dialog.exec_()
    if ok == 1:
        print(dialog.getFilePath())
