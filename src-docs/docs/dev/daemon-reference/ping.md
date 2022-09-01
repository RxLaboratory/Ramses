![META](authors:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2022;updated:2022/09/01)

# ping

**Query: `ping`**

Checks if the *Daemon* is running, and gets its version and current user information.

## Reply

### JSON

```json
{
    "accepted": true,
    "query": "ping",
    "success": true,
    "message": "Hi, this is the Ramses Daemon.",
    "content": {
        "version": "1.0.0",
        "ramses": "Ramses",
        "userUuid": "unique-uid"
    }
}
```

### Details

The *Daemon* replies with its version and information about the current user.

- *version*: **string**. The version of the client/daemon.
- *ramses*: **string**. The name of the client, should be "Ramses" for the official client.
- *userUuid*: **string**. The *UUID* of the user. Empty string if there's no logged-in user.
