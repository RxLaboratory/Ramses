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

from subprocess import Popen, PIPE

from .file_manager import RamFileManager
from .logger import log
from .constants import LogLevel, Log
from .daemon_interface import RamDaemonInterface
from .ram_settings import RamSettings

SETTINGS = RamSettings.instance()
DAEMON = RamDaemonInterface.instance()

class Ramses( object ):
    """The main class. One (and only one) instance globally available, instantiated during init time.

    Static Attributes:
        instance: Ramses
            The unique Ramses instance
    """

    # API Settings
    _version = SETTINGS.version
    apiReferenceUrl = SETTINGS.apiReferenceUrl
    addonsHelpUrl = SETTINGS.addonsHelpUrl
    generalHelpUrl = SETTINGS.generalHelpUrl

    _instance = None

    def __init__(self):
        """
        Ramses is a singleton and cannot be initialized with `Ramses()`. Call Ramses.instance() instead.

        Raises:
            RuntimeError
        """
        raise RuntimeError("Ramses can't be initialized with `Ramses()`, it is a singleton. Call Ramses.instance() instead.")

    @classmethod
    def instance( cls ):
        from .ram_state import RamState

        if cls._instance is None:
            
            cls._instance = cls.__new__(cls)

            log("I'm trying to contact the Ramses Client.", LogLevel.Info)
            cls._instance.connect()

            cls._offline = True
            cls.publishScripts = []
            cls.statusScripts = []
            cls.importScripts = []
            cls.replaceScripts = []
            cls.userScripts = {}

            # Get the default state (TO Do)
            cls.defaultState = cls._instance.state("WIP")

        return cls._instance

    def currentProject(self):
        """The current project.

        Returns:
            RamProject or None
        """
        return DAEMON.getCurrentProject()

    def setCurrentProject( self, project ):
        """Sets the current project (useful if offline)"""
        DAEMON.setCurrentProject( project.uuid() )

    def currentUser(self):
        """The current user.

        Returns:
            RamUser or None
        """
        return DAEMON.getCurrentUser()

    def online(self):
        """True if connected to the Daemon and the Daemon is responding.

        Returns:
            bool
        """
        return not self._offline

    def alternativeFolderPaths(self):  # TODO
        """A list of alternative absolute paths to the main Ramses folder.
        Missing files will be looked for in these paths (and copied to the main path if available),
        and they will be used if the main path is not available.

        Returns:
            str list
        """
        pass

    def backupFolderPath(self):  # TODO
        """A copy of the main folder where all files are stored.

        Returns:
            str
        """
        pass

    def folderPath(self):
        """The absolute path to the main Ramses folder, containing projects by default,
        config files, user folders, admin files…

        Returns:
            str
        """
        return DAEMON.getRamsesFolderPath()

    def projectsPath(self):
        """Returns the default path for projects"""

        folderPath = self.folderPath()
        if folderPath == "":
            return ""

        return RamFileManager.buildPath((
            folderPath,
            SETTINGS.folderNames.projects
        ))

    def usersPath(self):
        """Returns the default path for users"""

        folderPath = self.folderPath()
        if folderPath == "":
            return ""

        return RamFileManager.buildPath((
            folderPath,
            SETTINGS.folderNames.users
        ))

    def connect(self):
        """Checks Daemon availability and initiates the connection. Returns success.

        Returns:
            bool
        """

        # Check if already online
        self._offline = False
        if DAEMON.online():
            user = self.currentUser()
            if user:
                return True
            else:
                DAEMON.raiseWindow()
                log( Log.NoUser, LogLevel.Info )
        else:
            # Try to open the client
            self.showClient()

        self._offline = True
        return False

    def disconnect(self):
        """Gets back to offline mode.

        Returns:
            bool
        """
        self._offline = True

    def daemonInterface(self):
        """The Daemon interface.

        Returns:
            RamDaemonInterface
        """
        return DAEMON

    def project(self, shortName):
        """
        Gets a project using its shortName
        
        return:
            RamProject
        """
        projs = self.projects()
        for p in projs:
            if p.shortName() == shortName:
                return p
        return None

    def projects(self):
        """The list of available projects.

        Returns:
            list of RamProject
        """
        return DAEMON.getProjects()

    def state(self, stateShortName="WIP"):
        """Gets a specific state.

        Args:
            stateShortName (str, optional): Defaults to "WIP".

        Returns:
            RamState
        """

        if not self._offline:
            stts = self.states()
            for stt in stts:
                if stt.shortName() == stateShortName:
                    return stt
        return None

    def states(self):
        """The list of available states.

        Returns:
            list of RamState
        """
        from ramses.ram_state import RamState
        states = DAEMON.getObjects( "RamState" )
        # Order before returning
        states.sort( key=RamState.stateSorter )
        return states

    def showClient(self):
        """Raises the Ramses Client window, launches the client if it is not already running.
        """

        if SETTINGS.ramsesClientPath == "":
            self._offline = True
            return False

        try:
            p = Popen(SETTINGS.ramsesClientPath, stdin=PIPE, stdout=PIPE, stderr=PIPE)
        except: # pylint: disable=bare-except
            log("The Client is not available at " + SETTINGS.ramsesClientPath, LogLevel.Critical)
            return False

        if not p.poll():
            del p
        else:
            log("The Client can't be launched correctly.", LogLevel.Critical)
            return False

        return True

    def settings(self):
        """

        Args:
            (RamSettings): settings
        """
        return SETTINGS

    @staticmethod
    def version():
        """The current version of this API

        Returns:
            str
        """
        return Ramses._version

    def publish(self, item, step, filePath, publishOptions=None, showPublishOptions=False ):
        """Publishes the item; runs the list of scripts Ramses.publishScripts"""
        for script in self.publishScripts:
            script(item, step, filePath, publishOptions, showPublishOptions)

    def updateStatus(self, item, status, step=None):
        for script in self.statusScripts:
            script( item, status, step)

    def importItem(self, item, filePath, step=None, importOptions=None, showImportOptions=False ):
        for script in self.importScripts:
            script( item, filePath, step, importOptions, showImportOptions )

    def replaceItem(self, item, filePath, step=None, importOptions=None, showImportOptions=False):
        for script in self.replaceScripts:
            script( item, filePath, step, importOptions, showImportOptions )