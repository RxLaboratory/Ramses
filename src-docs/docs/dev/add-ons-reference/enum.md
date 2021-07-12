# Enumerated values (constants)

Enumerated values (a.k.a. Constants) are predefined sets of values to be used in specific methods. Their name *should be* in CAPITAL_SNAKE_CASE, and they’re grouped together by type (for exemple *Color.RED*, or *Shape.RECTANGLE*). The value itself can be of any type (array, string, number…).

In this documentation they're written like this: `ValueType.VALUE`, i.e. `ItemType.SHOT` or `ItemType.ASSET`, and they're used the same way in the actual code.

!!! warning
    For the current 0.x versions of the framework, not all enumerated values respect the naming CAPITAL_SNAKE_CASE convention. This will change before the release of 1.0; when using the *API* during the 0.x development phase, be careful when updating as you may need to rename these values in your code.  
    This reference will be kept up-to-date in real time.

## FileNames

The names of some files reserved by *Ramses*.

| Name | Actual value | Description |
| --- | --- | --- |
| FileNames.**META_DATA** | `"_ramses_data.json"` | |

## FolderNames

The names of the folders used by the [*Ramses Tree*](../../components/files/index.md).

| Name | Actual value | Description |
| --- | --- | --- |
| FolderNames.**admin** | `"00-ADMIN"` | |
| FolderNames.**assets** | `"04-ASSETS` | |
| FolderNames.**export** | `"06-EXPORT` | |
| FolderNames.**postProd** | `03-POST-PROD"` | |
| FolderNames.**preProd** | `"01-PRE-PROD"` | |
| FolderNames.**preview** | `"_preview"` | |
| FolderNames.**prod** | `"02-PROD"` | |
| FolderNames.**projects** | `"Projects"` | |
| FolderNames.**publish** | `"_published"` | |
| FolderNames.**shots** | `"05-SHOTS"` | |
| FolderNames.**userConfig** | `"Config"` | |
| FolderNames.**users** | `"Users"` | |
| FolderNames.**versions** | `"_versions"` | |

## ItemType

Used to differenciate between Shots, Assets, and General items.

| Name | Actual value | Description |
| --- | --- | --- |
| ItemType.**ASSET** | `'A'` | |
| ItemType.**GENERAL** | `'G'` | |
| ItemType.**SHOT** | `'S'` | |

## Log

A list of messages to be logged / shown to the user.

| Name | Actual value | Description |
| --- | --- | --- |
| Log.**MalformedName** | `"Cannot work on this file as it does not respect the Ramses' naming scheme, sorry."` | |
| Log.**NoProject** | `"There's no current project. Select a project first."` | |
| Log.**NotAnItem** | `"This file does not seem to be a valid Ramses Item, sorry."` | |
| Log.**NoUser** | `"There's no current user. You may need to log in."` | |
| Log.**PathNotFound** | `"The file or folder path could not be found."` | |
| Log.**StateNotFound** | `"State not found."` | |

## LogLevel

Importance of the log messages.

The API itself should not log any `LogLevel.Info`, this level is reserved for the add-ons developped using the API.

| Name | Actual value | Description |
| --- | --- | --- |
| LogLevel.**DataReceived** | `-2` | The raw data received from the *Daemon* |
| LogLevel.**DataSent** | `-1` | The queries sent to the *Daemon* |
| LogLevel.**Debug** | `0` | General debug information |
| LogLevel.**Info** | `1` | General information to be shown to the user |
| LogLevel.**Critical** | `2` | An non-blocking error |
| LogLevel.**Fatal** | `3` | A fatal error |

## MetaDataKeys

The available predefined keys to store file metadata.

| Name | Actual value | Description |
| --- | --- | --- |
| MetaDataKeys.**COMMENT** | `"comment"` | A comment associated to the file |
| MetaDataKeys.**PIPE_TYPE** | `"pipeFileShortName"` | The type of the pipe associated to the file |
| MetaDataKeys.**VERSION** | `"version"` | A version associated to the file |
| MetaDataKeys.**VERSION_FILE** | `"versionFilePath"` | A version file associated to the file |

## StepType

The type of a step. Usually used to filter available steps.

| Name | Value | Description |
| --- | --- | --- |
| StepType.**PRE_PRODUCTION** | `'PRE_PRODUCTION'` | |
| StepType.**ASSET_PRODUCTION** | `'ASSET_PRODUCTION'` | |
| StepType.**SHOT_PRODUCTION** | `'SHOT_PRODUCTION'` | |
| StepType.**POST_PRODUCTION** | `'POST_PRODUCTION'` | |
| StepType.**PRODUCTION** | `'PRODUCTION'` | Use for *ASSET* and *SHOT* production together. |
| StepType.**ALL** | `'ALL'` | Use for all steps. |

## UserRole

The role of the user (i.e. its administration rights).

| Name | Value | Description |
| --- | --- | --- |
| UserRole.**ADMIN** | `3` | |
| UserRole.**PROJECT_ADMIN** | `2` | |
| UserRole.**LEAD** | `1` | |
| UserRole.**STANDARD** | `0` | |

## Examples

```py
# Python

from ramses import RamItem, ItemType

# Get an item from a specific file path
anItem = RamItem.getFromPath( somePath )

if anItem is not None:
    # We want it only if it's a shot,
    # We can use the enum ItemType to compare the value.
    if anItem.itemType() == ItemType.SHOT:
        doSomething()
```

```js
// ExtendScript

#include ramses.jsxinc

// Get an item from a specific file path
var anItem = RamItem.getFromPath( somePath );

if (anItem != null)
{
    // We want it only if it's a shot,
    // We can use the enum ItemType to compare the value.
    if ( anItem.itemType() == ItemType.SHOT )
        doSomething();
}
```

![META](authors:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2021;updated:2021/06/10)
