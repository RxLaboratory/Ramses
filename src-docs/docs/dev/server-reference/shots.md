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
- *comment*: **string**. The new comment.
- *sequenceUuid*: **string** (optional). The UUID of the sequence to reassign the shot to.
- *duration*: **float** (optional). The new duration of the shot, in seconds.
- *uuid*: **string**. The shot's Universal Unique Identifier.
- *token*: **string**. The session token returned with [*login*](general.md#login).

**Reply content:**

Empty

**Examples:**

Query:  
`http://your.server/ramses/?updateShot&name=S003&shortName=003&sequenceUuid=123&duration=3.36&uuid=123&comment=A comment&token=123`

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

## setShotOrder

Moves a shot in the list. This method changes only the order value of the given shot.

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
    "query": "setShotOrder",
    "success": true,
    "message": "Shot moved.",
    "content": { }
}
```

## moveShot

Moves a shot in the list. This method will update the order of all the other shots of the same project accordingly.

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

## setShotStatus

Sets a new status for a shot

**Query attributes:**

- *uuid*: **string**. The status' Universal Unique Identifier.
- *shotUuid*: **string**. The shot's Universal Unique Identifier.
- *completionRation*: **int** (optionnal). The current completion ratio in the range [0, 100].
- *userUuid*: **string** (optionnal). The uuid of the user setting the status. Will use the currently connected user if not provided.
- *stateUuid*: **string**. The uuid of the associated state.
- *comment*: **string** (optionnal). A comment.
- *version*: **int** (optionnal). The current version.
- *stepUuid*: **string**. The uuid of the associated step.
- *token*: **string**. The session token returned with [*login*](general.md#login).

**Reply content:**

Empty

**Examples:**

Query:  
`http://your.server/ramses/?setAssetStatus&uuid=123&shotUuid=123&completionRatio=50&userUuid=123&stateUuid=123&comment=A nice comment&version=12&stepUuid=123&token=123`

Reply:

```json
{
    "accepted": true,
    "query": "setShotStatus",
    "success": true,
    "message": "Shot status updated.",
    "content": { }
}
```