# Queries for managing sequences

!!! hint
    There is no method to retrieve sequences directly; sequences are returned by the project method [`getProjects`](projects.md#getprojects)

[TOC]

## createSequence

Creates a new squence in the database and assigns it to a project.

**Query attributes:**

- *name*: **string**. The new name.
- *shortName*: **string**. The new shortName.
- *comment*: **string**. The new comment.
- *projectUuid*: **string**. The UUID of the project to assign the sequence to.
- *uuid*: **string** (optionnal). The asset group's Universal Unique Identifier.
- *token*: **string**. The session token returned with [*login*](general.md#login).

**Reply content:**

Empty

**Examples:**

Query:  
`http://your.server/ramses/?createSequence&name=Sequence 01&shortName=SEQ01&comment=A comment&projectUuid=456&uuid=123&token=123`

Reply:

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

Update sequence info in the database.

**Query attributes:**

- *name*: **string**. The new (or current for no change) name.
- *shortName*: **string**. The new (or current for no change) shortName.
- *uuid*: **string**. The sequence's Universal Unique Identifier.
- *token*: **string**. The session token returned with [*login*](general.md#login)

**Reply content:**

Empty

**Examples:**

Query:  
`http://your.server/ramses/?updateSequence&name=Sequence 01&shortName=SEQ01&uuid=123&token=123`

Reply:

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

Removes a sequence from the database.

**Query attributes:**

- *uuid*: **string**. The sequence's Universal Unique Identifier.
- *token*: **string**. The session token returned with [*login*](general.md#login).

**Reply content:**

Empty

**Examples:**

Query:  
`http://your.server/ramses/?removeSequence&uuid=123&token=123`

Reply:

```json
{
    "accepted": true,
    "query": "removeSequence",
    "success": true,
    "message": "Sequence uuid123 removed.",
    "content": { }
}
```
