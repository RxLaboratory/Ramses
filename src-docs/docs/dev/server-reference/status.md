# Queries for managing status

!!! hint
    - There is no method to retrieve statuses directly; statuses are returned by the project method [`getProjects`](projects.md#getprojects)

    - There is no method to create statuses; statuses are created by the asset or shot methods `setAssetStatus` and `setShotStatus`

[TOC]

## updateStatus

Update status info in the database.

**Query attributes:**

- *completionRatio*: **int** (optional). The completion ratio in the range [0,100].
- *version*: **int** (optional). The version.
- *comment*: **string** (optional). The comment.
- *stateUuid*: **string**. The Universal Unique Identifier of the associated state.
- *uuid*: **string** (optional). The asset's Universal Unique Identifier.
- *token*: **string**. The session token returned with [*login*](general.md#login).

**Reply content:**

Empty

**Examples:**

Query:  
`http://your.server/ramses/?updateStatus&uuid=123&stateUuid=456&comment=The new commment&version=12&completionRatio=50&token=123`

Reply:

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

Removes a status from the database.

**Query attributes:**

- *uuid*: **string**. The status's Universal Unique Identifier.
- *token*: **string**. The session token returned with [*login*](general.md#login).

**Reply content:**

Empty

**Examples:**

Query:  
`http://your.server/ramses/?removeStatus&uuid=123&token=123`

Reply:

```json
{
    "accepted": true,
    "query": "removeStatus",
    "success": true,
    "message": "Status removed.",
    "content": { }
}
```
