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
from .constants import UserRole
from .file_manager import RamFileManager
from .ram_settings import RamSettings

SETTINGS = RamSettings.instance()

class RamUser( RamObject ):
    """The class representing users."""

    def __init__( self, uuid="", data = None, create=False ):
        """
        Args:
            uuid (str)
        """
        super(RamUser, self).__init__( uuid, data, create, "RamUser" )

    def role( self ):
        """
        Returns:
            (Read-only) enumerated value: 'ADMIN', 'PROJECT_ADMIN', 'LEAD', or 'STANDARD'
        """
        roleStr = self.get("role", "standard")
        if roleStr == "admin":
            return UserRole.ADMIN
        if roleStr == "project":
            return UserRole.PROJECT_ADMIN
        if roleStr == "lead":
            return UserRole.LEAD
        return UserRole.STANDARD

    def configPath( self ): 
        """The path to the Config folder

        Arguments:
            absolute: bool

        Returns:
            str
        """

        path = SETTINGS.folderNames.config
        return RamFileManager.buildPath((
            self.folderPath(),
            path
        ))

