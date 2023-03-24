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

Read the section entitled *Events and Handlers* below (and see the *Examples*) for more information about the methods you can add to these lists.

| Attribute | Type | Default | Description |
| --- | --- | --- | --- |
| **importScripts** | *list* | `[]` | A list of functions to be triggered when `Ramses.instance().importItem()` is called.<br />If you're using one of the provided Add-ons, you can add your own handlers to this list so they're run when the user asks to import an item.<br />Note: official *Ramses Add-ons* will add an item to this list to provide a default implementation. If you're extending an existing add-on, you can reset the list or insert/append your own methods to keep the default behavior. |
| **openScripts** | *list* | `[]` | A list of functions to be triggered when `Ramses.instance().openFile()` is called.<br />If you're using one of the provided Add-ons, you can add your own handlers to this list so they're run when the user opens an item in the host application.<br />Note: official *Ramses Add-ons* will add an item to this list to provide a default implementation. If you're extending an existing add-on, you can reset the list or insert/append your own methods to keep the default behavior. |
| **publishScripts** | *list* | `[]` | A list of functions to be triggered when `Ramses.instance().publish()` is called.<br />If you're using one of the provided Add-ons, you can add your own handlers to this list so they're run when the user publishes the current file from the host application.<br />Note: official *Ramses Add-ons* will add an item to this list to provide a default implementation. If you're extending an existing add-on, you can reset the list or insert/append your own methods to keep the default behavior. |
| **replaceScripts** | *list* | `[]` | A list of functions to be triggered when `Ramses.instance().replaceItem()` is called.<br />If you're using one of the provided Add-ons, you can add your own handlers to this list so they're run when the user asks to import an item.<br />Note: official *Ramses Add-ons* will add an item to this list to provide a default implementation. If you're extending an existing add-on, you can reset the list or insert/append your own methods to keep the default behavior. |
| **saveScripts** | *list* | `[]` | A list of functions to be triggered when `Ramses.instance().saveFile()` is called.<br />If you're using one of the provided Add-ons, you can add your own handlers to this list so they're run when the user saves the current file in the host application.<br />Note: official Ramses Add-ons will add an item to this list to provide a default implementation. If you’re extending an existing add-on, you can reset the list or insert/append your own methods to keep the default behavior |
| **statusScripts** | *list* | `[]` | A list of functions to be triggered when `Ramses.instance().updateStatus()` is called.<br />If you're using one of the provided Add-ons, you can add your own handlers to this list so they're run when the user changes the current status of an asset / shot from the host application. |
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
| **importItem** | • *[RamItem](ram_item.md) or [RamAsset](ram_asset.md) or [RamShot](ram_shot)*: **item**,<br />• *string*: **filePath**,<br />• *[RamStep](ram_step.md)*: **step**=`None`,<br />• *dict*: **importOptions**=`None`,<br />• *bool*: **showImportOptions**=`false` | Runs the scripts in `Ramses.instance().importScripts`.<br />Read the section entitled *Events and Handlers* below for more details. |
| **openFile** | • *string*: **filePath**,<br />• *[RamItem](ram_item.md) or [RamAsset](ram_asset.md) or [RamShot](ram_shot)*: **item**=`None`,<br />• *[RamStep](ram_step.md)*: **step**=`None` | Runs the scripts in `Ramses.instance().openScripts`.<br />Read the section entitled *Events and Handlers* below for more details. |
| **project**<br />▹ *[RamProject](ram_project.md)* or *None* | | (Try to) retrieve a project using its short name. |
| **projects**<br />▹ *list of [RamProject](ram_project.md)* | | The list of available projects. |
| **projectsPath**<br />▹ *string* | | The path to the folder containing projects. |
| **states**<br />▹ *list of [RamState](ram_state.md)* | | The list of available states. |
| **state**<br />▹ *[RamState](ram_state.md)* | • *string*: **stateShortName**=`WIP` | Gets a specific state. |
| **online**<br />▹ *boolean* | | True if connected to the *Daemon* and the *Daemon* is responding. |
| **publish** | • *[RamItem](ram_item.md) or [RamAsset](ram_asset.md) or [RamShot](ram_shot)*: **item**,<br />• *[RamStep](ram_step.md)*: **step**,<br />• *string*: **filePath**,<br />• *dict*: **publishOptions**=`None`,<br />• *bool*: **showPublishOptions**=`false` | Runs the scripts in `Ramses.instance().publishScripts`.<br />Read the section entitled *Events and Handlers* below for more details. |
| **replaceItem** | • *[RamItem](ram_item.md) or [RamAsset](ram_asset.md) or [RamShot](ram_shot)*: **item**,<br />• *string*: **filePath**,<br />• *[RamStep](ram_step.md)*: **step**=`None`,<br />• *dict*: **importOptions**=`None`,<br />• *bool*: **showImportOptions**=`false` | Runs the scripts in `Ramses.instance().replaceScripts`.<br />Read the section entitled *Events and Handlers* below for more details. |
| **saveFile**<br />▹ *int* | • *string*: **filePath**,<br />• *[RamItem](ram_item.md) or [RamAsset](ram_asset.md) or [RamShot](ram_shot)*: **item**=`None`,<br />• *[RamStep](ram_step.md)*: **step**=`None`,<br />• *bool*: **incrementVersion**=`false`<br />• *stirng*: **comment**=`None`<br />• *string*:**newStateShortName**:`None` | Runs the scripts in `Ramses.instance().saveScripts`.<br />Read the section entitled *Events and Handlers* below for more details.<br />This method returns an error code:<br />• `-1`: One of the event handlers interrupted the process.<br />• `0`: OK.<br />• `1`: Malformed file name, can't get the item from the file. The file was not saved.<br />• `2`: The file was a restored version, it's been incremented then saved as a working file.<br />• `3`: The file was misplaced (in a reserved folder), and was incremented and saved in the right place.<br />• `4`: The file was too old (according to RamSettings.autoIncrementTimeout) and was incremented and saved. |
| **showClient** | | Raises the *Ramses Client* window, launches the client if it is not already running. |
| **setCurrentProject** | • *[RamProject](ram_project.md)*: **project** | Sets the current project. |
| **settings**<br />▹ [*RamSettings*](ram_settings.md) | | The settings unique instance. Same as `RamSettings.instance()` |
| **updateStatus** | • *[RamItem](ram_item.md) or [RamAsset](ram_asset.md) or [RamShot](ram_shot)*: **item**,<br />• *[RamStatus](ram_status.md)*: **status**,<br />• *[RamStep](ram_step.md)*: **step** |  Runs the scripts in `Ramses.instance().statusScripts`.<br />Read the section entitled *Events and Handlers* below for more details. |
| **usersPath**<br />▹ *string* | |  The path to the folder containing users. |

