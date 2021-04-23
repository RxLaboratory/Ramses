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
| ***constructor*** | *string*: **stepName**<br />*string*: **stepShortName** | |
| **commonFolderPath**<br />▹ *string* | | The absolute path to the folder containing the common files for this step |
| **templatesFolderPath**<br />▹ *string* | | The path to the template files of this step, relative to the common folder |
| **stepType**<br />▹ *enumerated value* | | The type of this step, one of `StepType.PRE_PRODUCTION`, `RamStep.SHOT_PRODUCTION`, `RamStep.ASSET_PRODUCTION`, `StepType.POST_PRODUCTION`|