![META](authors:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2021;updated:2021/09/08)

# RamMetaDataManager

A Class to get/set metadata from files.

*Ramses* will use a single sidecar file named `_ramses_data.json` (storing data in *json*) for all files located in a given folder;  
thus the metadata used by *Ramses* is set on a per-folder basis, and is not copied when a file is copied/moved: it does not make sense for *Ramses* to have the same metadata when a file is moved.

## The Meta-Data structure

The Meta-Data used by *Ramses* contains at least these attributes.

| Key Name | Type | Default Value | Description |
| --- | --- | --- | --- |
| [**`MetaDataKeys.COMMENT`**](enum.md) | *string* | `""` | A comment associated to the file |
| [**`MetaDataKeys.PIPE_TYPE`**](enum.md) | *int* | `""` | The type of the pipe associated to the file |
| [**`MetaDataKeys.VERSION`**](enum.md) | *int* | `-1` | A version associated to the file |
| [**`MetaDataKeys.VERSION_FILE`**](enum.md) | *string* | `""` | A version file associated to the file |

## Static Methods

| Method | Arguments | Description |
| --- | --- | --- |
| **getComment**<br />▹ *string* | *string*: **filePath** | Gets the comment associated to the file. |
| **getDate**<br />▹ *datetime* | *string*: **filePath** | Gets the date associated to the file. |
| **getFileMetaData**<br />▹ *object* or *dict* | *string*: **filePath** | Gets the meta-data of the given file. |
| **getMetaData**<br />▹ *object* or *dict* | *string*: **folderPath** | Gets the meta-data of all the files for the given folder.<br />Each file name is used as a key in the returned object. |
| **getMetaDataFile**<br />▹ *string* | *string*: **path** | Gets the path of the file containing the metadata for the given file or folder. |
| **getPipeType**<br />▹ *string* | *string*: **filePath** | Gets the type of the pipe which created this file (if it has previously been set by the addon using `setPipeType()`). |
| **getState**<br />▹ *string* | *string*: **filePath** | Gets the state short name of the file. |
| **getValue**<br />▹ *any* | *string*: **filePath**,<br />*string*: **key** | Gets the value of a specific key for the file. |
| **getVersion**<br />▹ *int* | *string*: **filePath** | Gets the version associated to the file. |
| **getVersionFilePath**<br />▹ *string* | *string*: **filePath** | Gets the version file associated to the file. |
| **setComment**<br />▹ *string* | *string*: **filePath**,<br />*string*: **comment** | Sets the comment associated to a file. |
| **setDate**<br />▹ *string* | *string*: **filePath**,<br />*date* or *datetime* or *int* or *string*: **date** | Sets the date associated to a file. |
| **setFileMetaData**<br /> | *string*: **filePath**,<br />*object* or *dict*: **fileData** | Writes the given meta-data to the meta-data file associated to this file. |
| **setMetaData**<br /> | *string*: **path**,<br />*object* or *dict*: **data** | Writes the given meta-data to the meta-data file associated to this folder. |
| **setPipeType**<br /> | *string*: **filePath**,<br />*string*: **pipeType** | Sets the pipe which created the file. The value should be the short name of the pipe. |
| **setState**<br /> | *string*: **filePath**,<br />*string*: **state** | Sets the state for the file. |
| **setValue** | *string*: **filePath**,<br />*string*: **key**,<br />*any*: **value** | Sets the value of a specific key for the file. |
| **setVersion** | *string*: **filePath**,<br />*int*: **version** | Sets the version associated to the file. |
| **setVersionFilePath** | *string*: **filePath**,<br />*string*: **versionFilePath** | Sets the version file associated to the file. |
