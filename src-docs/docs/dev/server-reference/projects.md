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

## getProjects

Retrieves the list of all projects.

**Query attributes:**

- *token*: **string**. The session token returned with [*login*](general.md#login).

**Reply content:**

The server replies an array of project information. Each project is an object with the following attributes:

- *name*: **string**. The name of the user.
- *shortName*: **string**. The username.
- *uuid*: **string**. The Universal Unique Identifier of this user.
- *folderPath*: **string**. The user folder path.
- *steps*: **array of int**. The list of steps represented by their ids associated with this project.
- *shots*: **array of int**. The list of shots represented by their ids associated with this project.

**Examples:**

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
            "steps": ["uuid123", "uuid456"],
            "shots": ["uuidshot123", "uuidshot456"]
        },
        {
            "name": "Purpleboy",
            "shortName": "PPB",
            "uuid": "2d7d7e01-671c-11e7-a78f-4ccc6a288527",
            "folderPath": "PROJECTS/PPB",
            "steps": ["uuid123", "uuid456"],
            "shots": ["uuidshot789", "uuidshot001"]
        }
    ]
}
```



## updateProject

Updates project information.

**Query attributes:**

- *name*: **string**. The new (or current for no change) name.
- *shortName*: **string**. The new (or current for no change) shortName.
- *folderPath*: **string** (optionnal). The path for the project folder.
- *uuid*: **string**. The user's Universal Unique Identifier.
- *token*: **string**. The session token returned with [*login*](general.md#login)

**Reply content:**

Empty

**Examples:**

Query:  
`http://your.server/ramses/?updateProject&name=Mythomen&shortName=Mytho&uuid=123&token=123`

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