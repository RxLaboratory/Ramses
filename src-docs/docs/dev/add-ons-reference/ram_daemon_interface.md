# RamDaemonInterface

The low-level Class used to communicate with the Ramses Daemon. This is a simple wrapper for the [*Daemon API*](../../daemon-reference/).

!!! note
    You should not need this class as all communications are automatically handled by the other classes.

There should be only one instance of the *Daemon Interface*, available with the [`Ramses.instance.daemonInterface()`](ramses.md) method.

[TOC]

## Attributes

| Attribute | Type | Default | Description |
| --- | --- | --- | --- |
| **port** | *integer* | `18185` | Listening port of the *Ramses Daemon*. Set by [RamSettings](ram_settings.md) |
| **address** | *string* | `"localhost"` | The IP or host name of the *Ramses Daemon*. Should never be changed unless you know what you're doing. By default, the *Daemon* only listens to *localhost / 127.0.01* |

## Methods

Read the [*Daemon API* reference](../../daemon-reference/) for more information about these methods.

These methods return the reply of the *Daemon* converted from *JSON* to an object, dict, structure, or whatever best suits the language in which the add-on is implemented.

| Method | Arguments | Description |
| --- | --- | --- |
| ***constructor*** | *integer*: **port**=`18185` | |
| **getProjects**<br />▹ *Object / Dict* | TODO | Gets the list of available projects |
| **getStates**<br />▹ *Object / Dict* | TODO | Gets the list of available states |
| **online**<br />▹ *boolean* | | True if the *Daemon* is available |
| **ping**<br />▹ *Object / Dict*  | | Gets the version and current user of the *Ramses Daemon*. |
| **publishAsset**<br />▹ *Object / Dict*  | TODO | |
| **publishShot**<br />▹ *Object / Dict*  | TODO | | |
| **raiseWindow** | | Raises the Ramses Client application main window |
| **setCurrentProject**<br />▹ *Object / Dict*  | *string*: **uuid** | Sets the current project |
| **updateShotStatus**<br />▹ *Object / Dict*  | TODO | |
| **updateAssetStatus**<br />▹ *Object / Dict*  | TODO | |
| **daemonVersion**<br />▹ *string* |  | The current version of the *Daemon* |

## Implementation

### Python

In Python, the methods from this class return a `dict` as constructed from the *JSON* reply of the *Daemon* using the standard `json.loads( str )` method.

### JavaScript / ExtendScript

In JS, the methods from this class return a standard JS object as constructed from the *JSON* reply of the *Daemon* using the standard `json2` library.