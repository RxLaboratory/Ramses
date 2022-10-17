"""
The Rx Asset Management System (Ramses) Maya Plugin.
This is a file for running tests and should not be used in production.
"""

from maya import cmds # pylint: disable=import-error
import ramses
import ramses_maya as ram
import dumaf as maf

RAMSES = ramses.Ramses.instance()
DAEMON = RAMSES.daemonInterface()

maf.mayapy.reset_script_session( ram.utils.PLUGIN_PATH )

states = RAMSES.states()
print(states)
for s in states:
    print(s)