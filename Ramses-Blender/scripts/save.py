bl_info = {
    "name": "Ramses Versionning",
    "category": "Pipeline"
}

import bpy
from pathlib import Path
import shutil

class RamsesVersionning(bpy.types.Operator):
    """Ramses pipeline tools"""
    bl_idname = "pipeline.ramses_versionning"
    bl_label = "Ramses versionning"
    bl_options = {'REGISTER'}

    increment = bpy.props.BoolProperty(name="Increment", description="Increments the version number", default=False, options={'HIDDEN'}, subtype='NONE', update=None, get=None, set=None)
    publish = bpy.props.BoolProperty(name="Publish", description="Publishes the current version", default=False, options={'HIDDEN'}, subtype='NONE', update=None, get=None, set=None)

    def ShowMessageBox(self, message = "", title = "Message Box", icon = 'INFO'):
        """Displays a simple message box"""
        def draw(self, context):
            self.layout.alert = True
            self.layout.label(text = message)
        bpy.context.window_manager.popup_menu(draw, title = title, icon = icon)

    def RamsesLog(self, log = ""):
        """Logs Ramses activity"""
        print("Ramses: " + log)


    def convertIntToString( self, integer , numChars = 3 ):
        """Converts an integer to a string with leading zeroes"""
        numString = str(integer)
        while len(numString) < numChars:
            numString = "0" + numString
        return numString

    def execute(self, context):
        ops  = bpy.ops

        self.RamsesLog(log='Saving scene...')

        # get current scene path
        currentFile = Path(context.blend_data.filepath)
        currentPath = currentFile.parent
        sceneName = currentFile.stem

        if sceneName == '':
            self.RamsesLog(log='This scene has never been saved yet.')
            #asks the user to save as
            self.ShowMessageBox(message="Save scene at least once first.", title="Not saved!", icon='ERROR')
            return {'CANCELLED'}

        # Save
        ops.wm.save_mainfile( filepath = context.blend_data.filepath, check_existing=False )
        self.RamsesLog(log = 'Saved "' +  sceneName + '.blend"')

        # Save as WIP (if not already WIP)
        if not sceneName.endswith("_WIP"):
            wipName = sceneName + "_WIP.blend"
            wipFile = Path( currentPath / wipName )
            wipPath = wipFile.as_posix()
            ops.wm.save_as_mainfile( filepath = wipPath, check_existing=False )
            currentFile = Path(context.blend_data.filepath)
            currentPath = currentFile.parent
            self.RamsesLog(log = 'Saved "' +  wipName + '"')
        # Remove WiP from scene name
        else:
            sceneName = sceneName[0:-4]

        # Save version

        # get or create versions folder
        versionsFolder = currentPath / '_versions'
        if not versionsFolder.exists():
            versionsFolder.mkdir()

        # get existing versions
        currentVersion = 0
        for f in versionsFolder.iterdir():
            if f.is_file() and f.stem.startswith(sceneName):
                versionName = f.stem
                versionString = versionName.replace(sceneName, '')
                versionString = versionString.replace('_wip','')
                if versionString.isdecimal():
                    version = int(versionString)
                    if version > currentVersion:
                        currentVersion = version

        # increment
        if currentVersion == 0 or self.increment:
            currentVersion += 1

        # copy
        versionFileName = sceneName + "_wip" + self.convertIntToString( integer = currentVersion ) + ".blend"
        versionFile = versionsFolder / versionFileName
        shutil.copy2( currentFile, versionFile )
        self.RamsesLog(log = 'Backed up version "' +  versionFileName + '"')

        return {'FINISHED'}

def menu_func(self, context):
    self.layout.operator(RamsesVersionning.bl_idname,  text="Ramses: Save")
    saveInc = self.layout.operator(RamsesVersionning.bl_idname,  text="Ramses: Save new version")
    saveInc.increment = True
    publish = self.layout.operator(RamsesVersionning.bl_idname,  text="Ramses: Publish")
    publish.publish = True

def register():
    bpy.utils.register_class(RamsesVersionning)
    bpy.types.TOPBAR_MT_file.append(menu_func)

def unregister():
    bpy.utils.unregister_class(RamsesVersionning)
    bpy.types.TOPBAR_MT_file.remove(menu_func)

if __name__ == "__main__":
    register()