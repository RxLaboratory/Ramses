![META](authors:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2021;updated:2022/06/28)

# User quick start guide

Here's all you need to know before you start using *Ramses*. Follow these few steps and you'll be able to benefit from all it's features very quickly.

## Introduction

*Ramses* can help you [**automate your workflow**](pipeline/pipeline.md), [**track**](pipeline/production.md) and [**plan**](pipeline/planning.md) your production. For this, it's made of several different [**components**](components/index.md), which can work both hand in hand or independently:

- the [*Ramses Server*](components/server/index.md) shares the *Ramses* metadata between several workstations. Users don't have any direct access to the server, it is managed by your administrator.
- the [***Ramses Client Application***](components/client/index.md) is the heart of *Ramses* on your workstation. It's the main *Ramses* interface.
- the [***Ramses Add-ons***](components/addons/index.md) can be scripts, extensions, plugins, add-ons for your creative software and handle the versionning and publishing of your files.

*Ramses* is modular; that means you don't need to use all components together: it's not mandatory to have access to a *Ramses Server* to use the application, and the *Add-ons* can also work independently.

No matter what, all these components also expect you to have a least a few predefined folders available in your computer or a shared location to store some data. That's the [***Ramses Tree***](components/files/index.md).

Follow the next steps to learn how to use *Ramses* and all of these components.

