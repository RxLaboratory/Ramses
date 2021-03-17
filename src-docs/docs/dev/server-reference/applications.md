# Queries for managing applications

[TOC]

## createApplication

Creates a new application e in the database.

**Query attributes:**

- *name*: **string**. The new name.
- *shortName*: **string**. The new shortName.
- *executableFilePath*: **string**. The path to the main executable binary of the application. Don't forget to encode `/` and `\` in the URL!
- *uuid*: **string** (optionnal). The file type's Universal Unique Identifier.
- *token*: **string**. The session token returned with [*login*](general.md#login).

**Reply content:**

Empty

**Examples:**

Query:  
`http://your.server/ramses/?createFileType&name=Autodesk Maya&shortName=Maya&executableFilePath=%2Fusr%2Fbin%2Fmaya&uuid=123&token=123`

Reply:

```json
{
    "accepted": true,
    "query": "createApplication",
    "success": true,
    "message": "Application \"Maya\" created.",
    "content": { }
}
```

## getApplications

Retrieves the list of all applications.

**Query attributes:**

None

**Reply content:**

The server replies an array of application information. Each application is an object with the following attributes:

- *name*: **string**. The name of the application.
- *shortName*: **string**. The short name of the application.
- *executableFilePath*: **string**. The path to the executable binary of the application.
- *uuid*: **string**. The Universal Unique Identifier of this application.

**Examples:**

Query:  
`http://your.server/ramses/?getApplications&token=123`

Reply:

```json
{
    "accepted": true,
    "query": "getApplications",
    "success": true,
    "message": "Applications list retrieved.",
    "content":  [
        {
            "name": "Autodesk Maya",
            "shortName": "Maya",
            "executableFilePath": "/usr/bin/maya",
            "uuid": "2d7d7e01-671c-11e7-a78f-4ccc6a288527"
        },
        {
            "name": "Blender",
            "shortName": "Blender",
            "executableFilePath": "/usr/bin/blender",
            "uuid": "2d7d7e01-671c-11e7-a78f-4ccc6a288527"
        }
    ]
}
```

## updateApplication

Update application info in the database.

**Query attributes:**

- *name*: **string**. The name of the user.
- *shortName*: **string**. The username.
- *uuid*: **string**. The Universal Unique Identifier of this file type.
- *executableFilePath*: **string**. The path to the executable binary of the application. Don't forget to encode `/` and `\` in the URL!
- *token*: **string**. The session token returned with [*login*](general.md#login)

**Reply content:**

Empty

**Examples:**

Query:  
`http://your.server/ramses/?updateApplication&name=Autodesk Maya&shortName=Maya&executableFilePath=%2Fusr%2Fbin%2Fmaya&uuid=123&token=123`

Reply:

```json
{
    "accepted": true,
    "query": "updateApplication",
    "success": true,
    "message": "Application \"Maya\" updated.",
    "content": { }
}
```

## removeApplication

Removes an application from the database.

**Query attributes:**

- *uuid*: **string**. The application's Universal Unique Identifier.
- *token*: **string**. The session token returned with [*login*](general.md#login).

**Reply content:**

Empty

**Examples:**

Query:  
`http://your.server/ramses/?removeApplication&uuid=123&token=123`

Reply:

```json
{
    "accepted": true,
    "query": "removeApplication",
    "success": true,
    "message": "Application uuid123 removed.",
    "content": { }
}
```


## assignFileType

Assigns a file type to an application.

**Query attributes:**

- *applicationUuid*: **string**. The Applcation's Universal Unique Identifier.
- *fileTypeUuid*: **string**. The File Type's Universal Unique Identifier.
- *type*: **string** (optional). The type, an enumerated value in `native`, `import`, `export`. Default is `native`.
- *token*: **string**. The session token returned with [*login*](general.md#login).

**Reply content:**

Empty

**Examples:**

Query:  
`http://your.server/ramses/?assignFileType&applicationUuid=123&fileTypeUuid=456&type=import&token=123`

Reply:

```json
{
    "accepted": true,
    "query": "assignFileType",
    "success": true,
    "message": "File type assigned to application as import type.",
    "content": { }
}
```

## unassignFileType

Unassigns a file type from an application.

**Query attributes:**

- *applicationUuid*: **string**. The Applcation's Universal Unique Identifier.
- *fileTypeUuid*: **string**. The File Type's Universal Unique Identifier.
- *type*: **string** (optional). The type, an enumerated value in `native`, `import`, `export`. If left empty, the file type is removed for all types.
- *token*: **string**. The session token returned with [*login*](general.md#login).

**Reply content:**

Empty

**Examples:**

Query:  
`http://your.server/ramses/?unassignUser&applicationUuid=123&fileTypeUuid=456&token=123`

Reply:

```json
{
    "accepted": true,
    "query": "unassignFileType",
    "success": true,
    "message": "File type unassigned from application.",
    "content": { }
}
```