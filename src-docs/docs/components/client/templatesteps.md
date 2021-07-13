![META](authors:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2021;updated:2021/07/13)

# ![](/img/icons/connections_sl.svg){: style="width:55px;"} Template Step management

![](/img/client/templatesteps.png)

This panel is used to list and manage all template steps of *Ramses*.

Template steps can be used when configuring a **project** and its [**pipeline**](../../pipeline/pipeline.md), to quickly create steps for the project. Read the section about [*Pipelines*](../../pipeline/pipeline.md) for more details about steps, and the section about the [step management in the *Ramses Client Application*](steps.md) to learn how to create and manage steps inside projects.

## List

![](/img/client/templatesteplist.png)

The list on the left lists all the template steps, and you can use the ***Search*** field to filter them by ***name*** or ***ID***.  
When changing the size of the rows in the list, more details about each template step can be displayed.

Use the ![](/img/icons/add_sl.svg){: style="width:18px;"} button to **create** a new template step. The ![](/img/icons/remove_sl.svg){: style="width:18px;"} button **removes** the selected template steps from *Ramses*[^1].

You can also use the `[DEL]` key to remove selected template steps.

You can scroll all lists using `[Middle click]` as well as the `[Mouse wheel]`.

## Edition

![](/img/client/templatestepedition.png)

!!! hint
    All these settings can be safely changed at any time as this information is not actually used in *Ramses* but just read at the moment a step is created from a template.

- ***Name*** is the template step's display name.
- ***ID*** is the **unique** identifier which is used in the files and folders (see the [*Ramses Tree*](../files/index.md) section and the [*Ramses naming convetions*](../files/naming.md)).
- ***Comment*** can be any text associated to the template step.
- Use the ***Type*** list to set the [type](../../pipeline/pipeline.md) of the step. [Read this for more information](../../pipeline/pipeline.md) about step types.
- The ***color*** is used to display the steps in the user interface and easily differenciate them in lists and the [schedule](schedule.md).
    - Input a custom hexadecimal color code in the field.
    - Click the ![](/img/icons/color2.svg){: style="width:18px;"} button to use a nice color selector.
- You can set the default ***estimation*** for the step. Read the section about [*Production Planning*](../../pipeline/planning.md) for more information.
    - For *Shot Production* steps, you can select if the estimation has to be evaluated **per shot** or on the duration, **per second**.
    - You can set a different default estimation for shots or assets depending on the **difficulty**. Estimation is made in **days**.

▹ **[More details about Steps and *Ramses*](../../pipeline/pipeline.md#steps)**.

▹ **[More details about estimations and planning](../../pipeline/planning.md)**.

[^1]:
    The data is not actually deleted from the server nor the local data, but just flagged as removed. A server administrator can still restore the data at any time, but there is no way (yet) to do that from the user interface of the application.
