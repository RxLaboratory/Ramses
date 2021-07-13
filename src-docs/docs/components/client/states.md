![META](authors:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2021;updated:2021/07/13)

# ![](/img/icons/check_sl.svg){: style="width:55px;"} State management

![](/img/client/states.png)

This panel is used to list and manage all the states used by *Ramses* for production tracking. **The states are shared by all projects**.

The list of available states can be completely customized.

Read the section about [*Production tracking*](../../pipeline/production.md) for more details about states and status.

## List

![](/img/client/statelist.png)

The list on the left lists all the states, and you can use the ***Search*** field to filter them by ***name*** or ***ID***.  
When changing the size of the rows in the list, more details about each state can be displayed.

Use the ![](/img/icons/add_sl.svg){: style="width:18px;"} button to **create** a new state. The ![](/img/icons/remove_sl.svg){: style="width:18px;"} button **removes** the selected states from *Ramses*[^1].

You can also use the `[DEL]` key to remove selected states.

You can scroll all lists using `[Middle click]` as well as the `[Mouse wheel]`.

**A few states can't be removed** as they are needed by *Ramses* automations (but they can still be customized, only their *ID* can't change): ***NO, STB, TODO, WIP, OK***.

## Edition

![](/img/client/stateedition.png)

- ***Name*** is the state's display name.
- ***ID*** is the **unique** identifier which is used in the files and folders (see the [*Ramses Tree*](../files/index.md) section and the [*Ramses naming convetions*](../files/naming.md)). It is also used in the user interface when there is too little room to use the name.
- ***Comment*** can be any text associated to the state. You can use it to describe the state and explain what it means. This comment has nothing to do with the actual production status of shots and assets, it is linked to the state itself and just describes it.
- The ***color*** is used to display the states in the user interface and easily differenciate them in lists.
    - Input a custom hexadecimal color code in the field.
    - Click the ![](/img/icons/color2.svg){: style="width:18px;"} button to use a nice color selector.
- The ***completion ratio*** is used to automatically set a default value to the completion of the tasks when their state changes. This task completion ratio can then be adjusted by the user for each individual task.

▹ **[More details about Production tracking and the states](../../pipeline/production.md#states)**.

[^1]:
    The data is not actually deleted from the server nor the local data, but just flagged as removed. A server administrator can still restore the data at any time, but there is no way (yet) to do that from the user interface of the application.
