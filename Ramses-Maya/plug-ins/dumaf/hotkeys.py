# -*- coding: utf-8 -*-

# Functions to create and edit "name commands" and hotkeys.

import maya.cmds as cmds # pylint: disable=import-error

class HotKey():
    
    @staticmethod
    def createNameCommand( name, annotation, pyCommand):
        # Because the 'sourceType' parameter of cmds.nameCommand is broken, we first need to create a runtime command to run our python code...

        # There is no way to edit a runtime command so we need to check if it
        # exists and then remove it if it does.
        if cmds.runTimeCommand(name, q=True, exists=True):
            cmds.runTimeCommand(name, e=True, delete=True)
        # Now, re-create it
        cmd = cmds.runTimeCommand(
            name, 
            ann=annotation, 
            category='User', 
            command=pyCommand, 
            commandLanguage='python'
            )

        # Create the command
        nc = cmds.nameCommand( name + "NameCommand", ann=annotation, command=cmd)

        return nc

    @staticmethod
    def createHotkey( pyCommand, shortcut, name, annotation = None, hotkeySetName='DuMAF' ):
        # We need a custom hotkeySet
        if not cmds.hotkeySet(hotkeySetName, exists=True):
            cmds.hotkeySet( hotkeySetName )
        # Set it as currrent
        cmds.hotkeySet( hotkeySetName, current=True, e=True )

        if not annotation: annotation = name

        shortcut = shortcut.lower()
        
        ctrl = False
        if 'ctrl+' in shortcut:
            ctrl = True
            shortcut = shortcut.replace('ctrl+', '')
        alt = False
        if 'alt+' in shortcut:
            alt = True
            shortcut = shortcut.replace('alt+', '')
        shift = False
        if 'shift+' in shortcut:
            shift = True
            shortcut = shortcut.replace('shift+', '')

        # Now we can create hotkeys
        cmd = HotKey.createNameCommand( name, annotation, pyCommand )
        cmds.hotkey(keyShortcut=shortcut, ctrlModifier=ctrl, altModifier=alt, shiftModifier=shift, name=cmd )
        cmds.savePrefs(hotkeys=True)


    @staticmethod
    def restoreOpenSceneHotkey():
        # We need to re-create a nameCommand, because Maya...
        command = cmds.nameCommand( annotation="OpenSceneNameCommand", command='OpenScene' )
        cmds.hotkey(keyShortcut='o', ctrlModifier = True, name=command)
        cmds.savePrefs(hotkeys=True)

    @staticmethod
    def restoreSaveSceneAsHotkey():
        # We need to re-create a nameCommand, because Maya...
        command = cmds.nameCommand( annotation="SaveSceneAsNameCommand", command='SaveSceneAs' )
        cmds.hotkey(keyShortcut='s', ctrlModifier = True, shiftModifier=True, name=command)
        cmds.savePrefs(hotkeys=True)

    @staticmethod
    def restoreSaveSceneHotkey():
        # We need to re-create a nameCommand, because Maya...
        command = cmds.nameCommand( annotation="SaveSceneNameCommand", command='SaveScene' )
        cmds.hotkey(keyShortcut='s', ctrlModifier = True, name=command)
        cmds.savePrefs(hotkeys=True)