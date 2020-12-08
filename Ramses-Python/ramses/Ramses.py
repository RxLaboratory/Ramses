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

        #todo
        pass
    
    def logout(self):
        """Logs the user out."""

        #todo
        pass

    def connect(self):
        """Checks server or client availability and initiates the connection. Returns success."""

        #todo
        pass

    def disconnect(self):
        """Gets back to offline mode."""

        #todo
        pass

    def createProject(self, projectName, projectShortName, folder = ""):
        """Returns RamProject.

        Args:
            projectName: str.
            projectShortName: str
            folder: str
        """

        #todo
        pass

    def createUser(self, userName, userShortName, folder = ""):
        """Returns RamUser.

        Args:
            userName: str
            userShortName: str
            folder: str
        """

        #todo
        pass

    def createStep(self, stepName, stepShortName):
        """Returns RamStep.

        Args:
            stepName: str.
            stepShortName: str.
        """

        #todo
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

        #todo
        pass
    
    def logout(self):
        """Logs the user out."""

        #todo
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

        #todo
        pass

    def logout(self):
        """Logs the user out."""

        #todo
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

        #todo
        pass

    def addShot(self, shot):
        """

        Args:
            shot: RamShot.
        """

        #todo
        pass

    def createShot(self, shotName):
        """

        Args:
            shotName: str.
        """

        #todo
        pass

    def createShots(self, range):
        """

        Args:
            range: int list.
        """

        #todo
        pass

    def addAsset(self, asset):
        """

        Args:
            asset: RamAsset
        """

        #todo
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

class RamShot( RamObject ):
    """

    Attributes:
        stepStatuses: list of RamStatus.
        assets: list of RamAsset.
            The assets used in thi s shot
    """

    def __init__(self, shotName):
        """

        Args:
            shotName: str.
        """

        self.stepStatuses = []
        self.assets = []

    def getFileName(self):
        #todo
        pass

class RamAsset( RamObject ):
    """

    Attributes:
        stepStatuses: list of RamStatus.
        tags: list of str.
    """

    def __init__(self, assetName, assetShortName):
        """

        Args:
            assetName: str.
            assetShortName: str.
        """

        self.stepStatuses = []
        self.assets = []
    
    def getFileName(self):
        #todo
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

