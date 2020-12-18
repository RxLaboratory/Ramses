import ramses as ram
import os
import datetime

# Initialise Ramses
ramses = ram.Ramses()

project = ram.RamProject("testProject", "Projet01", '/home/rainbox/RAINBOX/DEV_SRC/Ramses/Project-Tree-Example/Project01')
user = ram.RamUser('user01', 'user01', '')

ramses._currentProject = project
ramses._currentUser = user

#---------------------

blenderType = ram.RamFileType('blenderFile', 'blend', 'blend', 'blender')

modStep = ram.RamStep('MODELING', 'MOD')
modStep.fileType = blenderType

rigStep = ram.RamStep('RIGGING', 'RIG')
rigStep.fileType = blenderType

testAsset = ram.RamAsset("assetLongName", "ISOLDE")

testNames = [
    'PROJ_A_ISOLDE_RIG.blend',
    'PROJ_A_ISOLDE_RIG_resource.blend',
    'PROJ_A_ISOLDE_RIG_pub9.blend',
    'PROJ_A_ISOLDE_MOD_resource_wip10.tar.gz',
    'PROJ_G_SCRIPT_resource_pub009.tar.gz',
    'PROJ_G_SCRIPT_pub010.tar.gz',
    'PROJ_G_SCRIPT_0010.tar.gz',
    'PROJ_G_SCRIPT_0002.tar.gz',
    'PROJ_G_SCRIPT.tar.gz',
]

testShotSearch = [
    '012',
    '*012',
    'A-*',
    '*-003',
    '*test*',
    '',
]

#---------------------
#Tristan mod, no resource str. Wip, version, pub
path01 = '/home/rainbox/RAINBOX/DEV_SRC/Ramses/Project-Tree-Example/Project01/04-ASSETS/Characters/Projet01_A_TRISTAN/Projet01_A_TRISTAN_MOD/Projet01_A_TRISTAN_MOD.blend'
path02 = '/home/rainbox/RAINBOX/DEV_SRC/Ramses/Project-Tree-Example/Project01/04-ASSETS/Characters/Projet01_A_TRISTAN/Projet01_A_TRISTAN_MOD/ramses_versions/Projet01_A_TRISTAN_MOD_pub002.blend'
path03 = '/home/rainbox/RAINBOX/DEV_SRC/Ramses/Project-Tree-Example/Project01/04-ASSETS/Characters/Projet01_A_TRISTAN/Projet01_A_TRISTAN_MOD/ramses_publish/Projet01_A_TRISTAN_MOD.blend'

#Isolde mod, resource = test. Wip, version
path11 = '/home/rainbox/RAINBOX/DEV_SRC/Ramses/Project-Tree-Example/Project01/04-ASSETS/Characters/Projet01_A_ISOLDE/Projet01_A_ISOLDE_MOD/Projet01_A_ISOLDE_MOD_test.blend'
path12 = '/home/rainbox/RAINBOX/DEV_SRC/Ramses/Project-Tree-Example/Project01/04-ASSETS/Characters/Projet01_A_ISOLDE/Projet01_A_ISOLDE_MOD/ramses_versions/Projet01_A_ISOLDE_MOD_test_wip004.blend'

#Shot 001 light. Does not have ramses_versions
path21 = '/home/rainbox/RAINBOX/DEV_SRC/Ramses/Project-Tree-Example/Project01/05-SHOTS/Projet01_S_001/Projet01_S_001_LIGHT/Projet01_S_001_LIGHT.blend'

#Shot 001 anim, resource = 'crowd'. Wip, version
path31 = '/home/rainbox/RAINBOX/DEV_SRC/Ramses/Project-Tree-Example/Project01/05-SHOTS/Projet01_S_001/Projet01_S_001_ANIM/Projet01_S_001_ANIM_crowd.blend'
path32 = '/home/rainbox/RAINBOX/DEV_SRC/Ramses/Project-Tree-Example/Project01/05-SHOTS/Projet01_S_001/Projet01_S_001_ANIM/ramses_versions/Projet01_S_001_ANIM_crowd_pub003.blend'

test = ram.RamStepStatus()
result = test.getHistory(filePath = path11)

for element in result:
    print(element.user)
    print(element.date)
    print(element.version)
    print(element.state.shortName)

