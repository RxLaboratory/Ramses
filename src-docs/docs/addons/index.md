# Add-ons

Ramses comes with some add-ons to be used in your creative applications (*Blender*, *Maya*, *After Effects*...). They are basic add-ons to demonstrate some capabilities of Ramses for file versionning and asset management, and can be used as templates to develop your own add-ons working with Ramses.

They will be developed one after each other, and you're welcome to contribute with your own!

Here are some guidelines and descriptions about how these add-ons should work. We're trying to apply the same guidelines to all host applications for a unified experience. These add-ons should use the provided [*Scripting API*](../dev/add-ons-reference/index.md) available at first in *Python* and *Adobe ExtendsScript*.

[TOC]

## Introduction

Although the add-ons are primarily meant to be used in conjunction with the *Ramses Client Application* (which in turn may be connected to the *Ramses Server*), they can also be used on their own. In which case not all functionnalities will be available; the differences are mentionned in this documentation.

## Available Commands

Some of these commands manage files in the [*Ramses Tree*](../files/tree.md), see the [corresponding documentation](../files/tree.md).

All commands except *open Ramses* and *Settings* should first:

- If the add-on is not set to offline by the user (in the settings):
    - If the path to the *Client* executable file is invalid: warn the user and stop.
    - If the user is not connected, warn the user and stop

### Open

// TODO

### Save

If the file is not a working file, but a version coming from the *ramses_version* subfolder (named `restore-vXX`), runs *Save new version* instead of *Save*.

- If the file is a template ("template" in the resource name, and in a temp dir)
    - Asks for the name of the new asset/shot, its group, its sequence
    - Creates the corresponding folder,
    - Saves (as new version if the asset/shot already exists, with a warning)
- Else If the file is in the *ramses_version* folder or if its resource name contains *restore-vXX*.
    - Runs *Save new version*.
- Else
    - Saves the current file.
    - Copies it to the *ramses_versions* subfolder, appending the current state and version number.
- If *online*
    - Send the new status of the asset/shot for this step (version, date, state, comment, completion ratio) to the *Ramses Client*.

See the [naming scheme](../files/naming.md) and [folder structure](../files/tree.md) for more information.

### Save new version

- If the file is in the *ramses_version* folder or if its resource name contains *restore-vXX*.
    - Copies the current working file into the version folder, appending the state got from the *Ramses client* if available, or else just "v", and the incremented version number.
    - Saves the current file as the working file (in the working folder, and removing *restore-XX* from the resource name)
    - Removes the current file (version file, keeps only the new working file) if it was not in a *ramses_versions* subfolder
- Else
    - Saves the current file.
- Copies the current file to the *ramses_versions* subfolder, appending the current state and the incremented version number.
- *If the client is available*: sends the *update* command to the client, with the date, state and version of the current asset/shot. 

### Publish

- Saves the current file.
- Copies it to the *ramses_versions* subfolder, appending the current state and the incremented version number.
- Saves as a new file into the *published* subfolder.
- *If the client is available*: sends the *publish* command to the client, with the date, state and version of the current asset/shot.

### Retrieve version

- Gets a specific version back from the *ramses_versions* subfolder.
- Copies the file to the asset/shot folder, adds`restore-vXX` to the resource name, where `XX` is the restored version number.
- Opens it.

It is only when the file will be saved that this new copied file will be removed and the working file will be overwritten to a new version.

### Publish as template

- Saves the file in the corresponding step folder (in a "templates" subfolder), using a specific naming: `PROJECTID_G_stepID_template-resourceName.ext`
- (asks for overwrite if the file exists)
- Saves the file in a temp folder (to never overwrite a template when Ctrl+S)

### Open template

- Lists the available templates
- Opens the selected one
- Saves it in a temp folder

### Import template

- Lists the available templates
- Imports the selected one

### Settings

- Opens the settings editor of the Add-on.

### Open Ramses

- Opens the *Ramses Client* if available.
- Try to connect to the *Daemon*.
- Warns the user if not available/cannot connect to the *Daemon*.
- If the *Daemon* is available, sets the mode to "online". If not, sets to "offline".

## UI / UX

Depending on the host application, the implementation and User Interface / User Experience of the add-ons may vary, but they should follow these guidelines as closely as possible.

### Integrated commands

When possible, the commands should be integrated as seamlessly as possible in the host application, e.g. by adding or even replacing menu items for saving files, saving as., etc.

Especially for the *save* command, the default *save* shortcut (generally `[Ctrl] + [S]`) should be replaced by the *save* command for *Ramses*. Other *save* command shortcuts should be added, preferably the same for all add-ons:

- *Save*: `[Ctrl] + [S]`
- *Save new version*: `[Ctrl] + [Alt] + [S]`
- *Publish*: `[Ctrl] + [Shift] + [P]`

These shortcuts have been chosen to avoid conflicts in most host applications but can be changed to something else if they're still conflicting with something important.

### UI Panel

If possible, each *Ramses Add-on* should provide a UI panel integrated into the host software, but organized similarly in all add-ons, with specific variations to respect the host software UI guidelines.

The panel must display buttons for all available commands.

The panel must also provide a way to quickly set the current *Project* and *Step*, and show the opened asset or shot information: state, version, date, comment, completion ratio... This information is retrieved from the client application if available, or from the [*Ramses Tree*](../files/tree.md) if not. The panel and this information must be updated as soon as the opened file changes, or at least when the user interacts with it.

Information available in the panel:

- Current user (online only)
- Current project
- Step
- Current shot or asset
    - Containing asset group or sequence (sequence available only if online)
    - Date of latest version (read only)
    - Current state
    - Current resource name
    - Current version (read only)
    - Completion ratio
    - Is template (read only)

If the client application is not available, the *Project* and *Step* information is displayed but cannot be changed. *Comment* is not available.

### Settings

If the host application provides a way to display the settings of its add-ons, the settings should be located there (and the *settings* command should open this location if possible). If it does not, the Add-on must provide a dialog or a panel to let the user change the settings.

These settings are:

- Location of the *Ramses Client* executable file (*.exe* on *Windows*, *.app* on *MacOS*, *.appimage* or binary on *Linux*)
- Listening port of the *Ramses Daemon*

Note: the settings are saved in a shared location (user documents folder ?) between all add-ons so that they're configured only once. They're in an easy-to-edit JSON text file.