# General queries

[TOC]

## ping

Use this query to test if the server is available before logging in.

**Reply content:**

- *version*: **string**. The version of the current Ramses Server.
- *installed*: **boolean**. False if the server is not correctly installed (i.e. the install script has not been run). See the [*Server installatio*](../../install/server.md) section of this documentation.

**Examples:**

Query:  
`http://your.server/ramses/?ping`

Reply:

```json
{
    "accepted": true,
    "query": "ping",
    "success": true,
    "message": "Server Ready!",
    "content": {
        "version": "0.0.1",
        "installed": true
    }
}
```

## login

Logs in with a username and a (hashed) password.

**Query attributes:**

- *username*: **string**. The username.
- *password*: **string**. The (hashed) password.

!!! note
    Your application / client should not send a clear password, but it should hash it first, even when using a secure/SSL connection.

**Reply content:**

The server replies with the user which has logged in. Note that the content also includes the *token* you'll need for further requests.

- *name*: **string**. The name of the user.
- *shortName*: **string**. The username.
- *uuid*: **string**. The Universal Unique Identifier of this user.
- *folderPath*: **string**. The user folder path.
- *role*: **string**. The userrole.
- *token*: **string**. The token to use for further requests.

**Examples:**

Query:  
`http://your.server/ramses/?login&username=Duduf&password=123456`

Reply:

```json
{
    "accepted": true,
    "query": "login",
    "success": true,
    "message": "Server Ready!",
    "content": {
        "name": "Nicolas Dufresne",
        "shortName": "Duduf",
        "uuid": "2d7d7e01-671c-11e7-a78f-4ccc6a288527",
        "folderPath": "/home/duduf",
        "role": "admin",
        "token": "2f5d2e3d4e3af1a9d459c5731247abd1ee9a7123"
    }
}
```

## init

This special requests is used to retrieve all useful information when initializing the *Ramses Client*. It groups together in a single request the data which can be retrieved individually by `getUsers`, `getTemplateSteps`, `getTemplateAssetGroups`, `getStates`, `getFileTypes`, `getApplications` and `getProjects`. The project data contains only general project information (resolution, framerate...) but not the project content (shots, assets...). Use `getProject` to retrieve a specific project content.

**Query attributes:**

- *token*: **string**. The session token returned with [*login*](general.md#login).

**Reply content:**

The server replies with all the data. Read the documentation for the corresponding individual requests for more details.

- *users*: **array**.
- *templateSteps*: **array**.
- *templateAssetGroups*: **array**.
- *states*: **array**.
- *fileTypes*: **array**.
- *applications*: **array**.
- *projects*: **array**.

**Examples:**

Query:  
`http://your.server/ramses/?init&token=123`

Reply:

```json
{
    "accepted": true,
    "query": "login",
    "success": true,
    "message": "Server Ready!",
    "content": {
        "users": [ ... ],
        "templateSteps": [ ... ],
        "templateAssetGroups": [ ... ],
        "states": [ ... ],
        "fileTypes": [ ... ],
        "applications": [ ... ],
        "projects": [ ... ]
    }
}
```