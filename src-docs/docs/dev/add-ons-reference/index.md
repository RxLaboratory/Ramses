![META](authors:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2021;updated:2021/09/08)

# Scripting API reference

[*RxLaboratory*](https://rxlaboratory.org) provides a reference implementation of the API in *Python* and *Adobe ExtendScript*.

Other APIs may be developped, in which case all these APIs on all programming/scripting language should implement this reference, with the same classes, attributes and methods.

As this document is referring to different scripting languages, it uses general terms which may be different than what is actually used in specific languages.

- Python API: [source](https://github.com/RxLaboratory/Ramses-Py), [download](https://github.com/RxLaboratory/Ramses-Py/releases)
- ExtendScript API: [source (available soon)](#), [download (available soon)](#)

!!! hint
    Some of the elements described in this documentation have to be interpreted differently depending on the language used in the implementations of the *Ramses Add-ons API* and may vary a bit. Read the examples below to see the differences between a few scripting languages.

## Develop a DCC application Add-on using the Ramses API

To use Ramses right from any Digital Content Creation application, you can develop an add-on for this application which, by using the Ramses API, communicates with Ramses.

Such add-ons should implement methods to create, open, save, publish, import, and replace items (shots or assets), and also to update their production status.  
To get started, you should first implement these methods and register them in Ramses.

```py
# Python

# import ramses
import ramses as ram
# Get the instance
RAMSES = ram.Ramses.instance()

# The method to open a file
def opener( file_path, ram_item, ram_step ):
    """This method should open the file available at file_path, which is the working file for the given *RamItem* and *RamStep*"""

# The method to publish an item
def publisher( file_path, ram_item, ram_step, publish_options=None, show_publish_options=False ):
    """This method should publish the given *RamItem* at the given *RamStep*, from the working file available at file_path
    Options can optionally be passed to this method to specify a publishing file format for example
    """

# The method to import an item
def importer( file_paths, ram_item, ram_step, import_options=None, show_import_options=False ):
    """This method should import the files available in the file_paths list, which are files published from the given *RamItem* for the given *RamStep*"""

# Register the methods
RAMSES.openScripts.append( openener )
RAMSES.publishScripts.append( publisher )
RAMSES.importScripts.append( importer )
```

Now that you have a few methods to open working files, publish and import items, you can implement the UI and options of the add-on, depending on the host DCC application, and use the Ramses API to call them. By doing this, you also enable the user to add his own methods to the `on_open`, `on_publish`, `on_import` events.

All these events which can be implemented are included in the [`Ramses` class](ramses.md#events-and-handlers).

```py
# Python

# import ramses
import ramses as ram
# Get the instance
RAMSES = ram.Ramses.instance()

# A method which opens the working file of an item
def open_file():
    # First, show a UI to let the user select an item and a step
    open_ui = Open_UI()
    result = open_ui.exec()
    if result:
        item = open_ui.get_item()
        step = open_ui.get_step()
        # Get the working file.
        # stepFilePath() is a method from RamItem to retrieve the file for the given step of the item
        file_path = item.stepFilePath("", "", step)
        # Trigger the open event in ramses
        RAMSES.openFile(file_path)
        # Ramses will run the opener() method defined sooner, and all other custom handlers which may be hooked to the open event.

```

The Ramses API provides classes to handle all kind of objets (shots, assets, files, status, projects...) managed by the Ramses Framework. All these classes are documented here.

## Classes

This is the comprehensive list of available classes. In the *Ramses Add-ons API*, everything is contained in these classes.

| Class | Inheritance | Description |
| ---- | ---- | ---- |
| [Ramses](ramses.md) | | The main class. One (and only one) instance globally available, instantiated during init time. |
| [RamAsset](ram_asset.md) | [RamItem](ram_item.md) | An asset. |
| [RamDaemonInterface](ram_daemon_interface.md) | | The Class used to communicate with the Ramses Daemon |
| [RamFileManager](ram_file_manager.md) | | Generic tools to handle files and the [Ramses Naming Scheme](../../components/files/naming.md) |
| [RamFileType](ram_filetype.md) | | A File type (name and possible extensions) |
| [RamItem](ram_item.md) | [RamObject](ram_object.md) | The base class for Shots and Assets. |
| [RamMetaDataManager](ram_metadata_manager.md) | | A Class to get/set metadata from files. |
| [RamFileInfo](ram_file_info.md) | | A Class to handle ramses (file)names. |
| [RamObject](ram_object.md) | | The base class for most of Ramses objects. |
| [RamPipe](ram_pipe.md) | | A pipe which connects two steps together in the production pipeline |
| [RamPipeFile](ram_pipefile.md) | | The description of a file type going through a pipe |
| [RamProject](ram_project.md) | [RamObject](ram_object.md) | A project handled by Ramses. Projects contains general items, assets and shots. |
| [RamSettings](ram_settings.md) | | Gets and saves settings used by Ramses. | 
| [RamShot](ram_shot.md) | [RamItem](ram_item.md) | A shot. |
| [RamState](ram_state.md) | [RamObject](ram_object.md) | The state of a step (like "Waiting for approval", "Approved", etc.). |
| [RamStatus](ram_status.md) | | A state associated to a comment, the user who changed the state, etc. |
| [RamStep](ram_step.md) | [RamObject](ram_object.md) | A step in the production of the shots or assets of the project. |
| [RamUser](ram_user.md) | [RamObject](ram_object.md) | The class representing users. |

# Global Methods

| Method | Arguments | Description |
| --- | --- | --- |
| **log** | *string*: **message**,<br />*LogLevel*: **level**=`LogLevel.Info`| Logs a message to the console. The level must be one of `LogLevel.DataReceived`, `LogLevel.DataSent`, `LogLevel.Debug`, `LogLevel.Info`, `LogLevel.Critical`, `LogLevel.Fatal` |
| **printException** | | To be used in an `except` statement in Python or a `catch` statement in *JavaScript*. In *JavaScript* the error must be passed as an argument. Logs the exception to the console. |

## Examples

Here are a few examples showing how this API can be used.

### Include / Import the API

```py
# Python
# Use your usual preferred way to import the module.

from ramses import *

# The unique `Ramses` instance is available as `Ramses.instance()`.
myRamses = Ramses.instance()
theSettings = myRamses.settings
myShot = RamShot.getFromPath("path/to/the/shot")
aNewStatus = RamStatus(
    myRamses.state("WIP"),
    myRamses.currentUser(),
    "A comment"
    )
myShot.setStatus(aNewStatus, myRamses.currentStep())

# or (this is the recommended way)

import ramses as ram

myRamses = ram.Ramses.instance()
theSettings = myRamses.settings()
myShot = ram.RamShot.getFromPath("path/to/the/shot")
aNewStatus = ram.RamStatus(
    myRamses.state("WIP"),
    myRamses.currentUser(),
    "A comment"
    )
myShot.setStatus(aNewStatus, myRamses.currentStep())
```

```js
// ExtendScript
// Just include the file the usual way.

#include ramses.jsxinc

// The unique `Ramses` instance is available as `Ramses.instance()`.
var myRamses = Ramses.instance();
var theSettings = myRamses.settings();
var myShot = RamShot.getFromPath("path/to/the/shot");
var aNewStatus = new RamStatus(
    myRamses.state("WIP"),
    myRamses.currentUser(),
    "A comment"
    );
myShot.setStatus(aNewStatus, myRamses.currentStep());
```

### Enumerated values (constants)

Enumerated values (a.k.a. Constants) are predefined sets of values to be used in specific methods. Their name is in CAPITAL_SNAKE_CASE, and they’re grouped together by type (for exemple Color.RED, or Shape.RECTANGLE). The value itself can be of any type (array, string, number…).

In this documentation they're written like this: `ValueType.VALUE`, i.e. `ItemType.SHOT` or `ItemType.ASSET`, and they're used the same way in the actual code.

```py
# Python

from ramses import RamItem, ItemType

# Get an item from a specific file path
anItem = RamItem.getFromPath( somePath )

if anItem is not None:
    # We want it only if it's a shot,
    # We can use the enum ItemType to compare the value.
    if anItem.itemType() == ItemType.SHOT:
        doSomething()
```

```js
// ExtendScript

#include ramses.jsxinc

// Get an item from a specific file path
var anItem = RamItem.getFromPath( somePath );

if (anItem != null)
{
    // We want it only if it's a shot,
    // We can use the enum ItemType to compare the value.
    if ( anItem.itemType() == ItemType.SHOT )
        doSomething();
}
```

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
