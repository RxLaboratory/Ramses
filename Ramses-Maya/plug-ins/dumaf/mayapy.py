"""Useful methods for managing python by Maya"""

import inspect
import sys
from os.path import dirname

def reset_script_session(script_path=None):
    """Reloads the modules in the path. If script_path is omitted, uses the current script file path"""
    if script_path is None:
        script_path = dirname(__file__)
    # Convert this to lower just for a clean comparison later
    script_path = script_path.lower().replace("\\","/")

    to_delete = []
    # Iterate over all the modules that are currently loaded
    for key, module in sys.modules.items():
        # There's a few modules that are going to complain if you try to query them
        # so I've popped this into a try/except to keep it safe
        try:
            # Use the "inspect" library to get the moduleFilePath that the current module was loaded from
            module_file_path = inspect.getfile(module).lower().replace("\\","/")

            # Don't try and remove the startup script, that will break everything
            if module_file_path == __file__.lower():
                continue

            # If the module's filepath contains the userPath, add it to the list of modules to delete
            if script_path in module_file_path:
                print( "Removing %s" % key )
                to_delete.append(key)
        except Exception as err: # pylint: disable=broad-except
            print(err)

    # If we'd deleted the module in the loop above, it would have changed the size of the dictionary and
    # broken the loop. So now we go over the list we made and delete all the modules
    for module in to_delete:
        del sys.modules[module]
