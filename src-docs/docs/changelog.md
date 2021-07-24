![META](authors:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2021;updated:2021/07/12)

# Ramses Changelog

We're listing here all changes for all versions of the whole *Ramses* framework. To make things simpler, **all components always share the same version**, all are updated if one is updated. All the components you're using should always be in the same version.

For each version, there's also a list of **known issues**. To read more details about these issues, check our [tracking on *Github*](https://github.com/RxLaboratory/Ramses/labels/bug).

!!! note
    Versions ending with *`-Alpha`* or *`-Beta`* are released for testing purposes only and should not be used in production, unless you know what you're doing.

    In versions labeled *`Alpha`*, the [*APIs*](dev/index.md) may still vary a lot, especially for versions still in `0.x`.

## Planned Features

The detailed list of planned features and enhancements is [available on *Github*](https://github.com/RxLaboratory/Ramses/labels/enhancement).

- **Full _SSL_ encryption** when connecting to the server. *SSL* may already be available on *Linux* and will be available on *Windows* and *Mac OS* shortly. If *Ramses*' beta versions can already be tested without it, it is a mandatory feature for the release versions, as the security of your data is non-negociable.
- The **Dashboard** will be an area where each user accesses all its important data and schedule. It comes with notifications so you won't miss any change concerning your assets, shots and steps.
- The **Offline mode** for the *Ramses Client Application* will make it easier to use for one-man bands; you will be able to use *Ramses* without having to install the server. This also means you'll still be able to use *Ramses* even if you're not connected to the network, when you're in a train or on top of a mountain.
- **Project Planning**: a simple but indispensable tool to easily plan the production of the project, at the very beginning of the production. With this planning tool you will be able to set the important dates of the production, the deadlines for each step, all in an easy-to-use and visual panel.
- **Status automation**: when a character is rigged, it means it's ready to be animated. Status automation will automatically update the states of all the items according to the pipeline design, and your custom rules.
- **Data export and import**: if you're planning important software update, or for your backups at the end of a production, it may be useful to export and re-import your data.
- **File tools** will provide automatic ways to manage your files, create new assets and shots from already existing files, clean the tree, backup, remove temporary data, batch rename files and folders...
- **Video Editing tools** to manage your shots from your edits. Automatically set shot duration, order and other metadata by just importing your edit from a *Final Cut XML*, *EDL* or *CSV* file.
- A lot of **UI improvements** and tweaks are planned too, and work will be done to improve the performance of the *Ramses Client Application* even further.
- **Folder monitoring** will provide a way to handle versionning and production tracking even if there's no *Ramses Add-on* available for the application being used. 
- **_Syncthing_ integration** will provide [a free, open, and secure way to synchronize](https://syncthing.net/) all files and data of your production accross multiple workstations, servers, NAS, etc.

## 0.1.2-Alpha (In development)

Bugfixes and minor improvements.

### Improvements

- **Client**: Improved feedback when editing items.

### Fixed issues.

- **Client**: Menus, Context menus and clicks now work correctly on the Schedule Manager when there's no project selected at startup.
- **Client**: Minor code refactor to remove some warnings on *Qt 5.15* build.
- **Server**: Fixed an issue where users could not change their own password.

### Known Issues

#### Server

-  ***[#105](https://github.com/RxLaboratory/Ramses/issues/105)***: `Internal Server Error` when moving steps/changing their order.
-  ***[#107](https://github.com/RxLaboratory/Ramses/issues/107)***: Table prefixes can't be changed.

#### Client and Daemon

-  ***[#137](https://github.com/RxLaboratory/Ramses/issues/107)***: The *SSL* option should be disabled if *openSSL* is not available on the system.
-  ***[#105](https://github.com/RxLaboratory/Ramses/issues/105)***: `Internal Server Error` when moving steps/changing their order.
-  ***[#106](https://github.com/RxLaboratory/Ramses/issues/106)***: Errors in the *Python API* when special characters are sent by the *Daemon*.
-  ***[#110](https://github.com/RxLaboratory/Ramses/issues/110)***: *Ramses* should use a versionning system for published files.
-  ***[#136](https://github.com/RxLaboratory/Ramses/issues/136)***: Sanitation needed to check if names and IDs respect the *Ramses* naming conventions.
-  ***[#138](https://github.com/RxLaboratory/Ramses/issues/138)***: The client should list only projects the user is assigned to.
-  ***[#11](https://github.com/RxLaboratory/Ramses/issues/11)***: The client should check if it's connected to a server with the same version.
-  ***[#151](https://github.com/RxLaboratory/Ramses/issues/151)***: The client should switch to offline mode when the server session token becomes invalid (server timeout).
- On ***Windows***, the tray icon may not be removed if the client crashes.
- On ***Mac OS***, the user specific settings (current project, location of the nodes in the pipeline editor, ...) may not be saved.

#### Python API

-  ***[#106](https://github.com/RxLaboratory/Ramses/issues/106)***: crash when special characters are sent by the *Daemon*.
-  ***[#110](https://github.com/RxLaboratory/Ramses/issues/110)***: *Ramses* should use a versionning system for published files.

#### Add-ons

-  ***[#109](https://github.com/RxLaboratory/Ramses/issues/109)***: *Maya*. The version should be added in the comment when saving restored versions.
-  ***[#110](https://github.com/RxLaboratory/Ramses/issues/110)***: *Ramses* should use a versionning system for published files.
-  ***[#118](https://github.com/RxLaboratory/Ramses/issues/118)***: It seems the *Preview* function does not always work on *Maya 2020*.

## 0.1.1-Alpha

Initial release.
