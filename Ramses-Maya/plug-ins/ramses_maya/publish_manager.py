# -*- coding: utf-8 -*-
"""The entry point for publishing scenes"""

import os
import yaml
import ramses as ram
import dumaf as maf
from maya import cmds # pylint: disable=import-error
from .ui_publish import PublishDialog
from .utils_nodes import (
    get_del_on_publish_nodes,
    get_publish_nodes,
    delete_ramses_sets
)
from .utils import end_process
from .utils_attributes import (
    RamsesAttribute,
    set_ramses_managed,
    get_ramses_attr,
    set_ramses_attr
)
from .utils_options import (
    get_option
)

def get_publish_file_path(publish_info, extension, name):
    """Gets the path for publishing the file"""
    scene_info = publish_info.copy()
    scene_info.version = -1
    scene_info.state = ''
    scene_info.extension = extension
    if scene_info.resource != '':
        scene_info.resource = scene_info.resource + '-'
    scene_info.resource = scene_info.resource + name

    return scene_info.filePath()

def set_export_metadata( filepath, publish_info ):
    """Sets the metadata for the published file"""
    #pipeType = pipeType.split('-')[-1]
    #☺ram.RamMetaDataManager.setPipeType( filePath, pipeType )
    ram.RamMetaDataManager.setVersion( filepath, publish_info.version )
    ram.RamMetaDataManager.setState( filepath, publish_info.state )
    ram.RamMetaDataManager.setResource( filepath, publish_info.resource )

def publisher(item, step, file_path, publish_options=None, show_publish_options=False):
    """The publish entry point"""

    # Get options
    publish_nodes = ()
    if not publish_options:
        publish_options_str = step.publishSettings()
        if publish_options_str != "":
            publish_options = yaml.safe_load( publish_options_str )

    # Check if we need to show the publish dialog
    publish_dialog = PublishDialog()
    if not publish_options:
        publish_options = {}
    if "formats" not in publish_options or len(publish_options["formats"]) == 0 or show_publish_options:
        # Get the nodes
        nodes = get_publish_nodes()
        if len(nodes) == 0:
            return
        publish_dialog.load_nodes(nodes)
        publish_dialog.set_options(publish_options)
        if not publish_dialog.exec_():
            return
        publish_options = publish_dialog.get_options()
        publish_nodes = publish_dialog.get_nodes()
    else:
        # Prepare the publish nodes array with node names
        nodes = get_publish_nodes()
        publish_nodes = []
        for node in nodes:
            maf_node = maf.Node(node)
            node_name = maf_node.name().replace("_", " ")
            publish_nodes.append((node, node_name))

    # Backup file
    publish_info = ram.RamFileManager.getPublishInfo( file_path )
    # Prepare the file for backup in the published folder
    currentFilePath = cmds.file( q=True, sn=True )
    backup_info = publish_info.copy()
    backup_info.version = -1
    backup_info.state = ''
    # Save
    published_filepath = backup_info.filePath()
    cmds.file( rename = published_filepath )
    cmds.file( save=True, options="v=1;" )
    ram.RamMetaDataManager.appendHistoryDate( published_filepath )
    # Reopen initial file
    cmds.file(currentFilePath,o=True,f=True)
    ram.RamMetaDataManager.setVersion( published_filepath, publish_info.version )

    if "formats" not in publish_options or len(publish_options["formats"]) == 0:
        return

    if len(publish_nodes) == 0:
        return

    ram.log("Publishing " + str(item) + " for " + str(step) + " in:\n" + os.path.dirname( publish_info.filePath()), ram.LogLevel.Info)
    ram.log("-> These are the publish options:\n\n" + yaml.dump(publish_options), ram.LogLevel.Info )

    # Show progress
    progress_dialog = maf.ProgressDialog()
    progress_dialog.show()
    progress_dialog.setText("Publishing...")

    # Prepare the scene
    temp_data = maf.Scene.createTempScene()

    # Remove nodes to del on publish (if not in publish!)
    del_nodes = get_del_on_publish_nodes()
    for node in del_nodes:
        nodeSets = cmds.listSets(object=node)
        if nodeSets:
            if "Ramses_Publish" in nodeSets:
                continue
        node = maf.Node(node)
        node.remove()
    
    # Scene pre-processing

    progress_dialog.setText("Cleaning scene...")

    # Import references
    if get_option("import_references", publish_options, True):
        maf.Reference.importAll()
    # Remove namespaces
    if get_option("remove_namespaces", publish_options, True):
        maf.Namespace.removeAll()
    # Remove animation
    if get_option("remove_animation", publish_options, False):
        maf.animation.removeAll()

    # Remove Ramses Maya Sets
    delete_ramses_sets()

    progress_dialog.setMaximum(len(publish_nodes) + 1)
    progress_dialog.setText("Publishing nodes...")
    progress_dialog.increment()

    # Publish each node
    for node in reversed(publish_nodes):
        progress_dialog.setText("Publishing " + node[1] + "...")
        progress_dialog.increment()
        ram.log("Publishing " + node[1] + "...")
        publish_node(node, publish_options, publish_info)

    end_process(temp_data, progress_dialog)
    ram.log("Successful publish, Yay!")

