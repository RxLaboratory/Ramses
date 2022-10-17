# -*- coding: utf-8 -*-

import os

from PySide2.QtWidgets import ( # pylint: disable=no-name-in-module disable=import-error
    QDialog,
    QHBoxLayout,
    QListWidget,
    QListWidgetItem,
    QVBoxLayout,
    QCheckBox,
    QPushButton,
    QAbstractItemView,
    QLabel,
)

from PySide2.QtCore import ( # pylint: disable=no-name-in-module disable=import-error
    Slot,
    Qt,
)
from maya import cmds # pylint: disable=import-error
from ramses_maya.utils_attributes import list_ramses_nodes, get_item, get_state, get_step, get_ramses_attr, RamsesAttribute
from ramses_maya.ui_dialog import Dialog
import ramses
import dumaf

RAMSES = ramses.Ramses.instance()

class UpdateDialog( Dialog ):
    """The Dialog to update items in the scene"""

    def __init__(self, parent = None):
        super(UpdateDialog, self).__init__(parent)
        self.__setupUi()
        self.__listItems()
        self.__connectEvents()

    def __setupUi(self):

        self.setWindowTitle("Update Items")

        mainLayout = QVBoxLayout()
        mainLayout.setSpacing(3)
        self.main_layout.addLayout(mainLayout)

        columnLayout = QHBoxLayout()
        columnLayout.setContentsMargins(0,0,0,0)
        columnLayout.setSpacing(6)

        currentLayout = QVBoxLayout()
        currentLayout.setContentsMargins(0,0,0,0)
        currentLayout.setSpacing(3)
        
        self.onlyNewButton = QCheckBox("Show only updated items.")
        self.onlyNewButton.setChecked(True)
        currentLayout.addWidget( self.onlyNewButton )

        self.onlySelectedButton = QCheckBox("Show only selected nodes.")
        self.onlySelectedButton.setChecked(False)
        currentLayout.addWidget( self.onlySelectedButton )

        self.itemList = QListWidget()
        self.itemList.setSelectionMode(QAbstractItemView.ExtendedSelection)
        currentLayout.addWidget(self.itemList)

        self.currentDetailsLabel = QLabel("")
        currentLayout.addWidget(self.currentDetailsLabel)
        
        columnLayout.addLayout(currentLayout)

        updateLayout = QVBoxLayout()
        updateLayout.setContentsMargins(0,0,0,0)
        updateLayout.setSpacing(3)

        updateLabel = QLabel("Published versions:")
        updateLayout.addWidget(updateLabel)

        self.updateList = QListWidget()
        self.updateList.setSelectionMode(QAbstractItemView.SingleSelection)
        updateLayout.addWidget(self.updateList)

        self.updateDetailsLabel = QLabel("")
        updateLayout.addWidget(self.updateDetailsLabel)
        
        columnLayout.addLayout(updateLayout)

        mainLayout.addLayout(columnLayout)

        buttonsLayout = QHBoxLayout()
        buttonsLayout.setSpacing(2)

        self._updateButton = QPushButton("Update All")
        self._updateButton.setEnabled(False)
        buttonsLayout.addWidget( self._updateButton )
        self._updateSelectedButton = QPushButton("Update Selected")
        self._updateSelectedButton.setEnabled(False)
        buttonsLayout.addWidget( self._updateSelectedButton )
        self._cancelButton = QPushButton("Cancel")
        buttonsLayout.addWidget( self._cancelButton )

        mainLayout.addLayout( buttonsLayout )

    def __connectEvents(self):
        self._updateButton.clicked.connect( self.accept )
        self._cancelButton.clicked.connect( self.reject )
        self.onlyNewButton.clicked.connect( self.showOnlyNew )
        self.onlySelectedButton.clicked.connect( self.showOnlySelected )
        self.itemList.itemSelectionChanged.connect( self.selectionChanged )
        self._updateSelectedButton.clicked.connect( self._updateSelected )

    Slot()
    def _updateSelected(self):
        self.done(2)

    Slot()
    def selectionChanged(self):
        """Updates displayed info according to the selection"""
        items = self.itemList.selectedItems()
        self._updateSelectedButton.setEnabled(len(items) > 0)
        self.updateList.clear()

        currentItem = self.itemList.currentItem()
        if currentItem is not None and len(items) == 1:

            # Details

            currentDetails = '\n'.join((
                "Current version: " + str( currentItem.data(Qt.UserRole + 4) ),
                "Current state: " + currentItem.data(Qt.UserRole + 3).name()
            ))
            self.currentDetailsLabel.setText( currentDetails )

            state = currentItem.data(Qt.UserRole + 8)
            stateName = "- Not Found -"
            if state:
                stateName = state.name()

            updateDetails = '\n'.join((
                "Latest version: " + str( currentItem.data(Qt.UserRole + 7) ),
                "Latest state: " + stateName
            ))
            self.updateDetailsLabel.setText( updateDetails )

            # List available versions
            ramItem = currentItem.data(Qt.UserRole + 1)
            ramStep = currentItem.data(Qt.UserRole + 2)
            sourceFile = currentItem.data(Qt.UserRole + 5)
            resource = currentItem.data(Qt.UserRole + 6)
            sourceFileName = os.path.basename(sourceFile)
            publishedFolders = ramItem.publishedVersionFolderPaths( ramStep, sourceFileName, resource )
            for f in reversed(publishedFolders):
                updateItem = QListWidgetItem(self.updateList)

                splitName = os.path.basename(f).split('_')
                if len(splitName) == 3:
                    splitName[1] = 'v' + str(int(splitName[1]))
                    splitName[2] = str( ramses.Ramses.instance().state(splitName[2]).name() )
                elif len(splitName) == 2:
                    splitName[0] = 'v' + str(int(splitName[0]))
                    splitName[1] = str( ramses.Ramses.instance().state(splitName[1]).name() )

                itemName = ' | '.join(splitName)
                updateItem.setText( itemName )
                updateFile = ramses.RamFileManager.buildPath((f, sourceFileName))
                updateItem.setData(Qt.UserRole, updateFile)

            self.updateList.setCurrentItem( self.updateList.item(0) )
                            
        else:
            self.currentDetailsLabel.setText("")
    Slot()
    def showOnlyNew(self, checked = True):
        self.__filterList()

    Slot()
    def showOnlySelected(self, checked = True):
        self.__filterList()

    def __filterList(self):
        selection = cmds.ls( long=True, selection=True )
        if selection is None: selection = []

        onlySelected = self.onlySelectedButton.isChecked()
        onlyUpdated = self.onlyNewButton.isChecked()
    
        for i in range(0, self.itemList.count()):
            listItem = self.itemList.item( i )
            if not onlySelected and not onlyUpdated:
                listItem.setHidden(False)
                continue

            node = listItem.data(Qt.UserRole)
            updated = listItem.text().startswith('New: ')
            selected = node in selection

            if onlyUpdated and not updated:
                listItem.setHidden(True)
                continue
            if onlySelected and not selected:
                listItem.setHidden(True)
                continue

            listItem.setHidden(False)

    def __listItems(self):
        """List the items found in the scene"""
        nodes = list_ramses_nodes('')
        if len(nodes) == 0:
            self._updateButton.setEnabled(False)
            self._updateSelectedButton.setEnabled(False)
            return
        self._updateButton.setEnabled(True)
        for node in nodes:
            nodeName = dumaf.paths.baseName(node)

            ramItem = get_item( node )
            ramStep = get_step( node )
            ramState = get_state( node )

            if ramItem is None or ramStep is None: continue

            # Check source info
            sourceFile = get_ramses_attr( node, RamsesAttribute.SOURCE_FILE )
            version = get_ramses_attr( node, RamsesAttribute.VERSION )
            resource = get_ramses_attr(node, RamsesAttribute.RESOURCE )

            listItem = QListWidgetItem( self.itemList )
            listItem.setData(Qt.UserRole, node)
            listItem.setData(Qt.UserRole + 1, ramItem )
            listItem.setData(Qt.UserRole + 2, ramStep )
            listItem.setData(Qt.UserRole + 3, ramState )
            listItem.setData(Qt.UserRole + 4, version )
            listItem.setData(Qt.UserRole + 5, sourceFile )
            listItem.setData(Qt.UserRole + 6, resource )
            listItem.setData(Qt.UserRole + 7, '-Not found-')
            listItem.setData(Qt.UserRole + 8, None )
            listItem.setToolTip( node )

            itemText = ramItem.name() + ' | ' + ramStep.name()
            if resource != '':
                itemText = itemText + ' | ' + resource
            itemText = itemText + ' (' + nodeName + ')'

            updated = False

            if sourceFile is not None:
                # Get the latest one and check its version and state
                fileName = os.path.basename( sourceFile )
                latestFolder = ramItem.latestPublishedVersionFolderPath( ramStep, fileName, resource )
                latestFile = ramses.RamFileManager.buildPath((latestFolder, fileName))
                if latestFile != sourceFile and latestFile != '':
                    updated = True
                updateVersion = ramses.RamMetaDataManager.getVersion(latestFile)
                updateState = ramses.RamMetaDataManager.getState( latestFile )
                updateState = ramses.Ramses.instance().state(updateState)
                listItem.setData(Qt.UserRole + 7, updateVersion)
                listItem.setData(Qt.UserRole + 8, updateState)
                listItem.setData(Qt.UserRole + 9, latestFile )
            if updated:
                itemText = 'New: ' + itemText 

            elif self.onlyNewButton.isChecked():
                listItem.setHidden(True)

            listItem.setText(itemText)

    def getAllNodes(self):
        nodes = []
        for i in range(0, self.itemList.count()):
            item = self.itemList.item(i)
            if item.text().startswith('New: '):
                nodes.append( ( item.data(Qt.UserRole), item.data(Qt.UserRole + 9)) )
        return nodes

    def getSelectedNodes(self):
        nodes = []

        updateItem = self.updateList.currentItem()
        currentItem = self.itemList.currentItem()

        if updateItem is not None and currentItem is not None:
            nodes.append( ( currentItem.data(Qt.UserRole), updateItem.data(Qt.UserRole) ) )
            return nodes

        for item in self.itemList.selectedItems():
            nodes.append( ( item.data(Qt.UserRole), item.data(Qt.UserRole + 9) ) )
        return nodes

if __name__ == '__main__':
    updateDialog = UpdateDialog()
    ok = updateDialog.exec_()
    print(ok)
    if ok == 1:
        print( updateDialog.getAllNodes() )
    elif ok == 2:
        print( updateDialog.getSelectedNodes() )
