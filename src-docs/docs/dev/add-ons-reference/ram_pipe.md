# RamPipe

A pipe which connects two steps together in the production pipeline.

The pipe goes from the output step (which exports data into specific file types represented by the [`RamPipeFile`](ram_pipefile.md) class) to the input step (which imports that data).

## Static Methods

| Method | Arguments | Description |
| --- | --- | --- |
| **fromDict**<br />▹ *RamPipe* | *object* or *dict*: **pipeDict** | Returns a *RamPipe* instance built using the given object/dict, as the ones returned by [RamDaemonInterface](ram_daemon_interface.md). |

## Methods

| Method | Arguments | Description |
| --- | --- | --- |
| ***constructor*** | *string*: **inputStepShortName**,<br />*string*: **outputStepShortName**,<br />*list* of *[RamPipeFile](ram_pipefile.md)*: **pipeFiles** | |
| **inputStepShortName**<br />▹ *string* | | The short name of the input step |
| **outputStepShortName**<br />▹ *string* | | The short name of the output step |
| **pipeFiles**<br />▹ *list* of *[RamPipeFile](ram_pipefile.md)* | | The list of pipe files used through this step |

____

## API Dev notes

### (Im)mutable data

The data returned by the methods can be either [mutable or immutable](implementation.md#accessing-the-data).

| Method | Type of the returned data |
| --- | --- |
| **inputStepShortName** | <i class="fa fa-lock"></i> Immutable |
| **outputStepShortName** | <i class="fa fa-lock"></i> Immutable |
| **pipeFiles** | <i class="fa fa-lock"></i> Immutable |

![META](authors:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2021;updated:2021/06/08)