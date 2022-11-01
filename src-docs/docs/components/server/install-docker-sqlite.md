![META](authors:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2021-2022;updated:2022/11/01)

# Docker install (SQLite)

Use the *Docker SQLite* folder for a quick and easy setup of the *Ramses Server*. This is the recommended way of using the *Ramses Server* in a development environement.

2. Un-Zip the *ramses-server_docker-sqlite* archive you've downloaded.
3. On *Linux*, you may need to adjust permissions to the unzipped folder to make sure *Docker* has write access in there.
2. Go inside the *docker folder* and use the command `docker compose up`.
4. Go to `http://localhost:8001/ramses/install`.
5. Delete the `install` folder from the docker folder.

The default user for Ramses is `Admin` with the password `password`. You should start by changing this name and password using the [*Ramses Client Application*](../client/index.md).

!!! warning
    It is important to delete the `install` folder as soon as the server's been installed, to prevent anyone to run the install script again.

    When the install script is run, it wipes all existing data from the server's database!

## Customize the *Docker* container

There are some settings you may find useful to adjust in the provided `docker-compose.yml` file before you run your container.

### Default port

By default, the Docker Ramses Server is set to use the `8001` port. It may be easier to use the default `80` port.

### Secured connection (SSL)

In production, it is recommended to add an SSL certificate to the Ramses Server and set `$forceSSL = true;` in the Ramses Server `config/config.php` file.

In this case, you can add this port mapping to the *www* service:  
`- "XXX:443"`  
Change `XXX` by the port you want to expose, which should be `443` by default.