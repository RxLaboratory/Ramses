# RamSettings

Gets and saves settings used by Ramses.

One (and only one) instance globally available (RamSettings is a *singleton*), by calling `RamSettings.instance()` or `Ramses.instance().settings()`.

To get a setting, just get the corresponding attribute.

To change a setting temporarily, just set the corresponding attribute. If you want the change to be permanent (i.e. keep the setting for the next sessions), call the `save()` method.

By default, settings are saved in a `ramses_addons_settings.json` file, in the user's OS specific settings folder:

- Linux: `~/.config/RxLaboratory/Ramses/Config`
- Windows: `%appdata%/RxLaboratory/Ramses/Config`
- MacOS: ?

[TOC]

## Static Methods

| Method | Arguments | Description |
| --- | --- | --- |
| **instance**<br />▹ *RamSettings* | | Returns the *RamSettings* unique instance. |

## Attributes

| Attribute | Type | Default | Description |
| --- | --- | --- | --- |
| **autoConnect** | *boolean* | `true` | Wether to always try to (re)connect to the *Daemon* if offline. |
| **ramsesClientPath** | *string* | os-specific | Location of the Ramses Client executable file (.exe on Windows, .app on MacOS, .appimage or binary on Linux) |
| **ramsesClientPort** | *integer* | `18185` | Listening port of the Ramses Daemon |
| **logLevel** | *LogLevel* | `LogLevel.Info` | Changes the quantity of messages in the log. One of: `LogLevel.DataReceived`, `LogLevel.DataSent`, `LogLevel.Debug`, `LogLevel.Info`, `LogLevel.Critical`, `LogLevel.Fatal` |

## Methods

| Method | Arguments | Description |
| --- | --- | --- |
| **save** | | Saves the current settings to the disk. |
| **folderPath**<br />▹ *string* | |The folder path to the settings |
| **filePath**<br />▹ *string* | | The file path to the settings |

____

## API Dev notes

!!! note
    This section is for the development of the API only; you should not need these when developping your add-on using the API.

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