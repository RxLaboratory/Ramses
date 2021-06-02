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

You can easily add features to the *Ramses Maya Add-On*, especially by registering your own scripts/methods to be run when a status changes, when a scene is being published, or to import items.

You can also easily use the included [*Ramses Scripting API*](../dev/add-ons-reference/index.md) as a *Python module* to develop your own functions.

### Getting started

Technically, the simplest and recommended way to extend the *Ramses Maya Add-On* is to add your own module(s) inside the Add-on iteself. It is very easy and this way you do not need to register your script or plug-in with *Maya*, *Ramses* takes care of that for you.

#### Install the Add-On

First, install the *Ramses Maya Add-On* as described above.

These folders should be available:

- ***icons***: The icons used by the Add-On
- ***plug-ins***: The actual *Python* code
    - ***dumaf***: Some useful *Python* code for *Maya*
    - ***ramses***: The [*Ramses Scripting API*](../dev/add-ons-reference/index.md) as a *Python module*
    - ***ramses_maya***: The Add-On itself
- ***shelves***: The *Ramses Maya shelf*

#### Create your module

All you need to do to easily extend the *Ramses Maya Add-On* is to create your own module in the `plug-ins` folder.

Create a subfolder in the `plug-ins` folder, containing an empty `__init__.py` file. You can also optionally add your own *shelf* in the `shelves` folder.

The new tree should look like this:

- *icons*
- *plug-ins*
    - *dumaf*
    - *ramses*
    - *ramses_maya*
    - ***your_new_module***: contains `__init__.py`
- *shelves*: optionally add `shelf_YourModule.mel`

In the new `your_new_module/__init__.py` file, you will import or code all the methods you want to expose and make available in Maya, for example to be used in a *shelf* or any other *Python* script. You can also register your *Ramses* callbacks there (read below for more information about *Ramses* callbacks).

Here is an example of such an `__init__.py` file, assuming you have two other files (*anotherFile.py* and *aSecondFile.py*) in the same folder, containing the actual code and methods to import:

```py
from .anotherFile import aPublicMethod
from .aSecondFile import aPublishMethod, anImportMethod

# Now, 'aPublicMethod' can be called from everyhwere in Maya

# And we can register the publish and import method as callbacks for Ramses.
# For this, we need to import the Ramses Module (API)
from ramses import Ramses
# Now we just have to add the callbacks:
ramses = Ramses.instance()
# The publish method
ramses.publishScripts.append( aPublishMethod )
# The import method
ramses.importScripts.append( anImportMethod )
```

#### Import the module

Finally, you just have to import your new module in the `ramses_maya` module: add a simple import at the end of the `ramses_maya/__init__.py` file, which should look like this:

```py
from .ram_cmds import cmds_classes
from ramses import log, LogLevel

import your_new_module
```

Both the new module and the shelf will be automatically registered by the *Ramses Maya Add-On* when it's loaded in *Maya*.

#### Example

