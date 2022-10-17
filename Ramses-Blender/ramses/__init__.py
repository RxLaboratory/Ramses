bl_info = {
    "name": "Ramses Versionning",
    "category": "Pipeline",
    "blender": (2, 80, 0),
    "author": "Nicolas Dufresne",
    "location": "Menu > File",
    "version": (1, 1, 0),
    "description": "Saving and versionning tools."
}

import bpy # pylint: disable=import-error
from pathlib import Path
import shutil, os, time

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

def clean_file():
    # Screens
    for screen in bpy.data.screens:
        name = screen.name
        end_name = name.split(".")[-1]
        try:
            index = int(end_name)
        except:
            index = 0
        
        if index > 0 :
            bpy.data.screens[name].user_clear()

    # Purge Orphans
    bpy.ops.outliner.orphans_purge()

class RAMSES_Preferences( bpy.types.AddonPreferences ):
    # this must match the add-on name, use '__package__'
    # when defining this in a submodule of a python package.
    bl_idname = __name__

    increment_timeout: bpy.props.IntProperty(
        name="Incremental save time out (mn)",
        description="Trigger an incrementation of the file when saving every X minutes",
        default=120,
        subtype='TIME'
    )

    def draw(self, context):
        layout = self.layout
        layout.prop(self, "increment_timeout")

class RAMSES_OT_Save(types.Operator):
    """Saves the current version of the scene"""
    bl_idname = "wm.ramses_save"
    bl_label = "Ramses: Save"
    bl_options = {'REGISTER'}

    mode: bpy.props.StringProperty(default = 'NORMAL')

    def execute(self, context):
        """Performs Ramses saving operations (standard, incremental, publish)"""

        increment = self.mode == 'INCREMENT'
        publish = self.mode == 'PUBLISH'

        preferences = context.preferences.addons[__name__].preferences

                # if normal save and scene is too old, increment it anyway
        if not increment and not publish:
            modified = os.path.getmtime(context.blend_data.filepath)
            now = time.time()
            if preferences.increment_timeout * 60 < now - modified:
                increment = True

        ramLog(log='Saving scene...')

        # get current scene path
        currentFile = Path(context.blend_data.filepath)
        currentFolder = currentFile.parent
        sceneName = currentFile.stem

        if sceneName == '':
            ramLog(log='This scene has never been saved yet.')
            #asks the user to save as
            self.report({'ERROR'}, "Not Saved! Save this scene first using the \"save as\" command [CTRL + SHIFT + S]")
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
        if increment:
            self.report({'INFO'},"New version saved: " + str(currentVersion))

        # Publish

        if publish:
            clean_file()
            
            publishFileName = sceneName + ".blend"
            publishFile = currentFolder / publishFileName
            shutil.copy2( currentFile, publishFile )
            ramLog(log = 'Published "' +  versionFileName + '"')
            self.report({'INFO'},"Scene published! The version is: " + str(currentVersion))

        return {'FINISHED'}

def menu_func(self, context):
    self.layout.separator()
    self.layout.operator(RAMSES_OT_Save.bl_idname,  text="Ramses: Save").mode = 'NORMAL'
    self.layout.operator(RAMSES_OT_Save.bl_idname,  text="Ramses: Save new version").mode = 'INCREMENT'
    self.layout.operator(RAMSES_OT_Save.bl_idname,  text="Ramses: Publish").mode = 'PUBLISH'



classes = (
    RAMSES_Preferences,
    RAMSES_OT_Save,
)

def register():
    # register
    for cls in classes:
        utils.register_class(cls)
    # menu
    types.TOPBAR_MT_file.append(menu_func)

def unregister():
    # unregister
    for cls in reversed(classes):
        utils.unregister_class(cls)
    # menu
    #types.TOPBAR_MT_file.remove(menu_func)


if __name__ == "__main__":
    register()