# Queries for managing states

[TOC]

## createState

Creates a new state in the database.

**Query attributes:**

- *name*: **string**. The new name.
- *shortName*: **string**. The new shortName.
- *uuid*: **string** (optionnal). The step's Universal Unique Identifier.
- *token*: **string**. The session token returned with [*login*](general.md#login).

**Reply content:**

Empty

**Examples:**

Query:  
`http://your.server/ramses/?createState&name=Work in progress&shortName=WIP&uuid=123&token=123`

Reply:

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

Retrieves the list of all states.

**Query attributes:**

None

**Reply content:**

The server replies an array of state information. Each state is an object with the following attributes:

- *name*: **string**. The name of the user.
- *shortName*: **string**. The username.
- *uuid*: **string**. The Universal Unique Identifier of this user.
- *color*: **string**. The color to use to display the state, hexadecimal web color preceded with a `#`.
- *completionRatio*: **int**. The completion ratio in percentage.

**Examples:**

Query:  
`http://your.server/ramses/?getStates&token=123`

Reply:

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
            "uuid": "2d7d7e01-671c-11e7-a78f-4ccc6a288527",
            "color": "#6d6d6d",
            "completionRatio": 0
        },
        {
            "name": "To do",
            "shortName": "TODO",
            "uuid": "2d7d7e01-671c-11e7-a78f-4ccc6a288527",
            "color": "#00aaff",
            "completionRatio": 0
        },
        {
            "name": "Finished",
            "shortName": "OK",
            "uuid": "2d7d7e01-671c-11e7-a78f-4ccc6a288527",
            "color": "#00aa00",
            "completionRatio": 100
        }
    ]
}
```

## updateState

Update state info in the database.

**Query attributes:**

- *name*: **string**. The name of the user.
- *shortName*: **string**. The username.
- *comment*: **string**. The new comment.
- *uuid*: **string**. The Universal Unique Identifier of this user.
- *color*: **string** (optional). The color to use to display the state, hexadecimal web color preceded with a `#`.
- *completionRatio*: **int** (optional). The completion ratio in percentage.
- *token*: **string**. The session token returned with [*login*](general.md#login)

**Reply content:**

Empty

**Examples:**

Query:  
`http://your.server/ramses/?updateState&name=Work in progress&shortName=WIP&comment=A Comment&uuid=123&token=123`

Reply:

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

Removes a state from the database.

**Query attributes:**

- *uuid*: **string**. The step's Universal Unique Identifier.
- *token*: **string**. The session token returned with [*login*](general.md#login).

**Reply content:**

Empty

**Examples:**

Query:  
`http://your.server/ramses/?removeState&uuid=123&token=123`

Reply:

```json
{
    "accepted": true,
    "query": "removeState",
    "success": true,
    "message": "State uuid123 removed.",
    "content": { }
}
```
