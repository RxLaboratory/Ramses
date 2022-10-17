# -*- coding: utf-8 -*-

import maya.cmds as cmds # pylint: disable=import-error

class Reference():

    @staticmethod
    def importAll():
        refs = cmds.ls(type='reference')
        if refs is None:
            return
        for ref in refs:
            try:
                refFile = cmds.referenceQuery(ref, f=True)
                cmds.file(refFile, importReference=True)
            except:
                continue
