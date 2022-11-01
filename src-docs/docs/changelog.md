![META](authors:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2021-2022;updated:2022/11/01)

# Ramses Changelog

## By component

▹ **[Ramses Application](changelog-app.md)**

▹ **[Ramses Maya Add-on](changelog-maya.md)**

▹ **[Ramses Server](changelog-server.md)**

▹ **[Ramses API](changelog-api.md)**

## Versions

***Ramses* uses [semantic versionning](https://semver.org)** to keep track of changes and versions. Versions numbers always have this form: `M.m.p` where `M` is the major version number, `m` is the minor version number and `p` is the patch number.

*[Ramses Components](components/index.md)* should be considered **compatible** together **if and only if they share the same major and minor version numbers**, although if only the minor version number differs everything should still work. That means if you update a component to a new major or minor version, all other components should be updated too.

For example, a *0.6.0* application will work perfectly with a *0.6.2* server, but it may have minor issues with a *0.7.0* server and it will *not* work with a *1.1.0* server.

!!! note
    Versions ending with *`-Alpha`* or *`-Beta`* are released for testing purposes only and should not be used in production, unless you know what you're doing.

    In versions labeled *`Alpha`*, the [*APIs*](dev/index.md) may still vary a lot, especially for versions still in `0.x`. Changes are mentionned in this changelog.

## Planned Features

The detailed list of planned features and enhancements is [available on *Github*](https://github.com/RxLaboratory/Ramses/labels/enhancement).

- The **Dashboard** will be an area where each user accesses all its important data and schedule. It comes with notifications so you won't miss any change concerning your assets, shots and steps.
- **Project Planning**: a simple but indispensable tool to easily plan the production of the project, at the very beginning of the production. With this planning tool you will be able to set the important dates of the production, the deadlines for each step, all in an easy-to-use and visual panel.
- **Status automation**: when a character is rigged, it means it's ready to be animated. Status automation will automatically update the states of all the items according to the pipeline design, and your custom rules.
- **Data export and import**: if you're planning important software update, or for your backups at the end of a production, it may be useful to export and re-import your data.
- **File tools** will provide automatic ways to manage your files, create new assets and shots from already existing files, clean the tree, backup, remove temporary data, batch rename files and folders...
- **Video Editing tools** to manage your shots from your edits. Automatically set shot duration, order and other metadata by just importing your edit from a *Final Cut XML*, *EDL* or *CSV* file.
- A lot of **UI improvements** and tweaks are planned too, and work will be done to improve the performance of the *Ramses Client Application* even further.
- **Folder monitoring** will provide a way to handle versionning and production tracking even if there's no *Ramses Add-on* available for the application being used. 
- **_Syncthing_ integration** will provide [a free, open, and secure way to synchronize](https://syncthing.net/) all files and data of your production accross multiple workstations, servers, NAS, etc.
