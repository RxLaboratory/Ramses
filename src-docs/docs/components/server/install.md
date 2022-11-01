![META](authors:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2021-2022;updated:2022/11/01)

# Ramses Server installation

*Ramses Server* runs on very standard web servers, with the usual Web Server / *PHP* / *mySQL* stack. That means you can install it on almost any desktop or server computers, private server, shared hosting, workstation, Raspberry... It doesn't need much resources.

!!! warning
    The procedure to install and to update *Ramses* differ a bit. **Do not** follow the *installation* procedure to *update* your server, as it will wipe all your data!

    It is also a good habit to always backup the data before updating.

## Prerequisites

*Ramses Server* must be installed on a computer with:

- An **http web server** like *Apache*
- ***PHP 7***
- *Optionally*, an acces to a ***mySQL*** server and a dedicated database.
- An *SSL* certificate. Although this is not mandatory, it is **strongly** recommended to always encrypt your connections to the server, especially if the server is publicly available on the internet.

The recommended configuration is a standard *AMP* stack (*Apache* - *mySQL* - *PHP*), with at least 2GB of RAM if the *SQL* server is on the same computer than the *Apache* server.

This can easily be automated using [*Docker*](https://www.docker.com/) if you already have it installed. We provide a ready-to-use *Docker folder* to help you quickly install and run the server.

## Installation

You first have to decide what type of database the *Ramses Server* will be using. There are two options:

- ***MySQL*** or any other compatible server like *MariaDB*.  
	Use this if there will be a lot of users connected simultaneously to *Ramses*, and if you already know how to configure an SQL database (or you're ready to learn).
- ***SQLite*** is a simpler database, self-contained in a single file handled by the *Ramses Server*.
	Use this if there will be just a few users connected simultaneously to *Ramses*, or if you want a server easier to setup and backup.

Once you've made your choice, follow these guides according to your environement:

▹ **[Install from source](install-src.md)**

▹ **[Docker (MySQL)](install-docker-mysql.md)**

▹ **[Docker (SQLite)](install-docker-sqlite.md)**

You can [adjust the server configuration](config.md) by editing `config/config.php` and `config/config_logs.php`.