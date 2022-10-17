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
import platform
import json
from .constants import FolderNames, LogLevel
from .logger import log

theVersion = "0.5.0-Beta3"

class RamSettings( object ):
    """Gets and saves settings used by Ramses.

    To get a setting, just get the corresponding attribute.

    To change a setting temporarily, just set the corresponding attribute. If you want the change to be permanent (i.e. keep the setting for the next sessions), call the save() method.

    By default, settings are saved in a ramses_addons_settings.json file, in the user’s OS specific settings folder:

        Linux: ~/.config/RxLaboratory/Ramses/Config
        Windows: %appdata%/RxLaboratory/Ramses/Config
        MacOS: ?

    There is only one instance of RamSettings, available with the Ramses.instance().settings() method
    """

    _instance = None
    
    def __init__( self ):
        raise RuntimeError("RamSettings can't be initialized with `RamSettings()`, it is a singleton. Call `Ramses.instance().settings()` or `RamSettings.instance()` instead.")

    @classmethod
    def instance( cls ):
        if cls._instance is None:
            cls._instance = cls.__new__(cls)

            # Default Values

            # Location of the Ramses Client executable file (.exe on Windows, .app on MacOS, .appimage or binary on Linux)
            cls.ramsesClientPath =  cls.defaultRamsesClientPath = ""
            # Listening port of the Ramses Daemon
            cls.ramsesClientPort = cls.defaultRamsesClientPort = 18185
            # Minimum Log level printed when logging information
            cls.logLevel = cls.defaultLogLevel = LogLevel.Info
            # Timeout before auto incrementing a file, in minutes
            cls.autoIncrementTimeout = cls.defaultAutoIncrementTimeout = 120
            # A Debug mode to throw errors
            cls.debugMode = cls.defaultDebugMode = False
            # The custom settings
            cls.userSettings = {}

            # Not Documented: these are not settings to be customized (yet)

            cls.folderNames = FolderNames()
            cls.versionPrefixes = ['v','pub'] # The prefixes used in version files which are not states

            # API Settings
            cls.version = theVersion
            cls.apiReferenceUrl = "https://ramses.rxlab.guide/dev/"
            cls.addonsHelpUrl = "https://ramses.rxlab.guide/components/addons/"
            cls.generalHelpUrl = "https://ramses.rxlab.guide"

            # Set the path to the settings file and temporary folder (os-specific)
            system = platform.system()
            if system == 'Windows':
                cls._folderPath = os.path.expandvars('${APPDATA}/RxLaboratory/Ramses/Config')
                if not os.path.isdir( cls._folderPath ): 
                    os.makedirs( cls._folderPath )
                cls._filePath = cls._folderPath + '/ramses_addons_settings.json'
            else:
                cls._folderPath = ''
                cls._filePath = ''

            # Get settings from file
            if os.path.isfile( cls._filePath ):
                with open(cls._filePath, 'r') as settingsFile:
                    settingsStr = settingsFile.read()
                    settingsDict = json.loads( settingsStr )
                    if 'clientPath' in settingsDict:
                        cls.ramsesClientPath = settingsDict['clientPath']
                    if 'clientPort' in settingsDict:
                        cls.ramsesClientPort = settingsDict['clientPort']
                    if 'logLevel' in settingsDict:
                        cls.logLevel = settingsDict['logLevel']
                    if 'autoIncrementTimeout' in settingsDict:
                        cls.autoIncrementTimeout = settingsDict['autoIncrementTimeout']
                    if 'debugMode' in settingsDict:
                        cls.debugMode = settingsDict['debugMode']
                    if 'userSettings' in settingsDict:
                        cls.userSettings = settingsDict['userSettings']

        return cls._instance

    def folderPath(self):
        return self._folderPath

    def filePath(self):
        return self._filePath

    def save( self ):
        """Saves the current settings to the disk.
        """

        log("I'm saving your settings...")

        settingsDict = {
            'clientPath': self.ramsesClientPath,
            'clientPort': self.ramsesClientPort,
            'logLevel': self.logLevel,
            'autoIncrementTimeout': self.autoIncrementTimeout,
            'userSettings': self.userSettings,
            'debugMode': self.debugMode,
        }

        if self._filePath == '':
            raise ("Invalid path for the settings, I can't save them, sorry.")
        
        with open(self._filePath, 'w') as settingsFile:
            settingsFile.write( json.dumps( settingsDict, indent=4 ) )

        log("Settings saved!")
