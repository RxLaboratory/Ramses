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

from datetime import datetime
import linecache
import sys
from .constants import LogLevel

def log( message, level = LogLevel.Info ):
    from .ram_settings import RamSettings

    message = str(message)

    minLevel = RamSettings.instance().logLevel
    if (level < minLevel ): return
    
    if level == LogLevel.DataReceived:
        message = "Ramses has just recieved some data: " + message
    elif level == LogLevel.DataSent:
        message = "Ramses has just sent some data: " + message
    elif level == LogLevel.Debug:
        message = "Debug Info from Ramses: " + message
    elif level == LogLevel.Info:
        message = "Ramses says: " + message
    elif level == LogLevel.Critical:
        message = "/!\\ Critical error, Ramses is shouting: " + message
    elif level == LogLevel.Fatal:
        message = "/!\\ Fatal error, Ramses last words are: " + message

    now = datetime.now()

    #print( "[" + now.strftime("%Y/%m/%d - %H:%M:%s") + "] " + message )
    print( message )

def printException():
    exc_type, exc_obj, tb = sys.exc_info()
    f = tb.tb_frame
    lineno = tb.tb_lineno
    filename = f.f_code.co_filename
    linecache.checkcache(filename)
    line = linecache.getline(filename, lineno, f.f_globals)

    log(
        'an EXCEPTION was thrown:\n\tfrom `{}`\n\tat line {}: "{}"\n\t\t{}'.format(filename, lineno, line.strip(), exc_obj),
        LogLevel.Critical
        )
