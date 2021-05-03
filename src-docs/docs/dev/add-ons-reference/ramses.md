# Ramses

The main class. One (and only one) instance globally available (Ramses is a *singleton*), by calling `Ramses.instance()`.

[TOC]

## Static Attributes

| Attribute | Type | Default | Description |
| --- | --- | --- | --- |
| **apiReferenceUrl** | *string* | This page URL | A link to this API documentation |
| **addonsHelpUrl** | *string* | The URL to [this page](../../../addons/) | A link to the [Ramses add-ons](../../../addons/) documentation |
| **generalHelpUrl** | *string* | The URL to [this documentation](../../../) | A link to the [Ramses documentation](../../../) |

## Static Methods

| Method | Arguments | Description |
| --- | --- | --- |
| **instance**<br />▹ *Ramses* | | Returns the *Ramses* unique instance. |

### Examples

```py
# Python

# import the Ramses class
from ramses import Ramses
# Get the instance
ramses = Ramses.instance()
```

```js
// ExtendScript

// include the Ramses lib
#include ramses.jsxinc
// Get the instance
var ramses = Ramses.instance();
```

## Methods

| Method | Arguments | Description |
| --- | --- | --- |
| **alternativeFolderPaths**<br />▹ *string list* | | A list of alternative absolute paths to the main Ramses folder. Missing files will be looked for in these paths (and copied to the main path if available), and they will be used if the main path is not available. |
| **backupFolderPath**<br />▹ *string* | | A copy of the main folder where all files are stored. |
| **connect**<br />▹ *boolean* | | Checks *Daemon* availability and initiates the connection. <br />Returns success. |
| **currentProject**<br />▹ *RamProject* or *None* | | The current project. |
| **currentStep**<br />▹ *RamStep* or *None* | | The current step. |
| **currentUser**<br />▹ *RamUser* or *None* | | The current user. |
| **daemonInterface**<br />▹ *RamDaemonInterface* | | The *Daemon* interface. |
| **disconnect**<br />▹ *boolean* | | Gets back to offline mode. |
| **folderPath**<br />▹ *string* | | The absolute path to main Ramses folder, containing projects by default, config files, user folders, admin files... |
| **projects**<br />▹ *list of RamProject* | | The list of available projects. |
| **project**<br />▹ *RamProject* | *string*: **projectShortName** | Gets a specific project. |
| **states**<br />▹ *list of RamState* | | The list of available states. |
| **state**<br />▹ *RamState* | *string*: **stateShortName**=`WIP` | Gets a specific state. |
| **online**<br />▹ *boolean* | | True if connected to the *Daemon* and the *Daemon* is responding. |
| **showClient** | | Raises the *Ramses Client* window, launches the client if it is not already running. |
| **settings** | [*RamSettings*](ram_settings.md) |  | The settings. |
| **version**<br />▹ *string* |  | The current version of this API |