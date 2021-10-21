# RamStatus

A state associated to a comment, the user who changed the state, etc.

## Static Methods

| Method | Arguments | Description |
| --- | --- | --- |
| **fromDict**<br />▹ *RamStatus* | *dict or object*: **statusDict** | Builds a *RamStatus* from a dict or object like the one returned by the *[RamDaemonInterface](ram_daemon_interface.md)* |
| **fromPath**<br />▹ *RamStatus* | *string*: **filePath**<br /> | Returns a *RamStatus* instance built using the given file path. |

## Attributes

| Attribute | Type | Default | Description |
| --- | --- | --- | --- |
| **comment** | *string* |  | A user comment. |
| **completionRatio** | *float* |  | The ratio of completion of this status. |
| **date** | *datetime* |  | The date at which this status was created. |
| **published** | *boolean* |  | True if this version of the item has been published. |
| **state** | *[RamState](ram_state.md)* |  | The corresponding state. |
| **user** | *[RamUser](ram_user.md)* |  | The user who created this status. |
| **version** | *integer* |  | The version of the corresponding working file. |

## Methods

| Method | Arguments | Description |
| --- | --- | --- |
| ***constructor*** | *[RamState](ram_state.md)*: **state**,<br />*string*: **comment**=`""`,<br />*float*: **completionRatio**=`-1`,<br />*integer*: **version**=`0`,<br />*[RamUser](ram_user.md)*: **user**=`None`,<br />*datetime*: **stateDate**=`None` | If `user` is `None`, the current user (as returned by [`Ramses.instance().currentUser()`](ramses.md) will be used.<br />If `completionRatio` is `-1`, the completion ratio of the given state will be used. |

![META](authors:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2021;updated:2021/05/25)