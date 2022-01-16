![META](authors:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2021;updated:2021/10/24)

# Ramses Server update

!!! warning
    The procedure to install and to update *Ramses* differ a bit. **Do not** follow the installation procedure to update your server, as it will wipe all your data!

    It is also a good habit to always backup the data before updating.

Before updating, **it is always a good habit to backup your data!**

1. __Backup the server files (php)__: download them or move them to another folder. If you're using *SQLite*, the database is included in these files, it is called *ramses_data*.
2. __Backup the data if you're using a *MySQL* server__. Use *phpMyAdmin* to export the database, or any backup provided by your web hosting provider.
3. __Keep a copy of all files which names start with *config* (there should be: *config.php*, *config_security.php*, *config_logs.php*)__ in a safe place! If you lose the information from these files, you'll permanently lose access to part or all of your data, and nobody will ever be able to retrieve it. But be careful, if anyone gets access to them, they will be able to access and decrypt your data without you knowing it.

With this copy of your server, you'll be able to easily retrieve your data or revert to the previous version if anything goes wrong.

The safe update procedure is as follow:

1. __Un-Zip__ the *Ramses Server update package* you've downloaded.
2. __Download the current *config.php*, *config_security.php* and *config_logs.php* from your server__ and keep them safe.
3. __Update the new version of *config.php*__ with the same information from the one you've just retrieved.
3. __Update the new version of *config_logs.php*__ with the same information from the one you've just retrieved.
2. Note that it's a good habit to keep a backup copy of these files offline.
3. __Upload all files and folders__ from the update package to your server, __except the folder named *install*__.
4. __Go to http://yourserver.com/ramses/update__ (of course, replace the first part of this URL with your actual server address). The page may take some time to load.
5. You can then __remove the *update* and *install* folder__ from the server, everything is ready.

!!! note
    In the future, when *Ramses* won't be in a test version anymore, there should be an automatic update procedure.