import os
import re
from datetime import datetime

def getDate(e): #used in RamItem.getStepHistory to sort the list
    return e.date

def escapeRegEx( string ):
    reservedChars = "[.*+-?^=!:${|}[]\\/()"
    result = ""
    for char in string:
        if char in reservedChars:
            result = result + "\\" + char
        else:
            result = result + char
    return result

def getVersionRegEx():
    regexStr = getVersionRegExStr()
    regexStr = '^(' + regexStr + ')?(\\d+)$'
    regex = re.compile(regexStr, re.IGNORECASE)
    return regex

def getVersionRegExStr():
    if not Ramses.instance:
        raise Exception("Ramses must be instanciated.")

    version_prefixes = ['v','pub']
    for state in Ramses.instance.getStates():
        version_prefixes.append( state.shortName )

    regexStr = ''
    for prefix in version_prefixes[0:-1]:
        regexStr = regexStr + prefix + '|'
    regexStr = regexStr + version_prefixes[-1]
    return regexStr

def getRamsesNameRegEx():
    regexStr = getVersionRegExStr()

    regexStr = '^([a-z0-9+-]{1,10})_(?:([AS])_([a-z0-9+-]{1,10})|(G))_([a-z0-9+-]{1,10})(?:_((?!(?:' + regexStr + ')?[0-9]+)[a-z0-9+\\s-]+))?(?:_(' + regexStr + ')?([0-9]+))?\\.([a-z0-9.]+)$'

    regex = re.compile(regexStr, re.IGNORECASE)
    return regex

def isVersion(v):
    if re.match(getVersionRegEx(), v): return True
    return False

def isRamsesName(n):
    """Returns True if the given string respects the Ramses naming convention.

    The name should look like this:
        projShortName_ramType_objectShortName_stepShortName_resourceStr_versionPrefixVersionNumber.extension
    in which the ramType can be one of the following letters: A (asset), S (shot), G (general)
    and there is an objectShortName only for assets and shots;
    and the resourceStr is optional: it only serves to differentiate the main working files and its resources
    and the versionPrefixVersionNumber is optional
    Will return false if there is no extension.

    Args:
        n : name to be checked, without its path. Needs its extension
    """
    if re.match(getRamsesNameRegEx(), n): return True
    return False

def isRamsesItemFoldername(n):
    """Returns True if the given string respects the Ramses naming convention for items' root folders.

    Eg. Project01_A_ISOLDE
    """
    if re.match('^([a-z0-9+-]{1,10})_[ASG]_([a-z0-9+-]{1,10})$' , n , re.IGNORECASE): return True
    return False

def fixResourceStr( resourceStr ):
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

def buildRamsesFileName( project , step , ext , ramType = 'G' , objectShortName = '' , resourceStr = "" , version = -1 , version_prefixe = 'wip' ):
    #PROJECT_A_OBJECT_STEP_resourceStr_wip012.extension
    #PROJECT_G_STEP_resourceStr_wip012.extension

    resourceStr = fixResourceStr( resourceStr )
    ramsesFileName = project + '_' + ramType

    if ramType in ('A', 'S'):
        ramsesFileName = ramsesFileName + '_' + objectShortName

    ramsesFileName = ramsesFileName + '_' + step

    if resourceStr != '':
        ramsesFileName = ramsesFileName + '_' + resourceStr

    if version != -1:
        ramsesFileName = ramsesFileName + '_' + version_prefixe
        if version < 10:
            ramsesFileName = ramsesFileName + '00' + str(version)
        elif version < 100:
            ramsesFileName = ramsesFileName + '0' + str(version)
        else:
            ramsesFileName = ramsesFileName + str(version)
    
    ramsesFileName = ramsesFileName + '.' + ext

    return ramsesFileName

def decomposeRamsesFileName( ramsesFileName ):
    if type(ramsesFileName) != str:
        print("The given filename is not a str.")
        return None

    splitRamsesName = re.match(getRamsesNameRegEx(), ramsesFileName)

    if splitRamsesName == None:
        return None

    ramType = ''
    objectShortName = ''

    if splitRamsesName.group(2) in ('A', 'S'):
        ramType = splitRamsesName.group(2)
        objectShortName = splitRamsesName.group(3)
    else:
        ramType = splitRamsesName.group(4)

    optionalBlocks = ['', '', '', '']
    for i in range(0, 4):
        if splitRamsesName.group(i + 6) != None:
            optionalBlocks[i] = splitRamsesName.group( i + 6)

    blocks = {
        "projectID": splitRamsesName.group(1),
        "ramType": ramType,
        "objectShortName": objectShortName,
        "ramStep": splitRamsesName.group(5),
        "resourceStr": optionalBlocks[0],
        "state": optionalBlocks[1],
        "version": optionalBlocks[2],
        "extension": optionalBlocks[3],
    }

    return blocks

