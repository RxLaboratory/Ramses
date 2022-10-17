# -*- coding: utf-8 -*-
"""Some general utilitary stuff"""

import os
from PySide2.QtGui import ( # pylint: disable=no-name-in-module,import-error
    QIcon,
    QDesktopServices
)
from PySide2.QtCore import ( # pylint: disable=no-name-in-module
    Slot,
    QUrl
)
from maya import cmds # pylint: disable=import-error

# Some paths we need
MODULE_PATH = os.path.dirname(os.path.realpath(__file__))
PLUGIN_PATH = os.path.dirname(MODULE_PATH)
ICON_PATH = os.path.dirname(PLUGIN_PATH) + "/icons/"
PUBLISH_PRESETS_PATH = os.path.dirname(PLUGIN_PATH) + "/publish_presets/"
IMPORT_PRESETS_PATH = os.path.dirname(PLUGIN_PATH) + "/import_presets/"

def icon(name):
    """Gets QIcon for an icon from its name (without extension)"""
    return QIcon(ICON_PATH + name + ".png")

@Slot()
def open_help():
    """Opens the online help for the addon"""
    QDesktopServices.openUrl( QUrl( "https://ramses.rxlab.guide/0.5/components/addons/maya" ) )

@Slot()
def about_ramses():
    """Opens the web page about Ramses"""
    QDesktopServices.openUrl( QUrl( "https://rxlaboratory.org/tools/ramses" ) )

@Slot()
def open_api_reference():
    """Opens the online API reference"""
    QDesktopServices.openUrl( QUrl( "https://ramses.rxlab.guide/0.5/dev/add-ons-reference/" ) )

def end_process(temp_data, progress_dialog):
    """Ends a process on the scene (closes and removes the temp file)"""
    # Re-Open initial scene
    cmds.file(temp_data[1],o=True,f=True)

    # Remove temp file
    if os.path.isfile(temp_data[0]):
        os.remove(temp_data[0])

    if progress_dialog:
        progress_dialog.hide()
