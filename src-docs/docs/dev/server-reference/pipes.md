# Queries for managing pipes

Pipes are the connections between *[Steps](steps.md)* which form the pipeline.

!!! hint
    There is no method to retrieve pipes directly; pipes are returned by the project method [`getProjects`](projects.md#getprojects)

[TOC]

## createPipe

Creates a new pipe in the database.

**Query attributes:**

- *inputUuid*: **string**. The UUID of the input step.
- *outputUuid*: **string**. The UUID of the output step.
- *uuid*: **string** (optional). The pipe's Universal Unique Identifier.
- *token*: **string**. The session token returned with [*login*](general.md#login).

**Reply content:**

Empty

**Examples:**

Query:  
`http://your.server/ramses/?createPipe&inputUuid=456&outputUuid=789&uuid=123&token=123`

Reply:

```json
{
    "accepted": true,
    "query": "createPipe",
    "success": true,
    "message": "Pipe created.",
    "content": { }
}
```

## updatePipe

Update pipe info in the database.

**Query attributes:**

- *inputUuid*: **string** (optionnal). The UUID of the input step.
- *outputUuid*: **string** (optionnal). The UUID of the output step.
- *colorSpaceUuid*: **string**. The UUID of the color space for the files going through this pipe.
- *filetypeUuid*: **string**. The UUID of the file type going through this pipe.
- *uuid*: **string**. The pipe's Universal Unique Identifier.
- *token*: **string**. The session token returned with [*login*](general.md#login).

**Reply content:**

Empty

**Examples:**

Query:  
`http://your.server/ramses/?updatePipe&colorSpaceUuid=123&filetypeUuid=456&uuid=123&token=123`

Reply:

```json
{
    "accepted": true,
    "query": "updatePipe",
    "success": true,
    "message": "Pipe updated.",
    "content": { }
}
```

## removePipe

Removes a pipe group from the database.

**Query attributes:**

- *uuid*: **string**. The pipe's Universal Unique Identifier.
- *token*: **string**. The session token returned with [*login*](general.md#login).

**Reply content:**

Empty

**Examples:**

Query:  
`http://your.server/ramses/?removePipe&uuid=123&token=123`

Reply:

```json
{
    "accepted": true,
    "query": "removePipe",
    "success": true,
    "message": "Pipe removed.",
    "content": { }
}
```
