![META](authors:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2022;updated:2022/09/02)

# getObjects

**Query: `getObjects&type=RamObject`**

Gets all objects of a specific type.

## Reply

### JSON

```json
{
    "accepted": true,
    "query": "getData",
    "success": true,
    "message": "I've got the list of \"RamObject\".",
    "content": {
        "objects": [
            {
                "uuid": "unique-uid1",
                "data": {"Some JSON Data"}
            },
            {
                "uuid": "unique-uid2",
                "data": {"Some JSON Data"}
            },
            {
                "..."
            }
        ]
    }
}
```