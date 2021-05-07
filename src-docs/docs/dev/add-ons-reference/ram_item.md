# RamItem

Base class for [*RamAsset*](ram_asset.md) and [*RamShot*](ram_shot.md). An item of the project, either an asset or a shot.

Inherits: [***RamObject***](ram_object.md)

Inherited by: *[RamShot](ram_shot.md), [RamAsset](ram_asset.md)*

## Methods

| Method | Arguments | Description |
| --- | --- | --- |
| ***constructor*** | *string*: **itemName**,<br />*string*: **itemShortName**,<br />*string*: **itemFolder**=`""`,<br />*ItemType*: **itemType**=`ItemType.GENERAL`,<br />*string*: **projectShortName**=`""` | If `projectShortName` is not provided, it will be detected from the item folder (which can also be auto-detected in most cases) |
| **currentStatus**<br />▹ *RamStatus* | *RamStep* or *string*: **step**<br />*string*: **resource** = `""` | The current status for the given step |
| **folderPath**<br />▹ *string* |  | The absolute path to the folder containing the item |
| **isPublished**<br />▹ *bool* | *RamStep*: **step**<br />*string*: **resource** = `""` | Convenience function to check if there are published files in the publish folder. Equivalent to `len(self.publishedFilePaths(step, resource)) > 0` |
| **itemType**<br />▹ *ItemType* | | The type of this item. One of `ItemType.SHOT`, `ItemType.ASSET`, `ItemType.GENERAL` |
| **latestVersion**<br />▹ *integer* | *RamStep* or *string*: **step**<br />*string*: **resource** = `""`<br />*string*: **stateId** = `wip` | Returns the highest version number for the given state (wip, pub...). |
| **previewFolderPath**<br />▹ *string* | *RamStep* or *string*: **step** | Gets the path to the preview folder. Paths are relative to the root of the item folder. |
| **previewFilePaths**<br />▹ *list of string* | *RamStep* or *string*: **step**<br />*string*: **resource** = `""` | Gets the list of file paths in the preview folder. Paths are relative to the root of the item folder. |
| **projectShortName**<br />▹ *string* |  | Gets the short name of the project this item belongs to. |
| **publishedFolderPath**<br />▹ *string* | *RamStep* or *string*: **step** | Gets the path to the publish folder. Paths are relative to the root of the item folder. |
| **publishedFilePaths**<br />▹ *list of string* | *RamStep* or *string*: **step**<br />*string*: **resource** = `""` | Gets the list of file paths in the publish folder. Paths are relative to the root of the item folder. |
| **setStatus** | *RamStatus*: **status**<br />*[RamStep](ram_step.md)*: **step** | Sets the current status for the given step |
| **status**<br />▹ *RamStatus* | *[RamStep](ram_step.md)*: **step** | Gets the current status for the given step |
| **stepFolderPath**<br />▹ *string* | *RamStep or string*: **step** | The subfolder for the given step |
| **steps**<br />▹ *list of RamStep* | | Gets the list of steps concerning this item. |
| **versionFolderPath**<br />▹ *string* | *RamStep*: **step** | Path to the version folder relative to the item root folder |
| **versionFilePath**<br />▹ *string* | *RamStep*: **step**<br />*string*: **resource** = `""` | Latest version file path |

## Static Methods

| Method | Arguments | Description |
| --- | --- | --- |
| **fromPath**<br />▹ *RamShot* or *RamAsset* or *RamItem* or *None* | *string*: **fileOrFolderPath**<br /> | Returns either a *RamShot*, *RamAsset* or *RamItem* instance built using the given folder path. The path can be any file or folder path from the asset (a version file, a preview file, etc).<br />The type (shot, asset or general item) can be checked with `RamItem.itemType()` |

____

## API Dev notes

!!! note
    These section is for the development of the API only; you should not need these when developping your add-on using the API.

### (Im)mutable data

The data returned by the methods can be either [mutable or immutable](implementation.md#accessing-the-data).

| Method | Type of the returned data |
| --- | --- |
| **currentStatus** | <i class="fa fa-lock"></i> Immutable |
| **folderPath** | <i class="fa fa-lock"></i> Immutable |
| **isPublished** | <i class="fa fa-pen"></i> Mutable |
| **latestVersion** | <i class="fa fa-pen"></i> Mutable |
| **previewFolderPath** | <i class="fa fa-lock"></i> Immutable |
| **previewFilePaths** | <i class="fa fa-pen"></i> Mutable |
| **publishedFolderPath** | <i class="fa fa-lock"></i> Immutable |
| **publishedFilePaths** | <i class="fa fa-pen"></i> Mutable |
| **status** | <i class="fa fa-pen"></i> Mutable |
| **versionFolderPath** | <i class="fa fa-lock"></i> Immutable |
| **versionFilePath** | <i class="fa fa-pen"></i> Mutable |
| **wipFolderPath** | <i class="fa fa-lock"></i> Immutable |
| **wipFilePath** | <i class="fa fa-pen"></i> Mutable |

![META](authors:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2021;updated:2021/05/07)