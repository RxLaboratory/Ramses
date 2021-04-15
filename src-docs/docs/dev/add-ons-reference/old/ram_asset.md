# RamAsset

A class representing an asset.

Inherits: [***RamObject***](ram_object.md)

[TOC]

## Attributes

| Attribute | Type | Default | Description |
| --- | --- | --- | --- |

## Static Methods

| Method | Arguments | Description |
| --- | --- | --- |
| **getFromPath**<br />▹ *RamAsset* | *string*: **folderPath**<br /> | Returns a *RamAsset* instance built using the given folder path. |
| **getAssetFolder**<br />▹ *string* | *string*: **filePath**<br /> | Gets the folder path for the asset the file belongs to. |

## Methods

| Method | Arguments | Description |
| --- | --- | --- |
| ***constructor*** | *string*: **assetName**<br />*string*: **assetShortName** | |
| **getTags**<br />▹ *list of string* |  | Some tags describing the asset. |
| **getGroup**<br />▹ *string* | | The group containing this asset. |