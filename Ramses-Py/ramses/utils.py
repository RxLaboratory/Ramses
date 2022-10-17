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

def getDate( e ):
    # Used in RamItem.getStepHistory to sort the list
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

def intToStr( i, numDigits=3):
    intStr = str(i)
    while len(intStr) < numDigits:
        intStr = '0' + intStr
    return intStr

def removeDuplicateObjectsFromList( l ):
    newList = []
    for i in l:
        if not i in newList:
            newList.append(i)
    return newList
