![META](authors:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2021;updated:2021/08/03)

# Ramses Server update

!!! warning
    The procedure to install and to update *Ramses* differ a bit. **Do not** follow the installation procedure to update your server, as it will wipe all your data!

    It is also a good habit to always backup the data before updating.

The safest update procedure is as follow:

1. Un-Zip the *Ramses Server* you've downloaded.
2. Edit `config.php` with needed info (you can retrieve this info froñ the current file on your server).
3. Delete the `install` folder.
4. Delete all files from the server.
5. Copy the *Ramses Server* files to the web server.

!!! note
    In the future, when *Ramses* won't be in a test version anymore, there should be an automatic install procedure.