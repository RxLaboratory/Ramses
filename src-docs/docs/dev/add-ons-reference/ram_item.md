# RamItem

Base class for [*RamAsset*](ram_asset.md) and [*RamShot*](ram_shot.md). An item of the project, either an asset or a shot.

Inherits: [***RamObject***](ram_object.md)

Inherited by: *[RamShot](ram_shot.md), [RamAsset](ram_asset.md)*

[TOC]

## Attributes

| Attribute | Type | Default | Description |
| --- | --- | --- | --- |

## Methods

| Method | Arguments | Description |
| --- | --- | --- |
| ***constructor*** | *string*: **itemName**<br />*string*: **itemShortName**<br />*string*: **itemFolder**=`""` | |
| **currentStatus**<br />▹ *RamStatus* | *RamStep* or *string*: **step**<br />*string*: **resource** = `""` | The current status for the given step |
| **folderPath**<br />▹ *string* | *RamStep or string*: **step** = `""`| The absolute path to the folder containing the asset, or to the step subfolder if provided |
| **latestVersion**<br />▹ *integer* | *RamStep* or *string*: **step**<br />*string*: **resource** = `""`<br />*string*: **stateId** = `wip` | Returns the highest version number for the given state (wip, pub...). |
| **previewFolderPath**<br />▹ *string* | *RamStep* or *string*: **step** | Gets the path to the preview folder. Paths are relative to the root of the item folder. |
| **previewFilePaths**<br />▹ *list of string* | *RamStep* or *string*: **step**<br />*string*: **resource** = `""` | Gets the list of file paths in the preview folder. Paths are relative to the root of the item folder. |
| **publishedFolderPath**<br />▹ *string* | *RamStep* or *string*: **step** | Gets the path to the publish folder. Paths are relative to the root of the item folder. |
| **publishedFilePaths**<br />▹ *list of string* | *RamStep* or *string*: **step**<br />*string*: **resource** = `""` | Gets the list of file paths in the publish folder. Paths are relative to the root of the item folder. |
| **versionFolderPath**<br />▹ *string* | *RamStep*: **step** | Path to the version folder relative to the item root folder |
| **versionFilePath**<br />▹ *string* | *RamStep*: **step**<br />*string*: **resource** = `""` | Latest version file path relative to the item root folder |
| **wipFolderPath**<br />▹ *string* | *RamStep* or *string*: **step** | Path to the WIP folder relative to the item root folder |
| **wipFilePath**<br />▹ *string* | *RamStep* or *string*: **step**<br />*string*: **resource** = `""` | Current wip file path relative to the item root folder |
| **isPublished**<br />▹ *bool* | *RamStep*: **step**<br />*string*: **resource** = `""` | Convenience function to check if there are published files in the publish folder. Equivalent to `len(self.publishedFilePaths(step, resource)) > 0` |
| **setStatus** | *RamStatus*: **status**<br />*[RamStep](ram_step.md)*: **step** | Sets the current status for the given step |
| **status**<br />▹ *RamStatus* | *[RamStep](ram_step.md)*: **step** | Gets the current status for the given step |

____

## API Dev notes

!!! note
    These section is for the development of the API only; you should not need these when developping your add-on using the API.

### Notes about the methods

#### folderPath()

The folderPath methods checks if the folder is already available in the private attribute `_folderPath`. If so, it is simply returned.

If `_folderPath` is an empty string, it has to be set before it is returned: the path has to be built from the step short name, the item short name, and the project folder (given by the *Ramses* instance if it is available).

____

![META](authors:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2021;updated:2021/04/29)