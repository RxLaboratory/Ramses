![META](authors:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2021;updated:2021/07/12)

# Queries for managing sequences

!!! hint
    There is no method to retrieve sequences directly; sequences are returned by the project method [`getProjects`](projects.md#getprojects)

## createSequence

`http://your.server/ramses/?createSequence`

Creates a new squence in the database and assigns it to a project.

**Query attributes:**

- *name*: **string**. The new name.
- *shortName*: **string**. The new shortName.
- *comment*: **string**. The new comment.
- *projectUuid*: **string**. The UUID of the project to assign the sequence to.
- *uuid*: **string** (optionnal). The asset group's Universal Unique Identifier.
- *token*: **string**. The session token returned by [*login*](general.md#login).

**Reply content:**

Empty

**Reply body**:

```json
{
    "accepted": true,
    "query": "createSequence",
    "success": true,
    "message": "Sequence \"SEQ01\" created.",
    "content": { }
}
```

## updateSequence

`http://your.server/ramses/?updateSequence`

Update sequence info in the database.

**Query attributes:**

- *name*: **string**. The new (or current for no change) name.
- *shortName*: **string**. The new (or current for no change) shortName.
- *comment*: **string**. A new comment.
- *uuid*: **string**. The sequence's Universal Unique Identifier.
- *token*: **string**. The session token returned by [*login*](general.md#login)

**Reply content:**

Empty

**Reply body**:

```json
{
    "accepted": true,
    "query": "updateSequence",
    "success": true,
    "message": "Sequence \"SEQ01\" updated.",
    "content": { }
}
```

## removeSequence

`http://your.server/ramses/?removeSequence`

Removes a sequence from the database.

**Query attributes:**

- *uuid*: **string**. The sequence's Universal Unique Identifier.
- *token*: **string**. The session token returned with [*login*](general.md#login).

**Reply content:**

Empty

**Reply body**:

```json
{
    "accepted": true,
    "query": "removeSequence",
    "success": true,
    "message": "Sequence uuid123 removed.",
    "content": { }
}
```

## setSequenceOrder

`http://your.server/ramses/?setSequenceOrder`

Changes the order of a sequence in the project.

**Query attributes:**

- *uuid*: **string**. The sequence's Universal Unique Identifier.
- *order*: **int**. The new order.
- *token*: **string**. The session token returned with [*login*](general.md#login).

**Reply content:**

Empty

**Reply body**:

```json
{
    "accepted": true,
    "query": "setSequenceOrder",
    "success": true,
    "message": "Sequence moved.",
    "content": { }
}
```

