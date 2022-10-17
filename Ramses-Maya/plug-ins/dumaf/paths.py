# -*- coding: utf-8 -*-

# Functions to handle node paths.

import maya.cmds as cmds # pylint: disable=import-error

def baseName( node, keepNameSpace = False ):
    nodeName = node.split('|')[-1]
    if not keepNameSpace:
        nodeName = nodeName.split(':')[-1]
    return nodeName

def absolutePath( nodeName ):
    n = cmds.ls(nodeName, long=True)
    if n is not None:
        return n[0]
    return nodeName
