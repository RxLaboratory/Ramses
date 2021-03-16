# Ramses

The *Rx Asset Management System* is a complete framework to manage your assets, shots and generally speaking your motion pictures production pipeline.

[TOC]

## Objects

*Ramses* several [components](../components/index.md) all handle the same objects which are described precisely in this documentation.

Here are these objects:

- ![](../img/icons/user_sl.svg){: style="width:18px;"} A [***user***](users.md) is someone working on the production.
- ![](../img/icons/cinema-movie_sl.svg){: style="width:18px;"} A [***project***](projects.md) is the first level of the production. It could represent a feature film (or one of its sequences), a short movie, a TV Series episode...
    - ![](../img/icons/step-node_sl.svg){: style="width:18px;"} A [***step***](steps.md) is one of the stages each *asset* or *shot* has to go through to be produced (e.g. modeling, animation, lighting, compositing...)
    - ![](../img/icons/assets-properties-group_sd.svg){: style="width:18px;"} An [***Asset Group***](assetgroups.md) contains some of the *assets* of a project.
        - ![](../img/icons/property-asset_sd.svg){: style="width:18px;"} An [***Asset***](assets.md) is produced independently from the shots and can be used in several shots.
            - A ***Status*** describes the status of the production of the asset for each step.
    - ![](../img/icons/shots-frames_sd.svg){: style="width:18px;"} A ***Sequence*** contain some of the *shots* of a project.
        - A ***Shot*** is a single video used in the final edit.
            - A ***Status*** describes the status of the production of the shot for each step.
- ![](../img/icons/state.svg){: style="width:18px;"} A [***State***](states.md) is part of an asset or shot status, and defines its working state (e.g. work in progress, to do, OK, etc.)
- ![](../img/icons/application_sl.svg){: style="width:18px;"} An [***Application***](applications.md) is a piece of software used to produce the *assets* or *shots*.
    - ![](../img/icons/file_sl.svg){: style="width:18px;"} A [***File type***](filetypes.md) is used by *applications* either for working, previewing or publishing *assets* and *shots*.

## Ramses tree

The easiest way to let *Ramses* manage your files and pipeline is to just set its working folder. See the section entitled [*Ramese Tree*](../files/tree.md) for more information.

You can also set alternative paths to the *Ramses* working folder. Missing files will be looked for in these paths (and copied to the main path if available), and they will be used if the main path is not available.

Finally, you can set a backup path too, which is a copy of the main folder where all files are stored. *Ramses* is able to update this backup folder, but will never read data from it.

!!! note
    Alternative and Backup paths are not implemented yet.

## Object general attributes

All these objects are defined by at least:

- A full name: a longer name describing the object.
- A short name: a short version (less than 10 characters) of the name of the object, used in UI when there is not enough room and in filenames.

!!! warning
    The full name can be changed as any moment during the production.

    The short name can also be changed at any moment, as *Ramses* uses unique identifiers internally to refer to each objects, be be warned that the short names are used in the [*Ramses Tree*](../files/tree.md) so when changing the name of an object, you also need to rename/move all files and folders to the new name. This can easily be done at the start of the production but may become very difficult if the object is already used a lot.

## Object folders

Most of the folders are managed by *Ramses* inside the [*Ramses Tree*](../files/tree.md), but for some specific objects, the folder can also be user-defined. That's the case for:

- Projects
- Users

!!! note
    The [*Ramses Tree*](../files/tree.md) is relative to the main *Ramses* folder. This folder can be different on each workstation, NAS or other remote locations.
    
    If a custom folder is defined for a project, all its child-objects folders will be relative to that new project folder.

Read the [*Ramses Tree*](../files/tree.md) section of this documentation for more information.
