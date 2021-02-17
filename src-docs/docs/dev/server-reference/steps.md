# Queries for managing steps

!!! note
    The API also provides an access to "template steps" which can be assigned to projects. The calls are exactly the same except you have to insert the "Template" word (e.g. use `createTemplateStep` instead of `createStep`)

[TOC]

## createStep

Creates a new step in the database and assigns it to a project.

**Query attributes:**

- *name*: **string**. The new name.
- *shortName*: **string**. The new shortName.
- *projectUuid*: **string**. The UUID of the project to assign the step to.
- *uuid*: **string** (optionnal). The step's Universal Unique Identifier.
- *token*: **string**. The session token returned with [*login*](general.md#login).

!!! note
    When creating a template step with `createTemplateStep`, omit the *projectUuid* attribute.

**Reply content:**

Empty

**Examples:**

Query:  
`http://your.server/ramses/?createStep&name=Rigging&shortName=RIG&projectUuid=456&uuid=123&token=123`

Reply:

```json
{
    "accepted": true,
    "query": "createStep",
    "success": true,
    "message": "Step \"RIG\" created.",
    "content": { }
}
```

## getSteps

Retrieves the list of all steps.

**Query attributes:**

None

**Reply content:**

The server replies an array of step information. Each step is an object with the following attributes:

- *name*: **string**. The name of the user.
- *shortName*: **string**. The username.
- *uuid*: **string**. The Universal Unique Identifier of this user.
- *type*: **string**. The type of the step. One of "pre", "post", "shot", "asset" or "post".

**Examples:**

Query:  
`http://your.server/ramses/?getSteps&token=123`

Reply:

```json
{
    "accepted": true,
    "query": "getProjects",
    "success": true,
    "message": "Projects list retrieved.",
    "content":  [
        {
            "name": "Rigging",
            "shortName": "RIG",
            "uuid": "2d7d7e01-671c-11e7-a78f-4ccc6a288527",
            "type": "asset"
        },
        {
            "name": "Animation",
            "shortName": "ANIM",
            "uuid": "2d7d7e01-671c-11e7-a78f-4ccc6a288527",
            "type": "shot"
        }
    ]
}
```



## updateStep

Update step info in the database.

**Query attributes:**

- *name*: **string**. The new (or current for no change) name.
- *shortName*: **string**. The new (or current for no change) shortName.
- *type*: **string** (optionnal). The type of the step. One of "pre", "post", "shot", "asset" or "post".
- *uuid*: **string**. The user's Universal Unique Identifier.
- *token*: **string**. The session token returned with [*login*](general.md#login)

**Reply content:**

Empty

**Examples:**

Query:  
`http://your.server/ramses/?updateStep&name=Rigging&shortName=RIG&uuid=123&token=123`

Reply:

```json
{
    "accepted": true,
    "query": "updateStep",
    "success": true,
    "message": "Step \"RIG\" updated.",
    "content": { }
}
```

## removeStep

Removes a step from the database.

**Query attributes:**

- *uuid*: **string**. The step's Universal Unique Identifier.
- *token*: **string**. The session token returned with [*login*](general.md#login).

**Reply content:**

Empty

**Examples:**

Query:  
`http://your.server/ramses/?removeStep&uuid=123&token=123`

Reply:

```json
{
    "accepted": true,
    "query": "removeStep",
    "success": true,
    "message": "Step uuid123 removed.",
    "content": { }
}
```
