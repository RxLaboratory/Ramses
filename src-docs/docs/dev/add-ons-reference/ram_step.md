# RamStep

A step in the production of the shots or assets of the project.

Inherits: [***RamObject***](ram_object.md)

[TOC]

## Constants (enumerated values)

### StepType

The type of the step. Usually used to filter available steps.

| Name | Value | Description |
| --- | --- | --- |
| StepType.**PRE_PRODUCTION** | `'PRE_PRODUCTION'` | |
| StepType.**ASSET_PRODUCTION** | `'ASSET_PRODUCTION'` | |
| StepType.**SHOT_PRODUCTION** | `'SHOT_PRODUCTION'` | |
| StepType.**POST_PRODUCTION** | `'POST_PRODUCTION'` | |
| StepType.**PRODUCTION** | `'PRODUCTION'` | Use for *ASSET* AND *SHOT* production. |
| StepType.**ALL** | `'ALL'` | Use for all steps. |


## Attributes

| Attribute | Type | Default | Description |
| --- | --- | --- | --- |

## Methods

| Method | Arguments | Description |
| --- | --- | --- |
| ***constructor*** | *string*: **stepName**,<br />*string*: **stepShortName**,<br />*string*: **stepFolderPath**=`""`,<br />*StepType*: **stepType**=`""` | At least the folder or the type must be provided, but one is enough as the other info can be gotten from the [*Ramses Tree*](../../files/tree.md) |
| **commonFolderPath**<br />▹ *string* | | The absolute path to the folder containing the common files for this step |
| **templatesFolderPath**<br />▹ *string* | | The path to the template files of this step, relative to the common folder |
| **stepType**<br />▹ *enumerated value* | | The type of this step, one of `StepType.PRE_PRODUCTION`, `StepType.SHOT_PRODUCTION`, `StepType.ASSET_PRODUCTION`, `StepType.POST_PRODUCTION`|

____

## API Dev notes

!!! note
    These section is for the development of the API only; you should not need these when developping your add-on using the API.

### Notes about the methods

#### commonFolderPath()

The commonFolderPath methods checks if the folder is already available in the private attribute `_folderPath`. If so, it is simply returned.

If `_folderPath` is an empty string, it has to be set before it is returned: the path has to be built from the project folder (given by the *Ramses* instance if it is available), the step type, the step short name.

#### stepType()

If the private attribute `_type` is not an empty string, it is returned.

Otherwise, the type can be retrived from the parent folder of the folder returned by `commonFolderPath()` (if it's not also an empty string). See the [Ramses Tree](../../files/tree.md) for more info about the way this parent folder is named.

If found, the type is stored in the private attribute `_type` and then returned.
____

![META](authors:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2021;updated:2021/04/29)