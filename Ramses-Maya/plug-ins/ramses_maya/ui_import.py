# -*- coding: utf-8 -*-
"""UI for opening/importing/replacing items"""

import os
from PySide2.QtWidgets import ( # pylint: disable=no-name-in-module,import-error
    QDialog,
    QHBoxLayout,
    QVBoxLayout,
    QFormLayout,
    QComboBox,
    QListWidget,
    QListWidgetItem,
    QLabel,
    QPushButton,
    QWidget,
    QLineEdit,
    QAbstractItemView,
    QCheckBox,
    QTextEdit,
    QStackedLayout,
    QRadioButton
)
from PySide2.QtCore import ( # pylint: disable=no-name-in-module,import-error
    Slot,
    Qt
)
import yaml
import ramses as ram
from ramses_maya.ui_object_combobox import RamObjectBox
from ramses_maya.ui_dialog import Dialog
from ramses_maya.utils_options import (
    load_bool_preset,
    get_option
)
from ramses_maya.utils import IMPORT_PRESETS_PATH

RAMSES = ram.Ramses.instance()

class ImportDialog( Dialog ):
    """The main open/import/replace dialog"""
    # <== CONSTRUCTOR ==>

    def __init__(self, parent=None):
        super(ImportDialog,self).__init__(parent)

        self.__setup_ui()
        self.__load_projects()
        self.__connect_events()

    # <== PRIVATE METHODS ==>

    def __setup_ui(self):
        """Creates the dialog UI"""

        checkableButtonCSS = """
            QPushButton {
                background-color: rgba(0,0,0,0);
                border:none;
                padding: 5px;
                color: #eeeeee;
            }
            QPushButton:hover {
                background-color: #707070;
            }
            QPushButton:checked {
                background-color: #2b2b2b;
            }"""

        self.setWindowTitle( "Open / Import Item" )

        self.setMinimumWidth(500)

        mainLayout = QVBoxLayout()
        mainLayout.setSpacing(3)
        self.main_layout.addLayout(mainLayout)

        midLayout = QHBoxLayout()
        midLayout.setSpacing(3)

        topLayout = QFormLayout()
        topLayout.setFieldGrowthPolicy( QFormLayout.AllNonFixedFieldsGrow )
        topLayout.setSpacing(3)

        topLayout.addRow(QLabel("Info:"))

        self.projectBox = RamObjectBox()
        topLayout.addRow( "Project:", self.projectBox )

        self.typeWidget = QWidget()
        typeLayout = QVBoxLayout()
        typeLayout.setContentsMargins(0,0,0,0)
        typeLayout.setSpacing(3)
        self.assetButton = QRadioButton()
        self.assetButton.setText("Asset")
        typeLayout.addWidget(self.assetButton)
        self.shotButton = QRadioButton()
        self.shotButton.setText("Shot")
        typeLayout.addWidget(self.shotButton)
        self.templateButton = QRadioButton()
        self.templateButton.setText("Template")
        typeLayout.addWidget(self.templateButton)
        self.typeWidget.setLayout(typeLayout)
        self.typeWidget.setEnabled(False)
        topLayout.addRow( "Type:", self.typeWidget )

        self.actionWidget = QWidget()
        actionLayout = QVBoxLayout()
        actionLayout.setContentsMargins(0,0,0,0)
        actionLayout.setSpacing(3)
        self.openButton = QRadioButton()
        self.openButton.setText("Open item")
        self.openButton.setChecked(True)
        actionLayout.addWidget(self.openButton)
        self.importButton = QRadioButton()
        self.importButton.setText("Import item")
        actionLayout.addWidget(self.importButton)
        self.replaceButton = QRadioButton()
        self.replaceButton.setText("Replace selected items")
        actionLayout.addWidget(self.replaceButton)
        self.actionWidget.setLayout(actionLayout)
        topLayout.addRow( "Action:", self.actionWidget )

        self.optionsWidget = QWidget()
        optionsLayout = QVBoxLayout(self.optionsWidget)
        optionsLayout.setContentsMargins(0,0,0,0)
        optionsLayout.setSpacing(3)
        self.__show_import_options = QCheckBox("Edit import options")
        optionsLayout.addWidget(self.__show_import_options)
        topLayout.addRow( "Settings:", self.optionsWidget )

        midLayout.addLayout( topLayout )

        itemLayout = QVBoxLayout()
        itemLayout.setSpacing(3)
        self.itemLabel = QLabel("Item")
        self.itemLabel.hide()
        itemLayout.addWidget(self.itemLabel)
        self.groupBox = RamObjectBox()
        self.groupBox.hide()
        itemLayout.addWidget(self.groupBox)
        self.itemSearchField = QLineEdit()
        self.itemSearchField.setPlaceholderText('Search...')
        self.itemSearchField.setClearButtonEnabled(True)
        itemLayout.addWidget(self.itemSearchField)
        self.itemSearchField.hide()
        self.itemList = QListWidget()
        self.itemList.hide()
        itemLayout.addWidget(self.itemList)
        midLayout.addLayout(itemLayout)

        stepLayout = QVBoxLayout()
        stepLayout.setSpacing(3)
        stepLabel = QLabel("Step:")
        stepLayout.addWidget(stepLabel)
        self.stepList = QListWidget()
        stepLayout.addWidget(self.stepList)
        midLayout.addLayout(stepLayout)

        resourcesLayout = QVBoxLayout()
        resourcesLayout.setSpacing(3)
        self.resourcesLabel = QLabel("Resource:")
        resourcesLayout.addWidget(self.resourcesLabel)
        self.resourceList = QListWidget()
        resourcesLayout.addWidget(self.resourceList)
        midLayout.addLayout(resourcesLayout)

        versionsLayout = QVBoxLayout()
        versionsLayout.setSpacing(3)
        self.versionsLabel = QLabel("Version:")
        versionsLayout.addWidget(self.versionsLabel)
        self.publishVersionBox = QComboBox()
        versionsLayout.addWidget(self.publishVersionBox)
        self.publishVersionBox.setVisible(False)
        self.versionSearchField = QLineEdit()
        self.versionSearchField.setPlaceholderText('Search...')
        self.versionSearchField.setClearButtonEnabled(True)
        versionsLayout.addWidget(self.versionSearchField)
        self.versionList = QListWidget()
        versionsLayout.addWidget(self.versionList)
        midLayout.addLayout(versionsLayout)

        mainLayout.addLayout( midLayout )

        buttonsLayout = QHBoxLayout()
        buttonsLayout.setSpacing(2)
        self._openButton = QPushButton("Open")
        buttonsLayout.addWidget( self._openButton )
        self._importButton = QPushButton("Import")
        self._importButton.hide()
        buttonsLayout.addWidget( self._importButton )
        self._replaceButton = QPushButton("Replace")
        self._replaceButton.hide()
        buttonsLayout.addWidget( self._replaceButton )
        self._cancelButton = QPushButton("Cancel")
        buttonsLayout.addWidget( self._cancelButton )

        self._openButton.setEnabled(False)
        self._replaceButton.setEnabled(False)
        self._importButton.setEnabled(False)

        mainLayout.addLayout( buttonsLayout )

    def __connect_events(self):
        self.projectBox.currentIndexChanged.connect( self.__project_changed )

        self.assetButton.clicked.connect( self.__asset_button_clicked )
        self.shotButton.clicked.connect( self.__shot_button_clicked )
        self.templateButton.clicked.connect( self.__template_button_clicked )
        self.assetButton.clicked.connect( self.__type_changed )
        self.shotButton.clicked.connect( self.__type_changed )
        self.templateButton.clicked.connect( self.__type_changed )

        self.openButton.clicked.connect( self.__open_button_clicked )
        self.importButton.clicked.connect( self.__import_button_clicked )
        self.replaceButton.clicked.connect( self.__replace_button_clicked )
        self.openButton.clicked.connect( self.__action_changed )
        self.importButton.clicked.connect( self.__action_changed )
        self.replaceButton.clicked.connect( self.__action_changed )

        self._cancelButton.clicked.connect( self.reject )
        self._openButton.clicked.connect( self.accept )
        self._importButton.clicked.connect( self.__import )
        self._replaceButton.clicked.connect( self.__replace )
        self.itemList.currentRowChanged.connect( self.__update_resources )
        self.stepList.currentRowChanged.connect( self.__update_resources )
        self.resourceList.currentRowChanged.connect( self.__resource_changed )
        self.versionList.currentRowChanged.connect( self.__version_changed )
        self.groupBox.currentIndexChanged.connect( self.__update_items )
        self.itemSearchField.textChanged.connect( self.__search_item )
        self.versionSearchField.textChanged.connect( self.__search_version )
        self.publishVersionBox.currentIndexChanged.connect( self.__update_published_files )

    @Slot()
    def __asset_button_clicked(self):
        self.assetButton.setChecked(True)
        self.shotButton.setChecked(False)
        self.templateButton.setChecked(False)

    @Slot()
    def __shot_button_clicked(self):
        self.assetButton.setChecked(False)
        self.shotButton.setChecked(True)
        self.templateButton.setChecked(False)

    @Slot()
    def __template_button_clicked(self):
        self.assetButton.setChecked(False)
        self.shotButton.setChecked(False)
        self.templateButton.setChecked(True)

    @Slot()
    def __open_button_clicked(self):
        self.openButton.setChecked(True)
        self.importButton.setChecked(False)
        self.replaceButton.setChecked(False)
        self.__show_import_options.setEnabled(False)
        self.__show_import_options.setChecked(False)

    @Slot()
    def __import_button_clicked(self):
        self.openButton.setChecked(False)
        self.importButton.setChecked(True)
        self.replaceButton.setChecked(False)
        self.__show_import_options.setEnabled(True)

    @Slot()
    def __replace_button_clicked(self):
        self.openButton.setChecked(False)
        self.importButton.setChecked(False)
        self.replaceButton.setChecked(True)
        self.__show_import_options.setEnabled(True)

    def __load_projects(self):
        # Load projects
        projects = RAMSES.projects()
        self.projectBox.clear()

        if projects is None:
            self.assetButton.setChecked(False)
            self.shotButton.setChecked(False)
            self.templateButton.setChecked(False)
            return

        for project in RAMSES.projects():
            self.projectBox.addItem(str(project), project)

        self.projectBox.setCurrentIndex(-1)

    @Slot()
    def __project_changed(self, index):
        self.assetButton.setChecked(False)
        self.shotButton.setChecked(False)
        self.templateButton.setChecked(False)
        self.typeWidget.setEnabled(index != -1)
        if index == -1:
            return

    @Slot()
    def __type_changed( self ):
        shot = self.shotButton.isChecked()
        asset = self.assetButton.isChecked()
        template = self.templateButton.isChecked()

        # adjust UI
        if shot or asset:
            self.itemLabel.show()
            self.itemList.show()
            self.itemSearchField.show()
            self.groupBox.show()
            if asset:
                self.itemLabel.setText("Asset:")
            else:
                self.itemLabel.setText("Shot:")
        else:
            self.itemLabel.hide()
            self.itemList.hide()
            self.groupBox.hide()
            self.itemSearchField.hide()

        # reinit lists
        self.itemList.clear()
        self.stepList.clear()
        self.resourceList.clear()
        self.versionList.clear()
        self.groupBox.clear()

        if not shot and not asset and not template:
            return

        project = self.projectBox.currentData()
        if not project:
            return

        steps = ()

        # Load asset groups and asset steps
        if asset:
            groups = project.assetGroups()
            self.groupBox.blockSignals(True)
            self.groupBox.addItem("All", "")
            # Add groups
            for group in groups:
                self.groupBox.addItem(group.name(), group)
            self.groupBox.setCurrentIndex(-1)
            self.groupBox.blockSignals(False)
            steps = project.steps( ram.StepType.ASSET_PRODUCTION )
        # Load sequences, shots and shot steps
        elif shot:
            groups = project.sequences()
            self.groupBox.blockSignals(True)
            self.groupBox.addItem("All", "")
            # Add sequences
            for group in groups:
                self.groupBox.addItem(group.name(), group)
            self.groupBox.setCurrentIndex(-1)
            self.groupBox.blockSignals(False)
            steps = project.steps( ram.StepType.SHOT_PRODUCTION )
        # Load steps for templates
        elif template:
            steps = project.steps()

        # Populate steps
        for step in steps:
            n = str(step)
            item = QListWidgetItem( n )
            item.setData( Qt.UserRole, step )
            self.stepList.addItem( item )

    @Slot()
    def __update_items(self):

        project = self.projectBox.currentData()
        if not project:
            return

        items = ()
        if self.shotButton.isChecked():
            items = project.shots( sequence = self.groupBox.currentData() )
        elif self.assetButton.isChecked():
            items = project.assets( self.groupBox.currentData() )
        else: return

        self.itemList.clear()
        for item in items:
            n = str(item)
            listItem = QListWidgetItem( n )
            listItem.setData( Qt.UserRole, item )
            self.itemList.addItem( listItem )

    @Slot()
    def __search_item(self, text):
        text = text.lower()
        for i in range(0, self.itemList.count()):
            item = self.itemList.item(i)
            item.setHidden(
                text != '' and text not in item.text().lower()
                )

    @Slot()
    def __search_version(self, text):
        text = text.lower()
        for i in range(0, self.versionList.count()):
            item = self.versionList.item(i)
            item.setHidden(
                text != '' and text not in item.text().lower()
                )

    @Slot()
    def __action_changed(self):
        if self.openButton.isChecked():
            self._importButton.hide()
            self._replaceButton.hide()
            self._openButton.show()
            self.versionsLabel.setText("Version:")
            self.versionList.setSelectionMode(QAbstractItemView.SingleSelection)
            self.publishVersionBox.setVisible(False)
        else: # Import or replace
            if self.replaceButton.isChecked():
                self._replaceButton.show()
                self._importButton.hide()
                self.versionList.setSelectionMode(QAbstractItemView.SingleSelection)
            else:
                self._replaceButton.hide()
                self._importButton.show()
                self.versionList.setSelectionMode(QAbstractItemView.ExtendedSelection)
            self._openButton.hide()
            self.versionsLabel.setText("File:")
            self.publishVersionBox.setVisible(True)
        self.__update_resources()
        self.__resource_changed( self.resourceList.currentRow() )

    @Slot()
    def __update_resources(self):

        def listTemplateResources(step):
            folder = step.templatesFolderPath()
            if folder == '':
                return

            for f in os.listdir( folder ):
                # Template must be a folder
                fPath = ram.RamFileManager.buildPath((
                    folder,
                    f
                ))
                if not os.path.isdir(fPath):
                    continue

                nm = ram.RamFileInfo()
                if not nm.setFileName( f ):
                    continue

                if nm.project != step.projectShortName():
                    continue

                if nm.step != step.shortName():
                    continue

                for t in os.listdir( fPath ):
                    # Resource must be a file
                    resource = ram.RamFileManager.buildPath((
                        fPath,
                        t
                    ))
                    if not os.path.isfile(resource):
                        continue

                    if not nm.setFileName( t ):
                        continue

                    res = nm.resource

                    if nm.isRestoredVersion:
                        if res != '':
                            res = res + " | "
                        res = res + "v" + str(nm.restoredVersion) + " (restored)"

                    if res == "":
                        res = "Main (" + nm.extension + ")"

                    item = QListWidgetItem( nm.shortName + " | " + res )
                    item.setData( Qt.UserRole, resource )
                    item.setToolTip(t)
                    self.resourceList.addItem( item )

        self.resourceList.hide()
        self.resourcesLabel.hide()
        self._openButton.setEnabled(False)

        self.resourceList.clear()
        self.versionList.clear()

        stepItem = self.stepList.currentItem()
        if not stepItem:
            return
        step = stepItem.data(Qt.UserRole)

        currentItem = self.getItem()

        # If open, list resources in wip folder
        if self.openButton.isChecked():
            self.resourceList.show()
            self.resourcesLabel.show()
            # Shots and Assets
            if self.assetButton.isChecked() or self.shotButton.isChecked():
                if not currentItem:
                    return
                # List resources
                resources = currentItem.stepFilePaths( step )

                for resource in resources:
                    nm = ram.RamFileInfo()
                    nm.setFilePath(resource)
                    if nm.project == '':
                        continue

                    res = nm.resource

                    if nm.isRestoredVersion:
                        if res != '':
                            res = res + " | "
                        res = res + "v" + str(nm.restoredVersion) + " (restored)"

                    if res == "":
                        res = "Main (" + nm.extension + ")"
                        self._openButton.setEnabled(True)

                    item = QListWidgetItem( res )
                    item.setData( Qt.UserRole, resource )
                    self.resourceList.addItem( item )

            # Templates
            else:
                # List resources
                listTemplateResources(step)

        # If import asset or shot, list all subfolders
        elif self.assetButton.isChecked() or self.shotButton.isChecked():
            self.__list_published_versions()
        # If import template, list resources
        else:
            self.resourceList.show()
            self.resourcesLabel.show()
            listTemplateResources(step)

    def __list_published_versions(self):
        self.publishVersionBox.clear()
        folders = []
        currentItem = self.getItem()
        if not currentItem:
            return
        step = self.getStep()
        if not step:
            return
        folders = currentItem.publishedVersionFolderPaths( step )

        for f in reversed(folders): # list more recent first
            folderName = os.path.basename( f )
            folderName = folderName.split("_")
            title = ""

            # Test length to know what we've got
            if len(folderName) == 3: # resource, version, state
                title = folderName[0] + " | v" + folderName[1] + " | " + folderName[2]
            elif len(folderName) < 3: # version (state)
                if int(folderName[0]) != 0:
                    title = "v" + " | ".join(folderName)
            else:
                title = " | ".join(folderName)

            self.publishVersionBox.addItem(title, f)
            self.__update_published_files()

    @Slot()
    def __update_published_files(self):
        self.versionList.clear()
        # List available files
        folder = self.publishVersionBox.currentData()
        files = ram.RamFileManager.getRamsesFiles( folder )
        for f in files:
            nm = ram.RamFileInfo()
            fileName = os.path.basename(f)
            if not nm.setFileName(fileName):
                continue
            resource = nm.resource
            if resource == "":
                resource = "/ scene_backup /"
            title = resource + " (" + nm.extension + ")"
            item = QListWidgetItem( title )
            item.setData(Qt.UserRole, f)
            item.setToolTip(fileName)
            self.versionList.addItem(item)

    @Slot()
    def __resource_changed(self, row):

        # Import, list publish files for templates
        if self.importButton.isChecked():
            if self.assetButton.isChecked() or self.shotButton.isChecked():
                return
            self.__list_published_versions()
            return

        # Open, list versions
        if row < 0:
            self._openButton.setEnabled(False)
        else:
            self._openButton.setEnabled(True)

        self.versionList.clear()

        currentItem = self.getItem()
        if not currentItem:
            return

        # List versions
        versionFiles = ()
        versionFiles = currentItem.versionFilePaths( self.getResource(), self.getStep() )

        if len(versionFiles) == 0:
            return

        versionFiles.reverse()    

        # Add current
        item = QListWidgetItem("Current version")
        item.setData(Qt.UserRole, versionFiles[0])
        self.versionList.addItem(item)

        # Add other versions
        for v in versionFiles:
            fileName = os.path.basename( v )
            nm = ram.RamFileInfo()
            if not nm.setFileName( fileName ):
                continue
            comment = ram.RamMetaDataManager.getComment( v )
            itemText = nm.state + ' | ' + str( nm.version )
            if comment != "":
                itemText = itemText + ' | ' + comment
            item = QListWidgetItem( itemText )
            item.setData(Qt.UserRole, v)
            self.versionList.addItem(item)

    @Slot()
    def __version_changed(self, row):
        self._importButton.setEnabled(row >= 0)
        self._replaceButton.setEnabled(row >= 0)

    @Slot()
    def __import(self):
        self.done(2)

    @Slot()
    def __replace(self):
        self.done(3)

    # <== PUBLIC METHODS ==>

    def setMode(self, mode="open"):
        """Sets the mode of the window, either open, import or replace"""
        if mode == "import":
            self.__import_button_clicked()
        elif mode == "replace":
            self.__replace_button_clicked()
        else:
            self.__open_button_clicked()
        self.__action_changed()

    def setProject(self, project):
        """Sets the selected project"""
        if project is None:
            self.projectBox.setCurrentIndex(-1)
            return
        for i in range(self.projectBox.count()):
            testProject = self.projectBox.itemData(i)
            if not testProject:
                continue
            if testProject == project:
                self.projectBox.setCurrentIndex(i)
                return

        self.projectBox.addItem( str(project), project )
        self.projectBox.setCurrentIndex( self.projectBox.count() - 1)

    def setType( self, itemType):
        """Sets the type of item (asset, shot, template)"""
        if itemType == ram.ItemType.ASSET:
            self.assetButton.setChecked(True)
        elif itemType == ram.ItemType.SHOT:
            self.shotButton.setChecked(True)
        self.__type_changed()

    def setItem(self, itemShortName ):
        """Selects a specific item"""
        self.groupBox.setCurrentIndex(0)
        for i in range( 0, self.itemList.count()):
            listItem = self.itemList.item(i)
            if listItem.data(Qt.UserRole).shortName() == itemShortName:
                self.itemList.setCurrentItem(listItem)
                self.__update_resources()
                return

    def setStep(self, stepShortName):
        """Selects a specific step"""
        for i in range(0, self.stepList.count()):
            listItem = self.stepList.item(i)
            if listItem.data(Qt.UserRole).shortName() == stepShortName:
                self.stepList.setCurrentItem(listItem)
                self.__update_resources()
                return

    def getProject(self):
        """Returns the selected project"""
        return self.projectBox.currentData()

    def getItem(self):
        """Returns the Item (RamAsset, RamShot or RamItem if template) currently selected."""

        # if it's an asset or a shot, get from itemList
        if self.shotButton.isChecked() or self.assetButton.isChecked():
            item = self.itemList.currentItem()
            if not item:
                return None
            return item.data(Qt.UserRole)

        # if it's a template, gets a virtual item from the path of the selected resource (or version if import)
        item = self.resourceList.currentItem()
        if not item:
            return None
        return ram.RamItem.fromPath( item.data(Qt.UserRole), True )

    def getStep(self):
        """Returns the selected step"""
        item = self.stepList.currentItem()
        if not item:
            return None
        return item.data(Qt.UserRole)

    def getResource(self):
        """Returns the selected resource string if any"""
        item = self.resourceList.currentItem()
        if not item: return ""

        nm = ram.RamFileInfo()
        nm.setFilePath( item.data(Qt.UserRole) )
        return nm.resource

    def getFile(self):
        """Returns the selected file"""
        # return the selected version if it's not  the current
        rowForCurrent = -1
        if self.openButton.isChecked(): rowForCurrent = 0
        if self.versionList.currentRow() > rowForCurrent:
            return self.versionList.currentItem().data(Qt.UserRole)

        # no version selected, return the resource file
        # We can't import if no version file selected
        if self.importButton.isChecked(): return ""

        # return the selected resource if any
        item = self.resourceList.currentItem()
        if item:
            return item.data(Qt.UserRole)

        # If it's an asset or a shot which is selected, return the default (no resource)
        if self.assetButton.isChecked() or self.shotButton.isChecked():

            currentItem = self.getItem()
            if not currentItem: return ""
            step = self.getStep()
            if not step: return ""

            file = currentItem.stepFilePath("", "ma", step) # Try MA first
            if file != "": return file
            file = currentItem.stepFilePath("", "mb", step) # or MB
            return file

    def getFiles(self):
        """Returns the selected files"""
        if self.versionList.count() == 0:
            return ()

        # De-select the item 0, which is basically "no version"
        if self.openButton.isChecked():
            self.versionList.item(0).setSelected(False)
        items = self.versionList.selectedItems()
        if len(items) == 0:
            return self.getFile()

        files = []
        for item in items:
            files.append( item.data(Qt.UserRole) )
        return files

    def show_import_options(self):
        """Tells if the import options must be shown first"""
        return self.__show_import_options.isChecked()

