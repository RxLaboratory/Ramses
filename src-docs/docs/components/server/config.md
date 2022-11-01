![META](authors:Nicolas "Duduf" Dufresne;license:GNU-FDL;copyright:2021-2022;updated:2022/11/01)

# Server configuration

The server can and must be configured by editing these two files: `config/config.php` and `config/config_logs.php`.

## config.php

The `config.php` file contains the main configuration and should look like that:

```php
<?php
    /*
		Ramses: Rx Asset Management System
        
        This program is licensed under the GNU General Public License.

        Copyright (C) 20202-2021 Nicolas Dufresne and Contributors.

        This program is free software;
        you can redistribute it and/or modify it
        under the terms of the GNU General Public License
        as published by the Free Software Foundation;
        either version 3 of the License, or (at your option) any later version.

        This program is distributed in the hope that it will be useful,
        but WITHOUT ANY WARRANTY; without even the implied warranty of
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
        See the GNU General Public License for more details.

        You should have received a copy of the *GNU General Public License* along with this program.
        If not, see http://www.gnu.org/licenses/.
	*/

	// Edit this configuration file before running the install script at /install/index.php

	// === DEV MODE & DEBUG Info ===

	// Activates printing the SQL & PHP errors.
	// For security reasons, it is important to set this to false in production mode
	$devMode = false;
	// Sets the minimum level of the logs in the server replies sent to the clients
	// One of: 'DATA', 'DEBUG', 'INFO', 'WARNING', 'CRITICAL', 'FATAL'
	$logLevel = 'WARNING';

	// ==== SQL SETTINGS ====

	// either 'mysql' (works for MariaDB too) or 'sqlite'
	$sqlMode = 'sqlite';
	// Table prefix
	// should be a random character string (use https://duckduckgo.com/?q=generate+password)
	// you can setup multiple instances on the same DB
	// if each use a different prefix
	$tablePrefix = "5H3VBzSY";

	// ==== MySQL/MariaDB SETTINGS ====

	// Host URL
	$sqlHost = "db";
	$sqlPort = 3306;
	// Database name
	$sqlDBName = "ramses";
	// User
	$sqlUser = "ramses";
	// Password
	$sqlpassword = "password";

	// ==== SESSION & SECURITY SETTINGS ====

	// This must be the server public adress, exactly as used in the clients
	$serverAddress = "localhost/ramses";

	// Whether to accept only SSL connections
	// This should always be true, except maybe on dev environments.
	$forceSSL = true;

	// Session timeout (seconds)
	// The client will be disconnected after being idle for this time
	// 30 minutes by default (1800) are more than enough:
	// The official client makes a call at least every 2 minutes by default.
	// A negative value completely deactivates the timeout check.
	$sessionTimeout = 1800;
	// Max Session timeout (seconds)
	// The client will be disconnected no matter what after this time
	// 12 hours by default( 43200 )
	$cookieTimeout = 43200;

	// This should never be changed, unless you change the key before building the official client or implementing your own client.
	// It is used to break compatibility between some updates of the client/server framework
	// It can be used to make sure only your own client, built by yourself, can connect to your own server. In this case, keep it secret!
	// It is used to hash passwords.
	$clientKey = "drHSV2XQ";
?>
```

- `$devMode` should always be `false` unless this server is going to be used for development and tests in a development environement. For security reasons **do not set this to `true`** on a production server.
- `$logLevel` controls the quantity of logs included in the server replies and displayed by *Ramses Applications*. This is independant from the logs saved on the server configured by the `config/config_logs.php` file.
- `$sqlMode` can be either `'sqlite'` or `'mysql'` depending on your choice of configuration (read above).
- `$tablePrefix` is a prefix which will be used for the name of the database tables used by *Ramses*. It allows you to share a database with other instances of *Ramses* and even any other application (for example if your provider doesn't allow you to create more databases on your server).

These next options are only used in *MySQL* mode. Ignore them when using *SQLite*.

- `$sqlHost` is the host name or IP of your *MySQL* server. Don't forget the quotes!
- `$sqlPort` is the port used by the *MySQL* server. `3306` is the default for most installations, if you need something different, your hosting provider should tell you.
- `$sqlDBName` is the name of the specific database you have to create for *Ramses*. 
- `$sqlUser` is the name of the specific *MySQL* user you have to create for *Ramses*.
- `$sqlpassword` is the password of the specific *MySQL* user you have to create for *Ramses*.

!!! note
	When using the *MySQL Docker* folder, do not change the `sqlHost` value, unless you've changed the settings in the `docker_compose.yml` file.

And finally complete these settings too:

- `$serverAddress` must be the one and only address used by the *Ramses Applications* to connect to this server. If this is incorrectly set, *Ramses Applications* won't be able to connect to this server.
- `$forceSSL` will always redirect calls to `https` when set to `true`. Set this to `false` if you don't have any SSL certificate on this server (not recommended).
- `$sessionTimeout` is the time, in seconds, before an idle client is disconnected.
- `$cookieTimeout` is the time, in seconds, before a client is disconnected, no matter what.
- `$clientKey` can be changed *only* if your client is not the official open source *Ramses Client Application*. Otherwise, leave it to the default value. It is a way to make sure only your client can connect to the server. It is currently used only when setting up the first user (administrator) during the server installation.

## config_logs.php

*Ramses* can log what happens on the server. This can be useful for debugging or to diagnose issues on the server. By default, all logs are deactivated.

Logs are stored by date in the `logs` subfolder inside the ramses server folder. Connection logs and debug logs are stored as *CSV* files (one per day), request logs are *text* and *json* files (one subfolder per request).

This is the configuration for the logs:

```php
<?php
    /*
		Ramses: Rx Asset Management System
        
        This program is licensed under the GNU General Public License.

        Copyright (C) 20202-2021 Nicolas Dufresne and Contributors.

        This program is free software;
        you can redistribute it and/or modify it
        under the terms of the GNU General Public License
        as published by the Free Software Foundation;
        either version 3 of the License, or (at your option) any later version.

        This program is distributed in the hope that it will be useful,
        but WITHOUT ANY WARRANTY; without even the implied warranty of
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
        See the GNU General Public License for more details.

        You should have received a copy of the *GNU General Public License* along with this program.
        If not, see http://www.gnu.org/licenses/.
	*/

	// Edit this configuration file to adjust server logging

	// === GLOBALLY ENABLE LOGS ===
	$enableLogs = false;

    // Connexion logs
    $connexionLogs = false;
    // Requests logs
    $requestLogs = false;
    // Debug logs
    $debugLogs = false;

    // How long do we keep logs? (days)
    $logsExpiration = 2;
?>
```

- `$enableLogs`: set this to `true` or `false` to globally enable logs.
- `$connexionLogs`: when `true`, ramses will log all connections and disconnections to and from the server.
- `$requestLogs`: when `true`, ramses will log requests received by the server including: the request headers, body, and some metadata. The details of the request headers may change according to the web server you're using and its version.
- `$debugLogs`: when `true`, ramses will log some extra information coming with each request.
- `$logsExpiration` sets how many days the logs will be kept. Keep this value small if there's a lot of activity on your server to avoid quickly growing the log size.