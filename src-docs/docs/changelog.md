![META](authors:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2021-2022;updated:2021/01/22)

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

## Known Issues

### Server

-  ***[#107](https://github.com/RxLaboratory/Ramses/issues/107)***: Table prefixes can't be changed.

### Client and Daemon

- ***[#263](https://github.com/RxLaboratory/Ramses/issues/263)***: The client may crash when removing a project which is not empty.
- On ***Windows***, the tray icon may not be removed if the client crashes.
- On ***Mac OS***, the user specific settings (current project, location of the nodes in the pipeline editor, ...) may not be saved.

## 0.2.13-Alpha

### New

- **Client** and **Server**: The schedule now has a dedicated line to add notes/comments.

### Improvements

- **Client** The status of the connexion (offline, connected, secured) is now shown on the status bar. A warning is logged when the connexion is not secured.
- **Maya Add-on**: The *Rubika Flavor* is merged into the official add-on.
- **Maya Add-on**: Added update command
- **Maya Add-on**: Added publish settings
- **Maya Add-on**: Added import and update settings

### Fixes

- **Client**: Username and password are now correctly saved with new servers.
- **Client**: When logged out of the server, the client now shows the reason, and the error if any, before restarting. 

## 0.2.12-Alpha

### Improvements

- **Maya Add-On** and **Maya Add-on Rubika Flavor**: Option to hide HUD when rendering a thumbnail or playplast.

### Fixes

- **Maya Add-On** and **Maya Add-on Rubika Flavor**: Fixed an issue where sometimes the file path would not be correctly in the dialog window updated using the "save as/new scene" command.

## 0.2.11-Alpha

### New

- **Client**: Added a timeline to make it easier to manipulate shots, and to be used with future features...
- **Client** and **Server**: Added sequence color.

### Improvements

- ***Client***: Improved statistics panel.

### Fixes

- **Client** and **Server**: Fixed ordering shots and steps.
- **Client** and **Server**: Fixed changing a schedule item if it had been deleted before.
- **Server**: Steps estimation method is now correctly returned.
- **Server**: States are now correctly updated.

## 0.2.10-Alpha

### Improvements

- **Client**: More details shown in file lists (modification date, and more to come).

### Fixes

- **Client**: Fixed incorrect rights to edit steps and pipes for project administrators.
- **Maya Add-on**: Fixed freezing transform when there are constraints or other links (ignore).
- **Maya Add-on - Rubika Flavor**: Fixed minor publish issue with Sets.
- **Client**: Project edition is now correctly disabled for all users but Administrators.
- **Server**: Fixed an issue where step estimations would not be correctly updated.
- **Maya Add-On**: Fixed an issue where Maya Ascii (.ma) files would contain binary data.
- **Maya Add-On - Rubika Flavor**: When publishing animation, constraints are now correctly kept.

## 0.2.9-Alpha

### New

- **All**: The template versionning system has been changed and improved. Each template must now be stored in its own subfolder correctly named. See the *[Ramses Tree / Folder structure](components/files/tree.md#template-folders)* section for more details.
- **Client**: The UI is more customizable, and the previous window state (location, toolbars, dockers) is restored when starting the application.
- **Maya Add-on - Rubika Flavor**: Added a *replace* method to replace an existing asset/pipe with another one.
- **Python API**: Added `Ramses.replaceScripts` and `Ramses.replaceItem`.
- **Server**: Added logs and fixed security issues.

### Improvements

- **Client**: Important UI tweaks.
- **Maya Add-on**: Improved icons.
- **Client**: The current server address is now displayed in the bottom right corner of the application.
- **Client**: The previous window state and dock locations are now restored at startup.
- **Client**: UI tweaks and minor improvements.
- **Maya Add-on**: UI tweaks and new icons.

### Fixes

- **Client**: Fixed a potential crash when using filters on the tables.

## 0.2.8-Alpha

### Improvements

- **Maya Add-on - Rubika Flavor**: Fixed shading publication issues.

## 0.2.7-Alpha

### Improvements

- **Client**: Administrators can now modify the status history.

### Fixes

- **Client**: Fixed potential crash when searching/filtering tables.
- **Client**: Fixed the status which may not be updated using the right-click context menu.

## 0.2.6-Alpha

### Improvements

- **Client**: When a task is not automatically estimated, the estimation is now named "goal" instead of "estimation". Warning: estimations manually set to 0 day are temporarilly ignored, use 0.0 day instead. This will be fixed later in 0.3.0.
- **Client**: A new *Sort* menu in the tables allows to sort items by name, difficulty, estimation, completion ratio...
- **Client**: The UI has been tweaked to be a bit simpler to use and improve a bit the performance.

### Fixes

- **Client**: The title is now correctly shown on top of the properties panel for statuses/tasks.

## 0.2.5-Alpha

### Fixes

- **Client**: Fixed pipes breaking when re-ordering steps.
- **Client**: UI: Fixed the editor panel too big for its dock.
- **Server**: Fixed error thrown when updating some pipes.
- **Maya Add-on**: Fixed performance issue when using the *saveas* command.
- **Maya Add-on - Rubika Flavor**: Fixed publish shaders not publishing all nodes.
- **Maya Add-on - Rubika Flavor**: Fixed *update* command not listing shader nodes.
- **Maya Add-on - Rubika Flavor**: Fixed publish anim not hiding the progress bar at the end of the process.
- **Python API** and all **Python Add-ons**: Fixed performance issue when loading the list of assets with empty asset groups.

## 0.2.4-Alpha

### Improvements

- **Client**: Several UI improvements.
- **Client**: Performance improvements, especially when loading big projects and in the schedule table.
- **Client**: Keep a server history list in the server settings, to make it easier to switch servers when working with several different servers.
- **Client**: Now checks if an updated version is available; added a donation link.
- **Client** and **Server**: Added version compatibility checks.

### Fixes

- **Client**: Queue server requests to prevent spamming.
- **Server**: Fixed status not correctly updated when the estimation was 0.
- **Maya Add-on - Rubika Flavor**: Fixed publishing geometry and rigs with hidden nodes.
- **Maya Add-on**: Fixed publishing with invalid or missing references.

## 0.2.2-Alpha

### Improvements

- **Client**: Improved statistics accuracy and UI
- **Client**: Checks if updates are available and added a *donation* button
- **Client**: More UI tweaks

### Fixed issues

- **Server**: fixed server error when removing pipes from the pipeline.

## 0.2.1-Alpha

### Fixed issues

- **Server**: fixed update script not updating the database correctly.
- **Server**: fixed incomplete list of steps for some projects.
- **Client**: fixed crash when loading some projects.

## 0.2.0-Alpha

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
- **Server**: added the option to use an *SQLite* database instead of *MySQL*. This makes it even simpler to install and manage, when there isn't too many users.
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
- **Client**: Versions for working files and published items are now correctly sorted from highest version to lowest, according to the resource if any.
- **Maya Add-on**: The "preview" function now works correctly.
- **Maya Add-on (Rubika Flavor)**: Import Geo now works correctly.

## 0.1.3-Alpha

### Fixed issues

- **Client**: Loading of the projects taking a very long time has been fixed. Projects now load in an instant.
- **Client**: Linux .deb packaged fixed (the doc is now in its right folder).
- **Server and client**: Difficulty and estimations for shots are now correctly set.

## 0.1.2-Alpha

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
