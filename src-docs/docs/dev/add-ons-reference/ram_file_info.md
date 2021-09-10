![META](authors:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2021;updated:2021/09/08)

# RamFileInfo

A class to easily handle Ramses names using the [*Ramses Naming Scheme*](../../components/files/naming.md).

Read the sections about the *[Naming Scheme](../../components/files/naming.md)* for more details about names used by *Ramses*.

## Attributes

These attributes are automatically set by the `RamFileInfo.setFileName( name )` or `RamFileInfo.setFilePath( name )` methods; They can then be changed and a new file name can be generated using `RamFileInfo.fileName( )` or `RamFileInfo.filePath( )`

| Attribute | Type | Default | Description |
| --- | --- | --- | --- |
| **date** | *datetime* | undefined | The modification date of the file. |
| **extension** | *string* | `""` | The file extension (without the leading `.`). |
| **isRestoredVersion** | *boolean* | `false` | True if the corresponding file is a file created from restoring a previous version. |
| **project** | *string* | `""` | The project short name (ID). |
| **ramType** | *[ItemType](enum.md#itemtype)* | `""` | The type of the corresponding item. |
| **resource** | *string* | `""` | The resource string. |
| **restoredVersion** | *boolean* | `-1` | The version which was restored if `isRestoredVersion` is `true`. |
| **shortName** | *string* | `""` | The short name (ID) of the item. |
| **state** | *string* | `""` | The state short name (ID), if any. |
| **step** | *string* | `""` | The step short name (ID). |
| **version** | *int* | `-1` | The versin, `-1` if not set/found. |

## Methods

| Method | Arguments | Description |
| --- | --- | --- |
| ***constructor*** |  | Creates a new empty instance. |
| **copy**<br />▹ *RamFileInfo* |  | Creates a copy of the current instance. |
| **fileName**<br />▹ *string* |  | Constructs and returns the file name based on the attributes. |
| **filePath**<br />▹ *string* |  | Constructs and returns the file path based on the attributes. |
| **originalFileName**<br />▹ *string* |  | Gets the original file name without any modification. |
| **originalFilePath**<br />▹ *string* |  | Gets the original file path if it was set, without any modification. |
| **setFileName**<br />▹ *boolean* |  | Sets the filename and parses data from it. Returns `true` on success. |
| **setFilePath** |  | Parses all data which can be found from a file path. |

## Special

### Convert to string

*RamFileInfo*s can be converted to string the usual way. This is equivalent to calling `RamFileInfo.fileName()`.

```py
# Python

info = RamFileInfo( )
info.setFileName( aFileName )

print( info ) # implicit conversion
print( str(info) ) # explicit conversion

# Both will print:
>>> PROJ_A_anAsset_aStep_aResource.ext
```

```js
// ExtendScript

var info = new RamFileInfo( );
info.setFileName( aFileName );

$.writeln( obj.toString() );

// Prints:
// PROJ_A_anAsset_aStep_aResource.ext

```

### Comparing objects

*RamFileInfo* can be compared to check if two file info are the same, by comparing the attributes **except the state and the version**. That means all different versions of the same file are considered equal. The location is not compared either: two identical files but in different locations are considered the same.

```py
# Python

info = RamFileInfo( )
info.setFileName( aFileName )
other = RamFileInfo( )
other.setFileName( anotherFileName )
same = RamFileInfo( )
same.setFileName( aFileNameButAnotherVersionOrLocation )

# RamFileInfo can be compared as any other value

info == other # False
info == same # True

# It can be used with the `in` operator too
infoList = [ info, other ]
same in infoList # True

# But RamFileInfo is not hashable, so it cannot be used in a `set` or as a key in a `dict`

```

```js
// ExtendScript

var info = RamFileInfo( );
info.setFileName( aFileName );
var other = RamFileInfo( );
other.setFileName( anotherFileName );
var same = RamFileInfo( );
same.setFileName( aFileNameButAnotherVersionOrLocation );

// RamObject can be compared with the is() method

info.is(other) // false
info.is(same) // true

```

## Examples

```py
# Python

from ramses import RamFileInfo, ItemType

# Get info from an existing asset file
filePath = '/path/to/PROJ_A_MyAsset_RIG_aResource.blend'
nm = RamFileInfo()
if nm.setFileName(filePath:
    nm.version = 12 # Set the version to 12
    nm.state = "OK" # Set thje state
    print( nm.step ) # Prints: RIG
    print( nm.fileName() ) # Prints: PROJ_A_MyAsset_RIG_aResource_OK012.blend

# Build a filename from scratch
nm = RamFileInfo()
nm.project = "OTHER"
nm.shortName = "MyShot"
nm.ramType = ItemType.SHOT
nm.step = "ANIM"
nm.extension = "kra"
print( nm.fileName() ) # Prints: OTHER_S_MyShot_ANIM.kra

# A function to publish a file
# Note that this is actually the implementation of RamFileManager.publishFile()
def publishFile( filePath ):
    """Copies the given file to its corresponding publish folder"""
    fileInfo = RamFileManager.getPublishInfo( filePath )
    if fileInfo.project == "":
        return

    publishFilePath = fileInfo.filePath()
    RamFileManager.copy( filePath, publishFilePath )
    # Keep the date in the metadata, just in case
    RamMetaDataManager.setDate( publishFilePath, fileInfo.date )

    return publishFilePath
```

```js
// ExtendScript

#include ramses.jsxinc

// Get info from an existing asset file
var filePath = '/path/to/PROJ_A_MyAsset_RIG_aResource.blend';
var nm = new RamFileInfo();
if ( nm.setFileName(filePath) ) {
    nm.version = 12; // Set the version to 12
    nm.state = "OK"; // Set thje state
    alert( nm.step ); // Alerts: RIG
    alert( nm.fileName() ); // Alerts: PROJ_A_MyAsset_RIG_aResource_OK012.blend
}

// Build a filename from scratch
nm = RamFileInfo();
nm.project = "OTHER";
nm.shortName = "MyShot";
nm.ramType = ItemType.SHOT;
nm.step = "ANIM";
nm.extension = "kra";
alert( nm.fileName() ); // Alerts: OTHER_S_MyShot_ANIM.kra

// A function to publish a file
// Note that this is actually the implementation of RamFileManager.publishFile()
function publishFile( filePath ) {
    // Copies the given file to its corresponding publish folder
    var fileInfo = RamFileManager.getPublishInfo( filePath );
    if (fileInfo.project == "") return

    var publishFilePath = fileInfo.filePath();
    RamFileManager.copy( filePath, publishFilePath );
    // Keep the date in the metadata, just in case
    RamMetaDataManager.setDate( publishFilePath, fileInfo.date );

    return publishFilePath;
}
```