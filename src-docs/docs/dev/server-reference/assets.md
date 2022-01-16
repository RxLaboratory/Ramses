![META](authors:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2021-2022;updated:2022/01/16)

# Queries for managing assets

!!! hint
    There is no method to retrieve assets directly; assets are returned by the project method [`getProjects`](projects.md#getprojects)

## createAsset

`http://your.server/ramses/?createAsset`

Creates a new asset in the database and assigns it to an asset group.

**Query attributes:**

- *name*: **string**. The new name.
- *shortName*: **string**. The new shortName.
- *assetGroupUuid*: **string**. The UUID of the asset group to assign the asset to.
- *tags*: **string**. A comma separated list of tags.
- *uuid*: **string** (optional). The asset's Universal Unique Identifier.
- *version*: **string**. The version of the client.
- *token*: **string**. The session token returned by [*login*](general.md#login).

**Reply content:**

Empty

**Reply body**:

```json
{
    "accepted": true,
    "query": "createAsset",
    "success": true,
    "message": "Asset \"TRI\" created.",
    "content": { }
}
```

## updateAsset

`http://your.server/ramses/?updateAsset`

Update asset info in the database.

**Query attributes:**

- *name*: **string**. The new name.
- *shortName*: **string**. The new shortName.
- *comment*: **string**. The new comment.
- *assetGroupUuid*: **string**. The UUID of the asset group to assign the asset to.
- *tags*: **string**. A comma separated list of tags.
- *uuid*: **string** (optional). The asset's Universal Unique Identifier.
- *version*: **string**. The version of the client.
- *token*: **string**. The session token returned by [*login*](general.md#login).

**Reply content:**

Empty

**Reply body**:

```json
{
    "accepted": true,
    "query": "updateAsset",
    "success": true,
    "message": "Asset \"TRI\" updated.",
    "content": { }
}
```

## removeAsset

`http://your.server/ramses/?removeAsset`

Removes an asset group from the database.

**Query attributes:**

- *uuid*: **string**. The asset's Universal Unique Identifier.
- *version*: **string**. The version of the client.
- *token*: **string**. The session token returned by [*login*](general.md#login).

**Reply content:**

Empty

**Reply body**:

```json
{
    "accepted": true,
    "query": "removeAsset",
    "success": true,
    "message": "Asset uuid123 removed.",
    "content": { }
}
```

## setAssetStatus

`http://your.server/ramses/?setAssetStatus`

Sets a new status for an asset

**Query attributes:**

- *uuid*: **string** (optionnal). The new status' Universal Unique Identifier.
- *assetUuid*: **string**. The asset's Universal Unique Identifier.
- *assignedUserUuid*: **string**. The Universal Unique Identifier of the user assigned to this task.
- *userUuid*: **string**. The uuid of the user setting the status.
- *stateUuid*: **string**. The uuid of the associated state.
- *completionRation*: **int**. The current completion ratio in the range [0, 100].
- *comment*: **string**. A comment.
- *version*: **int**. The current version.
- *stepUuid*: **string**. The uuid of the associated step.
- *version*: **string**. The version of the client.
- *token*: **string**. The session token returned by [*login*](general.md#login).

**Reply content:**

Empty

**Reply body**:

```json
{
    "accepted": true,
    "query": "setAssetStatus",
    "success": true,
    "message": "Asset status updated.",
    "content": { }
}
```