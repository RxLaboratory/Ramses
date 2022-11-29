![META](authors:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2022;updated:2022/11/29)

# getSteps

**Query: `getSteps&projectUuid=uuid1&type=ALL`**

Gets the list of available steps.

## Reply

### JSON

```json
{
    "accepted": true,
    "query": "getSteps",
    "success": true,
    "message": "I've got the step list.",
    "content": {
        "steps": [ "unique-uid1", "unique-uid2", "unique-uid3" ]
    }
}
```

### Details

- *projectUuid*: **string**. The *UUID* of the project.
- *type*: **string**. The type of the steps. One of `PRE_PRODUCTION`, `PRODUCTION`, `SHOT_PRODUCTION`, `ASSET_PRODUCTION`, `POST_PRODUCTION`, `ALL`.
