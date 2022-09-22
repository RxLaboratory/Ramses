![META](authors:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2021-2022;updated:2022/09/22)

# Add-ons

!!! warning
    **Ramses is still under active development**, and this documentation is still being written.

    For now, the only *Add-on* available is the *Autodesk Maya Add-On*, others will follow in the upcoming weeks and months.

    This specific page of the documentation is just a draft, sorry!

Ramses comes with some add-ons to be used in your creative applications (*Blender*, *Maya*, *After Effects*...). They are basic add-ons to demonstrate some capabilities of Ramses for file versionning and asset management, and can be used as templates to develop your own add-ons working with Ramses.

They will be developed one at a time, and you're welcome to contribute with your own!

## Available Add-ons

| Host Application | Language | Status | Details |
|---|---|---|---|
| ► **[API](../../dev/add-ons-reference/index.md)** | Python | ![icon](/img/icons/blue_circle.png){: style="width:16px;"} | A Beta version is already available. |
| ► **[API](../../dev/add-ons-reference/index.md)** | ExtendScript | ![icon](/img/icons/blue_circle.png){: style="width:16px;"} | For Adobe Creative Cloud |
| API | UXP | ![icon](/img/icons/red_circle.png){: style="width:16px;"} | For Adobe Creative Cloud<br/>*UXP* is a new scripting environment for Adobe Software. *Ramses* won't officially support it until it is mature enough, which is not the case as of September 2022. This being said, the *ExtendScript* API may be compatible with *UXP*. |
| Adobe After Effects | ExtendScript | ![icon](/img/icons/blue_circle.png){style="width: 16px;"} | ![icon](/img/icons/blue_circle.png){style="width: 12px;"} **Versionning**, status and comments.<br /> ![icon](/img/icons/orange_diamond.png){style="width: 12px;"} **Preview**: easy playblast or thumbnail<br /> ![icon](/img/icons/orange_diamond.png){style="width: 12px;"} **Pipeline**: publish, import, replace, update assets<br /> ![icon](/img/icons/orange_diamond.png){style="width: 12px;"} **Pipeline**: Advanced customization using publish and import settings and presets |
| Adobe Illustrator | ExtendScript | ![icon](/img/icons/orange_diamond.png){: style="width:16px;"} | Low-priority |
| Adobe Photoshop | ExtendScript | ![icon](/img/icons/orange_diamond.png){style="width: 16px;"} | ![icon](/img/icons/orange_diamond.png){style="width: 12px;"} **Versionning**, status and comments.<br /> ![icon](/img/icons/orange_diamond.png){style="width: 12px;"} **Preview**: easy playblast or thumbnail<br /> ![icon](/img/icons/orange_diamond.png){style="width: 12px;"} **Pipeline**: publish, import, replace, update assets<br /> ![icon](/img/icons/orange_diamond.png){style="width: 12px;"} **Pipeline**: Advanced customization using publish and import settings and presets |
| ► [**Autodesk Maya**](maya.md) | Python | ![icon](/img/icons/green_tick.png){style="width: 16px;"} |  ![icon](/img/icons/green_tick.png){style="width: 12px;"} **Versionning**, status and comments.<br />![icon](/img/icons/green_tick.png){style="width: 12px;"} **Preview**: easy playblast or thumbnail<br />![icon](/img/icons/green_tick.png){style="width: 12px;"} **Pipeline**: publish, import, replace, update assets<br />![icon](/img/icons/green_tick.png){style="width: 12px;"} **Pipeline**: Advanced customization using publish and import settings and presets |
| Blender | Python | ![icon](/img/icons/blue_circle.png){style="width: 16px;"} | ![icon](/img/icons/blue_circle.png){style="width: 12px;"} **Versionning**, status and comments.<br /> ![icon](/img/icons/blue_circle.png){style="width: 12px;"} **Preview**: easy playblast or thumbnail<br /> ![icon](/img/icons/orange_diamond.png){style="width: 12px;"} **Pipeline**: publish, import, replace, update assets<br /> ![icon](/img/icons/orange_diamond.png){style="width: 12px;"} **Pipeline**: Advanced customization using publish and import settings and presets |
| Cinema 4D | ? | ![icon](/img/icons/red_circle.png){style="width: 16px;"} | |
| Houdini | ? | ![icon](/img/icons/red_circle.png){style="width: 16px;"} | |
| Inkscape | Python | ![icon](/img/icons/orange_diamond.png){: style="width:16px;"} | Low-priority |
| Krita | Python | ![icon](/img/icons/orange_diamond.png){style="width: 16px;"} | ![icon](/img/icons/orange_diamond.png){style="width: 12px;"} **Versionning**, status and comments.<br /> ![icon](/img/icons/orange_diamond.png){style="width: 12px;"} **Preview**: easy playblast or thumbnail<br /> ![icon](/img/icons/orange_diamond.png){style="width: 12px;"} **Pipeline**: publish, import, replace, update assets<br /> ![icon](/img/icons/orange_diamond.png){style="width: 12px;"} **Pipeline**: Advanced customization using publish and import settings and presets |
| The Foundry Nuke | Python | ![icon](/img/icons/orange_diamond.png){style="width: 16px;"} | ![icon](/img/icons/orange_diamond.png){style="width: 12px;"} **Versionning**, status and comments.<br /> ![icon](/img/icons/orange_diamond.png){style="width: 12px;"} **Preview**: easy playblast or thumbnail<br /> ![icon](/img/icons/orange_diamond.png){style="width: 12px;"} **Pipeline**: publish, import, replace, update assets<br /> ![icon](/img/icons/orange_diamond.png){style="width: 12px;"} **Pipeline**: Advanced customization using publish and import settings and presets |

