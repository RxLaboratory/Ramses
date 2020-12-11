# RamProject

A project handled by Ramses. Projects contains general items, assets and shots.

Inherits: [***RamObject***](ram_object.md)

[TOC]

## Attributes

| Attribute | Type | Default | Description |
| --- | --- | --- | --- |

## Methods

| Method | Arguments | Description |
| --- | --- | --- |
| ***constructor*** | *string*: **projectName**<br />*string*: **projectShortName**<br />*string*: **projectPath** | |
| **getAssets**<br />▹ *list of [RamAsset](ram_asset.md)* | *string*: **groupName**=`""`| If *groupName* is an empty string, returns all assets. |
| **getAssetGroups**<br />▹ *list of string* | | |
| **getShots**<br />▹ *list of [RamShot](ram_shot.md)* | *string*: **filter**=`"*"` | A filter to be applied to the name of the shots, using "*" as a wildcard. |
| **getSteps**<br />▹ *list of [RamStep](ram_step.md)* | *enumerated value*: **typeOrCat**=`ALL` | Use *typeOrCat* to filter the results.<br />One of: `ALL`, `ASSET`, `SHOT`, `PRE-PROD`, `PROD`, `POST-PROD`.<br />`PROD` represents a combination of `SHOT` and `ASSET` |

