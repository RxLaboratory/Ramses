# RamUser

The class representing users.

Inherits: [***RamObject***](ram_object.md)

## Static Methods

| Method | Arguments | Description |
| --- | --- | --- |
| **fromDict**<br />▹ *RamUser* | *dict or object*: **stepDict** | Builds a *RamUser* from a dict or object like the one returned by the *[RamDaemonInterface](ram_daemon_interface.md)* |

## Methods

| Method | Arguments | Description |
| --- | --- | --- |
| ***constructor*** | *string*: **userName**,<br />*string*: **userShortName**,<br />*string*: **userFolderPath**=`""`,<br />*UserRole*: **role**=`UserRole.STANDARD` | |
| **configPath**<br>▹ *string* | | The path to the *Config* folder for this user |
| **folderPath**<br>▹ *string* | | The path to the user folder |
| **role**<br />▹ *UserRole* | | One of `UserRole.ADMIN`, `UserRole.PROJECT_ADMIN`, `UserRole.LEAD` or `UserRole.STANDARD` |

____

## API Dev notes

!!! note
    These section is for the development of the API only; you should not need these when developping your add-on using the API.

### (Im)mutable data

The data returned by the methods can be either [mutable or immutable](implementation.md#accessing-the-data).

| Method | Type of the returned data |
| --- | --- |
| **configPath** | <i class="fa fa-lock"></i> Immutable |
| **folderPath** | <i class="fa fa-lock"></i> Immutable |
| **role** | <i class="fa fa-lock"></i> Immutable |

![META](authors:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2021;updated:2021/05/25)