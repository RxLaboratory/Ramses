# RamDaemonInterface

The low-level Class used to communicate with the Ramses Daemon. This is a simple wrapper for the [*Daemon API*](../../daemon-reference/).

One (and only one) instance globally available (RamDaemonInterface is a *singleton*), by calling `RamDaemonInterface.instance()` or `Ramses.instance().daemonInterface()`.

!!! note
    You should not need this class as all communications are automatically handled by the other classes.

## Static Methods

| Method | Arguments | Description | Dev Notes |
| --- | --- | --- | --- |
| **instance**<br />▹ *RamDaemonInterface* | | Returns the *RamDaemonInterface* unique instance. |
| **checkReply**<br />▹ *boolean* | *dict*: **reply** | Checks if the reply returned by a *RamDaemonInterface* instance is accepted, successful and has some content. |  |


## Methods

Please refer to the [*Daemon API* reference](../../daemon-reference/) for more information about these methods.

These methods return the reply of the *Daemon* converted from *JSON* to an object, dict, structure, or whatever best suits the language in which the add-on is implemented. This returned object can be converted to a [RamObject](ram_object.md) instance using the corresponding `RamObject.fromDict()` static method.

| Method | Arguments | Description | Dev Notes |
| --- | --- | --- | --- |
| ***constructor*** | *integer*: **port**=`18185` | |
| **daemonVersion**<br />▹ *string* |  | The current version of the *Daemon* | *TODO: change the `ping` method to store the version in a private attribute* |
| **getAssets**<br />▹ *Object / Dict* | | Gets the list of the assets for the current project |
| **getAsset**<br />▹ *Object / Dict* | *string*:**shortName**,<br />*string*:**name**=`""` | Gets a specific asset. The name is optional, as short names *should* be unique. | |
| **getAssetGroups***<br />▹ *Object / Dict* | | Gets the list of the asset groups for the current project | |
| **getCurrentProject**<br />▹ *Object / Dict* | | Gets the current project | |
| **getCurrentStatus**<br />▹ *Object / Dict* | *string*: **itemShortName**,<br />*string* : **itemName**,<br />*string*: **step**,<br />*ItemType*: **itemType**=`ItemType.SHOT` | Gets the status for the specific step for the item. `itemType` must be one of: `ItemType.SHOT` or `ItemType.ASSET` | |
| **getCurrentStatuses**<br />▹ *Object / Dict* | *string*: **itemShortName**,<br />*string* : **itemName**,<br />*string*: **step**,<br />*ItemType*: **itemType**=`ItemType.SHOT` | Gets the list of status (one per step) for the item. `itemType` must be one of: `ItemType.SHOT` or `ItemType.ASSET` | |
| **getCurrentUser**<br />▹ *Object / Dict* | | Gets the current user | |
| **getProjects**<br />▹ *Object / Dict* | | Gets the list of available projects |
| **getProject**<br />▹ *Object / Dict* | *string*:**shortName**,<br />*string*:**name**=`""` | Gets a specific project. The name is optional, as short names *should* be unique. | |
| **getRamsesFolderPath**<br />▹ *Object / Dict* | | Gets the main Ramses folder containing projects, config files, user files... | |
| **getPipes**<br />▹ *Object / Dict* | | Gets the list of pipes for the current project | TODO |
| **getShots**<br />▹ *Object / Dict* | *string*: **filter**=`""` | Gets the list of the shots for the current project. Filter is a way to get a subset of the shots: the daemon will return only shots containing the filter in their name or short name. The filter can contain a wildcard `"*"` | |
| **getShot**<br />▹ *Object / Dict* | *string*:**shortName**,<br />*string*:**name**=`""` | Gets a specific shot. The name is optional, as short names *should* be unique. | |
| **getStates**<br />▹ *Object / Dict* | | Gets the list of available states |
| **getState**<br />▹ *Object / Dict* | *string*:**shortName**,<br />*string*:**name**=`""` | Gets a specific state. The name is optional, as short names *should* be unique. | |
| **getSteps**<br />▹ *Object / Dict* | | Gets the list of the steps for the current project |
| **getStep**<br />▹ *Object / Dict* | *string*:**shortName**,<br />*string*:**name**=`""` | Gets a specific step. The name is optional, as short names *should* be unique. | |
| **online**<br />▹ *boolean* | | True if the *Daemon* is available |
| **ping**<br />▹ *Object / Dict*  | | Gets the version and current user of the *Ramses Daemon* |
| **publishAsset**<br />▹ *Object / Dict*  | | | TODO |
| **publishShot**<br />▹ *Object / Dict*  | | | TODO |
| **raiseWindow** | | Raises the Ramses Client application main window |
| **setCurrentProject**<br />▹ *Object / Dict*  | *string*: **shortName** | Sets the current project |
| **setAssetStatus**<br />▹ *Object / Dict*  | | | TODO |
| **setShotStatus**<br />▹ *Object / Dict*  | | | TODO |

## Examples

```py
# Python
daemon = RamDaemonInterface.instance()
reply = daemon.getProjects() # Call from the instance
if RamDaemonInterface.checkReply(reply): # Call from the class, it's a static method
    doSomething(reply['content'])
else:
    print (reply['message'])
```

```js
// JavaScript
var daemon = RamDaemonInterface.instance();
var reply = daemon.getProjects(); // Call from the instance
if ( RamDaemonInterface.checkReply(reply) ) { // Call from the class, it's a static method
    doSomething(reply['content']);
}
else {
    alert (reply['message']);
}
```

## Implementation

### Python

In Python, the methods from this class return a `dict` as constructed from the *JSON* reply of the *Daemon* using the standard `json.loads( str )` method.

### JavaScript / ExtendScript

In JS, the methods from this class return a standard JS object as constructed from the *JSON* reply of the *Daemon* using the standard `json2` library.

____

![META](authors:Nicolas "Duduf" Dufresne;medias:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2021;updated:2021/05/25)