def incrementRamsesFileName( ramsesFileName ):
    separatedBlocks = decomposeRamsesFileName( ramsesFileName )
    
    if separatedBlocks == None:
        print("The given filename does not match Ramses' naming convention.")
        return None

    if separatedBlocks["version"] == '':
        print("it has no version; creating one")
        ramsesFileName = ramsesFileName.split('.')[0] + "_wip001." + separatedBlocks["extension"]
        return ramsesFileName
    
    #Rebuilding name up to the version str:
    ramsesFileName = ''
    ramsesFileName = separatedBlocks["projectID"] + "_" + separatedBlocks["ramType"] + "_"
    if separatedBlocks["ramType"] in ('A', 'S'):
        ramsesFileName = ramsesFileName + separatedBlocks["objectShortName"] + "_"
    ramsesFileName = ramsesFileName + separatedBlocks["ramStep"] + "_"
    if separatedBlocks["resourceStr"] != '':
        ramsesFileName = ramsesFileName + separatedBlocks["resourceStr"] + "_"
    ramsesFileName = ramsesFileName + separatedBlocks["state"]

    #Creating new version str
    version = int(separatedBlocks["version"]) + 1
    version = str(version)
    while len(version) < len(separatedBlocks["version"]):
        version = "0" + version
    ramsesFileName = ramsesFileName + version + "." + separatedBlocks["extension"]

    return ramsesFileName

def log( message ):
    print(message)
    return

class Ramses():
    """The main class. One (and only one) instance globally available, instantiated during init time.

    Attributes:
        autoConnect: bool
            Whether to always try to (re)connect if offline.
        clientPort: int
            The port used to connect to the client.
        currentProject: RamProject
            (Read-only) The current project.
        currentUser: RamUser
            (Read-only) The current user.
        online: bool
            (Read-only) True if connected to the client and the client is responding.
    """

    def __init__(self, port = 1818, connect = True):
        """

        Args:
            port: int
            connect: bool
        """

        if Ramses.instance:
            raise Exception("There cannot be more than one instance of Ramses")

        self.clientPort = port
        self.autoConnect = True
        self._currentProject = None
        self._currentUser = None
        self._online = False
        Ramses.instance = self

        if connect:
            self.launchClient( True )

    @property
    def currentProject(self):
        return self._currentProject

    @property
    def currentUser(self):
        return self._currentUser

    @property
    def online(self):
        return self._online
    
    def backupFolderPath(self):
        """A copy of the main folder where all files are stored.

        Returns: str
        """
        #TODO
        pass

    def connect(self):
        """ Checks server or client availability and initiates the connection. Returns success.

        Returns: bool
        """
        #TODO
        pass

    def disconnect(self):
        """Gets back to offline mode.

        Returns: bool
        """
        #TODO
        pass

    def getAlternativeFolderPaths(self):
        """A list of alternative absolute paths to the main Ramses folder.
        
        Missing files will be looked for in these paths (and copied to the main path if available), and they will be used if the main path is not available.

        Returns: str list
        """
        #TODO
        pass

    def getProjects(self):
        """The list of available projects.
        
        Returns: list of RamProject
        """
        #TODO
        pass

    def getStates(self):
        """The list of available states

        Returns: list of RamState
        """
        # TODO get the list from the client
        states = [
            RamState("No", "NO", 1.0),
            RamState("To Do", "TODO", 0.0),
            RamState("Work in progress", "WIP", 0.2),
            RamState("OK", "OK", 1.0),
        ]
        return states

    def getState(self, stateShortName = "WIP"):
        for state in self.getStates():
            if state.shortName == stateShortName:
                return state
        return self.getState()

    def getSteps(self, typeOrCat = "ALL"):
        """The list of available steps.

        Use typeOrCat to filter the results.
        One of: ALL, ASSET, SHOT, PRE-PROD, PROD, POST-PROD.
        PROD represents a combination of SHOT and ASSET

        Returns: list of RamStep
        """
        #TODO
        pass

    def getUser(self, userShortName= ""):
        """Gets a specific user

        Args:
            userShortName: str
                The short name of the user.
            
        Returns: RamUser
        """

        if userShortName == "":
            return RamUser("J. Doe", "Anonymous")
        
        for user in self.getUsers():
            if user.shortName == userShortName: return user

        return RamUser("J. Doe", "Anonymous")

    def getUsers(self):
        """The list of available users.

        Returns: list of RamUser
        """
        #TODO
        pass

    def folderPath(self):
        """The absolute path to main Ramses folder, containing projects by default, config files, user folders, admin files...

        Returns: str
        """
        #TODO
        pass

    def launchClient(self, connect = True):
        """Launches the Ramses client and tries to connect to it. Returns success.

        Returns: bool
        """
        #TODO
        return False

    def login(self, userPassword, user = currentUser):
        """Logs the user in. Launches and connect to the client if necessary. Returns success.

        Returns: bool
        """
        #TODO
        pass
    
    def logout(self):
        """Logs the user out.
        """
        #TODO
        pass

    def request(self, request):
        """Posts a request to the connected client.

        Args:
            request: dict
                A dict which will be stringified as JSON: the request to post.

        Returns: None or dict
            The answer parsed from JSON.
        """
        return None

    def setCurrentProject(self, project):
        """

        Args:
            project: RamProject
        """
        #TODO
        pass

    def setCurrentUser(self, user):
        """

        Args:
            user: RamUser
        """
        #TODO
        pass

