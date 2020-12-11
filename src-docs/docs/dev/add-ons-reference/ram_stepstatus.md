# RamStepStatus

A history of RamStatus for a given step.

[TOC]

## Attributes

| Attribute | Type | Default | Description |
| --- | --- | --- | --- |

## Static Methods

| Method | Arguments | Description |
| --- | --- | --- |
| **getFromPath**<br />▹ *RamStepStatus* | *string*: **filePath**<br /> | Returns a *RamStepStatus* instance built using the given file path. |

## Methods

| Method | Arguments | Description |
| --- | --- | --- |
| ***constructor*** | | |
| **getHistory**<br />▹ *list of [RamStatus](ram_status.md)* | | |
| **setStatus** | *RamStatus*: **status** | Adds a new status to the history |