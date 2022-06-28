![META](authors:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2021;updated:2021/07/12)

# Ramses - The Rx Asset Management System: Introduction

!!! warning
    **Ramses is still under active development**, and this documentation is still being written.

    We strongly invite you to read the [changelog](../changelog.md) to know what's going on, and especially to have a look at the [known issues](../changelog.md).

This first part of the *Ramses* documention intents to help you understand the general concepts and key features of *Ramses* without going into technical details. As *Ramses* is not made of one single application but of several components, the technical description and user guide of each piece of software is available individually in [the second part of this documentation](../components/index.md).

## Features

*Ramses* has three different main features, which are closely related and for which each component plays its role: [**Pipeline automation**](pipeline.md), [**Production Tracking**](production.md) and [**Production Planning**](planning.md), along with the global [**administration**](administration.md) of course.

▹ **[Administration](administration.md)**.

▹ **[Pipeline](pipeline.md)**.

▹ **[Production Tracking](production.md)**.

▹ **[Planning](planning.md)**.

## The components

![](/img/LAB_Ramses_Doc_Schemas.png)

*Ramses* is a complete framework made of several components, as you can see on the image above.

Each component has to be installed individually, and not all components are mandatory to use *Ramses*.

Here is the list of all the components along with a few notes about them.

| Component | Relations | Description | Technical Documentation |
| --- | --- | --- | --- |
| ***Ramses Server*** | Serves the data to the *Ramses Client*. | The server stores and serves all shared data between all workstations in the studio.<br />The *Ramses Clients* installed on these workstations connect to it to retrieve tha data they need. | ▹ [Read the doc](../components/server/index.md). |
| ***Ramses Client Application*** | Connects to the *Ramses Server* to share the data.<br />Serves the data that the *Ramses Add-ons* need to automate the pipeline. | The client application is the central node of the *Ramses* framework. All information is available through the application, and it is able to drive the *Ramses Add-ons* in the creative applications. Note that it can be used stand-alone in an *offline mode* without the server, for one-man bands. | ▹ [Read the doc](../components/client/index.md). |
| ***Ramses Add-ons*** | Connect to the *Ramses Client* to get the data they need to automate the pipeline. | The add-ons integrate *Ramses* into your favorite creative applications (3D or 2D Animation, designing, compositing software, etc.). They can automate your workflow, and they handle all the file versionning, while providing production tracking data to the *Ramses client*. They can also be used as stand-alone plugins, without any other *Ramses* component, at the cost of less production management features. | ▹ [Read the doc](../components/addons/index.md). |
| ***Ramses Tree*** | Read by the *Ramses Add-ons* and the *Ramses Client*. | The *Ramses Tree* is the way *Ramses* stores and sorts your files. Thanks to a carefully crafted file and folder naming scheme and conventions, both the *Ramses Add-ons* and the *Ramses Client* can efficiently handle all your files and data. | ▹ [Read the doc](../components/files/index.md). |

!!! note
    Although in the future the *Ramses Client* will be able to be used as a stand-alone, offline application, **the currently available *Alpha or Beta Versions* absolutely need a connection to a _Ramses Server_** (there's no offline storage of the data for now).

    But that doesn't mean that you absolutely need an online server to test and use *Ramses*: you can already use it on a single workstation by locally installing a simple webserver. Explanations and a step-by-step guide is available in [the server documentation](../components/server/index.md).

!!! Hint
    **_Ramses_ is not meant to be available _as a service_**: we believe you should stay in control of your own data without relying onto unknown people, even people as loving as us (and you won't have to pay mensual or annual fees just to use *Ramses*).

    Even if you're running your own small studio or tiny collective, it's very easy to install and use the *Ramses* server, it can be done on any computer running *Windows*, *Linux* or *Mac OS*, or better, on any low-cost shared hosting with just a very small available space and a small *SQL* database. All the installation procedure is explained in [the server documentation](../components/server/index.md).

    This being said, we (will soon) also have a few options for [**servers we can install and manage for you** at a very low cost](https://rxlaboratory.org/tools/ramses/).

## Installation

In order to use *Ramses*, you have to install the components you need individually, one after each other.

| Component | Installation notes |
| --- | --- |
| ***Ramses Server*** | Requires a standard *AMP* stack (*Apache*, *MySQL*, *PHP*). We provide a guide to install one locally:<br />▹ [AMP server installation](../components/server/web-server.md)<br />Then you can install the *Ramses server* easily:<br />▹ [Installation guide](../components/server/install.md) |
| ***Ramses Client*** | The client is a simple application, installed the same way as other applications:<br />▹ [Installation guide](../components/client/install.md) |
| ***Ramses Add-ons*** | Add-ons installation differs depending on the host application:<br />▹ [Maya installation](../components/addons/maya.md)<br />▹ Other hosts coming soon! |

!!! note
    Although in the future the *Ramses Client* will be able to be used as a stand-alone, offline application, **the currently available *Alpha or Beta Versions* absolutely need a connection to a _Ramses Server_** (there's no offline storage of the data for now).

    This means that for now you **have to** install the *Ramses Server* or have one available somewhere.
