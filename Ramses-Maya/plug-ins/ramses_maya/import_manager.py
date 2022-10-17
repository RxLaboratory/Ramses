# -*- coding: utf-8 -*-
"""The entry point for importing assets"""

import re
import os
from maya import cmds # pylint: disable=import-error
import yaml
import ramses as ram
from dumaf import ProgressDialog, Node, Plugin
from .ui_import import ImportSettingsDialog
from .utils_options import get_option
from .utils_attributes import RamsesAttribute, get_ramses_attr, set_import_attributes, is_ramses_managed
from .utils_files import get_step_for_file

def importer( item, file_paths, step, import_options=None, show_import_options=False):
    """The entry point for importing assets"""

    # Try to find the current step
    current_scene_file = cmds.file( q=True, sn=True )
    current_step = get_step_for_file( current_scene_file )

    # Get options
    if not import_options:
        import_options = { "formats": [] }
        for p in step.outputPipes():
            ram.log("Checking pipe: " + str(p), ram.LogLevel.Debug)
            if current_step is None or current_step.shortName() == p.inputStepShortName():
                for f in p.pipeFiles():
                    options_str = f.customSettings()
                    ram.log("Found options:\n" + options_str, ram.LogLevel.Debug)
                    if options_str != "":
                        options = yaml.safe_load( options_str )
                        import_options['formats'].append( options )

    if len(import_options['formats']) == 0 or show_import_options:
        import_dialog = ImportSettingsDialog()
        if len(import_options['formats']) != 0:
            import_dialog.set_options(import_options)
        else:
            # Set defaults from file paths
            import_options = {}
            formats = []
            for f in file_paths:
                ext = os.path.splitext(f)[1]
                if ext != "":
                    o = {
                        'format': ext[1:]
                    }
                    formats.append(o)
            import_options['formats'] = formats
            import_dialog.set_options(import_options)

        if not import_dialog.exec_():
            return
        import_options = import_dialog.get_options()

    # Progress
    progress_dialog = ProgressDialog()
    progress_dialog.show()
    progress_dialog.setText("Importing items...")
    progress_dialog.setMaximum(len(file_paths) + 1)
    progress_dialog.increment()

    # Keep the Geo nodes and the selection to import the shaders on them automatically
    geo_nodes = cmds.ls(selection=True, type='transform', long=True)
    if not geo_nodes:
        geo_nodes = []

    # Look for shaders, as they must be imported at last
    # their filename ends with -shaders
    shader_files = []
    for file_path in reversed(file_paths):
        file_name = os.path.basename(file_path)
        ram.log("Checking file to import: " + file_name)
        file_name = os.path.splitext(file_name)[0]
        if file_name.endswith("-shaders"):
            ram.log("> Looks like a shader file!")
            shader_files.append(file_path)
            file_paths.remove(file_path)

    # Prepare scene to get its new stuff
    item_group = get_import_group(item)
    item_namespace = get_import_namespace(item)

    # Import files
    for file_path in file_paths:
        progress_dialog.setText("Importing: " + os.path.basename(file_path))
        progress_dialog.increment()

        # Get options
        options = get_format_options(file_path, import_options)
        lock_transform = get_option("lock_transformations", options, True)
        as_reference = get_option("as_reference", options, False)
        no_root_shape = get_option("no_root_shape", options, False)
        create_namespace = get_option("create_namespace", options, True)

        ns = item_namespace
        if not create_namespace:
            ns = ""

        new_nodes = import_file(file_path, as_reference, lock_transform, no_root_shape, item, ns, item_group, step)
        geo_nodes = geo_nodes + new_nodes

    # Import shaders
    for shader_file in shader_files:
        progress_dialog.setText("Importing: " + os.path.basename(file_path))
        progress_dialog.increment()

        # Get options
        options = get_format_options(shader_file, import_options)
        as_reference = get_option("as_reference", options, False)
        no_root_shape = get_option("no_root_shape", options, False)
        create_namespace = get_option("create_namespace", options, True)

        ns = item_namespace
        if not create_namespace:
            ns = ""

        new_nodes = import_file(shader_file, as_reference, False, no_root_shape, item, ns, item_group, step)
        # Apply shaders to the geo nodes
        # Get the shaders
        shaders = []
        for node in new_nodes:
            # get meshes
            meshes = cmds.listRelatives(node, ad=True, f=True, type='mesh')
            for mesh in meshes:
                node_history = cmds.listHistory( mesh, f=True )
                shading_engines = cmds.listConnections( node_history, type='shadingEngine')
                if shading_engines:
                    shaders = shaders + shading_engines
            # Rename the node
            cmds.rename(node, Node(node).name() + "_shaders")
        # Assign!
        ram.log("Got shaders:\n" + "\n> ".join(shaders), ram.LogLevel.Debug)
        apply_shaders(shaders, geo_nodes)

    progress_dialog.close()

def get_import_group( item ):
    """Gets or creates a maya group where to import the item"""
    # Get info
    item_short_name = item.shortName()
    item_name = item.name()
    item_type = item.itemType()
    item_group_name = item.group()

    # Get the Asset Group
    item_group = ''
    if item_type == ram.ItemType.ASSET:
        item_group = 'RamASSETS_' + item_group_name
    elif item_type == ram.ItemType.SHOT:
        item_group = 'RamSHOTS'
    else:
        item_group = 'RamITEMS'

    item_group = Node.get_create_group( item_group )

    # Check if the short name is not made only of numbers
    regex = re.compile('^\\d+$')
    if re.match(regex, item_short_name):
        item_short_name = item_type + item_short_name

    # Get the Item Group
    return Node.get_create_group( item_short_name + '_' + item_name, item_group )

