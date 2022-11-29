![META](authors:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2021-2022;updated:2022/09/01)

# RamAssetGroup

A group of assets.

|     |     |
| --- | --- |
| Inherits | *object* ► ***[RamObject](ram_object.md)*** ► *RamAssetGroup* |

## Static Methods

| Method | Arguments | Description |
| --- | --- | --- |
| **fromPath**<br />▹ *RamAssetGroup* or *None* | *string*: **fileOrFolderPath**<br /> | The path can be any file or folder path from inside the group (a version file, a preview file, etc). |

## Methods

| Method | Arguments | Description |
| --- | --- | --- |
| **project**<br />▹ [*RamProject*](ram_project.md) | | The project containing this group. |
| **assets**<br />▹ *list of [*RamAsset*](ram_asset.md)* | | The assets contained in this group. |
