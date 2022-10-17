# -*- coding: utf-8 -*-
"""Rendering functions"""

from maya import cmds

def set_renderable_camera(camera):
    cameras = cmds.ls(type='camera')
    for cam in cameras:
        # get the transform node
        cam = cmds.listRelatives(cam, parent=True, f=True, type='transform')[0]
        cmds.setAttr( cam + '.renderable', cam == camera)

def get_renderable_cameras():
    cameras = cmds.ls(type='camera')
    renderableCameras = []
    for camera in cameras:
        # get the transform node
        camera = cmds.listRelatives(camera, parent=True, f=True, type='transform')[0]
        if cmds.getAttr( camera + '.renderable'):
            renderableCameras.append(camera)
    return renderableCameras

def get_persp_cameras():
    cameras = cmds.ls(type='camera')
    perspCameras = []
    for camera in cameras:
        # get the transform node
        camera = cmds.listRelatives(camera, parent=True, f=True, type='transform')[0]
        if not cmds.camera(camera, orthographic=True, query=True):
            perspCameras.append(camera)
    return perspCameras

def get_ortho_cameras():
    cameras = cmds.ls(type='camera')
    orthoCameras = []
    for camera in cameras:
        # get the transform node
        camera = cmds.listRelatives(camera, parent=True, f=True, type='transform')[0]
        if cmds.camera(camera, orthographic=True, query=True):
            orthoCameras.append(camera)
    return orthoCameras