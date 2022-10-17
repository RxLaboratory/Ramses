"""
The Rx Asset Management System (Ramses) Maya Plugin.
This script reloads the whole plugin. Use this when developping to reload all included modules.
"""

import ramses_maya as ram
from dumaf import mayapy

print(ram.utils.PLUGIN_PATH)
mayapy.reset_script_session( ram.utils.PLUGIN_PATH )