class RamObject():
    """The base class for most of Ramses objects.
    
    Attributes:
        name: str.
            May contain spaces, [a-z] ,[A-Z], [0-9], [+-].
        shortName: str.
            Used for compact display and folder names, limited to 10 characters, must not contain spaces, may contain [a-z] ,[A-Z], [0-9], [+-].
        folderPath: str.
            The path relative to a project or the main Ramses folder, containing general files in relation to this object.
    """

    def __init__(self):
        self.name = ""
        self.shortName = ""
        self.folderPath = ""

class RamUser( RamObject ):
    """The class representing users.
    
    Attributes:
        role: (Read-only) enumerated value.
            'ADMIN', 'LEAD', or 'STANDARD'
    """

    def __init__(self, userName, userShortName, userFolderPath = "", role = 'STANDARD'):
        """

        Args:
            userName: str.
            userShortName: str.
            useFolderPath: str.
        """
        self.name = userName
        self.shortName = userShortName
        self.folderPath = userFolderPath
        self._role = "STANDARD"
    
    @property
    def role(self):
        return self._role
    
    def login(self):
        """Logs the user in. Returns success.

        Returns: bool
        """
        #TODO
        pass
    
    def logout(self):
        """Logs the user out.

        Returns: bool
        """
        #TODO
        pass

class RamApplication( RamObject ):
    """A class representing an application used in the pipeline (like Blender, Nuke, Krita, Photoshop...).

    Attributes:
        executableFilePath: str
            Path to the executable file of the application.
        exportTypes: list of RamFileType
            File types the app can export.
        importTypes: list of RamFileType
            File types the app can import.
        nativeTypes: list of RamFileType
            File types the app supports natively.
    """

    def __init__(self, appName, appShortName, execFilePath = ""):
        """

        Args:
            appName: str
            appShortName: str
            execFilePath: str
        """
        self.name = appName
        self.shortName = appShortName
        self.executableFilePath = execFilePath
        self.exportTypes = []
        self.importTypes = []
        self.nativeTypes = []

class RamFileType( RamObject ):
    """A class representing file types and their association with RamApplications.

    Attributes:
        defaultApplication: RamApplication
        extension: str
    """

    def __init__(self, typeName, typeShortName, ext, defaultApp):
        """By default, extension and shortName are the same.
        
        Args:
            typeName: str
            typeShortName: str
            ext: str
            defaultApp: RamApplication
        """
        self.name = typeName
        self.shortName = typeShortName
        self.extension = ext
        self.defaultApplication = defaultApp

class RamProject( RamObject ):
    """A project handled by Ramses. Projects contains general items, assets and shots.
    """
    def __init__(self, projectName, projectShortName, projectPath):
        """

        Args:
            projectName: str.
            projectShortName: str.
            projectPath: str.
        """
        self.name = projectName
        self.shortName = projectShortName
        self.folderPath = projectPath

    def getAbsolutePath(self, relativePath):
        """

        Attrs:
            relativePath: str
        
        Returns: str
        """
        return self.folderPath + '/' + relativePath

    def getAssets(self, groupName = ""):
        """If groupName is an empty string, returns all assets.

        Returns list of RamAsset
        """
        if not Ramses.instance:
            raise Exception("Ramses has to be instantiated first.")
        if not isinstance(groupName, str):
            raise TypeError("Group name must be a str")

        # If we're online, ask the client
        if Ramses.instance.online:
            # TODO
            return None

        # Else check in the folders
        assetsFolderPath = self.folderPath + '/04-ASSETS'
        groupsToCheck = []
        foundAssets = []
        foundFiles = []

        if groupName == "": #List all assets and groups found at the root
            foundFiles = os.listdir( assetsFolderPath )
            for foundFile in foundFiles:
                if not os.path.isdir( assetsFolderPath + '/' + foundFile): continue
                if isRamsesItemFoldername(foundFile):
                    if not foundFile.split('_')[1] == 'A': continue
                    foundAssetName = foundFile.split('_')[2]
                    foundAssetPath = "04-ASSETS/" + foundFile
                    foundAsset = RamAsset(assetName = foundAssetName, assetShortName = foundAssetName, folderPath = foundAssetPath)
                    foundAssets.append(foundAsset)
                else:
                    groupsToCheck.append(foundFile)
        else:
            if not os.path.isdir( assetsFolderPath + '/' + groupName):
                print("The following group of assets: " + groupName + " could not be found")
                return None
            groupsToCheck.append(groupName)
        
        for group in groupsToCheck:
            print("Checking this group: " + group)
            foundFiles = os.listdir( assetsFolderPath + '/' + group )
            for foundFile in foundFiles:
                if not os.path.isdir(assetsFolderPath + '/' + group + '/' + foundFile): continue
                if not isRamsesItemFoldername(foundFile): continue
                if not foundFile.split('_')[1] == 'A': continue
                foundAssetName = foundFile.split('_')[2]
                foundAssetPath = "04-ASSETS/" + group + "/" + foundFile
                foundAsset = RamAsset(assetName = foundAssetName, assetShortName = foundAssetName, folderPath = foundAssetPath)
                foundAssets.append(foundAsset)
        
        return foundAssets

    def getAssetGroups(self):
        """

        Returns: list of str
        """
        if not Ramses.instance:
            raise Exception("Ramses has to be instantiated first.")

        # If we're online, ask the client
        if Ramses.instance.online:
            # TODO
            return None

        # Else check in the folders
        assetsFolderPath = self.folderPath + '/04-ASSETS'
        if not os.path.isdir(assetsFolderPath):
            raise Exception("The asset folder for " + self.name + " (" + self.shortName + ") " + "could not be found.")

        foundFiles = os.listdir(assetsFolderPath)
        assetGroups = []

        for foundFile in foundFiles:
            if not os.path.isdir(assetsFolderPath + '/' + foundFile): continue
            if isRamsesItemFoldername(foundFile): continue
            assetGroups.append(foundFile)

        return assetGroups

    def getShots(self, filter = "*"):
        """A filter to be applied to the name of the shots, using "*"" as a wildcard.

        Returns: list of RamShot
        """
        if not Ramses.instance:
            raise Exception("Ramses has to be instantiated first.")

        # If we're online, ask the client
        if Ramses.instance.online:
            # TODO
            return None

        # Else check in the folders
        shotsFolderPath = self.folderPath + '/05-SHOTS'
        if not os.path.isdir(shotsFolderPath):
            raise Exception("The asset folder for " + self.name + " (" + self.shortName + ") " + "could not be found.")

        if filter != "" and not "*" in filter: #User is looking for a specific shot: no need to parse through everything
            foundShotPath = shotsFolderPath + '/' + self.shortName + '_S_' + filter
            if not os.path.isdir( foundShotPath):
                print("Shot " + filter + " was not found.")
                return []
            return [RamShot(shotName = filter, folderPath = foundShotPath)]

        if "*" in filter and filter != "*": #Preparing regex for wildcards
            filter = escapeRegEx(filter)
            filter = filter.replace('\\*' , '([a-z0-9+-]{1,10})?')
            regex = re.compile(filter, re.IGNORECASE)
        
        foundFiles = os.listdir(shotsFolderPath)
        foundShots = []

        for foundFile in foundFiles:
            if not os.path.isdir(shotsFolderPath + '/' + foundFile): continue
            if not isRamsesItemFoldername(foundFile): continue
            if not foundFile.split('_')[1] == 'S': continue

            foundShotName = foundFile.split('_')[2]
            
            if not filter in ("" , "*"):
                if not re.match(regex, foundShotName):
                    continue

            foundShotPath = shotsFolderPath + '/' + foundFile
            foundShot = RamShot( shotName = foundShotName , folderPath = foundShotPath )
            foundShots.append(foundShot)

        return foundShots

    def getSteps(self, typeOrCat = "ALL"): #TODO
        """Use typeOrCat to filter the results.

        One of: ALL, ASSET, SHOT, PRE-PROD, PROD, POST-PROD.
        PROD represents a combination of SHOT and ASSET

        Returns: list of RamStep
        """
        #TODO
        pass

