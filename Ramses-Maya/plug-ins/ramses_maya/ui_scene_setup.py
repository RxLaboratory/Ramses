# -*- coding: utf-8 -*-
import yaml
from PySide2.QtWidgets import (
    QFormLayout,
    QCheckBox,
    QComboBox,
    QHBoxLayout,
    QWidget,
    QLabel,
    QPushButton
)
from PySide2.QtCore import ( # pylint: disable=no-name-in-module,import-error
    Slot,
    Qt
)
from maya import cmds # pylint: disable=no-name-in-module,import-error
import dumaf as maf
from ramses import (
    ItemType,
    log
)
from .ui_dialog import Dialog

class SceneSetupDialog( Dialog ):
    """The Dialog to setup the scene according to Ramses and the project parameters."""

    # <== CONSTRUCTOR ==>

    def __init__(self, parent=None): # pylint: disable=useless-super-delegation
        super(SceneSetupDialog, self).__init__(parent)

        # <-- Setup -->
        self.__setup_ui()
        self.__connect_events()

        self.__fps = 0
        self.__duration = 0
        self.__width = 0
        self.__height = 0
        self.__cam_name = ""
        self.__handle_in = 0
        self.__handle_out = 0
        self.__first_image_number = 1
    
    # <== PRIVATE METHODS ==>

    def __setup_ui(self):
        self.setWindowTitle("Scene Setup")

        main_layout = QFormLayout()
        main_layout.setFormAlignment(Qt.AlignHCenter | Qt.AlignTop)
        self.main_layout.addLayout(main_layout)

        self.__ui_animation_label = QLabel("")
        main_layout.addRow("Animation:", self.__ui_animation_label )

        self.__ui_fps_box = QCheckBox("Fix framerate (24 fps)")
        main_layout.addRow("", self.__ui_fps_box )

        self.__ui_duration_box = QCheckBox("Fix duration (240 frames)")
        main_layout.addRow("", self.__ui_duration_box )

        self.__ui_rendering_label = QLabel("")
        main_layout.addRow("Rendering:", self.__ui_rendering_label )

        self.__ui_resolution_box = QCheckBox("Fix image size (1920x1080 px)")
        main_layout.addRow("", self.__ui_resolution_box )

        self.__ui_cam_widget = QWidget()
        cam_layout = QHBoxLayout(self.__ui_cam_widget)
        cam_layout.setContentsMargins(0,0,0,0)
        cam_layout.setSpacing(3)
        self.__ui_select_camera_box = QCheckBox("Set default camera: ")
        self.__ui_list_camera_box = QComboBox()
        self.__ui_list_camera_box.setEnabled(False)
        cam_layout.addWidget(self.__ui_select_camera_box)
        cam_layout.addWidget(self.__ui_list_camera_box)
        main_layout.addRow("", self.__ui_cam_widget )

        self.__ui_camera_name_box = QCheckBox("Rename camera (shotName)")
        main_layout.addRow("", self.__ui_camera_name_box )

        # <-- BOTTOM BUTTONS -->

        buttons_layout = QHBoxLayout()
        buttons_layout.setSpacing(2)
        self.main_layout.addLayout(buttons_layout)

        self.__ui_fix_button = QPushButton("Fix and continue")
        buttons_layout.addWidget( self.__ui_fix_button )
        self.__ui_ignore_button = QPushButton("Ignore and continue")
        buttons_layout.addWidget( self.__ui_ignore_button )
        self.__ui_cancel_button = QPushButton("Cancel")
        buttons_layout.addWidget( self.__ui_cancel_button )

    def __connect_events(self):
        self.__ui_cancel_button.clicked.connect(self.reject)
        self.__ui_fix_button.clicked.connect(self.fix_scene)
        self.__ui_ignore_button.clicked.connect(self.accept)

        self.__ui_select_camera_box.toggled.connect(self.__ui_list_camera_box.setEnabled)

    # <== PUBLIC METHODS ==>

    def setItem(self, item, step=None):
        ok = True
        
        project = item.project()
        
        # FPS
        project_fps = 0
        scene_fps = 0
        if project:
            scene_fps = maf.animation.get_framerate()
            project_fps = project.framerate()
            self.__fps = project_fps
        if project_fps == scene_fps:
            self.__ui_fps_box.setVisible(False)
            self.__ui_fps_box.setChecked(False)
        else:
            self.__ui_fps_box.setVisible(True)
            self.__ui_fps_box.setChecked(True)
            self.__ui_fps_box.setText("Fix framerate: " + str(project_fps))

        # Duration
        shot_duration = 0
        scene_duration = 0
        scene_handle_in = 0
        scene_handle_out = 0
        start_time = 1
        if item.itemType() == ItemType.SHOT:
            if step:
                settings = step.generalSettings()
                settings = yaml.safe_load( settings )
                shot_settings = settings.get("shot", {})
                self.__handle_in = shot_settings.get("handle_in", 0)
                self.__handle_out = shot_settings.get("handle_out", 0)
                self.__first_image_number = shot_settings.get("first_image_number", 1)
            start_time = cmds.playbackOptions(animationStartTime=True, query=True)
            end_time = cmds.playbackOptions(animationEndTime=True, query=True)
            anim_start_time = cmds.playbackOptions(minTime=True, query=True)
            anim_end_time = cmds.playbackOptions(maxTime=True, query=True)

            scene_duration = int(anim_end_time - anim_start_time)
            scene_handle_in = int(anim_start_time - start_time)
            scene_handle_out = int(end_time - anim_end_time)

            shot_duration = item.frames()
            self.__duration = shot_duration

        # Animation
        if project_fps == scene_fps and shot_duration == scene_duration and scene_handle_in == self.__handle_in and scene_handle_out == self.__handle_out and start_time == self.__first_image_number:
            self.__ui_animation_label.setText("OK!")
            self.__ui_duration_box.setVisible(False)
            self.__ui_duration_box.setChecked(False)
        else:
            ok = False
            self.__ui_animation_label.setText( "Current scene:\n" +
                str(scene_duration) +
                " frames @ " +
                str(scene_fps))
            self.__ui_duration_box.setVisible(True)
            self.__ui_duration_box.setChecked(True)
            self.__ui_duration_box.setText("Fix duration:\n• Shot: " +
                str(shot_duration) + " frames\n• Handles: " +
                str(self.__handle_in) + " frames in / " +
                str(self.__handle_out) + " frames out\n• First image number: " +
                str(self.__first_image_number) )

        # Image Size
        project_w = 0
        project_h = 0
        scene_w = 0
        scene_h = 0
        if project:
            project_w = project.width()
            project_h = project.height()
            scene_w = cmds.getAttr("defaultResolution.width")
            scene_h = cmds.getAttr("defaultResolution.height")
            self.__width = project_w
            self.__height = project_h
        if project_w == scene_w and project_h == scene_h:
            self.__ui_resolution_box.setVisible(False)
            self.__ui_resolution_box.setChecked(False)
        else:
            self.__ui_resolution_box.setVisible(True)
            self.__ui_resolution_box.setChecked(True)
            self.__ui_resolution_box.setText("Fix image size: " + str(project_w) + " x " + str(project_h) + " px")

        # Camera
        shot_name = ""
        scene_cam_name = ""
        if item.itemType() == ItemType.SHOT:
            renderable_cams = maf.rendering.get_renderable_cameras()
            if len(renderable_cams) == 1:
                scene_cam_name = maf.paths.baseName(renderable_cams[0])
            else:
                scene_cam_name = "Multiple_cameras"
            shot_name = item.name().replace(" ", "_").replace("-","_")
            self.__cam_name = shot_name + "_camera"
        if scene_cam_name.startswith(shot_name):
            self.__ui_cam_widget.setVisible(False)
            self.__ui_select_camera_box.setChecked(False)
            self.__ui_camera_name_box.setChecked(False)
            self.__ui_camera_name_box.setVisible(False)
        else:
            self.__ui_cam_widget.setVisible(True)
            self.__ui_select_camera_box.setChecked(True)
            self.__ui_camera_name_box.setChecked(True)
            self.__ui_camera_name_box.setVisible(True)
            self.__ui_camera_name_box.setText("Rename camera: " + shot_name + "_camera")
            maf.ui.update_cam_combobox(self.__ui_list_camera_box)

        if project_w == scene_w and project_h == scene_h and scene_cam_name.startswith(shot_name):
            self.__ui_rendering_label.setText("OK!")
        else:
            ok = False
            self.__ui_rendering_label.setText("Current scene:\n" + 
                str(scene_w) + " x " + str(scene_h) +
                " px | " +
                scene_cam_name)

        return ok

    @Slot()
    def fix_scene(self):
        if self.__ui_fps_box.isChecked() and self.__fps != 0:
            maf.animation.set_framerate(self.__fps)
        if self.__ui_duration_box.isChecked() and self.__duration != 0:
            start_time = self.__first_image_number
            cmds.playbackOptions(animationStartTime = start_time)
            cmds.playbackOptions(animationEndTime = start_time + self.__handle_in + self.__duration + self.__handle_out)
            cmds.playbackOptions(minTime = start_time + self.__handle_in)
            cmds.playbackOptions(maxTime = start_time + self.__handle_in + self.__duration)

        if self.__ui_resolution_box.isChecked():
            if self.__width != 0:
                cmds.setAttr("defaultResolution.width", self.__width)
            if self.__height != 0:
                cmds.setAttr("defaultResolution.height", self.__height)
        if self.__ui_select_camera_box.isChecked():
            maf.rendering.set_renderable_camera(self.__ui_list_camera_box.currentData(Qt.UserRole))
        if self.__ui_camera_name_box.isChecked():
            cameras = maf.rendering.get_renderable_cameras()
            for camera in cameras:
                try:
                    cmds.rename(camera, self.__cam_name)
                except RuntimeError as e:
                    log("Cannot rename camera: " + camera)
                    print(e)
        self.accept()