## Events and Handlers

Some events are triggered by Ramses on specific actions, like opening a file in the host application or importing an asset. *Handlers* are functions to be run when these events are triggered. For each of these events, you can add your own handlers if you're developping an add-on, and the end-user can also register script files containing handlers for these events.

- **When developping the add-on**: add the handler function to the corresponding list, like `Ramses.openScripts` or `Ramses.importScripts` for example.
- **Adding a user script**: add script files to the settings (with the UI of the add-on if available, or programmatically with [`RamSettings.userScripts`](ram_settings.md)). These script files must define a function for the desired event(s) named after the event.

There are two types of handler functions per event:

- **`before_event`** handlers are run before the default methods of the add-on (before all methods stored in `Ramses.eventScripts`).
- **`on_event`** handlers are run after the default methods of the add-on.

!!! warning
    If you're forking/modifying an official add-on, the events are automatically triggered, but if you're developing an add-on from scratch, you need to call the corresponding trigger methods in the `Ramses` class to process all registered handlers.

*Ramses* will pass a few arguments depending on the context to these handler functions.

Handlers must return `true` or nothing to let the process continue, and can return `false` to stop the process (the event propagation) and the next registered handlers.

| Event | Arguments | Description |
| --- | --- | --- |
| **`before_import_item`** \| **`on_import_item`**<br/>Trigger function: `Ramses.importItem()`<br/>Handler function list: `Ramses.importScripts` | • *[RamItem](ram_item.md) or [RamAsset](ram_asset.md) or [RamShot](ram_shot)*: **item**,<br />• *string*: **filePaths**,<br />• *[RamStep](ram_step.md)*: **step**=`None`,<br />• *dict*: **importOptions**=`None`,<br />• *bool*: **showImportOptions**=`false` | • `item` is the item from which the user wants to import a file.<br />• `filePaths` is the list of absolute path to the files the user wants to import. This list can be empty! In this case, it means the add-on must automatically choose the right file from the publish folder of the item and step.<br />• `step` is the step from which the user wants to import. You can check `step.outputPipes()` to know which file types have been published, and `inputPipes()` from the step you're importing to to know which files to accept.<br />• `importOptions` is a dictionnary (associative array) containing custom options you can use to import the item.<br />• `showImportOptions` tells the script to show a UI or not to edit the import options before importing. |
| **`before_open`** \| **`on_open`**<br/>Trigger function: `Ramses.openFile()`<br/>Handler function list: `Ramses.openScripts` | • *[RamItem](ram_item.md) or [RamAsset](ram_asset.md) or [RamShot](ram_shot)*: **item**,<br />• *string*: **filePath**,<br />• *[RamStep](ram_step.md)*: **step**=`None` | • `item` is the item the user has opened.<br />• `filePath` is the absolute path of the file being opened.<br />• `step` is the step being opened. |
| **`before_publish`** \| **`on_publish`**<br/>Trigger function: `Ramses.publish()`<br/>Handler function list: `Ramses.publishScripts` | • *[RamItem](ram_item.md) or [RamAsset](ram_asset.md) or [RamShot](ram_shot)*: **item**,<br />• *[RamStep](ram_step.md)*: **step**,<br />• *string*: **filePath**,<br />• *dict*: **publishOptions**=`None`,<br />• *bool*: **showPublishOptions**=`false` | • `item` is the item from which the user wants to publish a file.<br />• `step` is the step from which the user is publishing. You can check `step.outputPipes()` to know which file types to publish.<br />• `filePath` is the path of the file currently being published.<br />• `publishOptions` is a dictionnary (associative array) containing custom options you can use to publish the item.<br />• `showPublishOptions` tells the script to show a UI or not to edit the publish options before publishing. |
| **`before_replace_item`** \| **`on_replace_item`**<br/>Trigger function: `Ramses.replaceItem()`<br/>Handler function list: `Ramses.replaceScripts` | • *[RamItem](ram_item.md) or [RamAsset](ram_asset.md) or [RamShot](ram_shot)*: **item**,<br />• *string*: **filePath**,<br />• *[RamStep](ram_step.md)*: **step**=`None`,<br />• *dict*: **importOptions**=`None`,<br />• *bool*: **showImportOptions**=`false` | • `item` is the item from which the user wants to import a file.<br />• `filePath` is the absolute path to the file the user wants to import.<br />• `step` is the step from which the user wants to import. You can check `step.outputPipes()` to know which file types have been published, and `inputPipes()` from the step you're importing to to know which files to accept.<br />• `importOptions` is a dictionnary (associative array) containing custom options you can use to import the item.<br />• `showImportOptions` tells the script to show a UI or not to edit the import options before importing.<br />With this info, the method should replace selected objects with the imported items. |
| **`before_save`** \| **`on_save`**<br/>Trigger function: `Ramses.saveFile()`<br/>Handler function list: `Ramses.saveScripts`<br />▹ *bool* | • *[RamItem](ram_item.md) or [RamAsset](ram_asset.md) or [RamShot](ram_shot)*: **item**,<br />• *string*: **filePath**,<br />• *[RamStep](ram_step.md)*: **step**,<br />• *int*: **version**,<br />• *string*: **comment**,<br />• *bool*: **incremented** | • `item` is the item the user has saved.<br />• `filePath` is the absolute path of the file being saved.<br />• `step` is the step being saved.<br />• `version` is the version number for the file being saved.<br />• `comment` is the comment associated to this version of the file.<br />• `incremented` is `true` if this is a new version. |
| **`before_update_status`** \| **`on_update_status`**<br/>Trigger function: `Ramses.updateStatus()`<br/>Handler function list: `Ramses.statusScripts` | • *[RamItem](ram_item.md) or [RamAsset](ram_asset.md) or [RamShot](ram_shot)*: **item**,<br />• *[RamStatus](ram_status.md)*: **status**,<br />• *[RamStep](ram_step.md)*: **step** |  • `item` is the item which is being updated.<br />• `status` is the new status.<br />• `step` is the step being updated. |

