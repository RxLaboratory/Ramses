![META](authors:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2021-2022;updated:2022/02/07)

# ![](/img/icons/gotosettings.svg){: style="width:55px;"} Application settings and overview

<figure>
  <img src="/img/client/home.png"/>
  <figcaption>The home screen, shown when starting the application.</figcaption>
</figure>

When running the application for the first time, you need to go to the settings first to configure it.

## Overview

### General shortcuts and behaviour

In all the application, a few shortcuts are always the same, in addition to your system's usual shortcuts for *copy*, *paste*, *undo* etc.

- `[Middle Click]`: scroll/pan in lists, tables, timeline...
- `[Ctrl] + [Middle Click]` or `[Mouse Wheel]`: zoom in or out in views which can be zoomed.
- `[Shift] + [A]`: add/create something.
- `[Del]` or `[Shift] + [X]`: remove/delete selection.

All side panels (docks) can be moved around and docked to the sides or undocked by clicking on their titles.

Menu bars can also be moved to all sides of the application.

### Title bar

![](/img/client/titlebar.png)

The title bar show the main buttons to switch the panels on the left, and the visibility buttons on the right. Click and drag an empty space of the title bar to move the whole window.

![](/img/client/projectselector.png)

In the middle of the title bar is the selector where you can select the project you're currently working on.

The buttons on the right section change the size and visibility of the window.

- ![](/img/icons/minimize.svg){: style="width:18px;"} Minimize
- ![](/img/icons/maximize.svg){: style="width:18px;"} Maximize
- ![](/img/icons/hide_sd.svg){: style="width:18px;"} Hide. The application will not close but sit in the system tray.
- ![](/img/icons/quit.svg){: style="width:18px;"} Quit

Once logged in, the left section shows the buttons for the different panels.

![](/img/client/loggedintitlebar.png)

Which panels and buttons are available depends on [the **role** of the current user](../../pipeline/administration.md), and there may be fewer buttons than on the picture above.

- ![](/img/icons/admin.svg){: style="width:18px;"} [Administration](admin.md) panel (*Administrators* only).
- ![](/img/icons/cinema-movie-settings_bl.svg){: style="width:18px;"} [Project settings](project.md) panel (*Project administrators* and *Administrators* only).
- ![](/img/icons/edit-connections-pipeline_bl.svg){: style="width:18px;"} [Pipeline editor](pipeline.md) panel (*Project administrators* and *Administrators* only).
- ![](/img/icons/assets_sl.svg){: style="width:18px;"} [Assets table](production.md) panel.
- ![](/img/icons/shots.svg){: style="width:18px;"} [Shots table](production.md) panel.
- ![](/img/icons/calendar_sl.svg){: style="width:18px;"} [Schedule](schedule.md) panel.
- ![](/img/icons/counter_bl.svg){: style="width:18px;"} Statistics panel.

### Status bar

![](/img/client/statusbar.png)

The status bar at the bottom of the window displays temporary feedback, useful messages and help on its left section. On the right section, there are a few secondary buttons.

