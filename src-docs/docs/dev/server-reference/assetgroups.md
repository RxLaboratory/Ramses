# Queries for managing asset groups

!!! note
    The API also provides an access to "template asset groups" which can be assigned to projects. The calls are exactly the same except you have to insert the "Template" word (e.g. use `createTemplateAssetGroup` instead of `createAssetGroup`)

!!! hint
    There is no method to retrieve asset groups directly; asset groups are returned by the project method [`getProjects`](projects.md#getprojects)

[TOC]

## createAssetGroup

Creates a new asset group in the database and assigns it to a project.

**Query attributes:**

- *name*: **string**. The new name.
- *shortName*: **string**. The new shortName.
- *projectUuid*: **string**. The UUID of the project to assign the asset group to.
- *uuid*: **string** (optionnal). The asset group's Universal Unique Identifier.
- *token*: **string**. The session token returned with [*login*](general.md#login).

!!! note
    When creating a template asset group with `createTemplateAssetGroup`, omit the *projectUuid* attribute.

**Reply content:**

Empty

**Examples:**

Query:  
`http://your.server/ramses/?createAssetGroup&name=Rigging&shortName=RIG&projectUuid=456&uuid=123&token=123`

Reply:

```json
{
    "accepted": true,
    "query": "createAssetGroup",
    "success": true,
    "message": "Asset Group \"CHAR\" created.",
    "content": { }
}
```

## updateAssetGroup

Update asset group info in the database.

**Query attributes:**

- *name*: **string**. The new (or current for no change) name.
- *shortName*: **string**. The new (or current for no change) shortName.
- *uuid*: **string**. The asset group's Universal Unique Identifier.
- *token*: **string**. The session token returned with [*login*](general.md#login)

**Reply content:**

Empty

**Examples:**

Query:  
`http://your.server/ramses/?updateAssetGroup&name=Rigging&shortName=RIG&uuid=123&token=123`

Reply:

```json
{
    "accepted": true,
    "query": "updateAssetGroup",
    "success": true,
    "message": "Asset Group \"CHAR\" updated.",
    "content": { }
}
```

## removeAssetGroup

Removes an asset group from the database.

**Query attributes:**

- *uuid*: **string**. The asset group's Universal Unique Identifier.
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
    "query": "removeAssetGroup",
    "success": true,
    "message": "Asset Group uuid123 removed.",
    "content": { }
}
```
