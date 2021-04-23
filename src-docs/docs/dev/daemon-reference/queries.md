# Ramses Daemon: query reference

[TOC]

## ping

Checks if the *Daemon* is running, and gets its version and current user information.

### Query attributes

- *ping*

### Reply content

The *Daemon* replies with its version and information about the current user.

- *version*: **string**. The version of the client/daemon.
- *ramses*: **string**. The name of the client, should be "Ramses" for the official client.
- *userName*: **string**. The name of the user. Empty string if there's no logged-in user.
- *userShortName*: **string**. The short name of the user. Empty string if there's no logged-in user.
- *logged-in*:: **boolean**. True if a user is logged-in.

### Example

Query:  
`ping`

Reply:  
```json
{
    "accepted": true,
    "query": "ping",
    "success": true,
    "message": "Hi, this is the Ramses Daemon.",
    "content": {
        "version": "0.0.1",
        "ramses": "Ramses",
        "userName": "John Doe",
        "userShortName": "J.Doe",
        "logged-in": true
    }
}
```

## raise

Raises the *client* application main window.

### Query attributes

- *raise*

### Reply content

None

### Example

Query:  
`raise`

## getAssets

Returns the list of assets for the current project.

### Query attributes

- *getAssets*

### Reply content

The *Daemon* replies with the list of assets.

- *assets*: **array of objects**. The assets. Each asset is an object with:
    - *shortName*: **string**. The short name of the asset.
    - *name*: **string**. The name of the asset.
    - *folder*: **string**. The absolute path of the folder containing the asset.
    - *tags*: **array of strings**. The tags
    - *group*: **string**. The containing group name.

### Example

Query:  
`getAssets`

Reply:  
```json
{
    "accepted": true,
    "query": "getAssets",
    "success": true,
    "message": "Asset list retrived.",
    "content": {
        "assets": [
            {
                "shortName": "A1",
                "name": "Asset 01",
                "folder": "/path/to/A1",
                "tags": [
                    "tag1",
                    "tag2",
                    "tag3"
                ],
                "group": "Asset Group"
            },
            {
                "shortName": "A2",
                "name": "Asset 02",
                "folder": "/path/to/A2",
                "tags": [
                    "tag1",
                    "tag2",
                    "tag3"
                ],
                "group": "Asset Group"
            }
        ]
    }
}
```

## getCurrentProject

Returns the current project.

### Query attributes

- *getCurrentProject*

### Reply content

The *Daemon* replies with the project.

- *shortName*: **string**. The short name of the project.
- *name*: **string**. The name of the project.
- *width*: **int**. The width of the project in pixels.
- *height*: **int**. The width of the project in pixels.
- *framerate*: **float**. The framerate of the project.
- *folder*: **string**. The absolute path of the folder containing the project.

### Example

Query:  
`getCurrentProject`

Reply:  
```json
{
    "accepted": true,
    "query": "getProjects",
    "success": true,
    "message": "Current project is: Project 01.",
    "content": {
        "shortName": "P1",
        "name": "Project 01",
        "width": 2048,
        "height": 858,
        "framerate": 24.0,
        "folder": "/path/to/P1"
    }
}
```

## getCurrentUser

Returns the current user.

### Query attributes

- *getCurrentUser*

### Reply content

The *Daemon* replies with the user.

- *shortName*: **string**. The short name of the user.
- *name*: **string**. The name of the user.
- *role*: **string**. The role of the user, one of `'STANDARD'`, `'LEAD'`,`'PROJECT_ADMIN'`,`'ADMIN'`.
- *folderPath*: **string**. The absolute path to the user folder.

### Example

Query:  
`getCurrentUser`

Reply:  
```json
{
    "accepted": true,
    "query": "getCurrentUser",
    "success": true,
    "message": "Current user is: John Doe.",
    "content": {
        "shortName": "John",
        "name": "John Doe",
        "role": "LEAD",
        "folderPath": "/home/john/Ramses/Users/John"
    }
}
```


## getPipes

Returns the list of pipes for the current project.

### Query attributes

- *getPipes*

### Reply content

The *Daemon* replies with the list of Pipes.

- *pipes*: **array of objects**. The pipes. Each pipe is an object with:
    - *inputStepShortName*: **string**. The short name of the input step.
    - *outputStepShortName*: **string**. The short name of the output step.
    - *fileType*: **object**. The file type, an object with:
        - *name*: **string**. The name of the file type
        - *shortName*: **string**. The short name of the file type.
        - *extensions*: *array of string*. The extensions, including the leading `.`.

### Example

Query:  
`getPipes`

Reply:  
```json
{
    "accepted": true,
    "query": "getPipes",
    "success": true,
    "message": "Pipeline retrieved.",
    "content": {
        "pipes": [
            {
                "inputStepShortName": "TEX",
                "outputStepShortName": "MOD",
                "fileType": {
                    "name": "Alembic geometry data",
                    "shortName": "abc",
                    "extensions": [".abc"]
                    }
            },
            {
                "inputStepShortName": "STORY",
                "outputStepShortName": "CD",
                "fileType": {
                    "name": "JPEG Image",
                    "shortName": "jpg",
                    "extensions": [".jpg", ".jpeg"]
                    }
            }
        ]
    }
}
```

