# RamUser

The class representing users.

Inherits: [***RamObject***](ram_object.md)

[TOC]

## Attributes

| Attribute | Type | Default | Description |
| --- | --- | --- | --- |
| **role**<br />Read-only | *enumerated value* | `STANDARD` | One of `ADMIN`, `LEAD` or `STANDARD` |

## Methods

| Method | Arguments | Description |
| --- | --- | --- |
| ***constructor*** | *string*: **userName**<br />*string*: **userShortName**<br />*string*: **userFolderPath**=`""` | |
| **login**<br />▹ *boolean* | *string*: **password** | Logs the user in. Returns success. |
| **logout**<br />▹ *boolean* |  | Logs the user out. |
