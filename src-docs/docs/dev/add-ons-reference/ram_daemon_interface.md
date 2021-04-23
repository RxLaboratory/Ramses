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

## Static Methods

| Method | Arguments | Description | Dev Notes |
| --- | --- | --- | --- |
| **ckeckReply**<br />▹ *boolean* | *dict*: **reply** | Checks if the reply returned by a *RamDaemonInterface* instance is accepted, successful and has some content. |  |

### Examples

- `checkReply`

```py
# Python
daemon = Ramses.instance.daemonInterface()
reply = daemon.getProjects() # Call from the instance
if RamDaemonInterface.checkReply(reply): # Call from the class, it's a static method
    doSomething(reply['content'])
else:
    print (reply['message'])
```

```js
// JavaScript
var daemon = Ramses.instance.daemonInterface();
var reply = daemon.getProjects(); // Call from the instance
if ( RamDaemonInterface.checkReply(reply) ) { // Call from the class, it's a static method
    doSomething(reply['content']);
}
else {
    alert (reply['message']);
}
```

## Methods

Please refer to the [*Daemon API* reference](../../daemon-reference/) for more information about these methods.

These methods return the reply of the *Daemon* converted from *JSON* to an object, dict, structure, or whatever best suits the language in which the add-on is implemented.

| Method | Arguments | Description | Dev Notes |
| --- | --- | --- | --- |
| ***constructor*** | *integer*: **port**=`18185` | |
| **daemonVersion**<br />▹ *string* |  | The current version of the *Daemon* | *TODO: change the `ping` method to store the version in a private attribute* |
| **getAssets**<br />▹ *Object / Dict* | | Gets the list of the assets for the current project |
| **getCurrentProject**<br />▹ *Object / Dict* | | Gets the current project | |
| **getCurrentUser**<br />▹ *Object / Dict* | | Gets the current user | |
| **getProjects**<br />▹ *Object / Dict* | | Gets the list of available projects |
| **getPipes**<br />▹ *Object / Dict* | | Gets the list of pipes for the current project | TODO |
| **getShots**<br />▹ *Object / Dict* | | Gets the list of the shots for the current project |
| **getStates**<br />▹ *Object / Dict* | | Gets the list of available states |
| **getSteps**<br />▹ *Object / Dict* | | Gets the list of the steps for the current project |
| **online**<br />▹ *boolean* | | True if the *Daemon* is available |
| **ping**<br />▹ *Object / Dict*  | | Gets the version and current user of the *Ramses Daemon* |
| **publishAsset**<br />▹ *Object / Dict*  | | | TODO |
| **publishShot**<br />▹ *Object / Dict*  | | | TODO |
| **raiseWindow** | | Raises the Ramses Client application main window |
| **setCurrentProject**<br />▹ *Object / Dict*  | *string*: **uuid** | Sets the current project |
| **updateShotStatus**<br />▹ *Object / Dict*  | | | TODO |
| **updateAssetStatus**<br />▹ *Object / Dict*  | | | TODO |

## Implementation

### Python

In Python, the methods from this class return a `dict` as constructed from the *JSON* reply of the *Daemon* using the standard `json.loads( str )` method.

### JavaScript / ExtendScript

In JS, the methods from this class return a standard JS object as constructed from the *JSON* reply of the *Daemon* using the standard `json2` library.

____

![META](authors:Nicolas "Duduf" Dufresne;medias:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2021;updated:2021/04/22)