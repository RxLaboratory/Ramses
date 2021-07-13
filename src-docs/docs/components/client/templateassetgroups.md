![META](authors:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2021;updated:2021/07/13)

# ![](/img/icons/assetgroups_sl.svg){: style="width:55px;"} Template Asset Group management

![](/img/client/templateassetgroups.png)

This panel is used to list and manage all template asset groups of *Ramses*.

Template assset groups can be used when configuring a **project** and its [**pipeline**](../../pipeline/pipeline.md), to quickly create asset groups for the project. Read the section about [*Production tracking*](../../pipeline/production.md) for more details about asset groups, and the section about the [asset group management in the *Ramses Client Application*](assetgroups.md) to learn how to create and manage asset groups inside projects.

## List

![](/img/client/templateassetgrouplist.png)

The list on the left lists all the template asset groups, and you can use the ***Search*** field to filter them by ***name*** or ***ID***.  
When changing the size of the rows in the list, more details about each asset group can be displayed.

Use the ![](/img/icons/add_sl.svg){: style="width:18px;"} button to **create** a new template asset group. The ![](/img/icons/remove_sl.svg){: style="width:18px;"} button **removes** the selected template asset groups from *Ramses*[^1].

You can also use the `[DEL]` key to remove selected template asset groups.

You can scroll all lists using `[Middle click]` as well as the `[Mouse wheel]`.

## Edition

![](/img/client/templateassetgroupedition.png)

!!! hint
    All these settings can be safely changed at any time as this information is not actually used in *Ramses* but just read at the moment an asset group is created from a template.

- ***Name*** is the template asset group's display name.
- ***ID*** is the **unique** identifier which is used in the files and folders (see the [*Ramses Tree*](../files/index.md) section and the [*Ramses naming convetions*](../files/naming.md)).
- ***Comment*** can be any text associated to the template asset group.

!!! note
    Asset groups are one of the only elements for which the ***name*** is used as a folder name as part of the [*Ramses Tree*](../files/index.md).

▹ **[More details about Assets and *Ramses*](../../pipeline/production.md#assets)**.

[^1]:
    The data is not actually deleted from the server nor the local data, but just flagged as removed. A server administrator can still restore the data at any time, but there is no way (yet) to do that from the user interface of the application.
