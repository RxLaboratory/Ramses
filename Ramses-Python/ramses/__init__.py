'''
- getLatestPubVersion, getLatestVersion, isPublished

- class RamShot, getFromPath:
    TODO: complete attribute: stepStatuses, published
'''

import os
import re

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
version_prefixes = ['v', 'wip', 'pub']
regexStr = ''

def __init__():
    pass

def getVersionRegEx():
    regexStr = ''
    for prefix in version_prefixes[0:-1]:
        regexStr = regexStr + prefix + '|'
    regexStr = regexStr + version_prefixes[-1]
    regexStr = '^(' + regexStr + ')?(\\d+)$'

    regex = re.compile(regexStr, re.IGNORECASE)
    return regex

def getRamsesNameRegEx():
    regexStr = ''
    for prefix in version_prefixes[0:-1]:
        regexStr = regexStr + prefix + '|'
    regexStr = regexStr + version_prefixes[-1]

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

def fixResourceStr( resourceStr ):
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
        print("The given filename does not match Ramses' naming convention.")
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

        self.clientPort = port
        self.autoConnect = True
        self._currentProject = None
        self._currentUser = None
        self._online = False

        if connect:
            self._online = self.launchClient( True )

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
        """
        #TODO
        pass

    def connect(self):
        """ Checks server or client availability and initiates the connection. Returns success.
        """
        #TODO
        pass

    def disconnect(self):
        """Gets back to offline mode.
        """
        #TODO
        pass

    def getAlternativeFolderPaths(self):
        """A list of alternative absolute paths to the main Ramses folder.
        
        Missing files will be looked for in these paths (and copied to the main path if available), and they will be used if the main path is not available.
        """
        #TODO
        pass

    def getSteps(self, typeOrCat = "ALL"):
        """The list of available steps.

        Use typeOrCat to filter the results.
        One of: ALL, ASSET, SHOT, PRE-PROD, PROD, POST-PROD.
        PROD represents a combination of SHOT and ASSET
        """
        #TODO
        pass

    def getUsers(self):
        """The list of available users.
        """
        #TODO
        pass

    def folderPath(self):
        """The absolute path to main Ramses folder, containing projects by default, config files, user folders, admin files…
        """
        #TODO
        pass

    def launchClient(self, connect = True):
        """Launches the Ramses client and tries to connect to it. Returns success.
        """
        #TODO
        return False

    def login(self, userPassword, user = currentUser):
        """Logs the user in. Launches and connect to the client if necessary. Returns success.
        """
        #TODO
        pass
    
    def logout(self):
        """Logs the user out.
        """
        #TODO
        pass

    def projects(self):
        """The list of available projects. Returns list of RamProject.
        """
        #TODO
        pass

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

    def __init__(self, userName, userShortName, userFolderPath, role = 'STANDARD'):
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
        """
        #TODO
        pass
    
    def logout(self):
        """Logs the user out.
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
        self.exectuableFilePath = execFilePath
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

    def getAssets(self, groupName = ""):
        """If groupName is an empty string, returns all assets.
        """
        #TODO
        pass

    def getAssetGroups(self):
        #TODO
        pass

    def getShots(self, filter = "*"):
        """A filter to be applied to the name of the shots, using “*” as a wildcard.
        """
        #TODO
        pass

    def getSteps(self, typeOrCat = "ALL"):
        """Use typeOrCat to filter the results.

        One of: ALL, ASSET, SHOT, PRE-PROD, PROD, POST-PROD.
        PROD represents a combination of SHOT and ASSET
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

