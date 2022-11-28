![META](authors:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2021-2022;updated:2022/08/23)

# Ramses Server API

The *Ramses Server* is used to backup and sync data across *Ramses Applications* and workstations.

It provides a standard *http(s) REST API*. If you're developping your own client (or contributing to *Ramses Client*), you can easily exchange data with the server using this reference.

**Queries must be a *POST* request**; the body must be *JSON* encoded, the `Content-Type` being `application/json`.

Only one attribute must be set directly in the *URL* (like a *GET* request), the name of the query itself.

For now, the server only supports the **UTF-8** charset, so it is not mandatory to add the charset in the *Content-Type*, and any value other than `utf-8` will be ignored. This may change in future versions.

## Queries

The server provides a single endpoint, which is the root of the folder where it's installed. For example, for a server installed in a folder called *example* on the server at *https://ramses.rxlab.io*, the endpoint will be `https://ramses.rxlab.io/example`.

To query the server, you only need to add the query name as a parameter to this *URL*. For example, the URL for the `ping` query is `https://ramses.rxlab.io/example/?ping`.

All queries must be accompanied with a *JSON* object in the post body, containing at least these values:

```json
{
    "version": "0.6.0"
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
    "content": {},
    "serverUuid": "unique-uid",
    "debug": []
}
```

- `accepted`: **boolean**. True if the server has recognized the query and accepted it. Will be False if the query does not exist.
- `success`: **boolean**. True if the server has successfully processed the query, False in case of any error.
- `message`: **string**. A short feedback about the result, which can be shown to the user by the client.
- `query`: **string**. The query that was made that the server is replying to.
- `content`: **object**. The actual content of the reply, which varies depending on the query. Read this reference for more details.
- `serverUuid`: **string**. The universal unique identifier of this server. This value can be used to make sure a local DB/client connects to the right server and avoid corrupting data by mixing servers.
- `debug`: **array of objects**. Server (debug) logs.

### Debug and logs

The `debug` value of the reply is an array containing these objects:

```json
{
    "date": "2022-11-25 13:05:24",
    "level": "DEBUG",
    "message": "A debug log entry"
}
```

