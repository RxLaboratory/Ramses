bl_info = {
    "name": "Ramses Versionning",
    "category": "Pipeline",
    "blender": (2, 80, 0),
    "author": "Nicolas Dufresne",
    "location": "Menu > File",
    "version": (1, 0, 0),
    "blender": (2, 80, 0),
    "description": "Saving and versionning tools."
}

import bpy
from pathlib import Path
import shutil

ops  = bpy.ops
utils = bpy.utils
types = bpy.types

def showMessageBox( message = "", title = "Message Box", icon = 'INFO'):
    """Displays a simple message box"""
    def draw(self, context):
        self.layout.alert = True
        self.layout.label(text = message)
    bpy.context.window_manager.popup_menu(draw, title = title, icon = icon)

def ramLog( log = ""):
    """Logs Ramses activity"""
    print("Ramses: " + log)

def convertIntToString( integer , numChars = 3 ):
    """Converts an integer to a string with leading zeroes"""
    numString = str(integer)
    while len(numString) < numChars:
        numString = "0" + numString
    return numString

def ramSave( context, increment = False, publish = False ):
    """Performs Ramses saving operations (standard, incremental, publish)"""

    ramLog(log='Saving scene...')

    # get current scene path
    currentFile = Path(context.blend_data.filepath)
    currentFolder = currentFile.parent
    sceneName = currentFile.stem

    if sceneName == '':
        ramLog(log='This scene has never been saved yet.')
        #asks the user to save as
        showMessageBox(message="Save scene at least once first.", title="Not saved!", icon='ERROR')
        return {'CANCELLED'}

    # Save
    ops.wm.save_mainfile( filepath = context.blend_data.filepath, check_existing=False )
    ramLog(log = 'Saved "' +  sceneName + '.blend"')

    # Save as WIP (if not already WIP)
    if not sceneName.endswith("_WIP"):
        wipName = sceneName + "_WIP.blend"
        wipFile = Path( currentFolder / wipName )
        wipPath = wipFile.as_posix()
        ops.wm.save_as_mainfile( filepath = wipPath, check_existing=False )
        currentFile = Path(context.blend_data.filepath)
        currentFolder = currentFile.parent
        ramLog(log = 'Saved "' +  wipName + '"')
    # Remove WiP from scene name
    else:
        sceneName = sceneName[0:-4]

    # Copy version

    # get or create versions folder
    versionsFolder = currentFolder / '_versions'
    if not versionsFolder.exists():
        versionsFolder.mkdir()

    # get existing versions
    currentVersion = 0
    for f in versionsFolder.iterdir():
        if f.is_file() and f.stem.startswith(sceneName):
            versionName = f.stem
            versionString = versionName.replace(sceneName, '')
            versionString = versionString.replace('_wip','')
            versionString = versionString.replace('_pub','')
            if versionString.isdecimal():
                version = int(versionString)
                if version > currentVersion:
                    currentVersion = version

    # increment
    if currentVersion == 0 or increment or publish:
        currentVersion += 1

    # copy
    versionFileName = sceneName
    if publish:
        versionFileName = versionFileName + "_pub"
    else:
        versionFileName = versionFileName + "_wip"
    versionFileName = versionFileName + convertIntToString( integer = currentVersion ) + ".blend"
    versionFile = versionsFolder / versionFileName
    shutil.copy2( currentFile, versionFile )
    ramLog(log = 'Backed up version "' +  versionFileName + '"')

    # Publish

    if publish:
        publishFileName = sceneName + ".blend"
        publishFile = currentFolder / publishFileName
        shutil.copy2( currentFile, publishFile )
        ramLog(log = 'Published "' +  versionFileName + '"')   

    return {'FINISHED'}

class RAMSES_OT_Save(types.Operator):
    """Saves the current version of the scene"""
    bl_idname = "wm.ramses_save"
    bl_label = "Ramses: Save"
    bl_options = {'REGISTER'}

    def execute(self, context):
        return ramSave(context)

class RAMSES_OT_IncrementalSave(types.Operator):
    """Saves and increments a new version of the scene"""
    bl_idname = "wm.ramses_incremental_save"
    bl_label = "Ramses: Save Incremental"
    bl_options = {'REGISTER'}

    def execute(self, context):
        return ramSave(context, increment = True)

class RAMSES_OT_Publish(types.Operator):
    """Publishes a final version of the scene"""
    bl_idname = "wm.ramses_publish"
    bl_label = "Ramses: Publish"
    bl_options = {'REGISTER'}

    def execute(self, context):
        return ramSave(context, publish = True)

def menu_func(self, context):
    self.layout.separator()
    self.layout.operator(RAMSES_OT_Save.bl_idname,  text="Ramses: Save")
    self.layout.operator(RAMSES_OT_IncrementalSave.bl_idname,  text="Ramses: Save new version")
    self.layout.operator(RAMSES_OT_Publish.bl_idname,  text="Ramses: Publish")

def addKeyMap(name, idname, key, ctrl = False, alt = False, shift = False):
    wm = bpy.context.window_manager
    km = wm.keyconfigs.addon.keymaps.new( name=name, space_type='EMPTY' )
    kmi = km.keymap_items.new(idname, key, 'PRESS', ctrl=ctrl, shift = shift, alt = alt)
    ramsesVersionning_keymaps.append((km, kmi))


classes = (
    RAMSES_OT_Save,
    RAMSES_OT_IncrementalSave,
    RAMSES_OT_Publish
)

ramsesVersionning_keymaps = []

def register():
    # register
    for cls in classes:
        utils.register_class(cls)
    # menu
    types.TOPBAR_MT_file.append(menu_func)
    # keymaps
    wm = bpy.context.window_manager
    # check if keyconfigs is available (not in background)
    kc = wm.keyconfigs.addon
    if kc:
        addKeyMap('Ramses: Save', RAMSES_OT_Save.bl_idname, 'S', ctrl = True)
        addKeyMap('Ramses: Incremental Save', RAMSES_OT_IncrementalSave.bl_idname, 'S', ctrl = True, alt = True)
        addKeyMap('Ramses: Publish', RAMSES_OT_Publish.bl_idname, 'S', alt=True, ctrl=True,shift=True )       

def unregister():
    # unregister
    for cls in reversed(classes):
        utils.unregister_class(cls)
    # menu
    #types.TOPBAR_MT_file.remove(menu_func)
    #keymaps
    for km, kmi in ramsesVersionning_keymaps:
        km.keymap_items.remove(kmi)
    ramsesVersionning_keymaps.clear()

if __name__ == "__main__":
    register()