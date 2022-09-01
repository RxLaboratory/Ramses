![META](authors:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2021-2022;updated:2022/09/01)


# RamFileType

A File type.

File types are typically used with *[RamPipe](ram_pipe.md)*.

Inherits: [***RamObject***](ram_object.md)

## Methods

| Method | Arguments | Description |
| --- | --- | --- |
| **check**<br />▹ *boolean* | *string*: **filePath** | Checks if the given file is of this type. |
| **extensions**<br />▹ *list of string* | | The extensions which can be used for this file type, excluding the leading "." |
