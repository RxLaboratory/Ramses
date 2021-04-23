# ![](../img/icons/sequences.svg){: style="width:32px;"} Sequences

*Sequences* are a way to group *Shots*. A project contains at least one *Sequence*.

You can use projects and sequences in many different ways:

- Series (or a single season) can be a single project, each episode would then be a sequence. This works well for short episodes without too many shots.
- You can subdivide a long project, like a feature film, into sequences.
- For a short project, you could use a single sequence to make things simpler.

Each Sequence stores a list of [Shots](shots.md), and each [Shot](shots.md) belong to one and only one Sequence.

To the contrary of *[Assets](assets.md)* and *[Asset Groups](assetgroups.md)*, sequence don't have their own folder. All shots are always stored in the same folder (read *[Ramses Tree](../files/tree.md)* for more information).

## Attributes

As any other object in *Ramses*, the sequences are defined by their:

- ***Name***
- ***Short Name***

The sequences also have these additional attributes:

- ***Shots***: The list of shots stored by this sequence.

____

![META](authors:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2021;updated:2021/04/22)