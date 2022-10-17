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
from .ram_pipefile import RamPipeFile
from .daemon_interface import RamDaemonInterface

DAEMON = RamDaemonInterface.instance()

class RamPipe( RamObject ):
    """A pipe which connects two steps together in the production pipeline.
        The pipe goes from the output step (which exports data into a specific file type)
        to the input step (which imports that data)."""

    def __init__( self, uuid="", data = None, create=False ):
        """
        Args:
            uuid (str)
        """
        super(RamPipe, self).__init__( uuid, data, create, "RamPipe" )

    def inputStep(self):
        from .ram_step import RamStep
        return RamStep( self.get("inputStep", ""))

    def outputStep(self):
        from .ram_step import RamStep
        return RamStep( self.get("outputStep", ""))

    def inputStepShortName( self ):
        """The short name of the input step

        Returns:
            str
        """
        step = self.inputStep()
        if step:
            return step.shortName()
        return ""

    def outputStepShortName( self ):
        """The short name of the output step

        Returns:
            str
        """
        step = self.outputStep()
        if step:
            return step.shortName()
        return ""

    def pipeFiles( self ):
        pipeFileListUuid = self.get("pipeFiles")
        pipeFiles = []
        if not pipeFileListUuid:
            return pipeFiles
        for uuid in pipeFileListUuid:
            p = RamPipeFile( uuid )
            if p:
                pipeFiles.append(p)
        return pipeFiles

    def __str__( self ):
        return self.outputStepShortName() + " -> " + self.inputStepShortName()
