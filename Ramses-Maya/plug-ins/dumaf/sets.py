# -*- coding: utf-8 -*-

# Set-relative functions.

import maya.cmds as cmds # pylint: disable=import-error
import maya.api.OpenMaya as om # pylint: disable=import-error

def getNodes( setName ):
    # Create a list and add the set
    selectionList = om.MSelectionList()
    try:
        selectionList.add( setName )
    except:
        return []
    # Get the node for the set and create a FnSet
    node = selectionList.getDependNode( 0 )
    set = om.MFnSet(node)
    # Get all members of the set
    publishNodes = set.getMembers( True )
    publishPaths = []
    # An iterator to get all dagPaths
    iterator = om.MItSelectionList(publishNodes)
    while not iterator.isDone():
        dagPath = iterator.getDagPath()
        publishPaths.append( dagPath.fullPathName() )
        iterator.next()
    return publishPaths
        
def exists(setName):
    # Create a list and add the set
    selectionList = om.MSelectionList()
    try:
        selectionList.add( setName )
    except:
        return False
    return True

def create_if_not_exists(setName):
    if exists(setName):
        return
    cmds.sets(name=setName)