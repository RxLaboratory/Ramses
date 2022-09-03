![META](authors:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2021-2022;updated:2022/09/02)

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
| **addonsHelpUrl** | *string* | The URL to [this page](../../components/addons/) | A link to the [Ramses add-ons](../../components/addons/) documentation |
| **generalHelpUrl** | *string* | The URL to [this documentation](../../../) | A link to the [Ramses documentation](../../../) |

## Attributes

Read the section entitled *Callbacks* below (and see the *Examples*) for more information about the methods you can add to these lists.

| Attribute | Type | Default | Description |
| --- | --- | --- | --- |
| **importScripts** | *list* | `[]` | A list of scripts/functions to be triggered when `Ramses.instance().importItem()` is called.<br />If you're using one of the provided Add-ons, you can add your own callbacks to this list so they're run when the user asks to import an item.<br />Note: official *Ramses Add-ons* will add an item to this list to provide a default implementation. If you're extending an existing add-on, you can reset the list or insert/append your own methods to keep the default behavior. |
| **publishScripts** | *list* | `[]` | A list of scripts/functions to be triggered when `Ramses.instance().publish()` is called.<br />If you're using one of the provided Add-ons, you can add your own callbacks to this list so they're run when the user publishes the current file from the host application.<br />Note: official *Ramses Add-ons* will add an item to this list to provide a default implementation. If you're extending an existing add-on, you can reset the list or insert/append your own methods to keep the default behavior. |
| **replaceScripts** | *list* | `[]` | A list of scripts/functions to be triggered when `Ramses.instance().replaceItem()` is called.<br />If you're using one of the provided Add-ons, you can add your own callbacks to this list so they're run when the user asks to import an item.<br />Note: official *Ramses Add-ons* will add an item to this list to provide a default implementation. If you're extending an existing add-on, you can reset the list or insert/append your own methods to keep the default behavior. |
| **statusScripts** | *list* | `[]` | A list of scripts/functions to be triggered when `Ramses.instance().updateStatus()` is called.<br />If you're using one of the provided Add-ons, you can add your own callbacks to this list so they're run when the user changes the current status of an asset / shot from the host application. |
| **userScripts** | *dict* or *object* | `{}` | This *dict* or *object* is here for your convenience, to make it easy to register and call any method from anywhere you've imported Ramses without having to import the file containing the method. Just register with `Ramses.instance().userScripts["TheFunctionName"] = aFunction` and call the method with `Ramses.instance().userScripts["TheFunctionName"](some, args)`. |

## Methods