class RamStep( RamObject ):
    """Base class for RamAsset and RamShot. A step in the production of the shots or assets of the project.

    Attributes:
        assignedUsers: list of RamUser.
            Users working on this step.
        fileType: RamFileType.
            The main file type used for working on this step.
        leads: list of RamUser.
            Users leading (i.e. with admin rights and validating assets) this step.
        publishFileTypes: list of RamFileType.
            The file types published by this step.
        secondaryFileTypes: list of RamFileType
            Other file types which may be used when working on this step.
    """
    def __init__(self, stepName, stepShortName):
        """

        Args:
            stepName: str.
            stepShortName: str.
        """
        self.name = stepName
        self.shortName = stepShortName
        self.assignedUsers = []
        self.fileType = None
        self.leads = []
        self.publishedFileTypes = []
        self.secondaryFileTypes = []

class RamAssetStep( RamStep ):
    """A step in the production of the assets of the project.

    Attributes:
        dependsOn: list of RamAssetStep
            The steps this step depends on.
    """

    def __init__(self, stepName, stepShortName):
        """

        Args:
            stepName: str
            stepShortName: str
        """
        self.name = stepName
        self.shortName = stepShortName
        self.dependsOn = []
        self.assignedUsers = []
        self.fileType = None
        self.leads = []
        self.publishedFileTypes = []
        self.secondaryFileTypes = []

class RamShotStep( RamStep ):
    """A step in the production of the shots of the project.

    Attributes:
        dependsOn: list of RamStep.
    """

    def __init__(self, stepName, stepShortName):
        """

        Args:
            stepName: str.
            stepShortName: str.
        """
        self.name = stepName
        self.shortName = stepShortName
        self.dependsOn = []
        self.assignedUsers = []
        self.fileType = None
        self.leads = []
        self.publishedFileTypes = []
        self.secondaryFileTypes = []

