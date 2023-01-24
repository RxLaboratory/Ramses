![META](authors:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2021-2022;updated:2022/11/20)

# Ramses Maya Add-on Changelog

## 0.8.6-Beta

- Added color management options to the scene setup
- Added support for *OBJ* import/export

## 0.8.5-Beta

- Added "Recent Files" to the "Open" dialog
- Added tooltips showing the file name in the "Open" dialog
- Fixed publishing while removing some shapes which would remove children nodes

## 0.8.4-Beta

- Fixed recursion issue with some publish settings

## 0.8.3-Beta

- Fixed update/import error

## 0.8.2-Beta

- Fixed asset updater not finding some nodes

## 0.8.1-Beta

- Fixed incorrect pixel aspect ratio after setting the resolution
- Fixed error when import settings are malformed

## 0.8.0-Beta

- Fixed normals flipped in some cases when freezing geometry during the publishing process.

## 0.7.0-Beta

- Fixed 'save scene as' failing with random items.

## 0.6.0-Beta

- Fixed 'save scene as' with incorrect step settings

## 0.5.2-Beta

- Fixed 'save scene as' when the corresponding step doesn't have any general setting

## 0.5.1-Beta

- Removed duplicate formats in import settings
- Don't add selection to sets during scene setup
- Fixed publish with missing shaders

## 0.5.0-Beta

- Completely changed (and simplified) the server API to support syncing with clients.
- Warning: this update is not compatible with older versions. As we're moving from Alpha to Beta test, there's no way to migrate your data when updating this time, as we were just testing the framework. Now that we're in Beta test, this is fixed and future updates won't break compatibility anymore.

## 0.3.0-Alpha

- Improved security. Warning: new clients are not compatible with older servers, you have to upgrade both. The update will create/update a new Admin user with the password “password” and you’ll have to update all passwords for all users from that account to re-enable their accounts.
- The schedule now has a dedicated line to add notes/comments.
- Added publish settings to steps, to be used with add-ons.