Legend:  
![icon](/img/icons/green_tick.png){style="width: 16px;"} | Available  
![icon](/img/icons/blue_circle.png){style="width: 16px;"} | In development  
![icon](/img/icons/orange_diamond.png){style="width: 16px;"} | Planned  
![icon](/img/icons/red_circle.png){style="width: 16px;"} | Not supported yet, but you can develop your own add-on and contribute !

## Introduction

Here are some guidelines and descriptions about how these add-ons should work. We're trying to apply the same guidelines to all host applications for a unified experience. These add-ons should use the provided [*Scripting API*](../../dev/add-ons-reference/index.md) available at first in *Python* and *Adobe ExtendsScript*.

Although the add-ons are primarily meant to be used in conjunction with the *Ramses Client Application* (which in turn may be connected to the *Ramses Server*), they can also be used on their own. In which case not all functionnalities will be available; the differences are mentionned in this documentation.

## Available Commands

Some of these commands manage files in the [*Ramses Tree*](../files/tree.md), see the [corresponding documentation](../files/tree.md).

!!! note
    To be written...

## UI / UX

Depending on the host application, the implementation and User Interface / User Experience of the add-ons may vary, but they should follow these guidelines as closely as possible.

### Integrated commands

When possible, the commands should be integrated as seamlessly as possible in the host application, e.g. by adding or even replacing menu items for saving files, saving as., etc.

Especially for the *save* command, the default *save* shortcut (generally `[Ctrl] + [S]`) should be replaced by the *save* command for *Ramses*. Other *save* command shortcuts should be added, preferably the same for all add-ons:

- *Save*: `[Ctrl] + [S]`
- *Save new version*: `[Ctrl] + [Alt] + [S]`
- *Publish*: `[Ctrl] + [Shift] + [P]`

These shortcuts have been chosen to avoid conflicts in most host applications but can be changed to something else if they're still conflicting with something important.

### UI Panel

If possible, each *Ramses Add-on* should provide a UI panel integrated into the host software, but organized similarly in all add-ons, with specific variations to respect the host UI guidelines.

The panel must display buttons for all available commands.

!!! note
    To be improved and completed...

### Settings

If the host application provides a way to display the settings of its add-ons, the settings should be located there (and the *settings* command should open this location if possible). If it does not, the Add-on must provide a dialog or a panel to let the user change the settings.

These settings are, at least:

- Location of the *Ramses Client* executable file (*.exe* on *Windows*, *.app* on *MacOS*, *.appimage* or binary on *Linux*)
- Listening port of the *Ramses Daemon*
- Whether to connect to the daemon or not.
- Ramses data path (the folder containing the files)
- Development options (verbosity/debug mode)

Note: the settings are saved in a shared location (user documents folder ?) between all add-ons so that they're configured only once. They're in an easy-to-edit JSON text file.