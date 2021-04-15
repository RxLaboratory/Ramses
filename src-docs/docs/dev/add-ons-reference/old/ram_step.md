# RamStep

Base class for [*RamAsset*](ram_asset.md) and [*RamShot*](ram_shot.md). A step in the production of the shots or assets of the project.

Inherits: [***RamObject***](ram_object.md)

Inherited by: [*RamAsset*](ram_asset.md), [*RamShot*](ram_shot.md)

[TOC]

## Attributes

| Attribute | Type | Default | Description |
| --- | --- | --- | --- |
| **assignedUsers** | *list of [RamUser](ram_user.md)* | `[]` | Users working on this step. |
| **fileType** | *[RamFileType](ram_filetype.md)* | `None` | The main file type used for working on this step. |
| **leads** | *list of [RamUser](ram_user.md)* |  | Users leading (i.e. with admin rights and validating assets) this step. |
| **publishFileTypes** | *list of [RamFileType](ram_filetype.md)* |  | The file types published by this step. |
| **secondaryFileTypes** | *list of [RamFileType](ram_filetype.md)* |  | Other file types which may be used when working on this step. |

## Methods

| Method | Arguments | Description |
| --- | --- | --- |
| ***constructor*** | *string*: **stepName**<br />*string*: **stepShortName** | |