# Implementation / API Dev notes

!!! note
    This section is for the development of the low-level API only; you should not need these notes when developping your own add-on using the API.

!!! hint
    Some of the elements described in this documentation have to be interpreted depending on the language used in the implementations of the Ramses API and may vary a bit.

## Enumerated Values

*Enumerated values* (a.k.a. *Constants*) are predefined sets of values to be used in specific methods. Their name is in CAPITAL_SNAKE_CASE, and they're grouped together by type (for exemple `color.RED`, or `shape.RECTANGLE`). The value itself can be of any type (array, string, number...).

```py
# Python

# Enumerated values are public static attributes of a containing class. They're declared like this:
class Group():
    VALUE_1 = 0
    VALUE_2 = 1
    VALUE_3 = 2

# Example:
class Color():
    RED = [255,0,0]
    GREEN = [0,255,0]
    BLUE = [0,0,255]
```

```js
// ExtendScript

// Enumerated values are attributes in simple objects. They're declared like this:
var Group = {
    VALUE_1: 0,
    VALUE_2: 1,
    VALUE_3: 2
};

// Example:
var Color = {
    RED: [255,0,0],
    GREEN: [0,255,0],
    BLUE: [0,0,255]
};
```

## Accessing the data

In All "*RamClasses*", most of the data is retrieved with methods (instead of accessing it directly from an attribute). This allows the API to get the data from three different locations:

- already stored in a private attribute of the class instance (set by a previous call to the method or by the constructor).
- from the *Daemon*, if and only if `Ramses.instance().online` is `true`.
- from reading the *Ramses Tree*, the file structure, by retrieving available information from the file names (or eventually file meta-data).

The priority for each method of retrieval changes according to the type of data, which can be either *mutable* or *immutable*.

- Mutable data are data which may change regularly and which can't be stored (like the current status). In this case, they're retrieved by following these priorities:
    1. Got from the *Daemon* if available: set the private attribute before returning the data.
    2. Got from the *Ramses Tree*: set the private attribute before returning the data.
    3. If nothing was found, return the stored data from the corresponding private attribute.

- Immutable data are data which should not change during a single session (like paths). In this case, *Ramses* can store and return them without having to check again each time the method is called.
    1. If the private attribute contains the data, it is returned.
    2. Got from the *Daemon* if available: set the private attribute before returning the data.
    3. Got from the *Ramses Tree* if available: set the private attribute before returning the data.

Here are example concepts in Python and ExtendScript, like what's done in the provided implementations of the *Ramses Add-ons* API.

```py
# Python

class RamClass( object ):

    # The data can be passed to the constructor.
    # It may or may not have default values.
    def __init__(self, someImmutableData='', someMutableData=''): 
        self.__someImmutableData = someImmutableData
        self.__someMutableData = someMutableData

    def someMutableData( self ):
        if Ramses.instance().online():
            # If the Daemon is available, try to get the data there
            theData = Ramses.instance().getData()
            # Only if we've got the data, store and return it
            if theData is not None:
                self.__someMutableData = theData
                return self.__someMutableData

        # Try to get from the Ramses Tree
        theData = AnotherClass.getDataFromPath( somePath )
        # If we've got the data, store it
        if theData is not None:
            self.__someMutableData = theData

        # Last resort return
        return self.__someMutableData

    def someImmutableData( self ):
        # We start by checking if we already have some data
        if self.__someImmutableData != '':
            # Stop here. This improves performance with immutable data.
            return self.__someImmutableData

        # Now we can get the Data from the Daemon or the Ramses Tree
        if Ramses.instance().online():
            theData = Ramses.instance().getData()
            if theData is not None:
                self.__someMutableData = theData
                return self.__someMutableData

        theData = AnotherClass.getDataFromPath( somePath )
        if theData is not None:
            self.__someMutableData = theData

        return self.__someMutableData
```

```js
// ExtendScript

function RamClass( someImmutableData='', someMutableData='' )
{
    // The data can be passed to the constructor.
    // It may or may not have default values.
    this.__someImmutableData = someImmutableData;
    this.__someMutableData = someMutableData;
}

RamClass.prototype.someMutableData = function ()
{
    if ( Ramses.instance().online() )
    {
        // If the Daemon is available, try to get the data there
        var theData = Ramses.instance().getData();
        // Only if we've got the data, store and return it
        if ( theData != null )
        {
            this.__someMutableData = theData;
            return this.__someMutableData;
        }
    }      

    // Try to get from the Ramses Tree
    var theData = AnotherClass.getDataFromPath( somePath );
    // If we've got the data, store it
    if ( theData != null )
        this.__someMutableData = theData;

    // Last resort return
    return this.__someMutableData;
}

RamClass.prototype.someImmutableData = function ()
{
    // We start by checking if we already have some data
    if (self.__someImmutableData != '')
    {
        // Stop here. This improves performance with immutable data.
        return self.__someImmutableData;
    }
        
    // Now we can get the Data from the Daemon or the Ramses Tree
    if ( Ramses.instance().online() )
    {
        var theData = Ramses.instance().getData();
        if ( theData != null )
        {
            this.__someMutableData = theData;
            return this.__someMutableData;
        }
    }

    var theData = AnotherClass.getDataFromPath( somePath );
    if ( theData != null )
        this.__someMutableData = theData;

    return this.__someMutableData;
}

```


![META](authors:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2021;updated:2021/05/04)