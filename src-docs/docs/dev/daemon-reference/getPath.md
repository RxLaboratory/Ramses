![META](authors:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2022;updated:2022/09/01)

# getPath

**Query: `getPath&uuid=unique-uid`**

Gets the path for a specific *RamObject*.

## Reply

### JSON

```json
{
    "accepted": true,
    "query": "getData",
    "success": true,
    "message": "I've got the path of the object.",
    "content": {
        "path": "path/to/the/object"
    }
}
```