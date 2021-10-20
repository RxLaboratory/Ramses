![META](authors:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2021;updated:2021/07/12)

# Ramses Changelog

We're listing here all changes for all versions of the whole *Ramses* framework. To make things simpler, **all components always share the same version**, all are updated if one is updated. All the components you're using should always be in the same version.

For each version, there's also a list of **known issues**. To read more details about these issues, check our [tracking on *Github*](https://github.com/RxLaboratory/Ramses/labels/bug).

!!! note
    Versions ending with *`-Alpha`* or *`-Beta`* are released for testing purposes only and should not be used in production, unless you know what you're doing.

    In versions labeled *`Alpha`*, the [*APIs*](dev/index.md) may still vary a lot, especially for versions still in `0.x`. Changes are mentionned in this changelog.

## Planned Features

The detailed list of planned features and enhancements is [available on *Github*](https://github.com/RxLaboratory/Ramses/labels/enhancement).

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

## 0.2.0-Alpha (In Development)

### Improvements

- **All**: Added custom setttings in the pipe files.
- **Client**: Name and ID are now validated before any change to make sure they fit the Ramses naming scheme.
- **Client and server**: Added colors associated to users, shown in the schedule and item tables when the user is assigned to an item.
- **Client**: Added User and State filters in the production tables.
- **Client**: Added an option to show or hide all details in the tables.
- **Client**: Added the ability to copy and paste comments from the tables.
- **Client**: Added versionning for published files.
- **Client**: Better feedback in case a change is not accepted by the server.
- **Client**: Better UI feedback when creating/editing users.
- **Client**: Added menus and shortcuts in administration panels.
- **Client**: The pipeline layout is now saved per project too (and not only per user), with the ability to manually save and reload the default project layout.
- **Client**: The project folder is now a local setting; you can push the setting to the server to share it with everyone.
- **Client**: When offline, the client does not anymore try (and fail) to refresh its data from the server.
- **Client**: Major performance improvement of the UI.
- **Client**: Lots of UI tweaks.
- **Client**: Added a welcome page to help you adjust the most important *Ramses* settings on first launch.
- **Client**: *NO* state in tables is now greyed out.
- **Add-ons**: A comment is added to a file when it's restored from a previous version.
- **Add-ons**: Added a *debug mode* for developpers.
- **Add-ons**: For *Python* addons (all except *Adobe*), multi-threading makes long operations - like publishing or restoring files - non-blocking and much more performant.
- **Maya Add-on**: Updated to use the new version of the *Python API*, improves stability and performance.
- **Maya Add-on**: Handle the new versionning system for published files.
- **Maya Add-on**: Filter shots by sequence in the open/import dialog.
- **Maya Add-on**: New *Debug mode* setting for developpers.
- **Maya Add-on - Rubika Flavor**: Improved stability and fixed a bunch of bugs.
- **Maya Add-on - Rubika Flavor**: Improved publication for all pipes:
    - Ability to export both to Alembic and Maya at once.
    - Ability to always choose between *ASCII* or *Binary*.
    - Ability to export animated geometry.
    - Ability to publish/import as reference most of the pipes
- **Maya Add-on - Rubika Flavor**: Improved updater:
    - Filter selected nodes only
    - Revert to an older version
    - Keep PRS offsets
- **Server**: Name and ID are now validated before any change to make sure they fit the Ramses naming scheme.
- **Server**: Improved security with database encryption for sensible data (user personnal information and roles).

### Changes to the scripting API

- Added versionning for published files.
- Added the new `RamFileInfo` class to help handling file names.
- Improved the `RamFileManager` class and added a few new methods. Be careful, some method names have changed, and some methods were removed/moved to `RamFileInfo`, especially methods concerning file publication.
    - Removed or moved to `RamFileInfo`: `isRestoredFilePath`, `composeRamsesFileName`, `buildRamsesFileName`, `decomposeRamsesFilePath`, `isRamsesName`, `decomposeRamsesFileName`.
    - `RamFileManager.getLatestVersion` is now `getLatestVersionInfo` and returns a `RamFileInfo` instance.
    - `RamFileManager.getPublishPath` is now `getPublishInfo` and returns a `RamFileInfo` instance.
    - `RamFileManager.copyToPublish` is renamed `RamFileManager.publishFile`.
    - Copy operations for files is now multi-threaded. That improves a lot the performance of your scripts, but that means the script continues while the new file doesn't necessarilly exist yet, be careful!
- Updated `RamItem` to use the new versionning system for published files.
    - `RamItem.publishFilePaths(resource, step)` is replaced by `RamItem.publishedVersionFolderPaths(step)` which returns a list of folder paths (sorted from oldest to latest).
    - `RamItem.isPublished(resource, step)` no longer have the `resource` argument.
- Updated `RamStep` to use the new versionning system for published files.
    - `RamStep.templatesPublishFilePaths` is replaced by `RamStep.templatesPublishedVersionFolderPaths` which now returns a list of folders.
- `Ramses` callbacks for publishing, importing, changing status have changed a bit.
- Files are now copied using a separate thread whenever possible. This improves a lot the performance. Call `RamFileManager.waitFiles()` if your script needs the files to be actually written to continue.

### Fixed issues

- **Client**: Available projects are only the projects the user has been assigned to.
- **Client**: When clicking the "network settings" button from the login panel, you're now correctly redirected to the right network settings tab.
- **Client**: Fixed an wrong error when trying to log in with an unknown user name.
- **Client**: Crash when removing an asset group still containing assets.
- **Client**: Fixed an issue where shot/asset attribution would be reinitialized on edition from the production table.
- **Client**: Fixed the size of the dropdown menus sometimes being too small for long sequence/asset group names.
- **Maya Add-on**: The "preview" function now works correctly.
- **Maya Add-on (Rubika Flavor)**: Import Geo now works correctly.

### Known Issues

#### Server

-  ***[#107](https://github.com/RxLaboratory/Ramses/issues/107)***: Table prefixes can't be changed.

#### Client and Daemon

-  ***[#137](https://github.com/RxLaboratory/Ramses/issues/107)***: The *SSL* option should be disabled if *openSSL* is not available on the system.
-  ***[#11](https://github.com/RxLaboratory/Ramses/issues/11)***: The client should check if it's connected to a server with the same version.
-  ***[#151](https://github.com/RxLaboratory/Ramses/issues/151)***: The client should switch to offline mode when the server session token becomes invalid (server timeout).
- On ***Windows***, the tray icon may not be removed if the client crashes.
- On ***Mac OS***, the user specific settings (current project, location of the nodes in the pipeline editor, ...) may not be saved.

## 0.1.3-Alpha

Bugfixes.

### Fixed issues

- **Client**: Loading of the projects taking a very long time has been fixed. Projects now load in an instant.
- **Client**: Linux .deb packaged fixed (the doc is now in its right folder).
- **Server and client**: Difficulty and estimations for shots are now correctly set.

## 0.1.2-Alpha

Bugfixes and minor improvements.

### Improvements

- **Client**: Improved feedback when editing items.
- **Python API** and **Maya Add-on**: Improved comments when saving file (includes the version number when saving a restored version).

### Fixed issues

- **Client**: Menus, Context menus and clicks now work correctly on the Schedule Manager when there's no project selected at startup.
- **Client**: Minor code refactor to remove some warnings on *Qt 5.15* build.
- **Server**: Fixed an issue where users could not change their own password.
- **Maya Add-on - Rubika Flavor**: Deformers and shaders are now correctly reset on geometry update.

## 0.1.1-Alpha

Initial release.
