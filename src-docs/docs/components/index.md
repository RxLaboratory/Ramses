# Ramses Components

*Ramses* is a complete framework made of different components interacting together. This makes it very modular and scalable to any kind of production, from the freelancer working at home to the biggest studios.

[TOC]

## General Description

The *Ramses* framework can be viewed as a hirerachical tree, going from the data stored by the *Ramses Server* up to the [creative applications add-ons](../addons/index.md) and the [files and folder structure](../files/naming.md) on the workstations.

- ***Ramses Data***: all the meta-data is stored by *Ramses* in an *SQL* server.
- ***Ramses Server***: this data is managed by a webserver, which reads and writes the data on demand from connected *Ramses Clients*.
    - ***Ramses Client***: the client is run on each workstation and is the central application and provides the main User Interface of *Ramses*.
    - ***Ramses Daemon***: the daemon is run by the client itself, and is a local server which is used as a bridge between creative applications and the client itself. It handles all communications and data with the applications add-ons.
        - ***Ramses Add-ons***: the add-ons are scripts to be used from inside the creative applications (e.g. *Blender*, *Maya*, *After Effects*...) so that the artist does not have to leave his application to use *Ramses*.
            - ***Ramses Tree***: on the lowest level, *Ramses* manages the files and folder structure on the workstation (and/or clones on remote locations).

The modularity of Ramses comes from the fact that *children* modules don't necessarily need the *parent* module, at least for their core functionnalities. For example, you can use the *Ramses Client* without the *Ramses Server and Data*, or you can even use the *Ramses add-ons* without the *Ramses Client*.  
But you can not skip levels, i.e. the *Ramses Add-ons* can not be used with the *Ramses Server* without having both the *Ramses Client* and *Ramses Daemon*.

## Ramses Data

Most of the data used by *Ramses* is stored in SQL databases. When using the *Ramses Server*, the main data is stored in a remote SQL server; in its default implementation, the *Ramses Server* works with *MySQL*.

The *Ramses Client* keeps a local copy of the data in a SQLite database.

## Ramses Server

The *Ramses Server* is the interface between the client and the data. It is meant only to be used programmatically and it does not have any UI. It is a very standard *REST* web server written in *PHP*, exchanging data in *JSON*.

This means the *Ramses Server* and its associated *Ramses Data* can be easily installed everywhere, on a workstation (with a standard web server), on a shared hosting or a private server, no matter the operating system. Installation of *Ramses* on a server is as simple as copying *php* files to the server, and adding a little information in the configuration file.

## Ramses Client and Daemon

The client is the main *Ramses* application. It can be used without the *Ramses Server*, without losing any functionnality, apart from collaborative work. In this case, the data is stored locally. When the server is not available (e.g. loss of internet connection), the *Ramses Client* can still work offline, and synchronize the data as soon as it comes online later.

The client is a native application (C++), and is available on *Linux*, *Windows* and *MacOS*, which makes it fast and lightweight. It may be available later for *Android*.

The client also runs the *Ramses Daemon* which is a local server which communicates with the *Ramses Add-ons* and shares the data with them.

## Ramses Add-ons

The [add-ons](../addons/index.md) are scripts available inside the creative applications (e.g. *Blender*, *Maya*, *After Effects*...). They could be used without the client, with limited functionnalities. In this case, data is retrieved from the *Ramses Tree*.

The add-ons are in charge of file management and versionning, publishing, etc.

## Ramses Tree

On the lowest level, *Ramses* handles all [files](../files/naming.md) and [folders](../files/tree.md) for the managed projects. It is able to retrieve missing data from the file structure, and missing files from clones on remote and backup locations.

Both the Add-ons and the Client can manage the files and folders.