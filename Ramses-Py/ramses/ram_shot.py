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

from ramses.ram_sequence import RamSequence
from .ram_item import RamItem
from .daemon_interface import RamDaemonInterface
from .constants import ItemType
from .ramses import Ramses
from .logger import log
from .constants import LogLevel

# Keep the daemon at hand
DAEMON = RamDaemonInterface.instance()

class RamShot( RamItem ):
    """A shot"""

    @staticmethod
    def fromPath( fileOrFolderPath, virtualIfNotFound=False ):
        """Returns a RamShot instance built using the given path.
            The path can be any file or folder path from the asset
            (a version file, a preview file, etc)

        Args:
            path (str)

        Returns:
            RamShot
        """

        uuid = DAEMON.uuidFromPath( fileOrFolderPath, "RamShot" )

        if uuid != "":
            return RamShot(uuid)

        log( "The given path does not belong to a shot", LogLevel.Debug )
        return None

    def __init__( self, uuid="", data = None, create=False ):
        """
        Args:
            uuid (str)
        """
        super(RamShot, self).__init__( uuid, data, create, "RamShot" )

    def duration( self ):
        """The shot duration, in seconds

        Returns:
            float
        """

        return self.get("duration", 5)

    def frames( self ):
        """The shot duration, in frames

        Returns:
            int
        """

        duration = self.duration()
        project = self.project()
        fps = 24.0
        if project:
            fps = project.framerate()
        return int(duration * fps)

    def sequence(self):
        """The sequence containing this shot"""
        seqUuid = self.get("sequence", "")
        if seqUuid != "":
            return RamSequence( seqUuid )
        return None