class RamItem( RamObject ):
    """Base class for RamAsset and RamShot. An item of the project, either a general item, an asset or a shot.

    Attributes:
        published: bool
        stepStatuses: list of RamStepStatus
    """

    def __init__(self, itemName, itemShortName, itemFolder = ""):
        self.name = itemName
        self.shortName = itemShortName
        self.folderPath = itemFolder
        self.published = False
        self.stepStatuses = []
    
    def getLatestPubVersion(self, step, resource = ""): #TODO
        """Returns the highest version number for published item

        Args:
            step: RamStep
            resource: str
        """

        #TODO
        pass

        '''fullPath = ramsesFileName

        folderPath = os.path.dirname(fullPath)
        fileName = os.path.basename(fullPath)
        
        if os.path.isdir(folderPath + '/ramses_versions') == False:
            print("ramses_versions directory has not been found")
            return None
        
        ramsesName = fileName.split('.')[0]
        extension = fileName.split('.')[1]

        foundElements = os.listdir(folderPath + '/ramses_versions')
        highestPubVersion = 0

        for element in foundElements:
            if os.path.isfile( folderPath + '/ramses_versions/' + element ) == True : #in case the user has created folders in ramses_versions
                if element.lower().endswith('.' + extension):
                    if element.startswith(ramsesName + '_pub'):
                        pubVersion = getFileVersion(element)[1]
                        if pubVersion > highestPubVersion:
                            highestPubVersion = pubVersion

        return highestPubVersion'''
    
    def getLatestVersion( self, step, resource = "" ): #TODO
        """Returns the highest version number.

        Args:
            step: RamStep
            resource: str
        """

        if self.folderPath == '':
            print("The given item has no folderPath.")
            return None
        if not os.path.isdir( self.folderPath ):
            print("The given item's folder was not found.\nThis is the path that was checked:\n" + str(self.folderPath))
            return None
        if not isinstance(step, RamStep):
            print("Step arg needs to be a RamStep")
            return None

        baseName = os.path.basename(self.folderPath) + '_' + step.shortName #Name without the resource str (added later)
        stepFolderPath = str(self.folderPath) + '/' + baseName
        
        if os.path.isdir(stepFolderPath) == False:
            print("The folder for the following step: " + str(step.shortName) + " has not been found.")
            return None
        if os.path.isdir(stepFolderPath + '/ramses_versions') == False:
            print("ramses_versions directory has not been found")
            return None

        foundFiles = os.listdir(stepFolderPath + '/ramses_versions')
        highestFileVersion = 0

        for foundFile in foundFiles:
            if os.path.isfile( stepFolderPath + '/ramses_versions/' + foundFile ) == False: #This is in case the user has created folders in ramses_versions
                continue

            decomposedFoundFile = decomposeRamsesFileName(foundFile)

            if decomposedFoundFile == None:
                continue
            if decomposedFoundFile["resourceStr"] != resource:
                continue
            if decomposedFoundFile["version"] == '': #See next condition: int('') would raise an error
                continue
            
            if int(decomposedFoundFile["version"]) > highestFileVersion:
                highestFileVersion = int(decomposedFoundFile["version"])
        return highestFileVersion

    def getPublishedFilePaths(self, step, resource = ""):
        """Gets the list of file paths in the publish folder. Paths are relative to the root of the item folder.

        Args:
            step: RamStep
            resource: str
        """
        #TODO
        pass

    def getVersionFilePath(self, step, resource = ""):
        """Latest version file path relative to the item root folder.

        Args:
            step: RamStep
            resource: str
        """
        #TODO
        pass
    
    def getWIPFilePath(self, step, resource = ""):
        """Current wip file path relative to the item root folder.

        Args:
            step: RamStep
            resource: str
        """
        #TODO
        pass

    def setStepStatus(self, status, step):
        """Adds a new status in stepStatuses for the given step.

        Args:
            status: RamStatus
            step: RamStep
        """
        #TODO
        pass