class ImportSettingsDialog( Dialog ):
    """
    The Dialog to edit import settings
    """

    # <== CONSTRUCTOR ==>

    def __init__(self, parent=None): # pylint: disable=useless-super-delegation
        super(ImportSettingsDialog, self).__init__(parent)
        # <-- Setup -->
        self.__settings_widgets = []
        self.__setup_ui()
        self._dialog_add_preset_actions()
        self.__connect_events()
        self.set_preset_folder(IMPORT_PRESETS_PATH)
        self.__ui_preset_box.setCurrentIndex(-1)

    # <== PRIVATE METHODS ==>

    def __setup_ui(self):
        self.setWindowTitle("Import items")

        uber_layout = QHBoxLayout()
        uber_layout.setSpacing(3)
        self.main_layout.addLayout(uber_layout)

        self.__ui_files_box = QListWidget()
        self.__ui_files_box.setMaximumWidth( 150 )
        uber_layout.addWidget(self.__ui_files_box)

        self.__ui_stacked_layout = QStackedLayout()
        uber_layout.addLayout(self.__ui_stacked_layout)

        preset_widget = QWidget()
        preset_layout = QFormLayout(preset_widget)
        preset_layout.setFormAlignment(Qt.AlignHCenter | Qt.AlignTop)
        preset_layout.setSpacing(3)

        self.__ui_preset_box = QComboBox()
        preset_layout.addRow("Preset:", self.__ui_preset_box)
        
        self.__ui_stacked_layout.addWidget(preset_widget)

        # <-- BOTTOM BUTTONS -->

        buttons_layout = QHBoxLayout()
        buttons_layout.setSpacing(2)
        self.main_layout.addLayout(buttons_layout)

        self.__ui_import_button = QPushButton("Import")
        buttons_layout.addWidget( self.__ui_import_button )
        self.__ui_cancel_button = QPushButton("Cancel")
        buttons_layout.addWidget( self.__ui_cancel_button )

    def __connect_events(self):
        self.__ui_files_box.currentRowChanged.connect( self.__ui_stacked_layout.setCurrentIndex )
        self.__ui_preset_box.currentIndexChanged.connect( self.__ui_preset_box_current_changed )
        self.__ui_import_button.clicked.connect( self.accept )
        self.__ui_cancel_button.clicked.connect( self.reject )

    # <== PRIVATE SLOTS ==>

    @Slot(int)
    def __ui_preset_box_current_changed(self, index):
        if index < 0:
            return
        file_path = self.__ui_preset_box.itemData(index, Qt.UserRole)
        self.load_preset_file( file_path )

    # <== PRIVATE METHODS ==>

    def __add_file(self, file_dict):
        # Add entry
        format = file_dict["format"]
        if format == "*" or format == "":
            self.__ui_files_box.addItem("Format: Default (*)")
        else:
            self.__ui_files_box.addItem("Format: " + file_dict["format"])
        # Add widget
        widget = ImportSettingsWidget( self )
        self.__ui_stacked_layout.addWidget(widget)
        self.__settings_widgets.append(widget)
        widget.set_options(file_dict)

    # <== PUBLIC METHODS ==>

    def update_preset_files(self, preset_files):
        """Loads the preset files."""
        if len(preset_files) > 0:
            self.__ui_preset_box.clear()
            for preset_file in preset_files:
                name = os.path.basename(preset_file)
                name = os.path.splitext(name)[0]
                self.__ui_preset_box.addItem(name, preset_file)

    def get_options(self):
        """Gets the import options as a dict"""

        options = { }
        formats = []
        for w in self.__settings_widgets:
            formats.append( w.get_options() )

        options['formats'] = formats
        return options

    def set_options(self, options):
        """Loads options from a preset"""
        self.__ui_files_box.clear()
        if self.__settings_widgets:
            for w in self.__settings_widgets:
                self.__ui_stacked_layout.removeWidget(w)
        self.__settings_widgets = []

        # Add Presets
        self.__ui_files_box.addItem("Select: Preset")

        if not "formats" in options:
            return

        has_default = False
        for f in options['formats']:
            self.__add_file(f)
            if f['format'] == "*":
                has_default = True

        if not has_default:
            # Add default
            default =  {
                "format": "*"
            }
            self.__add_file(default)

