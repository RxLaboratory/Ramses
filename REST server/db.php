<?php
	
	/*
		Rainbox Asset Manager
		Database access
	*/

	try
	{
		$bdd = new PDO('mysql:host=' . $sqlHost . ';dbname=' . $sqlDBName . ';charset=utf8', $sqlUser, $sqlpassword,array(PDO::ATTR_ERRMODE => PDO::ERRMODE_EXCEPTION));
	}
	catch (Exception $e)
	{
		echo "oops";
		die('Erreur : ' . $e->getMessage());
	}
?>