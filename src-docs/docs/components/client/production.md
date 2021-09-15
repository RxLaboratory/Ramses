![META](authors:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2021;updated:2021/07/13)

# ![](/img/icons/assets_sl.svg){: style="width:55px;"} Asset and shot production tracking

![](/img/client/assettracking.png)

There are two panels to track the production of the **shots** and of the **assets**. From these tables, you can manage the **status** of all the assets and all the shots of the project, check their **completion ratio**, how much **time was spent** for each of them, check the **estimation**, and finally **how late (or not) you are**...

▹ **[More details about production tracking](../../pipeline/production.md)**.

## Overview

![](/img/client/production.png)

The production tables have three main areas: the **toolbar** on top contains a few useful menus and tools, the **main table** is where shots or assets are listed, and the **dock** shows the editors (on the right side of the main table by default).

### Toolbar

![](/img/client/productiontoolbar.png)

On its left side, the toolbar displays a few menus and tools.

- ![](/img/icons/select.svg){: style="width:18px;"} The first button is a selector to let you filter the displayed asset by asset group, or the shots by sequence.
- Use the ***Search*** field to filter the shots or assets according to their **names** or **IDS**.
- ![](/img/icons/show_sl.svg){: style="width:18px;"} View
    - ***Show time tracking***: when checked, information about the time spent (and lateness) will be shown on the colored progress bars in the main table.
    - ***Show completion***: when checked, information about the completion will be shown on the colored progress bars in the main table.
- ![](/img/icons/step-node_sl.svg){: style="width:18px;"} Steps
    - ***Select all*** shows all the steps in the table.
    - ***Select none*** hides all the steps from the table.
    - ***Select my steps*** shows only the steps where the current user has some tasks assigned.
    - Use the list of ***steps*** to select the ones you want to be shown in the main table.
- ![](/img/icons/state.svg){: style="width:18px;"} Status
    - ***Assign User*** assigns a user to all selected tasks.
    - ***Change state*** changes the state of all selected tasks.
    - ***Changer difficulty*** changes the difficulty of all selected tasks.
    - ***Set completion*** changes the completion ratio of all selected tasks.

