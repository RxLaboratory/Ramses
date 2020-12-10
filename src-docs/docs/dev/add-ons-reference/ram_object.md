# RamObject

The base class for most of Ramses objects.

[TOC]

## Attributes

| Attribute | Type | Default | Description |
| --- | --- | --- | --- |
| **name** | *string* | | May contain spaces, [a-z] ,[A-Z], [0-9], [+-]. |
| **shortName** | *string* | | Used for compact display and folder names, limited to 10 characters, must not contain spaces, may contain [a-z] ,[A-Z], [0-9], [+-]. |
| **folderPath** | *string* | | The path relative to a project or the main Ramses folder, containing all files in relation to this object. |

## Methods

| Method | Arguments | Description |
| --- | --- | --- |
| ***constructor*** | *string*: **objectName**<br />*string*: **objectShortName**<br />*string*: **objectFolder**=`""` | |