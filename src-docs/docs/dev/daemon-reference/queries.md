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

## setCurrentProject

Sets the current project

### Query attributes

- *setCurrentProject*
- *name*: **string**. The name of the project.
- *shortName*: **string**. The short name of the project.

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

![META](authors:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2021;updated:2021/04/05)