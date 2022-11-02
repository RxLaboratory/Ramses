![META](authors:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2021;updated:2022/11/02)

# Ramses Server update

Before updating, **it is always a good habit to backup your data!**

1. __Backup the server files (php)__: download them or move them to another folder. If you're using *SQLite*, the database is included in these files, it is the `data/ramses_data` file.
2. __Backup the data if you're using a *MySQL* server__. Use *phpMyAdmin* to export the database, or any backup provided by your web hosting provider.
3. __Keep a copy of of the *config* folder__ in a safe place! If you lose the information from these files, you'll permanently lose access to part or all of your data, and nobody will ever be able to retrieve it. But be careful, if anyone gets access to them, they will be able to access and decrypt your data without you knowing it.

With this copy of your server, you'll be able to easily retrieve your data or revert to the previous version if anything goes wrong.

The safe update procedure is as follow:

1. __Un-Zip__ the *Ramses Server* you've downloaded.
2. __Download the current *config* folder from your server__ and keep it safe.
3. __Update the new version of *config/config.php*__ with the same information from the one you've just retrieved.
3. __Update the new version of *config/config_logs.php*__ with the same information from the one you've just retrieved.
3. __Upload all files and folders__ from the update package to your server, __except the folder named *install*__.

!!! note
    In the future, when *Ramses* won't be in a test version anymore, there should be an automatic update procedure.