def get_import_namespace( item ):
    """Generates the new namespace for importing the item"""
    # Get/Generate the namespace
    item_short_name = item.shortName()
    item_type = item.itemType()
    # Check if the short name is not made only of numbers
    regex = re.compile('^\\d+$')
    if re.match(regex, item_short_name):
        item_short_name = item_type + item_short_name
    # And the namespace + its number
    """ import_namespace = item_short_name + '_001'
    i = 1
    while cmds.namespace( exists=import_namespace ):
        i = i+1
        i_str = str(i)
        while len(i_str) < 3:
            i_str = '0' + i_str
        import_namespace = item_short_name + '_'  + i_str"""

    return item_short_name

def import_file(file_path, as_reference, lock_transform, no_root_shape, item, item_namespace, item_group, step):
    """Imports the items in the file"""
    ram.log("Importing: " + file_path, ram.LogLevel.Debug)
    # Check the extension to load needed plugins
    ext = file_path.split('.')[-1]
    # Load alembic
    if ext == 'abc':
        # We may need to use alembic
        if Plugin.load("AbcImport"):
            ram.log("I have loaded the Alembic Export plugin, needed for the current task.")

    new_nodes = ()
    if as_reference:
        if item_namespace != "":
            new_nodes = cmds.file(
                file_path,
                r=True,
                ignoreVersion=True,
                mergeNamespacesOnClash=True,
                returnNewNodes=True,
                ns=item_namespace
                )
        else:
            new_nodes = cmds.file(
                file_path,
                r=True,
                ignoreVersion=True,
                returnNewNodes=True,
                defaultNamespace=True
                )
    else:
        if item_namespace != "":
            new_nodes = cmds.file (
                file_path,
                i=True,
                ignoreVersion=True,
                mergeNamespacesOnClash=True,
                returnNewNodes=True,
                ns=item_namespace,
                preserveReferences=True
                )
        else:
            new_nodes = cmds.file (
                file_path,
                i=True,
                ignoreVersion=True,
                returnNewNodes=True,
                preserveReferences=True,
                defaultNamespace=True
                )

    # Get root to create control, and move into its group
    root_nodes = []
    for node in new_nodes:
        # When parenting the roots, children won't exist anymore
        if not cmds.objExists(node):
            continue

        node = Node(node)

        # not a root
        if node.has_parent():
            continue
        # not a transform node
        if not node.is_transform():
            continue

        # Create root control
        rootName = node.name() + '_' + step.shortName()
        ctrl = node.create_root_controller( rootName, no_root_shape)
        # Set its color
        ctrl_shape = cmds.listRelatives(ctrl.path(), shapes=True, f=True, type='nurbsCurve')
        if (ctrl_shape):
            ctrl_shape = ctrl_shape[0]
            cmds.setAttr(ctrl_shape+'.overrideEnabled', 1)
            cmds.setAttr(ctrl_shape+'.overrideColor', 18)

        cmds.setAttr(ctrl.path()+'.useOutlinerColor',1)
        color = step.color()
        cmds.setAttr( ctrl.path()+'.outlinerColor', color[0]/255, color[1]/255, color[2]/255 )

        # Parent to the item group
        ctrl.parent_to( item_group )

        # Store Ramses Data!
        set_import_attributes( ctrl, item, step, file_path )

        # Lock transform except ramses managed children
        if  not as_reference and lock_transform:
            children = cmds.listRelatives(ctrl.path(), ad=True, f=True, type='transform')
            if children:
                for child in children:
                    if not is_ramses_managed(child):
                        child = Node(child)
                        child.lock_transform(True)

        root_nodes.append(ctrl.path())

    return root_nodes

def apply_shaders(shaders, geo_nodes, ignore_namespace=True):
    """Applies the shaders to the nodes, using the name of the objects stored by Ramses"""
    ram.log("Applying shaders to geometry")
    for node in geo_nodes:
        ram.log("checking: " + node, ram.LogLevel.Debug)
        # For all mesh
        meshes = cmds.listRelatives( node, ad=True, type='mesh', f=True)
        if meshes is None:
            ram.log("> No mesh in this node!", ram.LogLevel.Debug)
            continue
        for mesh in meshes:
            # Get the transform node (which has the name we're looking for)
            transform_node = cmds.listRelatives(mesh, p=True, type='transform')[0]
            transform_node = Node(transform_node)
            name = transform_node.name(keep_namespace = not ignore_namespace)
            # Look for a shader
            for shader in shaders:
                shaded_objects = get_ramses_attr(shader, RamsesAttribute.SHADED_OBJECTS)
                if shaded_objects is None:
                    continue
                shaded_objects = shaded_objects.split(",")
                if name in shaded_objects:
                    ram.log("> Found this shader " + shader + "\n> for " + name, ram.LogLevel.Debug)
                    cmds.sets(mesh, e=True, forceElement=shader)

def get_format_options( file_path, options ):
    """Returns the options for the given file"""
    ext = os.path.splitext(file_path)[1]
    if ext != "":
        ext = ext[1:]
    ram.log("Checking options for format: " + ext, ram.LogLevel.Debug)
    for o in options["formats"]:
        if o['format'] == ext:
            ram.log("Found these options:", ram.LogLevel.Debug)
            ram.log(o, ram.LogLevel.Debug)
            return o

    # Find default
    for o in options["formats"]:
        if o['format'] == "*":
            return o

    return { 'format': "*" }