![META](authors:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2021;updated:2021/07/12)

# Queries for managing status

!!! hint
    - There is no method to retrieve statuses directly; statuses are returned by the project method [`getProjects`](projects.md#getprojects)

    - There is no method to create statuses; statuses are created by the asset or shot methods `setAssetStatus` and `setShotStatus`

## updateStatus

`http://your.server/ramses/?updateStatus`

Update status info in the database.

**Query attributes:**

- *completionRatio*: **int** (optional). The completion ratio in the range [0,100].
- *version*: **int** (optional). The version.
- *comment*: **string** (optional). The comment.
- *stateUuid*: **string**. The Universal Unique Identifier of the associated state.
- *published*: **int**. `0` if the corresponding asset/shot is not published, `1` if it is.
- *assignedUserUuid*: **string**. The Universal Unique Identifier of the user assigned to this task.
- *timeSpent*: **int**. The time spent working on this task, in seconds.
- *date*: **string**. The date the status has been modified formatted as `yyyy-MM-dd hh:mm:ss`.
- *estimation*: **float**. The estimation in days.
- *difficulty*: **string**. The difficulty, one of `veryEasy`, `easy`, `medium`, `hard` or `veryHard`.
- *uuid*: **string** (optional). The asset's Universal Unique Identifier.
- *token*: **string**. The session token returned by [*login*](general.md#login).

**Reply content:**

Empty

**Reply body**:

```json
{
    "accepted": true,
    "query": "updateStatus",
    "success": true,
    "message": "Status updated.",
    "content": { }
}
```

## removeStatus

`http://your.server/ramses/?removeStatus`

Removes a status from the database.

**Query attributes:**

- *uuid*: **string**. The status's Universal Unique Identifier.
- *token*: **string**. The session token returned by [*login*](general.md#login).

**Reply content:**

Empty

**Reply body**:

```json
{
    "accepted": true,
    "query": "removeStatus",
    "success": true,
    "message": "Status removed.",
    "content": { }
}
```

## setStatusUser

`http://your.server/ramses/?setStatusUser`

Changes the user who's modified this status.

**Query attributes:**

- *uuid*: **string**. The status's Universal Unique Identifier.
- *userUuid*: **string**. The user's Universal Unique Identifier.
- *token*: **string**. The session token returned by [*login*](general.md#login).

**Reply content:**

Empty

**Reply body**:

```json
{
    "accepted": true,
    "query": "setStatusUser",
    "success": true,
    "message": "Status user changed.",
    "content": { }
}
```
