![META](authors:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2021-2022;updated:2022/11/01)

# Install from source

Installing from source is the easiest way to install the ramses server on any host, including shared hosting.

1. (*MySQL* only) Create a new base on your *SQL* server, along with its user and password.
2. Un-Zip the *Ramses Server* you've downloaded.
3. [Edit `config/config.php`](config.md) with all needed info.
2. Copy the *Ramses Server* files to the web server (you can just copy the `ramses` folder from the *ZIP* archive).
4. Go to `http://your.server/ramses/install`.
5. Delete the `install` folder from the server.

The default user for Ramses is `Admin` with the password `password`. You should start by changing this name and password using the [*Ramses Client Application*](../client/index.md).

!!! warning
    It is important to delete the `install` folder as soon as the server's been installed, to prevent anyone to run the install script again.

    When the install script is run, it wipes all existing data from the server's database!