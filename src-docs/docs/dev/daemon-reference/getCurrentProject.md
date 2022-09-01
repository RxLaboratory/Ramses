![META](authors:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2022;updated:2022/09/01)

# getCurrentProject

**Query: `getCurrentProject`**

Gets the current project (*UUID* and data).

## Reply

### JSON

```json
{
    "accepted": true,
    "query": "getCurrentProject",
    "success": true,
    "message": "\"Project Name\" is the current project.",
    "content": {
        "uuid": "unique-uid",
        "data": "{some JSON data}"
    }
}
```

### Details

- *uuid*: **string**. The *UUID* of the project. Empty string if there's no current project.
- *data*: **string**. The data of the project. Empty *JSON* object if there's no current project.
