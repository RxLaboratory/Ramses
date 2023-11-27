# Synology NAS

It is relatively easy to install the Ramses server on a Synology NAS server running DSM.

## Prepare the server

1. Install the needed packages using DSM and the package center on your NAS:
    - ***PHP 8.2*** (or more recent)
    - ***MariaDB 10***
    - ***Web Station***
    - ***phpMyAdmin***, to make it easier to manage the SQL databases.
2. You may create a specific web portal and web service for your Ramses server. The web service must use a *PHP 8.x* version, and should use *Apache* if available.
3. Create a new SQL base, along with its user and password, using phpMyAdmin.

## Install ramses

1. Unzip the Ramses server zip archive you've downloaded

...