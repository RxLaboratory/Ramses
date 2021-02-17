# Ramses

The *Rx Asset Management System* is a complete framework to manage your assets, shots and generally speaking your motion pictures production pipeline.

Its several [components](../components/index.md) all handle the same objects which are described precisely in this documentation.

Here are these objects:

- ![](../img/icons/user_sl.svg){: style="width:18px;"} A [***user***](users.md) is someone working on the production.
- ![](../img/icons/cinema-movie_sl.svg){: style="width:18px;"} A [***project***](projects.md) is the first level of the production. It could represent a feature film (or one of its sequences), a short movie, a TV Series episode...
    - ![](../img/icons/step-node_sl.svg){: style="width:18px;"} A [***step***](steps.md) is one of the stages each *asset* or *shot* has to go through to be produced (e.g. modeling, animation, lighting, compositing...)
    - ![](../img/icons/assets-properties-group_sd.svg){: style="width:18px;"} An ***Asset Group*** contains some of the *assets* of a project.
        - Asset
            - Status
    - ![](../img/icons/shots-frames_sd.svg){: style="width:18px;"} A ***Sequence*** contain some of the *shots* of a project.
        - Shot
            - Status
- State
- Application
    - File type

## Object general attributes

All these objects are defined by at least:

- A full name: a longer name describing the object.
- A short name: a short version (less than 10 characters) of the name of the object, used in UI when there is not enough room and in filenames.
- A folder: a physical location in the file structure to store its associated data.

## Object folders

Folders for object can be user-defined, and there are two ways to locate them.

- *Absolute*: the path is the exact same on all workstations.
- *Relative* (default): the path is relative to the main *Ramses* folder or to a project folder, and can vary on different workstations if these main *Ramses* or project folders are not at the same location.

The easiest way to let *Ramses* manage your file structure is to keep using relative paths, even for projects which can all be located inside the main *Ramses* folder.

Read the [*Ramses Tree*](../files/tree.md) section of this documentation for more information.

### Setting folder paths

*Ramses* will automatically detect if a path if relative or absolute, based on these rules:

- On Linux and MacOS, if a path starts with `/` or `~` it is aboslute, otherwise it is relative.
- On Windows, if a path starts with a drive specification like `C:\` or with `\\` it is absolute, otherwise it is relative.

When the path is relative, *Ramses* automatically locate the right parent according to the type of object (either the *Ramses* main folder, or the *users* subfolder, the *project* folder, etc.). Read the [*Ramses Tree*](../files/tree.md) section of this documentation for more information.