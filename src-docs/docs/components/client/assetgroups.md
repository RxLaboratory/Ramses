![META](authors:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2021;updated:2021/07/13)

# ![](/img/icons/assetgroups_sl.svg){: style="width:55px;"} Asset group settings

![](/img/client/assetgroups.png)

This panel is used to list and manage all the asset groups of the project. Asset groups are used to help you sort and filter the **assets** of the **project**. They're also used in the [***Ramses Tree***](../files/tree.md).

## List

![](/img/client/assetgrouplist.png)

The list on the left lists all the asset groups of the project, and you can use the ***Search*** field to filter them by ***name*** or ***ID***.  
When changing the size of the rows in the list, more details about each asset group can be displayed. The ![](/img/icons/explore.svg){: style="width:18px;"} ***folder*** button on the top right corner for each asset group in the list opens the asset group folder in your file explorer.

Use the ![](/img/icons/add_sl.svg){: style="width:18px;"} button to **create** a new asset group (either from scratch or using a template). The ![](/img/icons/remove_sl.svg){: style="width:18px;"} button **removes** the selected asset groups from the project[^1].

You can use the `[Shift + A]` shortcut key to quickly add new asset groups.

You can use the `[DEL]` or `[Shift + X]` shortcut key to quickly remove selected asset groups.

You can scroll all lists using `[Middle click]` as well as the `[Mouse wheel]`.

## Edition

![](/img/client/assetgroupedition.png)

- ***Name*** is the asset group's display name. **It can't be changed after the asset group folder has been created**.
- ***ID*** is the **unique** identifier which is used in the files and folders (see the [*Ramses Tree*](../files/index.md) section). Read the [*Ramses naming conventions*](../files/naming.md) to learn how *IDs* and names have to be formatted.
- ***Comment*** can be any text associated to the asset group and describing it.
- The ![](/img/icons/go-to-explore-reveal-folder_sl.svg){: style="width:18px;"} button opens the asset group **folder** in your file explorer. This folder contains the individual asset folders.
- ***Assets*** contained in the group are listed in the bottom section.
    - The ![](/img/icons/add_sl.svg){: style="width:18px;"} button instantly **creates a new asset** contained in the current group.
    - The ![](/img/icons/remove_sl.svg){: style="width:18px;"} button removes the selected assets from the project[^1]. You can also use the `[DEL]` key.
    - From the list, you can use the ![](/img/icons/explore.svg){: style="width:18px;"} *folder* button to quickly open a specific asset folder, and the ![](/img/icons/edit-comment-modify_sl.svg){: style="width:18px;"} *edit* button to edit them in a side docked panel. Read the [*asset settings*](assets.md) section for more details about asset edition.

!!! tip
    To change the *name* of the asset group, you can temporarilly rename its corresponding folder, change the *name* in *Ramses*, and finally rename the folder after the new name.

[^1]:
    The data is not actually deleted from the server nor the local data, but just flagged as removed. A server administrator can still restore the data at any time, but there is no way (yet) to do that from the user interface of the application.


