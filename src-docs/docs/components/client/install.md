![META](authors:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2021;updated:2021/07/13)

# ![](/img/icons/ramses-icon-large.svg){: style="width:55px;"} Application installation

The method to install Ramses depends on your operating system; no matter what, it is installed as any other standard application.

As soon as the application is installed and running, go to the **[settings](settings.md)** to configure it.

## Linux

On __*Debian*-based distributions__, like __*Ubuntu*__ for example, the easiest way to install the *Ramses Client Application* is to download and install the `.deb` package, which will also install all dependencies, using your usual package manager / installer.

There are two different version which depend on the version of *openSSL* (either *1.0* or *1.1*) and the *Qt* library (either *5.9* or *5.12*) available with your distribution; if you have any doubt, the *openSSL-1.0_Qt-5.9* version should work on most systems including older ones.

!!! tip
    In case you don't need a secure connexion to the server (for a local server for example), the version of *openSSL* does not really matter.

For **other distributions** or if you have any issues with the *.deb* package, you can just download and run the `.appImage` package. There are two versions of the package depending on the version of *openSSL* (either *1.0* or *1.1*) available with your distribution.

!!! warning
    When using the `.appImage` package, *openSSL* will not be automatically installed; if you need a secure connexion to the server you will have to install it with your usual package manager.

## Windows

There's no installer for *Windows* (yet). To install *Ramses*, you can just unzip the files in a folder of your choice.

If you want *Ramses* to start with the system, you can just copy a shortcut to `Ramses.exe` in the startup folder. You can access this startup folder with the shortcut `[Win + R]` and typing `shell:startup`.

## Mac OS

TODO