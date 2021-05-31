# Maya add-on

*Ramses* comes with an official add-on for *Autodesk Maya*, which actually consists in a *Python* plug-in and a custom shelf packaged in a simple module.

It is an example of an implementation of an add-on using the [*Ramses Python API*](../../dev/add-ons-reference) but it is already well suited for use in production.

## Installation

- [Download](https://github.com/Rainbox-dev/Ramses-Maya/archive/refs/heads/main.zip) and unzip the module.  
    To quickly deploy the module on several workstations, choose a shared location; the module can then be shared with all workstations and updated at once.
- Edit `Ramses.mod` with a text editor, and replace the path in the first line with the absolute path where you've unzipped the module.

This is how the content of this file looks like:

```
+ Ramses 0.0.1-dev D:\your\path\to\the\module
PYTHONPATH+:=plug-ins
MAYA_SHELF_PATH+:=shelves
```

Do not change anything else in the file.

- Copy `Ramses.mod` in one of your modules paths  
    e.g. `C:\Users\User\Documents\Maya\modules`.  
    You may need to create the *modules* folder if it does not exist yet.  
    Repeat this operation on all workstations where you need the module.
- Restart *Maya*.
- Activate the *Ramses* plug-in from *Maya*'s plug-in manager.

![Plugin manager](/img/maya/plugin_manager.png)

## Commands

As all other *Ramses* add-ons, the add-on for *Maya* adds a few commands to maya which you can use as any other command (e.g. to add a shortcut in a shelf, in a *Mel* script or in a *Python* script)

Here is the list of these commands.

| Command name | Flags | Description |
| --- | --- | ---|
| ramSave | **-set_comment -sc**: *boolean*,<br />**-comment -c**: *string* | Saves the current scene and copies a backup inside the *versions* subfolder.<br /> If *-set_comment* is set to `True`, *Ramses* will add a comment to the current version.<br />Setting *-comment* will prevent the dialog from showing, and set the given comment to the current version. |
| ramSaveVersion | | Saves a new version of the scene. A Dialog is shown to ask for an optionnal *Ramses Status* update. |
| ramRetrieveVersion | | Shows a dialog to select an older version of the scene to restore. |
| ramPublishTemplate | | Publishes the current scene as a template for a given step. |
| ramSettings | | Shows the settings dialog for the add-on. |
| ramOpenRamses | | Attempts to launch the *Ramses* application. |

### Examples

```py
# Python
import maya.cmds as cmds

# Save the current file
cmds.ramSave()

# Save the file with a comment (show the comment dialog)
cmds.ramSave(set_comment = True)

# Save the file with a custom comment
cmds.ramSave(sc = True, comment = "A new comment!")
```

### Shelf

Most of these commands are added with handy buttons in a custom shelf named *Ramses*

![Plugin manager](/img/maya/shelf.png)

![](/img/addon-icons/ramses.png){style="width:32px"} Opens the *Ramses* application if available.

![](/img/addon-icons/savefile_sl.png){style="width:32px"} Saves the current scene.

![](/img/addon-icons/savecomment.png){style="width:32px"} Adds a comment to the current scene.

![](/img/addon-icons/saveincremental_sl.png){style="width:32px"} Saves a new version of the scene, optionnaly updating the status of the corresponding *Ramses item*.

![](/img/addon-icons/publishtemplate.png){style="width:32px"} Publishes the current scene as a new template for a given step.

![](/img/addon-icons/retrieveversion.png){style="width:32px"} Restores a previous version of the current scene.

![](/img/addon-icons/settings.png){style="width:32px"} Opens the settings of the add-on.

## Extensibility

You can easily add features to the provided Add-On, especially by registering your own scripts/methods to be run when a status changes, when a scene is being published, or to import items.

*Ramses* stores three lists of callbacks you can extend.

- [`Ramses.publishScripts`](../dev/add-ons-reference/ramses.md) contains the functions to be called when a scene is being published.

When *Ramses* publishes a scene, it first copies the scene file to the publish folder, and then calls all the scripts listed there. Each callback you add to this list must take three arguments. Read the [Scripting API Reference](../dev/add-ons-reference/ramses.md) for more information.

- [`Ramses.statusScripts`](../dev/add-ons-reference/ramses.md) contains the functions to be called when a scene status changes.

When *Ramses* updates a status, it first copies the scene file to the version folder and calls the *Daemon* (if available) to notify the update, and then calls all the scripts listed there. Each callback you add to this list must take two arguments. Read the [Scripting API Reference](../dev/add-ons-reference/ramses.md) for more information.

- [`Ramses.importScripts`](../dev/add-ons-reference/ramses.md) contains the functions to be called when the user selects an item (shot or asset) to import.

By default, if and only if the list is empty, *Ramses* imports the selected item in the current scene, in a group named after the item, and in another group named after the asset group if it is an asset. Adding new callbacks to the list will de-activate this default behaviour so you can use your own methods for importing items. Each callback you add to this list must take two arguments. Read the [Scripting API Reference](../dev/add-ons-reference/ramses.md) for more information.

There are two ways to register your methods: by using a *Maya* command, or by [forking](https://github.com/Rainbox-dev/Ramses-Maya) the provided Add-On to add your functions.

Once the callbacks have been registered, they are automatically called when the user interacts with the add-on and wants to update/import/publish an item.

You can also explicitly call them by calling these three methods:

- [`Ramses.updateStatus( item, status )`](../dev/add-ons-reference/ramses.md)
- [`Ramses.importItem( item, step, filePath )`](../dev/add-ons-reference/ramses.md)
- [`Ramses.publish( item, filePath, publishFolderPath )`](../dev/add-ons-reference/ramses.md)

Read the [Scripting API Reference](../dev/add-ons-reference/ramses.md) for more information.

### Using the *Maya* *Ramses* command

TODO

### Forking the Add-On

When [forking](https://github.com/Rainbox-dev/Ramses-Maya) the add-on, you can easily write your own methods in your own files and import them into the add-on, to keep it easy to pull changes from the official add-on.

Just add your files in the `plug-ins/ramses_maya/` folder, and import them in `__init__.py`.

In your custom files, you can import the `ramses` module, and append your methods to the three callback lists.

#### Example

In this example, three files are added, with the import, status, and publish methods.

The file `myPublishCallbacks.py` creates and registers the callbacks used to publish shots and assets.

```py
# myPublishCallbacks.py

import ramses as ram

# Get the ramses instance
ramses = ram.Ramses.instance()

def publishAsset(item, filePath, publishFolderPath):
    """Callback used to publish Assets only"""
    # The item must be a RamAsset
    if item.itemType() != ram.ItemType.ASSET:
        return
    
    # Now we can do what we need using the RamAsset object, the filePath and publisFolderPath strings
    doSomething( item.shortName(), item.group(), filePath, publishFolderPath)
    # etc.

def publishShot(item, filePath, publishFolderPath):
    """Callback used to publish Shots only"""
    # The item must be a RamAsset
    if item.itemType() != ram.ItemType.SHOT:
        return
    
    # Now we can do what we need using the RamShot object, the filePath and publisFolderPath strings
    doSomething( item.shortName(), item.duration(), filePath, publishFolderPath)
    # etc.

# Finally, we just need to add the callbacks to the publish list
ramses.publishScripts.append( publishAsset )
ramses.publishScripts.append( publishShot )

```

The file `myStatusCallbacks.py` creates and registers the callbacks used when a status changes.

```py
# myStatusCallbacks.py

import ramses as ram

# Get the ramses instance
ramses = ram.Ramses.instance()

def updateAsset(item, status):
    """Callback used to update Assets only"""
    # The item must be a RamAsset
    if item.itemType() != ram.ItemType.ASSET:
        return
    
    # Now we can do what we need using the RamAsset object and the RamStatus object
    doSomething( item.shortName(), item.group(), status.completionRatio, status.user)
    # etc.

def updateShot(item, filePath, publishFolderPath):
    """Callback used to update Shots only"""
    # The item must be a RamShot
    if item.itemType() != ram.ItemType.SHOT:
        return
    
    # Now we can do what we need using the RamAsset object and the RamStatus object
    doSomething( item.shortName(), item.group(), status.completionRatio, status.user)
    # etc.

# Finally, we just need to add the callbacks to the status list
ramses.statusScripts.append( updateAsset )
ramses.statusScripts.append( updateShot )

```

The file `myImportCallbacks.py` creates and registers the callbacks used to import items.

```py
# myImportCallbacks.py

import ramses as ram

# Get the ramses instance
ramses = ram.Ramses.instance()

def importAsset(item, step, filePath):
    """Callback used to import Assets only"""
    # The item must be a RamAsset
    if item.itemType() != ram.ItemType.ASSET:
        return
    
    # Now we can do what we need using the RamAsset object, the RamStep object and the file path
    doSomething( item.shortName(), item.group(), step.shortName(), filePath)
    # etc.

# Finally, we just need to add the callbacks to the status list
ramses.importScripts.append( importAsset )

```

Finally, we just need to import these files in the `__init__.py` file. Notice we don't change anything in the file, just append the three imports at the end.

```py
# __init__.py
from .ram_cmds import cmds_classes
from ramses import log, LogLevel

import myImportCallbacks
import myStatusCallbacks
import myPublishCallbacks
```

!!! warning
    When pulling changes from the official *Add-On*, just make sure the `__init__.py` file is also updated, but keep (or add again) your three imports.  
    Your custom methods being stored in separate files won't be a problem.

![META](authors:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2021;updated:2021/05/28)