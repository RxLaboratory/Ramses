# pylint: disable=invalid-name
"""
The Rx Asset Management System (Ramses) Maya Plugin
"""

import ramses_maya as ram
import maya.api.OpenMaya as om # pylint: disable=import-error

def maya_useNewAPI(): # pylint: disable=invalid-name
    """
    The presence of this function tells Maya that the plugin produces, and
    expects to be passed, objects created using the Maya Python API 2.0.
    """
    #pass

SAVE_CMD = """
import maya.cmds as cmds
ok = cmds.pluginInfo('Ramses', loaded=True, q=True)
if not ok:
    cmds.loadPlugin('Ramses')
cmds.ramSave()
"""

OPEN_CMD = """
import maya.cmds as cmds
ok = cmds.pluginInfo('Ramses', loaded=True, q=True)
if not ok:
    cmds.loadPlugin('Ramses')
cmds.ramOpen()
"""

SAVE_AS_CMD = """
import maya.cmds as cmds
ok = cmds.pluginInfo('Ramses', loaded=True, q=True)
if not ok:
    cmds.loadPlugin('Ramses')
cmds.ramSaveAs()
"""

def installHotkeys():
    """
    Registers Ramses Hotkeys
    """
    # Register hotkeys
    settings = ram.RamSettings.instance()
    registerSave = True
    if 'useRamSaveSceneHotkey' in settings.userSettings:
        registerSave = settings.userSettings['useRamSaveSceneHotkey']
    registerOpen = True
    if 'useRamOpenceneHotkey' in settings.userSettings:
        registerOpen = settings.userSettings['useRamOpenceneHotkey']
    registerSaveAs = True
    if 'useRamSaveAsHotkey' in settings.userSettings:
        registerSaveAs = settings.userSettings['useRamSaveAsHotkey']

    if registerSave:
        ram.maf.HotKey.createHotkey(
            SAVE_CMD,
            'ctrl+s',
            'RamSaveScene',
            "Ramses Save Scene",
            "Ramses"
            )

    if registerOpen:
        ram.maf.HotKey.createHotkey(OPEN_CMD,
            'ctrl+o',
            'RamOpenScene',
            "Ramses Open Scene",
            "Ramses"
            )

    if registerSaveAs:
        ram.maf.HotKey.createHotkey(SAVE_AS_CMD,
            'ctrl+shift+s',
            'RamSaveSceneAs',
            "Ramses Save Scene As",
            "Ramses"
            )

def initializePlugin( obj ):
    """
    Initializes the plugin, loads the classes for maya commands
    """
    plugin = om.MFnPlugin(obj, ram.VENDOR, ram.VERSION)

    ram.log( "Hi, I'm Ramses, the Rx Asset Management System... I'm loading!" )

    for c in ram.cmds_classes:
        try:
            plugin.registerCommand( c.name, c.createCommand, c.createSyntax )
        except Exception as e: # pylint: disable=broad-except
            print(e)
            ram.log( "Failed to register command: %s\n" % c.name, ram.LogLevel.Critical )

    installHotkeys()

    ram.log( "I'm ready!" )

def uninitializePlugin( obj ):
    """
    Unitializes the plugin, unloads the classes for maya commands
    """
    plugin = om.MFnPlugin(obj, ram.VENDOR, ram.VERSION)

    # Rstore hotkeys
    ram.maf.HotKey.restoreSaveSceneHotkey()
    ram.maf.HotKey.restoreOpenSceneHotkey()
    ram.maf.HotKey.restoreSaveSceneAsHotkey()

    for c in reversed( ram.cmds_classes ):
        try:
            plugin.deregisterCommand( c.name )
        except Exception as e: # pylint: disable=broad-except
            print(e)
            ram.log( "Failed to unregister command: %s\n" % c.name )

    ram.log( "Thanks for playing with me. Much love, See you soon." )
