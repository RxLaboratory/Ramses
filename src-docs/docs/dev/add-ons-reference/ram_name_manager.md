![META](authors:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2021;updated:2021/09/08)

# RamNameManager

A class to easily handle Ramses names using the [*Ramses Naming Scheme*](../../components/files/naming.md).

Read the sections about the *[Naming Scheme](../../components/files/naming.md)* for more details about names used by *Ramses*.

## Attributes

These attributes are automatically set by the `RamNameManager.setFileName( name )` method; They can then be changed and a new file name can be generated using `RamNameManager.fileName( )`

| Attribute | Type | Default | Description |
| --- | --- | --- | --- |
| **extension** | *string* |  | The file extension (without the leading `.`). |
| **project** | *string* |  | The project short name (ID). |
| **ramType** | *[ItemType](enum.md#itemtype)* |  | The type of the corresponding item. |
| **resource** | *string* |  | The resource string. |
| **shortName** | *string* |  | The short name (ID) of the item. |
| **state** | *string* |  | The state short name (ID), if any. |
| **step** | *string* |  | The step short name (ID). |
| **version** | *int* |  | The versin, `-1` if not set/found. |

## Methods

| Method | Arguments | Description |
| --- | --- | --- |
| ***constructor*** |  | Creates a new empty instance. |
| **copy**<br />▹ *RamNameManager* |  | Creates a copy of the current instance. |
| **fileName**<br />▹ *string* |  | Constructs and returns the file name based on the attributes. |
| **originalFileName**<br />▹ *string* |  | Gets the original file name withoutany modification. |
| **setFileName**<br />▹ *boolean* |  | Sets the filename and parses data from it. Returns `true` on success. |
| **setFilePath** |  | Parses all data which can be found from a file path. |

## Examples

```py
# Python

from ramses import RamNameManager, ItemType

# Get info from an existing asset file
filePath = '/path/to/PROJ_A_MyAsset_RIG_aResource.blend'
nm = RamNameManager()
if nm.setFileName(filePath:
    nm.version = 12 # Set the version to 12
    nm.state = "OK" # Set thje state
    print( nm.step ) # Prints: RIG
    print( nm.fileName() ) # Prints: PROJ_A_MyAsset_RIG_aResource_OK012.blend

# Build a filename from scratch
nm = RamNameManager()
nm.project = "OTHER"
nm.shortName = "MyShot"
nm.ramType = ItemType.SHOT
nm.step = "ANIM"
nm.extension = "kra"
print( nm.fileName() ) # Prints: OTHER_S_MyShot_ANIM.kra
```

```js
// ExtendScript

#include ramses.jsxinc

// Get info from an existing asset file
var filePath = '/path/to/PROJ_A_MyAsset_RIG_aResource.blend';
var nm = new RamNameManager();
if ( nm.setFileName(filePath) ) {
    nm.version = 12; // Set the version to 12
    nm.state = "OK"; // Set thje state
    alert( nm.step ); // Alerts: RIG
    alert( nm.fileName() ); // Alerts: PROJ_A_MyAsset_RIG_aResource_OK012.blend
}

// Build a filename from scratch
nm = RamNameManager();
nm.project = "OTHER";
nm.shortName = "MyShot";
nm.ramType = ItemType.SHOT;
nm.step = "ANIM";
nm.extension = "kra";
alert( nm.fileName() ); // Alerts: OTHER_S_MyShot_ANIM.kra
```

____

## API Dev notes

!!! note
    These section is for the development of the API only; you should not need these when developping your add-on using the API.

Replace `RamFileManager.buildRamsesFileName`, `RamFileManager.composeRamsesFileName`,  `RamFileManager.isRamsesName`,  `RamFileManager.decomposeRamsesFileName`
by `RamNameManager.setFileName` and  `RamNameManager.fileName`

Replace `RamFileManager.decomposeRamsesFilePath` by `RamFileManager.setFilePath`