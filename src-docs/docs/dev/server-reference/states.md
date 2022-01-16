![META](authors:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2021-2022;updated:2022/01/16)

# Queries for managing states

## createState

`http://your.server/ramses/?createState`

Creates a new state in the database.

**Query attributes:**

- *name*: **string**. The new name.
- *shortName*: **string**. The new shortName.
- *uuid*: **string** (optionnal). The step's Universal Unique Identifier.
- *version*: **string**. The version of the client.
- *token*: **string**. The session token returned by [*login*](general.md#login).

**Reply content:**

Empty

**Reply body**:

```json
{
    "accepted": true,
    "query": "createState",
    "success": true,
    "message": "State \"WIP\" created.",
    "content": { }
}
```

## getStates

`http://your.server/ramses/?getStates`

Retrieves the list of all states.

**Query attributes:**

- *token*: **string**. The session token returned by [*login*](general.md#login).

**Reply content:**

The server replies an array of state information. Each state is an object with the following attributes:

- *name*: **string**. The name of the user.
- *shortName*: **string**. The username.
- *comment*: **string**. The comment.
- *uuid*: **string**. The Universal Unique Identifier of this user.
- *color*: **string**. The color to use to display the state, hexadecimal web color preceded with a `#`.
- *completionRatio*: **int**. The completion ratio in percentage.

**Reply body**:

```json
{
    "accepted": true,
    "query": "getStates",
    "success": true,
    "message": "States list retrieved.",
    "content":  [
        {
            "name": "Stand by",
            "shortName": "STB",
            "comment": "A Comment",
            "uuid": "2d7d7e01-671c-11e7-a78f-4ccc6a288527",
            "color": "#6d6d6d",
            "completionRatio": 0
        },
        {
            "name": "To do",
            "shortName": "TODO",
            "comment": "A Comment",
            "uuid": "2d7d7e01-671c-11e7-a78f-4ccc6a288527",
            "color": "#00aaff",
            "completionRatio": 0
        },
        {
            "name": "Finished",
            "shortName": "OK",
            "comment": "A Comment",
            "uuid": "2d7d7e01-671c-11e7-a78f-4ccc6a288527",
            "color": "#00aa00",
            "completionRatio": 100
        }
    ]
}
```

## updateState

`http://your.server/ramses/?updateState`

Update state info in the database.

**Query attributes:**

- *name*: **string**. The name of the user.
- *shortName*: **string**. The username.
- *comment*: **string**. The new comment.
- *uuid*: **string**. The Universal Unique Identifier of this user.
- *color*: **string** (optional). The color to use to display the state, hexadecimal web color preceded with a `#`.
- *completionRatio*: **int** (optional). The completion ratio in percentage.
- *version*: **string**. The version of the client.
- *token*: **string**. The session token returned with [*login*](general.md#login)

**Reply content:**

Empty

**Reply body**:

```json
{
    "accepted": true,
    "query": "updateState",
    "success": true,
    "message": "State \"WIP\" updated.",
    "content": { }
}
```

## removeState

`http://your.server/ramses/?removeState`

Removes a state from the database.

**Query attributes:**

- *uuid*: **string**. The step's Universal Unique Identifier.
- *version*: **string**. The version of the client.
- *token*: **string**. The session token returned with [*login*](general.md#login).

**Reply content:**

Empty

**Reply body**:

```json
{
    "accepted": true,
    "query": "removeState",
    "success": true,
    "message": "State uuid123 removed.",
    "content": { }
}
```
