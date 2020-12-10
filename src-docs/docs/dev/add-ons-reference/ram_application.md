# RamApplication

A class representing an application used in the pipeline (like Blender, Nuke, Krita, Photoshop…).

Inherits: [***RamObject***](ram_object.md)

[TOC]

## Attributes

| Attribute | Type | Default | Description |
| --- | --- | --- | --- |
| **executableFilePath** | *string* | `''` | Path to the executable file of the application. |
| **exportTypes** | *list of RamFileType* | `[]` | File types the app can export. |
| **importTypes** | *list of RamFileType* | `[]` | File types the app can import. |
| **nativeTypes** | *list of RamFileType* | `[]` | File types the app supports natively. |

## Methods

| Method | Arguments | Description |
| --- | --- | --- |
| ***constructor*** | *string*: **appName**<br />*string*: **appShortName**<br />*string*: **execFilePath**="" | |