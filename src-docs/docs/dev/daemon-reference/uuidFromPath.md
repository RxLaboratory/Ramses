![META](authors:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2022;updated:2022/09/01)

# uuidFromPath

**Query: `uuidFromPath&path=some/path`**

Gets the *UUID* of the object at this path.

## Reply

### JSON

```json
{
    "accepted": true,
    "query": "uuidFromPath",
    "success": true,
    "message": "I've found an object.",
    "content": {
        "uuid": "unique-uid"
    }
}
```

### Details

- *uuid*: **string**. The *UUID* of the object.