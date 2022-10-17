# -*- coding: utf-8 -*-
"""Scene functions"""

import os
import tempfile
import maya.cmds as cmds  # pylint: disable=import-error

from .nodes import Node


class Scene():
    """Scene functions"""

    @staticmethod
    def checkSaveState():
        """Checks if the current scene needs to be saved,
        and asks for the user to save if needed.
        Returns False if the user cancels the operation,
        True if the scene can be safely closed"""
        currentFilePath = cmds.file(q=True, sn=True)
        if cmds.file(q=True, modified=True):
            sceneName = os.path.basename(currentFilePath)
            if sceneName == '':
                sceneName = 'untitled scene'
            result = cmds.confirmDialog(
                message="Save changes to " + sceneName + "?",
                button=("Save", "Don't Save", "Cancel")
                )
            if result == 'Cancel':
                return False
            if result == 'Save':
                cmds.file(save=True, options="v=1;")
        return True

    @staticmethod
    def createTempScene(name=''):
        """Creates a working file.
        Returns the working file path and the current file path"""
        prevFile = cmds.file(q=True, sn=True)
        # Rename the file because we're going to modify stuff in there
        tempDir = tempfile.gettempdir()
        fileName = 'RamsesWorkingFile' + name + '.mb'
        tempFile = tempDir + '/' + fileName
        cmds.file(rename=tempFile)
        return (tempFile, prevFile)

    @staticmethod
    def importFile(filePath, nameSpace=''):
        """Imports a file and returns the new nodes"""
        nodes = cmds.file(
            filePath,
            i=True,
            ignoreVersion=True,
            mergeNamespacesOnClash=True,
            returnNewNodes=True,
            ns=nameSpace
        )
        return Node.get_nodes(nodes)
