'''
TODO: in constructors of all classes, check if inherited attributes are given a default value (otherwise they will not exist :) )
'''

import os
import re

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
        print("The given filename does not match Ramses' naming convention. It was this: " + ramsesFileName)
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

    def getSteps(self, typeOrCat = "ALL"):
        """The list of available steps.

        Use typeOrCat to filter the results.
        One of: ALL, ASSET, SHOT, PRE-PROD, PROD, POST-PROD.
        PROD represents a combination of SHOT and ASSET

        Returns: list of RamStep
        """
        #TODO
        pass

    def getUsers(self):
        """The list of available users.

        Returns: list of RamUser
        """
        #TODO
        pass

    def folderPath(self):
        """The absolute path to main Ramses folder, containing projects by default, config files, user folders, admin files…

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

    def getAbsolutePath(self, relativePath):
        """

        Attrs:
            relativePath: str
        
        Returns: str
        """
        print( Ramses.instance.currentProject.folderPath + '/' + relativePath)
        pass

    def getAssets(self, groupName = ""):
        """If groupName is an empty string, returns all assets.

        Returns list of RamAsset
        """
        #TODO
        pass

    def getAssetGroups(self):
        """

        Returns: list of str
        """
        #TODO
        pass

    def getShots(self, filter = "*"):
        """A filter to be applied to the name of the shots, using “*” as a wildcard.

        Returns: list of RamShot
        """
        #TODO
        pass

    def getSteps(self, typeOrCat = "ALL"):
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
        stepStatuses: list of RamStepStatus
    """

    def __init__(self, itemName, itemShortName, itemFolder = ""):
        self.name = itemName
        self.shortName = itemShortName
        self.folderPath = itemFolder
        self.stepStatuses = []

    def getLatestVersion( self, step, resource = "", stateId = 'wip' ):
        """Returns the highest version number for the given state (wip, pub…).

        Args:
            step: RamStep
            resource: str
            state: str

        Returns: int
        """
 
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
        if not isinstance(step, RamStep):
            raise TypeError("Step must be an instance of RamStep")
        if not isinstance(stateId, str):
            raise TypeError("State must be a str")

        baseName = os.path.basename(self.folderPath) + '_' + step.shortName #Name without the resource str (added later)
        stepFolderPath = folderPath + '/' + baseName
        
        if os.path.isdir(stepFolderPath) == False:
            print("The folder for the following step: " + step.shortName + " has not been found.")
            return None
        if os.path.isdir(stepFolderPath + '/ramses_versions') == False:
            print("ramses_versions directory has not been found")
            return None

        foundFiles = os.listdir(stepFolderPath + '/ramses_versions')
        highestFileVersion = 0

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
            if versionInt > highestFileVersion:
                highestFileVersion = versionInt

        return highestFileVersion

    def getPublishedFilePaths(self, step, resource = ""):
        """Gets the list of file paths in the publish folder. Paths are relative to the root of the item folder.

        Args:
            step: RamStep
            resource: str
        
        Returns: list of str
        """
        #TODO
        print("functionStart")
        print(self.folderPath)
        if self.folderPath == '':
            print("The given item has no folderPath.")
            return None
        if not os.path.isdir( self.folderPath ):
            print("The given item's folder was not found.\nThis is the path that was checked:\n" + self.folderPath)
            return None
        pass

    def getVersionFilePath(self, step, resource = ""):
        """Latest version file path relative to the item root folder.

        Args:
            step: RamStep
            resource: str
        
        Returns: str
        """
        #TODO
        pass
    
    def getWIPFilePath(self, step, resource = ""):
        """Current wip file path relative to the item root folder.

        Args:
            step: RamStep
            resource: str
        
        Returns: str
        """
        #TODO
        pass

    def isPublished(self, resource = ""):
        """Checks if there is a publish in the publish folder

        Args:
            resource: str
        
        Returns: bool
        """
        #TODO
        #returns false if self.folderPath == ''
        pass

    def setStepStatus(self, status, step):
        """Adds a new status in stepStatuses for the given step.

        Args:
            status: RamStatus
            step: RamStep
        """
        #TODO
        pass

class RamShot( RamItem ): #TODO: update getFromPath to take relativePaths into account
    """A shot.
    """

    def __init__(self, shotName, folderPath = ""):
        self.shortName = shotName
        self.name = shotName
        self.stepStatuses = []
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
    def getFromPath( filePath ): #TODO: relativePaths + complete attrs (stepStatuses)
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

        #Attrs from inheritances: stepStatuses (list of RamStatus), name (str), shortName (str), folderPath (str)
        shortName = blocks["objectShortName"]
        shot = RamShot( shotName = shortName)
        shot.name = blocks["objectShortName"]
        shot.folderPath = folderPath
        #TODO: stepStatuses

        return shot

class RamAsset( RamItem ): #TODO: getFromPath
    """A class representing an asset.
    """

    def __init__(self, assetName, assetShortName, folderPath=""):
        self.tags = []
        self.name = assetName
        self.shortName = assetShortName
        self.stepStatuses = []
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
                if re.match('^([a-z0-9+-]{1,10})_[ASG]_([a-z0-9+-]{1,10})$' , element , re.IGNORECASE) != None:
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
    def getFromPath(filePath):
        """Returns a RamAsset instance built using the given file path.
        """
        #TODO
        pass

    def getTags(self):
        """Some tags describing the asset.

        Returns: list of str
        """
        #TODO
        pass

    def getGroup(self):
        """The group containing this asset.

        Returns: str
        """
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
        """

        Returns: list of RamStatus
        """
        #TODO
        pass

    def setStatus(self, status):
        """Adds a new status to the history.

        Args:
            status: RamStatus
        """
        #TODO
        pass

# Initialization

Ramses.instance = None