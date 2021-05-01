# RamFileManager

Generic tools to handle files and the [Ramses Naming Scheme](../../files/naming.md)

Read the sections about the *[Ramses Tree](../../files/tree.md)* and *[Naming Scheme](../../files/naming.md)* for more details about names and folders used by *Ramses*.

[TOC]

## Static Methods

| Method | Arguments | Description |
| --- | --- | --- |
| **increment**<br />▹ *string* | *string*: **filePath**,<br />*string*: **stateShortName**=`"v"` | Increments and copies a new version of the given file inside the corresponding `_versions` subfolder. Returns the path to the new file. |
| **decomposeRamsesFileName**<br />▹ *dict* or *None* | *string*: **ramsesFileName** | Used on files that respect Ramses' naming convention: it separates the name into blocks (one block for the project's shortname, one for the step, one for the extension...). |
| **composeRamsesFileName**<br />▹ *dict* | *string*: **ramsesFileNameDict**,<br />*boolean*: **increment**=`false` |Builds a filename from a dict similar as the one returned by `decomposeRamsesFileName()`. |
| **buildRamsesFileName**<br />▹ *string* | *string*: **project**,<br />*string*: **step**,<br />*string*: **ext**,<br />*string*: **ramType**=`"G"`,<br />*string*: **objectShortName**=`""`,<br />*string*: **resourceStr**=`""`,<br />*int*: **version,**=`-1`,<br />*string*: **version_prefix**=`""` | Builds a filename respecting Ramses' naming conventions. |

### decomposeRamsesFileName( )

Used on files that respect Ramses' naming convention: it separates the name into blocks (one block for the project's shortname, one for the step, one for the extension...)

A Ramses filename can have all of these blocks:

`projectID_ramType_objectShortName_ramStep_resourceStr_versionBlock.extension`

- ramType can be one of the following letters: A (asset), S (shot), G (general).
- there is an objectShortName only for assets and shots.
- resourceStr is optional. It only serves to differentiate the main working file and its resources, that serve as secondary working files.
- versionBlock is optional. It's made of two blocks: an optional version prefix, also named state, followed by a version number.  
    Version prefixes consist of all the available states' shortnames ( see [`Ramses.getStates()`](ramses.md) ) and some additional prefixes. E.g. `"wip"`, `"v"`, ...

If the file does not match Ramses' naming convention, returns None.  
Else, returns a dictionary made of all the blocks:

```py
{
    "projectId",
    "ramType",
    "objectShortName",
    "ramStep",
    "resourceStr",
    "state",
    "version",
    "extension"
}
```

____

![META](authors:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2021;updated:2021/05/01)
