# User queries

## getCurrentUser

Returns the current user.

### Query attributes

- *getCurrentUser*

### Reply content

The *Daemon* replies with the user.

- *shortName*: **string**. The short name of the user.
- *name*: **string**. The name of the user.
- *role*: **string**. The role of the user, one of `'STANDARD'`, `'LEAD'`,`'PROJECT_ADMIN'`,`'ADMIN'`.
- *folderPath*: **string**. The absolute path to the user folder.

### Example

Query:  
`getCurrentUser`

Reply:  
```json
{
    "accepted": true,
    "query": "getCurrentUser",
    "success": true,
    "message": "Current user is: John Doe.",
    "content": {
        "shortName": "John",
        "name": "John Doe",
        "role": "LEAD",
        "folderPath": "/home/john/Ramses/Users/John"
    }
}
```

