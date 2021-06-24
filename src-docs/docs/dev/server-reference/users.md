# Queries for managing users

[TOC]

## updateUser

Logs in with a username and a (hashed) password.

**Query attributes:**

- *name*: **string**. The new (or current for no change) name.
- *shortName*: **string**. The new (or current for no change) shortName.
- *comment*: **string**. The new comment.
- *role*: **string** (optionnal). The new role of the user. One of `admin`, `project`, `lead`, or `standard`.
- *folderPath*: **string** (optionnal). The path for the user folder.
- *uuid*: **string**. The user's Universal Unique Identifier.
- *token*: **string**. The session token returned with [*login*](general.md#login).

**Reply content:**

Empty

**Examples:**

Query:  
`http://your.server/ramses/?updateUser&name=Nicolas Dufresne&shortName=Duduf&comment=A comment&uuid=123&token=123`

Reply:

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

Changes the password of a specific user

**Query attributes:**

- *current*: **string**. The current (hashed) password.
- *new*: **string**. The new (hashed) password.
- *uuid*: **string**. The user's Universal Unique Identifier.
- *token*: **string**. The session token returned with [*login*](general.md#login).

!!! note
    Your application / client should not send a clear password, but it should hash it first, even when using a secure/SSL connection.

**Reply content:**

Empty

**Examples:**

Query:  
`http://your.server/ramses/?updatePassword&current=123&new=456&uuid=123&token=123`

Reply:

```json
{
    "accepted": true,
    "query": "updateUser",
    "success": true,
    "message": "User \"Duduf\" updated.",
    "content": { }
}
```

## getUsers

Retrieves the list of all users.

**Query attributes:**

- *token*: **string**. The session token returned with [*login*](general.md#login).

**Reply content:**

The server replies an array of user information. Each user is an object with the following attributes:

- *name*: **string**. The name of the user.
- *shortName*: **string**. The username.
- *uuid*: **string**. The Universal Unique Identifier of this user.
- *folderPath*: **string**. The user folder path.
- *role*: **string**. The userrole. One of `admin`, `project`, `lead`, or `standard`.

**Examples:**

Query:  
`http://your.server/ramses/?getUsers&token=123`

Reply:

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
            "uuid": "2d7d7e01-671c-11e7-a78f-4ccc6a288527",
            "folderPath": "/home/duduf",
            "role": "admin",
        },
        {
            "name": "J. Doh",
            "shortName": "User",
            "uuid": "2d7d7e01-671c-11e7-a78f-4ccc6a288527",
            "folderPath": "/home/doe",
            "role": "standard",
        }
    ]
}
```

## createUser

Creates a new user in the database.

**Query attributes:**

- *name*: **string**. The new name.
- *shortName*: **string**. The new shortName.
- *uuid*: **string** (optionnal). The user's Universal Unique Identifier.
- *token*: **string**. The session token returned with [*login*](general.md#login).

**Reply content:**

Empty

**Examples:**

Query:  
`http://your.server/ramses/?createUser&name=Nicolas Dufresne&shortName=Duduf&uuid=123&token=123`

Reply:

```json
{
    "accepted": true,
    "query": "createUser",
    "success": true,
    "message": "User \"Duduf\" created.",
    "content": { }
}
```

## removeUser

Removes a user from the database.

**Query attributes:**

- *uuid*: **string** (optionnal). The user's Universal Unique Identifier.
- *token*: **string**. The session token returned with [*login*](general.md#login).

**Reply content:**

Empty

**Examples:**

Query:  
`http://your.server/ramses/?removeUser&uuid=123&token=123`

Reply:

```json
{
    "accepted": true,
    "query": "removeUser",
    "success": true,
    "message": "User 123 removed.",
    "content": { }
}
```