class RamItem( RamObject ):
    """Base class for RamAsset and RamShot. An item of the project, either a general item, an asset or a shot.
    """

    def __init__(self, itemName, itemShortName, itemFolder = ""):
        self.name = itemName
        self.shortName = itemShortName
        self.folderPath = itemFolder

    def getCurrentStatus(self, step, resource = ""):
        """

        Args:
            step: RamStep or str
                The step.
            resource: str
                The name of the resource.

        Returns: RamStatus
        """
        if not Ramses.instance:
            raise Exception("Ramses has to be instantiated first.")
        if Ramses.instance.online:
            # TODO ask the client
            return None
        
        # If offline
        currentVersionPath = self.getVersionFilePath(step, resource)
        if currentVersionPath == None:
            print("There was an error getting the latest version or none was found.")
            return None

        currentVersionPath = self.folderPath + '/' + currentVersionPath
        currentVersionPath = Ramses.instance.currentProject.getAbsolutePath(currentVersionPath)

        currentStatus = RamStatus.getFromPath(currentVersionPath)

        return currentStatus

    def getLatestVersion( self, step, resource = "", stateId = 'wip' ):
        """Returns the highest version number for the given state (wip, pub...).

        Args:
            step: RamStep
            resource: str
            state: str

        Returns: int
        """
        if not Ramses.instance:
            raise Exception("Ramses has to be instantiated first.")
 
        # If we're online, ask the client
        if Ramses.instance.online:
            # TODO
            return 0

        # Else check in the folders
        if self.folderPath == '':
            print("The given item has no folderPath.")
            return None

        folderPath = Ramses.instance.currentProject.folderPath + '/' + self.folderPath #Makes it absolute

        if not os.path.isdir( folderPath ):
            print("The given item's folder was not found.\nThis is the path that was checked:\n" + folderPath)
            return None
        if not isinstance(stateId, str):
            raise TypeError("State must be a str")

        stepShortName = ""
        if isinstance(step, str):
            stepShortName = step
        elif isinstance(step, RamStep):
            stepShortName = step.shortName
        else:
            raise TypeError("Step must be a str or an instance of RamStep")

        baseName = os.path.basename(self.folderPath) + '_' + stepShortName #Name without the resource str (added later)
        stepFolderPath = folderPath + '/' + baseName

        if os.path.isdir(stepFolderPath) == False:
            print("The folder for the following step: " + stepShortName + " has not been found.")
            return None
        if os.path.isdir(stepFolderPath + '/ramses_versions') == False:
            print("ramses_versions directory has not been found")
            return None

        foundFiles = os.listdir(stepFolderPath + '/ramses_versions')
        highestVersion = 0

        for foundFile in foundFiles:
            if not os.path.isfile( stepFolderPath + '/ramses_versions/' + foundFile ): #This is in case the user has created folders in ramses_versions
                continue

            decomposedFoundFile = decomposeRamsesFileName(foundFile)

            if decomposedFoundFile == None:
                continue
            if not foundFile.startswith(baseName): #In case other assets have been misplaced here
                continue
            if decomposedFoundFile["resourceStr"] != resource:
                continue
            if decomposedFoundFile["version"] == '':
                continue
            if decomposedFoundFile["state"] != stateId:
                continue
            
            versionInt = int(decomposedFoundFile["version"])
            if versionInt > highestVersion:
                highestVersion = versionInt

        return highestVersion

    def getPublishedFilePaths(self, step, resource = ""):
        """Gets the list of file paths in the publish folder. Paths are relative to the root of the item folder.

        Args:
            step: RamStep or str
                If str, it must be the step shortName (eg. 'RIG' instead of 'RIGGING')
            resource: str
        
        Returns: list of str
        """
        if not Ramses.instance:
            raise Exception("Ramses has to be instantiated first.")

        if isinstance(step, str):
            stepShortName = step
        elif isinstance(step, RamStep):
            stepShortName = step.shortName
        else:
            raise TypeError("Step must be a str or an instance of RamStep")

        if self.folderPath == '':
            print("The given item has no folderPath.")
            return None

        baseName = os.path.basename(self.folderPath)
        folderPath = Ramses.instance.currentProject.folderPath + '/' + self.folderPath

        if not os.path.isdir( folderPath ):
            print("The given item's folder was not found.\nThis is the path that was checked:\n" + folderPath)
            return None
        
        folderPath = folderPath + '/' + baseName + '_' + stepShortName
        if not os.path.isdir( folderPath + '/ramses_publish'):
            print("ramses_publish directory has not been found")
            return None
        
        foundFiles = os.listdir(folderPath + '/ramses_publish')
        foundFilePath = ""
        publishFiles = []

        for foundFile in foundFiles:
            if os.path.isdir(foundFile):
                continue

            blocks = decomposeRamsesFileName(foundFile)

            if blocks == None:
                continue
            if blocks["resourceStr"] != resource:
                continue
            if not foundFile.startswith(baseName):
                continue

            #Building file path relative to root of item folder
            foundFilePath = baseName + '_' + stepShortName + '/ramses_publish/' + foundFile
            publishFiles.append(foundFilePath)
            
        return publishFiles
    
    def getStepHistory(self, step, resource=""): #TODO
        """Gets the history of statuses for a given step and resource

        Args:
            step: RamStep or str
                The step.
            resource: str
                The name of the resource.

        Returns: list of RamStatus
        """
        if not Ramses.instance:
            raise Exception("Ramses has to be instantiated first.")
        if Ramses.instance.online:
            # TODO ask the client
            return []
        
        # If offline
        if isinstance(step, str):
            stepShortName = step
        elif isinstance(step, RamStep):
            stepShortName = step.shortName
        else:
            raise TypeError("Step must be a str or an instance of RamStep")

        if self.folderPath == '':
            print("The given item has no folderPath.")
            return None

        folderPath = Ramses.instance.currentProject.getAbsolutePath(self.folderPath)
        rootName = os.path.basename(folderPath)
        folderPath = folderPath + '/' + rootName + '_' + stepShortName

        if not os.path.isdir(folderPath):
            print("The directory for the given item's step has no been found")

        versionFolderPath = folderPath + '/ramses_versions'
        
        if not os.path.isdir(versionFolderPath):
            print("The ramses_versions directory for the given item's step has not been found")
            return None

        #Lists all files in the ramses_versions dir and checks them one by one. If they match the given file, a RamStatus is created.
        foundFiles = os.listdir(versionFolderPath)
        RamStatusList = []
        rootName = rootName + '_' + stepShortName
        if resource != "":
            rootName = rootName + '_' + resource

        for foundFile in foundFiles:
            foundFilePath = versionFolderPath + '/' + foundFile
            if not os.path.isfile(foundFilePath):
                continue
            if not foundFile.startswith(rootName):
                continue
            fileBlocks = decomposeRamsesFileName(foundFile)
            if fileBlocks == None:
                continue
            print(foundFile)
            RamStatusList.append(RamStatus.getFromPath(foundFilePath))
            RamStatusList.sort(key = getDate)
        return RamStatusList

    def getVersionFilePath(self, step, resource = ""):
        """Latest version file path relative to the item root folder.

        Args:
            step: RamStep
            resource: str
        
        Returns: str
        """
        if not Ramses.instance:
            raise Exception("Ramses has to be instantiated first.")

         # If we're online, ask the client
        if Ramses.instance.online:
            # TODO
            return 0
        
        #Else check in the folders
        #It is basically the same as getLatestVersion. Only difference is that it does not take the stateId into account and returns the path instead of the version number.
        if self.folderPath == '':
            print("The given item has no folderPath.")
            return None

        folderPath = Ramses.instance.currentProject.getAbsolutePath(self.folderPath)

        if not os.path.isdir( folderPath ):
            print("The given item's folder was not found.\nThis is the path that was checked:\n" + folderPath)
            return None

        if isinstance(step, str):
            stepShortName = step
        elif isinstance(step, RamStep):
            stepShortName = step.shortName
        else:
            raise TypeError("Step must be a str or an instance of RamStep")

        baseName = os.path.basename(self.folderPath) + '_' + stepShortName #Name without the resource str (added later)
        stepFolderPath = folderPath + '/' + baseName

        if os.path.isdir(stepFolderPath) == False:
            print("The folder for the following step: " + stepShortName + " has not been found.")
            return None
        if os.path.isdir(stepFolderPath + '/ramses_versions') == False:
            print("ramses_versions directory has not been found")
            return None

        foundFiles = os.listdir(stepFolderPath + '/ramses_versions')
        highestVersion = 0
        highestVersionFileName = ""

        for foundFile in foundFiles:
            if not os.path.isfile( stepFolderPath + '/ramses_versions/' + foundFile ): #This is in case the user has created folders in ramses_versions
                continue

            decomposedFoundFile = decomposeRamsesFileName(foundFile)

            if decomposedFoundFile == None:
                continue
            if not foundFile.startswith(baseName): #In case other assets have been misplaced here
                continue
            if decomposedFoundFile["resourceStr"] != resource:
                continue
            if decomposedFoundFile["version"] == '':
                continue
            
            versionInt = int(decomposedFoundFile["version"])
            if versionInt > highestVersion:
                highestVersion = versionInt
                highestVersionFileName = foundFile
        
        if highestVersionFileName == '':
            print("No version was found")
            return None

        highestVersionFilePath = baseName + "/ramses_versions/" + highestVersionFileName

        return highestVersionFilePath
    
    def getWIPFilePath(self, step, resource = ""):
        """Current wip file path relative to the item root folder.

        Args:
            step: RamStep
                If str, it must be the step shortName (eg. 'RIG' instead of 'RIGGING')
            resource: str
        
        Returns: str
        """
        if self.folderPath == '':
            print("The given item has no folderPath.")
            return None

        if not isinstance(step, RamStep):
            raise TypeError("Step must be an instance of RamStep")
        
        baseName = os.path.basename(self.folderPath) + '_' + step.shortName

        if step.fileType == None:
            raise Exception("The given step has no fileType; cannot build the path towards the working file (missing extension).")

        filePath = baseName + '/' + baseName + '.' + step.fileType.extension

        return filePath

    def isPublished(self, step, resource = ""):
        """Convenience function to check if there are published files in the publish folder.
        Equivalent to len(self.getPublishedFilePaths(step, resource)) > 0

        Args:
            resource: str
        
        Returns: bool
        """
        result = self.getPublishedFilePaths(step, resource)
        if result == None:
            return False
        return len(result) > 0

    def setStepStatus(self, status, step): #TODO
        """Adds a new status in stepStatuses for the given step.

        Args:
            status: RamStatus
            step: RamStep
        """

        # Needs to be online
        pass

