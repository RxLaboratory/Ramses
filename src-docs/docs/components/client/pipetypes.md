![META](authors:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2021;updated:2021/07/13)

# ![](/img/icons/connection2_sl.svg){: style="width:55px;"} Pipe settings

![](/img/client/pipetypes.png)

This panel is used to list and manage all the different types of pipes of the project which are used by the [**pipeline**](../../pipeline/pipeline.md).

Pipes can also be more easily created, edited and connected from the [**pipeline editor**](pipeline.md).

**Pipe types** define the type of files and data which can be used through the **pipes** between **steps**.

▹ **[More details about the pipeline](../../pipeline/pipeline.md)**.

## Edition

![](/img/client/pipetypeedition.png)

- ***ID*** is the **unique** identifier which is used in the files and folders (see the [*Ramses Tree*](../files/index.md) section). **It's this *ID* which is passed to the [*Ramses Add-ons*](../addons/index.md)** to let them know what they need to export or import, along with the other information from the pipe. Read the [*Ramses naming conventions*](../files/naming.md) to learn how *IDs* and names have to be formatted.
- ***Comment*** can be any text associated to the pipe and describing it.
- Use the ***Type*** list to set the [type](../../pipeline/pipeline.md) of the step. [Read this for more information](../../pipeline/pipeline.md) about step types.
- You can select one ***File type*** to be used through this pipe; if you need several different types, you can use multiple pipes between steps. Read the [*Pipeline Editor*](pipeline.md) section for more information.
- You can select one ***Color space*** to be used through this pipe; if you need several different color spaces, you can use multiple pipes between steps. Read the [*Pipeline Editor*](pipeline.md) section for more information.
- Use the ***Custom Settings*** to set default settings to be used by *Ramses Add-ons* to automate the pipeline. These settings will be read when importing this specific file type, if it is assigned to a incoming pipe of the corresponding steps. Using the official *Ramses Add-ons*, these settings should be formatted using the [*Yaml*](http://yaml.org) syntax. Read the [*Add-ons*](../addons/index.md) section for more information about the settings you can use.

▹ **[More details about the pipeline](../../pipeline/pipeline.md)**.

!!! warning
    Color spaces are not implemented yet!

[^1]:
    The data is not actually deleted from the server nor the local data, but just flagged as removed. A server administrator can still restore the data at any time, but there is no way (yet) to do that from the user interface of the application.


