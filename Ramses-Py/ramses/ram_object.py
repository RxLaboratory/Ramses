# -*- coding: utf-8 -*-

#====================== BEGIN GPL LICENSE BLOCK ======================
#
#  This program is free software; you can redistribute it and/or
#  modify it under the terms of the GNU General Public License
#  as published by the Free Software Foundation; either version 3
#  of the License, or (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program. If not, see <http://www.gnu.org/licenses/>.
#
#======================= END GPL LICENSE BLOCK ========================

import time
import json
import re
import os
import uuid as UUID
from .daemon_interface import RamDaemonInterface
from .logger import log, LogLevel

DAEMON = RamDaemonInterface.instance()
RE_UUID = re.compile("^[a-zA-Z0-9]+-[a-zA-Z0-9]+-[a-zA-Z0-9]+-[a-zA-Z0-9]+-[a-zA-Z0-9]+$")

class RamObject(object):
    """The base class for most of Ramses objects."""

    @staticmethod
    def isUuid( string ):
        if not isinstance(string, str):
            return False
        if RE_UUID.match(string):
            return True
        return False

    @staticmethod
    def getUuid( obj ):
        from .file_manager import RamFileManager

        if isinstance( obj, RamObject ):
            uuid = obj.uuid()
        elif obj is None:
            return ''
        else:
            uuid = obj

        return uuid

    @staticmethod
    def getShortName( obj ):
        if RamObject.isUuid( obj ):
            obj = RamObject(obj)
        if isinstance(obj, RamObject):
            return obj.shortName()
        # Must already be a short name
        return obj

    def __init__( self, uuid="", data = None, create=False, objectType="RamObject" ):
        """
        Args:
            uuid (str): The object's uuid
        """

        if uuid == "" and not create:
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

        if create:
            reply = DAEMON.create( self.__uuid, self.__data, objectType )
            if not DAEMON.checkReply(reply):
                log("I can't create this object.")

    def uuid( self ):
        """Returns the uuid of the object"""
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
        """Saves the new data for the object"""
        if isinstance(data, str):
            data = json.loads(data)

        self.__data = data
        self.__cacheTime = time.time()

        if not self.__virtual:
            DAEMON.setData( self.__uuid, data )

    def get(self, key, default = None):
        """Get a specific value in the data"""
        data = self.data()
        return data.get(key, default)

    def set(self, key, value):
        """Sets a new value in the object data"""
        data = self.data()
        data[key] = value
        self.setData(data)

    def name( self ):
        """
        Returns:
            str
        """
        return self.get('name', 'Unknown Object')

    def shortName( self ):
        """
        Returns:
            str
        """
        return self.get('shortName', 'Unknown')

    def comment( self ):
        """
        Returns:
            str
        """
        return self.get('comment', '')

    def color( self ):
        """Returns the color as (R,G,B)"""
        colorName = self.colorName().lstrip("#")
        return tuple(int(colorName[i:i+2], 16) for i in (0, 2, 4))

    def colorName(self):
        """Returns the color as #000000"""
        return self.get('color', '#e3e3e3')

    def settings( self ):
        """Returns the settings of this object"""
        return self.get('settings', {})

    def folderPath( self ):
        """Returns the folder corresponding to this object"""
        if self.__virtual:
            return self.get("folderPath", "")
        p = DAEMON.getPath( self.__uuid )
        if p != "" and not os.path.isdir( p ):
            try:
                os.makedirs( p )
            except:
                return ""
        return p

    def virtual( self ):
        """Checks if this object is virtual"""
        return self.__virtual

    def __str__( self ):
        n = self.shortName()
        if self.name() != '':
            if n != '': n = n + " | "
            n = n + self.name()
        return n

    def __eq__(self, other):
        try:
            return self.__uuid == other.uuid()
        except:
            return False
