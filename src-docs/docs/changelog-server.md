![META](authors:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2021-2022;updated:2022/11/01)

# Ramses Server Changelog

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