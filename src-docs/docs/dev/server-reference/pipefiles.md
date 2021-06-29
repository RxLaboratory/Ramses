# Queries for managing pipe files

Pipe Files describe the file formats and colors which go through the *[Pipes](pipes.md)*.

!!! hint
    There is no method to retrieve pipe files directly; pipe files are returned by the project method [`getProject`](projects.md#getproject)

## createPipeFile

Creates a new pipe file in the database.

**Query attributes:**

- *uuid*: **string** (optional). The UUID of the pipe file.
- *shortName*: **string**. The short name of the pipe file.
- *fileTypeUuid*: **string**. The UUID of the associated file type.
- *colorSpaceUuid*: **string** (optional). The UUID of the associated color space (if any).
- *projectUuid*: **string**. The UUID of the project this pipe file belongs to.
- *token*: **string**. The session token returned with [*login*](general.md#login).

**Reply content:**

Empty

**Examples:**

Query:  
`http://your.server/ramses/?createPipeFile&shortName=newPipeFile&fileTypeUuid=456&colorSpaceUuid=789&projectUuid=789&token=123`

Reply:

```json
{
    "accepted": true,
    "query": "createPipeFile",
    "success": true,
    "message": "Pipe file created.",
    "content": { }
}
```

## updatePipeFile

Updates pipe file info in the database.

**Query attributes:**

- *uuid*: **string**. The UUID of the pipe file.
- *shortName*: **string** (optional). The short name of the pipe file.
- *comment*: **string**. The new comment.
- *fileTypeUuid*: **string** (optional). The UUID of the associated file type.
- *colorSpaceUuid*: **string** (optional). The UUID of the associated color space.
- *token*: **string**. The session token returned with [*login*](general.md#login).

**Reply content:**

Empty

**Examples:**

Query:  
`http://your.server/ramses/?updatePipeFile&shortName=newPipeFile&fileTypeUuid=456&comment=A comment&colorSpaceUuid=789&token=123`

Reply:

```json
{
    "accepted": true,
    "query": "updatePipe",
    "success": true,
    "message": "Pipe file updated.",
    "content": { }
}
```

## removePipeFile

Removes a pipe file from the database.

**Query attributes:**

- *uuid*: **string**. The pipe file's Universal Unique Identifier.
- *token*: **string**. The session token returned with [*login*](general.md#login).

**Reply content:**

Empty

**Examples:**

Query:  
`http://your.server/ramses/?removePipeFile&uuid=123&token=123`

Reply:

```json
{
    "accepted": true,
    "query": "removePipe",
    "success": true,
    "message": "Pipe file removed.",
    "content": { }
}
```
