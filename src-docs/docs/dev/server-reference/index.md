![META](authors:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2021-2022;updated:2022/08/23)

# Ramses Server API

The *Ramses* server provides a standard *http(s) REST API*. If you're developping your own client (or contributing to *Ramses Client*), you can easily exchange data with the server using this reference.

**Queries must be a *POST* request**; the body must be *JSON* encoded, the `Content-Type` being `application/json`.

Only one attribute must be set directly in the *URL* (like a *GET* request), the name of the query itself.

For now, the server only supports the **UTF-8** charset, so it is not mandatory to add the charset in the *Content-Type*, and any value other than `utf-8` will be ignored. This may change in future versions.

## Queries

The server provides a single endpoint, which is the root of the folder where it's installed. For example, for a server installed in a folder called *example* on the server at *https://ramses.rxlab.io*, the endpoint will be `https://ramses.rxlab.io/example`.

To query the server, you only need to add the query name as a parameter to this *URL*. For example, the URL for the `ping` query is `https://ramses.rxlab.io/example/?ping`.

All queries must be accompanied with a *JSON* object in the post body, containing at least these values:

```json
{
    "version": "0.5.0"
}
```

- `version`: version of the client connected to the server.

## Replies

The server replies to all queries with a *JSON* object, containing these values:

```json
{
    "accepted": true,
    "success": true,
    "message": "Server ready.",
    "query": "ping",
    "content": {}
}
```

- `accepted`: **boolean**. True if the server has recognized the query and accepted it. Will be False if the query does not exist.
- `success`: **boolean**. True if the server has successfully processed the query, False in case of any error.
- `message`: **string**. A short feedback about the result, which can be shown to the user by the client.
- `query`: **string**. The query that was made that the server is replying to.
- `content`: **object**. The actual content of the reply, which varies depending on the query. Read this reference for more details.

## Syncing data

This is the process to sync data with the server.

1. `ping` to check if the server is available and check its version. This is mandatory before being able to log in.
2. `login` to authenticate yourself.
3. `sync` your data.

For the examples in this documentation, we assume the ramses server is available at `https://ramses.rxlab.io/example/`.

!!! note
    For the sake of readability, the *JSON* replies are pretty formatted with indentation and new lines in this documentation, but the server actually uses one-line replies.

### ping

Query: `https://ramses.rxlab.io/example/?ping`

#### Request body

```json
{
    "version": "0.5.0"
}
```

- `version`: version of the client.

#### Reply

The server replies with:

```json
{
    "accepted": true,
    "success": true,
    "message": "Server ready.",
    "query": "ping",
    "content": {
        "installed": true,
        "version": "0.5.0"
    }
}
```

- `installed` is false if the server has not been correctly installed yet.
- `version` is the version of the server, which should match the version of the client.

### login

Query: `https://ramses.rxlab.io/example/?login`

#### Request body

```json
{
    "version": "0.5.0",
    "username": "Duduf",
    "password": "123456"
}
```

- `version`: version of the client.
- `username`: user name.
- `password`: user password.

#### Reply

The server replies with:

```json
{
    "accepted": true,
    "success": true,
    "message": "Successful login. Welcome Duduf!",
    "query": "login",
    "content": {
        "username": "Duduf",
        "uuid": "uuid",
        "token": "token"
    }
}
```

- `username`: the user name of the user.
- `uuid`: the uuid of the user.
- `token`: a token which must be associated with `sync` queries later.

### sync

Query: `https://ramses.rxlab.io/example/?sync`

#### Request body

```json
{
    "version": "0.5.0",
    "token": "token",
    "previousSyncDate": "2022-06-15 15:44:23",
    "tables":[
        {
            "name": "RamApplication",
            "modifiedRows": [
                {
                    "uuid": "123456",
                    "data": "{some data}",
                    "modified": "2022-07-15 15:44:23",
                    "removed": 0
                },
                {
                    "uuid": "789123",
                    "data": "{some other data}",
                    "modified": "2022-08-24 15:44:23",
                    "removed": 1
                }
            ]
        },
        {
            "name": "RamStep",
            "modifiedRows": [
                {
                    "uuid": "123456",
                    "data": "{some data}",
                    "modified": "2022-07-15 15:44:23",
                    "removed": 0
                },
                {
                    "uuid": "789123",
                    "data": "{some other data}",
                    "modified": "2022-08-24 15:44:23",
                    "removed": 1
                }
            ]
        },
        {
            "name": "RamUser",
            "modifiedRows": [
                {
                    "uuid": "123456",
                    "userName": "Duf",
                    "data": "{some data}",
                    "modified": "2022-07-15 15:44:23",
                    "removed": 0
                },
                {
                    "uuid": "789123",
                    "userName": "Ana",
                    "data": "{some other data}",
                    "modified": "2022-08-24 15:44:23",
                    "removed": 1
                }
            ]
        }
    ]
}
```

- `version`: version of the client.
- `token`: the token got with `login`.
- `previousSyncDate`: the date and time of the previous sync.
- `tables`: the list of tables to sync.

#### Reply

The server replies with:

```json
{
    "accepted": true,
    "success": true,
    "message": "Data sync, OK!",
    "query": "sync",
    "content": {
        "tables":[
            {
                "name": "RamApplication",
                "modifiedRows": [
                    {
                        "uuid": "123456",
                        "data": "{some data}",
                        "modified": "2022-08-15 15:44:23",
                        "removed": 0
                    },
                    {
                        "uuid": "789123",
                        "data": "{some other data}",
                        "modified": "2022-08-26 15:44:23",
                        "removed": 1
                    }
                ]
            },
            {
                "name": "RamStep",
                "modifiedRows": [
                    {
                        "uuid": "123456",
                        "data": "{some data}",
                        "modified": "2022-08-15 15:44:23",
                        "removed": 0
                    },
                    {
                        "uuid": "789123",
                        "data": "{some other data}",
                        "modified": "2022-08-26 15:44:23",
                        "removed": 1
                    }
                ]
            }
        ]
    }
}
```

- `tables`: the list of tables to sync.
  - The `modifiedRows` are the rows which have been changed or added to the table since the previous sync.
