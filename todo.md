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

- FEATURE: [-] status & history
    - [ ] Add a "published" info: a checkbox, but check in files too if published files are found and the version corresponds
    - [ ] Check version in files, alert when changing status if version is found but does not correspond
    - [ ] When creating an asset or a shot, create a "STB" or "TODO" (depending on dependencies) status in all histories ?
    - [ ] Make sure TODO, STB, OK can't be removed
    - [ ] Implement automations (if published & right file format available -> set next step to TODO)
    - [ ] Implement Daemon status methods (updateShotStatus, updateAssetStatus, publishAsset, publishShot)
    - [ ] Add prop: assigned user (and use filters in tables)
- FEATURE: list assets in shots
- FEATURE: sequence order
- FEATURE: step: use template file (store in step folder as "PROJECTID_G_stepID_template.ext", open with corresponding app (add versionning later).)
- DOC: addon api: add open function description
- DOC & FEATURE: addon api: when activated, check current file (and update info)

## Other

- SERVER: add property "dont remove" to some states and the ramuser
- FEATURE: if user is not admin, list only projects he's assigned to
- FEATURE: Tools tab with
    - File tools:
        - Clean Tree: Remove non tracked asset/shot dirs, etc (collect in a trash)
        - Create assets/shots from tree
- FEATURE: SSL: check if avail and adjust settings page
- FEATURE: shortName and name sanitation (Ramses.validateName && Ramses.validateShortName, and server side too)
- FEATURE: implement main table
- FEATURE: option to restore removed items (admin/lead only) (or permanently delete)
- FEATURE: implement offline mode
- FEATURE: implement import-export data.
    - Server side: https://www.tutorialspoint.com/php/perform_mysql_backup_php.htm // do it regularly ? option in config.php , protect sql dumps folder with htaccess (create it in install.php)
    - Client side: define a data structure (json?) // Copy the sqlite local db once a day
- FEATURE: option to choose tray icon
- FEATURE: mutliple shots addition
- UI: Asset & Shot table
    - add state filtering (in header of step columns?)
- UI: Pipeline editor
    - snap button with a magnet icon
    - add button to show/hide scrollbars
- UI: move console to a dock widget
- set offline when token is set invalid (server timeout)
- REFACTOR: path metohds in each objects, not in Ramses
- REFACTOR: factorize stylesheet creation to be used per widget
- DOC: update json example in getProject(s) server reference
- SERVER DOC: comment in getters
- DOC: add meta to the doc
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