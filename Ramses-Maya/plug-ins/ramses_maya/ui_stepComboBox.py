# -*- coding: utf-8 -*-
"""
A ComboBox for selecting steps
"""

from PySide2.QtWidgets import (
    QComboBox
)
from PySide2.QtCore import ( # pylint: disable=no-name-in-module
    Slot,
    Qt,
    Signal,
)
from ramses import RamStep

class StepComboBox(QComboBox):
    """
    A ComboBox for selecting steps
    """
    # <== CONSTRUCTOR ==>

    def __init__(self, parent=None): # pylint: disable=useless-super-delegation
        super(StepComboBox, self).__init__(parent)

    # <== PUBLIC ==>

    def set_steps(self, steps):
        """Sets the list of steps"""

        self.clear()
        for step in steps:
            self.addItem(str(step), step)

    def current_step(self):
        return self.currentData()

    @Slot()
    def set_step(self, step):
        if isinstance(step, RamStep):
            for i in range(self.count()):
                if self.itemData(i, Qt.UserRole) == step:
                    self.setCurrentIndex(i)
                    return
        else:
            for i in range(self.count()):
                if self.itemData(i, Qt.UserRole).shortName() == step:
                    self.setCurrentIndex(i)
                    return
        return None
