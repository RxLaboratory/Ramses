![META](authors:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2021;updated:2021/07/13)

# ![](/img/icons/sequences.svg){: style="width:55px;"} Sequence settings

![](/img/client/sequences.png)

This panel is used to list and manage all the sequences of the project. Sequences are used to help you sort and filter the **shots** of the **project**. They're **not** used in the [*Ramses Tree*](../files/tree.md).

## List

![](/img/client/sequencelist.png)

The list on the left lists all the sequences of the project, and you can use the ***Search*** field to filter them by ***name*** or ***ID***.  
When changing the size of the rows in the list, more details about each sequence can be displayed.

Use the ![](/img/icons/add_sl.svg){: style="width:18px;"} button to **create** a new sequence. The ![](/img/icons/remove_sl.svg){: style="width:18px;"} button **removes** the selected sequences from the project[^1].

You can also use the `[DEL]` key to remove the selected sequences.

You can scroll all lists using `[Middle click]` as well as the `[Mouse wheel]`.

## Edition

![](/img/client/sequenceedition.png)

- ***Name*** is the sequence's display name.
- ***ID*** is the **unique** identifier. It is usually just a number, possibily with a simple prefix. Read the [*Ramses naming conventions*](../files/naming.md) to learn how *IDs* and names have to be formatted.
- ***Comment*** can be any text associated with the sequence and describing it.
- ***Shots*** contained in the sequence are listed in the bottom section.
    - The ![](/img/icons/add_sl.svg){: style="width:18px;"} button instantly **creates a new shot** contained in the current sequence.
    - The ![](/img/icons/remove_sl.svg){: style="width:18px;"} button removes the selected shots from the project[^1]. You can also use the `[DEL]` key.
    - From the list, you can use the ![](/img/icons/explore.svg){: style="width:18px;"} *folder* button to quickly open a specific shot folder, and the ![](/img/icons/edit-comment-modify_sl.svg){: style="width:18px;"} *edit* button to edit them in a side docked panel. Read the [*shot settings*](shots.md) section for more details about shot edition.

[^1]:
    The data is not actually deleted from the server nor the local data, but just flagged as removed. A server administrator can still restore the data at any time, but there is no way (yet) to do that from the user interface of the application.
