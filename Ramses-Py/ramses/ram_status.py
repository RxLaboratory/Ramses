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

from datetime import datetime
from .logger import log
from .constants import LogLevel
from .daemon_interface import RamDaemonInterface
from .ram_object import RamObject

DAEMON = RamDaemonInterface.instance()

class RamStatus( RamObject ):
    """A state associated to a comment, the user who changed the state, etc."""

    @staticmethod
    def fromPath( filePath ):
        """Returns a RamStatus instance built using the given file path.

        Args:
            filePath (str)

        Returns:
            RamStatus
        """

        uuid = DAEMON.uuidFromPath( filePath, "RamStatus" )

        if uuid != "":
            return RamStatus(uuid)

        log( "The given path does not belong to an item", LogLevel.Debug )
        return None

    def __init__( self, uuid="", data = None, create=False ):
        """
        Args:
            uuid (str)
        """
        super(RamStatus, self).__init__( uuid, data, create, "RamStatus" )

    def copy(self):
        """Returns a new copy of this status"""
        newData = self.data()
        newData['date'] = datetime.now().strftime("%Y-%m-%d- %H:%M:%S")
        newStatus = RamStatus( data=newData, create = True )
        return newStatus

    def date(self):
        """The date of the latest modification"""
        dateStr = self.get("date", "1818-05-05 00:00:00")
        return datetime.strptime(dateStr, "%Y-%m-%d- %H:%M:%S")

    def completionRatio(self):
        """The completion ratio"""
        return self.get("completionRatio", 50)

    def setCompletionRatio(self, completion):
        """Sets a new completion ratio"""
        data = self.data()
        data["completionRatio"] = completion
        data["date"] = datetime.now().strftime("%Y-%m-%d- %H:%M:%S")
        self.setData(data)

    def published(self):
        return self.get("published", False)

    def setPublished(self, published=True):
        data = self.data()
        data["published"] = published
        data["date"] = datetime.now().strftime("%Y-%m-%d- %H:%M:%S")
        self.setData(data)

    def state(self):
        """The state"""
        from .ram_state import RamState
        return RamState( self.get("state", "") )

    def setState(self, state):
        """Sets a new state"""
        data = self.data()
        data["state"] = state.uuid()
        data["date"] = datetime.now().strftime("%Y-%m-%d- %H:%M:%S")
        self.setData(data)

    def step(self):
        """The step"""
        from .ram_step import RamStep
        return RamStep( self.get("step", "") )

    def item(self):
        """The item"""
        from .ram_shot import RamShot
        from .ram_asset import RamAsset
        from .ram_item import RamItem
        itemType = self.get("itemType", 'item')
        if itemType == "shot":
            return RamShot( self.get("item", "") )
        elif itemType == "asset":
            return RamAsset( self.get("item", "") )
        else:
            return RamItem( self.get("item", "") )

    def user(self):
        """The last user who've modified the status"""
        from .ram_user import RamUser
        return RamUser( self.get("user", "") )

    def setUser(self, user):
        """Sets the user who's modifying the status"""
        data = self.data()
        data["user"] = user.uuid()
        data["date"] = datetime.now().strftime("%Y-%m-%d- %H:%M:%S")
        self.setData(data)

    def version(self):
        """The version"""
        return self.get("version", 1)

    def setVersion(self, version):
        """Sets the version"""
        data = self.data()
        data["version"] = version
        data["date"] = datetime.now().strftime("%Y-%m-%d- %H:%M:%S")
        self.setData(data)
