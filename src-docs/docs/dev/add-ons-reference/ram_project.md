![META](authors:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2021-2022;updated:2022/09/01)

# RamProject

A project handled by Ramses. Projects contains general items, assets and shots.

Inherits: [***RamObject***](ram_object.md)

## Static Methods

| Method | Arguments | Description |
| --- | --- | --- |
| **fromPath**<br />▹ *RamProject* | *string*: **path**<br /> | Returns a *RamProject* instance built using the given path. The path can be any file or folder path from the project |

## Methods

| Method | Arguments | Description |
| --- | --- | --- |
| **adminPath**<br />▹ *str* | | The path to the administration files of this project. When calling this method, the folder will created if it does not exist yet. |
| **absolutePath**<br />▹ *str* | *string*: **relativePath** | Builds an absolute path from a path relative to the project path |
| **assetGroups**<br />▹ *list of [RamAssetGroup](ram_assetgroup.md)* | | Available asset groups in this project |
| **assets**<br />▹ *list of [RamAsset](ram_asset.md)* | *string*: **groupName**=`""` | Available assets in this project and group. If *groupName* is an empty string, returns all assets. |
| **assetsPath**<br />▹ *str* | *string*: **assetGroup**=`""`| The path to the assets (including the asset group if provided) of this project. When calling this method, the folder will created if it does not exist yet. |
| **exportPath**<br />▹ *str* | | The path to the exported medias of this project. When calling this method, the folder will created if it does not exist yet. |
| **folderPath**<br />▹ *string* | | The project's root folder. |
| **framerate**<br />▹ *float* | | The project's framerate, in frames per second. |
| **height**<br />▹ *int* | | The project's height, in pixels. |
| **pipes**<br />▹ *list of [RamPipe](ram_pipe.md)* | | Available pipes in this project |
| **postProdPath**<br />▹ *str* | | The path to the post-production steps common files of this project. When calling this method, the folder will created if it does not exist yet. |
| **preProdPath**<br />▹ *str* | | The path to the pre-production steps common files of this project. When calling this method, the folder will created if it does not exist yet. |
| **prodPath**<br />▹ *str* | | The path to the production steps common files of this project. When calling this method, the folder will created if it does not exist yet. |
| **sequences**<br />▹ *list of [RamSequence](ram_sequence.md)* | | Available sequences in this project |
| **shots**<br />▹ *[RamShot](ram_shot.md)* | *string*: **nameFilter**=`"*"`,<br />*string*: **sequence**=`""`  | Available shots in this project |
| **shotsPath**<br />▹ *str* | | The path to the shots of this project. When calling this method, the folder will created if it does not exist yet. |
| **steps**<br />▹ *list of [RamStep](ram_step.md)* | *enumerated value*: **type**=`RamStep.ALL` | Available steps in this project. Use *type* to filter the results.<br />One of: `StepType.ALL`, `StepType.ASSET_PODUCTION`, `StepType.SHOT_PRODUCTION`, `StepType.PRE_PRODUCTION`, `StepType.PRODUCTION`, `StepType.POST_PRODUCTION`.<br />`StepType.PRODUCTION` represents a combination of `SHOT` and `ASSET`. |
| **width**<br />▹ *int* | | The project's width, in pixels. |

____

## API Dev notes

!!! note
    These section is for the development of the API only; you should not need these when developping your add-on using the API.

### Private methods

| Method | Arguments | Description |
| --- | --- | --- |
| **_getAssetsInFolder**<br />▹ *list* of *[RamAsset](ram_asset.md)* | *string*: **folderPath**,<br />*RamAssetGroup*: **assetGroup**=`None` | Recursively gets all assets contained in the given folder. |