- `date`: **string**. The date and time of the log entry.
- `level`: **string**. One of `"DATA"`, `"DEBUG"`, `"INFO"`, `"WARNING"`, `"CRITICAL"`, `"FATAL"`. The minimum level of the logs is `"WARNING"` by default (e.g. *Data*, *Debug* and *Info* logs won't be returned) and can be changed in the [config file](../../components/server/config.md).
- `lessage`: **string**. The log entry.

## Syncing data

This is the process to sync data with the server.

1. `ping` to check if the server is available and check its version. This is mandatory to initiate the session before being able to log in.
2. `login` to authenticate yourself.
3. `sync` starts the sync session.
4. `push` to push modified items (or an empty list to download all table data).
5. `fetch` to get some information, including the number of tables and rows available to pull.
6. `pull` to pull the updated data from the server.

For the examples in this documentation, we assume the ramses server is available at `https://ramses.rxlab.io/example/`.

!!! note
    For the sake of readability, the *JSON* replies are pretty formatted with indentation and new lines in this documentation, but the server actually uses one-line replies.

### ping

Query: `https://ramses.rxlab.io/example/?ping`

#### Request body

```json
{
    "version": "0.6.0"
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
        "version": "0.6.0"
    },
    "serverUuid": "unique-uid",
    "debug": []
}
```

- `installed` is false if the server has not been correctly installed yet.
- `version` is the version of the server, which should match the version of the client.

### login

Query: `https://ramses.rxlab.io/example/?login`

#### Request body

```json
{
    "version": "0.6.0",
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
    },
    "serverUuid": "unique-uid",
    "debug": []
}
```

- `username`: the user name of the user.
- `uuid`: the uuid of the user.
- `token`: a token which must be associated with `sync` queries later.

### sync

Query: `https://ramses.rxlab.io/example/?sync`

This query starts a sync session. It must be called before pushing data to the server.

#### Request body

```json
{
    "version": "0.8.0",
    "token": "token"
}
```

- `version`: version of the client.
- `token`: the token got with `login`.

#### Reply

The server replies with:

```json
{
    "accepted": true,
    "success": true,
    "message": "Sync session started. You can now push your changes.",
    "query": "sync",
    "content": {},
    "serverUuid": "unique-uid",
    "debug": []
}
```

### push

Query: `https://ramses.rxlab.io/example/?push`

This query is used to push (and commit changes once you're done) data to the server.

It can be called as many times as needed to push all the data, and the last call must include `"commit": true` to commit changes and be able to pull updated data from the server. If the *commit* is not done, nothing is changed on the server.

Only one table at a time can be pushed, and if there are a lot of rows to push, it is advised to split the data in several pushes to the same table.

#### Request body

```json
{
    "version": "0.8.0",
    "token": "token",
    "table": "RamApplication",
    "rows": [
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
            ],
    "previousSyncDate": "2022-06-15 15:44:23",
    "commit": false
}
```

- `version`: version of the client.
- `token`: the token got with `login`.
- `table`: the name of the table with the new data.
- `rows`: the new data.
- `previousSyncDate`: the date of the last previous sync; this is used to limit the data returned by the server to only data more recent than this value. Set it to a past date to download all data from a table.
- `commit`: set this to `true` if it's the last call to push and you're ready to save all changes on the server and get its updated data.

#### Reply

The server replies with:

```json
{
    "accepted": true,
    "success": true,
    "message": "Accepted data. Waiting for commit.",
    "query": "push",
    "content": {
        "commited": false
    },
    "serverUuid": "unique-uid",
    "debug": []
}
```

- `commited`: whether the data has been commited.

### fetch

Query: `https://ramses.rxlab.io/example/?fetch`

Use this query to get information about the available data after a commit. This includes the tables to pull (which should be the ones which have been pushed), and the number of items and pages to pull.

#### Request body

```json
{
    "version": "0.8.0",
    "token": "token"
}
```

- `version`: version of the client.
- `token`: the token got with `login`.

#### Reply

The server replies with:

```json
{
    "accepted": true,
    "success": true,
    "message": "There are 2 tables to pull from the server.",
    "query": "fetch",
    "content": {
        "tableCount": 2,
        "tables": [
            {
                "name": "RamApplication",
                "rowCount": 50,
                "deleteCount": 2,
                "pageCount": 5
            },
            {
                "name": "RamUser",
                "rowCount": 5,
                "deleteCount": 0,
                "pageCount": 1
            },
        ]
    },
    "serverUuid": "unique-uid",
    "debug": []
}
```

- `tableCount`: the number of available tables.
- `tables`: the table information, including:
    - `name`: the name of the table
    - `rowCount`: the number of updated/inserted rows
    - `deleteCount`: the number of rows which have been deleted
    - `pageCount`: the number of pages to pull. To improve performance, you can't pull all rows at once, but one page at a time.

### pull

Query: `https://ramses.rxlab.io/example/?pull`

Use this query to pull the new data from the server.

To improve performance, you can't pull all rows and all tables at once, but one page for one table at a time.

#### Request body

```json
{
    "version": "0.8.0",
    "token": "token",
    "table": "RamApplication",
    "page": 1
}
```

- `version`: version of the client.
- `token`: the token got with `login`.
- `table`: the name of the table to pull.
- `page`: the page to pull.

#### Reply

The server replies with:

```json
{
    "accepted": true,
    "success": true,
    "message": "Retrieved the 'RamApplication' table data, for page #1 (rows 0 to 9).",
    "query": "pull",
    "content": {
        "table": "RamApplication",
        "page": 1,
        "rows": [
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
        ],
        "deleted": [
            "uuid-1", "uuid-2"
        ]
    },
    "serverUuid": "unique-uid",
    "debug": []
}
```

- `table`: the name of the table.
- `page`: the page number.
- `rows`: the new and updated rows.
- `deleted`: the list of UUIDs which have been deleted from the table.

### pull (single object)

Query: `https://ramses.rxlab.io/example/?pull`

The *pull* query can also be used to get the data of a single object, using its UUID.


#### Request body

```json
{
    "version": "0.8.0",
    "token": "token",
    "table": "RamApplication",
    "uuid": "123456"
}
```

- `version`: version of the client.
- `token`: the token got with `login`.
- `table`: the name of the table to pull.
- `uuid`: the UUID of the object to pull.

#### Reply

The server replies with:

```json
{
    "accepted": true,
    "success": true,
    "message": "Retrieved the 'RamApplication' table data, for page #1 (rows 0 to 9).",
    "query": "pull",
    "content": {
        "uuid": "123456",
        "data": "{some data}",
        "modified": "2022-07-15 15:44:23",
        "removed": 0
    },
    "serverUuid": "unique-uid",
    "debug": []
}
```

### clean

Query: `https://ramses.rxlab.io/example/?clean`

Deletes rows from tables, using the objects' uuids.

#### Request body

```json
{
    "version": "0.6.0",
    "token": "token",
    "tables": [
        {
            "name": "tableName",
            "rows": ["uuid1", "uuid2", "uuid3"]
        },
        {
            "name": "otherTable",
            "rows": ["uuid4", "uuid5", "uuid6"]
        }
    ]
}
```

#### Reply

The server replies with:

```json
{
    "accepted": true,
    "success": true,
    "message": "Cleaned data, removed 6 rows.",
    "query": "clean",
    "content": {
        "count": 6
    },
    "serverUuid": "unique-uid",
    "debug": []
}
```

- `count`: the number of deleted rows.

### setPassword

Query: `https://ramses.rxlab.io/example/?setPassword`

#### Request body

```json
{
    "version": "0.6.0",
    "token": "token",
    "newPassword": "new password",
    "currentPassword": "current password",
    "uuid": "user-unique-uid"
}
```

- `version`: version of the client.
- `token`: the token got with `login`.
- `newPassword`: the new password to set.
- `currentPassword`: the current password of the user.
- `uuid`: the user *UUID*.

The server will check if the connected user is an administrator, and accept the request accordingly:

- If the connected user is the same as the user changing its password, the current password is mandatory and will be checked before making any change.
- If the connected user is not the same as the new password, the request will be accepted if and only if the connected user has the *administrator* role. In this case, the current password is not mandatory.

That means if there's only one administrator, who has forgotten its current password, only the server provider - someone who has an actual access to the server files (via *FTP* for example) - can set a new password, using *Ramses Server* development tools.