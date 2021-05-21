# RamStatus

A state associated to a comment, the user who changed the state, etc.

## Attributes

| Attribute | Type | Default | Description |
| --- | --- | --- | --- |
| **comment** | *string* |  | A user comment. |
| **completionRatio** | *float* |  | The ratio of completion of this status. |
| **date** | *datetime* |  | The date at which this status was created. |
| **state** | *[RamState](ram_state.md)* |  | The corresponding state. |
| **user** | *[RamUser](ram_user.md)* |  | The user who created this status. |
| **version** | *integer* |  | The version of the corresponding working file. |

## Static Methods

| Method | Arguments | Description |
| --- | --- | --- |
| **fromPath**<br />▹ *RamStatus* | *string*: **filePath**<br /> | Returns a *RamStatus* instance built using the given file path. |

## Methods

| Method | Arguments | Description |
| --- | --- | --- |
| ***constructor*** | *[RamState](ram_state.md)*: **state**,<br />*string*: **comment**=`""`,<br />*float*: **completionRation**=`None`,<br />*integer*: **version**=`0`,<br />*[RamUser](ram_user.md)*: **user**=`None`,<br />*datetime*: **stateDate**=`None` | |

![META](authors:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2021;updated:2021/05/07)