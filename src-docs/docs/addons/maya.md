# Maya add-on

*Ramses* comes with an official add-on for *Autodesk Maya*, which actually consists in a *Python* plug-in and a custom shelf packaged in a simple module.

It is an example of an implementation of an add-on using the [*Ramses Python API*](../../dev/add-ons-reference) but it is already well suited for use in production.

## Installation

- [Download](https://github.com/Rainbox-dev/Ramses-Maya/archive/refs/heads/main.zip) and unzip the module.
- Edit `Ramses.mod` with a text editor, and replace the path in the first line with the path where you've unzipped the module.

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

![META](authors:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2021;updated:2021/05/28)