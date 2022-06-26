# State and Status queries

## getState

Returns a state.

### Query attributes

- *getState*
- *shortName*: **string**. The short name of the state to retrieve.
- *name*: **string** (optional). The name of the state to retrieve.

### Reply content

The *Daemon* replies with more information about the state.

- *shortName*: **string**. The short name of the state.
- *name*: **string**. The name of the state.
- *completionRatio*: **float**. The completion ratio of the state, in the range `[0, 100]`.
- *color*: **array of integers**. The color for this state, `[R, G, B]` in the range `[0, 255]`.

### Example

Query:  
`getState&shortName=TODO`

Reply:  
```json
{
    "accepted": true,
    "query": "getState",
    "success": true,
    "message": "State retrived.",
    "content": {
            "shortName": "TODO",
            "name": "Waiting...",
            "completionRatio": 0,
            "color": [ 50, 50, 50]
        }
}
```

## getStates

Returns the list of available states.

### Query attributes

- *getStates*

### Reply content

The *Daemon* replies with the list of States.

- *states*: **array of objects**. The states. Each states is an object with:
    - *shortName*: **string**. The short name of the state.
    - *name*: **string**. The name of the state.
    - *completionRatio*: **float**. The completion ratio of the state, in the range `[0, 100]`.
    - *color*: **array of integers**. The color for this state, `[R, G, B]` in the range `[0, 255]`.

### Example

Query:  
`getStates`

Reply:  
```json
{
    "accepted": true,
    "query": "getStates",
    "success": true,
    "message": "State list retrived.",
    "content": {
        "states": [
            {
                "shortName": "TODO",
                "name": "Waiting...",
                "completionRatio": 0,
                "color": [ 50, 50, 50]
            },
            {
                "shortName": "WIP",
                "name": "Work In Progress",
                "completionRatio": 50,
                "color": [ 255, 0, 0]
            },
            {
                "shortName": "OK",
                "name": "Finished and approved",
                "completionRatio": 100,
                "color": [ 0, 255, 0]
            }
        ]
    }
}
```

## getCurrentStatus

Returns the current status of a given step for a given item (shot or asset).

### Query attributes

- *getCurrentStatus*
- *shortName*: **string**. The item short name.
- *name*: **string**. The item name.
- *type*: **string**. One of `"ASSET"` or `"SHOT"`.
- *step*: **string**. The step short name.

### Reply content

The *Daemon* replies with more information about the current status.

- *step*: **string**. The short name of the associated step.
- *comment*: **string**. The comment.
- *completionRatio*: **int**. The completion ratio, in the range [0,100].
- *date*: **string**. The date and time, in the format "yyyy-MM-dd hh:mm:ss".
- *state*: **string**. The state short name.
- *user*: **string**. The user short name.
- *version*: **int**. The version.

### Example

Query:  
`getCurrentStatus&shortName=TRI&name=Tristan&type=ASSET&step=RIG`

Reply:  
```json
{
    "accepted": true,
    "query": "getCurrentUser",
    "success": true,
    "message": "Current user is: John Doe.",
    "content": {
            "step": "RIG",
            "comment": "Working on it!",
            "completionRatio": 75,
            "date": "2021-04-12 10:55:23",
            "state": "WIP",
            "user": "Duduf",
            "version": 12
        }
}
```

## getCurrentStatuses

Returns the current status list (one per step) for a given item (shot or asset).

### Query attributes

- *getCurrentStatuses*
- *shortName*: **string**. The item short name.
- *name*: **string**. The item name.
- *type*: **string**. One of `"A"` or `"S"`.

### Reply content

The *Daemon* replies with the list of status.

- *status*: **array of objects**. The status. Each status is an object with:
    - *step*: **string**. The short name of the associated step.
    - *comment*: **string**. The comment.
    - *completionRatio*: **int**. The completion ratio, in the range [0,100].
    - *date*: **string**. The date and time, in the format "yyyy-MM-dd hh:mm:ss".
    - *state*: **string**. The state short name.
    - *user*: **string**. The user short name.
    - *version*: **int**. The version.

### Example

Query:  
`getCurrentStatuses&shortName=TRI&name=Tristan&type=ASSET`

Reply:  
```json
{
    "accepted": true,
    "query": "getCurrentStatuses",
    "success": true,
    "message": "Current status list retrieved.",
    "content": {
        "status": [
            {
                "step": "RIG",
                "comment": "Working on it!",
                "completionRatio": 75,
                "date": "2021-04-12 10:55:23",
                "state": "WIP",
                "user": "Duduf",
                "version": 12
            },
            {
                "step": "MOD",
                "comment": "Finished",
                "completionRatio": 100,
                "date": "2021-04-12 10:55:23",
                "state": "OK",
                "user": "Duduf",
                "version": 5
            }
            
        ]
    }
}
```

## setStatus

Sets a new status to an item

### Query attributes

- *setStatus*
- *shortName*: **string**. The item short name.
- *name*: **string**. The item name.
- *step*: **string**. The short name of the step.
- *type*: **string**. The type of item, either `"A"` or `"S"`.
- *state*: **string**. The short name of the state.
- *comment*: **string**. The comment.
- *published*: **boolean**. Whether the item has been published or not.
- *completionRatio*: **int**. The completion ration in %.
- *version*: **int**. The current version.
- *user*: **string**. The short name of the user.

### Reply content

None.

### Example

Query:  
`setStatus&shortName=TRI&name=Tristan&step=MOD&type=A&state=WIP&comment=A Comment&published=true&completionRatio=55&version=42&user=Duf`

Reply:  
```json
{
    "accepted": true,
    "query": "setStatus",
    "success": true,
    "message": "Status updated."
}
```
