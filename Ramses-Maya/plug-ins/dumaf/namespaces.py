# -*- coding: utf-8 -*-

import maya.cmds as cmds # pylint: disable=import-error

class Namespace():

    @staticmethod
    def removeAll():
        # Set the current namespace to the root
        cmds.namespace(setNamespace=':')

        # Remove namespaces containing nodes
        nodes = cmds.ls()
        namespaces = []
        for node in nodes:
            if ':' in node:
                nodeNamespaces = node.split(":")[0:-1]
                for nodeNamespace in nodeNamespaces:
                    if not nodeNamespace in namespaces:
                        namespaces.append(nodeNamespace)
        for namespace in namespaces:
            try:
                cmds.namespace(removeNamespace=namespace, mergeNamespaceWithRoot=True)
            except:
                pass

        # Remove remaining namespaces without merging with root
        namespaces = cmds.namespaceInfo(listOnlyNamespaces=True, recurse=True)
        for namespace in namespaces:
            try:
                cmds.namespace(removeNamespace=namespace)
            except:
                pass