class RamShot( RamItem ):
    """A shot.
    """

    def __init__(self, shotName, folderPath = ""):
        if not Ramses.instance:
            raise Exception("Ramses has to be instantiated first.")

        self.shortName = shotName
        self.name = shotName
        if folderPath == "": #Will look for the shot in the folders
            shotGroupName = Ramses.instance.currentProject.shortName + '_S_' + shotName
            rootPath = Ramses.instance.currentProject.folderPath + '/05-SHOTS'

            foundElements = os.listdir(rootPath)

            for element in foundElements:
                if not os.path.isdir(rootPath + '/' + element):
                    continue
                if element == shotGroupName:
                    folderPath = '05-SHOTS/' + shotGroupName
                    break
        self.folderPath = folderPath
    
    @staticmethod
    def getFromPath( folderPath ):
        """Returns a RamItem object built using the given folder path

        Args:
            folderPath: str
                Must point towards the root folder of the shot.
        """
        if not Ramses.instance:
            raise Exception("Ramses has to be instantiated first.")
        if not os.path.isdir(folderPath):
            folderPath = Ramses.instance.currentProject.getAbsolutePath( folderPath )
            if not os.path.isdir(folderPath):
                print("The given folder could not be found")
                return None

        folderName = os.path.basename(folderPath)

        if not isRamsesItemFoldername(folderName):
            print("The given folder does not respect Ramses' naming convention")
            return None
        
        folderBlocks = folderName.split('_')

        if not folderBlocks[1] == 'S':
            print("The given folder does not belong to a shot")
            return None

        shortName = folderBlocks[2]
        shotFolderPath = os.path.relpath(folderPath, Ramses.instance.currentProject.folderPath)

        shot = RamShot( shotName = shortName, folderPath = shotFolderPath )
        return shot

    @staticmethod
    def getShotFolder(filePath):
        """Gets the folder for the shot the file belongs to.

        Args:
            filePath: str
                The path to the file

        Returns: str
            The path to the folder
        """
        if not Ramses.instance:
            raise Exception("Ramses has to be instantiated first.")
        if not os.path.isfile(filePath):
            filePath = Ramses.instance.currentProject.getAbsolutePath(filePath)
            if not os.path.isfile(filePath):
                print("The given file could not be found")
                return None

        folderPath = os.path.dirname(filePath)
        folderName = os.path.basename(folderPath)

        if folderName in ('ramses_publish', 'ramses_versions'):
            folderPath = os.path.dirname(folderPath)
        
        folderPath = os.path.dirname(folderPath) #Goes to the root folder
        folderName = os.path.basename(folderPath)

        if not isRamsesItemFoldername(folderName):
            print("The given item's hierarchy does not respect Ramses' conventions.")
            return None
        
        if not '_S_' in folderName:
            print("The given file does not belong to a shot")
            return None

        return os.path.relpath(folderPath, Ramses.instance.currentProject.folderPath)

