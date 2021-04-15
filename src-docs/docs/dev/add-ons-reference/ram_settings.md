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