![META](authors:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2021-2022;updated:2022/09/01)

# Objects and data

In Ramses, everything is represented by *Objects*. An object consists of a ***Unique Universal Identifier (UUID)*** used to identify it across the whole system, and its associated ***data***, encoded as a *JSON Object*.

The whole object could thus be saved and shared as a *JSON* Document like this:

```json
{
    "uuid": "unique-uid",
    "data": {
        // Here's the data
    }    
}
```

An object may also be associated with some ***paths*** in the file system. These paths are dynamic and are not included in the associated *data*. They have to be queried or defined by the software.

Ramses Objects (i.e. *RamObjects*) can represent a shot, an asset, a user, an application, or a whole project for example. They can also contain lists of other objects. Lists of objects are basic string lists of *UUIDs* stored in the parent object data.

*UUIDs* and object data are synced by all the *Ramses Applications* and a *Ramses Server*, and are mirrored on all workstations and the server itself. Any other software (e.g. *APIs*, scripts, plugins...) should get the list of objects and all data using the [*Ramses Daemon*](../daemon-reference/indexmd) run by the *Ramses Application* which is guaranteed to be up-to-date. You can also use the *Daemon* to set new data to specific objects.

## Virtual objects

### RamObject

All objects derive from the *RamObject* and have at least the same properties.

!!! note
    Some optional properties may be missing in the *JSON* data as shared by *Ramses*. In this case, it's up to the software to decide of a default value.

```json
{
    "shortName": "a short ID",
    "name": "The object name",
    "comment": "A comment associated with the object",
    "color": "#e3e3e3", // Colors are Hexadecimal numbers with a leading #
    "settings": {
        // The settings object can contain additional arbitrary values,
        // depending on the object
        // these values can be set by any software / API
    }
}
```

### RamObjectList

A list of other objects.

```json
{
    "shortName": "a short ID",
    "name": "The object name",
    "comment": "A comment associated with the object",
    "color": "#e3e3e3",
    "settings": { },
    "list": [
        "uuid1",
        "uuid2",
        "..."
    ]
}
```

## Objects

### RamApplication

An application used in the production pipeline.

```json
{
    "shortName": "a short ID",
    "name": "The object name",
    "comment": "A comment associated with the object",
    "color": "#e3e3e3",
    "settings": { },
    "executableFilePath": "path/to/the/application/bin.exe",
    "nativeFileTypes": "RamFileType_list-uuid",
    "importFileTypes": "RamFileType_list-uuid",
    "exportFileTypes": "RamFileType_list-uuid",
}
```

### RamAsset

An asset used in one or several shots. The *RamAsset* is derived from the *RamItem* and has all its properties.

```json
{
    "shortName": "a short ID",
    "name": "The object name",
    "comment": "A comment associated with the object",
    "color": "", // Should be the same as the containing asset group
    "settings": { },
    "project": "RamProject-uuid",
    "productionType": "RamProject-uuid",
    "statusHistory": {
        "RamStep-uuid": "RamStatus_list-uuid",
        "RamStep-uuid": "RamStatus_list-uuid",
        "RamStep-uuid": "RamStatus_list-uuid",
        "...": "..."
    },
    "assetGroup": "RamAssetGroup-uuid",
    "tags": [
        "tag1",
        "tag2",
        "..."
    ]
}
```

### RamAssetGroup

A group of assets. All assets should be assigned to one and only one group.

```json
{
    "shortName": "a short ID",
    "name": "The object name",
    "comment": "A comment associated with the object",
    "color": "#e3e3e3",
    "settings": { },
    "project": "RamProject-uuid"
}
```

### RamFileType

A type of file.

```json
{
    "shortName": "abc", // Should be the default file extension
    "name": "The object name",
    "comment": "A comment associated with the object",
    "color": "#e3e3e3",
    "settings": { },
    "extensions": [
        "abc",
        "def",
        "etc"
    ],
    "previewable": true // Is it standard and can it be opened with the OS default file viewer?
}
```

### RamItem

An item (i.e. a shot or an asset or another production file/folder)

```json
{
    "shortName": "a short ID",
    "name": "The object name",
    "comment": "A comment associated with the object",
    "color": "#e3e3e3",
    "settings": { },
    "project": "RamProject-uuid",
    "productionType": "RamProject-uuid",
    "statusHistory": {
        "RamStep-uuid": "RamStatus_list-uuid",
        "RamStep-uuid": "RamStatus_list-uuid",
        "RamStep-uuid": "RamStatus_list-uuid",
        "...": "..."
    }
}
```

### RamPipe

A connexion between two steps in the production pipeline.

```json
{
    "shortName": "a short ID",
    "name": "The object name",
    "comment": "A comment associated with the object",
    "color": "#e3e3e3",
    "settings": { },
    "inputStep": "RamStep-uuid", // The step this pipe is connected to
    "outputStep": "RamStep-uuid", // The step this pipe comes from
    "project": "RamProject-uuid",
    "pipeFiles": "RamPipeFile_list-uuid" // The list of files used through this pipe
}
```

### RamPipeFile

A a specific format used through a pipe.

```json
{
    "shortName": "a short ID",
    "name": "a short ID.ext", // Should be the shortname and the extension of the file type
    "comment": "A comment associated with the object",
    "color": "#e3e3e3",
    "settings": { },
    "fileType": "RamFileType-uuid",
    "project": "RamProject-uuid",
    "customSettings": "- format: ext" // Settings to be used in applications when handling this format; formatted as a YAML string.
}
```

