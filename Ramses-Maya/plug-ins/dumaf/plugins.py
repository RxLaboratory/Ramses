# -*- coding: utf-8 -*-

import maya.cmds as cmds # pylint: disable=import-error

class Plugin():

    @staticmethod
    def load(pluginName):
        ok = cmds.pluginInfo(pluginName, loaded=True, q=True)
        if not ok:
            cmds.loadPlugin(pluginName)
            return True
        return False
