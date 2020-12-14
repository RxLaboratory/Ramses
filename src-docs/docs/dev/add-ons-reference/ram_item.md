# RamItem

Base class for [*RamAsset*](ram_asset.md) and [*RamShot*](ram_shot.md). An item of the project, either a general item, an asset or a shot.

Inherits: [***RamObject***](ram_object.md)

Inherited by: *[RamShot](ram_shot.md), [RamAsset](ram_asset.md)*

[TOC]

## Attributes

| Attribute | Type | Default | Description |
| --- | --- | --- | --- |
| **stepStatuses** | *list of [RamStepStatus](ram_stepstatus.md)* | `{}` | In the form `{"stepId":RamStepStatus}` |

## Methods

| Method | Arguments | Description |
| --- | --- | --- |
| ***constructor*** | *string*: **itemName**<br />*string*: **itemShortName**<br />*string*: **itemFolder**=`""` | |
| **getLatestVersion**<br />▹ *integer* | *RamStep*: **step**<br />*string*: **resource** = `""`<br />*string*: **stateId** = `wip` | Returns the highest version number for the given state (wip, pub...). |
| **getPublishedFilePaths**<br />▹ *list of string* | *RamStep* or *string*: **step**<br />*string*: **resource** = `""` | Gets the list of file paths in the publish folder. Paths are relative to the root of the item folder. |
| **getVersionFilePath**<br />▹ *string* | *RamStep*: **step**<br />*string*: **resource** = `""` | Latest version file path relative to the item root folder |
| **getWIPFilePath**<br />▹ *string* | *RamStep* or *string*: **step**<br />*string*: **resource** = `""` | Current wip file path relative to the item root folder |
| **isPublished**<br />▹ *bool* | *RamStep*: **step**<br />*string*: **resource** = `""` | Convenience function to check if there are published files in the publish folder. Equivalent to len(self.getPublishedFilePaths(step, resource)) > 0 |
| **setStepStatus** | *RamStatus*: **status**<br />*[RamStep](ram_step.md)*: **step** | Adds a new status in `stepStatuses` for the given step |