### Examples

#### Adding handlers with a user script file

Users can add their own handlers for specific events by just storing them in script file(s) and implementing a function named after the event(s).  
This is a simple example of a file like this containing a few handlers.

```py
# handlers.py

def on_open( item, file_path, step):
    """Triggered when an item is opened by the user"""

    if step.shortName() == "LIGHT":
        # Do Something here if it's a lighting step being opened
        doSomething(item)
        print("We've opened the Lighting step for" + item.name())
    else:
        print("Nothing to do for unknown steps...")

def on_publish( item, step, file_path, import_options, show_import_options):
    """Triggered when an item is published"""

    if step.shortName() == "MOD":
        # Show options to the user
        if show_import_options:
            options_dialog = show_options_ui()
            import_options = options_dialog.getResult()
        # Do Something here if it's a modeling step being published
        doSomething(item, import_options)
        print("We've published the Modeling step for" + item.name())
    else:
        print("Nothing to do for unknown steps...")
```

```js
// handlers.jsx

// Triggered when an item is opened by the user
function on_open( item, file_path, step) {
    
    if (step.shortName() == "LIGHT") {
        // Do Something here if it's a lighting step being opened
        doSomething(item);
        $.writeln("We've opened the Lighting step for" + item.name());
    }
    else {
        $.writeln("Nothing to do for unknown steps...");
    }
}

// Triggered when an item is published
function on_publish( item, step, file_path, import_options, show_import_options):

    if (step.shortName() == "MOD") {
        // Show options to the user
        if (show_import_options) {
            var options_dialog = show_options_ui();
            import_options = options_dialog.getResult();
        }
        // Do Something here if it's a modeling step being published
        doSomething(item, import_options);
        $.writeln("We've published the Modeling step for" + item.name());
    }
    else{
        $.writeln("Nothing to do for unknown steps...");
    }

```

These script files can be registered using the UI of the add-on or by adding them in the settings programmatically like this:

```py
# Python

# import ramses
import ramses as ram
# Get the instance
ramses = ram.Ramses.instance()

# The script containing the handlers
script_file = "/path/to/my/handlers.py"
# Register
settings = ramses.settings()
settings.userScripts.append( script_file )
settings.save()

# All handlers in the script file are now registered and will be run automatically with corresponding events
```

```js
// ExtendScript

// include the Ramses lib
#include ramses.jsxinc
// Get the instance
var ramses = Ramses.instance();

// The script containing the handlers
var scriptFile = "/path/to/my/handlers.py";
// Register
var settings = ramses.settings();
settings.userScripts.push( scriptFile );
settings.save();

// All handlers in the script file are now registered and will be run automatically with corresponding events
```

#### Adding handlers when implementing/extending an add-on

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
