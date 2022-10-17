# -*- coding: utf-8 -*-
"""The entry point for replacing items"""

import yaml
import os
from maya import cmds # pylint: disable=import-error
import ramses
import dumaf
from .utils_attributes import list_ramses_nodes
from .utils_options import get_option
from .update_manager import update
from .ui_import import ImportSettingsDialog
from .import_manager import get_import_group, get_import_namespace, import_file, get_format_options
from .utils_files import get_step_for_file

def replacer(item, file_path, step, import_options, show_import_options=False):
    """Runs a few checks and replaces selected nodes with the ones from the filepath"""

    # Try to find the current step
    current_scene_file = cmds.file( q=True, sn=True )
    current_step = get_step_for_file( current_scene_file )

    extension = os.path.splitext(file_path)[1][1:]

    # Get options
    if not import_options:
        import_options = { "formats": [] }
        for p in step.outputPipes():
            if current_step is None or current_step.shortName() == p.inputStepShortName():
                for f in p.pipeFiles():
                    options_str = f.customSettings()
                    if options_str != "":
                        options = yaml.safe_load( options_str )
                        if options['format'] == extension:
                            import_options['formats'].append( options )
                            break
                break

    if not 'formats' in import_options:
        import_options['formats'] = ()
    
    if show_import_options:
        import_dialog = ImportSettingsDialog()
        if len(import_options['formats']) != 0:
            import_dialog.set_options(import_options)
        else:
            # Set defaults from file paths
            import_options = {}
            formats = []
            if extension != "":
                o = {
                    'format': extension
                }
            formats.append(o)
            import_options['formats'] = formats
            import_dialog.set_options(import_options)

        if not import_dialog.exec_():
            return
        import_options = import_dialog.get_options()

    ramses.log("I'm replacing the selected nodes with " + str(item))

    # Keep the list of selected Ramses nodes (the selection changes when importing)
    original_nodes = list_ramses_nodes(selected_only=True)

    # Check if we're getting shaders
    file_name = os.path.basename(file_path)
    file_name = os.path.splitext(file_name)[0]

    # Prepare scene to get its new stuff
    item_group = get_import_group(item)
    item_namespace = get_import_namespace(item)

    options = get_format_options(file_path, import_options)
    lock_transform = get_option("lock_transformations", options, True)
    as_reference = get_option("as_reference", options, False)
    no_root_shape = get_option("no_root_shape", options, False)

    for original_node in original_nodes:
        new_nodes = import_file(file_path, as_reference, lock_transform, no_root_shape, item, item_namespace, item_group, step)
        update(original_node, new_nodes)
        original_node = dumaf.Node(original_node)
        original_node.remove()
