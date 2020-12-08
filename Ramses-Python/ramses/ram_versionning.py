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

def getLatestPubVersion( ramsesFileName ): #todo
    #todo. returns int, numero le plus haut parmi les pub
    pass

def getLatestVersion( ramsesFileName ): #todo
    #todo. returns int, numero le plus haut
    pass

def isPublished( ramsesFileName ): #todo
    #todo. returns bool. True if last version is a pub
    pass


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


print(incrementRamsesFileName("YUKU_G_STEP_wipcomment012wip012_pub012.blend"))
'''