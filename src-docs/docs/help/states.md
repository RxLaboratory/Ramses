# ![](../img/icons/check_sl.svg){: style="width:32px;"} States

*States* are part of *assets* and *shots* *status*, along with comments and history. They also define a also percentage of completion of the work for the corresponding *step* on the *asset* or *shot*, which is used for production management and evaluating the global completion of a project.

They can be configured globally to be used everywhere there's a need to show a *status*.

By default, *Ramses* includes a few predefined states, and three of them cannot be removed as they are needed by *Ramses* automations.

## Needed states

These states cannot be removed from *Ramses*, but you can modify all their attributes except their short name.

- ***Finished*** / ***OK*** (100%). To be used when an *asset*/*shot* is completely finished.
- ***To do*** / ***TODO*** (0%). This *asset*/*shot* is ready to be worked on.
- ***Stand by*** / ***STB*** (0%). Some dependencies are not satisfied to be able to work on this *asset*/*shot* yet.
- ***Nothing to do*** / ***NO*** (100%). There's nothing to do with this *asset*/*shot* for this step. These are ignored in the calculation of the completion of the project.

## Optional predefined states

These states are provided as examples, you can modify or remove them.

- ***Work in progress*** / ***WIP*** (50%)
- ***To be rendered*** / ***RDR*** (90%)
- ***Rendering*** / ***RDRNG*** (95%)
- ***Could be better*** / ***CBB*** (95%)
- ***To be checked*** / ***CHK*** (85%)
- ***Needs a retake*** / ***RTK*** (75%)

## Attributes

As any other object in *Ramses*, the steps are defined by their:

- ***Name***
- ***Short Name***

The states also have these additional attributes:

- ***Completion Ratio***: when an *asset* or a *shot* is in this state, it should be considered completed at this percentage.
- ***Color***: each state has its own color which is used for displaying them in the interfaces of the *Ramses* components, and provide a quick visual feedback on the production of the *assets*, *shots*, and *projects*.