- ![](/img/icons/reload_sl.svg){: style="width:18px;"} The *reload* button updates all data from the server. You should not need to click this button too often as the data is automatically and regularly pulled from the server.
- ![](/img/icons/bash-terminal_sl.svg){: style="width:18px;"} The *console* button shows the console panel (see the next section) where *Ramses* logs useful information for debugging or for developpers. Errors and warnings are also logged there.
- ![](/img/icons/warning_bl.svg){: style="width:18px;"} | ![](/img/icons/critical_bl.svg){: style="width:18px;"} A warning or an error button may be shown next, when there are some issues. Click on the button to show the latest logs and learn what's wrong; a *clear* button is available on the popup to remove the warning.
- The ***online***/***offline*** button can be used to switch between online and offline mode. Going online may need to input your password again.
- The ***user*** button (which shows your user name, or *Guest* if you're not logged in yet) gives access to a few user options (see below).
- ![](/img/icons/gotosettings.svg){: style="width:18px;"} The *settings* button leads you to the settings of the application.
- ![](/img/icons/help.svg){: style="width:18px;"} The *help* button shows a small menu with useful shortcuts.

#### The help menu

![](/img/client/helpmenu.png)

- ***Help*** leads you to this documentation.
- ***About*** shows a popup window with a few information about *Ramses* and other legal information.
- ***Bug Report*** is a shortcut to the [bug report form](https://github.com/RxLaboratory/Ramses/issues/new/choose), in case you have any feedback to share about the application.
- ***Chat*** is a shortcut to our [chatroom](http://chat.rxlab.info), currently hosted by a *Discord Server*, where you can get support for *Ramses* and have a chat with other users.
- ***About Qt*** shows a popup window with a few information about *Qt*, the free and open source *C++* framework used to develop the application.

#### The user menu

![](/img/client/loggedinstatusbar.png)

- ![](/img/icons/go-to-explore-reveal-folder_sl.svg){: style="width:18px;"} The *User folder* button opens your folder in your file explorer.
- ![](/img/icons/user-settings_sl.svg){: style="width:18px;"} The *Profile* button shows the user profile panel when you can update your information and password.
- ![](/img/icons/logout_sl.svg){: style="width:18px;"} The *Log out* button logs you out and sets the application to offline mode.

### Console

![](/img/client/console.png)

The console panel is where *Ramses* logs useful information for debugging or for developpers. Errors and warnings are also logged there, respectively in red and yellow.

As many other *Ramses* panel, the console is a *docked* panel which can be undocked or docked both on the right and left side of *Ramses* by just clicking its title and moving it.

![](/img/client/undock.png)

### User Profile

Use the *user menu* at the bottom right of the window (the button with your ID) to access your profile settings.

![](/img/client/profile.png)

Except for the password, there's no need to validate any change; changes are saved instantly.

- ***Name*** is your display name and can be changed any time (although it's not really advisable).
- ***ID*** is your unique identifier which is used in the files and folders (see [*Ramses Tree*](../files/index.md)). It can't be changed once your user folder has been created. Read the [*Ramses naming conventions*](../files/naming.md) to learn how *IDs* and names have to be formatted.
- ***Comment*** can be any text associated to your user. It is public.
- The ***Current role*** can never be changed by the user itself, and has to be changed by an administrator.
- To update your password, input your ***Current password*** and repeat your ***New password*** twice. Don't forget to click the ***Change password*** button to validate!
- You can change the ***folder*** where your personnal files are stored.
    - Leave the field empty to set back the default folder.
    - The blue ![](/img/icons/set-folder_sl.svg){: style="width:18px;"} button opens a folder selector to change the folder location.
    - The green ![](/img/icons/go-to-explore-reveal-folder_sl.svg){: style="width:18px;"} button opens the folder in your file explorer.

## ![](/img/icons/gotosettings.svg){: style="width:48px;"} Settings

To open the settings panel, click on the ![](/img/icons/gotosettings.svg){: style="width:18px;"} settings button at the bottom right corner of the window.

!!! Note
    These settings are local settings, and will not be shared with other workstations. Each workstation, (actually, each session on the system) has its own settings.

The purple ![](/img/icons/reinit.svg){: style="width:18px;"} button will restore all default settings.

Use the blue ![](/img/icons/close.svg){: style="width:18px;"} close button to close the settings. They are automatically saved.

### ![](/img/icons/color.svg){: style="width:32px;"} Appearance

![](/img/client/appearancesettings.png)

The first tab shows the appearance settings; you can change these as you wish.

### ![](/img/icons/storage_sl.svg){: style="width:32px;"} Ramses Data

![](/img/client/datasettings.png)

The second tab defines how *Ramses* will store the data on the filesystem, and has a few display option for the data.

- ***Ramses main folder*** defines the location of the main *Ramses* root folder, the one which will contain the `Config`, `Projects` and `Users` subfolders, as defined by the [*Ramses Tree*](../files/index.md).  
    By default, this folder is located in your user folder.

The blue ![](/img/icons/set-folder_sl.svg){: style="width:18px;"} button opens a folder selector to change the folder location.

The green ![](/img/icons/go-to-explore-reveal-folder_sl.svg){: style="width:18px;"} button opens the folder in your file explorer.

!!! Note
    Project folders and user folders can be individually changed later to be outside of this main *Ramses* root folder, for example if you need to locate specific projects in other arbitrary specific folders. But *Ramses* still needs this main folder to be defined, at least to store some configuration files.

- ***Date format*** defines how dates will be displayed in the user interface. It does not change anything about how the data is actually stored in the database and files.

- ***Time format*** defines how times will be displayed in the user interface. It does not change anything about how the data is actually stored in the database and files.

### ![](/img/icons/server-settings.svg){: style="width:32px;"} Server

![](/img/client/serversettings.png)

The third tab configures how the application will connect to the server.

- ***Server address*** must be the complete *URL* to the server.

- ***Secure connexion*** must be checked in case you want to encrypt the data sent to and recieved from the server. Your server must be compatible with *SSL* and have a valid certificate.

- ***Update every*** *X minutes* defines how often the application will automatically update all its data from the server. In order to keep the application very lightweight and the server very light and easy to install on all kind of computers, the local data is not updated in realtime (there's no pushing from the server), but the application will regularly pull changes from the server. This value does not need to be too low, as *Ramses* does not really need to be perfectly up-to-date every second. __The default value is *2 minutes*__.

- ***Server timeout*** defines how long the application will wait for the server response when getting or posting data. If this timeout expires, the server will be considered not working correctly and the application will switch to offline mode. You may raise a bit this value if your server or your connexion are particularly slow. __The default value is *3 seconds*__.

### ![](/img/icons/daemon.svg){: style="width:32px;"} Daemon

![](/img/client/daemon.png)

The *Daemon* is a background service, a small local server, which is started with the application and is in charge of communications with the [*Ramses Add-ons*](../addons/index.md).

- ***Listenning on port*** defines the local port used by the *Daemon* to receive connections and requests from the [*Ramses Add-ons*](../addons/index.md) and other services. You may change this value if it conflicts with other applications; in this case, you'll have to change it in the settings of all the [*Ramses Add-ons*](../addons/index.md) too. **The default value is _18185_**[^1].

- Click the ***Restart Daemon*** button when changing the listenning port to apply the change.

[^1]:
    This value has been chosen after the birth date of [Karl Marx](https://en.wikipedia.org/wiki/Karl_Marx), 5 May 1818.