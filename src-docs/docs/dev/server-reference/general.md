![META](authors:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2021-2022;updated:2022/01/16)

# General queries

## ping

`http://your.server/ramses/?ping`

Use this query to test if the server is available before logging in.

**Reply content:**

- *version*: **string**. The version of the current Ramses Server.
- *installed*: **boolean**. False if the server is not correctly installed (i.e. the install script has not been run). See the [*Server installatio*](../../components/server/install.md) section of this documentation.

**Reply body**:

```json
{
    "accepted": true,
    "query": "ping",
    "success": true,
    "message": "Server Ready!",
    "content": {
        "version": "0.1.1-Alpha",
        "installed": true
    }
}
```

## login

`http://your.server/ramses/?login`

Logs in with a username and a (hashed) password.

**Query attributes:**

- *username*: **string**. The username.
- *password*: **string**. The (hashed) password.
- *version*: **string**. The version of the client.

!!! note
    Your application / client should not send a clear password, but it should hash it first, even when using a secure/SSL connection.

**Reply content:**

If successful, the server replies with the user who's logged in. Note that the content also includes the *token* you'll need for further requests.

- *name*: **string**. The name of the user.
- *shortName*: **string**. The username.
- *uuid*: **string**. The Universal Unique Identifier of this user.
- *folderPath*: **string**. The user folder path.
- *role*: **string**. The userrole.
- *token*: **string**. The token to use for further requests.

If not successful, the content is empty and the message explains the error.

**Successful reply body**:

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

**Unsuccesful reply body**:

```json
{
    "accepted":true,
    "success":false,
    "message":"Invalid username",
    "query":"login",
    "content":[]
}
```

## init

`http://your.server/ramses/?init`

This special requests is used to retrieve all useful information when initializing the *Ramses Client*. It groups together in a single request the data which can be retrieved individually by `getUsers`, `getTemplateSteps`, `getTemplateAssetGroups`, `getStates`, `getFileTypes`, `getApplications` and `getProjects`. The project data contains only general project information (resolution, framerate...) but not the project content (shots, assets...). Use `getProject` to retrieve a specific project content.

**Query attributes:**

- *version*: **string**. The version of the client.
- *token*: **string**. The session token returned by [*login*](general.md#login).

**Reply content:**

The server replies with all the data. Read the documentation for the corresponding individual requests for more details.

- *users*: **array**.
- *templateSteps*: **array**.
- *templateAssetGroups*: **array**.
- *states*: **array**.
- *fileTypes*: **array**.
- *applications*: **array**.
- *projects*: **array**.

**Reply body**:

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