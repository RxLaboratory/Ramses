# -*- coding: utf-8 -*-
"""
Useful functions to manage Maya and Ramses custom attributes on nodes
"""

import os
import maya.cmds as cmds # pylint: disable=import-error
import dumaf as maf # pylint: disable=import-error
import ramses as ram

class RamsesAttribute():
    """Enum for attribute names"""
    MANAGED = 'ramsesManaged'
    STEP = 'ramsesStep'
    ITEM = 'ramsesItem'
    ASSET_GROUP = 'ramsesAssetGroup'
    ITEM_TYPE = 'ramsesItemType'
    SOURCE_FILE = 'ramsesSourceFile'
    SOURCE_TIME = 'ramsesTimeStamp'
    SHADING_TYPE = 'ramsesShadingType'
    SHADED_OBJECTS = 'ramsesShadedObjects'
    DT_TYPES = ('string','float2','float3')
    AT_TYPES = ('long', 'bool')
    IS_PROXY = 'ramsesIsProxy'
    VERSION = 'ramsesVersion'
    STATE = 'ramsesState'
    ORIGIN_POS = 'ramsesOriginalPos'
    ORIGIN_ROT = 'ramsesOriginalRot'
    ORIGIN_SCA = 'ramsesOriginalSca'
    RESOURCE = 'ramsesResource'

def set_import_attributes( node, item, step, file_path ):
    """Sets the attributes needed when importing an asset"""
    timestamp = os.path.getmtime( file_path )
    timestamp = int(timestamp)

    version = ram.RamMetaDataManager.getVersion( file_path )
    state = ram.RamMetaDataManager.getState( file_path )
    resource = ram.RamMetaDataManager.getResource(file_path)

    set_ramses_managed( node )
    set_ramses_attr( node, RamsesAttribute.SOURCE_FILE, file_path, 'string' )
    set_ramses_attr( node, RamsesAttribute.SOURCE_TIME, timestamp, 'long' )
    set_ramses_attr( node, RamsesAttribute.VERSION, version, 'long' )
    set_ramses_attr( node, RamsesAttribute.STATE, state, 'string' )
    set_ramses_attr( node, RamsesAttribute.STEP, str(step), 'string' )
    set_ramses_attr( node, RamsesAttribute.ITEM, str(item), 'string' )
    set_ramses_attr( node, RamsesAttribute.ITEM_TYPE, item.itemType(), 'string' )
    set_ramses_attr( node, RamsesAttribute.ASSET_GROUP, item.group(), 'string' )
    set_ramses_attr( node, RamsesAttribute.RESOURCE, resource, 'string' )

def set_ramses_attr3( node, attr, x, y, z, t):
    """Sets a 3-dimensionnal Ramses attribute to the node"""
    # Temporarily unlock ref edit
    cmds.optionVar(iv=("refLockEditable",1))
    try: # Some nodes won't accept new attributes
        # Add if not already there
        if attr not in cmds.listAttr(node):
            if t in RamsesAttribute.DT_TYPES:
                cmds.addAttr( node, ln= attr, dt=t)
            else:
                cmds.addAttr( node, ln=attr, at=t)
        # Unlock
        cmds.setAttr( node + '.' + attr, lock=False )
        # Set
        if t in RamsesAttribute.DT_TYPES:
            cmds.setAttr( node + '.' + attr, x, y, z, type=t)
        else:
            cmds.setAttr( node + '.' + attr, x, y, z )
        # Lock
        cmds.setAttr( node + '.' + attr, lock=True )
    except: # pylint: disable=bare-except
        pass
    cmds.optionVar(iv=("refLockEditable",0))

def set_ramses_attr( node, attr, value, t):
    """Sets a simple Ramses attribute to the node"""
    if isinstance(node, maf.Node):
        node = node.path()
    # Temporarily unlock ref edit
    cmds.optionVar(iv=("refLockEditable",1))
    try: # Some nodes won't accept new attributes
        # Add if not already there
        if attr not in cmds.listAttr(node):
            if t in RamsesAttribute.DT_TYPES:
                cmds.addAttr( node, ln= attr, dt=t)
            else:
                cmds.addAttr( node, ln=attr, at=t)
        # Unlock
        cmds.setAttr( node + '.' + attr, lock=False )
        # Set
        if t in RamsesAttribute.DT_TYPES:
            cmds.setAttr( node + '.' + attr, value, type=t)
        else:
            cmds.setAttr( node + '.' + attr, value )
        # Lock
        cmds.setAttr( node + '.' + attr, lock=True )
    except: # pylint: disable=bare-except
        pass
    cmds.optionVar(iv=("refLockEditable",0))

def get_ramses_attr( node, attr):
    """Gets a Ramses attribute from the node"""
    if attr not in cmds.listAttr(node):
        return ''
    return cmds.getAttr(node + '.' + attr)

def set_ramses_managed(node, managed=True):
    """Sets the node as managed by Ramses (an asset from the pipeline).
    Actually (un)checks the RamsesAttribute.MANAGED"""
    set_ramses_attr( node, RamsesAttribute.MANAGED, managed, 'bool' )

def get_item( node ):
    """Gets the RamItem corresponding to this node"""
    # try from path first
    sourcePath = get_ramses_attr( node, RamsesAttribute.SOURCE_FILE )
    if sourcePath != '':
        item = ram.RamItem.fromPath( sourcePath )
        if item is not None: return item
    # try from name
    name = get_ramses_attr( node, RamsesAttribute.ITEM )
    itemType = get_ramses_attr( node, RamsesAttribute.ITEM_TYPE )
    if name == '': return None
    return ram.RamItem.fromString( name, itemType )

def get_step( node ):
    """Gets the RamStep corresponding to this node"""
    # try from path first
    sourcePath = get_ramses_attr( node, RamsesAttribute.SOURCE_FILE )
    if sourcePath != '':
        step = ram.RamStep.fromPath( sourcePath )
        if step is not None: return step
    # try from name
    name = get_ramses_attr( node, RamsesAttribute.STEP )
    if name == '': return None
    return ram.RamStep.fromString( name )

def get_state( node ):
    """Gets the RamState corresponding to this node"""
    state = get_ramses_attr( node, RamsesAttribute.STATE )
    return ram.Ramses.instance().state(state)

def is_ramses_managed(node):
    """Checks if the node must be managed by Ramses (if the RamsesAttribute.MANAGED is checked)"""
    return get_ramses_attr( node, RamsesAttribute.MANAGED )

def list_ramses_nodes(node_type='transform', selected_only=False):
    """Lists all the nodes managed by Ramses in the scene"""
    # Scan all transform nodes
    scene_nodes = ()
    if node_type == '':
        scene_nodes = cmds.ls( long=True, selection=selected_only )
    else:
        scene_nodes = cmds.ls( type=node_type, long=True, selection=selected_only )

    nodes = []

    progress_dialog = maf.ProgressDialog()
    progress_dialog.show()
    progress_dialog.setText("Scanning Scene for Ramses Nodes")
    progress_dialog.setMaximum(len(scene_nodes))

    for node in scene_nodes:
        progress_dialog.increment()
        try:
            if is_ramses_managed(node):
                nodes.append(node)
        except: # pylint: disable=bare-except
            pass

    progress_dialog.hide()
    return nodes