### RamProject

A project (feature film, episode, short movie...)

```json
{
    "shortName": "a short ID",
    "name": "a short ID.ext", // Should be the shortname and the extension of the file type
    "comment": "A comment associated with the object",
    "color": "#e3e3e3",
    "settings": { },
    "steps": "RamStep_list-uuid",
    "assetGroups": "RamAssetGroup_list-uuid",
    "sequences": "RamSequence_list-uuid",
    "shots": "RamShot_list-uuid",
    "assets": "RamAsset_list-uuid",
    "pipeline": "RamPipe_list-uuid",
    "pipeFiles": "RamPipeFile_list-uuid",
    "users": "RamUser_list-uuid",
    "scheduleComments": "RamScheduleComment_list-uuid",
    "framerate": 24.0,
    "width": 2048,
    "height": 858,
    "aspectRatio": 2.85,
    "deadline": "2023-01-01"
}
```

### RamScheduleComment

A simple comment in the schedule.

```json
{
    "shortName": "a short ID",
    "name": "The object name",
    "comment": "A comment associated with the object",
    "color": "#e3e3e3",
    "settings": { },
    "date": "2022-09-01 12:00:00"
}
```

### RamScheduleEntry

An entry in the schedule, i.e. a step associated to a user and a date.

```json
{
    "shortName": "a short ID",
    "name": "The object name",
    "comment": "A comment associated with the object",
    "color": "#e3e3e3",
    "settings": { },
    "date": "2022-09-01 12:00:00",
    "user": "RamUser-uuid",
    "step": "RamStep-uuid"
}
```

### RamSequence

A sequence containing shots. All shots must be part of one and only one sequence.

```json
{
    "shortName": "a short ID",
    "name": "The object name",
    "comment": "A comment associated with the object",
    "color": "#e3e3e3",
    "settings": { },
    "project": "RamProject-uuid"
}
```

### RamShot

A shot. All shots must be part of one and only one sequence. The *RamShot* is derived from the *RamItem* and has all its properties.

```json
{
    "shortName": "a short ID",
    "name": "The object name",
    "comment": "A comment associated with the object",
    "color": "#e3e3e3",
    "settings": { },
    "project": "RamProject-uuid",
    "productionType": "RamProject-uuid",
    "statusHistory": {
        "RamStep-uuid": "RamStatus_list-uuid",
        "RamStep-uuid": "RamStatus_list-uuid",
        "RamStep-uuid": "RamStatus_list-uuid",
        "...": "..."
    },
    "sequence": "RamSequence-uuid",
    "duration": 5, // in seconds
    "assets": "RamAsset_list-uuid" // The list of assets used in this shot
}
```

### RamState

A state of production, used in the more general *RamStatus*.

```json
{
    "shortName": "a short ID",
    "name": "The object name",
    "comment": "A comment associated with the object",
    "color": "#e3e3e3",
    "settings": { },
    "completionRatio": 50 // %
}
```

### RamStatus

The status of the production of an item, for a specific step.

```json
{
    "shortName": "a short ID",
    "name": "The object name",
    "comment": "A comment associated with the object",
    "color": "#e3e3e3",
    "settings": { },
    "completionRatio": 50, // %
    "user": "RamUser-uuid", // The last user who's modified the status
    "step": "RamStep-uuid",
    "item": "RamItem-uuid or RamShot-uuid or RamAsset-uuid",
    "itemType": "asset", // One of asset or shot or item
    "state": "RamState-uuid",
    "version": 12,
    "date": "2022-09-15 13:00:00 ", // The modification date of the status
    "published": true, // Is this item ready to be used in another step?
    "assignedUser": "RamUser-uuid", // Who's working on this?
    "timeSpent": 3600, // Seconds
    "difficulty": "medium", // One of: veryEasy, easy, medium, hard, veryHard
    "goal": 3 // How long this shoud take, in days
}
```

### RamStep

A step in the production pipeline.

```json
{
    "shortName": "a short ID",
    "name": "The object name",
    "comment": "A comment associated with the object",
    "color": "#e3e3e3",
    "settings": { },
    "project": "RamProject-uuid",
    "estimationMultiplyGroup": "RamAssetGroup-uuid", // Used to estimate time for shots
    "type": "shot", // One of pre, asset, shot, post, all
    "publishSettings": "- format: ext", // Some settings to be used by an application to publish files. Should be a YAML-formatted string
    "estimationMethod": "shot", // Pne of shot, second. Estimate work per shot or per second
    "estimationVeryEasy": 0.1, // days
    "estimationEasy": 0.5,
    "estimationMedium": 1,
    "estimationHard": 2,
    "estimationVeryHard": 5
}
```

### RamUser

A user of Ramses.

```json
{
    "shortName": "a short ID",
    "name": "The object name",
    "comment": "A comment associated with the object",
    "color": "#e3e3e3",
    "settings": { },
    "role": "lead", // One of admin, project, lead, standard,
    "schedule": "RamScheduleEntry_list-uuid",
    "email": "contact@rxlaboratory.org"
}
```
!!! note
    User data is encrypted in databases saved by the *Ramses Application* and *Ramses Server* to protect user privacy. Extensions (scripts, APIs, etc) should **never** write user data to any file, unless it is safely encrypted too.

    The email should be used for technical features, like password retrieval, but never be available to other users.