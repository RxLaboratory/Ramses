# Scripting API reference

[*Rainbox Laboratory*](https://rainboxlab.org) provides a reference implementation of the API in *Python* and *Adobe ExtendScript*.

Other APIs may be developped, in which case all these APIs on all programming/scripting language should implement this reference, with the same classes, attributes and methods.

[TOC]

As this document is referring to different scripting languages, it uses general terms which may be different than what is actually used in specific languages.

*(TODO: ADD THE LINK TO THE GITHUB REPOS OF THE APIs / LINK TO GET THEM ON RXLAB.ORG)*

## Classes

This is the comprehensive list of available classes.

| Class | Inheritance | Description |
| ---- | ---- | ---- |
| [Ramses](ramses.md) | | The main class. One (and only one) instance globally available, instantiated during init time. |
| [RamAsset](ram_asset.md) | [RamItem](ram_item.md) | An asset. |
| [RamDaemonInterface](ram_daemon_interface.md) | | The Class used to communicate with the Ramses Daemon |
| [RamFileManager](ram_file_manager.md) | | Generic tools to handle files and the [Ramses Naming Scheme](../../files/naming.md) |
| [RamFileType](ram_filetype.md) | | A File type (name and possible extensions) |
| [RamItem](ram_item.md) | [RamObject](ram_object.md) | The base class for Shots and Assets. |
| [RamObject](ram_object.md) | | The base class for most of Ramses objects. |
| [RamPipe](ram_pipe.md) | | A pipe which connects two steps together in the production pipeline |
| [RamProject](ram_project.md) | [RamObject](ram_object.md) | A project handled by Ramses. Projects contains general items, assets and shots. |
| [RamSettings](ram_settings.md) | | Gets and saves settings used by Ramses. | 
| [RamShot](ram_shot.md) | [RamItem](ram_item.md) | A shot. |
| [RamState](ram_state.md) | [RamObject](ram_object.md) | The state of a step (like "Waiting for approval", "Approved", etc.). |
| [RamStatus](ram_status.md) | | A state associated to a comment, the user who changed the state, etc. |
| [RamStep](ram_step.md) | [RamObject](ram_object.md) | A step in the production of the shots or assets of the project. |
| [RamUser](ram_user.md) | [RamObject](ram_object.md) | The class representing users. |

## Examples

Here are a few examples showing how this API can be used.

### Save command for *Maya* (Python)

This example shows how to use the [RamFileManager](ram_file_manager.md) class to implement a save command in *Maya* with automatic backup in the *_versions* subfolder.

```python
import maya.cmds as cmds
import ramses as ram

ramses = ram.Ramses.instance()
settings = ram.RamSettings.instance()

def ramSave():
    # The current maya file
    currentFilePath = cmds.file( q=True, sn=True )
    ram.log("Saving file: " + currentFilePath)

    # Check if the Daemon is available, if Ramses is set to be used "online"
    if settings.online:
        if not ramses.connect():
            cmds.confirmDialog(
                title="No User",
                message="You must log in Ramses first!",
                button=["OK"],
                icon="warning"
                )
            ramses.showClient()
            cmds.error( "User not available: You must log in Ramses first!" )
            return

    # Get the save path (Ramses will check if the current file has to be renamed to respect the Ramses Tree and Naming Scheme)
    saveFilePath = ram.RamFileManager.getSaveFilePath( currentFilePath )
    if not saveFilePath: # Ramses may return None if the current file name does not respeect the Ramses Naming Scheme
        cmds.warning( ram.Log.MalformedName )
        # Set file to be renamed
        cmds.file( renameToSave = True )
        cmds.inViewMessage( msg='Malformed Ramses file name! <hl>Please save with a correct name first</hl>.', pos='midCenter', fade=True )
        return

    # If the current Maya file is inside a preview/publish/version subfolder, we're going to increment
    # to be sure not to lose the previous working file.
    increment = False
    if ram.RamFileManager.inReservedFolder( currentFilePath ):
        increment = True
        cmds.warning( "Incremented and Saved as " + saveFilePath )

    # Set the save name and save
    cmds.file( rename = saveFilePath )
    cmds.file( save=True, options="v=1;" )
    # Backup / Increment
    backupFilePath = ram.RamFileManager.copyToVersion( saveFilePath, increment=increment )
    backupFileName = os.path.basename( backupFilePath )
    decomposedFileName = ram.RamFileManager.decomposeRamsesFileName( backupFileName )
    newVersion = str( decomposedFileName['version'] )
    ram.log( "Scene saved! Current version is: " + newVersion )
    cmds.inViewMessage( msg='Scene saved! <hl>v' + newVersion + '</hl>', pos='midCenter', fade=True )
    
ramSave()
```

## Implementation Notes

Some of the elements described in this documentation have to be interpreted depending on the language used in the implementations of the Ramses API and may vary a bit.

### Include / Import

#### Python

Use your usual preferred way to import the module.

Then, the unique `Ramses` instance is available as `Ramses.instance`.

```py
from ramses import *

myRamses = Ramses.instance
theSettings = myRamses.settings
myShot = RamShot.getFromPath("path/to/the/shot")
aNewStatus = RamStatus(
    myRamses.state("WIP"),
    myRamses.currentUser(),
    "A comment"
    )
myShot.setStatus(aNewStatus, myRamses.currentStep())

# or

import ramses as ram

myRamses = ram.Ramses.instance
theSettings = myRamses.settings()
myShot = ram.RamShot.getFromPath("path/to/the/shot")
aNewStatus = ram.RamStatus(
    myRamses.state("WIP"),
    myRamses.currentUser(),
    "A comment"
    )
myShot.setStatus(aNewStatus, myRamses.currentStep())
```

#### Adobe ExtendScript

Just include the file the usual way.

Then, the unique `Ramses` instance is available as `Ramses.instance`.

```js
#include ramses.jsxinc

var myRamses = Ramses.instance;
var theSettings = myRamses.settings();
var myShot = RamShot.getFromPath("path/to/the/shot");
var aNewStatus = new RamStatus(
    myRamses.state("WIP"),
    myRamses.currentUser(),
    "A comment"
    );
myShot.setStatus(aNewStatus, myRamses.currentStep());
```

### Enumerated Values

*Enumerated values* (a.k.a. *Constants*) are predefined sets of values to be used in specific methods. Their name is in CAPITAL_SNAKE_CASE, and they're grouped together by type (for exemple `color.RED`, or `shape.RECTANGLE`). The value itself can be of any type (array, string, number...). In the previous examples, one could have `color.RED is [255,0,0]`.

#### Python

*Enumerated values* are public static attributes of a containing class. They're declared like this:

```py
class Group():
    VALUE_1 = 0
    VALUE_2 = 1
    VALUE_3 = 2

# Example:
class Color():
    RED = [255,0,0]
    GREEN = [0,255,0]
    BLUE = [0,0,255]
```

And they're used like this:

`myVar = Group.VALUE_1`

`myColor = Color.RED`

#### JavaScript / Adobe ExtendScript

*Enumerated values* are attributes in simple objects. They're declared like this:

```js
var Group = {
    VALUE_1: 0
    VALUE_2: 1
    VALUE_3: 2
};

// Example:
var Color = {
    RED: [255,0,0]
    GREEN: [0,255,0]
    BLUE: [0,0,255]
};
```

And they're used like this:

`var myVar = Group.VALUE_1;`

`var myColor = Color.RED;`