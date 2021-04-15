# RamStep

A step in the production of the shots or assets of the project.

Inherits: [***RamObject***](ram_object.md)

[TOC]

## Attributes

| Attribute | Type | Default | Description |
| --- | --- | --- | --- |

## Methods

| Method | Arguments | Description |
| --- | --- | --- |
| ***constructor*** | *string*: **stepName**<br />*string*: **stepShortName** | |
| **commonFolderPath**<br />▹ *string* | | The absolute path to the folder containing the common files for this step |
| **templatesFolderPath**<br />▹ *string* | | The path to the template files of this step, relative to the common folder |
| **type**<br />▹ *enumerated value* | | The type of this step, one of `RamStep.PRE_PRODUCTION`, `RamStep.SHOT_PRODUCTION`, `RamStep.ASSET_PRODUCTION`, `RamStep.POST_PRODUCTION`|