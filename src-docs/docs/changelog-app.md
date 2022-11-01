![META](authors:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2021-2022;updated:2022/11/01)

# Ramses Application Changelog

## Known Issues

- ***[#263](https://github.com/RxLaboratory/Ramses/issues/263)***: The client may crash when removing a project which is not empty.
- On ***Windows***, the tray icon may not be removed if the client crashes.
- On ***Mac OS***, the user specific settings (current project, location of the nodes in the pipeline editor, ...) may not be saved.
- On ***Linux***, interaction with the timeline may be limited (impossible to select a shot, or open its folder).

## 0.6.0-Beta

### New

- It is now possible to set the port to use to connect to the server.
- Database manager/cleaner (for administrators)

### Improvements

- A local database can now connect only to the server it was intended to.
- The application can now show the server logs.
- Better log and display of errors
- Set to offline in case of server errors.

### Fixes

- Fixed performance issues with the schedule.
- The resolution of the projects is now correctly saved.

## 0.5.0-Beta

### New

- Many UI and preformance improvements.
- Implemented the *Offline mode*
- Changed and simplified data exchange.
- Added step general settings

## 0.3.0-Alpha

### New

- Improved security. Warning: new clients are not compatible with older servers, you have to upgrade both. The update will create/update a new Admin user with the password “password” and you’ll have to update all passwords for all users from that account to re-enable their accounts.
- The Client shows the current RxLab’s funding status.
- The schedule now has a dedicated line to add notes/comments.
- Added publish settings to steps, to be used with add-ons.
- added a log level selector to the console.

### Improvements

- The status of the connexion (offline, connected, secured) is now shown on the status bar. A warning is logged when the connexion is not secured.
- Changing the file type of a step automatically updates the custom settings accordingly.