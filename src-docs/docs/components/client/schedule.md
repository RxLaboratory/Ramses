![META](authors:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2021;updated:2021/07/14)

# ![](/img/icons/calendar_sl.svg){: style="width:55px;"} Schedule and planning

![](/img/client/schedule2.png)

You can use the schedule panel to plan the production, per user and per step.

▹ **[More details about production planning](../../pipeline/planning.md)**.

!!! Note
    A future version of the application will include a more general schedule, to let you quickly select time ranges for each step, defining a starting and ending date for each, without having to assign tasks to individual users.

## Overview

The schedule table has two main areas: the **toolbar** on top contains a few useful menus and tools, and the **main table** is where you plan the production.

It is useful to keep the **statistics** side panel at hand when planning the production.

### Toolbar

![](/img/client/scheduletoolbar.png)

On its left side, the toolbar displays a few menus and tools.

![](/img/client/scheduletoolbarleft.png)

- ![](/img/icons/user_sl.svg){: style="width:18px;"} Users
    - ***Me***: shows only the current user in the table.
    - ***Select all***: shows all the users assigned to this project in the table.
    - ***Select none***: hides all the users from the table.
    - The ***User list*** can be used to select the users to show in the table.
- ![](/img/icons/calendar_sl.svg){: style="width:18px;"} Days
    - Select the days of the week to be shown in the table. This setting is kept across sessions, but it is a local setting, it is not shared with other workstations.
- ![](/img/icons/step-node_sl.svg){: style="width:18px;"} Step
    - ***Comment...*** adds a comment to the selected table cells (schedule entries).
    - ***Assign*** (un)assigns a step to the selected celles (schedule entries).
- ![](/img/icons/select.svg){: style="width:18px;"} ***Go to date*** scrolls the table to a specific date.
- ***Today*** scrolls the table to the current date.
- ***Deadline*** scrolls the table to the deadline of the current project.
- ***◀ Previous month*** scrolls the table to the previous month.
- ***Next month ▶***: I think you can guess what this button does.

The right side of the toolbar shows the time remaining before reaching the deadline of the project and lets you define the time range of the table.

![](/img/client/scheduletoolbarright.png)

!!! tip
    The ***Step*** menu, to assign steps and set comments, is also availabe as a context menu with the `[right click]` on the table cells.

### Main table

![](/img/client/scheduletable.png)

Each column represents a day, lines represent users. There are two lines per user to allow you to assign tasks only to half of a day.

Columns in the past are darker than columns in the future, the current day is shown in purple in the header; the project deadline is shown in red. To help you differenciate weeks, an alternate grey is used for to separate them. You can hide the days of the week which are not working days (a.k.a. Weekends) using the `Days` menu.

Planning the project with the table is very quick and easy:

- Click on any cell to assign a step to the corresponding user and date.

![](/img/client/scheduleselectstep.png)

- Use the context menu with `[Right click]` to assign steps to multiple users and multiple days at once.

![](/img/client/schedulecontextmenu.png)

- You can also add a comment to selected schedule entries; comments are shown when changing the size of the rows.
- There are a lot of different ways to select multiple cells in the table:
    - `[Left click + Drag]` to quickly select contiguous cells.
    - `[Shift + Left click]` to select the range between the active (last selected) cell and another cell.
    - `[Ctrl + Left click]` to select individual cells.
    - These three methods can be combined: for example, `[Ctrl + Left click + Drag]` to quickly select different groups of cells.

To check how many days you have to assign (or how many extra days you have already assigned) according to the estimations, use the ***Statistics*** panel.

## ![](/img/icons/counter_bl.svg){: style="width:55px;"} Statistics

The ***Statistics*** button on the title bar of the application opens a summary of the progress of all steps and the whole project.

![](/img/client/statistics.png)

For the project and each step, you can read the total number of days you have to assign (the ***estimation***), and for each step, you can read how many days have been **assigned** and how many are **missing** or how many **extra** days are assigned.