class RamShot( RamItem ): #TODO: getFromPath: complete attrs (published, stepStatuses)
    """A shot.
    """

    def __init__(self, shotName):
        self.shortName = shotName
        self.name = shotName
    
    @staticmethod
    def getFromPath( filePath ): #TODO: complete attrs (published, stepStatuses)
        """Returns a RamItem object built using the given file path

        Args:
            filePath: str.
                Must point towards a file in a step subfolder, such as PROJ_A_ISOLDE\\PROJ_A_ISOLDE_RIG\\PROJ_A_ISOLDE_RIG.blend
                The file also needs to respect Ramses' naming convention
        """

        if os.path.isfile(filePath) == False:
            print("The given file could not be found")
            return None

        folderPath = os.path.dirname(filePath) #From PROJ_A_ISOLDE\\PROJ_A_ISOLDE_RIG\\PROJ_A_ISOLDE_RIG.blend we go to PROJ_A_ISOLDE\\PROJ_A_ISOLDE_RIG
        folderPath = os.path.dirname(folderPath) #And then we go to PROJ_A_ISOLDE

        fileName = os.path.basename(filePath)

        if isRamsesName(fileName) == False:
            print("The given file does not match Ramses' naming convention or has no extension")
            return None
        
        blocks = decomposeRamsesFileName(fileName)

        if blocks["ramType"] != 'S':
            print("The given filepath does not point towards a shot")
            return None

        #Attrs from inheritances: published (bool), stepStatuses (list of RamStatus), name (str), shortName (str), folderPath (str)
        shortName = blocks["objectShortName"]
        shot = RamShot( shotName = shortName)
        shot.name = blocks["objectShortName"]
        shot.folderPath = folderPath
        #TODO: published
        #TODO: stepStatuses

        return shot

class RamAsset( RamItem ):
    """A class representing an asset.
    """

    def __init__(self, assetName, assetShortName):
        self.tags = []
        self.name = assetName
        self.shortName = assetShortName
    
    @staticmethod
    def getFromPath(filePath):
        """Returns a RamAsset instance built using the given file path.
        """
        #TODO
        pass

    def getTags(self):
        """Some tags describing the asset. Returns list of str.
        """
        #TODO
        pass

    def getGroup(self):
        """The group containing this asset.
        """
        #TODO
        pass

class RamState( RamObject ):
    """Represents a state used in a status, like "CHK" (To be checked), "OK" (ok), "TODO", etc.

    Attributes:
        completionRatio: float.
            The ratio of completion of this status
    """

    def __init__(self, stateName, stateShortName, completion):
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

    def __init__(self, state, user, comment = ""):
        """

        Args:
            state: RamState.
            user: RamUser.
            comment: str.
        """
        self.state = state
        self.completion = 0.0
        self.comment = comment
        self.user = user
        self.date = None
        self.version = 0
    
    @staticmethod
    def getFromPath(filePath):
        """Returns a RamStatus instance built using the given file path.

        Args:
            filePath: str
        """
        #TODO
        pass

class RamStepStatus():
    """A history of RamStatus for a given step.
    """

    def __init__(self):
        pass

    @staticmethod
    def getFromPath(filePath):
        """Returns a RamStepStatus instance built using the given file path.

        Args:
            filePath: str
        """
        #TODO
        pass

    def getHistory(self):
        #TODO
        pass

    def getStatus(self, status):
        """Adds a new status to the history.

        Args:
            status: RamStatus
        """
        #TODO
        pass

testNames = [
    'PROJ_A_ISOLDE_RIG.blend',
    'PROJ_A_ISOLDE_RIG_resource.blend',
    'PROJ_A_ISOLDE_RIG_pub9.blend',
    'PROJ_A_ISOLDE_MOD_resource_wip10.tar.gz',
    'PROJ_G_SCRIPT_resource_pub009.tar.gz',
    'PROJ_G_SCRIPT_pub010.tar.gz',
    'PROJ_G_SCRIPT_0010.tar.gz',
    'PROJ_G_SCRIPT_0002.tar.gz',
    'PROJ_G_SCRIPT.tar.gz',
]

animStep = RamStep(stepName = "animation", stepShortName = "ANIM")

ramShot = RamShot( '001' )
ramShot.folderPath = '/home/rainbox/RAINBOX/DEV_SRC/Ramses/Project-Tree-Example/Project01/05-SHOTS/Projet01_S_001'

print(ramShot.getLatestVersion(animStep, resource = ''))