A complete and working example has been made available: it is the extension made for the school [*Rubika Animation / Supinfocom*](http://rubika-edu.com), for the production pipeline of the graduation movies of the students.

You can [get it here](#), and it comes with a [comprehensive documentation](#).

Feel free to try and use it, and study it. As explained just before, all the extension is packaged in its own module called `rubika` in the `plug-ins` folder.

### Callbacks

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

- [`Ramses.updateStatus( item, status, stepShortName )`](../dev/add-ons-reference/ramses.md)
- [`Ramses.importItem( item, filePath, stepShortName )`](../dev/add-ons-reference/ramses.md)
- [`Ramses.publish( item, filePath, stepShortName )`](../dev/add-ons-reference/ramses.md)

Read the [Scripting API Reference](../dev/add-ons-reference/ramses.md) for more information.

#### Adding your custom callbacks

To register your callbacks, you just have to append them in the corresponding `Ramses` lists:

```py
# We need to import the Ramses Module (API)
from ramses import Ramses
# Now we just have to add the callbacks
ramses = Ramses.instance()
# A publish method
ramses.publishScripts.append( aPublishMethod )
# An import method
ramses.importScripts.append( anImportMethod )
# A status method
ramses.statusScripts.append( aStatusMethod )
```

Now, all these methods will be automatically called each time a status changes, an item is published, or an item is imported.

### Detailed Example

In this example, three files are added, with the import, status, and publish methods.

The file `plug-ins/your_new_module/myPublishCallbacks.py` creates the callbacks used to publish shots and assets.

```py
# myPublishCallbacks.py

import ramses as ram

# Get the ramses instance
ramses = ram.Ramses.instance()

def publishAsset(item, filePath, step):
    """Callback used to publish Assets only"""
    # The item must be a RamAsset
    if item.itemType() != ram.ItemType.ASSET:
        return
    
    # Now we can do what we need using the RamAsset object, the filePath and the Step
    doSomething( item.shortName(), item.group(), filePath, step.shortName())
    # etc.

def publishShot(item, filePath, step):
    """Callback used to publish Shots only"""
    # The item must be a RamAsset
    if item.itemType() != ram.ItemType.SHOT:
        return
    
    # Now we can do what we need using the RamShot object, the filePath and the Step
    doSomething( item.shortName(), item.duration(), filePath, step.shortName())
    # etc.

```

The file `plug-ins/your_new_module/myStatusCallbacks.py` creates the callbacks used when a status changes.

```py
# myStatusCallbacks.py

import ramses as ram

# Get the ramses instance
ramses = ram.Ramses.instance()

def updateAsset(item, status, step):
    """Callback used to update Assets only"""
    # The item must be a RamAsset
    if item.itemType() != ram.ItemType.ASSET:
        return
    
    # Now we can do what we need using the RamAsset object, the RamStatus object and the RamStep object
    doSomething( item.shortName(), item.group(), status.completionRatio, status.user, step.shortName())
    # etc.

def updateShot(item, filePath, publishFolderPath):
    """Callback used to update Shots only"""
    # The item must be a RamShot
    if item.itemType() != ram.ItemType.SHOT:
        return
    
    # Now we can do what we need using the RamAsset object, the RamStatus object and the RamStep object
    doSomething( item.shortName(), item.group(), status.completionRatio, status.user, step.shortName())
    # etc.

```

The file `plug-ins/your_new_module/myImportCallbacks.py` creates the callbacks used to import items.

```py
# myImportCallbacks.py

import ramses as ram

# Get the ramses instance
ramses = ram.Ramses.instance()

def importAsset(item, filePath, step):
    """Callback used to import Assets only"""
    # The item must be a RamAsset
    if item.itemType() != ram.ItemType.ASSET:
        return
    
    # Now we can do what we need using the RamAsset object, the RamStep object and the file path
    doSomething( item.shortName(), item.group(), step.shortName(), filePath)
    # etc.

```

The file `plug-ins/your_new_module/otherMethods.py` creates a few other methods, for example to be used in a *Maya shelf*.

```py
# otherMethods.py

def methodForAShelf():
    doSomething()

```

Finally, we just need to import these files in the `plug-ins/your_new_module/__init__.py` file of our custom module. We can register them here.

```py
# __init__.py
from myImportCallbacks import importAsset
from myStatusCallbacks import updateAsset, updateShot
from myPublishCallbacks import publishAsset, publishShot
from otherMethods import methodForAShelf

from ramses import Ramses

# Get the ramses instance
ramses = Ramses.instance()

# Add the callbacks to the publish list
ramses.publishScripts.append( publishAsset )
ramses.publishScripts.append( publishShot )

# Add the callbacks to the status list
ramses.statusScripts.append( updateAsset )
ramses.statusScripts.append( updateShot )

# Add the callbacks to the status list
ramses.importScripts.append( importAsset )
```

And don't forget to import your new module in `plug-ins/ramses_maya/__init__.py`:

```py
# plug-ins/ramses_maya/__init__.py
from .ram_cmds import cmds_classes
from ramses import log, LogLevel

import your_new_module
```

Don't change anything else in the file!

Now you can also use `methodForAShelf()` in an existing or new *shelf*, which can be stored in the `shelves` folder if you want it to be deployed with your extension.


![META](authors:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2021;updated:2021/05/28)