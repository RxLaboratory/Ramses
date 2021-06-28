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

## priority

- update ramstatus: published & assigned user
    - [ ] server doc (setstatus, project return, update status)
    - SERVER DOC: comment in getters

- FEATURE: [-] status & history
    - [ ] A RamObjectListMenu which inherits QMenu, with a "checkable" option
    - [ ] File tabs in statuseditwidget: main, versions, preview, published, with filters by resource. Open button (and double click), and create new from template.
    - [ ] Implement Daemon status methods (updateShotStatus, updateAssetStatus, publishAsset, publishShot)   
- FEATURE: list assets in shots
- FEATURE: sequence order
- FEATURE: schedule & time tracking
    - [ ] Implement step color
    - [ ] Implement server-side default estimations for (template)steps
    - [ ] Server side: a userschedule table with columns userId, stepId, date, comment
    - [ ] Implement RamObject: RamScheduleItem
    - [ ] Implement status estimations (with default values computed from step rules)
    - [ ] Implement time tracking per status
    - [ ] Implement Schedule page
        - Option to set hours per day
        - Option to set working days (mon, tue, etc)
        - Count how many users are needed according to project deadline
        - Count how many days have to be assigned

## Other

- FEATURE: User Area
    - Day schedule
    - What's changed (notifications)
- FEATURE: [-] status & history
    - [ ] batch change
    - [ ] new filter in tables for only status assigned to the user
    - [ ] new filter in tables by states
    - [ ] Custom filter in user box to get only users assigned to the step (use ramobject filteruuid as a qstringlist)
- FEATURE: Implement automations (if published & right file format available -> set next step to TODO)
- FEATURE: if user is not admin, list only projects he's assigned to
- FEATURE: Tools tab with
    - File tools:
        - Clean Tree: Remove non tracked asset/shot dirs, etc (collect in a trash)
        - Create assets/shots from tree
- FEATURE: SSL: check if avail and adjust settings page
- FEATURE: shortName and name sanitation (Ramses.validateName && Ramses.validateShortName, and server side too)
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
- REFACTOR: factorize stylesheet creation to be used per widget
- DOC: update json example in getProject(s) server reference
- SERVER: post in content as JSON?
- DOC: add meta to the doc
- DOC: addon api: add open function description
- TESTS: User rights
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