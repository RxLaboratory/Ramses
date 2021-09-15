![META](authors:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2021;updated:2021/07/13)

# ![](/img/icons/files_sd.svg){: style="width:55px;"} File type management

![](/img/client/filetypes.png)

This panel is used to list and manage all the file types *Ramses* needs to know for the [pipeline](../../pipeline/pipeline.md) management.

Read the section about the [*Pipeline*](../../pipeline/pipeline.md) for more details about how file types are used.

## List

![](/img/client/filetypelist.png)

The list on the left lists all the file types, and you can use the ***Search*** field to filter them by ***name*** or ***ID***.  
When changing the size of the rows in the list, more details about each file type can be displayed.

The file types are sorted by main extension.

Use the ![](/img/icons/add_sl.svg){: style="width:18px;"} button to **create** a new file type. The ![](/img/icons/remove_sl.svg){: style="width:18px;"} button **removes** the selected file types from *Ramses*[^1].

You can use the `[Shift + A]` shortcut key to quickly add new file types.

You can use the `[DEL]` or `[Shift + X]` shortcut key to quickly remove selected file types.

You can scroll all lists using `[Middle click]` as well as the `[Mouse wheel]`.

*Ramses* comes with a predefined list of file types, but this list is completely customizable: you can remove and add as many types as you need.

## Edition

![](/img/client/filetypeedition.png)

- ***Name*** is the file type's display name.
- ***Main extension*** is the extension used by default for the file type. You must not include the leading dot `.`.
- ***Comment*** can be any text associated to the file type, describing it.
- ***All extensions*** is a comma separated list of all the extensions this file can use. You don't need to include the leading dot. **The main extension must be repeated in this field**.
- ***Previewable*** can be checked to let *Ramses* know that the file can be used for previews (it is lightweight and can be displayed by standard players/image viewers).

!!! note
    A file can have multiple extensions, like *JPEG* for example which can use *jpg*, *jpeg*, or *jpe*. When it has multiple extensions, this means a specific file can be renamed to change the extension and it will still work exactly the same way.

    On the contrary, even if two files can be opened with the same software they may not be the same type with different extensions; for example, both `.ma` and `.mb` files are *Maya* files, but they're two different types anyway; renaming an *ma* file to *mb* will break it.

▹ **[More details about the pipeline](../../pipeline/pipeline.md)**.

[^1]:
    The data is not actually deleted from the server nor the local data, but just flagged as removed. A server administrator can still restore the data at any time, but there is no way (yet) to do that from the user interface of the application.
