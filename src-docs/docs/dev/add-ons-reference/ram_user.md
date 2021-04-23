# RamUser

The class representing users.

Inherits: [***RamObject***](ram_object.md)

[TOC]

## Constants (enumerated values)

### UserRole

The role of the user (i.e. its administration rights).

| Name | Value | Description |
| --- | --- | --- |
| UserRole.**ADMIN** | `3` | |
| UserRole.**PROJECT_ADMIN** | `2` | |
| UserRole.**LEAD** | `1` | |
| UserRole.**STANDARD** | `0` | |

## Attributes

| Attribute | Type | Default | Description |
| --- | --- | --- | --- |

## Methods

| Method | Arguments | Description |
| --- | --- | --- |
| ***constructor*** | *string*: **userName**,<br />*string*: **userShortName**,<br />*string*: **userFolderPath**=`""`,<br />*UserRole*: **role**=`UserRole.STANDARD` | |
| **configPath**<br>▹ *string* | | The path to the *Config* folder relative to the user folder |
| **folderPath**<br>▹ *string* | | The absolute path to the user folder |
| **role**<br />▹ *enumerated value* | | One of `UserRole.ADMIN`, `UserRole.PROJECT_ADMIN`, `UserRole.LEAD` or `UserRole.STANDARD` |
