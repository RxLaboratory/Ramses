import ramses as ram

# Initialise Ramses
ramses = ram.Ramses()

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
animStep = ram.RamStep(stepName = "animation", stepShortName = "ANIM")

ramShot = ram.RamShot( '001' )
#ramShot.folderPath = '/home/rainbox/RAINBOX/DEV_SRC/Ramses/Project-Tree-Example/Project01/05-SHOTS/Projet01_S_001'

ramShot.getPublishedFilePaths(step = animStep, resource = "crowd")