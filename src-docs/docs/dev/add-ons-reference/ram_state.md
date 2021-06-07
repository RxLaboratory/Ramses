# RamState

Represents a state used in a status, like "CHK" (To be checked), "OK" (ok), "TODO", etc.

Inherits: [***RamObject***](ram_object.md)

## Static Methods

| Method | Arguments | Description |
| --- | --- | --- |
| **fromDict**<br />▹ *RamState* | *dict or object*: **stateDict** | Builds a *RamState* from a dict or object like the one returned by the *[RamDaemonInterface](ram_daemon_interface.md)* |

## Methods

| Method | Arguments | Description |
| --- | --- | --- |
| ***constructor*** | *string*: **stateName**,<br />*string*: **stateShortName**,<br />*float*: **completionRatio**=`0`,<br />*list of int*: **color**=`[67, 67, 67]` | |
| **completionRatio**<br />▹ *integer* | | The ratio of completion of this state in the range `[0, 100]`. |
| **color**<br />▹ *array of integers* | | The color for this state, `[R, G, B]` in the range `[0, 255]`. |

____

## API Dev notes

!!! note
    These section is for the development of the API only; you should not need these when developping your add-on using the API.

### (Im)mutable data

The data returned by the methods can be either [mutable or immutable](implementation.md#accessing-the-data).

| Method | Type of the returned data |
| --- | --- |
| **completionRatio** | <i class="fa fa-lock"></i> Immutable |
| **color** | <i class="fa fa-lock"></i> Immutable |

![META](authors:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2021;updated:2021/05/25)