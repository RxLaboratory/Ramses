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

import socket, json

from .logger import log
from .constants import ItemType, LogLevel, Log

class RamDaemonInterface( object ):
    """The Class used to communicate with the Ramses Daemon

    Attributes:
        port: int.
            The listening port of the daemon
        online: bool (read-only).
            True if the Daemon is available
    """
    
    _instance = None

    @staticmethod
    def checkReply( obj ):
        if obj['accepted'] and obj['success'] and obj['content'] is not None:
            return obj['content']
        return {}

    @classmethod
    def instance( cls ):
        from .ram_settings import RamSettings
        
        if cls._instance is None:
            cls._instance = cls.__new__(cls)
            cls._port = RamSettings.instance().ramsesClientPort
            cls._address = 'localhost'

        return cls._instance

    def __init__(self):
        """
        Args:
            port: int.
        """
        raise RuntimeError("RamDaemonInterface can't be initialized with `RamDaemonInterface()`, it is a singleton. Call RamDaemonInterface.instance() or Ramses.instance().daemonInterface() instead.")

    def online(self):
        """Checks if the daemon is available"""
        return self.__testConnection()

    def ping(self):
        """Gets the version and current user of the ramses daemon.

        Read the Ramses Daemon reference at http://ramses.rxlab.guide/dev/daemon-reference/ for more information.

        Returns: dict.
            Read http://ramses.rxlab.guide/dev/daemon-reference/ for more information.
        """
        return self.__post('ping', 65536)

    def raiseWindow(self):
        """Raises the Ramses Client application main window.

        Read the Ramses Daemon reference at http://ramses.rxlab.guide/dev/daemon-reference/ for more information.
        """
        self.__post('raise')

    def getRamsesFolderPath(self):
        """Gets the path of the main Ramses folder"""
        reply = self.__post( "getRamsesFolder", 65536 )
        content = self.checkReply(reply)
        return content.get("path", "")

    def getObjects( self, objectType ):
        """Gets the list of the objects

        Read the Ramses Daemon reference at http://ramses.rxlab.guide/dev/daemon-reference/ for more information.

        Returns: list of RamObject.
        """

        from .ram_asset import RamAsset
        from .ram_assetgroup import RamAssetGroup
        from .ram_filetype import RamFileType
        from .ram_item import RamItem
        from .ram_object import RamObject
        from .ram_pipe import RamPipe
        from .ram_pipefile import RamPipeFile
        from .ram_project import RamProject
        from .ram_sequence import RamSequence
        from .ram_shot import RamShot
        from .ram_state import RamState
        from .ram_status import RamStatus
        from .ram_step import RamStep
        from .ram_user import RamUser

        if not self.__checkUser():
            self.__noUserReply('getProjects')
            return ()

        reply = self.__post(
            (
                "getObjects",
                ("type", objectType)
            ),
            65536 )
        content = self.checkReply(reply)
        objs = content.get("objects", ())
        objects = []
        for obj in objs:
            uuid = obj.get("uuid", "")
            data = obj.get("data", {})
            if objectType == "RamObject":
                o = RamObject( uuid, data=data)
            elif objectType == "RamAsset":
                o = RamAsset( uuid, data=data)
            elif objectType == "RamAssetGroup":
                o = RamAssetGroup( uuid, data=data)
            elif objectType == "RamFileType":
                o = RamFileType( uuid, data=data)
            elif objectType == "RamItem":
                o = RamItem( uuid, data=data)
            elif objectType == "RamPipe":
                o = RamPipe( uuid, data=data)
            elif objectType == "RamPipeFile":
                o = RamPipeFile( uuid, data=data)
            elif objectType == "RamProject":
                o = RamProject( uuid, data=data)
            elif objectType == "RamSequence":
                o = RamSequence( uuid, data=data)
            elif objectType == "RamShot":
                o = RamShot( uuid, data=data)
            elif objectType == "RamState":
                o = RamState( uuid, data=data)
            elif objectType == "RamStatus":
                o = RamStatus( uuid, data=data)
            elif objectType == "RamStep":
                o = RamStep( uuid, data=data)
            elif objectType == "RamUser":
                o = RamUser( uuid, data=data)
            if o:
                objects.append(o)
        return objects

    def getProjects(self):
        """Gets the list of the projects

        Read the Ramses Daemon reference at http://ramses.rxlab.guide/dev/daemon-reference/ for more information.
        
        Returns: list of RamProject.
        """

        from .ram_project import RamProject

        if not self.__checkUser():
            self.__noUserReply('getProjects')
            return ()

        reply = self.__post( "getProjects", 262144 )
        content = self.checkReply(reply)
        projectList = content.get("projects", ())
        projects = []
        for p in projectList:
            pid = p.get("uuid", "")
            pdata = p.get("data", {})
            projects.append( RamProject(
                uuid = pid,
                data = pdata
            ))

        return projects

    def getCurrentProject(self):
        """Gets the current project

        Read the Ramses Daemon reference at http://ramses.rxlab.guide/dev/daemon-reference/ for more information.
        
        Returns: RamProject.
        """

        from .ram_project import RamProject

        if not self.__checkUser():
            self.__noUserReply('getCurrentProject')
            return None

        reply = self.__post( "getCurrentProject", 65536 )
        content = self.checkReply(reply)
        uuid = content.get("uuid", "")
        if uuid == "":
            return None
        data = content.get("data", {})
        return RamProject(uuid, data)

    def getCurrentUser(self):
        """Gets the current user"""
        from .ram_user import RamUser
        content = self.checkReply( self.ping() )
        uuid =  content.get("userUuid", "")
        if uuid == "":
            return None
        return RamUser( uuid )

    def setCurrentProject(self, projectUuid):
        """Sets the current project.

        Read the Ramses Daemon reference at http://ramses.rxlab.guide/dev/daemon-reference/ for more information.
        
        Returns: dict.
        """

        if not self.__checkUser(): return self.__noUserReply('setCurrentProject')
        return self.__post( (
            "setCurrentProject",
            ('uuid', projectUuid)
            ), 65536 )

    def getData(self, uuid):
        """Gets the data for a specific RamObject.

        Read the Ramses Daemon reference at http://ramses.rxlab.guide/dev/daemon-reference/ for more information.
        
        Returns: dict.
        """

        if not self.__checkUser():
            self.__noUserReply('getData')
            return {}

        reply =  self.__post(
            (
                "getData",
                ('uuid', uuid)
            ),
            65536 )
        content = self.checkReply(reply)
        return content.get("data", {})

    def setData(self, uuid, data):
        """Sets the data of a specific RamObject.

        Read the Ramses Daemon reference at http://ramses.rxlab.guide/dev/daemon-reference/ for more information.
        
        Returns: dict.
        """

        if not isinstance(data, str):
            data = json.dumps(data)

        if not self.__checkUser(): return self.__noUserReply('setData')
        return self.__post( (
            "setData",
            ('uuid', uuid),
            ('data', data)
            ), 65536 )

    def getPath(self, uuid):
        """Gets the path for a specific RamObject.

        Read the Ramses Daemon reference at http://ramses.rxlab.guide/dev/daemon-reference/ for more information.
        
        Returns: dict.
        """
        
        if not self.__checkUser():
            self.__noUserReply('getPath')
            return ""

        reply =  self.__post( (
            "getPath",
            ('uuid', uuid)
            ), 65536 )
        content = self.checkReply(reply)
        return content.get("path", "")

    def uuidFromPath(self, path, type ):
        """Gets the uuid of an Object using its path.

        Read the Ramses Daemon reference at http://ramses.rxlab.guide/dev/daemon-reference/ for more information.
        
        Returns: dict.
        """
        
        if not self.__checkUser():
            self.__noUserReply('uuidFromPath')
            return ""

        reply = self.__post( (
            "uuidFromPath",
            ('path', path),
            ('type', type),
            ), 65536 )
        content = self.checkReply(reply)
        return content.get("uuid", "")

    def create(self, uuid, data, objectType):
        if not self.__checkUser():
            return self.__noUserReply('uuidFromPath')

        if not isinstance(data, str):
            data = json.dumps(data)

        return self.__post( (
            "create",
            ("uuid", uuid),
            ('data', data),
            ("type", objectType)
            ), 65536)

    def __buildQuery(self, query):
        """Builds a query from a list of args

        Args:
            query: str or tuple.
                If query is a str, it is returned as is.
                If it's a tuple, each item can be either an argument as a string, or a 2-tuple key/value pair.

        Returns: str.
            The query string in the form "key1&key2=value2&key3=value3"
        """

        if isinstance(query, str):
            return query

        queryList = []

        for arg in query:
            if isinstance(arg, str):
                if arg:
                    queryList.append(arg)
            else:
                queryList.append( "=".join(arg) )

        return "&".join(queryList)

    def __post(self, query, bufsize = 0):
        """Posts a query and returns a dict corresponding to the json reply
        
        Args:
            query: tuple.
                The list of arguments, which are themselves 2-tuples of key-value pairs (value may be an empty string)
            bufsize: int.
                The maximum amount of data to be received at once is specified by bufsize.
                
        Returns: dict or None.
            The Daemon reply converted from json to a python dict.
            None if there is an error or the Daemon is unavailable.
        """

        from .ramses import Ramses

        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

        query = self.__buildQuery( query )

        log( query, LogLevel.DataSent)

        try:
            s.connect((self._address, self._port))
        except Exception as e: #pylint: disable=broad-except
            log("Daemon can't be reached", LogLevel.Debug)
            log(str(e), LogLevel.Critical)
            ramses = Ramses.instance()
            ramses.disconnect()
            return

        s.sendall(query.encode('utf-8'))

        if bufsize == 0:
            s.close()
            return None

        data = s.recv(bufsize)
        s.close()

        try:
            obj = json.loads(data)
        except:
            log("Invalid reply data from the Ramses Daemon.", LogLevel.Critical)
            obj = {
                'accepted': False,
                'success': False
            }
            return obj

        log (str(data), LogLevel.DataReceived )

        if not obj['accepted']: log("Unknown Ramses Daemon query: " + obj['query'], LogLevel.Critical)
        if not obj['success']: log("Warning: the Ramses Daemon could not reply to the query: " + obj['query'], LogLevel.Critical)       
        if obj['message']: log(obj['message'], LogLevel.Debug)

        return obj

    def __testConnection(self):
        """Checks if the Ramses Daemon is available"""

        data = self.ping()

        if data is None:
            log("Daemon unavailable", LogLevel.Debug)
            return False
  
        content = data['content']
        if content is None:
            log("Daemon did not reply correctly")
            return False
        if content['ramses'] == "Ramses":
            return True
        if content['ramses'] == "Ramses-Client":
            return True

        log("Invalid content in the Daemon reply", LogLevel.Critical)
        return False

    def __checkUser(self):
        data = self.ping()

        if data is None:
            return False

        if 'content' in data:
            content = data['content']
        else:
            return False

        if content is None:
            return False

        if 'userUuid' in content:
            ok = content['userUuid'] != ""
        else:
            return False

        return ok

    def __noUserReply(self, query):
        log( Log.NoUser, LogLevel.Debug)
        return {
            'accepted': False,
            'success': False,
            'message': Log.NoUser,
            'query': query,
            'content': None
        }
