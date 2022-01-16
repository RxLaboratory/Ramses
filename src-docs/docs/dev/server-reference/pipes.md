![META](authors:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2021-2022;updated:2022/01/16)

# Queries for managing pipes

Pipes are the connections between *[Steps](steps.md)* which form the pipeline.

!!! hint
    There is no method to retrieve pipes directly; pipes are returned by the project method [`getProject`](projects.md#getproject)

## createPipe

`http://your.server/ramses/?createPipe`

Creates a new pipe in the database.

**Query attributes:**

- *inputUuid*: **string**. The UUID of the input step.
- *outputUuid*: **string**. The UUID of the output step.
- *uuid*: **string** (optional). The pipe's Universal Unique Identifier.
- *version*: **string**. The version of the client.
- *token*: **string**. The session token returned by [*login*](general.md#login).

**Reply content:**

Empty

**Reply body**:

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

`http://your.server/ramses/?updatePipe`

Update pipe info in the database.

**Query attributes:**

- *inputUuid*: **string** (optionnal). The UUID of the input step.
- *outputUuid*: **string** (optionnal). The UUID of the output step.
- *colorSpaceUuid*: **string**. The UUID of the color space for the files going through this pipe.
- *filetypeUuid*: **string**. The UUID of the file type going through this pipe.
- *uuid*: **string**. The pipe's Universal Unique Identifier.
- *version*: **string**. The version of the client.
- *token*: **string**. The session token returned by [*login*](general.md#login).

**Reply content:**

Empty

**Reply body**:

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

`http://your.server/ramses/?removePipe`

Removes a pipe group from the database.

**Query attributes:**

- *uuid*: **string**. The pipe's Universal Unique Identifier.
- *version*: **string**. The version of the client.
- *token*: **string**. The session token returned by [*login*](general.md#login).

**Reply content:**

Empty

**Reply body**:

```json
{
    "accepted": true,
    "query": "removePipe",
    "success": true,
    "message": "Pipe removed.",
    "content": { }
}
```

## assignPipeFile

`http://your.server/ramses/?assignPipeFile`

Assigns a pipe file type to a pipe.

**Query attributes:**

- *pipeFileUuid*: **string**. The PipeFile's Universal Unique Identifier.
- *pipeUuid*: **string**. The Pipe's Universal Unique Identifier.
- *version*: **string**. The version of the client.
- *token*: **string**. The session token returned by [*login*](general.md#login).

**Reply content:**

Empty

**Reply body**:

```json
{
    "accepted": true,
    "query": "assignPipeFile",
    "success": true,
    "message": "New File assigned to pipe.",
    "content": { }
}
```

## unassingPipeFile

`http://your.server/ramses/?unassingPipeFile`

Unassigns a pipe file type from a pipe.

**Query attributes:**

- *pipeFileUuid*: **string**. The PipeFile's Universal Unique Identifier.
- *pipeUuid*: **string**. The Pipe's Universal Unique Identifier.
- *version*: **string**. The version of the client.
- *token*: **string**. The session token returned by [*login*](general.md#login).

**Reply content:**

Empty

**Reply body**:

```json
{
    "accepted": true,
    "query": "unassignFileType",
    "success": true,
    "message": "File removed from pipe.",
    "content": { }
}
```