| Method | Arguments | Description |
| --- | --- | --- |
| **alternativeFolderPaths**<br />▹ *list* of *string* | | A list of alternative absolute paths to the main Ramses folder. Missing files will be looked for in these paths (and copied to the main path if available), and they will be used if the main path is not available. |
| **backupFolderPath**<br />▹ *list* of *string* | | A copy of the main folder where all files are stored. |
| **connect**<br />▹ *boolean* | | Checks *Daemon* availability and initiates the connection. <br />Returns success. |
| **currentProject**<br />▹ *[RamProject](ram_project.md)* or *None* | | The current project. |
| **currentUser**<br />▹ *[RamUser](ram_user.md)* or *None* | | The current user. |
| **daemonInterface**<br />▹ *RamDaemonInterface* | | The *Daemon* interface unique instance. Same as `RamDaemonInterface.instance()` |
| **disconnect**<br />▹ *boolean* | | Gets back to offline mode (stops all communications with the *Daemon*). |
| **folderPath**<br />▹ *string* | | The absolute path to main Ramses folder, containing projects by default, config files, user folders, admin files... |
| **importItem** | • *[RamItem](ram_item.md) or [RamAsset](ram_asset.md) or [RamShot](ram_shot)*: **item**,<br />• *string*: **filePath**,<br />• *[RamStep](ram_step.md)*: **step**=`None`,<br />• *dict*: **importOptions**=`None`,<br />• *bool*: **showImportOptions**=`false` | Runs the scripts in `Ramses.instance().importScripts`.<br />Read the section entitled *Callbacks* below for more details. |
| **project**<br />▹ *[RamProject](ram_project.md)* or *None* | | (Try to) retrieve a project using its short name. |
| **projects**<br />▹ *list of [RamProject](ram_project.md)* | | The list of available projects. |
| **projectsPath**<br />▹ *string* | | The path to the folder containing projects. |
| **states**<br />▹ *list of [RamState](ram_state.md)* | | The list of available states. |
| **state**<br />▹ *[RamState](ram_state.md)* | • *string*: **stateShortName**=`WIP` | Gets a specific state. |
| **online**<br />▹ *boolean* | | True if connected to the *Daemon* and the *Daemon* is responding. |
| **publish** | • *[RamItem](ram_item.md) or [RamAsset](ram_asset.md) or [RamShot](ram_shot)*: **item**,<br />• *[RamStep](ram_step.md)*: **step**,<br />• *string*: **filePath**,<br />• *dict*: **publishOptions**=`None`,<br />• *bool*: **showPublishOptions**=`false` | Runs the scripts in `Ramses.instance().publishScripts`.<br />Read the section entitled *Callbacks* below for more details. |
| **replaceItem** | • *[RamItem](ram_item.md) or [RamAsset](ram_asset.md) or [RamShot](ram_shot)*: **item**,<br />• *string*: **filePath**,<br />• *[RamStep](ram_step.md)*: **step**=`None`,<br />• *dict*: **importOptions**=`None`,<br />• *bool*: **showImportOptions**=`false` | Runs the scripts in `Ramses.instance().replaceScripts`.<br />Read the section entitled *Callbacks* below for more details. |
| **showClient** | | Raises the *Ramses Client* window, launches the client if it is not already running. |
| **setCurrentProject** | • *[RamProject](ram_project.md)*: **project** | Sets the current project. |
| **settings**<br />▹ [*RamSettings*](ram_settings.md) | | The settings unique instance. Same as `RamSettings.instance()` |
| **updateStatus** | • *[RamItem](ram_item.md) or [RamAsset](ram_asset.md) or [RamShot](ram_shot)*: **item**,<br />• *[RamStatus](ram_status.md)*: **status**,<br />• *[RamStep](ram_step.md)*: **step** |  Runs the scripts in `Ramses.instance().statusScripts`.<br />Read the section entitled *Callbacks* below for more details. |
| **usersPath**<br />▹ *string* | |  The path to the folder containing users. |

## Callbacks

The callbacks are the methods you can add to these lists: `Ramses.publishScripts`, `Ramses.statusScripts`, `Ramses.importScripts`, `Ramses.userScripts`.

When using one of the provided [Ramses Add-ons](../../components/addons/index.md), these callbacks will be called automatically when the user publishes an item, changes the status, or wants to import a file. This makes it very easy to extend these add-ons by just adding your own methods.

If you're developping your own add-on from scratch, it may be useful to add your methods to these lists to call them easily anyway; use `Ramses.publish()`, `Ramses.importItem()`, and `Ramses.updateStatus()`.

*Ramses* will pass a few arguments depending on the context to theses methods, which they'll have to handle. The table below describes the callbacks.

