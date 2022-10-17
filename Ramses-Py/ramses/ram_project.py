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

import os, re
from .file_info import RamFileInfo
from .daemon_interface import RamDaemonInterface
from .file_manager import RamFileManager
from .logger import log
from .ramses import Ramses
from .utils import escapeRegEx, removeDuplicateObjectsFromList
from .ram_settings import RamSettings
from .ram_object import RamObject
from .ram_asset import RamAsset
from .ram_assetgroup import RamAssetGroup
from .ram_sequence import RamSequence
from .ram_shot import RamShot
from .ram_step import RamStep
from .ram_pipe import RamPipe
from .constants import StepType, ItemType, LogLevel

DAEMON = RamDaemonInterface.instance()
SETTINGS = RamSettings.instance()

class RamProject( RamObject ):
    """A project handled by Ramses. Projects contains general items, assets and shots."""

    @staticmethod
    def fromPath( path ):
        """Creates a project object from any path, trying to get info from the given path"""

        uuid = DAEMON.uuidFromPath( path, "RamProject" )

        if uuid != "":
            return RamProject(uuid)

        log( "The given path does not belong to a project", LogLevel.Debug )
        return None

    def __init__( self, uuid="", data = None, create=False ):
        """
        Args:
            uuid (str)
        """
        super(RamProject, self).__init__( uuid, data, create, "RamProject" )

    def width( self ):
        """
        Returns:
            int
        """

        return self.get("width", 1920)

    def height( self ):
        """
        Returns:
            int
        """

        return self.get("height", 1080)

    def framerate( self ):
        """
        Returns:
            float
        """

        return self.get("framerate", 24.0)

    def absolutePath( self, relativePath="" ):
        """Builds an absolute path from a path relative to the project path

        Args:
            relativePath (str)

        Returns:
            str
        """
        return RamFileManager.buildPath((
            self.folderPath(),
            relativePath
        ))

    def adminPath( self ):
        """Returns the path of the Admin folder (creates it if it does not exist yet)"""

        projectFolder = self.folderPath()
        if not os.path.isdir( projectFolder ):
            return ''

        thePath = RamFileManager.buildPath((
            projectFolder,
            SETTINGS.folderNames.admin
        ))

        if not os.path.isdir( thePath ):
            try:
                os.makedirs( thePath )
            except:
                return ""
        
        return thePath

    def preProdPath( self ):
        """Returns the path of the PreProd folder (creates it if it does not exist yet)"""

        projectFolder = self.folderPath()
        if not os.path.isdir( projectFolder ):
            return ''

        thePath = RamFileManager.buildPath((
            projectFolder,
            SETTINGS.folderNames.preProd
        ))

        if not os.path.isdir( thePath ):
            try:
                os.makedirs( thePath )
            except:
                return ""

        return thePath

    def prodPath( self ):
        """Returns the path of the Prod folder (creates it if it does not exist yet)"""

        projectFolder = self.folderPath()
        if not os.path.isdir( projectFolder ):
            return ''

        thePath = RamFileManager.buildPath((
            projectFolder,
            SETTINGS.folderNames.prod
        ))

        if not os.path.isdir( thePath ):
            try:
                os.makedirs( thePath )
            except:
                return ""

        return thePath

    def postProdPath( self ):
        """Returns the path of the PostProd folder (creates it if it does not exist yet)"""

        projectFolder = self.folderPath()
        if not os.path.isdir( projectFolder ):
            return ''

        thePath = RamFileManager.buildPath((
            projectFolder,
            SETTINGS.folderNames.postProd
        ))

        if not os.path.isdir( thePath ):
            try:
                os.makedirs( thePath )
            except:
                return ""

        return thePath

    def assetsPath( self, assetGroup='' ):
        """Returns the path of the Assets folder (creates it if it does not exist yet)"""

        projectFolder = self.folderPath()
        if not os.path.isdir( projectFolder ):
            return ''

        thePath = RamFileManager.buildPath((
            projectFolder,
            SETTINGS.folderNames.assets,
            assetGroup
        ))

        if not os.path.isdir( thePath ):
            try:
                os.makedirs( thePath )
            except:
                return ""
        
        return thePath

    def shotsPath( self ):
        """Returns the path of the Shots folder (creates it if it does not exist yet)"""

        projectFolder = self.folderPath()
        if not os.path.isdir( projectFolder ):
            return ''

        thePath = RamFileManager.buildPath((
            projectFolder,
            SETTINGS.folderNames.shots
        ))

        if not os.path.isdir( thePath ):
            try:
                os.makedirs( thePath )
            except:
                return ""
        
        return thePath

    def exportPath( self ):
        """Returns the path of the Export folder (creates it if it does not exist yet)"""

        projectFolder = self.folderPath()
        if not os.path.isdir( projectFolder ):
            return ''

        thePath = RamFileManager.buildPath((
            projectFolder,
            SETTINGS.folderNames.export
        ))

        if not os.path.isdir( thePath ):
            try:
                os.makedirs( thePath )
            except:
                return ""
        
        return thePath

    def assets( self, assetGroup=None ):
        """Available assets in this project and group.
        If groupName is an empty string, returns all assets.

        Args:
            groupName (str, optional): Defaults to "".

        Returns:
            list of RamAsset
        """

        assets = []

        assetListUuid = self.get("assets", "")
        for uuid in assetListUuid:
            asset = RamAsset( uuid )
            if assetGroup:
                if assetGroup != asset.assetGroup():
                    continue
            assets.append( asset )
        return assets

    def assetGroups( self ):
        """Available asset groups in this project

        Returns:
            list of RamAssetGroup
        """
        assetGroups = []

        assetGroupListUuid = self.get("assetGroups", "")
        for uuid in assetGroupListUuid:
            assetGroups.append( RamAssetGroup(uuid) )
        return assetGroups

    def shots( self, nameFilter = "*", sequence = None ):
        """Available shots in this project

        Args:
            nameFilter

        Returns:
            list of RamShot
        """
        shots = []

        shotListUuid = self.get("shots", "")
        for uuid in shotListUuid:
            shot = RamShot(uuid)
            if sequence:
                if sequence != shot.sequence():
                    continue
            if nameFilter != "*" and nameFilter != "":
                if not nameFilter in shot.name() and not nameFilter in shot.shortName():
                    continue
            shots.append( shot )
        return shots

    def sequences( self ):
        """The sequences of this project"""
        sequences = []

        sequenceListUuid = self.get("sequences", "")
        for uuid in sequenceListUuid:
            sequences.append( RamSequence(uuid) )
        return sequences

    def step(self, shortName):
        """
        Gets a step using its shortName
        
        return:
            RamStep
        """
        stps = self.steps()
        for s in stps:
            if s.shortName() == shortName:
                return s
        return None

    def steps( self, stepType=StepType.ALL ):
        """Available steps in this project. Use type to filter the results.
            One of: RamStep.ALL, RamStep.ASSET_PODUCTION, RamStep.SHOT_PRODUCTION, RamStep.PRE_PRODUCTION, RamStep.PRODUCTION, RamStep.POST_PRODUCTION.
            RamStep.PRODUCTION represents a combination of SHOT and ASSET

        Args:
            typeStep (enumerated value, optional): Defaults to RamStep.ALL.

        Returns:
            list of RamStep
        """
        steps = []

        stepListUuid = self.get("steps", "")
        for uuid in stepListUuid:
            step = RamStep(uuid)
            st = step.stepType()
            if stepType == StepType.ALL:
                steps.append(step)
                continue
            if st == stepType:
                steps.append(step)
                continue
            if stepType == StepType.PRODUCTION:
                if st == StepType.SHOT_PRODUCTION or st == StepType.ASSET_PRODUCTION:
                    steps.append(step)
        return steps

    def pipes( self ):
        """Available pipes in this project

        Returns:
            list of RamPipe
        """
        pipes = []

        pipeListUuid = self.get("pipeline", "")
        for uuid in pipeListUuid:
            pipes.append( RamPipe(uuid) )
        return pipes

    def _getAssetsInFolder(self, folderPath, assetGroup=None ):
        """lists and returns all assets in the given folder"""
        assetList = []
        
        for foundFile in os.listdir( folderPath ):
            # look in subfolder
            if os.path.isdir( folderPath + '/' + foundFile ):
                assets = self._getAssetsInFolder( folderPath + '/' + foundFile, assetGroup )
                assetList = assetList + assets
            
            # Get Asset
            asset = RamAsset.fromPath( folderPath + '/' + foundFile )
            if asset is None:
                continue
            if asset.group() == assetGroup:
                assetList.append( asset )

        return removeDuplicateObjectsFromList( assetList )
