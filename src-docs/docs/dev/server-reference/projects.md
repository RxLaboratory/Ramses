# Queries for managing projects

[TOC]

## createProject

Creates a new project in the database.

**Query attributes:**

- *name*: **string**. The new name.
- *shortName*: **string**. The new shortName.
- *uuid*: **string** (optionnal). The user's Universal Unique Identifier.
- *token*: **string**. The session token returned with [*login*](general.md#login).

**Reply content:**

Empty

**Examples:**

Query:  
`http://your.server/ramses/?createProject&name=Mythomen&shortName=Mytho&uuid=123&token=123`

Reply:

```json
{
    "accepted": true,
    "query": "createProject",
    "success": true,
    "message": "Project \"Mytho\" created.",
    "content": { }
}
```

## getProject

Retrieves a single project.

**Query attributes:**

- *uuid*: **string**. The project unique identifier.
- *token*: **string**. The session token returned with [*login*](general.md#login).

**Reply content:**

The server replies with a project object with the following attributes:

- *name*: **string**. The name of the user.
- *shortName*: **string**. The username.
- *uuid*: **string**. The Universal Unique Identifier of this user.
- *folderPath*: **string**. The user folder path.
- *framerate*: **float**. The framerate of the project.
- *width*: **integer**. The width of the project in pixels.
- *height*: **integer**. The height of the project in pixels.
- *aspectRatio*: **float**. The aspectRatio of the project.
- *pipeFiles*: **array of object**. The list of files used through the pipes. This pipeFile object containes these properties:
    - *uuid*: **string**.
    - *shortName*: **string**.
    - *fileTypeUuid*: **string**.
    - *colorSpaceUuid*: **string**.
    - *projectUuid*: **string**.
- *steps*: **array: of object**. The list of steps. This step object contains these properties:
    - *name*: **string**.
    - *shortName*: **string**.
    - *uuid*: **string**.
    - *projectUuid*: **string**. The UUID of the project.
    - *type*: **string**. One of `asset`, `shot`, `pre`, or `post`.
    - *order*: **int**. Order of the step in the pipeline.
- *pipes*: **array of object**. The list of pipes connecting the steps. The pipe object contains these properties:
    - *uuid*: **string**.
    - *projectUuid*: **string**. The UUID of the project.
    - *inputStepUuid*: **string**. The UUID of the input step.
    - *outputStepUuid*: **string**. The UUID of the output step.
    - *pipeFiles*: **array of string**. The list of UUIDs of the pipe file types going through this pipe.
- *assetGroups*: **array of object**. The list of asset groups. This asset group object contains these properties:
    - *name*: **string**
    - *shortName*: **string**.
    - *uuid*: **string**.
    - *projectUuid*: **string**.
- *assets*:  **array of object**. The list of assets. This asset object contains these properties:
    - *name*: **string**.
    - *shortName*: **string**.
    - *tags*: **string**. A comma-separated list of descriptive tags.
    - *assetGroupUuid*: **string**. The UUID of the containing asset group.
    - *uuid*: **string**.
- *sequences*: **array of object**. The list of sequences. This sequence object contains these properties:
    - *name*: **string**
    - *shortName*: **string**.
    - *uuid*: **string**.
    - *projectUuid*: **string**.
- *shots*:  **array of object**. The list of shots. This shot object contains these properties:
    - *name*: **string**.
    - *shortName*: **string**.
    - *duration*: **float**. The duration of the shot in seconds.
    - *order*: **integer**. The order of the shot in the sequence.
    - *sequenceUuid*: **string**. The UUID of the containing sequence.
    - *uuid*: **string**.

**Examples:**

Query:  
`http://your.server/ramses/?getProject&uuid=123abc&token=123`

Reply:

```json
{
    "accepted": true,
    "query": "getProject",
    "success": true,
    "message": "Project retrieved",
    "content": {
            "name": "Mythomen",
            "shortName": "Mytho",
            "uuid": "123abc",
            "folderPath": "PROJECTS/MYTHO",
            "framerate": 25.0,
            "width": 1920,
            "height": 1080,
            "aspectRatio": 1.78,
            "steps": ["uuid123", "uuid456"],
            "shots": ["uuidshot123", "uuidshot456"]
        }
}
```

## getProjects

Retrieves the list of all projects.

**Query attributes:**

- *token*: **string**. The session token returned with [*login*](general.md#login).

**Reply content:**

The server replies with an array of project information. Each project is an object with the following attributes:

- *name*: **string**. The name of the user.
- *shortName*: **string**. The username.
- *uuid*: **string**. The Universal Unique Identifier of this user.
- *folderPath*: **string**. The user folder path.
- *framerate*: **float**. The framerate of the project.
- *width*: **integer**. The width of the project in pixels.
- *height*: **integer**. The height of the project in pixels.
- *aspectRatio*: **float**. The aspectRatio of the project.

Query:  
`http://your.server/ramses/?getProjects&token=123`

Reply:

```json
{
    "accepted": true,
    "query": "getProjects",
    "success": true,
    "message": "Projects list retrieved.",
    "content":  [
        {
            "name": "Mythomen",
            "shortName": "Mytho",
            "uuid": "2d7d7e01-671c-11e7-a78f-4ccc6a288527",
            "folderPath": "PROJECTS/MYTHO",
            "framerate": 25.0,
            "width": 1920,
            "height": 1080,
            "aspectRatio": 1.78
        },
        {
            "name": "Purpleboy",
            "shortName": "PPB",
            "uuid": "2d7d7e01-671c-11e7-a78f-4ccc6a288527",
            "folderPath": "PROJECTS/PPB",
            "framerate": 24.0,
            "width": 2048,
            "height": 858,
            "aspectRatio": 2.39
        }
    ]
}
```

## updateProject

Updates project information.

**Query attributes:**

- *name*: **string**. The new (or current for no change) name.
- *shortName*: **string**. The new (or current for no change) shortName.
- *comment*: **string**. The new comment.
- *folderPath*: **string** (optionnal). The path for the project folder.
- *uuid*: **string**. The user's Universal Unique Identifier.
- *token*: **string**. The session token returned with [*login*](general.md#login)

**Reply content:**

Empty

**Examples:**

Query:  
`http://your.server/ramses/?updateProject&name=Mythomen&comment=A comment&shortName=Mytho&uuid=123&token=123`

Reply:

```json
{
    "accepted": true,
    "query": "updateProject",
    "success": true,
    "message": "Project \"Mytho\" updated.",
    "content": { }
}
```

## removeProject

Removes a project from the database.

**Query attributes:**

- *uuid*: **string**. The project's Universal Unique Identifier.
- *token*: **string**. The session token returned with [*login*](general.md#login).

**Reply content:**

Empty

**Examples:**

Query:  
`http://your.server/ramses/?removeProject&uuid=123&token=123`

Reply:

```json
{
    "accepted": true,
    "query": "removeProject",
    "success": true,
    "message": "Project uuid123 removed.",
    "content": { }
}
```

## assignStep

Assigns a template step to a project

**Query attributes:**

- *stepUuid*: **string**. The template step's Universal Unique Identifier.
- *projectUuid*: **string**. The project's Universal Unique Identifier.
- *token*: **string**. The session token returned with [*login*](general.md#login).

**Reply content:**

Empty

**Examples:**

Query:  
`http://your.server/ramses/?assignStep&stepUuid=123&projectUuid=123&token=123`

Reply:

```json
{
    "accepted": true,
    "query": "assignStep",
    "success": true,
    "message": "Step uuid123 associated with project uuid456.",
    "content": { }
}
```

{
    "accepted": true,
    "query": "assignStep",
    "success": true,
    "message": "Step uuid123 removed from project uuid456.",
    "content": { }
}
```