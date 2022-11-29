![META](authors:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2022;updated:2022/11/29)

# getPipes

**Query: `getPipes&projectUuid=uuid1`**

Gets the list of available pipes.

## Reply

### JSON

```json
{
    "accepted": true,
    "query": "getPipes",
    "success": true,
    "message": "I've got the pipeline.",
    "content": {
        "pipes": [ "unique-uid1", "unique-uid2", "unique-uid3" ]
    }
}
```

### Details

- *projectUuid*: **string**. The *UUID* of the project.
