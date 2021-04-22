# RamProject

A project handled by Ramses. Projects contains general items, assets and shots.

Inherits: [***RamObject***](ram_object.md)

[TOC]

## Attributes

| Attribute | Type | Default | Description |
| --- | --- | --- | --- |

## Methods

| Method | Arguments | Description |
| --- | --- | --- |
| ***constructor*** | *string*: **projectName**<br />*string*: **projectShortName**<br />*string*: **projectPath** | |
| **absolutePath**<br />▹ *str* | *string*: **relativePath** | Builds an absolute path from a path relative to the project path |
| **asset**<br />▹ *[RamAsset](ram_asset.md)* | *string*: **assetShortName** | Gets an asset with its short name. |
| **assets**<br />▹ *list of [RamAsset](ram_asset.md)* | *string*: **groupName**=`""` | Available assets in this project and group. If *groupName* is an empty string, returns all assets. |
| **assetGroups**<br />▹ *list of string* | | Available asset groups in this project |
| **pipes**<br />▹ *list of [RamPipe](ram_pipe.md)* | | Available pipes in this project |
| **inputPipes**<br />▹ *list of [RamPipe](ram_pipe.md)* | *string*:**inputStepShortName** | Gets all pipes using this step as input |
| **outputPipes**<br />▹ *list of [RamPipe](ram_pipe.md)* | *string*:**outputStepShortName** | Gets all pipes using this step as output |
| **shot**<br />▹ *[RamShot](ram_shot.md)* | *string*: **shotShortName** | Gets a shot with its short name. |
| **shots**<br />▹ *[RamShot](ram_shot.md)* | | Available shots in this project |
| **step**<br />▹ *[RamStep](ram_step.md)* | *string*: **stepShortName** | Gets a step with its short name. | 
| **steps**<br />▹ *list of [RamStep](ram_step.md)* | *enumerated value*: **type**=`RamStep.ALL` | Available steps in this project. Use *type* to filter the results.<br />One of: `RamStep.ALL`, `RamStep.ASSET_PODUCTION`, `RamStep.SHOT_PRODUCTION`, `RamStep.PRE_PRODUCTION`, `RamStep.PRODUCTION`, `RamStep.POST_PRODUCTION`.<br />`RamStep.PRODUCTION` represents a combination of `SHOT` and `ASSET` | 

____

![META](authors:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2021;updated:2021/04/22)