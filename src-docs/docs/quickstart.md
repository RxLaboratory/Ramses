![META](authors:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2021;updated:2021/07/20)

# Quick start guide

Here's all you need to know before you start using *Ramses*. Follow these few steps and you'll be able to benefit from all it's features very quickly.

## Introduction

*Ramses* can help you [**automate your workflow**](pipeline/pipeline.md), [**track**](pipeline/production.md) and [**plan**](pipeline/planning.md) your production. For this, it's made of several different [**components**](components/index.md), which can work both hand in hand or independently:

- the [***Ramses Server***](components/server/index.md) shares the data between several workstations, on a local network or through the internet, if you're working with a small or big team.
- the [***Ramses Client Application***](components/client/index.md) is the heart of *Ramses* on each workstation. It's the main *Ramses* interface, connects to the server, and receive information from the *Ramses Add-Ons*
- the [***Ramses Add-ons***](components/addons/index.md) can be scripts, extensions, plugins, add-ons for your creative software and handle the versionning and publishing of your files, transmitting some information to the *Ramses Client Application*.

*Ramses* is modular; that means you don't need to use all components together: it's not mandatory to have access or install a *Ramses Server*, and the *Add-ons* can work independently.

No matter what, all these components also expect you to have a least a few predefined folders available in your computer to store some data. That's the [***Ramses Tree***](components/files/index.md).

Follow the next steps to learn how to prepare your pipeline and computers to work with *Ramses* and all of these components.

