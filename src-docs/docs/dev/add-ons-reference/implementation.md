![META](authors:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2021-2022;updated:2022/09/03)

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

In All "*RamClasses*", most of the data is retrieved with methods (instead of accessing it directly from an attribute). This allows the API to get and set the data from the *Daemon*, if and only if `Ramses.instance().online` is `true`.

Some instances of these classes can be *virtual*. A *virtual* instance is an object which is not stored in the database; it's meant to be temporary. In this case, the data can be set by the *API* itself and is not retrieved nor updated with the *Daemon*.

There is also a cache system, to prevent having too many calls to the *Daemon* and improve performance. The *Daemon* is called if and only if the data stored in the object is more than two second old. That means subsequent calls to the data in a single process should result to a single call to the *Daemon*.

Here are example concepts in Python and ExtendScript, like what's done in the provided implementations of the *Ramses Add-ons* API.

```py
# Python

DAEMON = RamDaemonInterface.instance()

class RamClass( object ):

    # The data can be passed to the constructor.
    def __init__( self, uuid="", data = None, virtual=True, objectType="RamObject" ):
        """
        Args:
            uuid (str): The object's uuid
        """
        
        if uuid == "" and virtual:
            self.__virtual = True
        else:
            self.__virtual = False
        
        if uuid == "":
            uuid = str(UUID.uuid4())
        self.__uuid = uuid

        if isinstance(data, str):
            data = json.loads(data)
        if data:
            self.__data = data
            self.__cacheTime = time.time()
        else:
            self.__data = {}
            self.__cacheTime = 0

        if not virtual:
            reply = DAEMON.create( self.__uuid, self.__data, objectType )
            if not DAEMON.checkReply(reply):
                log("I can't create this object.")

        def uuid( self ):
        return self.__uuid

    def data( self ):
        """Gets the data for this object"""
        if self.__virtual:
            return self.__data

        # Check if the cached data is recent enough
        # there's a 2-second timeout to not post too many queries
        # and improve performance
        cacheElapsed = time.time() - self.__cacheTime
        if self.__data and cacheElapsed < 2:
            return self.__data

        # Get the data from the daemon
        data = DAEMON.getData( self.__uuid )

        if data:
            self.__data = data
            self.__cacheTime = time.time()

        return self.__data

    def setData( self, data):

        if isinstance(data, str):
            data = json.loads(data)           

        self.__data = data

        if not self.__virtual:
            DAEMON.setData( self.__uuid, data )

    def get(self, key, default = None):
        """Get a specific value in the data"""
        data = self.data()
        return data.get(key, default)

    def name( self ):
        """
        Returns:
            str
        """
        return self.get('name', 'Unknown Object')
```
