![META](authors:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2021;updated:2021/08/03)

# Ramses Server update

!!! warning
    The procedure to install and to update *Ramses* differ a bit. **Do not** follow the installation procedure to update your server, as it will wipe all your data!

    It is also a good habit to always backup the data before updating.

The safe update procedure is as follow:

1. Un-Zip the *Ramses Server update package* you've downloaded.
2. Remove all files from your server **except _config.php_**. Note that it's a good habit to keep a backup copy of this file offline.
3. Uplaod all files and folders from the update package to your server.
4. Go to http://yourserver.com/ramses/update (of course, replace the first part of this URL with your actual server address). The page may take some time to load.
5. You can then remove the *update* folder from the server, everything is ready.

!!! note
    In the future, when *Ramses* won't be in a test version anymore, there should be an automatic install procedure.