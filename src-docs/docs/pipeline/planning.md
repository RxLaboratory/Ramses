![META](authors:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2021;updated:2021/07/12)

# ![](../img/icons/calendar_sl.svg){: style="width:55px;"} Production Planning

<figure>
  <img src="/img/client/schedule.png"/>
  <figcaption>The schedule table, as shown in the <i>Ramses Client Application</i>.</figcaption>
</figure>

!!! warning
    **Ramses is still under active development**, and this documentation is still being written.

    We strongly invite you to read the [changelog](../changelog.md) to know what's going on, and especially to have a look at the [known issues](../changelog.md).

As *Ramses* is able to (automatically) estimate the time needed for the production of the individual **assets** and **shots**, it can help you plan the production, and keep track of the **time needed** to produce any project.

At the **step** level, **default estimations** can be set to be used on shots and assets, depending on the difficulty of their productions. Then, each individual **estimation** can be customized for a finer evaluation of the time needed for the production.

## Estimations

To establish a realistic schedule, it is important to be able to estimate the time needed to produce the project, as well as tracking the production to adjust these estimations if needed.

*Ramses* helps you to quickly estimate this time needed, and then helps you to make it as precise as you need.

The unit used for estimations by *Ramses* is the *day* (which can be divided into smaller parts of a day if you need). Counting in days is precise enough for planning a production, and this way you don't need to know exactly how many hours are (actually) worked in a day.

!!! note
    There's no way with *Ramses* to link the production tracking data to individual users (meaning that there's no easy way to build statistics about users or compare their productivity). That's by design, don't ask for it.

### ![](../img/icons/connections_sl.svg){: style="width:48px;"} Step estimations

For each step, it is possible to set a **default estimation** which will be used for all assets and shots.

This estimation, usually set in days of work, can vary with the **difficulty** for the production of each specific asset or shot; *Ramses* lets you define an estimation for five predefined levels of difficulty: *very easy*, *easy*, *medium*, *hard*, *very hard*.

This default estimation is used to **automatically evaluate the time needed** for the production of the whole step, depending on the assets or shots which have to be produced, at first based on a medium difficulty. **Adjusting the difficulty per shot or asset** will then automatically update the global estimation to raise the precision.

For the production of **shots**, the estimations can also be optionnaly and automatically multiplied by the **duration** of the shot, so that the estimation can be made **per second** instead of **per shot**, and also by the **number of assets used by the shot**, for example to multiply the estimation by the number of characters in the shot.

▹ **[Step management in the *Ramses Client Application*](../components/client/steps.md)**.

### ![](../img/icons/assets_sl.svg){: style="width:48px;"} Asset and shots estimations

The estimation set by default by the **step** and automatically computed based on the number of assets, the duration and the difficulty can always be individually **overriden** for each shot and asset, for a more precise control on the estimations.

This is done through the [*Production Tracking*](production.md)

▹ **[Production tracking in the *Ramses Client Application*](../components/client/production.md)**.

## ![](../img/icons/calendar_sl.svg){: style="width:48px;"} Schedule

Based on the estimations, *Ramses* evaluates the number of **days of work** needed to complete the work on each **step** for all assets and shots.

It is then easier to prepare the production planning and establish a schedule, by assigning days of work to **users** and **steps**.

*Ramses* will keep track of the number of days already assigned and missing (or extra) days to help you plan the production.

In case of **lateness** on the production of a step (shown by *Ramses*), it means the quantity or the diffuclty of the work was underestimated; all that needs to be done is then to **adjust the estimations at the step level** to update all assets and shots estimations, then check if the schedule can still be respected, or assign more users or days of work to the step.

▹ **[Schedule panel in the *Ramses Client Application*](../components/client/schedule.md)**.