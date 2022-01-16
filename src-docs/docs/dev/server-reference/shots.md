![META](authors:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2021-2022;updated:2022/01/16)

# Queries for managing shots

!!! hint
    There is no method to retrieve shots directly; shots are returned by the project method [`getProjects`](projects.md#getprojects)

## createShot

`http://your.server/ramses/?createShot`

Creates a new shot in the database and assigns it to a sequence group.

**Query attributes:**

- *name*: **string**. The new name.
- *shortName*: **string**. The new shortName.
- *sequenceUuid*: **string**. The UUID of the sequence to assign the shot to.
- *duration*: **float** (optional). The duration of the shot in seconds.
- *order*: **integer** (optional). The order at which to insert the new shot. If omitted, the shot is added at the end of the list.
- *uuid*: **string** (optional). The shot's Universal Unique Identifier.
- *version*: **string**. The version of the client.
- *token*: **string**. The session token returned by [*login*](general.md#login).

**Reply content:**

Empty

**Reply body**:

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

`http://your.server/ramses/?updateShot`

Update shot info in the database.

**Query attributes:**

- *name*: **string**. The new name.
- *shortName*: **string**. The new shortName.
- *comment*: **string**. The new comment.
- *sequenceUuid*: **string** (optional). The UUID of the sequence to reassign the shot to.
- *duration*: **float** (optional). The new duration of the shot, in seconds.
- *uuid*: **string**. The shot's Universal Unique Identifier.
- *version*: **string**. The version of the client.
- *token*: **string**. The session token returned by [*login*](general.md#login).

**Reply content:**

Empty

**Reply body**:

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

`http://your.server/ramses/?setShotOrder`

Moves a shot in the list. This method changes only the order value of the given shot.

**Query attributes:**

- *order*: **intenger**. The new order for the shot.
- *uuid*: **string**. The shot's Universal Unique Identifier.
- *version*: **string**. The version of the client.
- *token*: **string**. The session token returned by [*login*](general.md#login).

**Reply content:**

Empty

**Reply body**:

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

`http://your.server/ramses/?moveShot`

Moves a shot in the list. This method will update the order of all the other shots of the same project accordingly.

**Query attributes:**

- *order*: **intenger**. The new order for the shot.
- *uuid*: **string**. The shot's Universal Unique Identifier.
- *version*: **string**. The version of the client.
- *token*: **string**. The session token returned by [*login*](general.md#login).

**Reply content:**

Empty

**Reply body**:

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

`http://your.server/ramses/?removeShot`

Removes a shot from the database.

**Query attributes:**

- *uuid*: **string**. The shot's Universal Unique Identifier.
- *version*: **string**. The version of the client.
- *token*: **string**. The session token returned by [*login*](general.md#login).

**Reply content:**

Empty

**Reply body**:

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

`http://your.server/ramses/?setShotStatus`

Sets a new status for a shot

**Query attributes:**

- *uuid*: **string**. The status' Universal Unique Identifier.
- *shotUuid*: **string**. The shot's Universal Unique Identifier.
- *completionRation*: **int**. The current completion ratio in the range `[0, 100]`.
- *userUuid*: **string**. The uuid of the user setting the status.
- *stateUuid*: **string**. The uuid of the associated state.
- *comment*: **string**. A comment.
- *version*: **int**. The current version.
- *stepUuid*: **string**. The uuid of the associated step.
- *assignedUserUuid*: **string**. The uuid of the assigned user for this task.
- *version*: **string**. The version of the client.
- *token*: **string**. The session token returned with [*login*](general.md#login).

**Reply content:**

Empty

**Reply body**:

```json
{
    "accepted": true,
    "query": "setShotStatus",
    "success": true,
    "message": "Shot status updated.",
    "content": { }
}
```

## assignAsset

`http://your.server/ramses/?assignAsset`

Assigns an asset to the shot.

**Query attributes:**

- *assetUuid*: **string**. The asset's Universal Unique Identifier.
- *uuid*: **string**. The shot's Universal Unique Identifier.
- *version*: **string**. The version of the client.
- *token*: **string**. The session token returned by [*login*](general.md#login).

**Reply content:**

Empty

**Reply body**:

```json
{
    "accepted": true,
    "query": "assignAsset",
    "success": true,
    "message": "Asset assigned.",
    "content": { }
}
```

## unassignAsset

`http://your.server/ramses/?unassignAsset`

Unassigns  an asset from the shot.

**Query attributes:**

- *assetUuid*: **string**. The asset's Universal Unique Identifier.
- *uuid*: **string**. The shot's Universal Unique Identifier.
- *version*: **string**. The version of the client.
- *token*: **string**. The session token returned by [*login*](general.md#login).

**Reply content:**

Empty

**Reply body**:

```json
{
    "accepted": true,
    "query": "unassignAsset",
    "success": true,
    "message": "Asset unassigned..",
    "content": { }
}
```