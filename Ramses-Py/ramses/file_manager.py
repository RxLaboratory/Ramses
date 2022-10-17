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

import os, re, shutil, codecs
from threading import Thread
from .ram_settings import RamSettings
from .utils import intToStr
from .logger import log
from .constants import LogLevel, Log, FolderNames
from .file_info import RamFileInfo

# Keep the settings at hand
settings = RamSettings.instance()

class RamFileManager():
    """A Class to help managing files using the Ramses naming scheme"""

    # Cache stuff
    __writingThreads = []

    @staticmethod
    def copy( originPath, destinationPath, separateThread=True ):
        """Copies a file, in a separated thread if separateThread is True"""
        if separateThread:
            t = Thread( target=RamFileManager.copy, args=(originPath, destinationPath, False) )
            log( "Launching parallel copy of a file.", LogLevel.Debug )
            t.start()
            RamFileManager.__writingThreads.append(t)
        else:
            log("Starting copy of: " + os.path.basename( originPath ) + "\nto: " + destinationPath, LogLevel.Debug )
            shutil.copy2( originPath, destinationPath )
            log("Finished writing: " + os.path.basename( destinationPath ), LogLevel.Debug )

    @staticmethod
    def waitFiles():
        """Waits for all writing operations to finish"""
        for t in RamFileManager.__writingThreads:
            t.join()

    @staticmethod
    def getRamsesFiles( folderPath, resource = None ):
        """Gets all files respecting the Ramses naming scheme in the given folder
        Returns a list of file paths"""
        if not os.path.isdir(folderPath):
            return []

        files = []

        for f in os.listdir(folderPath):
            nm = RamFileInfo()
            if nm.setFileName(f):
                if resource is None or nm.resource == resource:
                    files.append( RamFileManager.buildPath((
                        folderPath,
                        f
                    )))

        return files

    @staticmethod
    def isAssetStep( stepShortName, assetsPath ):
        """Checks the production type of the given step in the assets folder"""
        # Try to find the type from shots or assets folders
        if os.path.isdir( assetsPath ):
            # each folder is an asset group
            for folder in os.listdir( assetsPath ):
                groupPath = assetsPath + "/" + folder 
                # Check only dirs
                if not os.path.isdir( groupPath ):
                    continue
                # each folder is an asset
                for assetFolder in os.listdir( groupPath ):
                    assetPath = groupPath + '/' + assetFolder
                    if not os.path.isdir( assetPath ):
                        continue
                    # each folder is a step working folder in the asset
                    for stepFolder in os.listdir( assetPath ):
                        nm = RamFileInfo()
                        if nm.setFileName( stepFolder ):
                            if nm.step == stepShortName:
                                return True
        return False

    @staticmethod
    def isShotStep( shotShortName, shotsPath ):
        """Checks the production type of the given step in the shots folder"""
        if os.path.isdir( shotsPath ):
            #  each folder is a shot
            for shotFolder in os.listdir( shotsPath ):
                shotPath = shotsPath + '/' + shotFolder
                if not os.path.isdir( shotPath ):
                    continue
                # each folder is a step working folder in the shot
                for shotFolder in os.listdir( shotPath ):
                    nm = RamFileInfo()
                    if nm.setFileName( shotFolder ):
                        if nm.step == shotShortName:
                            return True
        return False

    @staticmethod
    def getProjectFolder( path ):
        """Tries to get the root folder of the project"""

        name = os.path.basename( path )

        while name != '':
            if RamFileManager.isProjectFolder( path ):
                return path
            # Move up to the parent folder
            path = os.path.dirname(path)
            name = os.path.basename(path)
        
        return ''

    @staticmethod
    def isProjectFolder( folderPath ):
        """Checks if the given folder is the project root"""
        if not os.path.isdir( folderPath ):
            return False

        foundFiles = os.listdir( folderPath )
        for foundFile in foundFiles:
            if os.path.isfile(foundFile):
                continue
            folderName = os.path.basename( foundFile )
            if folderName in (
                    '00-ADMIN',
                    '01-PRE-PROD',
                    '02-PROD',
                    '03-POST-PROD',
                    '04-ASSETS',
                    '05-SHOTS',
                    '06-EXPORT'
                ):
                return True

        return False

    @staticmethod
    def getSaveFilePath( path ):
        """Gets the save path for an existing file.
        This path is not the same as the file path if the file path is located in the versions/preview/publish subfolder"""

        nm = RamFileInfo()
        nm.setFilePath( path )
        if nm.project == '':
            return ""

        if nm.ramType == '':
            return ""

        if os.path.isfile( path ):
            saveFolder = os.path.dirname( path )
        else:
            saveFolder = path

        if saveFolder == "":
            return ""

        # Get up the tree until we find a reserved folder and get out of it!
        path = os.path.dirname(saveFolder)
        name = os.path.basename(path)
        while name != '':
            # Found it!
            if RamFileManager.isReservedFolder( path ):
                saveFolder = os.path.dirname( path )
                break
            # Move up to the parent folder
            path = os.path.dirname(path)
            name = os.path.basename(path)

        saveFileName = nm.fileName()

        return RamFileManager.buildPath(( saveFolder, saveFileName ))

    @staticmethod
    def restoreVersionFile( filePath, separateThread=True ):
        """Restores a file from a versions subfolder"""
        if not RamFileManager.inVersionsFolder( filePath ):
            log( "This version can't be restored, it is not in a Ramses version subfolder.", LogLevel.Critical )
            return

        from .metadata_manager import RamMetaDataManager

        fileName = os.path.basename( filePath )

        nm = RamFileInfo()
        if not nm.setFileName( fileName ):
            log( Log.MalformedName, LogLevel.Critical )
            return

        # Set the resource, remove state and version
        nm.resource = nm.resource +  "+restored-v" + str(nm.version) + "+"
        nm.state = ""
        nm.version = -1
        restoredFileName = nm.fileName()

        versionFolder = os.path.dirname( filePath )
        saveFolder = os.path.dirname( versionFolder )

        restoredFilePath = saveFolder + '/' + restoredFileName
        RamFileManager.copy( filePath, restoredFilePath,separateThread )
        RamMetaDataManager.appendHistoryDate( restoredFilePath )
        return restoredFilePath

    @staticmethod
    def publishFile( filePath ):
        """Copies the given file to its corresponding publish folder"""
        from .metadata_manager import RamMetaDataManager

        fileInfo = RamFileManager.getPublishInfo( filePath )
        if fileInfo.project == "":
            return

        publishFilePath = fileInfo.filePath()
        RamFileManager.copy( filePath, publishFilePath )
        RamMetaDataManager.appendHistoryDate( publishFilePath )

        return publishFilePath

    @staticmethod
    def getPublishInfo( filePath ):
        """Gets the publish info of the given file (including the version subfolder)"""  

        if not os.path.isfile( filePath ):
            raise Exception( "Missing File: Cannot publish a file which does not exists: " + filePath )

        log("Getting publish file: " + filePath, LogLevel.Debug)

        # Check File Name
        fileInfo = RamFileInfo()
        fileInfo.setFilePath( filePath )

        publishFolder = RamFileManager.getPublishFolder( filePath )

        # Check version
        versionInfo = RamFileManager.getLatestVersionInfo( filePath )

        # Generate Subfolder name
        versionFolder = ""
        # resource if any
        if versionInfo.resource != "":
            versionFolder = versionInfo.resource + "_"
        # version number
        if (versionInfo.version <= 0): versionFolder = versionFolder + intToStr( 1 )
        else: versionFolder = versionFolder + intToStr( versionInfo.version )
        # State
        if versionInfo.state != "" and versionInfo.state.lower() != "v":
            versionFolder = versionFolder + "_" + versionInfo.state

        # The complete path
        newFilePath = RamFileManager.buildPath ((
            publishFolder,
            versionFolder
        ))
        # make it if it does not exist yet
        if not os.path.isdir( newFilePath ):
            os.makedirs( newFilePath )

        # add the fileName
        newFilePath = RamFileManager.buildPath ((
            newFilePath,
            fileInfo.fileName()
        ))

        # store in a new info
        publishedInfo = RamFileInfo()
        publishedInfo.setFilePath( newFilePath )
        # Reset the date, version, etc
        publishedInfo.date = fileInfo.date
        publishedInfo.version = versionInfo.version
        if versionInfo.state != "" and versionInfo.state.lower() != "v":
            publishedInfo.state = versionInfo.state

        return publishedInfo

    @staticmethod
    def getPublishedVersions( filePath ):
        """"Gets all the version subfolders in the publish path"""
        folder = RamFileManager.getPublishFolder( filePath )

        folders = []
        for f in os.listdir(folder):
            folderPath = RamFileManager.buildPath(( folder, f ))
            if not os.path.isdir(folderPath): continue
            folders.append( folderPath )

        return folders

    @staticmethod
    def copyToVersion( filePath, increment = False, stateShortName="" ):
        """Copies and increments a file into the version folder

        Returns the filePath of the new file version"""
        from .metadata_manager import RamMetaDataManager

        if not os.path.isfile( filePath ):
            raise Exception( "Missing File: Cannot increment a file which does not exists: " + filePath )

        log("Incrementing version for file: " + filePath, LogLevel.Debug)

        # Check File Name
        fileInfo = RamFileInfo()
        fileInfo.setFilePath( filePath )
        if fileInfo.project == '':
            log( Log.MalformedName, LogLevel.Critical )
            return

        # Look for the latest version to increment and save
        versioInfo = RamFileManager.getLatestVersionInfo( filePath, stateShortName )
        
        fileInfo.version = versioInfo.version
        if stateShortName == "":
            fileInfo.state = versioInfo.state
        else:
            fileInfo.state = stateShortName

        if increment:
            fileInfo.version += 1

        if fileInfo.version <= 0:
            fileInfo.version = 1

        newFileName = fileInfo.fileName()

        versionsFolder = RamFileManager.getVersionFolder( filePath )

        newFilePath = RamFileManager.buildPath(( versionsFolder, newFileName ))
        RamFileManager.copy( filePath, newFilePath )
        RamMetaDataManager.appendHistoryDate( newFilePath )
        return newFilePath

    @staticmethod
    def getLatestVersionInfo( filePath, defaultStateShortName="v", previous = False ):
        """Gets the latest version number and state of a file
        
        Returns RamFileInfo
        """

        latestVersionFilePath = RamFileManager.getLatestVersionFilePath( filePath, previous )
        versionInfo = RamFileInfo()
        versionInfo.setFilePath( latestVersionFilePath )
        if versionInfo.state == '':
            versionInfo.state = defaultStateShortName
        return versionInfo

    @staticmethod
    def getLatestVersionFilePath( filePath, previous=False ):
        """Gets the file path of the latest version"""
        # Check File Name
        fileName = os.path.basename( filePath )
        nm = RamFileInfo()
        if not nm.setFileName( fileName ):
            log( Log.MalformedName, LogLevel.Critical )
            return ''

        # Get versions
        versionsFolder = RamFileManager.getVersionFolder( filePath )

        foundFiles = os.listdir( versionsFolder )
        highestVersion = 0

        versionFilePath = ''
        prevVersionFilePath = ''

        for foundFile in foundFiles:
            if not os.path.isfile( versionsFolder + '/' + foundFile ): # This is in case the user has created folders in _versions
                continue

            foundNM = RamFileInfo()
            if not foundNM.setFileName( foundFile ):
                continue
            if foundNM.project != nm.project:
                continue
            if foundNM.ramType != nm.ramType:
                continue
            if foundNM.shortName != nm.shortName:
                continue
            if foundNM.step != nm.step:
                continue
            if foundNM.resource != nm.resource:
                continue
            if foundNM.version == -1:
                continue

            version = foundNM.version
            if version > highestVersion:
                highestVersion = version
                prevVersionFilePath = versionFilePath
                versionFilePath = versionsFolder + '/' + foundFile

        if previous:
            return prevVersionFilePath

        return versionFilePath

    @staticmethod
    def getVersionFilePaths( filePath ):
        # Check File Name
        fileName = os.path.basename( filePath )
        nm = RamFileInfo()
        if not nm.setFileName( fileName ):
            log( Log.MalformedName, LogLevel.Critical )

        # Get versions
        versionsFolder = RamFileManager.getVersionFolder( filePath )

        foundFiles = os.listdir( versionsFolder )
        versionFiles = []

        for foundFile in foundFiles:
            foundFilePath = versionsFolder + '/' + foundFile
            if not os.path.isfile( foundFilePath ): # This is in case the user has created folders in _versions
                continue
            
            foundNM = RamFileInfo()
            if not foundNM.setFileName( foundFile ):
                continue
            if foundNM.project != nm.project:
                continue
            if foundNM.ramType != nm.ramType:
                continue
            if foundNM.shortName != nm.shortName:
                continue
            if foundNM.step != nm.step:
                continue
            if foundNM.resource != nm.resource:
                continue

            versionFiles.append( foundFilePath )

        versionFiles.sort( key = RamFileManager._versionFilesSorter )
        return versionFiles

    @staticmethod
    def getVersionFolder( filePath ):
        """Gets the versions folder for this file"""

        fileFolder = os.path.dirname( filePath )
        versionsFolderName = settings.folderNames.versions

        if RamFileManager.inVersionsFolder( filePath ):
            versionsFolder = fileFolder

        elif RamFileManager.inPublishFolder( filePath ) or RamFileManager.inPreviewFolder( filePath ):
            wipFolder = os.path.dirname( fileFolder )
            versionsFolder = wipFolder + '/' + versionsFolderName
        
        else:
            versionsFolder = fileFolder + '/' + versionsFolderName

        if not os.path.isdir( versionsFolder ):
            os.makedirs( versionsFolder )

        return versionsFolder

    @staticmethod
    def getPublishFolder( filePath ):
        """Gets the published folder for this file"""

        fileFolder = os.path.dirname( filePath )
        publishFolderName = settings.folderNames.publish

        if RamFileManager.inPublishFolder( filePath ):
            publishFolder = fileFolder

        elif RamFileManager.inVersionsFolder( filePath ) or RamFileManager.inPreviewFolder( filePath ):
            wipFolder = os.path.dirname( fileFolder )
            publishFolder = wipFolder + '/' + publishFolderName

        else:
            publishFolder = fileFolder + '/' + publishFolderName

        if not os.path.isdir( publishFolder ):
            os.makedirs( publishFolder )

        return publishFolder

    @staticmethod
    def inPreviewFolder( path ):
        """Checks if the given path is inside a "preview" folder"""
        currentFolder = os.path.dirname(path)
        currentFolderName = os.path.basename( currentFolder )
        return currentFolderName == settings.folderNames.preview

    @staticmethod
    def inPublishFolder( path ):
        """Checks if the given path is inside a "published" folder"""
        currentFolder = os.path.dirname(path)
        currentFolderName = os.path.basename( currentFolder )
        if currentFolderName == settings.folderNames.publish: return True
        currentFolder = os.path.dirname(currentFolder)
        currentFolderName = os.path.basename( currentFolder )
        return currentFolderName == settings.folderNames.publish

    @staticmethod
    def inVersionsFolder( path ):
        """Checks if the given path is inside a "versions" folder"""
        currentFolder = os.path.dirname(path)
        currentFolderName = os.path.basename( currentFolder )
        return currentFolderName == settings.folderNames.versions

    @staticmethod
    def isReservedFolder( path ):
        """Checks if this is a reserved folder"""
        name = os.path.basename( path )
        return name in [
            FolderNames.versions,
            FolderNames.publish,
            FolderNames.preview
        ]

    @staticmethod
    def inReservedFolder( path ):
        """Checks if the given path is (directly) inside a "versions/preview/published" folder"""
        currentFolder = path
        if os.path.isfile( path ):
            currentFolder = os.path.dirname( path )

        return RamFileManager.isReservedFolder( currentFolder )
    
    @staticmethod
    def validateName( name ):
        """Checks if the name is valid, respects the Ramses naming scheme"""

        # Accept empty names
        if name == "":
            return True

        regex = re.compile('^[ a-zA-Z0-9+-]{1,256}$', re.IGNORECASE)
        if re.match(regex, name):
            return True
        return False

    @staticmethod
    def validateShortName( name ):
        """Checks if the name is valid, respects the Ramses naming scheme"""
        regex = re.compile('^[a-z0-9+-]{1,10}$', re.IGNORECASE)
        if re.match(regex, name):
            return True
        return False

    @staticmethod
    def buildPath( folders ):
        """Builds a path with a list of folder names or subpaths,
        adding the '/' only if needed, and ignoring empty blocks"""

        fullPath = ''

        for folder in folders:
            if folder == '':
                continue
            if not fullPath.endswith('/') and not fullPath == '':
                fullPath = fullPath + '/'
                
            fullPath = fullPath + folder

        return fullPath

    @staticmethod
    def _isRamsesItemFoldername( n ):
        """Low-level, undocumented. Used to check if a given folder respects Ramses' naming convention for items' root folders.
        
        The root folder should look like this:
            projectID_ramType_objectShortName

        Returns: bool
        """
        if re.match( '^([a-z0-9+-]{1,10})_([ASG])_([a-z0-9+-]{1,10})$', n , re.IGNORECASE): return True
        return False

    @staticmethod
    def _fixResourceStr( resourceStr ):
        """Low-level, undocumented. Used to remove all forbidden characters from a resource.

        Returns: str
        """
        forbiddenCharacters = {
            '"' : ' ',
            '_' : '-',
            '[' : '-',
            ']' : '-',
            '{' : '-',
            '}' : '-',
            '(' : '-',
            ')' : '-',
            '\'': ' ',
            '`' : ' ',
            '.' : '-',
            '/' : '-',
            '\\' : '-',
            ',' : ' ' 
            }

        fixedResourceStr = ''
        for char in resourceStr:
            if char in forbiddenCharacters:
                fixedResourceStr = fixedResourceStr + forbiddenCharacters[char]
            else:
                fixedResourceStr = fixedResourceStr + char
        return fixedResourceStr

    @staticmethod
    def _versionFilesSorter( f ):
        fileName = os.path.basename(f)
        nm = RamFileInfo()
        if not nm.setFileName( fileName ):
            return -1
        return nm.version

    @staticmethod
    def _publishVersionFoldersSorter( f ):
        folderName = os.path.basename(f)
        folderNameList = folderName.split('_')
        numBlocks = len(folderNameList)
        # Invalid, return the lowest value
        if numBlocks == 0 or numBlocks > 3: return float('-inf')
        # Single or dual block, should be a number
        if numBlocks == 1 or numBlocks == 2: return int(folderNameList[0])
        # Triple block, there's a resource, we return the version
        # -resourceId + version
        resourceBytes = folderNameList[0].encode('utf_8', 'replace')
        resourceInt = - int( codecs.encode( resourceBytes, 'hex' ), 16 ) * 1000
        return resourceInt + int( folderNameList[1] )
