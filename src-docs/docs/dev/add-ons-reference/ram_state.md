# RamState

Represents a state used in a status, like "CHK" (To be checked), "OK" (ok), "TODO", etc.

Inherits: [***RamObject***](ram_object.md)

[TOC]

## Attributes

| Attribute | Type | Default | Description |
| --- | --- | --- | --- |

## Methods

| Method | Arguments | Description |
| --- | --- | --- |
| ***constructor*** | *string*: **stateName**<br />*string*: **stateShortName**<br />*float*: **completionRatio** | |
| **completionRatio**<br />▹ *integer* | | The ratio of completion of this state in the range `[0, 100]`. |
| **color**<br />▹ *array of integers* | | The color for this state, `[R, G, B]` in the range `[0, 255]`. |