# RamUser

The class representing users.

Inherits: [***RamObject***](ram_object.md)

[TOC]

## Attributes

| Attribute | Type | Default | Description |
| --- | --- | --- | --- |

## Methods

| Method | Arguments | Description |
| --- | --- | --- |
| ***constructor*** | *string*: **userName**<br />*string*: **userShortName**<br />*string*: **userFolderPath**=`""` | |
| **configPath**<br>▹ *string* | | The path to the *Config* folder relative to the user folder |
| **folderPath**<br>▹ *string* | | The absolute path to the user folder |
| **role**<br />▹ *enumerated value* | | One of `RamUser.ADMIN`, `RamUser.PROJECT_ADMIN`, `RamUser.LEAD` or `RamUser.STANDARD` |
