![META](authors:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2021-2022;updated:2022/11/20)

# Ramses Application Changelog

## 0.7.0-Beta

### New

- New syncing methods with the *Ramses Server* which are slower but more reliable, especially with big sets of data.

### Improvements

- Performance improvements, especially with big sets of data

### Fixes

- Fixed potential crash when loading some items from the *Ramses Scripting API*.
- Fixed potential crash when adding entries to the schedule.
- Fixed potential crash when loading specific databases.
- Fixed schedule entries which may not be correctly shown. No data was lost, it was just a display issue.

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