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

import os
from platform import version
from .file_info import RamFileInfo
from .ram_object import RamObject
from .file_manager import RamFileManager
from .file_info import RamFileInfo
from .daemon_interface import RamDaemonInterface
from .logger import log
from .constants import Log, LogLevel, ItemType, FolderNames

# Keep the daemon at hand
DAEMON = RamDaemonInterface.instance()

class RamItem( RamObject ):
    """
    Base class for RamAsset and RamShot.
    An item of the project, either an asset or a shot.
    """

    @staticmethod
    def fromPath( fileOrFolderPath, virtualIfNotFound = False ):
        """Returns a RamAsset or RamShot instance built using the given path.
        The path can be any file or folder path from the asset 
        (a version file, a preview file, etc)

        Args:
            path (str)

        Returns:
            RamAsset or RamShot
        """

        from .ram_asset import RamAsset
        from .ram_shot import RamShot
        from .ramses import Ramses

        RAMSES = Ramses.instance()

        nm = RamFileInfo()
        nm.setFilePath( fileOrFolderPath )

        if nm.ramType == ItemType.ASSET:
            uuid = DAEMON.uuidFromPath( fileOrFolderPath, "RamAsset" )
        elif nm.ramType == ItemType.SHOT:
            uuid = DAEMON.uuidFromPath( fileOrFolderPath, "RamShot" )
        else:
            uuid = DAEMON.uuidFromPath( fileOrFolderPath, "RamItem" )

        if uuid != "":
            if nm.ramType == ItemType.ASSET:
                return RamAsset(uuid)
            elif nm.ramType == ItemType.SHOT:
                return RamShot(uuid)
            else:
                return RamItem(uuid)

        if virtualIfNotFound:
            # Get some info from the file name
            project = RAMSES.project( nm.project )
            projectUuid = ""
            # Get the right folder
            folderPath = RamFileManager.getSaveFilePath( fileOrFolderPath )
            folderPath = os.path.dirname(folderPath)
            if project:
                projectUuid = project.uuid()

            data = {
                "name": nm.shortName,
                "shortName": nm.shortName,
                "project": projectUuid,
                "folderPath": folderPath
            }

            if nm.ramType == ItemType.ASSET:
                return RamAsset(data=data, create=False)
            elif nm.ramType == ItemType.SHOT:
                return RamShot(data=data, create=False)
            else:
                return RamItem(data=data, create=False)

        log( "The given path does not belong to an item", LogLevel.Debug )
        return None

    # Do not document itemType as it's used only by derived classes
    def __init__( self, uuid="", data = None, create=False, objectType="RamItem" ):
        """
        Args:
            uuid (str)
        """
        super(RamItem, self).__init__( uuid, data, create, objectType )
        if objectType == "RamShot":
            self.__itemType = ItemType.SHOT
        elif objectType == "RamAsset":
            self.__itemType = ItemType.ASSET
        else:
            self.__itemType = ItemType.GENERAL

    def currentStatus( self, step ):
        """The current status for the given step

        Args:
            step (RamStep or str): The step.
            resource (str, optional): The name of the resource. Defaults to "".

        Returns:
            RamStatus
        """

        from .ram_status import RamStatus

        # The current status should be the last one
        history = self.stepStatusHistory( step )

        if len(history) == 0:
            return None

        return RamStatus(history[-1])

    def isPublished( self, step="" ):
        """Convenience function to check if there are published files in the publish folder.
            Equivalent to len(self.publishedVersionFolderPaths(step, resource)) > 0

        Args:
            step (RamStep)
            resource (str, optional): Defaults to "".

        Returns:
            bool
        """
        result = self.publishedVersionFolderPaths( step )
        return len( result ) > 0

    def itemType( self ):
        """Returns the type of the item"""
        return self.__itemType

    def latestPublishedVersionFolderPath( self, step="", fileName='', resource=None ):
        """Gets the latest published version folder for the given fileName. Returns the latest folder if the fileName is omitted or an empty string"""
        versionFolders = self.publishedVersionFolderPaths(step, fileName, resource)
        
        if len(versionFolders) == 0:
            return ''
        return versionFolders[-1]

    def latestVersion( self, resource="", state="", step=""):
        """Returns the highest version number for the given state (wip, pub…) (or all states if empty string).

        Args:
            step (RamStep or str)
            resource (str, optional): Defaults to "".
            stateId (str, optional): Defaults to "wip".

        Returns:
            int
        """

        state = RamObject.getShortName(state)
        step = RamObject.getShortName(step)

        highestVersion = -1
       
        versionFolder = self.versionFolderPath( step )
        if versionFolder == '':
            return highestVersion

        for file in os.listdir( versionFolder ):
            nm = RamFileInfo()
            if not nm.setFileName( file ):
                continue
            if nm.step != step and step != '':
                continue
            if nm.resource == resource:
                if nm.state == state or state == "":
                    if nm.version > highestVersion:
                        highestVersion = nm.version

        return highestVersion

    def latestVersionFilePath( self, resource="", state="", step="" ):
        """Latest version file path

        Args:
            step (RamStep or str)
            resource (str, optional): Defaults to "".

        Returns:
            str
        """

        step = RamObject.getShortName(step)

        versionFolderPath = self.versionFolderPath(step )

        if versionFolderPath == '':
            return ''

        versionFile = ''
        highestVersion = -1

        for file in os.listdir( versionFolderPath ):
            nm = RamFileInfo()
            if not nm.setFileName( file ):
                continue
            if nm.step != step and step != '':
                continue
            if nm.resource == resource:
                if nm.state == state or state == '':
                    if nm.version > highestVersion:
                        highestVersion = nm.version
                        versionFile = RamFileManager.buildPath((
                            versionFolderPath,
                            file
                        ))

        return versionFile

    def previewFolderPath( self, step="" ):
        """Gets the path to the preview folder.
            Paths are relative to the root of the item folder.

        Args:
            step (RamStep or str)

        Returns:
            str
        """
        # Check step, return shortName (str) or "" or raise TypeError:
        stepFolder = self.stepFolderPath(step )

        if stepFolder == '':
            return ''

        previewFolder = RamFileManager.buildPath(( 
            stepFolder,
            FolderNames.preview
            ))

        if not os.path.isdir(previewFolder):
            os.makedirs(previewFolder)

        return previewFolder

    def previewFilePaths( self, resource="", step=""):
        """Gets the list of file paths in the preview folder.
            Paths are relative to the root of the item folder.

        Args:
            step (RamStep or str)
            resource (str, optional): Defaults to "".

        Returns:
            list of str
        """

        previewFolderPath = self.previewFolderPath(step)
        # return RamFileManager.getFileWithResource( previewFolderPath, resource)

    def project(self):
        """Returns the project this item belongs to"""
        from .ram_project import RamProject

        groupData = {}

        if self.__itemType == ItemType.SHOT:
            seqUuid = self.get("sequence", "")
            if seqUuid:
                groupData = DAEMON.getData( seqUuid )

        elif self.__itemType == ItemType.ASSET:
            agUuid = self.get("assetGroup", "")
            if agUuid:
                groupData = DAEMON.getData( agUuid )

        projUuid = groupData.get("project", "")
        return RamProject(projUuid)

    def projectShortName(self):
        """Returns the short name of the project this item belongs to"""

        proj = self.project()
        if proj:
            return proj.shortName()
        else:
            return ""

    def publishFolderPath( self, step=""):
        """Gets the path to the publish folder.
        Paths are relative to the root of the item folder.

        Args:
            step (RamStep or str)   

        Returns:
            str
        """
        # Check step, return shortName (str) or "" or raise TypeError:
        stepFolder = self.stepFolderPath(step )

        if stepFolder == '':
            return ''

        publishFolder = RamFileManager.buildPath(( 
            stepFolder,
            FolderNames.publish
            ))

        if not os.path.isdir(publishFolder):
            os.makedirs(publishFolder)

        return publishFolder

    def publishedVersionFolderPaths( self, step="", fileName = '', resource=None ):
        """Gets the list of file paths in the publish folder.
            Paths are relative to the root of the item folder.

        Args:
            step (RamStep or str)
            resource (str, optional): Defaults to None (returns all resources).

        Returns:
            list of str
        """

        publishFolderPath = self.publishFolderPath(step)

        versionFolders = []
        for f in os.listdir(publishFolderPath):
            folderPath = RamFileManager.buildPath(( publishFolderPath, f ))
            if not os.path.isdir(folderPath): continue
            versionFolders.append( folderPath )

        versionFolders.sort(key=RamFileManager._publishVersionFoldersSorter)

        if len(versionFolders) == 0: return versionFolders

        if fileName == '' and resource is None: return versionFolders

        publishedFolders = []
        #filter by filename and resource
        for folder in versionFolders:
            # Check the resource
            if resource is not None:
                folderName = os.path.basename( folder ).split('_')
                if len(folderName) != 3 and resource != '': continue
                elif len(folderName) == 3 and resource != folderName[0]: continue

            if fileName == '':
                publishedFolders.append(folder)
                continue

            for f in os.listdir(folder):
                if f == fileName: publishedFolders.append(folder)

        return publishedFolders

    def setStatus( self, status, step ):
        """Sets the current status for the given step

        Args:
            status (RamStatus)
            step (RamStep)
        """
        stepUuid = RamObject.getUuid(step)
        if stepUuid == "":
            return

        # Get the step history
        statusHistory = self.stepStatusHistory(step)

        # Append the status
        statusHistory.append(status.uuid())

        # Re-set the data
        self.set("statusHistory-" + stepUuid, statusHistory)

    def stepFilePath(self, resource="", extension="", step="", ):
        """Returns a specific step file"""
        step = RamObject.getShortName( step )

        stepFolder = self.stepFolderPath(step)
        if stepFolder == '':
            return ''

        pShortName = self.projectShortName()
        if pShortName == '':
            return ''

        nm = RamFileInfo()
        nm.project = pShortName
        nm.step = step
        nm.extension = extension
        nm.ramType = self.itemType()
        nm.shortName = self.shortName()
        nm.resource = resource

        fileName = nm.fileName()

        filePath = RamFileManager.buildPath((
            stepFolder,
            fileName
        ))
        if os.path.isfile(filePath):
            return filePath
        return ""

    def stepFilePaths( self, step="" ):
        """Returns the step files"""
        step = RamObject.getShortName( step )

        stepFolder = self.stepFolderPath(step)
        if stepFolder == '':
            return []

        pShortName = self.projectShortName()
        if pShortName == '':
            return []

        files = []

        for file in os.listdir(stepFolder):
            # check file
            nm = RamFileInfo()
            if not nm.setFileName( file ):
                continue
            if nm.project != pShortName or nm.step != step or nm.shortName != self.shortName() or nm.ramType != self.itemType():
                continue
            files.append(RamFileManager.buildPath((
                stepFolder,
                file
            )))
        return files

    def stepFolderPath(self, step=""):
        """Returns the working folder for the given step"""
        # General items don't have subfolders for steps
        if self.itemType() == ItemType.GENERAL:
            return self.folderPath()

        # We need a short name
        step = RamObject.getShortName( step )

        folderPath = self.folderPath()
        if folderPath == "" or step == "" or self.itemType() == ItemType.GENERAL:
            return folderPath

        project = self.projectShortName()

        nm = RamFileInfo()
        nm.project = project
        nm.step = step
        nm.ramType = self.itemType()
        nm.shortName = self.shortName()
        stepFolderName = nm.fileName()

        stepFolderPath = RamFileManager.buildPath((
            folderPath,
            stepFolderName
        ))

        if not os.path.isdir(stepFolderPath):
            os.makedirs( stepFolderPath )

        return stepFolderPath

    def stepStatusHistory( self, step ):
        stepUuid = RamObject.getUuid(step)
        if stepUuid == "":
            return []

        return self.get("statusHistory-" + stepUuid)

    def versionFilePaths( self, resource="", step="" ):
        """Gets all version files for the given resource"""

        step = RamObject.getShortName( step )

        versionFolderPath = self.versionFolderPath(step )

        if versionFolderPath == '':
            return ''

        pShortName = self.projectShortName()
        if pShortName == '':
            return []

        files = []

        for file in os.listdir( versionFolderPath ):
            nm = RamFileInfo()
            if not nm.setFileName( file ):
                continue
            if nm.project != pShortName:
                continue
            itemType = self.itemType()
            if nm.ramType != itemType:
                continue
            if itemType == ItemType.GENERAL:
                if self.shortName() != nm.shortName:
                    continue
            else:
                if nm.step != step or nm.shortName != self.shortName():
                    continue
            if nm.resource == resource:
                files.append(RamFileManager.buildPath((
                    versionFolderPath,
                    file
                )))

        files.sort( key = RamFileManager._versionFilesSorter )
        return files

    def versionFolderPath( self, step="" ): 
        """Path to the version folder relative to the item root folder

        Args:
            step (RamStep)

        Returns:
            str
        """
        # Check step, return shortName (str) or "" or raise TypeError:
        stepFolder = self.stepFolderPath( step )

        if stepFolder == '':
            return ''

        versionFolder = RamFileManager.buildPath(( 
            stepFolder,
            FolderNames.versions
            ))

        if not os.path.isdir(versionFolder):
            os.makedirs( versionFolder )
        
        return versionFolder

    # Documented in RamAsset and RamShot
    def group( self ):
        """The name of group containing this asset or shot. (e.g. 'Props' or 'Sequence01')

        Returns:
            str
        """

        groupData = {}

        if self.__itemType == ItemType.SHOT:
            seqUuid = self.get("sequence", "")
            if seqUuid != "":
                groupData = DAEMON.getData( seqUuid )

        elif self.__itemType == ItemType.ASSET:
            agUuid = self.get("assetGroup", "")
            if agUuid != "":
                groupData = DAEMON.getData( agUuid )

        return groupData.get("name", "")