![META](authors:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2021;updated:2021/07/13)

# ![](/img/icons/cinema-movie-settings_sl.svg){: style="width:55px;"} Project management

![](/img/client/project_management.png)

This panel is used to list and manage all projects of *Ramses*.

## List

![](/img/client/projectlist.png)

The list on the left lists all the projects, and you can use the ***Search*** field to filter them by ***name*** or ***ID***.  
When changing the size of the rows in the list, more details about each project can be displayed. The ![](/img/icons/explore.svg){: style="width:18px;"} ***folder*** button on the top right corner for each project in the list opens the project folder in your file explorer.

Use the ![](/img/icons/add_sl.svg){: style="width:18px;"} button to create a new project. The ![](/img/icons/remove_sl.svg){: style="width:18px;"} button **removes** the selected projects from *Ramses*[^1].

You can also use the `[DEL]` key to remove selected projects.

You can scroll all lists using `[Middle click]` as well as the `[Mouse wheel]`.

## Edition

![](/img/client/projectedition.png)

- ***Name*** is the project's display name and can be changed any time.
- ***ID*** is the **unique** identifier which is used in the files and folders (see [*Ramses Tree*](../files/index.md)). It can't be changed after the project folder has been created. Read the [*Ramses naming conventions*](../files/naming.md) to learn how *IDs* and names have to be formatted.
- ***Comment*** can be any text associated to the project.
- ***Resolution*** is the **width** and **height** of the final video, in pixels. The first button allows you to quickly select from a list of common resolutions.
- ***Framerate*** is the framerate of the final video, in frames per second. Use the first button to quickly select it from a list of common framerates.
- ***Deadline*** is the date at which the project must be finished and delivered.
- You can change the ***folder*** where the project files are stored.
    - Leave the field empty to set back the default folder inside the [*Ramses Tree*](../files/index.md).
    - The blue ![](/img/icons/set-folder_sl.svg){: style="width:18px;"} button opens a folder selector to change the folder location.
    - The green ![](/img/icons/go-to-explore-reveal-folder_sl.svg){: style="width:18px;"} button opens the folder in your file explorer.
- Use the ***Users*** list to assign and unassign users to the project.
    - The ![](/img/icons/add_sl.svg){: style="width:18px;"} button contains a list of available users, and an entry to quickly create a new user without having to switch to the [users](users.md) tab.
    - The ![](/img/icons/remove_sl.svg){: style="width:18px;"} button unassigns the selected users from the project but does not remove them from *Ramses*. You can also use the `[DEL]` key.
    - From the list, you can use the ![](/img/icons/explore.svg){: style="width:18px;"} *folder* button to quickly open a specific user folder, and the ![](/img/icons/edit-comment-modify_sl.svg){: style="width:18px;"} *edit* button to edit them in a side docked panel. Read the [*user management*](users.md) section for more details about user edition.

▹ **[More details about Projects and *Ramses*](../../pipeline/administration.md#projects)**.

[^1]:
    The data is not actually deleted from the server nor the local data, but just flagged as removed. A server administrator can still restore the data at any time, but there is no way (yet) to do that from the user interface of the application.