If the current user is at least a *[project adminstrator](../../pipeline/administration.md#users)*, the toolbar also displays an entry to let you quickly create new items (shots or assets) or remove selected items.

!!! tip
    The ***Status*** menu is also availabe as a context menu with the `[right click]`.

### Main table

![](/img/client/productiontable.png)

From the main table, you can have a look at all the **status** of all shots or assets, and a summary for steps on the column headers.

The **progress bar in the step headers** goes from red to green when the **completion ratio** raises. If a second reddish part is shown to the right of the progress bar, it means the production of this step is late; the bigger and the redder it is, the later you are!

The first column contains all the information about the shots or the assets (duration, tags, etc.).

For all cells, you can open the corresponding folder with the ![](/img/icons/go-to-explore-reveal-folder_sl.svg){: style="width:18px;"} *folder* button, and if you have enough rights you can edit the corresponding shot, asset or status with the ![](/img/icons/edit-comment-modify_sl.svg){: style="width:18px;"} *edit* button.

When changing the size of the rows, more details can be shown for each task, including a preview image.  
The image is pulled from the `_preview` subfolder of the shot or the asset folder which you can open with the ![](/img/icons/go-to-explore-reveal-folder_sl.svg){: style="width:18px;"} *folder* button on the top right corner of the cell. *Ramses* will get the first *PNG* or *JPG* [correctly named](../files/naming.md) image from the folder, or any other image if a correct one is not found.

Each cell contains information about a task (the combination of an asset or shot and a step).

![](/img/client/assetstatus.png)

The title of the cell shows the *ID* of the asset or shot, of the step, and of the current state. The current version of the working file is shown in parenthesis.

The ![](/img/icons/history.svg){: style="width:18px;"} button shows the status history of the task.

The **progress bar** can show multiple information.

- Its color corresponds to the current [state color](states.md).
- It shows a combination of the completion ratio and the time spent on the task, depending on the options checked in the *View* menu.

In the following example, the completion is a bit more than 50%, and the color is the default yellow of the *Work in progress* state.

<figure>
  <img src="/img/client/progressadvance.png"/>
  <figcaption>The darker part shows where we should be after the time spent: we're a bit in advance!</figcaption>
</figure>

<figure>
  <img src="/img/client/progresslate.png"/>
  <figcaption>The darker part shows where we should be after the time spent: we're a bit late.</figcaption>
</figure>

<figure>
  <img src="/img/client/progressverylate.png"/>
  <figcaption>Lateness is shown in red, we're very late...</figcaption>
</figure>

If you uncheck the `View ▸ Show time tracking` option, you can view only the completion ratio:

![](/img/client/progresswithouttime.png)

When unchecking the `View ▸ Show completion` option, you can view only the time spent:

<figure>
  <img src="/img/client/progresstime.png"/>
  <figcaption>If it's red, it's not a good sign... Grey would mean we're not late, but red...</figcaption>
</figure>

Click the ![](/img/icons/edit-comment-modify_sl.svg){: style="width:18px;"} *edit* button to edit the current status.

![](/img/client/statusedition.png)

!!! warning
    Unlike other editors, changes made to status are not saved automatically. Don't forget to click the ![](/img/icons/apply_sl.svg){: style="width:18px;"} *apply* button.

- Use the colored ***state*** box on the top left to change the current state.
- Use the ***completion*** slider on the top right to change completion ratio. Just click the slider without dragging to precisely edit the value.
- You can add a ***comment*** just below the completion ratio.
- Changing the ***difficulty*** will change the automatic estimation, according to the [settings](steps.md) of the corresponding [step](steps.md).
- Uncheck the ***Auto*** checkbox to ***manually estimate*** the time you need to finish the task. Once unchecked, the estimation will not be automatically changed anymore if you change the difficulty or if the [step settings](steps.md) change.
- The ***time spent*** is automatically updated by *Ramses* when you edit the status, according to the information it gets from the [*Ramses Add-ons*](../addons/index.md) and the data it gathers from [the files](../files/index.md) if they're correctly named and located. You can manually override this value.
- The current ***version*** should be automatically set by *Ramses* if you're using the [*Ramses Add-ons*](../addons/index.md); otherwise you can set it manually.
- If the task is published and ready to be used and imported in the next step, check the ***Published*** box. Again, *Ramses* should check it automatically if you've published the task using the [*Ramses Add-ons*](../addons/index.md).
- The bottom box is used to ***Assign a user*** to the task. Leave it to *All Users* to unassign the task.

All these settings can be set precisely by the [*Ramses Add-ons*](../addons/index.md) thanks to [pipeline](../../pipeline/pipeline.md) automations, and the resulting data can be used for [production tracking](../../pipeline/production.md)) and [planning](../../pipeline/planning.md)

▹ **[More details about the pipeline](../../pipeline/pipeline.md)**.

▹ **[More details about production tracking](../../pipeline/production.md)**.

▹ **[More details about planning](../../pipeline/planning.md)**.

The bottom part of the status editor shows the available ***working***, ***published*** and ***preview*** files.

From these three lists, you can open any file by just double-clicking it. You can restore previous versions of working and published files, and the ![](/img/icons/add_sl.svg){: style="width:18px;"} *add* button in the *Work* tab can create a new file from a step template (if any template has been published).

Files must respect the [*Ramses naming scheme*](../files/naming) and [folders](../files/tree.md) to be detected and listed here, but you don't necessarily have to use the [*Ramses Add-ons*](../addons/index.md), all files will be detected and handled by *Ramses*.

## ![](/img/icons/counter_bl.svg){: style="width:55px;"} Statistics

The ***Statistics*** button on the title bar of the application opens a summary of the progress of all steps and the whole project.

![](/img/client/statistics.png)

The top part shows the statistics for the whole project.

- ***Time remaining*** is the count of days before reaching the deadline of the project.
- ***Completion*** is the global completion of the project.
- ***Lateness*** is an estimation of the lateness for all steps; 0% means all is well, a negative lateness means there's some advance (no, this never happens), a positive value means the production is late. 100% means it took twice the time originally estimated to get to the current completion ratio!
- The ***estimation*** shows how many days are needed, according to all estimations, to finishe the project.
- The last line shows if you need to assign more days in the [schedule](schedule.md) ("missing") or if you have some extra days planned.

The same information is available for all steps.
