![META](authors:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2021-2022;updated:2022/11/20)

# Ramses API Changelog

## 0.8.0-Beta

This version includes important changes in the API, for a better stability and compatibility with the 0.8.x framework. Read the API reference for more details.

- The `RamDaemonInterface` includes new useful methods to retrieve data.
- The `RamItem` class no longer have the `setStatus()` method. Status are now updated by just setting new values in the `RamStatus` object retrieved with `RamItem.currentStatus(step)`.
- The `RamItem` class no longer have the `stepStatusHistory()` method. Status histories are disabled for now.
- The `RamSequence` and `RamAssetGroup` classes now include a `shots()` and `assets()` methods to retrieve the shots and assets they contain.

## 0.7.0-Beta

- 0.7.x compatibility update.

## 0.6.0-Beta

- 0.6.x compatibility update.

## 0.5.0-Beta

- Changed and simplified data exchange.
- Added step general settings