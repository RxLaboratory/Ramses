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

## Notes

- SI Rules for units: https://physics.nist.gov/cuu/Units/checklist.html

## priority

- doc: general TOC above page TOC
- doc: check padding and margins on duik doc

- [ ] server doc (setstatus, project return, update status, assignAsset, unassignAsset, project shotassets, setSequenceOrder)
- SERVER DOC: comment in getters
- server doc: request in post as json or form-encoded, except the query

- Build installer, add folder to PATH, add startup entry
- Finish / Test Maya addon

## second priority

- improve server security: see https://stackoverflow.com/questions/10916284/how-to-encrypt-decrypt-data-in-php for crypting names

## Other

- REFACTOR:
    - Move common stuff from delegates to a RamDelegate
    - Add a few static methods to draw progress, etc
- PERFS:
    - RamSchedule from RamObjectList with a QMap<QDateTime, RamScheduleEntry> ?
    - Batch queries for Item table (same as schedule batch methods)
    - Remove (lots of) signals to calls ; data should be computed as soon as updated, not on draw
- UI:
    - Fuzzy logic for lateness color in status
    - Fuzzy logic for completion color in steps
- FEATURE: General
    - RamApplication: set tray icon (dark/light/color)
    - shortName and name sanitation (Ramses.validateName && Ramses.validateShortName, and server side too)
    - SSL: check if avail and adjust settings page
    - if user is not admin, list only projects he's assigned to
    - compute latenessRatio both from actual timespent and schedule (mean? add an option?)
- FEATURE: Folder monitoring
    - Auto version / backup
- FEATURE: User Area
    - Day schedule
    - What's changed (notifications)
- FEATURE: status & history
    - "Asset" and "Shot" menu to add/remove item(s)
    - [ ] new filter in tables by states
- FEATURE: Pipeline
    - Implement automations (if published & right file format available -> set next step to TODO)
- FEATURE: Tools tab with
    - File tools:
        - Clean Tree: Remove non tracked asset/shot dirs, etc (collect in a trash)
        - Create assets/shots from tree
        - Empty trash (using optionnal date filter)
    - Data tools
        - Import / Export data
            - Server side: https://www.tutorialspoint.com/php/perform_mysql_backup_php.htm // do it regularly ? option in config.php , protect sql dumps folder with htaccess (create it in install.php)
            - Client side: define a data structure (json?) // Copy the sqlite local db once a day
        - List and manage/restore/delete removed objects
- FEATURE: implement offline mode
- FEATURE: Shot list:
    - set/update shot list from final cut XML
    - set/update shot list from csv
    - export shot list to csv
    - export shot list to final cut XML
    - mutliple shots addition
- UI: tooltip appearance
- UI: add status tips & tooltips on controls
- UI: move all folder display to the bottom of edit widgets
- UI: Asset & Shot table
    - add state filtering (in header of step columns?)
- UI: Pipeline editor
    - snap button with a magnet icon
    - add button to show/hide scrollbars
- UI: move console to a dock widget
- set offline when token is set invalid (server timeout)
- REFACTOR: factorize stylesheet creation to be used per widget
- DOC: update json example in getProject(s) server reference
- DOC: don't forget shortcuts (pipe, del, ctrl+f)
- DOC: Tree: _trash folders
- DOC: add meta to the doc
- DOC: addon api: add open function description
- DOC: addon api: add RamStatus.published / daemon.setStatus add published arg
- TESTS: test remove from database then refresh client
- TESTS: test other modifications from database then refresh client
- TESTS: test using two clients simultaneously (from different virtual machines)