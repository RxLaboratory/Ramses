# Ramses Server API

The *Ramses* server provides a standard *http(s) REST API*. If you're developping your own client (or contributing to *Ramses Client*), you can easily exchange data with the server using this reference.

Queries are made by building a *URI* containing all information, and the server replies with a *JSON* object in the body. For almost all queries (i.e. all but *ping* and *login*) you'll need to be logged in. Initiating a connection to the server usually follows these steps:

1. *ping* to check if the server is available and check its version.
2. *login* to authenticate yourself and get a session token.

For the examples in this documentation, we assume the ramses server is available at `http://your.server/ramses/`.

For the sake of readability, the *JSON* replies are formatted with indentation and new lines in this documentation, but the server actually uses one-line replies.

## Server Replies

The *JSON* objects sent by the server all have the same content:

- *accepted*: **boolean**. True if the server has recognized the query and accepted it. Will be False if the query does not exist.
- *success*: **boolean**. True if the server has successfully processed the query, False in case of any error.
- *message*: **string**. A short feedback about the result, which can be shown to the user by the client.
- *query*: **string**. The query that was made that the server is replying to.
- *content*: **object**. The actual content of the reply, which varies depending on the query. Read this reference for more details.

## Queries

- General
    - [ping](general.md#ping)
    - [login](general.md#login)
- Users
    - [updateUser](users.md#updateuser)
    - [updatePassword](users.md#updatepassword)
    - [getUsers](users.md#getusers)
    - [createUser](users.md#createuser)
    - [removeUser](users.md#removeuser)
- Projects
    - [createProject](projects#createproject)
    - [getProjects](projects#getprojects)
    - [updateProject](projects#updateproject)
    - [removeProject](projects#removeproject)
    - [assignStep](projects#assignstep)
    - [unassignStep](projects#unassignstep)

