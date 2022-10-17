# -*- coding: utf-8 -*-
"""The entry point for updating items"""

from maya import cmds # pylint: disable=import-error
import dumaf
from .utils_attributes import is_ramses_managed

def update( node, new_nodes ):
    """Updates the node (and children) with the new nodes"""
    # Get a locator for the current node
    root_locator = create_update_locator( node )

    # Keep current deformer and rendering sets
    node = dumaf.Node(node)
    node_sets = node.sets()

    # Get ramses locators
    node_locators = get_node_locators( node.path() )

    new_root_ctrls = update_nodes( node.path(), new_nodes, root_locator, node_sets, node_locators )

    cmds.delete(root_locator)
    for loc in node_locators:
        cmds.delete(loc)

    return new_root_ctrls

def create_update_locator( node ):
    """Creates a locator to update the node"""
    # Create a locator to keep current transform
    root_locator = cmds.spaceLocator(name='_ramsesTempLocator_')
    # Snap!
    root_locator = dumaf.Node(root_locator)
    root_locator.snap( node )

    return root_locator.path()

def get_node_locators( root_node ):
    """Gets the locators for all child ramses nodes"""
    current_nodes = cmds.listRelatives(root_node, ad = True, f=True)
    if current_nodes is None:
        return []
    locators = []
    for node in current_nodes:
        if is_ramses_managed( node ):
            node_name = dumaf.paths.baseName(node, True)
            loc = cmds.spaceLocator(name=node_name + '_ramsesTempLocator_')
            loc = dumaf.Node(loc)
            loc.snap( node )
            locators.append(loc.path())
    return locators

def update_nodes( old_node, new_nodes, root_locator, node_sets, node_locators ):
    """Replaces the old node by the new nodes"""
    root_ctrls = []
    for new_node in new_nodes:

        # Move to the locator
        new_node = dumaf.Node(new_node)
        new_node.snap( root_locator )

        # Re-set deformers and rendering sets
        children = cmds.listRelatives( new_node.path(), ad = True, f = True)
        if children is not None:
            children.append(new_node.path())
            for child in children:
                new_name = dumaf.paths.baseName(child, True)
                # Reset deformers and shaders
                if new_name in node_sets:
                    new_sets = node_sets[new_name]
                    for new_set in new_sets:
                        try:
                            cmds.sets( child, add=new_set )
                        except: # Shaders have to be forced as an object can't be in two shader sets at once # pylint: disable=bare-except
                            try: # There may still be special sets
                                cmds.sets( child, forceElement=new_set )
                            except:
                                pass
                # Snap to locator
                if is_ramses_managed( child ):
                    for loc in node_locators:
                        loc_name = dumaf.paths.baseName(loc, True)
                        loc_name = loc_name.split('_ramsesTempLocator_')[0]
                        if loc_name == new_name:
                            child = dumaf.Node(child)
                            child.snap( loc )

        # Re-parent the root to the previous parent
        root_parent = cmds.listRelatives( old_node, parent=True, f=True, type='transform')
        if root_parent is not None:
            root_parent = root_parent[0]
        else:
            root_parent = '|'

        new_node.parent_to( root_parent )

        root_ctrls.append(new_node.path())

    return root_ctrls
