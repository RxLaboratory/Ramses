# RamDaemonInterface

This is a low-level Class used to communicate with the Ramses Daemon. This is a simple wrapper for the [*Daemon API*](../../daemon-reference/).

One (and only one) instance is globally available (RamDaemonInterface is a *singleton*), by calling `RamDaemonInterface.instance()` or `Ramses.instance().daemonInterface()`.

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
| **create**<br />▹ *Object / Dict* | • *string*: **uuid**<br/>• *string* or *Object/ Dict*: **data**<br/>• *string*: **objectType** | Creates a new object, with the given data | |
| **getCurrentProject**<br />▹ [*RamProject*](ram_project.md) or *None* | | Gets the current project | |
| **getCurrentUser**<br />▹ [*RamUser*](ram_user.md) or *None* | | Gets the current user | |
| **getData**<br />▹ *Object / Dict* | • *string*: **uuid** | Gets the data of a specific [*RamObject*](ram_object.md). | |
| **getObjects**<br />▹ *list of [*RamObject*](ram_object.md)* | • *string*: **objectType** | Gets the list of objects of a given type. The returned class type depends on the type of object and may be a derived class of *RamObject*. |
| **getPath**<br />▹ *string* | • *string*: **uuid** | Gets the path for a specific object. |
| **getProjects**<br />▹ *list of [*RamProject*](ram_project.md)* | | Gets the list of available projects (for the current user) |
| **getRamsesFolderPath**<br />▹ *string* | | Gets the main Ramses folder containing projects, config files, user files... | |
| **online**<br />▹ *boolean* | | True if the *Daemon* is available |
| **ping**<br />▹ *Object / Dict*  | | Gets the version and current user of the *Ramses Daemon* |
| **raiseWindow** | | Raises the Ramses Application main window |
| **setCurrentProject**<br />▹ *Object / Dict*  | • *string*: **uuid** | Sets the current project |
| **setData**<br />▹ *Object / Dict* | • *string*: **uuid**<br/>• *string* or *Object/ Dict*: **data** | Sets the data for a specific [*RamObject*](ram_object.md). | |
| **uuidFromPath**<br />▹ *string* | • *string*: **path**<br/>• *string* : **type** = `"RamObject"` | Gets the *UUID* of a [*RamObject*](ram_object.md) using its path. | |

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