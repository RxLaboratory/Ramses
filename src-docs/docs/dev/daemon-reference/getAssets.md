![META](authors:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2022;updated:2022/11/29)

# getAssets

**Query: `getAssets&projectUuid=uuid1&groupUuid=uuid2`**

Gets the list of available assets for the project or asset group.

## Reply

### JSON

```json
{
    "accepted": true,
    "query": "getAssets",
    "success": true,
    "message": "I've got the asset list.",
    "content": {
        "assets": [ "unique-uid1", "unique-uid2", "unique-uid3" ]
    }
}
```

### Details

- *projectUuid*: (optionnal) **string**. The *UUID* of the project. May be omitted if and only if the asset group is provided.
- *groupUuid*: (optionnal) **string**. The *UUID* of the asset group.