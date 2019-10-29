#include "Ramses_required/Ramses_license.jsxinc"

//encapsulate the script in a function to avoid global variables
( function ( thisObj ) {

    //=============== INCLUDES ================

    #include DuAEF.jsxinc
    DuAEF.init("Ramses", "0.0.2");

    #include 'Ramses_required/Ramses_w25icons.jsxinc'
    #include 'Ramses_required/Ramses_w14icons.jsxinc'

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
    //DuAEF.debug = settings.data.debugMode;
    DuAEF.debug = false;

    //set zip path explicitly so it works both in dev and deployed as a single file
    if (DuAEF.win) {
        DuAEF.Zip.processPath = new File($.fileName).parent.absoluteURI + "/7za.exe";
    }


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

    function ui_archiveCompsButton_clicked () {
        var projectFile = app.project.file;
        //todo prompt for destination and save
        if ( !projectFile ) {
            alert( 'Please save this project with After Effects once before using Ramses.' );
            return;
        }
        Ramses.archive( true, ui_aCompZipCheckBox.checked);
        ui_aComp_window.hide();
    }

    function ui_archiveProjectButton_clicked () {
        var projectFile = app.project.file;
        //todo prompt for destination and save
        if ( !projectFile ) {
            alert( 'Please save this project with After Effects once before using Ramses.' );
            return;
        }
        Ramses.archive( false, ui_aProjectZipCheckBox.checked);
        ui_aProject_window.hide();
    }

    function ui_aFolderCancelButton_clicked() {
        ui_aFolder_window.hide();
    }

    function ui_aCompCancelButton_clicked() {
        ui_aComp_window.hide();
    }

    function ui_aProjectCancelButton_clicked() {
        ui_aProject_window.hide();
    }

    function ui_aFolderOKButton_clicked() {

        var inputFolder = ui_aFolderInputSelector.getFolder();
        if (!inputFolder) {
            alert("Incorrect input folder, please check its path.");
            return;
        }

        var outputFolder = ui_aFolderOutputSelector.getFolder();
        if (!outputFolder) {
            alert("Incorrect output folder, please check its path.");
            return;
        }

        var archiveType = DuAEF.Ramses.ArchiveModes.ONE_FOR_ALL;
        if (ui_aFolderGroupedSelector.index == 0) archiveType = DuAEF.Ramses.ArchiveModes.ONE_PER_AEP;
        else if (ui_aFolderGroupedSelector.index == 1) archiveType = DuAEF.Ramses.ArchiveModes.ONE_PER_FOLDER;
        Ramses.archiveFolder(
            inputFolder,
            outputFolder,
            ui_aFolderRecursiveCheckbox.checked,
            ui_aFolderZipCheckbox.checked,
            ui_aFolderReduceCheckbox.checked,
            archiveType
            );
    }

    // _______ UI SETUP _______
    var ui = DuAEF.DuScriptUI.createUI( thisObj, DuAEF.scriptName );

    var ui_buttonsGroup = DuAEF.DuScriptUI.addGroup( ui.contents );
    var ui_bottomLine = DuAEF.DuScriptUI.addGroup( ui.contents );

    // ============ UI CONTENT =================

    // MAIN PALETTE

    // Save
    var ui_saveButton = DuAEF.DuScriptUI.addButton( ui_buttonsGroup, '', DuAEF.DuBinary.toFile( w25_save2_l ), 'Save project', DuAEF.DuBinary.toFile( w25_save2_r ) );
    ui_saveButton.alignment = [ 'left', 'top' ];
    // Save New Version
    var ui_saveIncrementalButton = DuAEF.DuScriptUI.addButton( ui_buttonsGroup, '', DuAEF.DuBinary.toFile( w25_saveinc_l ), 'Save project and increment version', DuAEF.DuBinary.toFile( w25_saveinc_r ) );
    ui_saveIncrementalButton.alignment = [ 'left', 'top' ];
    // Publish
    var ui_publishButton = DuAEF.DuScriptUI.addButton( ui_buttonsGroup, '', DuAEF.DuBinary.toFile( w25_publish_l ), 'Validate and publish project', DuAEF.DuBinary.toFile( w25_publish_r ) );
    ui_publishButton.alignment = [ 'left', 'top' ];
    // Archive comps
    var ui_archiveCompsGroup = DuAEF.DuScriptUI.addGroup(ui_buttonsGroup, 'column');
    ui_archiveCompsGroup.alignment = [ 'left', 'top' ];
    var ui_archiveCompsButton = DuAEF.DuScriptUI.addButton(
        ui_archiveCompsGroup,
        '',
        DuAEF.DuBinary.toFile( w25_archivecomp_l ),
        "Exports selected compositions (or all if there's no selection)",
        DuAEF.DuBinary.toFile( w25_archivecomp_r )
        );
    var ui_archiveCompOptionsButton = DuAEF.DuScriptUI.addButton(
        ui_archiveCompsGroup,
        '',
        DuAEF.DuBinary.toFile( w14_plus_m ),
        "Options",
        DuAEF.DuBinary.toFile( w14_plus_r )
        );
    // Archive project
    var ui_archiveProjectGroup = DuAEF.DuScriptUI.addGroup(ui_buttonsGroup, 'column');
    ui_archiveProjectGroup.alignment = [ 'left', 'top' ];
    var ui_archiveProjectButton = DuAEF.DuScriptUI.addButton(
        ui_archiveProjectGroup,
        '',
        DuAEF.DuBinary.toFile( w25_archiveproject_l ),
        "Archives the current project, reducing it using the compositions at the root of the project.",
        DuAEF.DuBinary.toFile( w25_archiveproject_r )
        );
    var ui_archiveProjectOptionsButton = DuAEF.DuScriptUI.addButton(
        ui_archiveProjectGroup,
        '',
        DuAEF.DuBinary.toFile( w14_plus_m ),
        "Options",
        DuAEF.DuBinary.toFile( w14_plus_r )
        );
    // Archive folder
    var ui_archiveFolderButton = DuAEF.DuScriptUI.addButton(
        ui_buttonsGroup,
        '',
        DuAEF.DuBinary.toFile( w25_archivefolder_l ),
        "Archives a folder, collecting files for all AEPs found in the folder.",
        DuAEF.DuBinary.toFile( w25_archivefolder_r )
        );
    ui_archiveFolderButton.alignment = [ 'left', 'top' ]; 
    // Version
    var ui_versionLabel = DuAEF.DuScriptUI.addStaticText( ui_bottomLine, 'v' + DuAEF.scriptVersion + ' | rainboxlab.org' );
    ui_versionLabel.alignment = [ 'left', 'bottom' ];

    // OPTION DIALOGS

    // Archive Folder Options
    var ui_aFolder_window = DuAEF.DuScriptUI.createPopup( );
    var ui_aFolder_contents = ui_aFolder_window.contents;
    var ui_aFolderInputSelector = DuAEF.DuScriptUI.addFolderSelector(
        ui_aFolder_contents,
        "Input...",
        true,
        "Select the input folder containing the AEP files to archive."
        );
    var ui_aFolderOutputSelector = DuAEF.DuScriptUI.addFolderSelector(
        ui_aFolder_contents,
        "Output...",
        true,
        "Select the output folder where to archive the projects."
        );
    var ui_aFolderReduceCheckbox = DuAEF.DuScriptUI.addCheckBox(
        ui_aFolder_contents,
        "Reduce projects",
        undefined,
        "Reduces the project before collecting files, using all comps found at first level in the project (not in subfolders).",
        undefined
        );
    var ui_aFolderRecursiveCheckbox = DuAEF.DuScriptUI.addCheckBox(
        ui_aFolder_contents,
        "Include subfolders",
        undefined,
        "Search for AEP files in subfolders too.",
        undefined
        );
    var ui_aFolderZipCheckbox = DuAEF.DuScriptUI.addCheckBox(
        ui_aFolder_contents,
        "Compress archives (Zip)",
        undefined,
        "Compress each project archive in a zip file.",
        undefined
        );
    var ui_aFolderGroupedSelector = DuAEF.DuScriptUI.addSelector( ui_aFolder_contents );
    ui_aFolderGroupedSelector.addButton(
        "One archive per AEP",
        undefined,
        "One archive for each project found.",
        undefined
        );
    ui_aFolderGroupedSelector.addButton(
        "One archive per folder",
        undefined,
        "One archive for each sub-folder found, containing all AEP from the subfolder. All footage for projects of the same folder will be shared when possible",
        undefined
        );
    ui_aFolderGroupedSelector.addButton(
        "One archive for all",
        undefined,
        "One archive for all projects. All footage will be shared when possible.",
        undefined
        );
    var ui_aFolderValidButtons = DuAEF.DuScriptUI.addGroup( ui_aFolder_contents );
    var ui_aFolderCancelButton = DuAEF.DuScriptUI.addButton(
        ui_aFolderValidButtons,
        'Cancel',
        DuAEF.DuBinary.toFile( w25_back_m ),
        '',
        DuAEF.DuBinary.toFile( w25_back_r )
        );
    var ui_aFolderOKButton = DuAEF.DuScriptUI.addButton(
        ui_aFolderValidButtons,
        'Archive Folder',
        DuAEF.DuBinary.toFile( w25_check_g ),
        "Archives a folder, collecting files for all AEPs found in the folder.",
        DuAEF.DuBinary.toFile( w25_check_r )
        );

    // Archive Comp Options
    var ui_aComp_window = DuAEF.DuScriptUI.createPopup( );
    var ui_aComp_contents = ui_aComp_window.contents;
    var ui_aCompZipCheckBox = DuAEF.DuScriptUI.addCheckBox(
        ui_aComp_contents,
        "Compress archives (Zip)",
        undefined,
        "Compress the project archive in a zip file.",
        undefined
        );
    var ui_aCompValidButtons = DuAEF.DuScriptUI.addGroup( ui_aComp_contents );
    var ui_aCompCancelButton = DuAEF.DuScriptUI.addButton(
        ui_aCompValidButtons,
        'Cancel',
        DuAEF.DuBinary.toFile( w25_back_m ),
        '',
        DuAEF.DuBinary.toFile( w25_back_r )
        );
    var ui_aCompOKButton = DuAEF.DuScriptUI.addButton(
        ui_aCompValidButtons,
        'Archive Compositions',
        DuAEF.DuBinary.toFile( w25_check_g ),
        "Archives selected compositions (or all if there's no selection)",
        DuAEF.DuBinary.toFile( w25_check_r )
        );
    // Archive Project Options
    var ui_aProject_window = DuAEF.DuScriptUI.createPopup( );
    var ui_aProject_contents = ui_aProject_window.contents;
    var ui_aProjectZipCheckBox = DuAEF.DuScriptUI.addCheckBox(
        ui_aProject_contents,
        "Compress archives (Zip)",
        undefined,
        "Compress the project archive in a zip file.",
        undefined
        );
    var ui_aProjectValidButtons = DuAEF.DuScriptUI.addGroup( ui_aProject_contents );
    var ui_aProjectCancelButton = DuAEF.DuScriptUI.addButton(
        ui_aProjectValidButtons,
        'Cancel',
        DuAEF.DuBinary.toFile( w25_back_m ),
        '',
        DuAEF.DuBinary.toFile( w25_back_r )
        );
    var ui_aProjectOKButton = DuAEF.DuScriptUI.addButton(
        ui_aProjectValidButtons,
        'Archive Project',
        DuAEF.DuBinary.toFile( w25_check_g ),
        "Archives current project",
        DuAEF.DuBinary.toFile( w25_check_r )
        );
    // ========== INIT =========================
    ui_aFolderRecursiveCheckbox.setChecked(true);
    ui_aFolderReduceCheckbox.setChecked(true);
    ui_aFolderGroupedSelector.setCurrentIndex(0);
    
    // ========== CONNECT EVENTS ===============

    ui_saveButton.onClick = ui_saveButton_clicked;
    ui_saveIncrementalButton.onClick = ui_saveIncrementalButton_clicked;
    ui_publishButton.onClick = ui_publishButton_clicked;
    ui_archiveCompsButton.onClick = ui_aCompOKButton.onClick = ui_archiveCompsButton_clicked;
    ui_archiveProjectButton.onClick = ui_aProjectOKButton.onClick = ui_archiveProjectButton_clicked;
    ui_aFolder_window.tieTo( ui_archiveFolderButton );
    ui_aComp_window.tieTo( ui_archiveCompOptionsButton );
    ui_aProject_window.tieTo( ui_archiveProjectOptionsButton );
    ui_aFolderCancelButton.onClick = ui_aFolderCancelButton_clicked;
    ui_aCompCancelButton.onClick = ui_aCompCancelButton_clicked;
    ui_aProjectCancelButton.onClick = ui_aProjectCancelButton_clicked;
    ui_aFolderOKButton.onClick = ui_aFolderOKButton_clicked;

    //Show UI
    DuAEF.DuScriptUI.showUI( ui );

    DuAEF.enterRunTime();

} )( this );
