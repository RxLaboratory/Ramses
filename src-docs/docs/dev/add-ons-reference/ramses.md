# Ramses

The main class. One (and only one) instance globally available (Ramses is a *singleton*), by calling `Ramses.instance()`.

## Static Methods

| Method | Arguments | Description |
| --- | --- | --- |
| **instance**<br />▹ *Ramses* | | Returns the *Ramses* unique instance. |
| **version**<br />▹ *string* | | The current version of this API |

## Static Attributes

| Attribute | Type | Default | Description |
| --- | --- | --- | --- |
| **apiReferenceUrl** | *string* | This page URL | A link to this API documentation |
| **addonsHelpUrl** | *string* | The URL to [this page](../../../addons/) | A link to the [Ramses add-ons](../../../addons/) documentation |
| **generalHelpUrl** | *string* | The URL to [this documentation](../../../) | A link to the [Ramses documentation](../../../) |

## Attributes

| Attribute | Type | Default | Description |
| --- | --- | --- | --- |
| **publishScripts** | *list* | `[]` | A list of scripts/functions to be triggered when `Ramses.instance().publish()` is called.<br />If you're using one of the provided Add-ons, you can add your own callbacks to this list so they're run when the user publishes the current file from the host application.<br />The callback must be a method which takes two arguments: the *[RamItem](ram_item.md)*/*[RamShot](ram_shot.md)*/*[RamAsset](ram_asset.md)* being published, the file path (a string) of the document being published, which should be the active document in the host application, and the publish folder path (a string). |
| **statusScripts** | *list* | `[]` | A list of scripts/functions to be triggered when `Ramses.instance().updateStatus()` is called.<br />If you're using one of the provided Add-ons, you can add your own callbacks to this list so they're run when the user changes the current status of an asset / shot from the host application.<br />The callback must be a method which takes two arguments: the *[RamItem](ram_item.md)*/*[RamShot](ram_shot.md)*/*[RamAsset](ram_asset.md)* being updated, and the new *[RamStatus](ram_status.md)*|
| **importScripts** | *list* | `[]` | A list of scripts/functions to be triggered when `Ramses.instance().import()` is called.<br />If you're using one of the provided Add-ons, you can add your own callbacks to this list so they're run when the user asks to import an item.<br />The callback must be a method which takes three arguments: the *[RamItem](ram_item.md)*/*[RamShot](ram_shot.md)*/*[RamAsset](ram_asset.md)* being imported, the [*RamStep*](ram_step.md), and the associated file path (a string). |

## Methods

