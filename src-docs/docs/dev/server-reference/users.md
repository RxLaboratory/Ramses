![META](authors:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2021;updated:2022/01/16)

# Queries for managing users

## updateUser

`http://your.server/ramses/?updateUser`

Logs in with a username and a (hashed) password.

**Query attributes:**

- *name*: **string**. The new (or current for no change) name.
- *shortName*: **string**. The new (or current for no change) shortName.
- *comment*: **string**. The new comment.
- *email*: **string**. The new email.
- *role*: **string** (optionnal). The new role of the user. One of `admin`, `project`, `lead`, or `standard`.
- *color*: **string**. The color of the user.
- *folderPath*: **string** (optionnal). The path for the user folder.
- *uuid*: **string**. The user's Universal Unique Identifier.
- *version*: **string**. The version of the client.
- *token*: **string**. The session token returned by [*login*](general.md#login).

**Reply content:**

Empty

**Reply body**:

```json
{
    "accepted": true,
    "query": "updateUser",
    "success": true,
    "message": "User \"Duduf\" updated.",
    "content": { }
}
```

## updatePassword

`http://your.server/ramses/?updatePassword`

Changes the password of a specific user

**Query attributes:**

- *current*: **string**. The current (hashed) password.
- *new*: **string**. The new (hashed) password.
- *uuid*: **string**. The user's Universal Unique Identifier.
- *version*: **string**. The version of the client.
- *token*: **string**. The session token returned by [*login*](general.md#login).

!!! tip
    Your application / client should not send a clear password, but it should hash it first, even when using a secure/SSL connection.

**Reply content:**

Empty

**Reply body**:

```json
{
    "accepted": true,
    "query": "updateUser",
    "success": true,
    "message": "User updated.",
    "content": { }
}
```

## getUsers

`http://your.server/ramses/?getUsers`

Retrieves the list of all users.

**Query attributes:**

- *token*: **string**. The session token returned by [*login*](general.md#login).

**Reply content:**

The server replies an array of user information. Each user is an object with the following attributes:

- *name*: **string**. The name of the user.
- *shortName*: **string**. The username.
- *comment*: **string**. A comment.
- *email*: **string**. The email.
- *uuid*: **string**. The Universal Unique Identifier of this user.
- *color*: **string**. The color.
- *folderPath*: **string**. The user folder path.
- *role*: **string**. The userrole. One of `admin`, `project`, `lead`, or `standard`.

**Reply body**:

```json
{
    "accepted": true,
    "query": "getUsers",
    "success": true,
    "message": "Users list retrieved.",
    "content":  [
        {
            "name": "Nicolas Dufresne",
            "shortName": "Duduf",
            "email": "big@boss.com",
            "uuid": "2d7d7e01-671c-11e7-a78f-4ccc6a288527",
            "folderPath": "/home/duduf",
            "role": "admin",
            "color": "#e3e3e3"
        },
        {
            "name": "J. Doh",
            "shortName": "User",
            "email": "",
            "uuid": "2d7d7e01-671c-11e7-a78f-4ccc6a288527",
            "folderPath": "/home/doe",
            "role": "standard",
            "color": "#674357"
        }
    ]
}
```

## createUser

`http://your.server/ramses/?createUser`

Creates a new user in the database.

**Query attributes:**

- *name*: **string**. The new name.
- *shortName*: **string**. The new shortName.
- *email*: **string**. The email.
- *color*: **string** (optional). The color.
- *uuid*: **string** (optionnal). The user's Universal Unique Identifier.
- *version*: **string**. The version of the client.
- *token*: **string**. The session token returned with [*login*](general.md#login).

**Reply content:**

Empty

**Reply body**:

```json
{
    "accepted": true,
    "query": "createUser",
    "success": true,
    "message": "User created.",
    "content": { }
}
```

## removeUser

`http://your.server/ramses/?removeUser`

Removes a user from the database.

**Query attributes:**

- *uuid*: **string** (optionnal). The user's Universal Unique Identifier.
- *version*: **string**. The version of the client.
- *token*: **string**. The session token returned by [*login*](general.md#login).

**Reply content:**

Empty

**Reply body**:

```json
{
    "accepted": true,
    "query": "removeUser",
    "success": true,
    "message": "User 123 removed.",
    "content": { }
}
```