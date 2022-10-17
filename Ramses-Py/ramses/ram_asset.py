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

from .logger import log
from .ram_item import RamItem
from .ram_assetgroup import RamAssetGroup
from .daemon_interface import RamDaemonInterface
from .constants import LogLevel

# Keep the daemon at hand
DAEMON = RamDaemonInterface.instance()

class RamAsset( RamItem ):
    """A class representing an asset."""

    @staticmethod
    def fromPath( fileOrFolderPath, virtualIfNotFound = False ):
        """Returns a RamAsset instance built using the given path.
            The path can be any file or folder path from the asset
            (a version file, a preview file, etc)

        Args:
            fileOrFolderPath (str)

        Returns:
            RamAsset
        """
        uuid = DAEMON.uuidFromPath( fileOrFolderPath, "RamAsset" )

        if uuid != "":
            return RamAsset(uuid)

        log( "The given path does not belong to an asset", LogLevel.Debug )
        return None

    def __init__( self, uuid="", data = None, create=False ):
        """
        Args:
            uuid (str)
        """
        super(RamAsset, self).__init__( uuid, data, create, "RamAsset" )

    def tags( self ):
        """Some tags describing the asset. An empty list if the Daemon is not available.

        Returns:
            list of str
        """

        return self.get("tags", [])

    def assetGroup( self ):
        """The asset group containing this asset"""
        agUuid = self.get("assetGroup", "")
        if agUuid != "":
            return RamAssetGroup( agUuid )
        return None
