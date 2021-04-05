# Queries for managing shots

!!! hint
    There is no method to retrieve shots directly; shots are returned by the project method [`getProjects`](projects.md#getprojects)

[TOC]

## createShot

Creates a new shot in the database and assigns it to a sequence group.

**Query attributes:**

- *name*: **string**. The new name.
- *shortName*: **string**. The new shortName.
- *sequenceUuid*: **string**. The UUID of the sequence to assign the shot to.
- *duration*: **float** (optional). The duration of the shot in seconds.
- *order*: **integer** (optional). The order at which to insert the new shot. If omitted, the shot is added at the end of the list.
- *uuid*: **string** (optional). The shot's Universal Unique Identifier.
- *token*: **string**. The session token returned with [*login*](general.md#login).

**Reply content:**

Empty

**Examples:**

Query:  
`http://your.server/ramses/?createShot&name=s001&shortName=001&sequenceUuid=123&duration=2.12&order=12&uuid=123&token=123`

Reply:

```json
{
    "accepted": true,
    "query": "createShot",
    "success": true,
    "message": "Shot \"001\" created.",
    "content": { }
}
```

## updateShot

Update shot info in the database.

**Query attributes:**

- *name*: **string**. The new name.
- *shortName*: **string**. The new shortName.
- *sequenceUuid*: **string** (optional). The UUID of the sequence to reassign the shot to.
- *duration*: **float** (optional). The new duration of the shot, in seconds.
- *uuid*: **string**. The shot's Universal Unique Identifier.
- *token*: **string**. The session token returned with [*login*](general.md#login).

**Reply content:**

Empty

**Examples:**

Query:  
`http://your.server/ramses/?updateShot&name=S003&shortName=003&sequenceUuid=123&duration=3.36&uuid=123&token=123`

Reply:

```json
{
    "accepted": true,
    "query": "updateShot",
    "success": true,
    "message": "Shot \"003\" updated.",
    "content": { }
}
```

## moveShot

Moves a shot in the list.

**Query attributes:**

- *order*: **intenger**. The new order for the shot.
- *uuid*: **string**. The shot's Universal Unique Identifier.
- *token*: **string**. The session token returned with [*login*](general.md#login).

**Reply content:**

Empty

**Examples:**

Query:  
`http://your.server/ramses/?moveShot&order=6&uuid=123&token=123`

Reply:

```json
{
    "accepted": true,
    "query": "moveShot",
    "success": true,
    "message": "Shot moved.",
    "content": { }
}
```

## removeShot

Removes a shot from the database.

**Query attributes:**

- *uuid*: **string**. The shot's Universal Unique Identifier.
- *token*: **string**. The session token returned with [*login*](general.md#login).

**Reply content:**

Empty

**Examples:**

Query:  
`http://your.server/ramses/?removeShot&uuid=123&token=123`

Reply:

```json
{
    "accepted": true,
    "query": "removeShot",
    "success": true,
    "message": "Shot removed.",
    "content": { }
}
```
