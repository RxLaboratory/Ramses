# Ramses

The main class. One (and only one) instance globally available, instantiated during init time.

[TOC]

## Attributes

| Attribute | Type | Default | Description |
| --- | --- | --- | --- |
| **autoConnect** | *boolean* | `true` | Wether to always try to (re)connect if offline. |
| **clientPort** | *integer* | `1818` | The port used to connect to the client. |
| **currentProject**<br />*Read-only* | *RamProject* | | The current project. |
| **currentUser**<br />*Read-only* | *RamUser* | | The current user. |
| **online**<br/>*Read-only* | *boolean* | | True if connected to the client and the client is responding. |

## Methods

| Method | Arguments | Description |
| --- | --- | --- |
| ***constructor*** | *integer*: **port**=`1818`<br />*boolean*: **connect**=`true` | If connect is true, will launch the client if needed and then tries to connect to it. |
| **backupFolderPath**<br />▹ *string* | | A copy of the main folder where all files are stored. |
| **connect**<br />▹ *boolean* | | Checks server or client availability and initiates the connection. Returns success. |
| **disconnect**<br />▹ *boolean* | | Gets back to offline mode. |
| **getAlternativeFolderPaths**<br />▹ *string list* | | A list of alternative absolute paths to the main Ramses folder. Missing files will be looked for in these paths (and copied to the main path if available), and they will be used if the main path is not available. |
| **getProjects**<br />▹ *list of RamProject* | | The list of available projects. |
| **getStates**<br />▹ *list of RamState* | | The list of available states. |
| **getState**<br />▹ *RamState* | *string*: **stateShortName**=`WIP` | |
| **getSteps**<br />▹ *list of RamStep* | *enumerated value*: **typeOrCat**=`ALL` | Use *typeOrCat* to filter the results.<br />One of: `ALL`, `ASSET`, `SHOT`, `PRE-PROD`, `PROD`, `POST-PROD`.<br />`PROD` represents a combination of `SHOT` and `ASSET` |
| **getUsers**<br />▹ *list of RamUser* | | The list of available users. |
| **getUser**<br />▹ *RamUser* | *string*: **userShortName**=`""` | Gets a specific user. |
| **folderPath**<br />▹ *string* | | The absolute path to main Ramses folder, containing projects by default, config files, user folders, admin files... |
| **launchClient**<br />▹ *boolean* | *boolean*: **connect**=`true`| Launches the Ramses client and tries to connect to it. Returns success |
| **login**<br />▹ *boolean* | *string*: **userPassword**<br />*RamUser*: **user**=`currentUser` | Logs the user in. Launches and connect to the client if necessary.  Returns success. |
| **request**<br />▹ *None or dict* | *dict*: **request**<br /> | Posts a request to the connected client. |
| **logout**| | Logs the user out. |
| **setCurrentProject** | *[RamProject](ram_project.md)*: **project** | |
| **setCurrentUser** | *[RamUser](ram_user.md)*: **user** | |