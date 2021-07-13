![META](authors:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2021;updated:2021/07/13)

# ![](/img/icons/applications_sl.svg){: style="width:55x;"} Application management

![](/img/client/applications.png)

This panel is used to list and manage all the applications *Ramses* needs to know for the [pipeline](../../pipeline/pipeline.md) management.

Read the section about the [*Pipeline*](../../pipeline/pipeline.md) for more details about how applications are used.

## List

![](/img/client/applicationlist.png)

The list on the left lists all the applications, and you can use the ***Search*** field to filter them by ***name*** or ***ID***.  

Use the ![](/img/icons/add_sl.svg){: style="width:18px;"} button to **create** a new application. The ![](/img/icons/remove_sl.svg){: style="width:18px;"} button **removes** the selected applications from *Ramses*[^1].

You can also use the `[DEL]` key to remove selected applications.

You can scroll all lists using `[Middle click]` as well as the `[Mouse wheel]`.

*Ramses* comes with a predefined list of applications, but this list is completely customizable: you can remove and add as many applications as you need.

## Edition

![](/img/client/applicationedition.png)

- ***Name*** is the state's display name.
- ***ID*** is the **unique** identifier which is used in the files and folders (see the [*Ramses Tree*](../files/index.md) section and the [*Ramses naming convetions*](../files/naming.md)).
- ***Comment*** can be any text associated to the application, describing it.
- ***Executable file*** is the path to the executable binary used to launch the application.
    - The blue ![](/img/icons/set-folder_sl.svg){: style="width:18px;"} button opens a file selector to change the file path.
- The ***Native***, ***Import*** and ***Export*** buttons display three lists of file types to be associated with the application:
    - Native file types are the ones which can be edited (read and written while keeping them editable) with the application.
    - Import file types are files which can be read by the application.
    - Export file types are files which can be written by the application, but losing the ability to edit it later.
    - The ![](/img/icons/add_sl.svg){: style="width:18px;"} button contains a list of available file types, and an entry to quickly create a new file type without having to switch to the [file types](filetypes.md) tab.
    - The ![](/img/icons/remove_sl.svg){: style="width:18px;"} button unassigns the selected file types from the application but does not remove them from *Ramses*. You can also use the `[DEL]` key.
    - From the list, you can use the ![](/img/icons/edit-comment-modify_sl.svg){: style="width:18px;"} *edit* button to edit them in a side docked panel. Read the [*file type management*](filetypes.md) section for more details about file type edition.

!!! tip
    The *executable file* field is optional. To open a specific file, if this field is left empty, *Ramses* will use the default application from the system instead, so it may be quicker and easier to set the file association at the system level.

    Using this field can be handy in case you're using different versions of the same software, or if you want to override default file association, in which case  *Ramses* needs the actual executable file path.

▹ **[More details about the pipeline](../../pipeline/pipeline.md)**.

[^1]:
    The data is not actually deleted from the server nor the local data, but just flagged as removed. A server administrator can still restore the data at any time, but there is no way (yet) to do that from the user interface of the application.