class RamAsset( RamItem ):
    """A class representing an asset.
    """

    def __init__(self, assetName, assetShortName, folderPath=""):
        if not Ramses.instance:
            raise Exception("Ramses has to be instantiated first.")
        self.tags = []
        self.name = assetName
        self.shortName = assetShortName

        if folderPath == "": #Will look for the asset in the folders
            assetGroupName = Ramses.instance.currentProject.shortName + '_A_' + assetShortName
            rootPath = Ramses.instance.currentProject.folderPath + '/04-ASSETS'

            foundElements = os.listdir(rootPath)
            foundGroups = []

            for element in foundElements: #Checks everything at the root and adds to foundGroups anything that looks like an asset group (Characters, Props, Sets...), for further testing
                if not os.path.isdir(rootPath + '/' + element):
                    continue
                if element == assetGroupName:
                    folderPath = '04-ASSETS/' + assetGroupName
                    break
                if isRamsesItemFoldername(element):
                    continue

                foundGroups.append(element)

            for group in foundGroups: #Further testing
                if folderPath != "":
                    break

                foundAssets = os.listdir ( rootPath + '/' + group )
                
                for asset in foundAssets:
                    if asset == assetGroupName:
                        folderPath = '04-ASSETS/' + group + '/' + assetGroupName
                        break
        self.folderPath = folderPath

    @staticmethod
    def getAssetFolder(filePath):
        """Gets the folder for the asset the file belongs to.

        Args:
            filePath: str
                The path to the file

        Returns: str
            The path to the folder
        """
        if not Ramses.instance:
            raise Exception("Ramses has to be instantiated first.")
        if not os.path.isfile(filePath):
            filePath = Ramses.instance.currentProject.getAbsolutePath(filePath)
            if not os.path.isfile(filePath):
                print("The given file could not be found")
                return None

        folderPath = os.path.dirname(filePath)
        folderName = os.path.basename(folderPath)

        if folderName in ('ramses_publish', 'ramses_versions'):
            folderPath = os.path.dirname(folderPath)
        
        folderPath = os.path.dirname(folderPath) #Goes to the root folder
        folderName = os.path.basename(folderPath)

        if not isRamsesItemFoldername(folderName):
            print("The given item's hierarchy does not respect Ramses' conventions.")
            return None
        
        if not '_A_' in folderName:
            print("The given file does not belong to an asset")
            return None

        return os.path.relpath(folderPath, Ramses.instance.currentProject.folderPath)
     
    @staticmethod
    def getFromPath( folderPath ):
        """Returns a RamAsset instance built using the given file path.

        Args:
            folderPath: str
                Must point towards the root folder of the shot.
        """
        if not Ramses.instance:
            raise Exception("Ramses has to be instantiated first.")
        if not os.path.isdir(folderPath):
            folderPath = Ramses.instance.currentProject.getAbsolutePath( folderPath )
            if not os.path.isdir(folderPath):
                print("The given folder could not be found")
                return None

        folderName = os.path.basename(folderPath)

        if not isRamsesItemFoldername(folderName):
            print("The given folder does not respect Ramses' naming convention")
            return None
        
        folderBlocks = folderName.split('_')

        if not folderBlocks[1] == 'A':
            print("The given folder does not belong to an asset")
            return None

        shortName = folderBlocks[2]
        assetFolderPath = os.path.relpath(folderPath, Ramses.instance.currentProject.folderPath)

        asset = RamAsset( assetName = shortName, assetShortName = shortName, folderPath = assetFolderPath)
        return asset

    def getTags(self): #TODO
        """Some tags describing the asset.

        Returns: list of str
        """
        #TODO
        pass

    def getGroup(self):
        """The group containing this asset.

        Returns: str
        """
        if not Ramses.instance:
            raise Exception("Ramses has to be instantiated first.")
        # If we're online, ask the client
        if Ramses.instance.online:
            #TODO
            return None

        if self.folderPath == '':
            print("The given item has no folderPath.")
            return None
        if not os.path.isdir( self.folderPath ):
            print("The given item's folder was not found.\nThis is the path that was checked:\n" + self.folderPath)
            return None
        
        parentFolder = os.path.dirname(self.folderPath)
        parentFolderName = os.path.basename(parentFolder)

        if parentFolderName != '04-ASSETS':
            return parentFolderName
            
        return None

