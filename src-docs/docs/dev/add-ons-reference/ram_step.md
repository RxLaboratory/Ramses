# RamStep

A step in the production of the shots or assets of the project.

Inherits: [***RamObject***](ram_object.md)

## Static Methods

| Method | Arguments | Description |
| --- | --- | --- |
| **fromDict**<br />▹ *RamStep* | *dict or object*: **stepDict** | Builds a *RamStep* from a dict or object like the one returned by the *[RamDaemonInterface](ram_daemon_interface.md)* |
| **fromPath**<br />▹ *RamStep* | *string*: **path**<br /> | Returns a *RamStep* instance built using the given file path. |

## Methods

| Method | Arguments | Description |
| --- | --- | --- |
| ***constructor*** | *string*: **stepName**,<br />*string*: **stepShortName**,<br />*string*: **stepFolderPath**=`""`,<br />*StepType*: **stepType**=`""` | At least the folder or the type must be provided, but one is enough as the other info can be gotten from the [*Ramses Tree*](../../components/files/tree.md) |
| **color**<br />▹ *list* of *float* | | A color used for displaying this step, an RGB list. |
| **folderPath**<br />▹ *string* | | The absolute path to the folder containing the common files for this step |
| **inputPipes**<br />▹ *list* of *[RamPipe](ram_pipe.md)* | | The pipes comming into this step. |
| **outputPipes**<br />▹ *list* of *[RamPipe](ram_pipe.md)* | | The pipes going out of this step. |
| **project**<br />▹ *[RamProject](ram_project.md)* | | Gets the project this step belongs too. To improve performance, if only the shortName is needed, prefer using `projectShortName()` |
| **projectShortName**<br />▹ *string* | | Gets the short name of the project this step belongs too. |
| **stepType**<br />▹ *StepType* | | The type of this step, one of `StepType.PRE_PRODUCTION`, `StepType.SHOT_PRODUCTION`, `StepType.ASSET_PRODUCTION`, `StepType.POST_PRODUCTION`|
| **templatesFolderPath**<br />▹ *string* | | The absolute path to the folder containing the template files for this step |
| **templatesPublishFilePaths**<br />▹ *list* of *string* | | The list of published files for the templates |
| **templatesPublishPath**<br />▹ *string* | | The absolute path to the folder where templates are published |

____

## API Dev notes

!!! note
    These section is for the development of the API only; you should not need these when developping your add-on using the API.

### (Im)mutable data

The data returned by the methods can be either [mutable or immutable](implementation.md#accessing-the-data).

| Method | Type of the returned data |
| --- | --- |
| **commonFolderPath** | <i class="fa fa-lock"></i> Immutable |
| **inputPipes** | <i class="fa fa-lock"></i> Immutable |
| **outputPipes** | <i class="fa fa-lock"></i> Immutable |
| **stepType** | <i class="fa fa-lock"></i> Immutable |
| **templatesFolderPath** | <i class="fa fa-lock"></i> Immutable |

![META](authors:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2021;updated:2021/05/25)