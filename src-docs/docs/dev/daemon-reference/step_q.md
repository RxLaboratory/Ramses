# Step queries

## getStep

Returns a step.

### Query attributes

- *getStep*
- *shortName*: **string**. The short name of the step to retrieve.
- *name*: **string** (optional). The name of the step to retrieve.

### Reply content

The *Daemon* replies with more information about the step.

- *shortName*: **string**. The short name of the step.
- *name*: **string**. The name of the step.
- *color*: **array** of **float**. A color for displaying the step.
- *folder*: **string**. The absolute path to the common folder of the step.
- *type*: **string**. One of `"PRE_PRODUCTION"`, `"ASSET_PRODUCTION"`, `"SHOT_PRODUCTION"`, `"POST_PRODUCTION"`.

### Example

Query:  
`getStep&shortName=RIG`

Reply:  
```json
{
    "accepted": true,
    "query": "getStep",
    "success": true,
    "message": "Step retrived.",
    "content": {
            "shortName": "RIG",
            "name": "Rigging",
            "folder": "/path/to/RIG",
            "type": "ASSET_PRODUCTION",
            "color": [0.5, 0.5, 0.5]
        }
}
```

## getSteps

Returns the list of available steps for the current project.

### Query attributes

- *getSteps*

### Reply content

The *Daemon* replies with the list of Steps.

- *steps*: **array of objects**. The steps. Each step is an object with:
    - *shortName*: **string**. The short name of the step.
    - *name*: **string**. The name of the step.
    - *folder*: **string**. The absolute path to the common folder of the step.
    - *type*: **string**. One of `"PRE_PRODUCTION"`, `"ASSET_PRODUCTION"`, `"SHOT_PRODUCTION"`, `"POST_PRODUCTION"`.

### Example

Query:  
`getSteps`

Reply:  
```json
{
    "accepted": true,
    "query": "getSteps",
    "success": true,
    "message": "Step list retrived.",
    "content": {
        "steps": [
            {
                "shortName": "RIG",
                "name": "Rigging",
                "folder": "/path/to/RIG",
                "type": "ASSET_PRODUCTION"
            },
            {
                "shortName": "ANIM",
                "name": "Animation",
                "folder": "/path/to/ANIM",
                "type": "SHOT_PRODUCTION"
            }
        ]
    }
}
```