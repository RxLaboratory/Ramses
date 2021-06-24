- REFACTOR: server: enclose column names in `` & other improvements in SQL queries (refer to setAssetStatus)
- FEATURE: SSL: check if avail and adjust settings page

- client deployment process to write/document:
    - Ubuntu 18.04
    - Ubuntu 20.04
    - Win
    - Mac
    - build on older ubuntu with Qt 5.9 &openssl 1.0 /// and 20.04 with Qt 5.12(test!!) & openssl 1.1 (use 20.10 or 21.04?)
    - package 2 deb for both ubuntu versions / openssl 1.0 / 1.1
    - package 2 appimages for both ubuntu versions / openssl 1.0 / 1.1
    - add openssl in dependencies for the deb package / warn it must be avail with the appimage
    - win
    - mac

## pipeline tools first

- FEATURE: Asset & Shot table
    - [WIP] StatusEditTable(RamObjectList *items)  
        - Ability to sort steps and shots (in project editor only for steps)
        - add state filtering (in header of step columns?)
- FEATURE: add comment field on every object
- FEATURE: [-] status & history
    - [ ] Add a "published" info: a checkbox, but check in files too if published files are found and the version corresponds
    - [ ] Check version in files, alert when changing status if version is found but does not correspond
    - [ ] When creating an asset or a shot, create a "STB" or "TODO" (depending on dependencies) status in all histories ?
    - [ ] Add a "Ramses" admin user (which can't be removed) for automated tasks
    - [ ] Make sure TODO, STB, OK can't be removed
    - [ ] Implement automations (if published & right file format available -> set next step to TODO)
    - [ ] Implement Daemon status methods (updateShotStatus, updateAssetStatus, publishAsset, publishShot)
    - [ ] Add prop: assigned user (and use filters in tables)
- FEATURE: Step order
- FEATURE: step: use template file (store in step folder as "PROJECTID_G_stepID_template.ext", open with corresponding app (add versionning later).)
- DOC: addon api: add open function description
- DOC & FEATURE: addon api: when activated, check current file (and update info)

## Other

- UI: Pipeline editor
    - snap button with a magnet icon
    - add button to show/hide scrollbars
- REFACTOR: path metohds in each objects, not in Ramses
- FIX: project admin: create step & asset groups from template
- REFACTOR: Use SIGNAL and SLOT macros for all connections, to make debugging easier
- FEATURE: Tools tab with
    - File tools:
        - Clean Tree: Remove non tracked asset/shot dirs, etc (collect in a trash)
        - Create assets/shots from tree
- REFACTOR: ramloader with new uuid accessors
- SERVER: add property "dont remove" to some states and the ramuser
- FIX: when clearing RamObjectListWidget disconnect all objects, not just the lists (cf ItemTableWidget)
- PERFs: in ItemTableWidget and RamObjectListWidget use QTableWidgetItem data to store obj uuid, and remove all casts from widgets
- FEATURE: if user is not admin, list only projects he's assigned to
- BUGFIX: Crash on reload
- BUGFIX: Crash when changing shot sequence in shoteditwidget
- REFACTOR: create a objectListComboBox (project selector, filters, sequencebox in shots, asstgroups in assets)
- REFACTOR: replace the step combobox in StatusHistoryWidget by a new StepSelectorWidget (derived from a new objectListComboBox ?)
- REFACTOR: itemhistory as an uberlist of statushistory (one per step)
- UI: Display widgets display more info when resized
- REFACTOR:
    - QList<RamObject*> -> RamObjectList
        - [x] steps in project
            - [x] users in steps
            - [ ] users in project (uberlist)
            - [x] apps in steps
            - [ ] apps in project (uberlist)
- REFACTOR: factorize stylesheet creation to be used per widget
- UI: ShotEditWidget & AssetEditWidget: tabs for status / files
- BUG: pipeedit: filetype list not always updated
- DOC: update json example in getProject(s) server reference
- UI: move console to a dock widget
- sequence order
- set offline when token is set invalid (server timeout)
- FEATURE: mutliple shots addition
- DOC: add meta to the doc
- FEATURE: Pipe editor: when creating a pipe, select it after creation
- FEATURE: shortName and name sanitation (Ramses.validateName && Ramses.validateShortName, and server side too)
- FEATURE: implement shots table 
- FEATURE: implement assets table
- FEATURE: implement main table
- FEATURE: option to restore removed items (admin/lead only) (or permanently delete)
- FEATURE: implement offline mode
- FEATURE: implement import-export data.
    - Server side: https://www.tutorialspoint.com/php/perform_mysql_backup_php.htm // do it regularly ? option in config.php , protect sql dumps folder with htaccess (create it in install.php)
    - Client side: define a data structure (json?) // Copy the sqlite local db once a day
- FEATURE: option to choose tray icon
- TESTS: test remove from database then refresh client
- TESTS: test other modifications from database then refresh client
- TESTS: test using two clients simultaneously (from different virtual machines)

WIP 

- Server

- [x] sql db
- [x] server fix unassign
- [x] server doc WIP

- Client

- [x] dbi
- [x] RamShot
- [x] RamSequence shot list
- [x] RamProject shot(uuid) & removeshot(uuid)
- [x] ramses (get in gotsequences -> gotshots)

- UI

- [ ] shoteditwidget
- [ ] shotmanagerwidget
- [ ] doc

- Daemon -> get pipes
- pipelinewidget (pipe changed et newpipe) (pour le titre des connexions)