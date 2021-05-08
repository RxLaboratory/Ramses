# RamAsset

A class representing an asset.

Inherits: [***RamItem***](ram_item.md)

## Attributes

| Attribute | Type | Default | Description |
| --- | --- | --- | --- |

## Static Methods

| Method | Arguments | Description |
| --- | --- | --- |
| **fromPath**<br />▹ *RamAsset* | *string*: **folderPath**<br /> | Returns a *RamAsset* instance built using the given path. The path can be any file or folder path from the asset (a version file, a preview file, etc) |

## Methods

| Method | Arguments | Description |
| --- | --- | --- |
| ***constructor*** | *string*: **assetName**,<br />*string*: **assetShortName**,<br />*string*: **assetFolder**,<br />*string*: **assetGroup**=`""`,<br />*string*: **tags**=`""` | |
| **tags**<br />▹ *list of string* |  | Some tags describing the asset. An empty string if the *Daemon* is not available. |
| **group**<br />▹ *string* | | The name of group containing this asset. |

____

## API Dev notes

!!! note
    These section is for the development of the API only; you should not need these when developping your add-on using the API.

### (Im)mutable data

The data returned by the methods can be either [mutable or immutable](implementation.md#accessing-the-data).

| Method | Type of the returned data |
| --- | --- |
| **group** | <i class="fa fa-lock"></i> Immutable |
| **tags** | <i class="fa fa-pen"></i> Mutable |

![META](authors:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2021;updated:2021/05/07)