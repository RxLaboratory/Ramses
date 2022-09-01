![META](authors:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2021-2022;updated:2022/09/01)

# RamStatus

A state associated to a comment, the user who changed the state, etc.

Inherits: [***RamObject***](ram_object.md)

## Static Methods

| Method | Arguments | Description |
| --- | --- | --- |
| **fromPath**<br />▹ *RamStatus* | *string*: **filePath**<br /> | Returns a *RamStatus* instance built using the given file path. |

## Methods

| Method | Arguments | Description |
| --- | --- | --- |
| **completionRatio**<br />▹ *integer* |  | The ratio of completion of this status. |
| **date**<br />▹ *datetime* |  | The date at which this status was created. |
| **item**<br />▹ *[RamAsset](ram_asset.md)* or *[RamItem](ram_item.md)* or *[RamShot](ram_shot.md)* | | The item this status refers to. |
| **published**<br />▹ *boolean* |  | True if this version of the item has been published. |
| **state**<br />▹ *[RamState](ram_state.md)* |  | The corresponding state. |
| **user**<br />▹ *[RamUser](ram_user.md)* |  | The user who created this status. |
| **version**<br />▹ *integer* |  | The version of the corresponding working file. |
| **setCompletionRatio** | • *integer*: **completion** | Sets the ratio of completion of this status. |
| **setPublished** | • *boolean*: **published**=`true` | Sets if this version of the item has been published. |
| **setState** | • [*RamState*](ram_state.md): **state** | Sets the corresponding state. |
| **setUser** | • [*RamUser*](ram_user.md): **user** | Sets the user who created this status. |
| **setVersion** | • *integer*: **version** | Sets the version of the corresponding working file. |
| **step**<br />▹ *[RamStep](ram_step.md)* | | The step this status refers to. |