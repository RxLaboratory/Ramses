# RamObject

The base class for most of Ramses objects.

Inherited by: *[RamItem](ram_item.md), [RamProject](ram_project.md), [RamState](ram_state.md), [RamStep](ram_step.md), [RamUser](ram_user.md)*

## Static Methods

| Method | Arguments | Description |
| --- | --- | --- |
| **fromDict**<br />▹ *RamObject* | *object* or *dict*: **objectDict** | Returns a *RamObject* instance built using the given object/dict, as the ones returned by [RamDaemonInterface](ram_daemon_interface.md).<br />This method is reimplemented in inherited classes to return an inherited instance. |
| **getObjectShortName**<br />▹ *string* | *object* or *string*: **object** | Convenience method. Returns the object's short name or the string itself; use this to parse arguments in methods which need to use the shortname but can get either an object or a string as an argument. |

## Methods

| Method | Arguments | Description |
| --- | --- | --- |
| ***constructor*** | *string*: **objectName**<br />*string*: **objectShortName** | |
| **name**<br />▹ *string* | | |
| **shortName**<br />▹ *string* | | |

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

Two objects are considered the same **if and only if** their short names are the same. Names will not be compared, which means **object short names in ramses have to be unique**.

```py
# Python

obj = RamObject( "Name", "ShortName" )
other = RamObject( "Other Object", "otheShortName" )
same = RamObject( "Same Object", "ShortName" )

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

var obj = RamObject( "Name", "ShortName" );
var other = RamObject( "Other Object", "otheShortName" );
var same = RamObject( "Same Object", "ShortName" );

// RamObject can be compared with the is() method

obj.is(other) // false
obj.is(same) // true

```

## API Dev notes

!!! note
    These section is for the development of the API only; you should not need these when developping your add-on using the API.

### (Im)mutable data

The data returned by the methods can be either [mutable or immutable](implementation.md#accessing-the-data).

| Method | Type of the returned data |
| --- | --- |
| **name** | <i class="fa fa-lock"></i> Immutable |
| **shortName** | <i class="fa fa-lock"></i> Immutable |

### Implementation

#### Converting objects to string

```py
# Python

# Overload the __str__() method

class RamObject:
    def __str__( self ):
        return self._shortName + " | " + self._name

```

```js
// ExtendScript

// Overload the toString() method

function RamObject() {};

RamObject.prototype.toString = function ()
{
    return this._shortName + " | " + this._name;
}
```

#### Object comparison

```py
# Python

# Overload the __eq__() method

class RamObject:
    def __eq__( self, other ):
        return self._shortName == other._shortName

```

```js
// ExtendScript

// Implements an is() method

function RamObject() {};

RamObject.prototype.is = function ( other )
{
    return this._shortName == other._shortName;
}
```

____

![META](authors:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2021;updated:2021/05/25)