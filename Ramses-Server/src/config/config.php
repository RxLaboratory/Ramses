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

	// === DEV MODE ===

	// Activates printing the SQL & PHP errors.
	// For security reasons, it is important to set this to false in production mode
	$devMode = false;

	// ==== SQL SETTINGS ====

	// either 'mysql' (works for MariaDB too) or 'sqlite'
	$sqlMode = 'sqlite';

	// ==== MySQL/MariaDB SETTINGS ====

	// Host URL
	$sqlHost = "localhost";
	$sqlPort = 3306;
	// Database name
	$sqlDBName = "ramses";
	// User
	$sqlUser = "ramses";
	// Password
	$sqlpassword = "password";
	// Table prefix
	// should be a random character string (use https://duckduckgo.com/?q=generate+password)
	// you can setup multiple instances on the same DB
	// if each use a different prefix
	$tablePrefix = "5H3VBzSY";

	// ==== SESSION & SECURITY SETTINGS ====

	// Whether to accept only SSL connections
	// This should always be true, except maybe on dev environments.
	$forceSSL = true;

	// Session timeout (seconds)
	// The client will be disconnected after being idle for this time
	// 30 minutes by default (1800) are more than enough:
	// The official client makes a call at least every 2 minutes by default.
	$sessionTimeout = 1800;
	// Max Session timeout (seconds)
	// The client will be disconnected no matter what after this time
	// 12 hours by default( 43200 )
	$cookieTimeout = 43200;

	// This must be the server public adress, exactly as used in the clients
	$serverAddress = "localhost/ramses";

	// This should never be changed, unless you change the key before building the official client or implementing your own client.
	// It is used to break compatibility between some updates of the client/server framework
	// It can be used to make sure only your own client, built by yourself, can connect to your own server. In this case, keep it secret!
	// It is used to hash passwords.
	$clientKey = "drHSV2XQ";
?>
