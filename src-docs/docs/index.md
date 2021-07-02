# Ramses - The Rainbox Asset Management System

!!! warning
    Ramses is still under active development, and this documentation is still being written.  
    The first public Beta version will be available very soon.

![](/img/LAB_Ramses_Doc_Schemas.png)

## What's this?

*Ramses*, the Rx Asset Management System, is a complete framework to help you, your team, your studio, track the production of your films.

It is meant to be used both by freelancers, small or big teams.

Let's have a look at a few key features.

### ▹ Free and open source

*Ramses* is licensed under the GNU General Public License v3, which means **you're free** to do anything you'd like with it, use it the way you want, modify it, and share your modifications.

### ▹ Modular and scalable

*Ramses* is made of **several components which can be used either individually or as a whole framework**. If you just need an add-on to your creative software improve your workflow and versionning, you could use only on of the [*Ramses Add-ons*](addons/index.md). If you're interested in production tracking, you'll need the [*Ramses Client Application*](#) and maybe connect it to the [*Ramses Server*](#) to stay connected with your team.

Of course, each of these components can work together and share information for **a seamless integration into your workflow**.

### ▹ Planning

![](img/client/schedule.png)

*Ramses* helps you easily **plan your production**, and estimate how long you'll need to produce your project.

For each shot and asset of the project, you'll be able to (automatically) estimate how long it will take to produce, and *Ramses* will keep track of all estimations for you so you can plan accordingly.

### ▹ Production Tracking

![](img/client/assettracking.png)

At any time, *Ramses* can tell you where you are in the production, how late (or not) you are, and warn you if something's going wrong.

You can **always keep a close (or overall) look at the production**, and check which assets and shots are longer or shorter to produce, and the state in which they are at any time.

### ▹ Pipeline automation

![](img/client/pipeline.png)

In combination with the *Ramses Add-ons*, *Ramses* is able to **automate your pipeline**, managing the **versionning** of the files, the exports, imports and all these manual tasks a robot could do for you.

With a nice and modern nodal view, it's easy to design your pipeline, connecting each step and setting how the data should go from one step to the other. All this information will be used both by the *Ramses* production tracking algorithms and the *Add-ons* to help you **focus on your real work: creation**.

### ▹ (Very) Easy-to-use

A tool like *Ramses* is here to help, not to give you a headake.

All user interface components have been carefully crafted to be beautiful, but most of all to be **intuitive and practical**. Everything has to be clear, precise, and meaningful.

Of course, there's this comprehensive documentation if you have any doubt...

### ▹ And also...

- Why would one want to track one's production from a web browser? *Ramses* comes with its own **native application for _Linux_, _Mac OS_ and _Windows_**. It's designed to by very light and unintrusive, fast and responsive.  
It ususally takes about ten times less memory than a usual web-browser with a single tab opened.

- *Ramses* is **extensible**. Although it already comes with some add-ons for your creative software, if you have specific needs, you can use the provided [*Python* and *Adobe ExtendScript* API](dev/add-ons-reference/index.md), to implement your own *add-ons* which fully integrate with both *Ramses* and your creative applications. The [*Daemon*](dev/daemon-reference/index.md) and the [*Server*](dev/server-reference/index.md) API are also fully documented. And, after all, *Ramses* is open source!

- *Ramses* can **manage all your files**: the idea is that you won't ever need to search for any file through the system's explorer.

![](img/client/home.png)

## Table of Contents

- Introduction (this page)
- **The Pipeline**
    - [Administration](#)
    - [User Management](#)
    - [Pipeline](#)
    - [Production management](#)
    - [Panning and time tracking](#)
- **Ramses Components**
    - [Introduction](components/index.md)
    - The Ramses server
        - [Installation](install/server.md)
        - [Administration](#)
    - The Ramses application (client)
        - [Installation and settings](#)
        - [Administration]
        - [Project settings](#)
        - [The pipeline](#)
        - [Assets and shots production management](#)
        - [Schedule and statistics](#)
    - Ramses Add-ons
        - [General information](addons/index.md)
        - [Add-on for Autodesk Maya](addons/maya.md)
        - [Add-on for Autodesk Maya - The Rubika Flavor](#)
    - The Ramses Tree
        - [Naming scheme](files/naming.md)
        - [Folder Structure](files/tree.md)
- **Extending Ramses, Development**
    - [Introduction](dev/index.md)
    - [Files and folders](dev/files/naming.md)
    - [The Scripting API (Add-ons)](dev/add-ons-reference/index.md)
    - [The Ramses Daemon API](dev/daemon-reference/index.md)
    - [The Ramses Server API](dev/server-reference/index.md)
- **Miscellaneous**
    - [Credits](credits.md)
    - [Changelog](changelog.md)
    - Licenses
        - [Software: GNU GPLv3](licenses/gnu-gpl.md)
        - [Documentation: GNU FDL](licenses/gfdl.md)
        - [Medias: CC BY-NC-SA 4.0](https://creativecommons.org/licenses/by-nc-sa/4.0/)
## License

### Software

The software this documentation is referring to is licensed under the ***GNU General Public License***.

<span style="font-size:0.75em;">**Copyright (C)  2020 Nicolas Dufresne and Contributors.**  
This program is free software; you can redistribute them and/or modify them under the terms of the GNU General Public License as published by the Free Software Foundation; either version 3 of the License, or (at your option) any later version.</span>

<span style="font-size:0.75em;">This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.</span>

<span style="font-size:0.75em;">You should have received a copy of the *GNU General Public License* along with this program . If not, see [http://www.gnu.org/licenses/](http://www.gnu.org/licenses/).</span>

![GNU](img/licenses/gnu.png){: style="height:75px;"} ![GPL3](img/licenses/gplv3.png){: style="height:50px;"}

### Icons and other graphical elements

All graphical elements, especially icons, are from [***RxUI***](http://rxui-docs.rainboxlab.org) by Nicolas Dufresne and [RxLaboratory](https://rainboxlab.org) are licensed under [CC BY-NC-SA 4.0](https://creativecommons.org/licenses/by-nc-sa/4.0/).

<span style="font-size:0.75em;">**Copyright (C) 2021 Nicolas Dufresne and Contributors.**

[![CC](https://mirrors.creativecommons.org/presskit/icons/cc.svg){: style="height:35px;"} ![BY](https://mirrors.creativecommons.org/presskit/icons/by.svg){: style="height:35px;"} ![NC](https://mirrors.creativecommons.org/presskit/icons/nc.svg){: style="height:35px;"} ![SA](https://mirrors.creativecommons.org/presskit/icons/sa.svg){: style="height:35px;"}](https://creativecommons.org/licenses/by-nc-sa/4.0/)

### This Documentation

<span style="font-size:0.75em;">**Copyright (C)  2020 Nicolas Dufresne and Contributors.**  
Permission is granted to copy, distribute and/or modify this document under the terms of the GNU Free Documentation License, Version 1.3 or any later version published by the Free Software Foundation;  
with no Invariant Sections, no Front-Cover Texts, and no Back-Cover Texts.
A copy of the license is included in the section entitled "[Documentation License](licenses/gfdl.md)".</span>

![GNU](img/licenses/gnu.png){: style="height:75px;"} ![GFDL](img/licenses/gfdl-logo.png){: style="height:50px;"}

![META](authors:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2021;updated:2021/06/10)
