'''
WIP:
- getLatestPubVersion, getLatestVersion, isPublished:
    Work, but assume the given argument is the full path towards the file.
- class RamShot, getFromPath:
    TODO: check if the argument is valid (is a path, points towards a file that respects ramses' naming convention)
    TODO: check if there already exists a RamShot with this name and that pathFolder
    TODO: complete attributes: stepStatuses, published

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

def isVersion(v):
    if re.match(getVersionRegEx(), v): return True
    return False

def fixComment( comment ):
    fixedComment = ''
    for char in comment:
        if char in forbiddenCharacters:
            fixedComment = fixedComment + forbiddenCharacters[char]
        else:
            fixedComment = fixedComment + char
    return fixedComment

def buildRamsesFileName( project , step , ext , ramType = 'G' , objectShortName = '' , comment = "" , version = -1 , version_prefixe = 'wip' ):
    #PROJECT_A_OBJECT_STEP_comment_wip012.extension
    #PROJECT_G_STEP_comment_wip012.extension

    comment = fixComment( comment )
    ramsesFileName = project + '_' + ramType

    if ramType in ('A', 'S'):
        ramsesFileName = ramsesFileName + '_' + objectShortName

    ramsesFileName = ramsesFileName + '_' + step

    if comment != '':
        ramsesFileName = ramsesFileName + '_' + comment

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
    splitramsesFileName = ramsesFileName.split('_')
    splitExtension = splitramsesFileName[-1].split('.')

    blocks = splitramsesFileName[0:-1]
    blocks.append(splitExtension[0])
    extension = ''

    if len(splitExtension) > 1: # If more than one item, all items are part of the extension except the first one
        extension = '.'.join(splitExtension[1:])
    else:
        extension = None

    return blocks, extension

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

def getFileComment( ramsesFileName ):
    blocks = decomposeRamsesFileName( ramsesFileName )[0]

    if blocks[1] == 'G' and len(blocks) > 3 or blocks[1] in ('A', 'S') and len(blocks) > 4: #is long enough to have a comment and/or a version
        if blocks[1] == 'G' and isVersion(blocks[3]) == False :
            return blocks[3]
        elif blocks[1] in ('A', 'S') and isVersion(blocks[4]) == False :
            return blocks[4]

    return None

def getFileVersion( ramsesFileName ):
    blocks = decomposeRamsesFileName( ramsesFileName )[0]
    fileVersion = 0
    state = ''

    if blocks[1] == 'G' and len(blocks) > 3 or blocks[1] in ('A', 'S') and len(blocks) > 4: #is long enough to have a comment and/or a version
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

def getLatestPubVersion( ramsesFileName ): #TODO: get filepath from RamAsset/RamShot
    """Returns int. Highest version among all the published files.
    """
    #For now, assumes the filename includes the whole path
    #From the ramsesFileName, try to find a corresponding RamAsset/RamShot and try to get the folderPath from it?
    
    fullPath = ramsesFileName

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

    return highestPubVersion

def getLatestVersion( ramsesFileName ): #TODO: get filepath from RamAsset/RamShot
    """Returns int. Highest version among all the version files.
    """
    #For now, assumes the filename includes the whole path
    #From the ramsesFileName, try to find a corresponding RamAsset/RamShot and try to get the folderPath from it?
    
    fullPath = ramsesFileName

    folderPath = os.path.dirname(fullPath)
    fileName = os.path.basename(fullPath)

    if os.path.isdir(folderPath + '/ramses_versions') == False:
        print("ramses_versions directory has not been found")
        return None

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
                            highestFileVersion = fileVersion

    return highestFileVersion

def isPublished( ramsesFileName ): #TODO: get filepath from RamAsset/RamShot
    """Returns bool. True if last version is a pub.
    """
    #For now, assumes the filename includes the whole path
    #From the ramsesFileName, try to find a corresponding RamAsset/RamShot and try to get the folderPath from it?

    fullPath = ramsesFileName

    latestVersion = getLatestVersion(fullPath)

    if latestVersion == None:
        print("ramses_versions directory has not been found")
        return False
    if latestVersion == 0:
        print("No version has been found")
        return False
    
    latestPubVersion = getLatestPubVersion(fullPath)
    if latestVersion <= latestPubVersion:
        return True
    
    return False

class Ramses():
    """The main class, instantiated during init.

    Attributes:
        localPort: int.
            The port used to connect to the client if it's an add-on, or the port to listen to if this is the client
        serverUrl: str.
            The url to the php server
        useSSL: bool.
            Wether to use SSL to connect to the php server
        online: bool, read only.
            For the client app, true when connected to the network and logges in to the php server
            For addons, true when client app is available and responding
        user: RamUser.
            The user currently using Ramses
        project: RamProject.
            The current project
        projects: list of RamProject.
            The projects
        users: list of RamUser.
            The users
        steps: list of RamStep.
            The steps
        folderPath: str.
            The absolute path to main Ramses folder, containing projects by default, config files, user folders, admin files...
        alternateFolderPaths: str list.
            A list of alternate absolute paths to the main Ramses folder.
            Missing files will be looked for in these paths (and copied to the main path if available), and they will be used if the main path is not available.
        backupFolderPath: str.
            A copy of the main folder where all files are stored.     
    """

    def __init__(self, port = 1818, url = "", ssl = True, connect = True):
        """

        Args:
            port: int.
            url: str.
            ssl: bool.
            connect: bool.
        """

        self.localPort = port
        self.serverUrl = url
        self.useSSL = ssl
        self.online = False
        self.user = None
        self.project = None
        self.projects = []
        self.users = []
        self.folderPath = ""
        self.alternateFolderPaths = []
        self.backupFolderPath = ""

        if connect:
            self.connect() 
    
    def login(self):
        """Logs the user in. Returns success."""

        #TODO
        pass
    
    def logout(self):
        """Logs the user out."""

        #TODO
        pass

    def connect(self):
        """Checks server or client availability and initiates the connection. Returns success."""

        #TODO
        pass

    def disconnect(self):
        """Gets back to offline mode."""

        #TODO
        pass

    def createProject(self, projectName, projectShortName, folder = ""):
        """Returns RamProject.

        Args:
            projectName: str.
            projectShortName: str
            folder: str
        """

        #TODO
        pass

    def createUser(self, userName, userShortName, folder = ""):
        """Returns RamUser.

        Args:
            userName: str
            userShortName: str
            folder: str
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

class RamObject():
    """The base class for all Ramses objects.
    
    Attributes:
        name: str.
            May contain spaces, [a-z] ,[A-Z], [0-9], [+-].
        shortName: str.
            Used for compact display, limited to 10 characters, must not contain spaces, may contain [a-z] ,[A-Z], [0-9], [+-].
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

    def __init__(self, userName, userShortName, useFolderPath, role = 'STANDARD'):
        """

        Args:
            userName: str.
            userShortName: str.
            useFolderPath: str.
            role: enumerated.
        """

        self.role = role
        self.loggedIn = True
    
    def login(self):
        """Logs the user in. Returns success."""

        #TODO
        pass
    
    def logout(self):
        """Logs the user out."""

        #TODO
        pass

class RamApplication( RamObject ):
    """

    Attributes:
        importTypes: list of RamFileType.
            File types the app can import
        exportTypes: list of RamFileType.
            File types the app can export
        nativeTypes: list of RamFileType.
            File types the app supports natively
        executableFilePath: str.
            Path to the executable file of the application
    """

    def __init__(self, appName, appShortName, execFilePath):
        """

        Args:
            appName: str.
            appShortName: str.
            execFilePath: str.
        """

        self.importTypes = []
        self.exportTypes = []
        self.nativeTypes = []
        self.exectuableFilePath = execFilePath
    
    def login(self):
        """Logs the user in. Returns success."""

        #TODO
        pass

    def logout(self):
        """Logs the user out."""

        #TODO
        pass

class RamFileType( RamObject ):
    """

    Attributes:
        extension: str.
        defaultApplication: RamApplication.
    """

    def __init__(self, typeName, ext, defaultApp):
        """By default, extension and shortName are the same.
        
        Args:
            typeName: str.
            ext: str.
            defaultApp: RamApplication
        """

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

    def createShots(self, range):
        """

        Args:
            range: int list.
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
        fileTypes: list of RamFileType.
        publishFileTypes: list of RamFileType.
        assignedUsers: list of RamUser.
        leads: list of RamUser.
    """
    def __init__(self, stepName, stepShortName):
        """

        Args:
            stepName: str.
            stepShortName: str.
        """

        self.fileTypes = []
        self.publishedFileTypes = []
        self.assignedUsers = []
        self.leads = []

class RamAssetStep( RamStep ):
    """

    Attributes:
        dependsOn: list of RamAssetStep.
    """

    def __init__(self, stepName, stepShortName):
        """

        Args:
            stepName: str.
            stepShortName: str.
        """

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

        self.dependsOn = []

class RamItem( RamObject ):
    """

    Attributes:
        published: bool.
        stepStatuses: list of RamStatus.
    """

    def __init__(self):
        self.published = False
        self.stepStatuses = []
    
    def getWIPFilePath(self):
        """Wip file path relative to the item root folder
        """
        #TODO
        pass

    def getVersionFilePath(self):
        """Version file path relative to the item root folder
        """
        #TODO
        pass
    
    def getPublishedFilePaths(self):
        """Gets the list of file paths in the publish folder, relative to the item root folder
        """
        #TODO
        pass
    
    def getLatestVersion(self):
        """Returns the highest version number
        """
        #TODO
        pass

    def getLatestPubVersion(self):
        """Returns the highest version number for published item
        """
        #TODO
        pass

class RamShot( RamItem ): #WIP getFromPath
    """

    Attributes:
        assets: list of RamAsset.
            The assets used in this shot
    """

    def __init__(self):
        self.assets = []
    
    @staticmethod
    def getFromPath( filePath ): #WIP - TODO
        """Generate a RamShot from a given filePath

        Args:
            filePath: str.
        """
        #Attrs from inheritances: published (bool), stepStatuses (list of RamStatus), name (str), shortName (str), folderPath (str)

        #TODO : check if it is a correct path and a str
        #TODO : check if it respects ramses' naming convention (return None if it aien't)
        #Check if there already exists a RamShot with the same shortname and folder?

        #TODO: check if already published
        # > isPublished()
        #TODO: stepStatuses

        folderPath = os.path.dirname(filePath)
        fileName = os.path.basename(filePath)

        blocks = decomposeRamsesFileName( fileName )[0]

        if blocks[1] != 'S':
            print("The given filepath does not point towards a shot")
            return None
        
        shortName = blocks[2]

        shot = RamShot()
        shot.shortName = shortName
        shot.name = shortName
        shot.folderPath = folderPath

        return shot

    def getFileName(self):
        #TODO
        pass

class RamAsset( RamItem ):
    """

    Attributes:
        tags: list of str.
    """

    def __init__(self):
        self.tags = []
    
    @staticmethod
    def getFromPath( filePath ):
        #TODO
        pass

    def getFileName(self):
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
testProject = "YUKU"
testRamType = "S"
testobjectShortName = "GRANDMA"
testStepName = "RIG"
testComment = ""
testVersion = -1
testExtension = "tar.gz.testing"

print(buildRamsesFileName( project = testProject, step = testStepName, ext = testExtension, ramType = testRamType, objectShortName = testobjectShortName,  comment = testComment, version = testVersion))

tests = [
    "YUKU_G_STEP_comment_v001.tar.gz.test",
    "YUKU_G_STEP_comment_v001",
    "YUKU_G_STEP_v001",
    "YUKU_G_STEP.blend",
    "YUKU_A_GRANDMA_RIG_comment_v012.blend",
    "YUKU_A_GRANDMA_RIG_v012.blend",
    "YUKU_A_GRANDMA_RIG_comment.blend"
    ]

for test in tests:
    print("Testing: " + test)
    print(incrementRamsesFileName(test))

'''
testShotPath = '/home/rainbox/RAINBOX/DEV_SRC/Ramses/Project-Tree-Example/PROJ/PROJ_G_SHOTS/PROJ_S_001/PROJ_S_001_ANIM/PROJ_S_001_ANIM_crowd.blend'
testAssetPath = '/home/rainbox/RAINBOX/DEV_SRC/Ramses/Project-Tree-Example/PROJ/PROJ_G_ASSETS/PROJ_G_ASSETS_Characters/PROJ_A_ISOLDE/PROJ_A_ISOLDE_MOD/PROJ_A_ISOLDE_MOD_test.blend'

'''testShot = RamShot.getFromPath( testShotPath )
print(testShot.shortName)
print(testShot.folderPath)

testAsset = RamAsset.getFromPath( testAssetPath )'''

print(isPublished(testAssetPath))



