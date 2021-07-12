![META](authors:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2021;updated:2021/07/12)

# Ramses Server API

The *Ramses* server provides a standard *http(s) REST API*. If you're developping your own client (or contributing to *Ramses Client*), you can easily exchange data with the server using this reference.

**Queries must be a *POST* request**; the body can be either *JSON* or *URL* encoded, the `Content-Type` being respectively `application/json` or `application/x-www-form-urlencoded`. However, some queries (explained in this document) are not supported in the *URL* encoded form, so it is better to **always use *JSON* if possible**.

!!! tip
    In *URL* encoded queries, all special characters should be percent-encoded (e.g. by replacing `/` with `%2F`).

Only one attribute must be set directly in the *URL* (like a *GET* request), the name of the query itself.

For now, the server only supports the **UTF-8** charset, so it is not mandatory to add the charset in the *Content-Type*, and any value other than `utf-8` will be ignored. This may change in future versions.

Here is a minimal example of a query using *URL* encoding:

*URL*: `http://your.server/ramses/?createStep`

```
POST /ramses HTTP/1.1
Host: your.server
Content-Type: application/x-www-form-urlencoded
User-Agent: Ramses v0.1.1-Alpha
Cookie: PHPSESSID=u3jb5ep3bvbrm29kutpue7t4ds
Content-Length: 153

uuid=b615b4be-66fd-5fd3-8095-2074155a29a8&shortName=ANIM&name=Animation&type=shot&comment=&color=%23c992ff&token=94393b8e6396fb2bb295552d6d122544d5db1d1f
```

And this is the same query using *JSON* encoding:

*URL*: `http://your.server/ramses/?createStep`

```
POST /ramses HTTP/1.1
Host: your.server
Content-Type: application/json
User-Agent: Ramses v0.1.1-Alpha
Cookie: PHPSESSID=u3jb5ep3bvbrm29kutpue7t4ds
Content-Length: 181

{"uuid":"b615b4be-66fd-5fd3-8095-2074155a29a8","shortName":"ANIM","name":"Animation","type":"shot","comment":"","color":"#c992ff","token":"94393b8e6396fb2bb295552d6d122544d5db1d1f"}
```

The parameters to use in the body are described in the next sections of this reference. The *Cookie* is set by the server on the first connection, the *User-Agent* is set by the client.

The server replies with a *JSON* object in the body.

For almost all queries (i.e. all but *ping* and *login*) you'll need to be logged in. Initiating a connection to the server usually follows these steps:

1. *ping* to check if the server is available and check its version.
2. *login* to authenticate yourself and get a session token.
3. *init* to get the main data (user list, project list...).

For the examples in this documentation, we assume the ramses server is available at `http://your.server/ramses/`.

For the sake of readability, the *JSON* replies are pretty formatted with indentation and new lines in this documentation, but the server actually uses one-line replies.

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

    There is no query yet to restore data which has been removed, but using the `create` queries with the same *uuid* as the removed item will restore it.

- General
    - [ping](general.md#ping)
    - [login](general.md#login)
    - [init](general.md#init)
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
    - [assignUser](projects#assignuser)
    - [unassignUser](projects#unassignuser)
- Steps and Template Steps
    - [createStep](steps#createstep)
    - [updateStep](steps#updatestep)
    - [setSteporder](steps#setsteporder)
    - [moveStep](steps#movestep)
    - [removeStep](steps#removestep)
    - [setStepEstimations](steps#setstepestimations)
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
    - [setAssetStatus](assets#setassetstatus)
- Sequences
    - [createSequence](sequences#createsequence)
    - [updateSequence](sequences#updatesequence)
    - [removeSequence](sequences#removesequence)
    - [setSequenceOrder](sequences#setsequenceorder)
- Shots
    - [createShot](shots#createshot)
    - [updateShot](shots#updateshot)
    - [setShotOrder](shots#setshotorder)
    - [moveShot](shots#movehot)
    - [removeShot](shots#removeshot)
    - [setShotStatus](shots#setshotstatus)
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
- PipeFiles
    - [createPipeFile](pipefiless#createpipefile)
    - [updatePipeFile](pipefiless#updatepipefile)
    - [removePipeFile](pipefiless#removepipefile)
- Pipes
    - [createPipe](pipes#createpipe)
    - [updatePipe](pipes#updatepipe)
    - [removePipe](pipes#removepipe)
- Status
    - [updateStatus](status#updatestatus)
    - [removeStatus](status#removestatus)
    - [setStatusUser](status#setstatususer)
- Schedule Entrries
    - [createSchedule](schedule#createschedule)
    - [createSchedules](schedule#createschedules)
    - [updateSchedule](schedule#updateschedule)
    - [updateSchedules](schedule#updateschedules)
    - [removeSchedule](schedule#removeschedule)
    - [removeSchedules](schedule#removeschedules)