| Callback | Arguments | Description |
| --- | --- | --- |
| **importItem** | • *[RamItem](ram_item.md) or [RamAsset](ram_asset.md) or [RamShot](ram_shot)*: **item**,<br />• *string*: **filePath**,<br />• *[RamStep](ram_step.md)*: **step**=`None`,<br />• *dict*: **importOptions**=`None`,<br />• *bool*: **showImportOptions**=`false` | - `item` is the item from which the user wants to import a file.<br />- `filePaths` is the list of absolute path to the files the user wants to import. This list can be empty! In this case, it means the add-on must automatically choose the right file from the publish folder of the item and step.<br />- `step` is the step from which the user wants to import. You can check `step.outputPipes()` to know which file types have been published, and `inputPipes()` from the step you're importing to to know which files to accept.<br />- `importOptions` is a dictionnary (associative array) containing custom options you can use to import the item.<br />- `showImportOptions` tells the script to show a UI or not to edit the import options before importing. |
| **publish** | • *[RamItem](ram_item.md) or [RamAsset](ram_asset.md) or [RamShot](ram_shot)*: **item**,<br />• *[RamStep](ram_step.md)*: **step**,<br />• *string*: **filePath**,<br />• *dict*: **publishOptions**=`None`,<br />• *bool*: **showPublishOptions**=`false` | - `item` is the item from which the user wants to publish a file.<br />- `step` is the step from which the user is publishing. You can check `step.outputPipes()` to know which file types to publish.<br />- `filePath` is the path of the file currently being published.<br />- `publishOptions` is a dictionnary (associative array) containing custom options you can use to publish the item.<br />- `showPublishOptions` tells the script to show a UI or not to edit the publish options before publishing. |
| **replaceItem** | • *[RamItem](ram_item.md) or [RamAsset](ram_asset.md) or [RamShot](ram_shot)*: **item**,<br />• *string*: **filePath**,<br />• *[RamStep](ram_step.md)*: **step**=`None`,<br />• *dict*: **importOptions**=`None`,<br />• *bool*: **showImportOptions**=`false` | - `item` is the item from which the user wants to import a file.<br />- `filePath` is the absolute path to the file the user wants to import.<br />- `step` is the step from which the user wants to import. You can check `step.outputPipes()` to know which file types have been published, and `inputPipes()` from the step you're importing to to know which files to accept.<br />- `importOptions` is a dictionnary (associative array) containing custom options you can use to import the item.<br />- `showImportOptions` tells the script to show a UI or not to edit the import options before importing.<br />With this info, the method should replace selected objects with the imported items. |
| **updateStatus** | • *[RamItem](ram_item.md) or [RamAsset](ram_asset.md) or [RamShot](ram_shot)*: **item**,<br />• *[RamStatus](ram_status.md)*: **status**,<br />• *[RamStep](ram_step.md)*: **step** |  - `item` is the item which is being updated.<br />- `status` is the new status.<br />- `step` is the step being updated. |

## Examples

```py
# Python

# import ramses
import ramses as ram
# Get the instance
ramses = ram.Ramses.instance()

# A simple method
def published(item, filePath, step):
    ram.log("Hello, I've been published!")

# Another one
def aFunction( arg1, arg2):
    ram.log("I'm doing something")

# Adds the method to the scripts which will be run when the add-on publishes a file
ramses.publishScripts.append( published )

# Let's store a function in the userScripts to make it available everywhere
ramses.userScripts["MyFunction"] = aFunction

# The provided Ramses add-ons for Blender, Maya, etc. automatically trigger these scripts.
# If you're developping another addon, you have to call Ramses.publish() to run them
ramses.publish(currentItem, 'a path', 'STEP')

# From anywhere in the app where the Ramses module has been imported, you can call 'aFunction' with
ramses.userScripts["MyFunction"](some, arg)
```

```js
// ExtendScript

// include the Ramses lib
#include ramses.jsxinc
// Get the instance
var ramses = Ramses.instance();

// A simple method
function published(item, filePath, step)
{
    ram.log("Hello, I've been published!");
}

// Another one
function aFunction( arg1, arg2)
{
    ram.log("I'm doing something");
}

// Adds the method to the scripts which will be run when the add-on publishes a file
ramses.publishScripts.push( published );

// Let's store a function in the userScripts to make it available everywhere
ramses.userScripts["MyFunction"] = aFunction;

// The provided Ramses add-ons for Blender, Maya, etc. automatically trigger these scripts.
// If you're developping another addon, you have to call Ramses.publish() to run them
ramses.publish(currentItem, 'a path', 'STEP');

// From anywhere in the app, you can call 'aFunction' with
ramses.userScripts["MyFunction"](some, arg);
```

____

## API Dev notes

!!! note
    These section is for the development of the API only; you should not need these when developping your add-on using the API.

### Private attributes

| Attribute | Type | Default | Description |
| --- | --- | --- | --- |
| **defaultState** | *[RamState](ram_state.md)* | | The state to use as default ("WIP"). |
