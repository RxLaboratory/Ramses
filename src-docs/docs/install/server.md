# Ramses Server installation

*Ramses Server* runs on very standard web servers, with the usual Web Server / PHP / mySQL stack. That means you can install it on almost any desktop or server computers, private server, shared hosting, workstation, Raspberry... It does not need much resources.

[TOC]

## Prerequisites

*Ramses Server* must be installed on a computer with:

- A **web server**
- ***PHP 7***

It also needs an acces to a ***mySQL*** database.

The recommended configuration is a standard AMP stack (*Apache* - *mySQL* - *PHP*), with at least 4GB of RAM if the *SQL* server is on the same computer.

!!! note
    As *Ramses* is still in development, the only compatible SQL database is *mySQL*. We may add support for other databases in the future.

## Installation

1. Create a new base on the *SQL* server, along with its user and password.
2. Copy the *Ramses Server* files to the web server.
3. Edit `config.php` with needed info.
4. Load `install.php`.
5. Delete `install.php` from the server.