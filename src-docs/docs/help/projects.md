# ![](../img/icons/cinema-movie-settings_sl.svg){: style="width:32px;"} Projects

*Projects* represent the final artwork: a feature film or one of its sequence, an episode in a TV series, a short movie, a music video...

It is made of *Shots* which are in turned made of *Assets*.

*Assets* and *Shots* are produced through different *Steps* which are interdependent.

*Shots* are grouped in *Sequences*, and a project contains at least one sequence.

*Assets* are grouped in *Asset Groups* (e.g. characters, sets, props...).

!!! note
    It is not mandatory to use sequences, and for short projects (less than a hundred shots), it is usually easier to just have a single sequence.

## Attributes

As any other object in *Ramses*, the projects are defined by their:

- ***Name***
- ***Short Name***
- ***Framerate*** is the framerate of the project.
- ***Height*** is the height of the final video, in pixels.
- ***Width*** is the width of the final video, in pixels.
- ***Aspect Ratio*** is the aspect ratio of the final video (which may be different than width/height if it does not use square pixels).
- ***Project Folder*** which is by default in the *projects* subfolder of the main *Ramses* folder.

The projects also have these additional attributes:

- ![](../img/icons/connections_sd.svg){: style="width:18px;"} ***Steps***: the list of steps in the production pipeline.
- ![](../img/icons/assets-properties-group_sd.svg){: style="width:18px;"} ***Asset Groups*** contain the *Assets* used in this project.
- ![](../img/icons/shots-frames_sd.svg){: style="width:18px;"} ***Sequences*** or shot groups which contain the *Shots* of the projects.
- ![](../img/icons/users_bd.svg){: style="width:18px;"} ***Users***: the list of users assigned to this project.