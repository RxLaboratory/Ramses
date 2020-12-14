import ramses as ram

import os
# Initialise Ramses
ramses = ram.Ramses()

project = ram.RamProject("testProject", "Projet01", '/home/rainbox/RAINBOX/DEV_SRC/Ramses/Project-Tree-Example/Project01')
ramses._currentProject = project

'''
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
'''

testStep = ram.RamStep('stepLongName', 'MOD')
testAsset = ram.RamAsset("assetLongName", "TRISTAN")

testShot = ram.RamShot.getFromPath('/home/rainbox/RAINBOX/DEV_SRC/Ramses/Project-Tree-Example/Project01/05-SHOTS/Projet01_S_001/Projet01_S_001_ANIM/Projet01_S_001_ANIM_crowd.blend')

test = ram.RamAsset.getFromPath('/home/rainbox/RAINBOX/DEV_SRC/Ramses/Project-Tree-Example/Project01/04-ASSETS/Characters/Projet01_A_TRISTAN/Projet01_A_TRISTAN_RIG/Projet01_A_TRISTAN_RIG.blend')
print(test.folderPath)


