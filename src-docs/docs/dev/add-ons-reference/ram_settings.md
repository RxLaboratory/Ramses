# RamSettings

Gets and saves settings used by Ramses.

To get a setting, just get the corresponding attribute.

To change a setting temporarily, just set the corresponding attribute. If you want the change to be permanent (i.e. keep the setting for the next sessions), call the `save()` method.

By default, settings are saved in a `ramses_addons_settings.json` file, in the user's OS specific settings folder:

- Linux: `~/.config/RxLaboratory/Ramses/`
- Windows: `%appdata%/RxLaboratory/Ramses/`
- MacOS: ?

There is only one instance of RamSettings, available with the [`Ramses.instance.settings()`](ramses.md) method

[TOC]

## Attributes

| Attribute | Type | Default | Description |
| --- | --- | --- | --- |
| **autoConnect** | *boolean* | `true` | Wether to always try to (re)connect to the *Daemon* if offline. |
| **ramsesClientPath** | *string* | os-specific | Location of the Ramses Client executable file (.exe on Windows, .app on MacOS, .appimage or binary on Linux) |
| **ramsesClientPort** | *integer* | `18185` | Listening port of the Ramses Daemon |
| **folderPath** | *string* | os-specific | Read-only. The folder path to the settings |
| **filePath** | *string* | os-specific | Read-only. The file path to the settings |

## Methods

| Method | Arguments | Description |
| --- | --- | --- |
| **save** | | Saves the current settings to the disk. |

____

## API Dev notes

!!! note
    These section is for the development of the API only; you should not need these when developping your add-on using the API.

### Private Attributes

| Attribute | Type | Default | Description |
| --- | --- | --- | --- |
| **folderNames** | *FolderNames* | | A simple class containing the common names for subfolders. |
| **defaultStates** | *list of [RamState](ram_state.md)* | | The list of states to use if the *Daemon* is not available. |

#### FolderNames Attributes

FolderNames is a simple class which contains common names for subfolders.

| Attribute | Type | Default | Description |
| --- | --- | --- | --- |
| **preview** | *string* | "_preview" | |
| **publish** | *string* | "_published" | |
| **versions** | *string* | "_versions" | |
| **userConfig** | *string* | "Config" | |
| **stepTemplates** | *string* | "Templates" | |

### Examples

```py
ramses = Ramses.instance
settings = ramses.settings
theAsset = RamAsset("thing", "stuff")
thePreviewFolder = theAsset.folderPath() + "/" + settings.folderNames.preview
```
____

![META](authors:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2021;updated:2021/04/22)