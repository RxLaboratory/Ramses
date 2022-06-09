![META](authors:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2021-2022;updated:2022/01/16)

# Queries for managing projects

## createProject

`http://your.server/ramses/?createProject`

Creates a new project in the database.

**Query attributes:**

- *name*: **string**. The new name.
- *shortName*: **string**. The new shortName.
- *uuid*: **string** (optionnal). The user's Universal Unique Identifier.
- *version*: **string**. The version of the client.
- *token*: **string**. The session token returned by [*login*](general.md#login).

**Reply content:**

Empty

**Reply body**:

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

`http://your.server/ramses/?getProject`

Retrieves a single project.

**Query attributes:**

- *uuid*: **string**. The project unique identifier.
- *version*: **string**. The version of the client.
- *token*: **string**. The session token returned by [*login*](general.md#login).

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
    - *uuid*: **string**. The Universal Unique Identifier of this pipe file.
    - *shortName*: **string**. The short name.
    - *comment*: **string**. A comment for this pipe file.
    - *fileTypeUuid*: **string**. The UUID of the associated file type.
    - *colorSpaceUuid*: **string**. The UUID of the associated color space.
    - *projectUuid*: **string**. The UUID of the project.
- *steps*: **array: of object**. The list of steps. This step object contains these properties:
    - *name*: **string**. The name.
    - *shortName*: **string**. The short name.
    - *comment*: **string**. A comment for this step.
    - *uuid*: **string**. The Universal Unique Identifier of this step.
    - *projectUuid*: **string**. The UUID of the project.
    - *type*: **string**. One of `asset`, `shot`, `pre`, or `post`.
    - *order*: **int**. Order of the step in the pipeline.
    - *color*: **string**. The hexadecimal code of the color used to display the step in the UI.
    - *estimationMethod*: **string**. For shot production steps, the method used for estimations. One of `shot` or `second`.
    - *estimationVeryEasy*: **float**. The very easy estimation in days.
    - *estimationEasy*: **float**. The easy estimation in days.
    - *estimationMedium*: **float**. The medium estimation in days.
    - *estimationHard*: **float**. The hard estimation in days.
    - *estimationVeryHard*: **float**. The very hard estimation in days.
    - *multiplyGroupUuid*: **string**. The uuid of an asset group used to multiply estimations.
- *pipes*: **array of object**. The list of pipes connecting the steps. The pipe object contains these properties:
    - *uuid*: **string**.
    - *projectUuid*: **string**. The UUID of the project.
    - *inputStepUuid*: **string**. The UUID of the input step.
    - *outputStepUuid*: **string**. The UUID of the output step.
    - *pipeFiles*: **array** of **string**. The list of UUIDs of the pipe file types going through this pipe.
- *assetGroups*: **array of object**. The list of asset groups. This asset group object contains these properties:
    - *name*: **string**. The name of the asset group.
    - *shortName*: **string**. The short name of the asset group.
    - *comment*: **string**. The comment.
    - *uuid*: **string**. The UUID of this asset group.
    - *projectUuid*: **string**. The UUID of the project.
- *assets*:  **array of object**. The list of assets. This asset object contains these properties:
    - *name*: **string**. The name of the asset.
    - *shortName*: **string**. The short name of the asset.
    - *comment*: **string**. A comment.
    - *tags*: **string**. A comma-separated list of descriptive tags.
    - *assetGroupUuid*: **string**. The UUID of the containing asset group.
    - *uuid*: **string**. The UUID of this asset.
    - *statusHistory*: **array** of **object**. The status history for this asset. Each object represents a past status, and contains these properties:
        - *uuid*: **string**. The UUID of this status.
        - *completionRatio*: **int**. The completion ratio, a percentage in the range `[0-100]`.
        - *comment*: **string**. A comment.
        - *version*: **int**. The version.
        - *date*: **string**. The date and time of the status, formatted as `yyyy-MM-dd hh:mm:ss`.
        - *userUuid*: **string**. The UUID of the user who set this status.
        - *stateUuid*: **string**. The UUID of the associated state.
        - *stepUuid*: **string**. The UUID of the associated step.
        - *assignedUserUuid*: **string**. The UUID of the user assigned to this task.
        - *published*: **int**. The whether the asset has been published, `0` or `1`.
        - *timeSpent*: **int**. The time spent working on this asset, in seconds.
        - *difficulty*: **string**. The difficulty, one of `veryEasy`, `easy`, `medium`, `hard` or `veryHard`.
        - *estimation*: **float**. The estimation, in days. If this is a negative number, it means the estimation has to be automatically computed.
        - *assetUuid*: **string**. The UUID of the associated asset.
- *sequences*: **array of object**. The list of sequences. This sequence object contains these properties:
    - *name*: **string**. The name of the sequence.
    - *shortName*: **string**. The short name of the sequence.
    - *comment*: **string**.  A comment.
    - *uuid*: **string**. The UUID of this sequence.
    - *projectUuid*: **string**. The UUID of the project.
    - *order*: **int**. Order of the sequence in the project.
- *shots*:  **array of object**. The list of shots. This shot object contains these properties:
    - *name*: **string**.  The name of the shot.
    - *shortName*: **string**. The short name of the shot.
    - *comment*: **string**. A comment.
    - *duration*: **float**. The duration of the shot in seconds.
    - *order*: **integer**. The order of the shot in the sequence.
    - *sequenceUuid*: **string**. The UUID of the containing sequence.
    - *uuid*: **string**. The UUID of this shot.
    - *assetUuids*: **array** of **string**. The list of the asset uuid assigned to this shot.
    - *statusHistory*: **array** of **object**. The status history for this asset. Each object represents a past status, and contains these properties:
        - *uuid*: **string**. The UUID of this status.
        - *completionRatio*: **int**. The completion ratio, a percentage in the range `[0-100]`.
        - *comment*: **string**. A comment.
        - *version*: **int**. The version.
        - *date*: **string**. The date and time of the status, formatted as `yyyy-MM-dd hh:mm:ss`.
        - *userUuid*: **string**. The UUID of the user who set this status.
        - *stateUuid*: **string**. The UUID of the associated state.
        - *stepUuid*: **string**. The UUID of the associated step.
        - *assignedUserUuid*: **string**. The UUID of the user assigned to this task.
        - *published*: **int**. The whether the shot has been published, `0` or `1`.
        - *timeSpent*: **int**. The time spent working on this shot, in seconds.
        - *difficulty*: **string**. The difficulty, one of `veryEasy`, `easy`, `medium`, `hard` or `veryHard`.
        - *estimation*: **float**. The estimation, in days. If this is a negative number, it means the estimation has to be automatically computed.
        - *shotUuid*: **string**. The UUID of the associated shot.
- *schedule*: **array** of **object**. The list of schedule entries. Each entry is an object containing these properties:
    - *uuid*: **string**. The UUID of this entry.
    - *date*: **string**. The date of the entry, formatted as `yyyy-MM-dd hh:mm:ss`.
    - *comment*: **string**. A comment.
    - *userUuid*: **string**. The UUID of the associated user.
    - *stepUuid*: **string**. The UUID of the associated step.
- *scheduleComments*: **array** of **object**. The list of schedule comments. Each comment is an object containing these properties:
    - *uuid*: **string**. The UUID of this comment.
    - *date*: **string**. The date of the comment, formatted as `yyyy-MM-dd hh:mm:ss`.
    - *comment*: **string**. A comment.
    - *color*: **string**. A color in hex format.
    - *projectUuid*: **string**. The UUID of the associated project.

**Reply body**:

```json
{
    "accepted": true,
    "query": "getProject",
    "success": true,
    "message": "Project retrieved",
    "content": {
            "name": "Mythomen",
            "shortName": "Mytho",
            "comment": "A comment",
            "uuid": "2d7d7e01-671c-11e7-a78f-4ccc6a288527",
            "folderPath": "PROJECTS/MYTHO",
            "framerate": 25.0,
            "width": 1920,
            "height": 1080,
            "aspectRatio": 1.78,
            "deadline": "2021-06-23",
            "users": ["uuid123", "uuid456"],
            "pipeFiles": [
                {
                    "uuid":"uuid123",
                    "shortName":"geoPipe",
                    "comment":"A comment",
                    "fileTypeUuid":"uuid789",
                    "colorSpaceUuid":"uuid333",
                    "projectUuid":"2d7d7e01-671c-11e7-a78f-4ccc6a288527"
                },
                {
                    "uuid":"uui456",
                    "shortName":"shaderPipe",
                    "comment":"A comment",
                    "fileTypeUuid":"uuid789",
                    "colorSpaceUuid":"uuid333",
                    "projectUuid":"2d7d7e01-671c-11e7-a78f-4ccc6a288527"
                }
            ],
            "steps": [
                {
                    "uuid":"uuid123",
                    "shortName":"ANIM",
                    "comment":"A comment",
                    "name":"Animation",
                    "type":"shot",
                    "order":2,
                    "color":"#123456",
                    "estimationMethod":"second",
                    "estimationVeryEasy":0.2,
                    "estimationEasy":0.5,
                    "estimationMedium":1.0,
                    "estimationHard":2.0,
                    "estimationVeryHard":3.0,
                    "multiplyGroupUuid":"uuid789",
                    "projectUuid":"2d7d7e01-671c-11e7-a78f-4ccc6a288527"
                },
                {
                    "uuid":"uuid456",
                    "shortName":"RIG",
                    "comment":"A comment",
                    "name":"Rigging",
                    "type":"asset",
                    "order":1,
                    "color":"#789456",
                    "estimationMethod":"shot",
                    "estimationVeryEasy":0.2,
                    "estimationEasy":0.5,
                    "estimationMedium":1.0,
                    "estimationHard":2.0,
                    "estimationVeryHard":3.0,
                    "multiplyGroupUuid":"",
                    "projectUuid":"2d7d7e01-671c-11e7-a78f-4ccc6a288527"
                }
            ],
            "pipes": [
                {
                    "uuid":"uuid123",
                    "inputStepUuid":"uuid456",
                    "outputStepUuid":"uuid789",
                    "pipeFiles": ["uuid111","uuid222"],
                    "projectUuid":"2d7d7e01-671c-11e7-a78f-4ccc6a288527"
                },
                {
                    "uuid":"uuid123",
                    "inputStepUuid":"uuid456",
                    "outputStepUuid":"uuid789",
                    "pipeFiles": ["uuid111","uuid222"],
                    "projectUuid":"2d7d7e01-671c-11e7-a78f-4ccc6a288527"
                }
            ],
            "assetGroups": [
                {
                    "uuid":"uuid123",
                    "shortName":"CHARS",
                    "comment":"A Comment",
                    "name": "Characters",
                    "projectUuid":"2d7d7e01-671c-11e7-a78f-4ccc6a288527"
                },
                {
                    "uuid":"uuid456",
                    "shortName":"PROPS",
                    "comment":"A Comment",
                    "name": "Props",
                    "projectUuid":"2d7d7e01-671c-11e7-a78f-4ccc6a288527"
                }
            ],
            "assets": [
                {
                    "uuid":"uuid456",
                    "shortName":"TRISTAN",
                    "name":"Tristan",
                    "comment":"A main character",
                    "tags": "main, male, warrior",
                    "assetGroupUuid": "uuid123",
                    "statusHistory": [
                        {
                            "uuid":"uuid123",
                            "completionRatio": 0,
                            "comment":"To do",
                            "version": 0,
                            "date":"2021-06-23 12:35:23",
                            "userUuid":"uuid111",
                            "stateUuid":"uuid789",
                            "stepUuid":"uuid777",
                            "assignedUserUuid":"uuid888",
                            "published": 0,
                            "timeSpent": 3600,
                            "difficulty":"hard",
                            "estimation": 12.5,
                            "assetUuid":"uuid456"
                        }
                    ]
                }
            ],
            "sequences": [
                {
                    "uuid": "uuid456",
                    "shortName": "S001",
                    "name": "Sequence 001 - At sea",
                    "comment": "A comment",
                    "order": 1,
                    "projectUuid": "2d7d7e01-671c-11e7-a78f-4ccc6a288527",
                }
            ],
            "shots": [
                {
                    "uuid":"uuid456",
                    "shortName":"001",
                    "name":"Shot 001",
                    "comment":"A comment",
                    "duration": 3.2,
                    "order": 1,
                    "sequenceUuid": "uuid123",
                    "assetUuids": ["uuid111","uuid222"],
                    "statusHistory": [
                        {
                            "uuid":"uuid123",
                            "completionRatio": 0,
                            "comment":"To do",
                            "version": 0,
                            "date":"2021-06-23 12:35:23",
                            "userUuid":"uuid111",
                            "stateUuid":"uuid789",
                            "stepUuid":"uuid777",
                            "assignedUserUuid":"uuid888",
                            "published": 0,
                            "timeSpent": 3600,
                            "difficulty":"hard",
                            "estimation": 12.5,
                            "assetUuid":"uuid456"
                        }
                    ]
                }
            ],
            "schedule": [
                {
                    "uuid":"uuid23",
                    "date":"2021-06-13 12:00:00",
                    "comment":"A comment",
                    "userUuid":"uuid456",
                    "stepUuid":"uuid789",
                },
                {
                    "uuid":"uuid45",
                    "date":"2021-06-14 00:00:00",
                    "comment":"A comment",
                    "userUuid":"uuid456",
                    "stepUuid":"uuid789",
                }
            ],
            "scheduleComments": [
                {
                    "uuid":"uuid23",
                    "date":"2021-06-13 12:00:00",
                    "comment":"A comment",
                    "color":"#2345269",
                    "projectUuid":"uuid789",
                },
                {
                    "uuid":"uuid45",
                    "date":"2021-06-14 00:00:00",
                    "comment":"A comment",
                    "color":"#FF12BB",
                    "projectUuid":"uuid789",
                }
            ],
        }
}
```

## getProjects

`http://your.server/ramses/?getProjects`

Retrieves the list of all projects.

**Query attributes:**

- *version*: **string**. The version of the client.
- *token*: **string**. The session token returned by [*login*](general.md#login).

**Reply content:**

The server replies with an array of project information. Each project is an object with the following attributes:

- *name*: **string**. The name of the user.
- *shortName*: **string**. The username.
- *comment*: **string**. A comment for the project.
- *uuid*: **string**. The Universal Unique Identifier of this user.
- *folderPath*: **string**. The user folder path.
- *framerate*: **float**. The framerate of the project.
- *width*: **integer**. The width of the project in pixels.
- *height*: **integer**. The height of the project in pixels.
- *aspectRatio*: **float**. The aspectRatio of the project.
- *deadline*: **string**. The date of the deadline, formatted as `yyyy-MM-dd`.
- *users*: **array** of **string**. The list of user Uuid assigned to this project.

The project objects also contain empty lists for the project content (shots, assets, etc.). To retrieve the content, use `getProject` for the given project.

**Reply body**:

```json
{
    "accepted": true,
    "query": "getProjects",
    "success": true,
    "message": "Project list retrieved.",
    "content":  [
        {
            "name": "Mythomen",
            "shortName": "Mytho",
            "comment": "A comment",
            "uuid": "2d7d7e01-671c-11e7-a78f-4ccc6a288527",
            "folderPath": "PROJECTS/MYTHO",
            "framerate": 25.0,
            "width": 1920,
            "height": 1080,
            "aspectRatio": 1.78,
            "deadline": "2021-06-23",
            "users": ["uuid123", "uuid456"],
            "pipeFiles": [],
            "steps": [],
            "pipes": [],
            "assetGroups": [],
            "assets": [],
            "sequences": [],
            "shots": [],
            "schedule": []
        },
        {
            "name": "Purpleboy",
            "shortName": "PPB",
            "comment": "A comment",
            "uuid": "2d7d7e01-671c-11e7-a78f-4ccc6a288527",
            "folderPath": "PROJECTS/PPB",
            "framerate": 24.0,
            "width": 2048,
            "height": 858,
            "aspectRatio": 2.39,
            "deadline": "2020-05-15",
            "users": ["uuid123", "uuid456"],
            "pipeFiles": [],
            "steps": [],
            "pipes": [],
            "assetGroups": [],
            "assets": [],
            "sequences": [],
            "shots": [],
            "schedule": []
        }
    ]
}
```

## updateProject

`http://your.server/ramses/?updateProject`

Updates project information.

**Query attributes:**

- *name*: **string**. The new (or current for no change) name.
- *shortName*: **string**. The new (or current for no change) shortName.
- *comment*: **string**. The new comment.
- *folderPath*: **string** (optionnal). The path for the project folder.
- *uuid*: **string**. The user's Universal Unique Identifier.
- *version*: **string**. The version of the client.
- *token*: **string**. The session token returned by [*login*](general.md#login)

**Reply content:**

Empty

**Reply body**:

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

`http://your.server/ramses/?removeProject`

Removes a project from the database.

**Query attributes:**

- *uuid*: **string**. The project's Universal Unique Identifier.
- *token*: **string**. The session token returned by [*login*](general.md#login).

**Reply content:**

Empty

**Reply body**:

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

`http://your.server/ramses/?assignStep`

Assigns a template step to a project

**Query attributes:**

- *stepUuid*: **string**. The template step's Universal Unique Identifier.
- *projectUuid*: **string**. The project's Universal Unique Identifier.
- *version*: **string**. The version of the client.
- *token*: **string**. The session token returned by [*login*](general.md#login).

**Reply content:**

Empty

**Reply body**:

```json
{
    "accepted": true,
    "query": "assignStep",
    "success": true,
    "message": "Step uuid123 associated with project uuid456.",
    "content": { }
}
```

## assignUser

`http://your.server/ramses/?assignUser`

Assigns a user to a project

**Query attributes:**

- *userUuid*: **string**. The user's Universal Unique Identifier.
- *projectUuid*: **string**. The project's Universal Unique Identifier.
- *version*: **string**. The version of the client.
- *token*: **string**. The session token returned by [*login*](general.md#login).

**Reply content:**

Empty

**Reply body**:

```json
{
    "accepted": true,
    "query": "assignUser",
    "success": true,
    "message": "User assigned to project.",
    "content": { }
}
```

## unassignUser

`http://your.server/ramses/?unassignUser`

Unassigns a user to a project

**Query attributes:**

- *userUuid*: **string**. The user's Universal Unique Identifier.
- *projectUuid*: **string**. The project's Universal Unique Identifier.
- *version*: **string**. The version of the client.
- *token*: **string**. The session token returned by [*login*](general.md#login).

**Reply content:**

Empty

**Reply body**:

```json
{
    "accepted": true,
    "query": "unassignUser",
    "success": true,
    "message": "User unassigned from project.",
    "content": { }
}
```
