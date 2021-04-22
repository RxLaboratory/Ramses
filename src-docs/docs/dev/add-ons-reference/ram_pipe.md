# RamPipe

A pipe which connects two steps together in the production pipeline.

The pipe goes from the output step (which exports data into a specific file type) to the input step (which imports that data).

[TOC]

## Attributes

| Attribute | Type | Default | Description |
| --- | --- | --- | --- |

## Methods

| Method | Arguments | Description |
| --- | --- | --- |
| ***constructor*** | *string*: **inputStepShortName**,<br />*string*: **outputStepShortName**,<br />*[RamFileType](ram_filetype.md)*: **fileType** | |
| **inputStepShortName**<br />▹ *string* | | The short name of the input step |
| **outputStepShortName**<br />▹ *string* | | The short name of the output step |
| **fileType**<br />▹ *[RamFileType](ram_filetype.md)* | | The file type used through the pipe |

____

## API Dev notes

!!! note
    These section is for the development of the API only; you should not need these when developping your add-on using the API.

### Private Static Methods

| Method | Arguments | Description |
| --- | --- | --- |
| **createFromDaemonObj**<br />▹ *RamPipe* | *dict*: **daemonReplyObj** | Use this method to construct a RamPipe with an object got from the Daemon |
| **listFromDaemonReply**<br />▹ *list of RamPipe* | *list*: **daemonReplyContent** | Use this method to construct the list of *RamPipe* got from the Daemon reply content using [`RamDaemonInterface.getPipes()`](ram_daemon_interface.md)<br />This methods just loops through all objects in the list given as an argument, and use `createFromDaemonObj(obj)` on them. |

### Examples

```py
# Python

ramses = Ramses.instance
daemonReply = ramses.daemonInterface().getPipes()
content = daemonReply['content']

pipes = RamPipe.listFromDaemonReply(content) # A list of RamPipe

aPipe = pipes[0] # The first RamPipe
```

____

![META](authors:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2021;updated:2021/04/22)