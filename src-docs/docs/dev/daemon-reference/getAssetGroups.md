![META](authors:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2022;updated:2022/11/29)

# getAssetGroups

**Query: `getAssetGroups&projectUuid=uuid1`**

Gets the list of available asset groups.

## Reply

### JSON

```json
{
    "accepted": true,
    "query": "getAssetGroups",
    "success": true,
    "message": "I've got the asset group list.",
    "content": {
        "assetGroups": [ "unique-uid1", "unique-uid2", "unique-uid3" ]
    }
}
```

### Details

- *projectUuid*: **string**. The *UUID* of the project.
