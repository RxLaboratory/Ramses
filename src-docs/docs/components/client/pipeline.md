![META](authors:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2021;updated:2021/07/13)

# ![](/img/icons/edit-connections-pipeline_bl.svg){: style="width:55px;"} Pipeline editor

![](/img/client/pipelineeditor.png)

The pipeline panel is accessible only to [*Administrator* and *Project administrator* users](../../pipeline/administration.md). This is where the project pipeline can be created and edited.

## Overview

The pipeline editor has three main areas: the **toolbar** on top contains a few useful menus and tools, the **nodal view** is where steps and pipes are connected, and the **dock** shows the editors (on the right side of the nocal view by default).

All docked panels can be undocked (as shown on the picture below) or docked on the other side of the nodal view, by dragging their title bar at the top of the panel.

![](/img/client/pipestepundock.png)

### Toolbar

![](/img/client/pipetoolbar.png)

On its left side, the toolbar displays a few menus.

- ![](/img/icons/frame-focus-center-target_sd.svg){: style="width:18px;"} View
    - ***Center view*** resets the zoom of the nodal view to 100% and centers it. Shortcut: `[Home]`
    - ***Center selection*** centers the selected nodes and zooms in on them. Shortcuts: `[F]` or `[.]` on the numpad.
- ![](/img/icons/select.svg){: style="width:18px;"} Select
    - ***Select all steps*** selects all the steps in the nodal view. Shortcut: `[A]`
    - ***Select children steps*** selects all the steps following the currently selected one(s). Repeat the command to expand the selection on the child side. Shortcut: `[Ctrl + A]`
    - ***Select parent steps*** selects all the steps preceding the currently selected one(s). Repeat the command to expand the selection on the parent side. Shortcut: `[Alt + A]`
- ![](/img/icons/connections_sd.svg){: style="width:18px;"} Layout
    - ***Layout all steps*** automatically arranges all the nodes using a smart (or not) algorithm to (try to) make the layout clearer. Shortcut: `[Shift + L]`
    - ***Layout selected steps*** will use the same smart (or not) algorithm to (try to) make the layout clearer for the selected nodes. Shortcut: `[Alt + L]`
- ![](/img/icons/step-node_sl.svg){: style="width:18px;"} Step
    - ***Create new step*** creates a new step from scratch in the project, and opens a side docked editor to edit its details. Shortcut: `[Shift + A]`
    - The list of ***Template steps*** can be used to quickly create a new step from an template.
    - ***Remove selected steps*** removes the selected steps from the project[^1]. Shortcut: `[Shift + X]`
    - ***Delete selection*** removes everything selected (steps and pipes) from the project[^1]. Shortcut: `[Del]`
- ![](/img/icons/connection_sl.svg){: style="width:18px;"} Pipe
    - ***Remove selected pipes*** removes the selected pipes from the project[^1], disconnected corresponding steps. Shortcut: `[Alt + X]`

On its right side, the toolbar has a few settings available.

- ![](/img/icons/frame-focus-center-target_sl.svg){: style="width:18px;"} ***Center view***, Shortcut: `[Home]`
- ![](/img/icons/frame-focus-center-target-nodes_sl.svg){: style="width:18px;"} ***Center selection***, Shortcuts: `[F]` or `[.]` on the numpad
- ***Zoom*** changes the zoom of the nodal view. Minimum zoom is 25%.
- The ***Grid size*** is used when snapping nodes and defines the snap precision. It is displayed as small crosses in the nodal view when it has focus.
- ***Snap to grid*** will snap the nodes to the grid when moving them, allowing you to perfectly align them, you maniac.

### Node view

![](/img/client/nodeview.png)

In the node view, you can move around all the nodes (steps) by just clicking and dragging them. To select multiple nodes, just click on the background and drag the mouse to use a box selection.

You can select both nodes (steps) and connections (pipes); when a node or a pipe is selected, its corresponding editor is shown in the right dock area to let you edit its details.

To manipulate the view, use the following shortcuts:

- `[middle click]` to **pan**.
- `[Ctrl + middle click]` to **zoom**.

[^1]:
    The data is not actually deleted from the server nor the local data, but just flagged as removed. A server administrator can still restore the data at any time, but there is no way (yet) to do that from the user interface of the application.


