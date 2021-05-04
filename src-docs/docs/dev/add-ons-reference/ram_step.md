# RamStep

A step in the production of the shots or assets of the project.

Inherits: [***RamObject***](ram_object.md)

## Methods

| Method | Arguments | Description |
| --- | --- | --- |
| ***constructor*** | *string*: **stepName**,<br />*string*: **stepShortName**,<br />*string*: **stepFolderPath**=`""`,<br />*StepType*: **stepType**=`""` | At least the folder or the type must be provided, but one is enough as the other info can be gotten from the [*Ramses Tree*](../../files/tree.md) |
| **commonFolderPath**<br />▹ *string* | | The absolute path to the folder containing the common files for this step |
| **stepType**<br />▹ *enumerated value* | | The type of this step, one of `StepType.PRE_PRODUCTION`, `StepType.SHOT_PRODUCTION`, `StepType.ASSET_PRODUCTION`, `StepType.POST_PRODUCTION`|
| **templatesFolderPath**<br />▹ *string* | | The path to the template files of this step, relative to the common folder |

____

## API Dev notes

!!! note
    These section is for the development of the API only; you should not need these when developping your add-on using the API.

### (Im)mutable data

The data returned by the methods can be either [mutable or immutable](implementation.md#accessing-the-data).

| Method | Type of the returned data |
| --- | --- |
| **commonFolderPath** | <i class="fa fa-lock"></i> Immutable |
| **stepType** | <i class="fa fa-lock"></i> Immutable |
| **templatesFolderPath** | <i class="fa fa-lock"></i> Immutable |

![META](authors:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2021;updated:2021/05/04)