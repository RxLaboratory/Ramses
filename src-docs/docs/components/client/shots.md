![META](authors:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2021;updated:2021/07/13)

# ![](/img/icons/shots.svg){: style="width:55px;"} Shot settings

![](/img/client/shots.png)

This panel is used to list and manage all the **shots** of the **project**. There is no [production tracking](production.md) from this panel, as it is used only to edit shot settings.

## List

![](/img/client/shotlist.png)

The list on the left lists all the shots of the project, and you can use the ***Search*** field to filter them by ***name*** or ***ID***. The top selector filters the shots according to their **containing sequence**.
When changing the size of the rows in the list, more details about each shot can be displayed. The ![](/img/icons/explore.svg){: style="width:18px;"} ***folder*** button on the top right corner for each shot in the list opens the shot folder in your file explorer.

Use the ![](/img/icons/add_sl.svg){: style="width:18px;"} button to **create** a new shot in the current sequence. The ![](/img/icons/remove_sl.svg){: style="width:18px;"} button **removes** the selected shots from the project[^1].

You can also use the `[DEL]` key to remove the selected shots.

You can scroll all lists using `[Middle click]` as well as the `[Mouse wheel]`.

## Edition

![](/img/client/shotedition.png)

- ***Name*** is the shot's display name.
- ***ID*** is the **unique** identifier which is used in the files and folders (see the [*Ramses Tree*](../files/index.md) section). **It can't be changed after the shot folder has been created**. It usually is just a number. Read the [*Ramses naming conventions*](../files/naming.md) to learn how *IDs* and names have to be formatted.
- ***Comment*** can be any text associated to the shot and describing it.
- The ***Duration*** of the shot can be set either in seconds or frames. *Ramses* uses the [project framerate](projects.md) to update the other value.
- You can change the ***Sequence*** which contains this shot.
- The ![](/img/icons/go-to-explore-reveal-folder_sl.svg){: style="width:18px;"} button opens the shot **folder** in your file explorer. This folder contains the individual working files, each in its step subfolder. For more details, read the [*Ramses Tree*](../files/tree.md) section.
- Use the ***Asset*** list to assign assets to the shots.
    - The top selector filters the assets according to their **containing group**.
    - The ![](/img/icons/add_sl.svg){: style="width:18px;"} button contains the list of available assets for the current group. There's also an entry to quickly create a new asset without having to switch to the [asset management panel](assets.md).
    - The ![](/img/icons/remove_sl.svg){: style="width:18px;"} button unassigns the selected assets from the shot, but they're not removed from the project. You can also use the `[DEL]` key.
    - From the list, you can use the ![](/img/icons/explore.svg){: style="width:18px;"} *folder* button to quickly open a specific asset folder, and the ![](/img/icons/edit-comment-modify_sl.svg){: style="width:18px;"} *edit* button to edit them in a side docked panel. Read the [*asset settings*](assets.md) section for more details about asset edition.

The list of **assets** as long as the **duration** can be used to automatically multiply the estimation of the shot. Read the [planning](../../pipeline/planning.md) section for more information about estimations, and the [step management](steps.md) section to learn how to configure steps and automatic default estimations.

[^1]:
    The data is not actually deleted from the server nor the local data, but just flagged as removed. A server administrator can still restore the data at any time, but there is no way (yet) to do that from the user interface of the application.


