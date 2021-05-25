# RamShot

A shot.

Inherits: [***RamItem***](ram_item.md)

## Static Methods

| Method | Arguments | Description |
| --- | --- | --- |
| **fromDict**<br />▹ *RamShot* | *dict or object*: **shotDict** | Builds a *RamShot* from a dict or object like the one returned by the *[RamDaemonInterface](ram_daemon_interface.md)* |
| **fromPath**<br />▹ *RamShot* | *string*: **folderPath**<br /> | Returns a *RamShot* instance built using the given folder path. The path can be any file or folder path from the asset (a version file, a preview file, etc) |

## Methods

| Method | Arguments | Description |
| --- | --- | --- |
| ***constructor*** | *string*: **shotName**,<br />*string*: **shotShortName**,<br />*string*: **shotFolder**,<br />*float*: **duration**=`0.0` | |
| **duration**<br />▹ *float* | | The shot duration, in seconds |

____

## API Dev notes

!!! note
    These section is for the development of the API only; you should not need these when developping your add-on using the API.

### (Im)mutable data

The data returned by the methods can be either [mutable or immutable](implementation.md#accessing-the-data).

| Method | Type of the returned data |
| --- | --- |
| **duration** | <i class="fa fa-pen"></i> Mutable |

![META](authors:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2021;updated:2021/05/25)