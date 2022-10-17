# -*- coding: utf-8 -*-
"""A ComboBox for displaying RamObjects"""

from PySide2.QtWidgets import ( # pylint: disable=no-name-in-module
    QComboBox,
)
from PySide2.QtGui import (  # pylint: disable=no-name-in-module
    QColor,
    QPalette,
    )
from PySide2.QtCore import ( # pylint: disable=no-name-in-module
    Slot,
)

import ramses as ram
RAMSES = ram.Ramses.instance()

class RamObjectBox( QComboBox ):
    """A ComboBox for displaying RamObjects"""
    def __init__(self, parent = None):
        super(RamObjectBox,self).__init__(parent)
        self.currentIndexChanged.connect( self.indexChanged )

    @Slot()
    def indexChanged(self, i):
        """Sets the color of the box"""
        color = QColor( 93, 93, 93 )
        pal = self.palette()

        obj = self.itemData(i)
        if obj:
            colorName = self.itemData(i).colorName()
            color = QColor( colorName )

        # adjust Lightness
        if color.lightness() > 120:
            color.setHsl( color.hue(), color.saturation(), 120)

        pal.setColor(QPalette.Button, color)
        self.setPalette(pal)

    def setObject(self, ramObject):
        """Sets the current ramobject"""
        i = 0
        while i < self.count():
            if self.itemData( i ) == ramObject:
                self.setCurrentIndex( i )
                return
            i = i+1

    def getObject(self):
        """Gets the current ramobject"""
        return self.itemData( self.currentIndex() )
