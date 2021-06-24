# Queries for managing assets

!!! hint
    There is no method to retrieve assets directly; assets are returned by the project method [`getProjects`](projects.md#getprojects)

[TOC]

## createAsset

Creates a new asset in the database and assigns it to an asset group.

**Query attributes:**

- *name*: **string**. The new name.
- *shortName*: **string**. The new shortName.
- *assetGroupUuid*: **string**. The UUID of the asset group to assign the asset to.
- *tags*: **string**. A comma separated list of tags.
- *uuid*: **string** (optional). The asset's Universal Unique Identifier.
- *token*: **string**. The session token returned with [*login*](general.md#login).

**Reply content:**

Empty

**Examples:**

Query:  
`http://your.server/ramses/?createAsset&name=Tristan&shortName=TRI&tags=Main, Warrior&assetGroupUuid=456&uuid=123&token=123`

Reply:

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

Update asset info in the database.

**Query attributes:**

- *name*: **string**. The new name.
- *shortName*: **string**. The new shortName.
- *comment*: **string**. The new comment.
- *assetGroupUuid*: **string**. The UUID of the asset group to assign the asset to.
- *tags*: **string**. A comma separated list of tags.
- *uuid*: **string** (optional). The asset's Universal Unique Identifier.
- *token*: **string**. The session token returned with [*login*](general.md#login).

**Reply content:**

Empty

**Examples:**

Query:  
`http://your.server/ramses/?updateAsset&name=Tristan&shortName=TRI&tags=Main, Warrior&assetGroupUuid=456&comment=A comment&uuid=123&token=123`

Reply:

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

Removes an asset group from the database.

**Query attributes:**

- *uuid*: **string**. The asset's Universal Unique Identifier.
- *token*: **string**. The session token returned with [*login*](general.md#login).

**Reply content:**

Empty

**Examples:**

Query:  
`http://your.server/ramses/?removeAssetGroup&uuid=123&token=123`

Reply:

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

Sets a new status for an asset

**Query attributes:**

- *uuid*: **string**. The status' Universal Unique Identifier.
- *assetUuid*: **string**. The asset's Universal Unique Identifier.
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
`http://your.server/ramses/?setAssetStatus&uuid=123&assetUuid=123&completionRatio=50&userUuid=123&stateUuid=123&comment=A nice comment&version=12&stepUuid=123&token=123`

Reply:

```json
{
    "accepted": true,
    "query": "setAssetStatus",
    "success": true,
    "message": "Asset status updated.",
    "content": { }
}
```