!!! Warning
    Although in the future the *Ramses Client* will be able to be used as a stand-alone, offline application, **the currently available *Alpha or Beta Versions* absolutely need a connection to a _Ramses Server_** (there's no offline storage of the data for now). That's temporary.

!!! Note
    For now, the only *Add-on* available is the one for *Autodesk Maya*, but others will follow soon (for *Blender*, *After Effects*, *Nuke*...)

    *Ramses* also includes several [*APIs*](dev/index.md) to let you extend it and develop your own *Add-ons*.

## Tips

Although *Ramses* is made to be as simple as possible to use, it's a production tool which both helps and needs you to be organized, and you'll have to familiarize yourself with its organization.

- Read this documentation before installing everything and clicking everywhere! The [*Principles*](pipeline/index.md) section and the [*Ramses Tree*](components/files/index.md) section are especially important.
- In [files and folders](components/files/index.md), it is important to respect the [*Ramses* naming scheme](components/files/naming.md); take some time to familiarize yourself with it.
- On some systems (*Linux* and sometimes *Mac OS* depending on its settings), the case matters in file and folder names. Even on *Windows*, it is safer to assume that the name `Tristan` is different than `tristan`.
- *Ramses* is just a piece of software. It's dumb. The folder name `_preview` is not the same as `preview`. Be careful with [naming conventions](components/files/naming.md) and [folder structure](components/files/tree.md).

## 1 - Prepare the folder structure, the *Ramses Tree*

In order to work correctly, *Ramses* expects a few folders to be available on the workstations; depending on what you plan to do with *Ramses* you don't necessarily all of them, and they don't necessarily need to be shared with all workstations.

### 1.A - A main *Ramses* folder

*Ramses* needs a main folder available on each computer; by default, it expects to find it in your documents folder.

The *Ramses* main folder default path is:

- *Linux*: `/home/user/Ramses`
- *Mac OS*: `/users/user/Ramses`
- *Windows*: `C:\users\user\Ramses`

This path can be customized in the [*Ramses Client Application*](components/client/index.md) or the [*Ramses Add-ons*](components/addons/index.md) settings.

This folder contains *at least*:

- A `Config` subfolder which contains some settings file *Ramses* needs to work correctly.
- A `Projects` subfolder which *may* contain the files for your projects. This is where *Ramses* expects to find your production files *by default*, but the location of each project can be customized.
- A `Users` subfolder which contains user specific files. As for projects, each user path can be customized if needed.

__Before using *Ramses*, you need to choose where this main folder will be located__.

There are several possibilities:

- When using *Ramses* on a **single workstation**, the location of the folder does not really matters, you can create it anywhere you'd like
- If you want to **share your files** between different workstations, there are two recommended ways:
    - Use a **synchronization** tool (like [*Syncthing*](https://syncthing.net/), *Dropbox*, *Synology Drive*, etc.). In this case, you can locate the *Ramses* main folder anywhere, and add it to the synchronization.
    - Use a **shared location** like a *network drive* on *Windows*, a *Samba* server, a *FTP* server, etc. In this case, the *Ramses* main folder can be located on this shared location.

You could also syncrhonize the files manually; if you just send and receive your files manually, you can still use *Ramses* to manage them: just make sure everyone stores them in the same folder structure, inside a project folder managed by *Ramses*. Read below to learn more about project folders.

▹ [__More details about the *Ramses Tree*__](components/files/index.md).

!!! hint
    The recommended way to share files between workstations when using *Ramses* is [*Syncthing*](https://syncthing.net/): it is a free, decentralized and secured way to sync files between as many nodes as you need, and works on all systems. On bigger productions, if you need a central server too, just install it on the server or a computer with an access to the *LAN*/*SAN* location.

    This way, the data is automatically replicated on all workstations, which makes it safe and reliable. Having instant access to the data locally improves a lot the performance.

    *Ramses* will include *Syncthing* in future versions, to help you automatically setup your environment.

### 1.B - Project folders

By default, *Ramses* expects the project folders to be located into the `Projects` subfolder of the main *Ramses* folder, but **you can customize the path independently for each project**, for example to locate them in different shared locations or synchronized folders.

__To change the path for a specific project, you need to use the [*Ramses Client Application*](components/client/index.md)__.

But you can already prepare the path(s) for the project(s) you're going to track with *Ramses*.

You don't necessarily need to create any subfolder inside the project folder, as *Ramses* will create them automatically *when it needs them*. But if you're starting to use *Ramses* for a project you've already started, you may need to manually rename and sort the existing files; in this case, you need to follow the *Ramses* specifications for naming and the folder structure, which are explained [here](components/files/index.md).

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

!!! warning
    In the current *Alpha* and *Beta* testing versions of *Ramses*, this setting is shared with all connected clients, so customized paths for the projects *must* be the same on all workstations.

    This will change in future versions.
    
    Note that it's not the case for the *Ramses* main folder, which can be different on each workstation.

## 2 - Prepare the *Ramses Server*

If you're going to share data between different workstations and users, you'll need access to a [***Ramses Server***](components/server/index.md) which can be on a local network or on the internet.

▹ If you already have a server available (or don't need it), you can go to the next step *3 - The Ramses Client Application*.

!!! Warning
    Although in the future the *Ramses Client* will be able to be used as a stand-alone, offline application, **the currently available *Alpha or Beta Versions* absolutely need a connection to a _Ramses Server_** (there's no offline storage of the data for now). That's temporary.

The *Ramses Server* is a very standard *PHP + MySQL* web server and its [installation](components/server/install.md) is just a matter of creating an empty database and copying a bunch of files, if you already have access to a web server.

The easy and versatile way to use a *Ramses Server* is to order a simple shared hosting from any provider (the *Ramses Server* will work on most basic servers as long as *PHP* and a *MySQL*-compatible database are available) and install it there as any simple website.

But if you'd prefer to install *Ramses* on a computer on your local network, you'll first need to install a standard web server with *PHP* and *MySQL*, like what's called an *AMP* stack (*AMP* stands for *Apache* - that's the web server software - *PHP* - a programming language - *MySQL* - a database software). This is pretty common and can be done on all systems (*Linux*, *Mac OS*, *Windows*), and we provide [step-by-step instructions](components/server/web-server.md) to help you in the [server documentation](components/server/index.md).

To connect to the server, you'll just need its *URL*, which will probably look like `http(s)://your-server.com/ramses` or could be `http://localhost/ramses` if you've installed it locally, or maybe something like `http://192.168.1.150/ramses` if it's on a local network for example.

▹ [__Read the documentation of the *Ramses Server*__](components/server/index.md).

## 3 - The Ramses Client Application

The [*Ramses Client Application*](components/client/index.md) is the main User Interface of *Ramses* and should be installed on all workstations. It gathers the shared data from a [*Ramses Server*](components/server/index.md) and communicates with the [*Ramses Add-ons*](components/addons/index.md). It also handles the [folder structure](components/files/index.md) and tracks your files and versions.

!!! Warning
    Although in the future the *Ramses Client* will be able to be used as a stand-alone, offline application, **the currently available *Alpha or Beta Versions* absolutely need a connection to a _Ramses Server_** (there's no offline storage of the data for now). That's temporary.

### 3.A - Installation

[Installing the application](components/client/install.md) is standard and very simple, depending on your system. Instructions are available in the [documentation](components/client/install.md) here.

Before starting to use the [*Ramses Client Application*](components/client/index.md) there are a few steps to configure it correctly.

### 3.B - Settings

Open the application, and go to the settings with the ![](/img/icons/gotosettings.svg){: style="width:18px;"} settings button at the bottom right of the application.

Start with the ![](/img/icons/storage_sl.svg){: style="width:18px;"} *Ramses Data* tab.

![](/img/client/datasettings.png)

In this tab, set the __*Ramses* main folder__, the one you chose at the step *1-A* above.

Next, switch to the ![](/img/icons/server-settings.svg){: style="width:18px;"} Server tab.

![](/img/client/serversettings.png)

In this tab, ***Server address*** must be the complete *URL* to the server as defined at the step *2* above or given by your administrator.

Check the ***Secure connexion*** box in case you want to encrypt the data sent to and recieved from the server. Your server must be compatible with *SSL* (that's standard) and have a valid certificate (not all providers provide them...).

▹ [__All settings are explained here__](components/client/settings.md).

That's all, now you can get back to the *log in* panel and... log in. If you've just installed the *Ramses Server*, the default user should be `Admin` with the password `password`. Otherwise, your administrator should've given you your username and password.

### 3.C - Update the Admin account

▹ If you're not the administrator who've just installed the server, you can go to the next step *3.D - Project settings*.

In case you've just installed the server, start by changing the *Admin* name and password: go to the ![](/img/icons/admin.svg){: style="width:18px;"} `Admin ► Users` ![](/img/icons/users_bl.svg){: style="width:18px;"} tab.

![](/img/client/user_management.png)

Select the *Administrator* user, change its ***name*** and ***ID***. Type `password` in the ***Current password*** field, and enter a ***New password*** *twice* then click on ***Change password***.

▹ [__User management is explained here__](components/client/users.md).

### 3.D - Project settings

If you're joining an existing project, and if you've decided at the step *1-B* that the project folder should *not* be located inside the default *Ramses* folder, check the folder in the ![](/img/icons/cinema-movie-settings_bl.svg){: style="width:18px;"} *project* panel

![](/img/client/project.png)

There, you can check (and change if needed) the ***Folder*** location of the project. If you want to revert back to the default location inside the *Ramses* main folder, leave the field empty.

!!! warning
    In the current *Alpha* or *Beta* version of *Ramses*, this setting is shared by **all** users (unlike the *Ramses* main folder location which can be changed individually), so **all** users **must** have the folder available at the exact same path on all computers. This is *not* the case when using the default *Ramses* main folder.

    Also, to access this setting, the user need have either the [*Administrator* or *Project Administrator* role](pipeline/administration.md).

    This is temporary and will change in future versions of *Ramses*.

▹ [__Project settings are detailed here__](components/client/project.md).

### 3.E - Folders, file access and thumbnails

To access or even create folders to store your files and be sure *Ramses* finds them, it is easier to use *Ramses* than create them manually.

From the ![](/img/icons/assets_sl.svg){: style="width:18px;"} *Assets* or ![](/img/icons/shots.svg){: style="width:18px;"} *Shots* panels, clicking on any ![](/img/icons/explore.svg){: style="width:18px;"} ***folder*** button opens (and creates if needed) the corresponding folder in your file explorer.

![](/img/client/assettracking.png)

When changing the size of the rows, more details can be shown for each task, including a preview image.  
The image is pulled from the `_preview` subfolder of the shot or the asset folder which you can open with the ![](/img/icons/go-to-explore-reveal-folder_sl.svg){: style="width:18px;"} *folder* button on the top right corner of the cell. *Ramses* will get the first *PNG* or *JPG* [correctly named](components/files/naming.md) image from the folder, or any other image if a correct one is not found.

Each cell contains information about a task (the combination of an asset or shot and a step).

![](/img/client/assetstatus.png)

## 4 - Install the *Ramses Add-Ons*

Finally, you can install the [*Ramses Add-ons*](components/addons/index.md) you need. Installation differs depending on the host application (*Maya*, *Blender*, *After Effects*...), [read the doc](components/addons/index.md) for more information.