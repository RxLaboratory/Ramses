# Queries for managing steps

!!! note
    The API also provides an access to "template steps" which can be assigned to projects. The calls are exactly the same except you have to insert the "Template" word (e.g. use `createTemplateStep` instead of `createStep`)

!!! hint
    There is no method to retrive steps directly; steps are returned by the project method [`getProjects`](projects.md#getprojects)

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

## updateStep

Update step info in the database.

**Query attributes:**

- *name*: **string**. The new (or current for no change) name.
- *shortName*: **string**. The new (or current for no change) shortName.
- *comment*: **string**. The new comment.
- *type*: **string** (optionnal). The type of the step. One of `asset`, `shot`, `pre`, or `post`.
- *order*: **int** (optionnal). The order of the step in the production pipeline.
- *uuid*: **string**. The user's Universal Unique Identifier.
- *token*: **string**. The session token returned with [*login*](general.md#login)

**Reply content:**

Empty

**Examples:**

Query:  
`http://your.server/ramses/?updateStep&name=Rigging&shortName=RIG&comment=A comment&uuid=123&token=123`

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

## setStepOrder

Moves a step in the list. This method changes only the order value of the given step.

**Query attributes:**

- *order*: **intenger**. The new order for the shot.
- *uuid*: **string**. The shot's Universal Unique Identifier.
- *token*: **string**. The session token returned with [*login*](general.md#login).

**Reply content:**

Empty

**Examples:**

Query:  
`http://your.server/ramses/?setStepOrder&order=6&uuid=123&token=123`

Reply:

```json
{
    "accepted": true,
    "query": "setStepOrder",
    "success": true,
    "message": "Step moved.",
    "content": { }
}
```

## moveStep

Moves a step in the list. This method will update the order of all the other steps of the same project accordingly.

**Query attributes:**

- *order*: **intenger**. The new order for the shot.
- *uuid*: **string**. The shot's Universal Unique Identifier.
- *token*: **string**. The session token returned with [*login*](general.md#login).

**Reply content:**

Empty

**Examples:**

Query:  
`http://your.server/ramses/?moveStep&order=6&uuid=123&token=123`

Reply:

```json
{
    "accepted": true,
    "query": "moveStep",
    "success": true,
    "message": "Step moved.",
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

## assignUser

Assigns a user to a step.

**Query attributes:**

- *stepUuid*: **string**. The step's Universal Unique Identifier.
- *userUuid*: **string**. The user's Universal Unique Identifier.
- *token*: **string**. The session token returned with [*login*](general.md#login).

**Reply content:**

Empty

**Examples:**

Query:  
`http://your.server/ramses/?assignUser&stepUuid=123&userUuid=456&token=123`

Reply:

```json
{
    "accepted": true,
    "query": "assignUser",
    "success": true,
    "message": "User assigned to step.",
    "content": { }
}
```

## unassignUser

Unassigns a user from a step.

**Query attributes:**

- *stepUuid*: **string**. The step's Universal Unique Identifier.
- *userUuid*: **string**. The user's Universal Unique Identifier.
- *token*: **string**. The session token returned with [*login*](general.md#login).

**Reply content:**

Empty

**Examples:**

Query:  
`http://your.server/ramses/?unassignUser&stepUuid=123&userUuid=456&token=123`

Reply:

```json
{
    "accepted": true,
    "query": "unassignUser",
    "success": true,
    "message": "User unassigned from step.",
    "content": { }
}
```

## assignApplication

Assigns an application to a step.

**Query attributes:**

- *stepUuid*: **string**. The step's Universal Unique Identifier.
- *applicationUuid*: **string**. The application's Universal Unique Identifier.
- *token*: **string**. The session token returned with [*login*](general.md#login).

**Reply content:**

Empty

**Examples:**

Query:  
`http://your.server/ramses/?assignApplication&stepUuid=123&applicationUuid=456&token=123`

Reply:

```json
{
    "accepted": true,
    "query": "assignApplication",
    "success": true,
    "message": "Application assigned to step.",
    "content": { }
}
```

## unassignApplication

Unassigns an application from a step.

**Query attributes:**

- *stepUuid*: **string**. The step's Universal Unique Identifier.
- *applicationUuid*: **string**. The application's Universal Unique Identifier.
- *token*: **string**. The session token returned with [*login*](general.md#login).

**Reply content:**

Empty

**Examples:**

Query:  
`http://your.server/ramses/?unassignApplication&stepUuid=123&applicationUuid=456&token=123`

Reply:

```json
{
    "accepted": true,
    "query": "unassignApplication",
    "success": true,
    "message": "Application unassigned from step.",
    "content": { }
}
```