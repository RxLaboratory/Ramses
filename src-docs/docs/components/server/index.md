![META](authors:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2021;updated:2022/08/23)

# The Ramses Server

The server stores and serves all shared data to the *Ramses Application* between all workstations in the studio. The *Ramses Applications* installed on these workstations connect to it to sync the data they need.

You can either [install your own](../components/server/install.md) or rent a server managed by *RxLaboratory* (or any other provider you trust).

It is just a small software meant to be installable on small hardware and with very low requirements. You can install it on any computer in the studio (even just a simple [*Raspberry*](https://www.raspberrypi.org/)) and make it available on your *Local Area Network*, or make it available from the internet. You can also easily install it on a shared or private hosting, even a low-cost one.

## Requirements

### Required Software

The *Ramses Server* must be installed on a computer with:

- An **http(s) web server** like *Apache*
- ***PHP 7***

### Optional Software

- An acces to a ***mySQL*** or ***MariaDB*** or any other compatible server and a dedicated database.

Without such a database server, the *Ramses Server* can still be run correctly (it will use an *SQLite* database which doesn't need any particular server), but performance may be poor with too many different concurrent users.

- An ***SSL*** certificate. Although this is not mandatory, it is **strongly** recommended to always encrypt your connections to the server, especially if the server is publicly available on the internet.

### Recommended configuration

The recommended configuration is a standard *AMP* stack (*Apache* - *mySQL* - *PHP*), with at least 2GB of RAM if the *SQL* server is on the same computer than the *Apache* server.

- The needed space for the *Ramses Server* files (*PHP*) is only *`200 KB`*!
- If you're not using a *mySQL* database, you'll need more space to store the data directly with the server files. How much space depends on the size of the projects you'll work on, but it will never be huge. *`100 MB`* should be more than enough in most cases.
- If you're using a *mySQL* database, *`100 MB`* should be more than enough for the vast majority of users.

## Technical description

The *Ramses Server* is a pretty standard *REST API* written in *PHP* which just serves as an interface to an *SQL* database. It receives requests from the *Ramses Client* and responds with the needed data.

For developers who may want to extend or modify *Ramses*, or develop their own client, [the complete reference of this *API* is available here](../../dev/server-reference/index.md).