!!! Warning
    Although in the future the *Ramses Client Application* will be able to be used as a stand-alone, offline application, **the currently available *Alpha or Beta Versions* absolutely need a connection to a _Ramses Server_** (there's no offline storage of the data for now). That's temporary.

!!! Note
    For now, the only official *Add-on* available is the one for [*Autodesk Maya*](components/addons/maya.md), but others will follow soon (for *Blender*, *After Effects*, *Nuke*...)

    As *Ramses* is free and open source, and provides easy-to-use [APIs](../dev/add-ons-reference/index.md) in *Python* and *Adobe ExtendScript*, other users may have developped other unofficial add-ons.

## Tips

Although *Ramses* is made to be as simple as possible to use, it's a production tool which both helps and needs you to be organized, and you'll have to familiarize yourself with its organization.

- Read this documentation before installing everything and clicking everywhere! The [*Principles*](pipeline/index.md) section and the [*Ramses Tree*](components/files/index.md) section are especially important.
- In [files and folders](components/files/index.md), it is important to respect the [*Ramses* naming scheme](components/files/naming.md); take some time to familiarize yourself with it.
- On some systems (*Linux* and sometimes *Mac OS* depending on its settings), the case matters in file and folder names. Even on *Windows*, it is safer to assume that the name `Tristan` is different than `tristan`.
- *Ramses* is just a piece of software. It's dumb. The folder name `_preview` is not the same as `preview`. Be careful with [naming conventions](components/files/naming.md) and [folder structure](components/files/tree.md).

## 1 - Check the Ramses folder, the *Ramses Tree*

In order to work correctly, *Ramses* expects a few folders to be available on your workstation or a shared location. Your administrator should give you the needed information.

- You must have a __*Ramses* main folder__, which stores at least some configuration files. In default configurations, this folder actually stores everything you need : user files, project files, etc. In this case, it should contain at least two subfolders: `Users` and `Projects`. The `Projects` subfolder is the most important one: it is there you'll work and save all your data.
- You may store your __project files in separate folders__, either locally or in a shared location; in this case, for each project, you'll have to know where its folder is. It's in this specific location that you'll work and save your data instead of the *Ramses* main folder.

▹ [__More details about the *Ramses Tree*__](components/files/index.md).

### 1.A - Project folders

In short, a project folder should contain *at least* these subfolders.

- `00-ADMIN` contains the administration files. *Ramses* doesn't handle these yet.
- `01-PRE-PROD` contains pre-production step files (with a subfolder for each step).
- `02-PROD` contains production step common files (libraries, templates, etc, with a subfolder for each step), but not the working files.
- `03-POST-PROD` contains post-production step files (with a subfolder for each step.
- `04-ASSETS` contains asset working files (with a subfolder for each group of assets).
- `05-SHOTS` contains shot working files (with a subfolder for each shot).
- `06-EXPORTS` contains media files for the whole project.

Sorting your files this way enables a lot of features in *Ramses* (thumbnails in the UI, direct access to the files from the *Ramse Client Application*, automatic time tracking, etc.). Read the details about [naming conventions](components/files/naming.md) and the [folder structure](components/files/tree.md) to know more.

▹ [__More details about the file and folder structure__](components/files/index.md).

## 2 - The Ramses Client Application

The [*Ramses Client Application*](components/client/index.md) is the main User Interface of *Ramses*.

!!! Warning
    Although in the future the *Ramses Client* will be able to be used as a stand-alone, offline application, **the currently available *Alpha or Beta Versions* absolutely need a connection to a _Ramses Server_** (there's no offline storage of the data for now). That's temporary.

### 3.A - Installation

[Installing the application](components/client/install.md) is standard and very simple, depending on your system. Instructions are available in the [documentation](components/client/install.md) here.

Before starting to use the [*Ramses Client Application*](components/client/index.md) there are a few steps to configure it correctly.

### 3.B - Settings

The first time the *Client* is run, it shows the following welcome screen, asking for mandatory settings.

![](../img/client/welcome-screen.png)

At any time, you can also go to the settings with the ![](/img/icons/gotosettings.svg){: style="width:18px;"} settings button at the bottom right of the application.

- ![](/img/icons/storage_sl.svg){: style="width:18px;"} Local Data

The first mandatory setting to set is the location of the *Ramses* main folder, where *Ramses* will store all local data by default.

The __*Ramses* main folder__ location should be given by your admnistrator. Be careful, it *not* a specific *project* folder; it is a folder which should contain at least one of these subfolders: `Config`, `Projects`, `Users`.

- ![](/img/icons/server-settings.svg){: style="width:18px;"} Server

The ***Server address*** must be the complete *URL* to the server as given by your administrator. It should look like one of these: `ramses.rxlab.io/instance`, `localhost/ramses`, `192.168.1.5/ramses`, `your-server.com/ramses`...

The ***Secure connexion*** box should be checked by default. Only with some servers or local installation it may be unchecked.

That's all, now you can log in, using your user name and password as given by your administrator.

▹ [__All other settings are explained here__](components/client/settings.md).

### 3.C - Project settings

If you're joining an existing project, and your administrator gave you a specific path for its data, you can go to the ![](/img/icons/cinema-movie-settings_bl.svg){: style="width:18px;"} *project* panel tp set its location.

![](/img/client/project.png)

By default, the project is located inside the *Ramses* main folder as defined in step *3.B* but you can override it in this panel.

▹ [__Project settings are detailed here__](components/client/project.md).

### 3.D - Folders, file access and thumbnails

To access or even create folders to store your files and be sure *Ramses* finds them, it is easier to use *Ramses* than create them manually.

From the ![](/img/icons/assets_sl.svg){: style="width:18px;"} *Assets* or ![](/img/icons/shots.svg){: style="width:18px;"} *Shots* panels, clicking on any ![](/img/icons/explore.svg){: style="width:18px;"} ***folder*** button opens (and creates if needed) the corresponding folder in your file explorer.

![](/img/client/assettracking.png)

When changing the size of the rows, more details can be shown for each task, including a preview image.  
The image is pulled from the `_preview` subfolder of the shot or the asset folder which you can open with the ![](/img/icons/go-to-explore-reveal-folder_sl.svg){: style="width:18px;"} *folder* button on the top right corner of the cell. *Ramses* will get the first *PNG* or *JPG* [correctly named](components/files/naming.md) image from the folder, or any other image if a correct one is not found.

Each cell contains information about a task (the combination of an asset or shot and a step).

![](/img/client/assetstatus.png)

## 4 - Install the *Ramses Add-Ons*

Finally, you can install the [*Ramses Add-ons*](components/addons/index.md) you need. Installation differs depending on the host application (*Maya*, *Blender*, *After Effects*...), [read the doc](components/addons/index.md) for more information.