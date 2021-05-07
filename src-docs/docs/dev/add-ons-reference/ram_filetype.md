# RamFileType

A File type.

File types are typically used with *[RamPipe](ram_pipe.md)*.

Inherits: [***RamObject***](ram_object.md)

## Methods

| Method | Arguments | Description |
| --- | --- | --- |
| ***constructor*** | *string*: **name**,<br />*string*: **shortName**,<br />*list of strings*: **extensions** | |
| **extensions**<br />▹ *list of string* | | The extensions which can be used for this file type, including the "." |

____

## API Dev notes

!!! note
    These section is for the development of the API only; you should not need these when developping your add-on using the API.

### (Im)mutable data

The data returned by the methods can be either [mutable or immutable](implementation.md#accessing-the-data).

| Method | Type of the returned data |
| --- | --- |
| **extensions** | <i class="fa fa-lock"></i> Immutable |

![META](authors:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2021;updated:2021/05/07)
