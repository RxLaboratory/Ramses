![META](authors:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2022;updated:2022/09/01)

# getRamsesFolder

**Query: `getRamsesFolder`**

Gets the folder where all data is stored.

## Reply

### JSON

```json
{
    "accepted": true,
    "query": "getRamsesFolder",
    "success": true,
    "message": "I've got the Ramses folder.",
    "content": {
        "path": "path/to/ramses/data"
    }
}
```

### Details

- *path*: **string**. The path to the data.
