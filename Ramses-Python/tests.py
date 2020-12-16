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

status = ram.RamStatus.getFromPath( "/home/rainbox/RAINBOX/DEV_SRC/Ramses/Project-Tree-Example/Project01/04-ASSETS/Characters/Projet01_A_TRISTAN/Projet01_A_TRISTAN_MOD/Projet01_A_TRISTAN_MOD.blend" )