class RamState( RamObject ):
    """Represents a state used in a status, like "CHK" (To be checked), "OK" (ok), "TODO", etc.

    Attributes:
        completionRatio: float.
            The ratio of completion of this status
    """

    def __init__(self, stateName, stateShortName, completion=0.0):
        """

        Args:
            stateName: str.
            stateShortName: str.
            completion: float.
        """
        self.name = stateName
        self.shortName = stateShortName
        self.completionRatio = completion

class RamStatus():
    """A state associated to a comment, the user who changed the state, etc.

    Attributes:
        comment: string
            A user comment.
        completionRatio: float
            The ratio of completion of this status.
        date: datetime
            The date at which this status was created.
        state: RamState
            The corresponding state.
        user: RamUser
            The user who created this status.
        version: int
            The version of the corresponding working file.
    """

    def __init__(self, state, user=None, comment = "", version=0, stateDate=None):
        """

        Args:
            state: RamState.
            user: RamUser.
            comment: str.
        """
        if not Ramses.instance:
            raise Exception("Ramses has to be instantiated first.")

        self.state = state
        self.completionRatio = state.completionRatio
        self.comment = comment

        # Get J. Doe
        if not user:
            user = Ramses.instance.getUser()
        self.user = user

        if not stateDate:
            stateDate = datetime.datetime(year = 2020, month = 1, day = 1)

        self.date = stateDate
        self.version = version
    
    @staticmethod
    def getFromPath(filePath):
        """Returns a RamStatus instance built using the given file path.

        Args:
            filePath: str
        """
        if not Ramses.instance:
            raise Exception("Ramses has to be instantiated first.")
        if not isinstance(filePath, str):
            raise TypeError("File path needs to be a str")
        if not os.path.isfile(filePath):
            filePath = Ramses.instance.currentProject.getAbsolutePath( filePath )
            if not os.path.isfile(filePath):
                print("The given file could not be found")
                return None

        baseName = os.path.basename(filePath)
        blocks = decomposeRamsesFileName(baseName)
    
        if blocks == None:
            print("The given file does not respect Ramses' naming convention")
            return None

        version = 0
        stateId = 'WIP'

        if blocks["version"] != '': #The file is already a version: gets the version info directly from it
            version = int(blocks["version"])
            if blocks["state"] != '':
                stateId = blocks["state"]
                stateId = stateId.upper()

        elif blocks["ramType"] in ('A', 'S'): #Builds a RamItem out of the given file, to then try to get its latest version info
            if blocks["ramType"] == 'A':
                item = RamAsset.getFromPath(filePath)
            else:
                item = RamShot.getFromPath(filePath)

            latestVersionFilePath = item.getVersionFilePath(step = blocks["ramStep"], resource = blocks["resourceStr"])

            if not latestVersionFilePath in (None, 0): #If it has at least one version
                latestVersionFileName = os.path.basename(latestVersionFilePath)
                latestVersionBlocks = decomposeRamsesFileName( latestVersionFileName )

                version = int(latestVersionBlocks["version"])
                if latestVersionBlocks["state"] != '':
                    stateId = latestVersionBlocks["state"]
                    stateId = stateId.upper()

        state = Ramses.instance.getState(stateId)

        dateTimeStamp = os.path.getmtime(filePath)
        dateTime = datetime.fromtimestamp( dateTimeStamp )

        status = RamStatus(state, None, "", version, dateTime)

        return status

# Initialization
Ramses.instance = None