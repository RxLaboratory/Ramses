# RamMetaDataManager

A Class to get/set metadata from files.

*Ramses* will use a single sidecar file named `_ramses_data.json` (storing data in *json*) for all files located in a given folder;  
thus the metadata used by *Ramses* is set on a per-folder basis, and is not copied when a file is copied/moved: it does not make sense for *Ramses* to have the same metadata when a file is moved.

## The Meta-Data structure

The Meta-Data used by *Ramses* contains these attributes.

| Key | Type | Default Value | Description |
| --- | --- | --- | --- |
| **comment** | *string* | `""` | A comment associated to the file |

## Static Methods

| Method | Arguments | Description |
| --- | --- | --- |
| **getComment**<br />▹ *string* | *string*: **filePath** | Gets the comment associated to the file. |
| **setComment**<br />▹ *string* | *string*: **filePath**,<br />*string*: **comment** | Sets the comment associated to a file. |
| **getMetaDataFile**<br />▹ *string* | *string*: **path** | Gets the path of the file containing the metadata for the given file or folder. |
| **getFileMetaData**<br />▹ *object* or *dict* | *string*: **filePath** | Gets the meta-data of the given file. |
| **getMetaData**<br />▹ *object* or *dict* | *string*: **path** | Gets the meta-data of all the files for the given folder.<br />Each file name is used as a key in the returned object. |
| **setMetaData**<br /> | *string*: **path**,<br />*object* or *dict*: **data** | Writes the given meta-data to the meta-data file associated to this folder. |

![META](authors:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2021;updated:2021/05/28)
