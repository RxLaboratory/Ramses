'''TO DO:
-getLatestPubVersion

-getLatestVersion

-isPublished

-getAsset(filename) -> RamAsset / None

-getShot(filename) -> RamShot / None

'''

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

def buildRamsesName( project , step , ext , ramType = 'G' , objectName = '' , comment = "" , version = -1 , version_prefixe = 'wip' ):
    #PROJECT_A_OBJECT_STEP_comment_wip012.extension
    #PROJECT_G_STEP_comment_wip012.extension

    comment = fixComment( comment )
    ramsesName = project + '_' + ramType

    if ramType in ('A', 'S'):
        ramsesName = ramsesName + '_' + objectName

    ramsesName = ramsesName + '_' + step

    if comment != '':
        ramsesName = ramsesName + '_' + comment

    if version != -1:
        ramsesName = ramsesName + '_' + version_prefixe
        if version < 10:
            ramsesName = ramsesName + '00' + str(version)
        elif version < 100:
            ramsesName = ramsesName + '0' + str(version)
        else:
            ramsesName = ramsesName + str(version)
    
    ramsesName = ramsesName + '.' + ext

    return ramsesName

def decomposeRamsesName( ramsesName ):
    splitRamsesName = ramsesName.split('_')
    splitExtension = splitRamsesName[-1].split('.')

    blocks = splitRamsesName[0:-1]
    blocks.append(splitExtension[0])
    extension = ''

    if len(splitExtension) > 1: # If more than one item, all items are part of the extension except the first one
        extension = '.'.join(splitExtension[1:])
    else:
        extension = None

    return blocks, extension

def getFileProjectId( ramsesName ):
    fileProjectId = ramsesName.split('_')[0]
    return fileProjectId

def getFileRamTypeId( ramsesName ):
    fileRamTypeId = ramsesName.split('_')[1]
    return fileRamTypeId

def getFileObjectId( ramsesName ):
    if getFileRamTypeId( ramsesName ) == 'G':
        print("File is not an object")
        return None
    fileObjectId = ramsesName.split('_')[2]
    return fileObjectId

def getFileStepId( ramsesName ):
    if getFileRamTypeId( ramsesName ) == 'G':
        fileStepId = ramsesName.split('_')[2]
    else:
        fileStepId = ramsesName.split('_')[3]
    return fileStepId

def getFileComment( ramsesName ):
    blocks = decomposeRamsesName( ramsesName )[0]

    if blocks[1] == 'G' and len(blocks) > 3 or blocks[1] in ('A', 'S') and len(blocks) > 4: #is long enough to have a comment and/or a version
        if blocks[1] == 'G' and isVersion(blocks[3]) == False :
            return blocks[3]
        elif blocks[1] in ('A', 'S') and isVersion(blocks[4]) == False :
            return blocks[4]

    return None

def getFileVersion( ramsesName ):
    blocks = decomposeRamsesName( ramsesName )[0]
    fileVersion = 0
    state = ''

    if blocks[1] == 'G' and len(blocks) > 3 or blocks[1] in ('A', 'S') and len(blocks) > 4: #is long enough to have a comment and/or a version
        if isVersion(blocks[-1]):
            match = re.findall( getVersionRegEx(), blocks[-1] )
            state = match[0][0]
            fileVersion = int(match[0][1])
            return state, fileVersion

    return None

def getFileExtension( ramsesName ):
    fileExtension = decomposeRamsesName( ramsesName )[-1]
    return fileExtension

def incrementRamsesName( ramsesName ):
    separatedBlocks, extension = decomposeRamsesName( ramsesName )
    state, fileVersion = getFileVersion( ramsesName )

    if fileVersion == None:
        fullBlocks = ramsesName.split('.')[0]
        ramsesName = fullBlocks + '_wip001.' + extension
        return ramsesName

    fileVersion = fileVersion + 1
    ramsesName = ''

    for block in separatedBlocks[0:-1]: #rebuilds name up to the file version
        ramsesName = ramsesName + block + '_'
    
    ramsesName = ramsesName + state
    
    if fileVersion < 10:
        ramsesName = ramsesName + '00' + str(fileVersion)
    elif fileVersion < 100:
        ramsesName = ramsesName + '0' + str(fileVersion)
    else:
        ramsesName = ramsesName + str(fileVersion)
    
    if extension != None:
        ramsesName = ramsesName + '.' + extension

    return ramsesName

def getLatestPubVersion( ramsesName ): #todo
    #todo. returns int, numero le plus haut parmi les pub
    pass

def getLatestVersion( ramsesName ): #todo
    #todo. returns int, numero le plus haut
    pass

def isPublished( ramsesName ): #todo
    #todo. returns bool. True if last version is a pub
    pass


'''
testProject = "YUKU"
testRamType = "S"
testObjectName = "GRANDMA"
testStepName = "RIG"
testComment = ""
testVersion = -1
testExtension = "tar.gz.testing"

print(buildRamsesName( project = testProject, step = testStepName, ext = testExtension, ramType = testRamType, objectName = testObjectName,  comment = testComment, version = testVersion))


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
    print(incrementRamsesName(test))
'''

print(incrementRamsesName("YUKU_G_STEP_wipcomment012wip012_pub012.blend"))
