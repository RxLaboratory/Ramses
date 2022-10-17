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

class RamState( RamObject ):
    """Represents a state used in a status, like “CHK” (To be checked), “OK” (ok), “TO_DO”, etc."""

    @staticmethod
    def stateSorter( s ):
        """Used to sort list of states"""
        return s.completionRatio()

    def __init__( self, uuid="", data = None, create=False ):
        """
        Args:
            uuid (str)
        """
        super(RamState, self).__init__( uuid, data, create, "RamState" )

    def completionRatio( self ):
        """The ratio of completion of this state in the range [0, 100].

        Returns:
            int
        """
        return self.get("completionRatio", 50)
