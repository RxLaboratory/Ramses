# RamStatus

A state associated to a comment, the user who changed the state, etc.

[TOC]

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
| **getFromPath**<br />▹ *RamStatus* | *string*: **filePath**<br /> | Returns a *RamStatus* instance built using the given file path. |

## Methods

| Method | Arguments | Description |
| --- | --- | --- |
| ***constructor*** | *[RamState](ram_state.md)*: **state**<br />*[RamUser](ram_user.md)*: **user**=`None`<br />*string*: **comment**=`""`<br />*integer*: **version**=`0`<br />*datetime*: **stateDate**=`None`<br />*float*: **completionRation**=`None` | |
