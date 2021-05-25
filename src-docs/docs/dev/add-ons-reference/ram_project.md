# RamProject

A project handled by Ramses. Projects contains general items, assets and shots.

Inherits: [***RamObject***](ram_object.md)

## Static Methods

| Method | Arguments | Description |
| --- | --- | --- |
| **fromDict**<br />▹ *RamProject* | *dict or object*: **projectDict** | Builds a *RamProject* from a dict or object like the one returned by the *[RamDaemonInterface](ram_daemon_interface.md)* |
| **fromPath**<br />▹ *RamProject* | *string*: **path**<br /> | Returns a *RamProject* instance built using the given path. The path can be any file or folder path from the project |

## Methods

| Method | Arguments | Description |
| --- | --- | --- |
| ***constructor*** | *string*: **projectName**,<br />*string*: **projectShortName**,<br />*string*: **projectPath**=`""`,<br />*int*: **width**=`1920`,<br />*int*: **height**=`1080`,<br />*float*: **framerate**=`24.0` | |
| **adminPath**<br />▹ *str* | | The path to the administration files of this project. When calling this method, the folder will created if it does not exist yet. |
| **absolutePath**<br />▹ *str* | *string*: **relativePath** | Builds an absolute path from a path relative to the project path |
| **asset**<br />▹ *[RamAsset](ram_asset.md)* | *string*: **assetShortName** | Gets an asset with its short name. |
| **assetGroups**<br />▹ *list of string* | | Available asset groups in this project |
| **assets**<br />▹ *list of [RamAsset](ram_asset.md)* | *string*: **groupName**=`""` | Available assets in this project and group. If *groupName* is an empty string, returns all assets. |
| **assetsPath**<br />▹ *str* | | The path to the assets of this project. When calling this method, the folder will created if it does not exist yet. |
| **exportPath**<br />▹ *str* | | The path to the exported medias of this project. When calling this method, the folder will created if it does not exist yet. |
| **folderPath**<br />▹ *string* | | The project's root folder. |
| **framerate**<br />▹ *float* | | The project's framerate, in frames per second. |
| **height**<br />▹ *int* | | The project's height, in pixels. |
| **inputPipes**<br />▹ *list of [RamPipe](ram_pipe.md)* | *string*:**inputStepShortName** | Gets all pipes using this step as input |
| **outputPipes**<br />▹ *list of [RamPipe](ram_pipe.md)* | *string*:**outputStepShortName** | Gets all pipes using this step as output |
| **pipes**<br />▹ *list of [RamPipe](ram_pipe.md)* | | Available pipes in this project |
| **postProdPath**<br />▹ *str* | | The path to the post-production steps common files of this project. When calling this method, the folder will created if it does not exist yet. |
| **preProdPath**<br />▹ *str* | | The path to the pre-production steps common files of this project. When calling this method, the folder will created if it does not exist yet. |
| **prodPath**<br />▹ *str* | | The path to the production steps common files of this project. When calling this method, the folder will created if it does not exist yet. |
| **shot**<br />▹ *[RamShot](ram_shot.md)* | *string*: **shotShortName** | Gets a shot with its short name. |
| **shots**<br />▹ *[RamShot](ram_shot.md)* | | Available shots in this project |
| **shotsPath**<br />▹ *str* | | The path to the shots of this project. When calling this method, the folder will created if it does not exist yet. |
| **step**<br />▹ *[RamStep](ram_step.md)* | *string*: **stepShortName** | Gets a step with its short name. | 
| **steps**<br />▹ *list of [RamStep](ram_step.md)* | *enumerated value*: **type**=`RamStep.ALL` | Available steps in this project. Use *type* to filter the results.<br />One of: `StepType.ALL`, `StepType.ASSET_PODUCTION`, `StepType.SHOT_PRODUCTION`, `StepType.PRE_PRODUCTION`, `StepType.PRODUCTION`, `StepType.POST_PRODUCTION`.<br />`StepType.PRODUCTION` represents a combination of `SHOT` and `ASSET`. |
| **width**<br />▹ *int* | | The project's width, in pixels. |

____

## API Dev notes

!!! note
    These section is for the development of the API only; you should not need these when developping your add-on using the API.

### Private methods

| Method | Arguments | Description |
| --- | --- | --- |
| **_getAssetsInFolder**<br />▹ *list* of *[RamAsset](ram_asset.md)* | *string*: **folderPath**,<br />*string*: **groupName**=`""` | Recursively gets all assets contained in the given folder. |

### (Im)mutable data

The data returned by the methods can be either [mutable or immutable](implementation.md#accessing-the-data).

If a method is not listed here, it means the corresponding data is never stored in a class attribute, and must always be got from the Daemon or the tree.

| Method | Type of the returned data |
| --- | --- |
| **framerate** | <i class="fa fa-lock"></i> Immutable |
| **height** | <i class="fa fa-lock"></i> Immutable |
| **width** | <i class="fa fa-lock"></i> Immutable |

![META](authors:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2021;updated:2021/05/25)