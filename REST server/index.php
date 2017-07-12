<?php

	/*
		Server for Rainbox Asset Manager
	*/

	//configuration
	include ("config.php");

	// server should keep session data for AT LEAST  sessionTimeout
	ini_set('session.gc_maxlifetime', $sessionTimeout);

	// each client should remember their session id for EXACTLY sessionTimeout
	session_set_cookie_params($sessionTimeout);

	session_start();

	$now = time();
	if (isset($_SESSION['discard_after']) && $now > $_SESSION['discard_after'])
	{
		// this session has worn out its welcome; kill it and start a brand new one
		session_unset();
		session_destroy();
		session_start();
	}
	else
	{
		// either new or old, it should live at most for sessionTimeout
		$_SESSION['discard_after'] = $now + $sessionTimeout;
	}

	//add the _ after table prefix
	if (strlen($tablePrefix) > 0) $tablePrefix = $tablePrefix . "_";

	//result of the request
	$reply = Array();
	$reply["accepted"] = false;

	//if a request type was specified only
	if (isset($_GET["type"]))
	{
		$reply["type"] = $_GET["type"];
		$reply["success"] = false;

		//connect to database
		include('db.php');

		//login
		include ("login.php");

		if ($reply["type"] != "login")
		{
			if (isset($_SESSION["login"]) AND $_SESSION["login"])//if logged in
			{
				//users
				include ("users.php");
				//statuses
				include ("statuses.php");
				//stages
				include ("stages.php");
				//projects
				include ("projects.php");
				//shots
				include ("shots.php");
				//assets
				include("assets.php");

				//if accepted display result
				if ($reply["accepted"])
				{
					//Display the result of the request
					echo json_encode($reply);
				}
				else//if nothing was accepted
				{
					$reply["message"] = "Unknown request";
					$reply["accepted"] = false;
					echo json_encode($reply);
				}
			}//if not logged in
			else
			{
				$reply["message"] = "You have been logged out by the server.";
				$reply["accepted"] = false;
				session_destroy();
				echo json_encode($reply);
			}
		}
	}
	else //no request type
	{
		$reply["message"] = "Invalid request";
		$reply["accepted"] = false;
		$reply["type"] = "Unknown";
		echo json_encode($reply);
	}



?>
