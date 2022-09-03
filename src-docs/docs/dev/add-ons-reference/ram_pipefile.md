![META](authors:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2021-2022;updated:2022/09/01)

# RamPipeFile

A file which goes through a [`RamPipe`](ram_pipe.md).

|     |     |
| --- | --- |
| Inherits | *object* ► ***[RamObject](ram_object.md)*** ► *RamPipeFile* |

## Methods

| Method | Arguments | Description |
| --- | --- | --- |
| **check**<br />▹ *boolean* | *string*: **filePath** | Checks if the given file can go through this pipe. |
| **colorSpace**<br />▹ *string* | | The color space used through the pipe. |
| **customSettings**<br />▹ *string* | | Can be any string which can be set from the pipeline editor in the [*Ramses Client Application*](../../components/client/index.md). Use this to add more information to configure your pipeline and pass this information to the addons. |
| **getFiles**<br />▹ *list* of *string* | *string*: **folderPath** | Gets all the files which can go through this pipe in the given folder. |
| **fileType**<br />▹ *[RamFileType](ram_filetype.md)* | | The file type used through the pipe. |
