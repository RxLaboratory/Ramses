# RamPipe

A pipe which connects two steps together in the production pipeline.

The pipe goes from the output step (which exports data into a specific file type) to the input step (which imports that data).

## Static Methods

| Method | Arguments | Description |
| --- | --- | --- |
| **fromDict**<br />▹ *RamPipe* | *object* or *dict*: **pipeDict** | Returns a *RamPipe* instance built using the given object/dict, as the ones returned by [RamDaemonInterface](ram_daemon_interface.md). |

## Methods

| Method | Arguments | Description |
| --- | --- | --- |
| ***constructor*** | *string*: **inputStepShortName**,<br />*string*: **outputStepShortName**,<br />*[RamFileType](ram_filetype.md)*: **fileType** | |
| **colorSpace**<br />▹ *string* | | The color space used through the pipe |
| **inputStepShortName**<br />▹ *string* | | The short name of the input step |
| **outputStepShortName**<br />▹ *string* | | The short name of the output step |
| **fileType**<br />▹ *[RamFileType](ram_filetype.md)* | | The file type used through the pipe |

![META](authors:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2021;updated:2021/05/07)