![META](authors:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2021-2022;updated:2022/09/01)

# RamObject

The base class for most of Ramses objects.

Inherited by: *[RamItem](ram_item.md), [RamProject](ram_project.md), [RamState](ram_state.md), [RamStatus](ram_status.md), [RamStep](ram_step.md), [RamUser](ram_user.md)*

## Static Methods

| Method | Arguments | Description |
| --- | --- | --- |
| **isUuid**<br />▹ *bool* | • *string*: **str** | Checks if a given string is a valid *UUID*. |
| **getUuid**<br />▹ *string* | *object* or *string*: **object** | Convenience method. Returns the object's uuid or the string itself; use this to parse arguments in methods which need to use the uuid but can get either an object or a string as an argument. |
| **getShortName**<br />▹ *string* | *object* or *string*: **object** | Convenience method. Returns the object's short name; the argument could be either the uuid or an object, or even a short name already. |

## Methods

| Method | Arguments | Description |
| --- | --- | --- |
| ***constructor*** | • *string*: **uuid**=`""`<br/>• *Object / Dict*: **data**=`None`<br/>• *bool*: **create**=`false`<br/>• *string*: **objectType**=`"RamObject"` | You should always provide the *UUID* unless you're creating a new object. In this case, set `create = true` and the object type. An object without *UUID* nor the `create` option will be *virtual*. |
| **color**<br />▹ *string* | | The color used to display this object. |
| **comment**<br />▹ *string* | | The comment for this object. |
| **data**<br />▹ *Object / dict* | | The data of this object. |
| **folderPath**<br />▹ *string* | | The path of this object (or an empty string). |
| **get**<br />▹ *any* | • *string*: **key**<br/>• *any*: **default** = `None` | Gets a specific value from the data. If the key can't be found, returns the default value. |
| **name**<br />▹ *string* | | The name of this object. |
| **setData**<br />▹ *string* | • *string* or *Object / Dict*: **data** | Sets new internal data for this object (and sends it to the application). |
| **shortName**<br />▹ *string* | | The short name of this object. |
| **uuid**<br />▹ *string* | | The universal unique identified (UUID) of this object. |
| **virtual**<br />▹ *bool* | | Checks if this object is virtual. A virtual object is an object which is not saved in the system and will disappear after when the program quits. Invalid *UUIDs* will be converted to virtual objects. |

## Special

### Convert to string

*RamObject*s can be converted to string the usual way.

```py
# Python

obj = RamObject( "Name", "ShortName" )

print( obj ) # implicit conversion
print( str(obj) ) # explicit conversion

# Both will print:
>>> ShortName | Name
```

```js
// ExtendScript

var obj = new RamObject( "Name", "ShortName" );

$.writeln( obj.toString() );

// Prints:
// ShortName | Name

```

### Comparing objects

*RamObjects* can be compared to check if two objects are the same (comparing the values, not the object instances).

Two objects are considered the same **if and only if** their *UUIDs* are the same. Names will not be compared.

```py
# Python

obj = RamObject( "unique-uid1" )
other = RamObject( "unique-uid2" )
same = RamObject( "unique-uid1" )

# RamObject can be compared as any other value

obj == other # False
obj == same # True

# It can be used with the `in` operator too
objList = [ obj, other ]
same in objList # True

# But RamObject is not hashable, so it cannot be used in a `set` or as a key in a `dict`

```

```js
// ExtendScript

var obj = RamObject( "unique-uid1" );
var other = RamObject( "unique-uid2" );
var same = RamObject( "unique-uid1" );

// RamObject can be compared with the is() method

obj.is(other) // false
obj.is(same) // true

```

### Cached data

To limit the number of queries to the *Ramses Daemon* and improve performance, the data is cached for two seconds. That means any change in the *Ramses Application* (by the user or a sync process) in between two calls on a *RamObject* instance will be ignored if there is less than two seconds between these calls.

A way to force the update of the data is to reinit the internal data of the *RamObject* to an empty dictionnary/object.

```py
# Python

# Create an object
obj = RamObject("unique-uid")
# Get some info
name = obj.name()

# Do stuff, in less than two seconds.

# If we absolutely need the latest data
# (i.e. it may have changed since the call to 'name()')
# reset the internal data
obj.__data = {}

# Now we can get up-to-date data
name = obj.name()

```

```js
// ExtendScript

// Create an object
var obj = new RamObject("unique-uid");
// Get some info
var name = obj.name();

// Do stuff, in less than two seconds.

// If we absolutely need the latest data
// (i.e. it may have changed since the call to 'name()')
// reset the internal data
obj.__data = {};

// Now we can get up-to-date data
name = obj.name();

```

!!! note
    When setting new data, the cache is automatically updated first, to be sure to avoid conflicts.

## API Dev notes

!!! note
    These section is for the development of the API only; you should not need these when developping your add-on using the API.

### Implementation

#### Converting objects to string

```py
# Python

# Overload the __str__() method

class RamObject:
    def __str__( self ):
        return self.shortName() + " | " + self.name()

```

```js
// ExtendScript

// Overload the toString() method

function RamObject() {};

RamObject.prototype.toString = function ()
{
    return this.shortName() + " | " + this.name();
}
```

#### Object comparison

```py
# Python

# Overload the __eq__() method

class RamObject:
    def __eq__( self, other ):
        return self.__uuid == other.uuid()

```

```js
// ExtendScript

// Implements an is() method

function RamObject() {};

RamObject.prototype.is = function ( other )
{
    return this.__uuid == other.__uuid;
}
```
