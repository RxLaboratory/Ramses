![META](authors:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2021-2022;updated:2022/09/01)

# RamAsset

A class representing an asset.

Inherits: [***RamItem***](ram_item.md)

## Static Methods

| Method | Arguments | Description |
| --- | --- | --- |
| **fromPath**<br />▹ *RamAsset* | *string*: **folderPath**<br /> | Returns a *RamAsset* instance built using the given path. The path can be any file or folder path from the asset (a version file, a preview file, etc) |

## Methods

| Method | Arguments | Description |
| --- | --- | --- |
| **tags**<br />▹ *list of string* |  | Some tags describing the asset. An empty string if the *Daemon* is not available. |
| **group**<br />▹ *string* | | The name of the group containing this asset. |
| **assetGroup**<br />▹ *[RamAssetGroup](ram_assetgroup.md)* | | The group containing this asset. |
