# Ramses

*Ramses*, the Rx Asset Management System, is a complete framework to help you, your team, your studio, to track the production of your films.

It is meant to be used both by freelancers, small or big teams.

**▹ [Read the doc on ramses.rxlab.guide](http://ramses.rxlab.guide)!**

**▹ [Developer guides are available on ramses.rxlab.io](http://ramses.rxlab.io).**

**▹ [Post your feedback, bug reports and feature requests in the issues](https://github.com/RxLaboratory/Ramses/issues/new/choose)!**

**▹ [Join us on chat.rxlab.info](http://chat.rxlab.info), we don't bite!**

## ▹ Free and open source

*Ramses* is licensed under the GNU General Public License v3, which means **you're free** to do anything you'd like with it, use it the way you want, modify it, and share your modifications.

## ▹ Modular and scalable

*Ramses* is made of **several components which can be used either individually or as a whole framework**. If you just need an add-on to your creative software to improve your workflow and versionning, you could use only one of the *Ramses Add-ons*. If you're interested in production tracking, you'll need the *Ramses Client Application* and maybe connect it to the *Ramses Server* to stay connected with your team.

Of course, each of these components can work together and share information for **a seamless integration into your workflow**.

This scalability makes *Ramses* a good choice for individuals as well as teams of all sizes.

- The [***Ramses Server***](https://github.com/RxLaboratory/Ramses-Server/tree/master) is a *PHP + MySQL* server (a *REST API*) *which stores the data and serves it to the *Ramses Client Applications* installed on the workstations.

- The [***Ramses Client Application***](https://github.com/RxLaboratory/Ramses-Client/tree/master) is a *Qt/C++* application run on each workstation and is the central application which provides the main User Interface of Ramses.

- The ***Ramses Add-ons*** are scripts to be used from inside the creative applications (e.g. Blender, Maya, After Effects…) so that the artist does not have to leave his application to use *Ramses*, and which provide versionning and production tracking tools.
    - They use the [***Ramses Python API***](https://github.com/RxLaboratory/Ramses-Py/tree/master) or the ***Ramses ExtendScript API*** (not available yet).
    - [***Ramses-Ae***](https://github.com/RxLaboratory/Ramses-Ae/tree/main) is the *Add-on* for *Adobe After Effects*.
    - [***Ramses-Blender***](https://github.com/RxLaboratory/Ramses-Blender/tree/main) is the *Add-on* for *Blender*.
    - [***Ramses-Maya***](https://github.com/RxLaboratory/Ramses-Maya/tree/main) is the *Add-on* for *Autodesk Maya*.

## ▹ Production Planning

![](http://ramses.rxlab.guide/img/client/schedule.png)

*Ramses* helps you easily **plan your production**, and estimate how long you'll need to produce your project.

For each shot and asset of the project, you'll be able to (automatically) estimate how long it will take to produce, and *Ramses* will keep track of all estimations for you so you can plan accordingly.

## ▹ Production Tracking

![](http://ramses.rxlab.guide/img/client/assettracking.png)

At any time, *Ramses* can tell you where you are in the production, how late (or not) you are, and warn you if something's going wrong.

You can **always keep a close (or overall) look at the production**, and check which assets and shots are longer or shorter to produce, and the state in which they are at any time.

## ▹ Pipeline automation

![](http://ramses.rxlab.guide/img/client/pipeline.png)

In combination with the *Ramses Add-ons* which integrate *Ramses* in your favorite creative software, *Ramses* is able to **automate your pipeline**, managing the **versionning** of the files, the exports, imports and all these manual tasks a robot could do for you.

<figure>
  <img src="http://ramses.rxlab.guide/img/maya/shelf.png"/>
  <figcaption>An example: the Autodesk Maya shelf of the Ramses Maya Add-on.</figcaption>
</figure>

With a nice and modern nodal view, it's easy to design your pipeline, connecting each step and setting how the data should go from one step to the other. All this information will be used both by the *Ramses* production tracking algorithms and the *Add-ons* to help you **focus on your real work: creation**.

## ▹ (Very) Easy-to-use

A tool like *Ramses* is here to help, not to give you a headache.

All user interface components have been carefully crafted to be beautiful, but most of all to be **intuitive and practical**. Everything has to be clear, precise, and meaningful.

Of course, there's this comprehensive documentation if you have any doubt...

## ▹ And also...

- Why would one want to track one's production from a web browser? *Ramses* comes with its own **native application for _Linux_, _Mac OS_ and _Windows_**. It's designed to by very light and unintrusive, fast and responsive.  
It ususally takes about ten times less memory than a usual web-browser with a single tab opened.

- *Ramses* is **extensible**. Although it already comes with some add-ons for your creative software, if you have specific needs, you can use the provided [*Python* and *Adobe ExtendScript* API](dev/add-ons-reference/index.md), to implement your own *add-ons* which fully integrate with both *Ramses* and your creative applications. The [*Daemon*](dev/daemon-reference/index.md) and the [*Server*](dev/server-reference/index.md) API are also fully documented. And, after all, *Ramses* is open source!

- *Ramses* can [**manage all your files**](#): the idea is that you won't ever need to search for any file through the system's explorer.

## ▹ The future

**_Ramses_ is (far from) not finished**. And probably never will: it is going to evolve, and there are a lot of other planned features and improvements on their way!

**▹ [Read the changelog for more details about what's going on, and known issues](http://ramses.rxlab.guide/changelog/)**
