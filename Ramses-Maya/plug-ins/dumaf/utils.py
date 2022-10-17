"""Utils"""

from time import time
import os
from inspect import getsourcefile
from maya import cmds # pylint: disable=import-error
from dupyf import updater

def getModulePath():
    """Gets the folder containing the module"""
    return os.path.dirname( os.path.abspath(getsourcefile(lambda:0))  )

def checkUpdate( toolName, toolVersion, language="en", preRelease=False, discreet=True ):
    """Checks if an update is available for the tool"""
    from dumaf.ui import UpdateDialog

    # if discreet, only once a day
    if discreet:
        now = time()
        latest = cmds.optionVar(q='dumaf-latestupdate-' + toolName)
        if now - latest < 86400:
            return


    info = updater.checkUpdate(
        "http://api.rxlab.io",
        toolName,
        toolVersion,
        "Maya",
        cmds.about(majorVersion=True) + "." + cmds.about(minorVersion=True) + "." + cmds.about(patchVersion=True),
        preRelease,
        language
        )

    cmds.optionVar(intValue=('dumaf-latestupdate-' + toolName, time()))

    if not info["update"]:
        if not discreet:
            cmds.confirmDialog(
                title=toolName,
                message=toolName + " is up-to-date!",
                button=["OK"],
                icon="information"
            )
        print(toolName + " is up-to-date (" + toolVersion + ").")
        return

    dialog = UpdateDialog( info, toolName, toolVersion)
    dialog.exec_()

if __name__ == "__main__":
    checkUpdate( "Ramses-Maya", "0.0.1", discreet=False)
