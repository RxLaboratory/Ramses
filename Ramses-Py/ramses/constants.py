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

class FolderNames():
    projects = "Projects"
    users = "Users"
    preview = "_preview"
    versions = "_versions"
    publish = "_published"
    config = "_config"
    stepTemplates = "Templates"
    admin = "00-ADMIN"
    preProd = "01-PRE-PROD"
    prod = "02-PROD"
    postProd = "03-POST-PROD"
    assets = "04-ASSETS"
    shots = "05-SHOTS"
    export = "06-EXPORT"

class FileNames():
    META_DATA = "_ramses_data.json"

class MetaDataKeys():
    COMMENT = 'comment'
    VERSION = 'version'
    VERSION_FILE = 'versionFilePath'
    PIPE_TYPE = 'pipeFileShortName'
    MODIFICATION_HISTORY = 'history'
    DATE = 'date'
    STATE = 'stateShortName'
    RESOURCE = 'resource'

class ItemType():
    GENERAL='G'
    ASSET='A'
    SHOT='S'

class Log():
    MalformedName = "Cannot work on this file as it does not respect the Ramses' naming scheme, sorry."
    NoUser = "There's no current user. You may need to log in."
    PathNotFound = "The file or folder path could not be found."
    NoProject = "There's no current project. Select a project first."
    StateNotFound = "State not found."
    NotAnItem = "This file does not seem to be a valid Ramses Item, sorry."

class LogLevel():
    DataReceived = -2
    DataSent = -1
    Debug = 0
    Info = 1
    Critical = 2
    Fatal = 3

class StepType():
    PRE_PRODUCTION = 'PRE_PRODUCTION'
    ASSET_PRODUCTION = 'ASSET_PRODUCTION'
    SHOT_PRODUCTION = 'SHOT_PRODUCTION'
    POST_PRODUCTION = 'POST_PRODUCTION'
    ALL = 'ALL' # tous
    PRODUCTION = 'PRODUCTION' # asset and shot

class UserRole():
    STANDARD = 0
    LEAD = 1
    PROJECT_ADMIN = 2
    ADMIN = 3
