![META](authors:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2021-2022;updated:2022/11/20)

# Ramses Server Changelog

## 0.8.3-Beta

- Fixed character encoding issues.

## 0.8.2-Beta

- The automatic database cleaner now runs some tasks on the schedule table too.
- Fixed a bug in the database auto-cleaner, which may throw an error in some specific cases.

## 0.8.1-Beta

- Fixed a nasty bug in the sync process which prevented the correct sync between the applications.

## 0.8.0-Beta

- Regularly cleans the database, to keep it smaller and more performant, as well as resolve corrupted data issues which may happen during failed sync or using incompatible clients.
- Added a "maintenance mode".
- Fixed an error when pulling a single object data.

## 0.7.0-Beta

Starting with this version, the *Ramses Server* is only compatible with versions of MySQL more recent than 8.0, though it can still use SQLite too.

### New

- New syncing methods which are slower but more reliable, especially with big sets of data.
- Improved performance with big sets of data.
- When using SQLite, 'VACUUM' is run at regular intervals to optimize the size of the file and fix potential issues with failed queries.
- Refuse client connexions if they're using the wrong version.

### Fixes

- Accepts requests with missing 'Content-Type' header. Assumes it's *JSON* by default.

## 0.6.0-Beta

### New

- We now provide tested *Docker* configurations to easily install and run *Ramses Docker containers*.
- Each instance of a *Ramses Server* now had a unique universal identifier (UUID) to make sure a client won't connect to a wrong server by mistake.
- Added new server-side logs.
- The *login* requests now returns details (data) about the connected user (if login is successful).
- Ability to deactivate the session timeout.

### Fixes

- Fixed the user table in case a (removed) username already exists when creating a new user.
- Fixed installation issues when using *SQLite*.

## 0.5.1-Beta

### New

- Added (debug) logs to the replies which can be shown by clients.

### Fixes

- Fixed issues when table prefixes start with a number.
- Fixed connection issues when the URL ends with a /
- Ignore some sync errors when not in Dev mode

## 0.5.0-Beta

### New

- Changed and simplified data exchange.
- Added step general settings

## 0.3.0-Alpha

### New

- The Rubika Flavor is merged into the official add-on.
- Added update command
- Added publish settings
- Added import and update settings