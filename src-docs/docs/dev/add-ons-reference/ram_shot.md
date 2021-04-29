# RamShot

A shot.

Inherits: [***RamItem***](ram_item.md)

[TOC]

## Attributes

| Attribute | Type | Default | Description |
| --- | --- | --- | --- |

## Static Methods

| Method | Arguments | Description |
| --- | --- | --- |
| **getFromPath**<br />▹ *RamShot* | *string*: **folderPath**<br /> | Returns a *RamShot* instance built using the given folder path. The path can be any file or folder path from the asset (a version file, a preview file, etc) |

## Methods

| Method | Arguments | Description |
| --- | --- | --- |
| ***constructor*** | *string*: **shotName**,<br />*string*: **shotShortName**,<br />*string*: **shotFolderPath**,<br />*float*: **duration**=`0.0` | |
| **duration**<br />▹ *float* | | The shot duration, in seconds |

____

## API Dev notes

!!! note
    These section is for the development of the API only; you should not need these when developping your add-on using the API.

### Notes about the methods

#### duration()

The duration is not available when the *Daemon* is offline. In this case, the method should return `0.0`.  
The implementation is simple: the duration is stored in the private `_duration` attrbibute by the constructor: the duration should be passed to the constructor when the shot is constructed from the *Daemon* data.

____

![META](authors:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2021;updated:2021/04/29)