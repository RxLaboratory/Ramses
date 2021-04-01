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

!!! note
    `remove` queries do not actually delete data from the database, but set a `removed` attribute to true, to prevent any data loss.  
    For now, the only way to permanently delete data is to edit the database by yourself (using SQL queries or MySQL workbench or phpmyadmin, etc.)

    There is no query yet to restore data which has been removed, but using the `create` queries with the same name and shortname as the removed item will restore it.

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
- Steps and Template Steps
    - [createStep](steps#createstep)
    - [updateStep](steps#updatestep)
    - [removeStep](steps#removestep)
    - [assignUser](steps#assignuser)
    - [unassignUser](steps#unassignuser)
    - [assignApplication](steps#assignapplication)
    - [unassignApplication](steps#unassignapplication)
- Asset Groups and Template Asset Groups
    - [createAssetGroup](assetgroups#createassetgroup)
    - [updateAssetGroup](assetgroups#updateassetgroup)
    - [removeAssetGroup](assetgroups#removeassetgroup)
- Assets
    - [createAsset](assets#createasset)
    - [updateAsset](assets#updateasset)
    - [removeAsset](assets#removeasset)
- Sequences
    - [createSequence](sequences#createsequence)
    - [updateSequence](sequences#updatesequence)
    - [removeSequence](sequences#removesequence)
- States
    - [createState](states#createstate)
    - [getStates](states#getstates)
    - [updateState](states#updatestate)
    - [removeState](states#removestate)
- File Types
    - [createFileType](filetypes#createfiletype)
    - [getFileTypes](filetypes#getfiletype)
    - [updateFileType](filetypes#updatefiletype)
    - [removeFileType](filetypes#removefiletype)
- Applications
    - [createApplication](applications#createapplication)
    - [getApplications](applications#getapplication)
    - [updateApplication](applications#updateapplication)
    - [removeApplication](applications#removeapplication)
    - [assignFileType](steps#assignfiletype)
    - [unassignFileType](steps#unassignfiletype)
- Pipes
    - [createPipe](pipes#createpipe)
    - [updatePipe](pipes#updatepipe)
    - [removePipe](pipes#removepipe)