def publish_node( published_node, publish_options, publish_info ):
    """Publishes a specific node"""
    node = maf.Node(published_node[0])

    # Move to center of the scene
    node.move_to_zero()

    # Remove hidden
    if get_option("remove_hidden_nodes", publish_options, True):
        node.remove_hidden_children()

    # Types
    if "types" in publish_options:
        types_options = publish_options["types"]
        if "list" in types_options:
            types_list = types_options["list"]
            if len(types_list) > 0:
                if get_option("mode", types_options, "remove") == "remove":
                    node.remove_types(types_list)
                else:
                    node.keep_types(types_list)

    # Extra shapes
    if get_option("remove_extra_shapes", publish_options, False):
        node.remove_extra_shapes()

    # Delete history
    if get_option("delete_history", publish_options, False):
        node.delete_history()

    # Remove empty
    if get_option("remove_empty_groups", publish_options, False):
        node.remove_empty()

    # Freeze transform
    if "freeze_transform" in publish_options:
        case_sensitive = get_option("case_sensitive", publish_options["freeze_transform"], False)
        whitelist = get_option("whitelist", publish_options["freeze_transform"], ())
        freeze = True
        children = node.children(transform_only=True)
        children.append(node)
        for child in children:
            child = maf.Node(child)
            child_name = child.name()
            for name in whitelist:
                if case_sensitive:
                    name = name.lower()
                    child_name = child_name.lower()
                if name in child_name:
                    freeze = False
                    break
            if freeze:
                child.freeze_transform()

    # And publish types!
    for frmt in publish_options["formats"]:
        # Detect format
        maya_scene = False
        maya_shaders = False
        maya_extension = "mb"
        alembic = False
        ass = False

        if frmt == "ma" or frmt == "mb":
            maya_scene = True
            maya_extension = frmt
        elif "ma" in frmt or "mb" in frmt:
            if "ma" in frmt:
                frmt = frmt["ma"]
                maya_extension = "ma"
            else:
                frmt = frmt["mb"]
                maya_extension = "mb"
            maya_shaders = get_option("only_shaders", frmt, False)
            maya_scene = not maya_shaders
        elif frmt == "abc":
            alembic = True
        elif "abc" in frmt:
            alembic = True
            frmt = frmt["abc"]
        elif frmt == "ass" or "ass" in frmt:
            ass = True

        if maya_scene:
            publish_maya_scene(node, frmt, maya_extension, publish_info, published_node[1])
        elif maya_shaders:
            publish_maya_shaders(node, frmt, maya_extension, publish_info, published_node[1] + "_shaders")
        elif alembic:
            publish_alembic(node, frmt, publish_info, published_node[1])
        elif ass:
            publish_ass(node, frmt, publish_info, published_node[1])

def publish_maya_scene(node, options, extension, publish_info, name):
    """Publishes the node as a maya scene"""
    # manage joints
    joints_mode = get_option("joints", options, "disable")
    if joints_mode in ("disable", "lock", "hide"):
        joints = cmds.ls(type='joint')
        if joints is not None:
            for joint in joints:
                if joints_mode in ("hide", "lock"):
                    cmds.setAttr( joint + '.visibility', False )
                    if joints_mode == "lock":
                        cmds.setAttr(joint + '.visibility', lock=True)
                else:
                    cmds.setAttr( joint + '.drawStyle', 2 )

    # Lock hidden
    if get_option("lock_hidden_nodes", options, True):
        node.lock_visibility(True, lock_children=True, only_hidden=True)

    if get_option("lock_transformations", options, True):
        node.lock_transform(True, lock_children=True)

    # Get path and save
    file_path = get_publish_file_path( publish_info, extension, name )
    cmds.select(clear=True)
    node.select()
    maya_type = 'mayaBinary'
    if extension == 'ma':
        maya_type = 'mayaAscii'
    cmds.file( rename=file_path )
    cmds.file( exportSelected=True, options="v=1;", typ=maya_type)
    set_export_metadata( file_path, publish_info)

