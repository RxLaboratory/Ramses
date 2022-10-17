# -*- coding: utf-8 -*-
"""The UI for changing the scene status/publishing"""

from PySide2.QtWidgets import ( # pylint: disable=no-name-in-module
    QLabel,
    QDialog,
    QFormLayout,
    QHBoxLayout,
    QVBoxLayout,
    QSlider,
    QSpinBox,
    QTextEdit,
    QPushButton,
    QCheckBox,
)
from PySide2.QtCore import ( # pylint: disable=no-name-in-module
    Qt
)
from ramses_maya.ui_object_combobox import RamObjectBox
from ramses_maya.ui_dialog import Dialog

import ramses as ram
RAMSES = ram.Ramses.instance()

class StateBox( RamObjectBox ):
    """A Combobox to show states, whcih changes color according to the state"""
    def __init__(self, parent = None):
        super(StateBox,self).__init__(parent)

        # Populate states from Ramses
        for state in RAMSES.states():
            self.addItem( state.shortName(), state )

        self.setState( RAMSES.defaultState )

    def setState(self, state):
        """Sets the current state"""
        self.setObject(state)

    def getState(self):
        """Gets the current state"""
        return self.getObject()

class StatusDialog( Dialog ):
    """The Dialog for editing the status"""

    def __init__(self, parent = None):
        super(StatusDialog,self).__init__(parent)
        self.__setup_ui()
        self.__conect_evenhts()

    def __setup_ui(self):
        self.setWindowTitle( "Incremental Save: Update Status" )
        self.setMinimumWidth( 400 )

        main_layout = QVBoxLayout()
        main_layout.setSpacing(3)
        self.main_layout.addLayout(main_layout)

        top_layout = QHBoxLayout()
        top_layout.setSpacing(3)

        self.__ui_state_box = StateBox()
        top_layout.addWidget( self.__ui_state_box )

        self.__ui_completion_slider = QSlider( Qt.Horizontal )
        self.__ui_completion_slider.setMaximum(100)
        top_layout.addWidget( self.__ui_completion_slider )
        self.__ui_completion_box = QSpinBox( )
        self.__ui_completion_box.setMinimum( 0 )
        self.__ui_completion_box.setMaximum( 100 )
        self.__ui_completion_box.setSuffix( "%" )
        top_layout.addWidget( self.__ui_completion_box )

        main_layout.addLayout( top_layout )

        options_layout = QFormLayout()
        options_layout.setFieldGrowthPolicy( QFormLayout.AllNonFixedFieldsGrow )
        options_layout.setSpacing(3)

        self.__ui_publish_box = QCheckBox("Publish the current scene.")
        options_layout.addRow( "Publication:", self.__ui_publish_box )

        self.__ui_publish_settings_box = QCheckBox("Edit publish settings.")
        self.__ui_publish_settings_box.setEnabled(False)
        options_layout.addRow( "", self.__ui_publish_settings_box )

        self.__ui_preview_box = QCheckBox("Create preview files (thumbnail or playblast).")
        options_layout.addRow( "Preview:", self.__ui_preview_box )

        self.__ui_comment_label = QLabel("Comment:")
        self.__ui_comment_edit = QTextEdit()
        options_layout.addRow( self.__ui_comment_label, self.__ui_comment_edit )

        main_layout.addLayout( options_layout )

        buttons_layout = QHBoxLayout()
        buttons_layout.setSpacing(2)

        self.__ui_save_button = QPushButton("Update Status and Save")
        buttons_layout.addWidget( self.__ui_save_button )
        self.__ui_skip_button = QPushButton("Skip and just Save")
        buttons_layout.addWidget( self.__ui_skip_button )
        self.__ui_cancel_button = QPushButton("Cancel")
        buttons_layout.addWidget( self.__ui_cancel_button )

        main_layout.addLayout( buttons_layout )

    def __conect_evenhts(self):
        self.__ui_completion_slider.valueChanged.connect( self.__ui_completion_box.setValue )
        self.__ui_completion_box.valueChanged.connect( self.__ui_completion_slider.setValue )
        self.__ui_state_box.currentIndexChanged.connect(self.stateChanged )
        self.__ui_save_button.clicked.connect( self.accept )
        self.__ui_cancel_button.clicked.connect( self.reject )
        self.__ui_skip_button.clicked.connect( self.skip )
        self.__ui_publish_box.clicked[bool].connect( self.__ui_publish_settings_box.setEnabled )

    def setVirtual(self):
        """
        Sets the dialog to edit a virtual item status
        (no comment, no completion ratio)
        """
        self.__ui_completion_slider.hide()
        self.__ui_completion_box.hide()
        self.__ui_comment_label.hide()
        self.__ui_comment_edit.hide()
        # Use names in statebox
        for i in range(self.__ui_state_box.count() ):
            state = self.__ui_state_box.itemData(i)
            self.__ui_state_box.setItemText( i, state.name() )

    def stateChanged(self, i):
        """Updates the completion according to the state"""
        state = self.__ui_state_box.itemData(i)
        self.__ui_completion_box.setValue( state.completionRatio() )

    def setStatus( self, status):
        """Sets the status in the dialog"""
        self.__ui_state_box.setState( status.state() )
        self.__ui_completion_box.setValue( status.completionRatio() )
        self.__ui_comment_edit.setPlainText( status.comment() )

    def setState( self, state ):
        """Sets a state in the dialog"""
        stateName = ram.RamObject.getShortName( state )
        for i in range(self.__ui_state_box.count() ):
            if stateName == self.__ui_state_box.itemData(i).shortName():
                self.__ui_state_box.setCurrentIndex(i)

    def getState(self):
        """Returns the chosen state"""
        return self.__ui_state_box.getState()

    def getCompletionRatio(self):
        """Returns the completion ratio"""
        return self.__ui_completion_box.value()

    def getComment(self):
        """Returns the comment"""
        return self.__ui_comment_edit.toPlainText()

    def publish(self):
        """Checks if the scene has to be published"""
        return self.__ui_publish_box.isChecked()

    def edit_publish_settings(self):
        """Checks if the publish settings have to be edited"""
        return self.__ui_publish_settings_box.isChecked()

    def skip(self):
        """Skip the status update and just save"""
        self.done(2)

    def setPublish(self, pub=True):
        """Checks the publish box"""
        self.__ui_publish_box.setChecked(pub)

    def preview(self):
        """Checks the preview box"""
        return self.__ui_preview_box.isChecked()

if __name__ == '__main__':
    statusDialog = StatusDialog()
    # statusDialog.setVirtual()
    ok = statusDialog.exec_()
    print(ok)
