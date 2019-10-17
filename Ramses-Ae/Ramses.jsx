#include "Ramses_required/Ramses_license.jsxinc"

//encapsulate the script in a function to avoid global variables
( function ( thisObj ) {

    //=============== INCLUDES ================

    #include DuAEF.jsxinc
    DuAEF.init("Ramses", "0.0.0-b");

    #include 'Ramses_required/Ramses_w25icons.jsxinc'

    // ================ SETTINGS ==============

    // Restores the default settings
    //TODO move in DuAEF.DuSettings along with a function to set defaults
    function setDefaultSettings( s ) {
        s.data.debugMode = true;
        s.data.versionFolderName = DuAEF.Ramses.versionFolderName = '_versions';
        s.data.wipName = DuAEF.Ramses.wipName = 'WIP';
        s.save();
    }

    // Create/load settings
    var settings = new DuSettings( DuAEF.scriptName );
    setDefaultSettings( settings );


    // temp debug mode
    DuAEF.debug = settings.data.debugMode;


    // ================ FUNCTIONS =============
    
    #include 'Ramses_required/Ramses_api-functions.jsxinc'

    function ui_saveButton_clicked () {
        var projectFile = app.project.file;
        //todo prompt for destination and save
        if ( !projectFile ) {
            alert( 'Please save this project with After Effects once before using Ramses.' );
            return;
        }
        Ramses.save();
    }

    function ui_saveIncrementalButton_clicked () {
        var projectFile = app.project.file;
        if ( !projectFile ) {
            alert( 'Please save this project with After Effects once before using Ramses.' );
            return;
        }
        Ramses.saveIncremental();
    }

    function ui_publishButton_clicked () {
        var projectFile = app.project.file;
        if ( !projectFile ) {
            alert( 'Please save this project with After Effects once before using Ramses.' );
            return;
        }
        Ramses.publish();
    }

    function ui_cleanButton_clicked () {
        DuAEF.DuAE.Project.collectFiles( undefined, false, true);
    }


    // _______ UI SETUP _______
    var ui = DuAEF.DuScriptUI.createUI( thisObj, DuAEF.scriptName );

    var ui_buttonsGroup = DuAEF.DuScriptUI.addGroup( ui.contents );
    var ui_bottomLine = DuAEF.DuScriptUI.addGroup( ui.contents );

    // ============ UI CONTENT =================

    // Save
    ui_saveButton = DuAEF.DuScriptUI.addButton( ui_buttonsGroup, '', DuAEF.DuBinary.toFile( w25_save2_l ), 'Save project', DuAEF.DuBinary.toFile( w25_save2_r ) );
    ui_saveButton.alignment = [ 'left', 'top' ];
    // Save New Version
    ui_saveIncrementalButton = DuAEF.DuScriptUI.addButton( ui_buttonsGroup, '', DuAEF.DuBinary.toFile( w25_saveinc_l ), 'Save project and increment version', DuAEF.DuBinary.toFile( w25_saveinc_r ) );
    ui_saveIncrementalButton.alignment = [ 'left', 'top' ];
    // Publish
    ui_publishButton = DuAEF.DuScriptUI.addButton( ui_buttonsGroup, '', DuAEF.DuBinary.toFile( w25_publish_l ), 'Validate and publish project', DuAEF.DuBinary.toFile( w25_publish_r ) );
    ui_publishButton.alignment = [ 'left', 'top' ];
    // Clean
    ui_cleanButton = DuAEF.DuScriptUI.addButton( ui_buttonsGroup, '', DuAEF.DuBinary.toFile( w25_broom_l ), 'Cleans the version folder', DuAEF.DuBinary.toFile( w25_broom_r ) );
    ui_cleanButton.alignment = [ 'left', 'top' ];
    // Version
    ui_versionLabel = DuAEF.DuScriptUI.addStaticText( ui_bottomLine, 'v' + DuAEF.scriptVersion + ' | rainboxlab.org' );
    ui_versionLabel.alignment = [ 'left', 'bottom' ];

    // ========== CONNECT EVENTS ===============

    ui_saveButton.onClick = ui_saveButton_clicked;
    ui_saveIncrementalButton.onClick = ui_saveIncrementalButton_clicked;
    ui_publishButton.onClick = ui_publishButton_clicked;
    ui_cleanButton.onClick = ui_cleanButton_clicked;

    //Show UI
    DuAEF.DuScriptUI.showUI( ui );

    DuAEF.enterRunTime();

} )( this );
