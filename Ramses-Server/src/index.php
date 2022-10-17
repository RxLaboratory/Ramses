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

	$__ROOT__ = dirname(__FILE__); 

	//configuration and init 
	require_once("config/config.php");
	require_once("config/config_logs.php");
	require_once("functions.php");
	require_once("logger.php");
	require_once("session_manager.php");

	// INIT
	include("init.php");

	//prepare reply
	require_once("reply.php");

	//get request metadata
	include("clientmetadata.php");

	//ping
	include("ping.php");

	//if the server is not installed, can't do anything more
	if (!$installed)
	{
		$reply["success"] = false;
		$reply["accepted"] = false;
		$reply["message"] = "This Ramses server is not installed yet.";
		printAndDie();
	}

	// this session has worn out its welcome; kill it and start a brand new one
	if (time() > $_SESSION['discard_after']) logout("Disconnected (Session expired)", "Your session has expired, you need to log-in.");

	//connect to database
	require_once('db.php');

	//login
	include("login.php");

	//secured operations, check token first
	$token = getArg("token");
	if ($token != $_SESSION["token"]) logout("Disconnected (Invalid token)", "Invalid token! [Warning] This may be a security issue!");

	
	include("sync.php");
	include("set_password.php");
	include("clean.php");

?>
