![META](authors:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2022;updated:2022/11/29)

# getShots

**Query: `getShots&projectUuid=uuid1&sequenceUuid=uuid2`**

Gets the list of available shots for the project or sequence.

## Reply

### JSON

```json
{
    "accepted": true,
    "query": "getShots",
    "success": true,
    "message": "I've got the shot list.",
    "content": {
        "shots": [ "unique-uid1", "unique-uid2", "unique-uid3" ]
    }
}
```

### Details

- *projectUuid*: (optionnal) **string**. The *UUID* of the project. May be omitted if and only if the sequence is provided.
- *sequenceUuid*: (optionnal) **string**. The *UUID* of the sequence.