# RamAsset

A class representing an asset.

Inherits: [***RamItem***](ram_item.md)

[TOC]

## Attributes

| Attribute | Type | Default | Description |
| --- | --- | --- | --- |

## Static Methods

| Method | Arguments | Description |
| --- | --- | --- |
| **getFromPath**<br />▹ *RamAsset* | *string*: **folderPath**<br /> | Returns a *RamAsset* instance built using the given path. The path can be any file or folder path from the asset (a version file, a preview file, etc) |

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

### Notes about the methods

#### assetGroup()

The asset group should always be retrived from the folders; only if for any reason the group name can't be retrived from the folders, the private attribute `_group` is returned.  
This private attribute is set by the constructor, passed to it from the data got by the *Daemon*.

#### tags()

Tags are not available when the *Daemon* is offline. In this case, the method should return an empty string.  
The implementation is simple: tags are stored in the private `_tags` attrbibute by the constructor: the tags should be passed to the constructor when the asset is constructed from the *Daemon* data.
____

![META](authors:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2021;updated:2021/04/29)