class ImportSettingsWidget( QWidget ):
    """
    The Dialog to edit import settings for a single pipe
    """
    # <== CONSTRUCTOR ==>

    def __init__(self, parent=None): # pylint: disable=useless-super-delegation
        super(ImportSettingsWidget, self).__init__(parent)
        # <-- Setup -->
        self.__setup_ui()
        self.__connect_events()
        self.__format = "*"
        self.__update_preset()

    def __setup_ui(self):
        uber_layout = QHBoxLayout(self)

        # <-- GENERAL -->

        main_layout = QFormLayout()
        main_layout.setFormAlignment(Qt.AlignHCenter | Qt.AlignTop)
        uber_layout.addLayout(main_layout)

        self.__ui_reference_box = QCheckBox("As reference")
        main_layout.addRow("Import:", self.__ui_reference_box )

        self.__ui_lock_transform_box = QCheckBox("Lock transformations")
        self.__ui_lock_transform_box.setChecked(True)
        main_layout.addRow("", self.__ui_lock_transform_box)

        self.__ui_namespace_box = QCheckBox("Create namespace")
        self.__ui_namespace_box.setChecked(True)
        main_layout.addRow("", self.__ui_namespace_box)

        self.__ui_no_root_shape_box = QCheckBox("Don't add shape")
        self.__ui_no_root_shape_box.setChecked(False)
        main_layout.addRow("Root node:", self.__ui_no_root_shape_box)
    

        self.__ui_apply_shaders_box = QCheckBox("Apply to selected nodes")
        self.__ui_apply_shaders_box.setChecked(True)
        main_layout.addRow("Shaders:", self.__ui_apply_shaders_box)

        # <-- PRESET -->

        preset_widget = QWidget()
        preset_layout = QVBoxLayout(preset_widget)
        preset_layout.setSpacing(3)
        preset_layout.setContentsMargins(3,3,3,3)
        uber_layout.addWidget(preset_widget)

        preset_label = QLabel("You can use this preset in Ramses to set\nthe current settings as default settings for pipes.")
        preset_layout.addWidget(preset_label)
        self.__ui_preset_edit = QTextEdit()
        self.__ui_preset_edit.setReadOnly(True)
        preset_layout.addWidget(self.__ui_preset_edit)

    def __connect_events(self):
        self.__ui_reference_box.toggled.connect( self.__ui_reference_box_clicked )
        self.__ui_lock_transform_box.toggled.connect( self.__update_preset )
        self.__ui_apply_shaders_box.toggled.connect( self.__update_preset )
        self.__ui_no_root_shape_box.toggled.connect( self.__update_preset )
        self.__ui_namespace_box.toggled.connect( self.__update_preset )

    @Slot()
    def __update_preset(self):
        # Main options
        options = self.get_options()
        options_str = yaml.dump(options)
        self.__ui_preset_edit.setText(options_str)

    @Slot(bool)
    def __ui_reference_box_clicked(self, checked):
        self.__ui_lock_transform_box.setDisabled(checked)
        if checked:
            self.__ui_lock_transform_box.setChecked(False)
        self.__update_preset()

    def get_options(self):
        """Gets the import options as a dict"""

        options = {}
        if self.__format != "":
            options["format"] = self.__format
        
        as_ref = self.__ui_reference_box.isChecked()
        options["as_reference"] = as_ref
        if not as_ref:
            options["lock_transformations"] = self.__ui_lock_transform_box.isChecked()
        else:
            options["lock_transformations"] = False
        
        options["apply_shaders"] = self.__ui_apply_shaders_box.isChecked()

        options["no_root_shape"] = self.__ui_no_root_shape_box.isChecked()

        options["create_namespace"] = self.__ui_namespace_box.isChecked()
        
        return options

    def set_options(self, options):
        """Loads options from a preset"""

        load_bool_preset("lock_transformations", options, self.__ui_lock_transform_box, True)
        load_bool_preset("as_reference", options, self.__ui_reference_box, False)
        load_bool_preset("apply_shaders", options, self.__ui_apply_shaders_box, True)
        load_bool_preset("no_root_shape", options, self.__ui_no_root_shape_box, False)
        load_bool_preset("create_namespace", options, self.__ui_namespace_box, True)

        self.__format = get_option("format", options, "*")

        self.__update_preset()

if __name__ == '__main__':
    dialog = ImportDialog()
    ok = dialog.exec_()
    print(ok)
