![META](authors:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2022;updated:2022/11/29)

# getStatus

**Query: `getStatus&itemUuid=uuid1&stepUuid=uuid2`**

Gets the list of available assets for the project or asset group.

## Reply

### JSON

```json
{
    "accepted": true,
    "query": "getStatus",
    "success": true,
    "message": "I've found a status.",
    "content": {
        "uuid": "unique-uid",
        "data": {"Some JSON Data"}
    }
}
```

### Details

- *itemUuid*: **string**. The *UUID* of the item.
- *stepUuid*: **string**. The *UUID* of the step.