def publish_maya_shaders(node, options, extension, publish_info, name):
    """Publishes the shaders as a maya file"""

    # If there's no mesh, nothing to do
    meshes = node.meshes()
    if len(meshes) == 0:
        return

    # Prepare the data info we're exporting
    all_shading_engines = []

    # Get shading info
    for mesh in meshes:
        node_history = cmds.listHistory( mesh, f=True )
        node_shading_engines = cmds.listConnections( node_history, type='shadingEngine')

        # Get the name from parent (transform node for this mesh)
        object_name = cmds.listRelatives(mesh, p=True)[0]
        # Remove namespace if any
        object_name = object_name.split(':')[-1]

        if node_shading_engines is not None:
            for shading_engine in node_shading_engines:
                # Get the first surface shader to rename the engine
                try:
                    surface_shaders = cmds.listConnections(shading_engine + '.surfaceShader')
                    if surface_shaders:
                        surface_shader = surface_shaders[0]
                        surface_shader_name = surface_shader.split(':')[-1]
                        # Rename
                        if shading_engine != 'initialShadingGroup':
                            shading_engine = cmds.rename( shading_engine, surface_shader_name + "_Engine")

                        # List the objects this engine is shading
                        set_ramses_managed( shading_engine )
                        object_names = get_ramses_attr( shading_engine, RamsesAttribute.SHADED_OBJECTS )
                        if object_names is None:
                            object_names = ''
                        else:
                            object_names = object_names + ','
                        object_names = object_names + object_name
                        set_ramses_attr( shading_engine, RamsesAttribute.SHADED_OBJECTS, object_names, 'string')
                except: # pylint: disable=bare-except
                    pass

                all_shading_engines.append(shading_engine)

    if len(all_shading_engines) == 0:
        ram.log("Sorry, I did not find any shader to publish...", ram.LogLevel.Info)
        return

    # Select and export shadingEngines
    # Get path and save
    file_path = get_publish_file_path( publish_info, extension, name + "-shaders" )

    cmds.select(clear=True)
    spheres = []
    offset = 0
    for shading_engine in all_shading_engines:
        shading_engine = maf.Node(shading_engine)
        # create a sphere per shader and export that
        sphere = cmds.polySphere(name=shading_engine.name().replace("_Engine","") + "_shader", constructionHistory=False)[0]
        # Assign shader
        cmds.sets(sphere, e=True, forceElement=shading_engine.path())
        # Move on the X axis
        cmds.setAttr(sphere + ".translateX", offset)
        offset = offset + 2
        spheres.append(sphere)
    root_group = cmds.group(spheres, name=name)
    cmds.select(root_group)

    maya_type = 'mayaBinary'
    if extension == 'ma':
        maya_type = 'mayaAscii'
    cmds.file( rename=file_path )
    cmds.file( exportSelected=True, options="v=1;", typ=maya_type)
    set_export_metadata( file_path, publish_info)

def publish_alembic(node, options, publish_info, name):
    """Publishes the node as alembic"""
    # We need ABC Export, of course
    maf.Plugin.load("AbcExport")

    file_path = get_publish_file_path( publish_info, 'abc', name )

    # Collect options
    in_frame = 1
    out_frame = 1
    frame_step = 1.0
    if "animation" in options:
        in_frame = int(cmds.playbackOptions(q=True,ast=True))
        out_frame = int(cmds.playbackOptions(q=True,aet=True))
        handle_in = get_option("handle_in", options["animation"], 0)
        handle_out = get_option("handle_out", options["animation"], 0)
        in_frame = in_frame - handle_in
        out_frame = out_frame - handle_out
        frame_step = get_option("frame_step", options["animation"], 1.0)

    filter_euler = ''
    if get_option("filter_euler_rotations", options, True):
        filter_euler = '-eulerFilter'

    renderable = ''
    if get_option("renderable_only", options, True):
        renderable = '-renderableOnly'

    abc_options = ' '.join([
        '-frameRange ' + str(in_frame) + ' ' + str(out_frame),
        filter_euler,
        '-step ' + str(frame_step),
        '-autoSubd', # crease
        '-uvWrite',
        '-writeUVSets',
        '-worldSpace',
        '-writeVisibility',
        '-dataFormat hdf',
        renderable,
        '-root ' + node.path(),
        '-file "' + file_path + '"',
    ])

    ram.log("These are the alembic options:\n" + abc_options, ram.LogLevel.Info)

    # Export
    cmds.AbcExport(j=abc_options)
    # Meta data
    set_export_metadata( file_path, publish_info)

def publish_ass(node, options, publish_info, name):
    """Publishes the node as an arnold scene source"""
    # We need Arnold, of course
    maf.Plugin.load('mtoa')

    node.select()
    file_path = get_publish_file_path( publish_info, 'ass', name )

    cmds.arnoldExportAss(f=file_path, s=True, mask=223, lightLinks=0, shadowLinks=0, cam="perspShape" )
    set_export_metadata( file_path, publish_info)
