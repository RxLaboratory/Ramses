![META](authors:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2022;updated:2022/09/01)

# getData

**Query: `getData&uuid=unique-uid`**

Gets the data for a specific *RamObject*.

## Reply

### JSON

```json
{
    "accepted": true,
    "query": "getData",
    "success": true,
    "message": "I've got some data.",
    "content": {
        "uuid": "unique-uid",
        "data": {"Some JSON Data"}
    }
}
```