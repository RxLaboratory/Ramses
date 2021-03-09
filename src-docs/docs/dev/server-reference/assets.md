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
- *assetGroupUuid*: **string**. The UUID of the asset group to assign the asset to.
- *tags*: **string**. A comma separated list of tags.
- *uuid*: **string** (optional). The asset's Universal Unique Identifier.
- *token*: **string**. The session token returned with [*login*](general.md#login).

**Reply content:**

Empty

**Examples:**

Query:  
`http://your.server/ramses/?updateAsset&name=Tristan&shortName=TRI&tags=Main, Warrior&assetGroupUuid=456&uuid=123&token=123`

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
