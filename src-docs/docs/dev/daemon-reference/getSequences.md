![META](authors:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2022;updated:2022/11/29)

# getSequences

**Query: `getSequences&projectUuid=uuid1`**

Gets the list of available sequences.

## Reply

### JSON

```json
{
    "accepted": true,
    "query": "getSequences",
    "success": true,
    "message": "I've got the sequence list.",
    "content": {
        "sequences": [ "unique-uid1", "unique-uid2", "unique-uid3" ]
    }
}
```

### Details

- *projectUuid*: **string**. The *UUID* of the project.