## getProjects

Returns the list of available projects.

### Query attributes

- *getProjects*

### Reply content

The *Daemon* replies with the list of projects.

- *projects*: **array of objects**. The projects. Each project is an object with:
    - *shortName*: **string**. The short name of the project.
    - *name*: **string**. The name of the project.
    - *width*: **int**. The width of the project in pixels.
    - *height*: **int**. The width of the project in pixels.
    - *framerate*: **float**. The framerate of the project.
    - *folder*: **string**. The absolute path of the folder containing the project.

### Example

Query:  
`getProjects`

Reply:  
```json
{
    "accepted": true,
    "query": "getProjects",
    "success": true,
    "message": "Project list retrived.",
    "content": {
        "projects": [
            {
                "shortName": "P1",
                "name": "Project 01",
                "width": 2048,
                "height": 858,
                "framerate": 24.0,
                "folder": "/path/to/P1"
            },
            {
                "shortName": "P2",
                "name": "Project 02",
                "width": 1998,
                "height": 1080,
                "framerate": 60.0,
                "folder": "/path/to/P2"
            }
        ]
    }
}
```

## getShots

Returns the list of assets for the current project.

### Query attributes

- *getShots*

### Reply content

The *Daemon* replies with the list of Shots.

- *shots*: **array of objects**. The shots. Each shot is an object with:
    - *shortName*: **string**. The short name of the shot.
    - *name*: **string**. The name of the shot.
    - *folder*: **string**. The absolute path of the folder containing the shot.
    - *duration*: **float**. The duration of the shot, in seconds

### Example

Query:  
`getShots`

Reply:  
```json
{
    "accepted": true,
    "query": "getShots",
    "success": true,
    "message": "Shot list retrived.",
    "content": {
        "shots": [
            {
                "shortName": "S1",
                "name": "Shot 01",
                "folder": "/path/to/S1",
                "duration": 1.25
            },
            {
                "shortName": "S2",
                "name": "Shot 02",
                "folder": "/path/to/S2",
                "duration": 2.4
            }
        ]
    }
}
```

## getStates

Returns the list of available states.

### Query attributes

- *getStates*

### Reply content

The *Daemon* replies with the list of States.

- *states*: **array of objects**. The states. Each states is an object with:
    - *shortName*: **string**. The short name of the state.
    - *name*: **string**. The name of the state.
    - *completionRatio*: **float**. The completion ratio of the state, in the range `[0, 100]`.
    - *color*: **array of integers**. The color for this state, `[R, G, B]` in the range `[0, 255]`.

### Example

Query:  
`getStates`

Reply:  
```json
{
    "accepted": true,
    "query": "getStates",
    "success": true,
    "message": "State list retrived.",
    "content": {
        "states": [
            {
                "shortName": "TODO",
                "name": "Waiting...",
                "completionRatio": 0,
                "color": [ 50, 50, 50]
            },
            {
                "shortName": "WIP",
                "name": "Work In Progress",
                "completionRatio": 50,
                "color": [ 255, 0, 0]
            },
            {
                "shortName": "OK",
                "name": "Finished and approved",
                "completionRatio": 100,
                "color": [ 0, 255, 0]
            }
        ]
    }
}
```

## getSteps

Returns the list of available steps for the current project.

### Query attributes

- *getSteps*

### Reply content

The *Daemon* replies with the list of Steps.

- *steps*: **array of objects**. The steps. Each step is an object with:
    - *shortName*: **string**. The short name of the step.
    - *name*: **string**. The name of the step.
    - *folder*: **string**. The absolute path to the common folder of the step.
    - *type*: **string**. One of `"PRE_PRODUCTION"`, `"ASSET_PRODUCTION"`, `"SHOT_PRODUCTION"`, `"POST_PRODUCTION"`.

### Example

Query:  
`getSteps`

Reply:  
```json
{
    "accepted": true,
    "query": "getSteps",
    "success": true,
    "message": "Step list retrived.",
    "content": {
        "steps": [
            {
                "shortName": "RIG",
                "name": "Rigging",
                "folder": "/path/to/RIG",
                "type": "ASSET_PRODUCTION"
            },
            {
                "shortName": "ANIM",
                "name": "Animation",
                "folder": "/path/to/ANIM",
                "type": "SHOT_PRODUCTION"
            }
        ]
    }
}
```

## setCurrentProject

Sets the current project

### Query attributes

- *setCurrentProject*
- *shortName*: **string**. The short name (or the uuid) of the project.

### Reply content

The *Daemon* replies with more information about the project.

- *name*: **string**. The name of the project.
- *shortName*: **string**. The short name of the project.
- *path*: **string**. The absolute path to the project.
- *uuid*: **boolean**. The internal unique identifier of the project.

### Example

Query:  
`setCurrentProject&name=The Project&shortName=PROJ`

Reply:  
```json
{
    "accepted": true,
    "query": "setCurrentProject",
    "success": true,
    "message": "Current project set to: The Project.",
    "content": {
        "name": "The Project",
        "shortName": "PROJ",
        "path": "/home/user/Ramses/PROJECTS/The Project",
        "uuid": "12456abc"
    }
}
```

____

![META](authors:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2021;updated:2021/04/22)