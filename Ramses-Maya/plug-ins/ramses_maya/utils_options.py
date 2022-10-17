# -*- coding: utf-8 -*-
"""Utilitaries to manage options dicts"""

from PySide2.QtCore import Qt # pylint: disable=no-name-in-module


def get_option(name, options, default=None):
    """Returns the option or the default value if it is missing"""
    if name in options:
        return options[name]

    return default

def load_bool_preset(name, options, checkbox, default=None):
    """Sets a checkbox to the option"""
    if name in options:
        checkbox.setChecked(options[name])
    elif default is not None:
        checkbox.setChecked(default)

def load_enum_preset( name, options, combobox, default=None ):
    """Sets a combobox to the option, using the items data"""
    data = default
    if name in options:
        data = options[name]

    if data is not None:
        for i in range(combobox.count()):
            if combobox.itemData(i, Qt.UserRole) == options[name]:
                combobox.setCurrentIndex(i)
                break

def load_number_preset( name, options, spinbox, default=None):
    """Sets a spinbox to the option"""
    if name in options:
        spinbox.setValue( options[name] )
    elif default is not None:
        spinbox.setValue( default )
