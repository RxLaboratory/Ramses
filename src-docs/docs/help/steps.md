# ![](../img/icons/connections_sl.svg){: style="width:32px;"} Steps

*Steps* represent the stages the assets and shots have to go through before completion, e.g. Modeling, Rigging, Animation, Lighting...

There are 4 different types of steps:

- ![](../img/icons/cinema-movie_sl.svg){: style="width:18px;"} ***Pre-Production*** includes all the steps of the work which has to be done to prepare and develop the project, but which will not actually be included in the project (e.g. the storyboard, the script, etc.)
-  ![](../img/icons/property-asset_sl.svg){: style="width:18px;"} ***Asset Production*** steps are the steps building assets which are needed to finalize the shots of the project (e.g. the characters, props, sets, etc.)
-  ![](../img/icons/shot-frame_sd.svg){: style="width:18px;"} ***Shot Production*** steps are the stages where shots are built using assets (e.g. animation, lighting, compositing, etc.)
-  ![](../img/icons/film_sl.svg){: style="width:18px;"} ***Post-Production*** includes all the steps from the edit to the delivery, where the work is made on the whole edit and not individual shots (e.g. editing, sound design, music, etc.)

Each steps also stores the list of users assigned to it.

!!! hint
    *Ramses* comes with a set of step templates, but they can be completely adjusted to any production needs; for example, if you consider *Character Desgin* to be a pre-production step instead of an asset production step, it's up to you to customize the templates.

## Attributes

As any other object in *Ramses*, the steps are defined by their:

- ***Name***
- ***Short Name***

The steps also have these additional attributes:

- ***Type***: either *Pre-Production*, *Asset Production*, *Shot Production* or *Post-Production*
- ***Users***: The list of users assigned to the step.
- ***Folder***: Each step also has its own folder inside the project file structure, to keep all common files for the given step. See the [*Ramses Tree*](../files/tree.md) for more information.