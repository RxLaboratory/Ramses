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

from .ram_object import RamObject
from .daemon_interface import RamDaemonInterface
from .logger import log
from .constants import LogLevel

DAEMON = RamDaemonInterface.instance()

class RamAssetGroup( RamObject ):
    """A group of assets"""

    @staticmethod
    def fromPath( fileOrFolderPath ):
        """Returns a RamAssetGroup instance built using the given path.
        The path can be any file or folder path from the asset 
        (a version file, a preview file, etc)

        Args:
            path (str)

        Returns:
            RamAssetGroup
        """
        uuid = DAEMON.uuidFromPath( fileOrFolderPath, "RamAssetGroup" )

        if uuid != "":
            return RamAssetGroup(uuid)

        log( "The given path does not belong to an item", LogLevel.Debug )
        return None

    def __init__( self, uuid="", data = None, create=False ):
        """
        Args:
            uuid (str)
        """
        super(RamAssetGroup, self).__init__( uuid, data, create, "RamAssetGroup" )

    def project(self):
        """The project containing this group"""
        from .ram_project import RamProject
        uuid = self.get("project", "")
        if uuid != "":
            return RamProject(uuid)
        return None
        