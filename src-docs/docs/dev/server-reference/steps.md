![META](authors:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2021;updated:2022/01/16)

# Queries for managing steps

!!! note
    The *API* also provides an access to "template steps" which can be assigned to projects. The calls are exactly the same except you have to insert the "Template" word (e.g. use `createTemplateStep` instead of `createStep`)

!!! hint
    There is no method to retrive steps directly; steps are returned by the project method [`getProjects`](projects.md#getprojects)

## createStep

`http://your.server/ramses/?createStep`

Creates a new step in the database and assigns it to a project.

**Query attributes:**

- *name*: **string**. The new name.
- *shortName*: **string**. The new shortName.
- *projectUuid*: **string**. The UUID of the project to assign the step to.
- *uuid*: **string** (optionnal). The step's Universal Unique Identifier.
- *version*: **string**. The version of the client.
- *token*: **string**. The session token returned by [*login*](general.md#login).

!!! note
    When creating a template step with `createTemplateStep`, omit the *projectUuid* attribute.

**Reply content:**

Empty

**Reply body**:

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

`http://your.server/ramses/?updateStep`

Update step info in the database.

**Query attributes:**

- *name*: **string**. The new (or current for no change) name.
- *shortName*: **string**. The new (or current for no change) shortName.
- *comment*: **string**. The new comment.
- *publishSettings*: **string**. Custom settings to be used when publishing files for this step.
- *type*: **string**. The type of the step. One of `asset`, `shot`, `pre`, or `post`.
- *color*: **string**. The color to use in the UI to display this step.
- *uuid*: **string**. The user's Universal Unique Identifier.
- *version*: **string**. The version of the client.
- *token*: **string**. The session token returned by [*login*](general.md#login)

**Reply content:**

Empty

**Reply body**:

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

`http://your.server/ramses/?setStepOrder`

Moves a step in the list. This method changes only the order value of the given step.

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
    "query": "setStepOrder",
    "success": true,
    "message": "Step moved.",
    "content": { }
}
```

## moveStep

`http://your.server/ramses/?moveStep`

Moves a step in the list. This method will update the order of all the other steps of the same project accordingly.

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
    "query": "moveStep",
    "success": true,
    "message": "Step moved.",
    "content": { }
}
```

## removeStep

`http://your.server/ramses/?removeStep`

Removes a step from the database.

**Query attributes:**

- *uuid*: **string**. The step's Universal Unique Identifier.
- *version*: **string**. The version of the client.
- *token*: **string**. The session token returned by [*login*](general.md#login).

**Reply content:**

Empty

**Reply body**:

```json
{
    "accepted": true,
    "query": "removeStep",
    "success": true,
    "message": "Step uuid123 removed.",
    "content": { }
}
```

## setStepEstimations

`http://your.server/ramses/?setStepEstimations`

Updates the default estimations for this step

**Query attributes:**

- *uuid*: **string**. The step's Universal Unique Identifier.
- *method*: **string**. For shot steps, the method to use for estimations. One of `shot` or `second`.
- *veryEasy*: **float**. The estimation for very easy items, in days.
- *easy*:**float**. The estimation for easy items, in days.
- *medium*: **float**. The estimation for medium items, in days.
- *hard*: **float**. The estimation forhard items, in days.
- *veryHard*: **float**. The estimation for very hard items, in days.
- *multiplyGroupUuid*: **string**. For shot steps, the uuid of an asset group used to multiply estimations.
- *version*: **string**. The version of the client.
- *token*: **string**. The session token returned by [*login*](general.md#login).

**Reply content:**

Empty

**Reply body**:

```json
{
    "accepted": true,
    "query": "setStepEstimations",
    "success": true,
    "message": "Step updated.",
    "content": { }
}
```

## assignApplication

`http://your.server/ramses/?assignApplication`

Assigns an application to a step.

**Query attributes:**

- *stepUuid*: **string**. The step's Universal Unique Identifier.
- *applicationUuid*: **string**. The application's Universal Unique Identifier.
- *version*: **string**. The version of the client.
- *token*: **string**. The session token returned by [*login*](general.md#login).

**Reply content:**

Empty

**Reply body**:

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

`http://your.server/ramses/?unassignApplication`

Unassigns an application from a step.

**Query attributes:**

- *stepUuid*: **string**. The step's Universal Unique Identifier.
- *applicationUuid*: **string**. The application's Universal Unique Identifier.
- *version*: **string**. The version of the client.
- *token*: **string**. The session token returned by [*login*](general.md#login).

**Reply content:**

Empty

**Reply body**:

```json
{
    "accepted": true,
    "query": "unassignApplication",
    "success": true,
    "message": "Application unassigned from step.",
    "content": { }
}
```