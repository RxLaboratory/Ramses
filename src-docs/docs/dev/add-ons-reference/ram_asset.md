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
| ***constructor*** | *string*: **assetName**<br />*string*: **assetShortName** | |
| **tags**<br />▹ *list of string* |  | Some tags describing the asset. |
| **group**<br />▹ *string* | | The group containing this asset. |