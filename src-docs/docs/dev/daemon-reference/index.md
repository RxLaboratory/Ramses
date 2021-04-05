# Ramses Daemon API

The *Ramses Daemon* is a local server used by the *Ramses Add-ons* to get and set data to the client (which in turn communicates with the *Ramses Server* to store the shared data) and the whole system.

[TOC]

The *Ramses Daemon* is automatically run by the *Ramses client*, the native application which is the heart of *Ramses*.

## Connecting to the Daemon

*Add-ons* can communicate with the *Daemon* through a standard *TCP Socket*. The *Daemon* listens on port `18185`[^1] by default (but this can be adjusted in the *Ramses Client* settings), and is automatically started by the *Ramses client*. It is terminated when the *client* quits.

Most of *Ramses* data can only be accessed if a user is logged-in in the *Ramses Client*, thus before communicating with the *Daemon*, a *Ramses Add-on* must check if the *Client* (and its *Daemon*) is running (or start it), and if a user is logged-in.

The initial connection to the Daemon usually follows these steps:

1. Check if the *Daemon* is running by sending the `ping` query.
    1. If the *Daemon* replies, connection is ready
    2. Else, try to start the client and `ping` again.
2. Check the version of the *Daemon* which is part of the reply to the `ping`.
3. Check if the user is logged-in; if not, show an alert and raise the *Ramses Client* window.

## Communicating with the Daemon

### Queries

*Add-ons* can post queries which use the form of the attributes part of a standard *URL* query; they are a single-line string of `key=value` pairs (the value may be optional) seperated by the `&` character. They should be encoded in *UTF-8*, and special characters should use a standard *URL percent-encoding*. There's no need to enclose strings in quotes.

Example queries:

`ping`

`setCurrentProject&shortName=PROJ&name=The Project`

### Replies

The *Daemon* replies with a *JSON* object (encoded in *UTF-8*) which contains the following attributes:

- *accepted*: **boolean**. True if the *Daemon* has recognized the query and accepted it. Will be False if the query does not exist.
- *success*: **boolean**. True if the *Daemon* has successfully processed the query, False in case of any error.
- *message*: **string**. A short feedback about the result, which can be shown to the user by the *Add-on*. It contains explanation about the error, if any.
- *query*: **string**. The query that was made that the *Daemon* is replying to.
- *content*: **object**. The actual content of the reply, which varies depending on the query. Read this reference for more details.

!!! note
    Replies are a single-line string, but they're prettyfied in this documentation for the sake of readabiliy

Example Replies:

```json
{
    "accepted": true,
    "query": "ping",
    "success": true,
    "message": "Hi, this is the Ramses Daemon.",
    "content": {
        "version": "0.0.1",
        "userName": "John Doe",
        "userShortName": "J.Doe",
        "logged-in": true
    }
}
```

```json
{
    "accepted": true,
    "query": "setCurrentProject",
    "success": true,
    "message": "Current project set to: The Project.",
    "content": {
        "name": "The Project",
        "shortName": "PROJ",
        "path": "/home/user/Ramses/PROJECTS/The Project",
        "uuid": "12456abc"
    }
}
```

[^1]:
    This port was chosen in memory of Karl Marx, whose birth date was 05/05/1818.

![META](authors:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2021;updated:2021/04/05)