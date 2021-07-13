![META](authors:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2021;updated:2021/07/12)

# Queries for managing schedule entries

A schedule entry associates a date with a user and a step.

!!! hint
    There is no method to retrieve schedule entries directly; schedule entries are returned by the project method [`getProjects`](projects.md#getprojects)

## createSchedule

`http://your.server/ramses/?createSchedule`

Creates a new entry in the database.

**Query attributes:**

- *date*: **string**. The date, formatted as `yyyy-MM-dd hh:mm:ss`.
- *userUuid*: **string**. The UUID of the user.
- *stepUuid*: **string**. The UUID of the step.
- *uuid*: **string** (optional). The entry's Universal Unique Identifier.
- *token*: **string**. The session token returned by [*login*](general.md#login).

**Reply content:**

Empty

**Reply body**:

```json
{
    "accepted": true,
    "query": "createSchedule",
    "success": true,
    "message": "Schedule updated.",
    "content": { }
}
```

## createSchedules

`http://your.server/ramses/?createSchedules`

Creates several new entries in the database.

!!! warning
    This query cannot be *URL* encoded, it must be posted as *JSON*.

**Query attributes:**

- *entries*: **array** of **object**. Eeach entry must be an object with these properties:
    - *date*: **string**. The date, formatted as `yyyy-MM-dd hh:mm:ss`.
    - *userUuid*: **string**. The UUID of the user.
    - *stepUuid*: **string**. The UUID of the step.
    - *uuid*: **string** (optional). The entry's Universal Unique Identifier.
- *token*: **string**. The session token returned by [*login*](general.md#login).

**Reply content:**

Empty

**Reply body**:

```json
{
    "accepted": true,
    "query": "createSchedules",
    "success": true,
    "message": "Schedule updated.",
    "content": { }
}
```

## updateSchedule

`http://your.server/ramses/?updateSchedule`

Updates a new entry in the database.

**Query attributes:**

- *date*: **string**. The date, formatted as `yyyy-MM-dd hh:mm:ss`.
- *comment*: **string**. A comment.
- *userUuid*: **string**. The UUID of the user.
- *stepUuid*: **string**. The UUID of the step.
- *uuid*: **string** (optional). The entry's Universal Unique Identifier.
- *token*: **string**. The session token returned by [*login*](general.md#login).

**Reply content:**

Empty

**Reply body**:

```json
{
    "accepted": true,
    "query": "updateSchedule",
    "success": true,
    "message": "Schedule updated.",
    "content": { }
}
```

## updateSchedules

`http://your.server/ramses/?updateSchedules`

Updates several new entries in the database.

!!! warning
    This query cannot be *URL* encoded, it must be posted as *JSON*.

**Query attributes:**

- *entries*: **array** of **object**. Eeach entry must be an object with these properties:
    - *date*: **string**. The date, formatted as `yyyy-MM-dd hh:mm:ss`.
    - *comment*: **string**. A comment.
    - *userUuid*: **string**. The UUID of the user.
    - *stepUuid*: **string**. The UUID of the step.
    - *uuid*: **string**. The entry's Universal Unique Identifier.
- *token*: **string**. The session token returned by [*login*](general.md#login).

**Reply content:**

Empty

**Reply body**:

```json
{
    "accepted": true,
    "query": "updateSchedules",
    "success": true,
    "message": "Schedule updated.",
    "content": { }
}
```

## removeSchedule

`http://your.server/ramses/?removeSchedule`

Removes an entry from the database.

**Query attributes:**

- *uuid*: **string**. The entry's Universal Unique Identifier.
- *token*: **string**. The session token returned by [*login*](general.md#login).

**Reply content:**

Empty

**Reply body**:

```json
{
    "accepted": true,
    "query": "removeSchedule",
    "success": true,
    "message": "Schedule updated.",
    "content": { }
}
```

## removeSchedules

`http://your.server/ramses/?removeSchedules`

Removes several entries from the database.

!!! warning
    This query cannot be *URL* encoded, it must be posted as *JSON*.

**Query attributes:**

- *entries*: **array** of **object**. Eeach entry must be an object with these properties:
    - *uuid*: **string**. The entry's Universal Unique Identifier.
- *token*: **string**. The session token returned by [*login*](general.md#login).

**Reply content:**

Empty

**Reply body**:

```json
{
    "accepted": true,
    "query": "removeSchedules",
    "success": true,
    "message": "Schedule updated.",
    "content": { }
}
```