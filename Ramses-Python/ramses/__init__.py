'''
- getLatestPubVersion, getLatestVersion, isPublished:
    TODO: make them actual methods of RamItem

- class RamShot, getFromPath:
    TODO: complete attribute: stepStatuses, published

- added function isRamsesName and getRamsesNameRegEx.
    RegEx has limitations:
        projectShortName shouldn't be longer than 15 characters;
        objectShortName shouldn't be longer than 50 characters;
        ramStep shouldn't be longer than 15 characters;
        resourceStr shouldn't be longer than 50 characters;
        version (prefix + number) shouldn't be longer than 15 characters;
        extension shouldn't be longer than 50 characters.
    TODO: see if these limitations are enough.

- decomposeRamsesFileName:
    Now it returns a lovely dictionary.
    Remove all get functions? getStepId, getResourceStr, ...
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
    regexStr = '^(' + regexStr + ')(\\d+)$'

    regex = re.compile(regexStr, re.IGNORECASE)
    return regex

def getRamsesNameRegEx(): #TODO update regex
    regexStr = ''
    for prefix in version_prefixes[0:-1]:
        regexStr = regexStr + prefix + '|'
    regexStr = regexStr + version_prefixes[-1]
    #OLD REGEX
    regexStr = '^([a-z]{1,15}[_])(([AS][_]([a-z]|[0-9]){1,50})|G)[_][a-z]{1,15}([_]([a-z]|[0-9]|[ -]){1,50})?([_](?:' + regexStr + ')[0-9]{1,15})?([.]([.]|[a-z]){1,50})$'
    
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
    splitRamsesFileName = ramsesFileName.split('_')
    splitExtension = splitRamsesFileName[-1].split('.')

    lastElement = splitRamsesFileName[-1] #Working on last element, which has the extension stuck to it
    splitRamsesFileName.pop(-1)
    lastElement = lastElement.split('.')[0]
    splitRamsesFileName.append(lastElement)

    objectShortName = ''
    ramStep = ''
    optionalBlocksIndex = 4 #Optional blocks: resourceStr, versionStr. 4 for Assets and Shots, 3 for General

    if splitRamsesFileName[1] in ('A', 'S'):
        objectShortName = splitRamsesFileName[2]
        ramStep = splitRamsesFileName[3]
        optionalBlocksIndex = 4
    else:
        ramStep = splitRamsesFileName[2]
        optionalBlocksIndex = 3

    resourceStr = ''
    versionStr = ''
        
    if len(splitRamsesFileName) > optionalBlocksIndex:  #It is long enough to have a comment and/or a version
        if isVersion( splitRamsesFileName[optionalBlocksIndex] ) == True: #It has a version and no resourceStr, since the resourceStr always comes before the versionStr.
            versionStr = splitRamsesFileName[optionalBlocksIndex]
        else:
            resourceStr = splitRamsesFileName[optionalBlocksIndex]

            if splitRamsesFileName[optionalBlocksIndex] != splitRamsesFileName[-1]:
                versionStr = splitRamsesFileName[-1]

    state = ''
    version = -1

    if versionStr != '':
        match = re.findall( getVersionRegEx(), versionStr )
        state = match[0][0]
        version = int(match[0][1])

    extension = ''
    if len(splitExtension) > 1: # If more than one item, all items are part of the extension except the first one
        extension = '.'.join(splitExtension[1:])

    blocks = {
        "projectID": splitRamsesFileName[0],
        "ramType": splitRamsesFileName[1],
        "objectShortName": objectShortName,
        "ramStep": ramStep,
        "resourceStr": resourceStr,
        "state": state,
        "version": version,
        "extension": extension,
    }

    return blocks

def getFileProjectId( ramsesFileName ):
    fileProjectId = ramsesFileName.split('_')[0]
    return fileProjectId

def getFileRamTypeId( ramsesFileName ):
    fileRamTypeId = ramsesFileName.split('_')[1]
    return fileRamTypeId

def getFileObjectId( ramsesFileName ):
    if getFileRamTypeId( ramsesFileName ) == 'G':
        print("File is not an object")
        return None
    fileObjectId = ramsesFileName.split('_')[2]
    return fileObjectId

def getFileStepId( ramsesFileName ):
    if getFileRamTypeId( ramsesFileName ) == 'G':
        fileStepId = ramsesFileName.split('_')[2]
    else:
        fileStepId = ramsesFileName.split('_')[3]
    return fileStepId

def getFileResourceStr( ramsesFileName ):
    blocks = decomposeRamsesFileName( ramsesFileName )[0]

    if blocks[1] == 'G' and len(blocks) > 3 or blocks[1] in ('A', 'S') and len(blocks) > 4: #is long enough to have a resourceStr and/or a version
        if blocks[1] == 'G' and isVersion(blocks[3]) == False :
            return blocks[3]
        elif blocks[1] in ('A', 'S') and isVersion(blocks[4]) == False :
            return blocks[4]

    return None

def getFileVersion( ramsesFileName ):
    blocks = decomposeRamsesFileName( ramsesFileName )[0]
    fileVersion = 0
    state = ''

    if blocks[1] == 'G' and len(blocks) > 3 or blocks[1] in ('A', 'S') and len(blocks) > 4: #is long enough to have a resourceStr and/or a version
        if isVersion(blocks[-1]):
            match = re.findall( getVersionRegEx(), blocks[-1] )
            state = match[0][0]
            fileVersion = int(match[0][1])
            return state, fileVersion

    return None

def getFileExtension( ramsesFileName ):
    fileExtension = decomposeRamsesFileName( ramsesFileName )[-1]
    return fileExtension

def incrementRamsesFileName( ramsesFileName ):
    separatedBlocks, extension = decomposeRamsesFileName( ramsesFileName )
    state, fileVersion = getFileVersion( ramsesFileName )

    if fileVersion == None:
        fullBlocks = ramsesFileName.split('.')[0]
        ramsesFileName = fullBlocks + '_wip001.' + extension
        return ramsesFileName

    fileVersion = fileVersion + 1
    ramsesFileName = ''

    for block in separatedBlocks[0:-1]: #rebuilds name up to the file version
        ramsesFileName = ramsesFileName + block + '_'
    
    ramsesFileName = ramsesFileName + state
    
    if fileVersion < 10:
        ramsesFileName = ramsesFileName + '00' + str(fileVersion)
    elif fileVersion < 100:
        ramsesFileName = ramsesFileName + '0' + str(fileVersion)
    else:
        ramsesFileName = ramsesFileName + str(fileVersion)
    
    if extension != None:
        ramsesFileName = ramsesFileName + '.' + extension

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
    
    def alternativeFolderPaths(self):
        """A list of alternative absolute paths to the main Ramses folder.
        
        Missing files will be looked for in these paths (and copied to the main path if available), and they will be used if the main path is not available.
        Returns a str list.
        """
        #TODO
        pass

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

    def steps(self):
        """The list of available steps.
        """
        #TODO
        pass

    def users(self):
        """The list of available users.
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
        role: enumerated value.
            'ADMIN', 'LEAD', or 'STANDARD'
        loggedIn: bool, readonly.
            The user has been correctly logged in
    """

    def __init__(self, userName, userShortName, userFolderPath, role = 'STANDARD'):
        """

        Args:
            userName: str.
            userShortName: str.
            useFolderPath: str.
            role: enumerated.
        """

        self.role = role
        self.loggedIn = True
        self.name = userName
        self.shortName = userShortName
        self.folderPath = userFolderPath
    
    def login(self):
        """Logs the user in. Returns success."""

        #TODO
        pass
    
    def logout(self):
        """Logs the user out."""

        #TODO
        pass

class RamApplication( RamObject ): #TODO ONCE PULLED
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
    """

    Attributes:
    steps: list of RamStep.
    shots: list of RamShot.
    assets: list of RamAsset.
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
        self.steps = []
        self.shots = []
        self.assets = []

    def addStep(self, step):
        """

        Args:
            step: RamStep.
        """

        #TODO
        pass

    def addShot(self, shot):
        """

        Args:
            shot: RamShot.
        """

        #TODO
        pass

    def createShot(self, shotName):
        """

        Args:
            shotName: str.
        """

        #TODO
        pass

    def createShots(self, range, prefix = "", suffix = ""):
        """

        Args:
            range: int list.
        """

        #TODO
        pass

    def createStep(self, stepName, stepShortName):
        """Returns RamStep.

        Args:
            stepName: str.
            stepShortName: str.
        """

        #TODO
        pass

    def addAsset(self, asset):
        """

        Args:
            asset: RamAsset
        """

        #TODO
        pass

class RamStep( RamObject ):
    """

    Attributes:
        fileType: RamFileType.
            The main file type used for working on this step
        secondaryFileTypes: list of RamFileType
            Other file types which may be used when working on this step
        publishFileTypes: list of RamFileType.
            The file types published by this step
        assignedUsers: list of RamUser.
        leads: list of RamUser.
    """
    def __init__(self, stepName, stepShortName):
        """

        Args:
            stepName: str.
            stepShortName: str.
        """
        self.fileType = None
        self.secondaryFileTypes = []
        self.publishedFileTypes = []
        self.assignedUsers = []
        self.leads = []
        self.name = stepName
        self.shortName = stepShortName

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
    """

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
        
        '''self.folderPath : folder général. PROJ A ISOLDE
        ramStep : get the step. go in subfolder. PROJ A ISOLDE MOD
        resource : if it is a resource. helps in building the name.
        extension: is it needed? anyway, can be found in RamStep.fileTypes'''

        if self.folderPath == '':
            print("The given item has no folderPath.")
            return None
        if os.path.isdir( self.folderPath ) == False:
            print("The given item's folder was not found.\nThis is the path that was checked:\n" + str(self.folderPath))
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
        if resource == '':
            #TODO
            pass

        for foundFile in foundFiles:
            if os.path.isfile( stepFolderPath + '/ramses_versions/' + foundFile ) == False: #This is in case the user has created folders in ramses_versions
                continue
                
        '''ramsesFileName = "aaaa.heck"
        fullPath = ramsesFileName

        folderPath = os.path.dirname(fullPath)
        fileName = os.path.basename(fullPath)

        ramsesName = fileName.split('.')[0]
        extension = fileName.split('.')[1]

        foundElements = os.listdir(folderPath + '/ramses_versions')
        highestFileVersion = 0

        for element in foundElements:
            if os.path.isfile( folderPath + '/ramses_versions/' + element ) == True : #in case the user has created folders in ramses_versions
                if element.lower().endswith('.' + extension):
                    if element.startswith(ramsesName):
                        fileVersion = getFileVersion(element)[1]
                        if fileVersion != None:
                            if fileVersion > highestFileVersion:
                                highestFileVersion = fileVersion'''

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

class RamShot( RamItem ):
    """

    Attributes:
        assets: list of RamAsset.
            The assets used in this shot
    """

    def __init__(self, shotName):
        self.assets = []
        self.shortName = shotName
        self.name = shotName
    
    @staticmethod
    def getFromPath( filePath ):
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
        
        blocks = decomposeRamsesFileName( fileName )[0]

        if blocks[1] != 'S':
            print("The given filepath does not point towards a shot")
            return None

        #Attrs from inheritances: published (bool), stepStatuses (list of RamStatus), name (str), shortName (str), folderPath (str)
        shortName = blocks[2]
        shot = RamShot( shotName = shortName)
        shot.folderPath = folderPath
        #TODO: isPublished
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

class RamState( RamObject ):
    """Represents a state used in a status, like CHK (to be checked), OK (ok), TODO, etc.

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
        state: RamState.
        completionRatio: float.
            The ratio of completion of this status
        comment: string.
        user: RamUser.
        date: datetime.
            The date at which this status was created
        version: int.
            The version of the corresponding working file
        published: bool
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
        self.published = False

class RamStepStatus():
    """A history of RamStatus for a given step.

    Attributes:
        history: list of RamStatus.
    """

    def __init__(self):
        self.history = []

'''
rigStep = RamStep(stepName = "animation", stepShortName = "ANIM")

ramShot = RamShot( '001' )
ramShot.folderPath = '/home/rainbox/RAINBOX/DEV_SRC/Ramses/Project-Tree-Example/PROJ/PROJ_G_SHOTS/PROJ_S_001'

ramShot.getLatestVersion( ramStep = rigStep , resourceStr = 'crowd')
'''

tests = [
    'PROJ_A_ISOLDE_RIG.blend',
    'PROJ_A_ISOLDE_RIG_resource.blend',
    'PROJ_A_ISOLDE_RIG_pub02.blend',
    'PROJ_A_ISOLDE_MOD_resource_wip010.tar.gz',
    'PROJ_G_SCRIPT_resource_pub010.tar.gz',
    'PROJ_G_SCRIPT_pub010.tar.gz',
    'PROJ_G_SCRIPT.tar.gz'
]

for test in tests:
    print('-------------------\nTest start for: ' + test + '\n')
    print(decomposeRamsesFileName(test))
    print('\n')