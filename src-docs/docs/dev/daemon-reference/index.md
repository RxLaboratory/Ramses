![META](authors:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2022;updated:2022/09/01)

# Ramses Daemon API

The *Ramses Daemon* is a local server used by the *Ramses Add-ons* to get and set data to the client (which in turn may communicate with the *Ramses Server* to sync data) and the whole system.

The *Ramses Daemon* is automatically run by the *Ramses Application*, which is the beating heart of *Ramses*.

## Connecting to the Daemon

*Add-ons* can communicate with the *Daemon* through a standard *TCP Socket*. The *Daemon* listens on port `18185`[^1] by default (but this can be adjusted in the *Ramses Application* settings), and is automatically started by the *Ramses Application*. It is terminated when the *client* quits. To avoid conflicts, it may be temporarily suspended when the client is syncing data with the *Ramses Server* but all requests are queued and all of them should be processed.

Most of the *Ramses* data can only be accessed if a user is logged-in in the *Ramses Application*, thus before communicating with the *Daemon*, a *Ramses Add-on* must check if the *Application* (and its *Daemon*) is running (or start it), and if a user is logged-in.

The initial connection to the Daemon usually follows these steps:

1. Check if the *Daemon* is running by sending the [`ping`](ping.md) query.
    1. If the *Daemon* replies, connection is ready
    2. Else, try to start the *Application* and [`ping`](ping.md) again.
2. Check the version of the *Daemon* which is part of the reply to the [`ping`](ping.md).
3. Check if a user is logged-in (this is also part of the reply to the [`ping`](ping.md)); if not, show an alert and raise the *Ramses Application* window.

After these steps, other queries can be used to get or set new data and information.

## Communicating with the Daemon

### Queries

*Add-ons* can post queries with the form of a standard *form-encoded URL* query; they are a single-line string of `key=value` pairs (the value may be optional) seperated by the `&` character. They should be encoded in *UTF-8*. There's no need to enclose strings in quotes.

Example queries:

[`ping`](ping.md)

[`getProjects`](getProjects.md)

[`setCurrentProject&uuid=unique-uid`](setCurrentProject.md)

### Replies

The *Daemon* replies with a *JSON* object (encoded in *UTF-8*) which contains the following attributes:

- *accepted*: **boolean**. True if the *Daemon* has recognized the query and accepted it. Will be False if the query does not exist.
- *success*: **boolean**. True if the *Daemon* has successfully processed the query, False in case of any error.
- *message*: **string**. A human-readable feedback about the result, which can be shown to the user by the *Add-on*. It contains explanation about the error, if any.
- *query*: **string**. The query that was made that the *Daemon* is replying to.
- *content*: **object**. The actual content of the reply, which varies depending on the query. Read this reference for more details.

!!! note
    Replies are a single-line string, but they're prettyfied in this documentation for the sake of readabiliy

A reply looks like this:

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

### Objects and their data

In Ramses, everything is represented by *Objects*. An object consists of a ***Unique Universal Identifier (UUID)*** used to identify them across the whole system, and its associated ***data***, encoded as a *JSON Object*.

Ramses Objects (i.e. *RamObjects*) can represent a shot, an asset, a user, an application, or a whole project for example. They can also contain lists of other objects. Lists of objects are basic string lists of *UUIDs* stored in the parent object data.

*UUIDs* and object data are synced by all the *Ramses Applications* and a *Ramses Server*, and are mirrored on all workstations and the server itself. Any other software (e.g. *APIs*, scripts, plugins...) should get the list of objects and all data using this *Ramses Daemon* run by the *Ramses Application* which is guaranteed to be up-to-date. You can also use the *Daemon* to set new data to specific objects.

Data can be retrived and updated with these two simple queries: [`getData`](getData.md) and [`setData`](setData.md).

[^1]:
    This port was chosen in memory of Karl Marx, whose birth date was 05/05/1818.