import ramses as ram
import os

# Initialise Ramses
ramses = ram.Ramses()

project = ram.RamProject("testProject", "Projet01", '/home/rainbox/RAINBOX/DEV_SRC/Ramses/Project-Tree-Example/Project01')
ramses._currentProject = project

#Test elements

blenderType = ram.RamFileType('blenderFile', 'blend', 'blend', 'blender')

modStep = ram.RamStep('MODELING', 'MOD')
modStep.fileType = blenderType

rigStep = ram.RamStep('RIGGING', 'RIG')
rigStep.fileType = blenderType

testAsset = ram.RamAsset("assetLongName", "ISOLDE")

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

#Actual testing

print(testAsset.getWIPFilePath(modStep))

