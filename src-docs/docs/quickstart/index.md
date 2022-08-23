![META](authors:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2021;updated:2022/08/23)

# Quick start guide

Here's all you need to know before you start using *Ramses*. Follow these few steps and you'll be able to benefit from all it's features very quickly.

## Introduction

*Ramses* can help you [**automate your workflow**](../pipeline/pipeline.md), [**plan**](../pipeline/planning.md) and [**track**](../pipeline/production.md) your production.

- To benefit from all *Ramses* features, you just need to install the [***Ramses Application***](../components/client/index.md) on your workstations.

- To help you manage your files and automate your workflow, you can also install optional [***Ramses Add-ons***](../components/addons/index.md). They can be scripts, extensions, plugins, add-ons for your creative software and how you use them and their features can vary depending on the host software.

- To sync all the *Ramses Data* across workstations and between multiple users, you'll need to connect to an optional [***Ramses Server***](../components/server/index.md). You can either [install your own](../components/server/install.md) or rent a server managed by *RxLaboratory* (or any other provider you trust).


!!! Note
    The *Ramses Application* (and *Add-ons*) can be used without server; all the features will still be available, except for the ability to sync the data across workstations and multiple users at the same time.

No matter what, all these components also expect you to have a least a few predefined folders available in your computer to store some data. That's the [***Ramses Tree***](../components/files/index.md).

Follow the next steps to learn how to prepare your pipeline and computers to work with *Ramses* and all of these components.

## Tips

Although *Ramses* is made to be as simple as possible to use, it's a production tool which both helps and needs you to be organized, and you'll have to familiarize yourself with its organization.

- Read this documentation before installing everything and clicking everywhere! The [*Principles*](../pipeline/index.md) section and the [*Ramses Tree*](../components/files/index.md) section are especially important.
- In [files and folders](../components/files/index.md), it is important to respect the [*Ramses* naming scheme](../components/files/naming.md); take some time to familiarize yourself with it.
- On some systems (*Linux* and sometimes *Mac OS* depending on its settings), the text case matters in file and folder names. Even on *Windows*, it is safer to assume that the name `Tristan` is different than `tristan`.
- *Ramses* is just a piece of software. It's dumb. The folder name `_preview` is not the same as `preview`. Be careful with [naming conventions](../components/files/naming.md) and [folder structure](../components/files/tree.md).

## 1 - Check the Ramses folder, the *Ramses Tree*

In order to work correctly, *Ramses* expects a few folders to be available on your workstation or a shared location. Your administrator should give you the needed information.

- You must have a __*Ramses* main folder__, which stores at least some configuration files. In default configurations, this folder actually stores everything you need : user files, project files, etc. In this case, it should contain at least two subfolders: `Users` and `Projects`. The `Projects` subfolder is the most important one: it is there you'll work and save all your data.
- You may store your __project files in separate folders__, either locally or in a shared location; in this case, for each project, you'll have to know where its folder is. It's in this specific location that you'll work and save your data instead of the *Ramses* main folder.

▹ [__More details about the *Ramses Tree*__](../components/files/index.md).

### 1.A - Project folders

In short, a project folder should contain *at least* these subfolders.

- `00-ADMIN` contains the administration files. *Ramses* doesn't handle these yet.
- `01-PRE-PROD` contains pre-production step files (with a subfolder for each step).
- `02-PROD` contains production step common files (libraries, templates, etc, with a subfolder for each step), but not the working files.
- `03-POST-PROD` contains post-production step files (with a subfolder for each step.
- `04-ASSETS` contains asset working files (with a subfolder for each group of assets).
- `05-SHOTS` contains shot working files (with a subfolder for each shot).
- `06-EXPORTS` contains media files for the whole project.

Sorting your files this way enables a lot of features in *Ramses* (thumbnails in the UI, direct access to the files from the *Ramse Client Application*, automatic time tracking, etc.). Read the details about [naming conventions](../components/files/naming.md) and the [folder structure](../components/files/tree.md) to know more.

▹ [__More details about the file and folder structure__](../components/files/index.md).

## 2 - The Ramses Application

The [*Ramses Client Application*](../components/client/index.md) is the main User Interface of *Ramses*.

### 2.A - Installation

[Installing the application](../components/client/install.md) is standard and very simple, depending on your system. Instructions are available in the [documentation](../components/client/install.md) here.

Before starting to use the [*Ramses Client Application*](../components/client/index.md) there are a few steps to configure it correctly.

### 2.B - Settings

### 2.C - Create a database



### 2.D - Project settings

If you're joining an existing project, and your administrator gave you a specific path for its data, you can go to the ![](/img/icons/cinema-movie-settings_bl.svg){: style="width:18px;"} *project* panel to set its location.

![](/img/client/project.png)

By default, the project is located inside the *Ramses* main folder as defined in the previous step, but you can override it in this panel.

▹ [__Project settings are detailed here__](../components/client/project.md).

## 3 - Install the *Ramses Add-Ons*

Finally, you can install the [*Ramses Add-ons*](../components/addons/index.md) you need. Installation differs depending on the host application (*Maya*, *Blender*, *After Effects*...), [read the doc](../components/addons/index.md) for more information.