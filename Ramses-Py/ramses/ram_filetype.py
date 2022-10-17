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

class RamFileType( RamObject ):
    """A File type.
    File types are typically used with RamPipe."""

    def __init__( self, uuid="", data = None, create=False ):
        """
        Args:
            uuid (str)
        """
        super(RamFileType, self).__init__( uuid, data, create, "RamFileType" )

    def extensions( self ):
        """The extensions which can be used for this file type, including the “.”

        Returns:
            list of string
        """
        extensions = self.get('extensions', [])
        shortName = self.shortName()
        if not shortName in extensions:
            extensions.insert(0, shortName)

        return extensions

    def check(self, filePath):
        """Checks if the given file is of this type"""

        fileBlocks = filePath.split('.')

        if len(fileBlocks) < 2:
            return False

        if fileBlocks[-1] in self.extensions():
            return True

        return False