# -*- coding: utf-8 -*-
"""
Useful functions to manage Ramses nodes
"""

import maya.cmds as cmds # pylint: disable=import-error
import ramses as ram # pylint: disable=import-error
from .utils_attributes import (
    set_ramses_attr,
    RamsesAttribute,
)

def get_publish_nodes():
    """
    Lists all the nodes to be published
    """
    try:
        nodes = cmds.sets( 'Ramses_Publish', nodesOnly=True, q=True )
    except ValueError:
        ram.log("Nothing to publish! The asset you need to publish must be listed in a 'Ramses_Publish' set.")
        cmds.inViewMessage( msg='Nothing to publish! The asset you need to publish must be listed in a <hl>Ramses_Publish</hl> set.', pos='midCenter', fade=True )
        return []

    if nodes is None or len(nodes) == 0:
        ram.log("The 'Ramses_Publish' set is empty, there's nothing to publish!")
        cmds.inViewMessage( msg="The <hl>Ramses_Publish</hl> set is empty, there's nothing to publish!", pos='midCenter', fade=True )
        return []

    return nodes

def get_proxy_nodes( show_alert = False ):
    """
    Lists all the nodes to be published as proxies
    """
    try:
        nodes = cmds.sets( 'Ramses_Proxies', nodesOnly=True, q=True )
    except ValueError:
        if show_alert:
            ram.log("Can't find any proxy! The proxy you need to publish must be listed in a 'Ramses_Proxies' set.")
            cmds.inViewMessage( msg="Can't find any proxy! The proxy you need to publish must be listed in a <hl>Ramses_Proxies</hl> set.", pos='midCenter', fade=True )
        return []

    if nodes is None or len(nodes) == 0:
        if show_alert:
            ram.log("The 'Ramses_Proxies' set is empty.")
            cmds.inViewMessage( msg="The <hl>Ramses_Proxies</hl> set is empty.", pos='midCenter', fade=True )
        return []

    for node in nodes:
        set_ramses_attr( node, RamsesAttribute.IS_PROXY, True, 'bool' )

    return nodes

def get_del_on_publish_nodes( ):
    """
    Lists all the nodes to be deleted on publish
    """
    try:
        nodes = cmds.sets( 'Ramses_DelOnPublish', nodesOnly=True, q=True )
    except ValueError:
        return []

    if nodes is None:
        return []

    return nodes

def delete_ramses_sets():
    """Removes the sets created by Ramses in the scene"""

    try:
        cmds.delete( 'Ramses_Publish' )
    except ValueError:
        pass

    try:
        cmds.delete( 'Ramses_Proxies' )
    except ValueError:
        pass

    try:
        cmds.delete( 'Ramses_DelOnPublish' )
    except ValueError:
        pass