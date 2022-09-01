![META](authors:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2022;updated:2022/09/01)

# getProjects

**Query: `getProjects`**

Gets the list of available projects for the current user.

## Reply

### JSON

```json
{
    "accepted": true,
    "query": "getProjects",
    "success": true,
    "message": "I've got the project list.",
    "content": {
        "projects": [
            "unique-uid1",
            "unique-uid2",
            "unique-uid3"
        ]
    }
}
```

### Details

- *projects*: **list** of **string**. The list of available project *UUIDs*.
