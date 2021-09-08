# RamFileManager

Generic tools to handle files and the [*Ramses Naming Scheme*](../../components/files/naming.md)

Read the sections about the *[Ramses Tree](../../components/files/tree.md)* and *[Naming Scheme](../../components/files/naming.md)* for more details about names and folders used by *Ramses*.

## Static Methods

| Method | Arguments | Description |
| --- | --- | --- |
| **buildPath**<br />▹ *boolean* | *list of string*: **folders** | Builds a path with a list of folder names or subpaths, adding the `"/"` only if needed. Paths never include any trailing `"/"` |
| **copyToPublish**<br />▹ *string* | *string*: **filePath** | Copies the given file inside the corresponding `_published` subfolder.<br />Returns the path to the new file. |
| **copyToVersion**<br />▹ *string* | *string*: **filePath**,<br />*boolean*: **increment**=`false`,<br />*string*: **dafaultStateShortName**=`"v"` | Copies the given file inside the corresponding `_versions` subfolder, optionnaly incrementing the version number.<br />Use *defaultStateShortName* to change the state to be used if not found in the current or previous version.<br />Returns the path to the new file. |
| **getLatestVersion**<br />▹ *list of int and string* | *string*: **filePath**,<br />*string*: **dafaultStateShortName**=`"v"`,<br />*boolean*: **previous**=`false` | Gets the latest version number and state for the given file, which can be located either in the main folder or one of the *_versions*, *_preview* or *_published* subfolder.<br />Use *defaultStateShortName* to change the state to be used if not found in the current or previous version.<br />If *previous* is true, will return the version immediately before the latest one.<br />Returns a list with the version number as first item, the state as second item, and the date as the third item. |
| **getLatestVersionFilePath**<br />▹ *string* | *string*: **filePath**,<br />*boolean*: **previous**=`false` | Gets the latest version file path for the given file, which can be located either in the main folder or one of the *_versions*, *_preview* or *_published* subfolder.<br />If *previous* is true, will return the version immediately before the latest one. |
| **getProjectFolder**<br />▹ *string* | *string*: **path** | Gets the root folder of the project this path belongs to. |
| **getPublishFolder**<br />▹ *string* | *string*: **filePath** | Gets the *_published_* folder path for any given file. |
| **getRamsesFiles**<br />▹ *list* of *string* | *string*: **folderPath**, <br />*string*: **resource**=`None` | Gets all the files respecting the *Ramses* naming scheme inside the folder, optionally corresponding to the given resource only. |
| **getSaveFilePath**<br />▹ *string* | *string*: **filePath** | Gets and returns the path to be used to save the file given as argument, with a name respecting the [*Ramses Naming Scheme*](../../components/files/naming.md). If the file is in a *_versions*, *_preview* or *_published* folder, the save file is located in the parent folder. |
| **getVersionFilePaths**<br />▹ *string* | *string*: **filePath** | Gets the corresponding version files from the *_versions* folder path for any given file. |
| **getVersionFolder**<br />▹ *string* | *string*: **filePath** | Gets the *_versions* folder path for any given file. |
| **inPreviewFolder**<br />▹ *boolean* | *string*: **filePath** | Checks if a given file is located in a *_preview* folder. |
| **inPublishFolder**<br />▹ *boolean* | *string*: **filePath** | Checks if a given file is located in a *_published* folder. |
| **inReservedFolder**<br />▹ *boolean* | *string*: **filePath** | Checks if a given file is located in a *_published* or *_versions* or *_preview* folder. |
| **inVersionsFolder**<br />▹ *boolean* | *string*: **filePath** | Checks if a given file is located in a *_versions* folder. |
| **isAssetStep**<br />▹ *boolean* | *string*: **stepShortName**,<br />*string*: **assetsPath** | Checks using the information found in `assetsPath` (which should be the assets folder of a given project) if the given short name corresponds to an asset production step. |
| **isProjectFolder**<br />▹ *boolean* | *string*: **folderPath** | Checks if a given folder is the project root folder. |
| **isRestoredFilePath**<br />▹ *boolean* or *int* | *string*: **filePath** | Checks if the file is a working file which has been restored from a previous version (contains `"+restored-vXX+"` in its resource name).<br />If it is a restored older version, the version number is returned, otherwise returns `false`. |
| **isShotStep**<br />▹ *boolean* | *string*: **shotShortName**,<br />*string*: **shotsPath** | Checks using the information found in `shotsPath` (which should be the shots folder of a given project) if the given short name corresponds to a shot production step. |
| **restoreVersionFile** | *string*: **filePath** | Restores an older version. |
| **validateName**<br />▹ *boolean* | *string*: **name** | Checks if this name respects the Ramses naming scheme. |
| **validateShortName**<br />▹ *boolean* | *string*: **shortName** | Checks if this short name respects the Ramses naming scheme. |

____

## API Dev notes

!!! note
    These section is for the development of the API only; you should not need these when developping your add-on using the API.

### Private methods

| Method | Arguments | Description |
| --- | --- | --- |
| **_isRamsesItemFoldername**<br />▹ *boolean* | *string*: **n** | Checks if a given folder respects Ramses' naming convention for items' root folders.<br />The root folder should look like this:<br />`"projectID_itemType_objectID"` |

![META](authors:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2021;updated:2021/06/10)
