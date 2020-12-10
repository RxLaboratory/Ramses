# RamItem

Base class for [*RamAsset*](ram_asset.md) and [*RamShot*](ram_shot.md). An item of the project, either a general item, an asset or a shot.

Inherits: [***RamObject***](ram_object.md)

[TOC]

## Attributes

| Attribute | Type | Default | Description |
| --- | --- | --- | --- |
| **published** | *boolean* | | |
| **stepStatuses** | *list of [RamStatus](ram_status.md)* | | |

## Methods

| Method | Arguments | Description |
| --- | --- | --- |
| ***constructor*** | *string*: **itemName**<br />*string*: **itemShortName**<br />*string*: **itemFolder**=`""` | |
| **getLatestPubVersion**<br />▹ *integer* | *RamStep*: **step**<br />*string*: **resource** = `""` | Returns the highest version number for published item |
| **getLatestVersion**<br />▹ *integer* | *RamStep*: **step**<br />*string*: **resource** = `""` | Returns the highest version number |
| **getPublishedFilePaths**<br />▹ *list of string* | *RamStep*: **step**<br />*string*: **resource** = `""` | Gets the list of file paths in the publish folder. paths are relative to the root of the item folder. |
| **getVersionFilePath**<br />▹ *string* | *RamStep*: **step**<br />*string*: **resource** = `""` | Latest version file path relative to the item root folder |
| **getWIPFilePath**<br />▹ *string* | *RamStep*: **step**<br />*string*: **resource** = `""` | Current wip file path relative to the item root folder |