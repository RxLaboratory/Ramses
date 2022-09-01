![META](authors:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2021-2022;updated:2022/09/01)

# RamPipe

A pipe which connects two steps together in the production pipeline.

The pipe goes from the output step (which exports data into specific file types represented by the [`RamPipeFile`](ram_pipefile.md) class) to the input step (which imports that data).

Inherits: [***RamObject***](ram_object.md)

## Methods

| Method | Arguments | Description |
| --- | --- | --- |
| **inputStep**<br />▹ [*RamStep*](ram_step.md) | | The input step |
| **inputStepShortName**<br />▹ *string* | | The short name of the input step |
| **outputStep**<br />▹ [*RamStep*](ram_step.md) | | The output step |
| **outputStepShortName**<br />▹ *string* | | The short name of the output step |
| **pipeFiles**<br />▹ *list* of [*RamPipeFile*](ram_pipefile.md) | | The list of pipe files used through this step |
