# -*- coding: utf-8 -*-

import maya.cmds as cmds # pylint: disable=import-error
from math import floor, ceil

FRAMERATES=( '2 fps','3 fps','4 fps','5 fps','6 fps','8 fps','10 fps','12 fps','15 fps','16 fps','20 fps','23.976 fps','24 fps','25 fps','29.97 fps','29.97 df','30 fps',
    '40 fps','47.952 fps','48 fps','50 fps','59.94 fps','60 fps','75 fps','80 fps','90 fps',
    '100 fps','120 fps','125 fps','150 fps','200 fps','240 fps','250 fps','300 fps','375 fps','400 fps','500 fps','600 fps','750 fps',
    '1200 fps','1500 fps','2000 fps','3000 fps','6000 fps','44100 fps','48000 fps' )

def removeAll():
    keys = cmds.ls(type='animCurveTL') + cmds.ls(type='animCurveTA') + cmds.ls(type='animCurveTU')
    for key in keys:
        try:
            cmds.delete(key)
        except:
            pass

def get_framerate():
    '''
    Return an int of the current frame rate
    '''
    currentUnit = cmds.currentUnit(query=True, time=True)
    if 'fps' in currentUnit:
        return float(currentUnit.replace('fps',''))
        
    if currentUnit == 'film':
        return 24
    if currentUnit == 'show':
        return 48
    if currentUnit == 'pal':
        return 25
    if currentUnit == 'ntsc':
        return 30
    if currentUnit == 'palf':
        return 50
    if currentUnit == 'ntscf':
        return 60
    

    return 1
    
def set_framerate(fps):
    if type(fps) == int or type(fps) == float:
        fps=str(fps)+'fps'
    else:
        fps=fps.replace(' ','')
    # Change Framerate
    cmds.currentUnit(time=str(fps)+'fps')
    # Round start and end time
    start_time = cmds.playbackOptions(animationStartTime=True, query=True)
    end_time = cmds.playbackOptions(animationEndTime=True, query=True)
    current_time = cmds.currentTime( query=True )
    min_time = cmds.playbackOptions(minTime=True, query=True)
    max_time = cmds.playbackOptions(maxTime=True, query=True)
    start_time = floor(start_time)
    min_time = floor(min_time)
    end_time = ceil(end_time)
    max_time = ceil(max_time)
    current_time = int(current_time)
    cmds.playbackOptions(animationStartTime=start_time)
    cmds.playbackOptions(animationEndTime=end_time)
    cmds.playbackOptions(minTime=min_time)
    cmds.playbackOptions(maxTime=max_time)
    cmds.currentTime( current_time )