| Method | Arguments | Description |
| --- | --- | --- |
| **alternativeFolderPaths**<br />▹ *list* of *string* | | A list of alternative absolute paths to the main Ramses folder. Missing files will be looked for in these paths (and copied to the main path if available), and they will be used if the main path is not available. |
| **backupFolderPath**<br />▹ *list* of *string* | | A copy of the main folder where all files are stored. |
| **connect**<br />▹ *boolean* | | Checks *Daemon* availability and initiates the connection. <br />Returns success. |
| **currentProject**<br />▹ *[RamProject](ram_project.md)* or *None* | | The current project. |
| **currentStep**<br />▹ *[RamStep](ram_step.md)* or *None* | | The current step. |
| **currentUser**<br />▹ *[RamUser](ram_user.md)* or *None* | | The current user. |
| **daemonInterface**<br />▹ *RamDaemonInterface* | | The *Daemon* interface unique instance. Same as `RamDaemonInterface.instance()` |
| **disconnect**<br />▹ *boolean* | | Gets back to offline mode (stops all communications with the *Daemon*). |
| **folderPath**<br />▹ *string* | | The absolute path to main Ramses folder, containing projects by default, config files, user folders, admin files... |
| **importItem** | *[RamItem](ram_item.md) or [RamAsset](ram_asset.md) or [RamShot](ram_shot)*: **item**,<br />*[*RamStep*](ram_step.md)*: **step**<br />*string*: **filePath** | Runs the scripts in `Ramses.instance().importScripts`. |
| **project**<br />▹ *[RamProject](ram_project.md)* | *string*: **projectShortName** | Gets a specific project. |
| **projects**<br />▹ *list of [RamProject](ram_project.md)* | | The list of available projects. |
| **projectsPath**<br />▹ *string* | | The path to the folder containing projects. |
| **states**<br />▹ *list of [RamState](ram_state.md)* | | The list of available states. |
| **state**<br />▹ *[RamState](ram_state.md)* | *string*: **stateShortName**=`WIP` | Gets a specific state. |
| **online**<br />▹ *boolean* | | True if connected to the *Daemon* and the *Daemon* is responding. |
| **publish** | *[RamItem](ram_item.md) or [RamAsset](ram_asset.md) or [RamShot](ram_shot)*: **item**,<br />*string*: **filePath**,<br />*string*: **publishFolderPath** | Runs the scripts in `Ramses.instance().publishScripts`. |
| **showClient** | | Raises the *Ramses Client* window, launches the client if it is not already running. |
| **settings** | [*RamSettings*](ram_settings.md) | The settings unique instance. Same as `RamSettings.instance()` |
| **updateStatus** | *[RamItem](ram_item.md) or [RamAsset](ram_asset.md) or [RamShot](ram_shot)*: **item**,<br />*[RamStatus](ram_status.md)*: **status** |  Runs the scripts in `Ramses.instance().statusScripts`. |
| **usersPath**<br />▹ *string* | |  The path to the folder containing users. |

## Examples

```py
# Python

# import ramses
import ramses as ram
# Get the instance
ramses = ram.Ramses.instance()

# A simple method
def published():
    ram.log("Hello, I've been published!")

# Adds the method to the scripts which will be run when the add-on publishes a file
ramses.publishScripts.append( published )

# The provided Ramses add-ons for Blender, Maya, etc. automatically trigger these scripts.
# If you're developping another addon, you have to call Ramses.publish() to run them
ramses.publish()
```

```js
// ExtendScript

// include the Ramses lib
#include ramses.jsxinc
// Get the instance
var ramses = Ramses.instance();

// Adds the method to the scripts which will be run when the add-on publishes a file
ramses.publishScripts.push( published )

// The provided Ramses add-ons for Blender, Maya, etc. automatically trigger these scripts.
// If you're developping another addon, you have to call Ramses.publish() to run them
ramses.publish()
```

____

## API Dev notes

!!! note
    These section is for the development of the API only; you should not need these when developping your add-on using the API.

### Private attributes

| Attribute | Type | Default | Description |
| --- | --- | --- | --- |
| **defaultStates** | *list of [RamState](ram_state.md)* | | The list of states to use if the *Daemon* is not available. |
| **defaultState** | *[RamState](ram_state.md)* | | The state to use as default ("WIP"). |

### (Im)mutable data

The data returned by the methods can be either [mutable or immutable](implementation.md#accessing-the-data).

| Method | Type of the returned data |
| --- | --- |
| **alternativeFolderPaths** | <i class="fa fa-lock"></i> Immutable |
| **backupFolderPath** | <i class="fa fa-lock"></i> Immutable |
| **currentProject** | <i class="fa fa-pen"></i> Mutable |
| **currentStep** | <i class="fa fa-pen"></i> Mutable |
| **currentUser** | <i class="fa fa-pen"></i> Mutable |
| **folderPath** | <i class="fa fa-lock"></i> Immutable |
| **projects** | <i class="fa fa-pen"></i> Mutable |
| **project** | <i class="fa fa-pen"></i> Mutable |
| **states** | <i class="fa fa-lock"></i> Immutable |
| **state** | <i class="fa fa-pen"></i> Mutable |

![META](authors:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2021;updated:2021/05/25)