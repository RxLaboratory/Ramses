<?php

	/*
		Rainbox Asset Manager
		Creates an admin user in the SQL Database
	*/

    //connect to database

    echo ( "Connecting to the database...<br />" );

    include('../config.php');
    include('../config_security.php');
    include('../functions.php');
    include('../db.php');

    echo ( "Database found and working!<br />" );

    setupTablePrefix();

    echo ( "Setup admin user...<br />" );

    //Setup admin user
    $uuid = uuid();
    $shortName = "Admin";
    $name = encrypt("Administrator");
    $email = encrypt("");
    $pswd = str_replace("/", "", $serverAddress) . "password" . $clientKey;
    $pswd = hash("sha3-512", $pswd);
    $pswd = hashPassword($pswd, $uuid);
    $comment = "The default Administrator user. Don't forget to rename it and change its password!";
    $role = hashRole('admin');
   
    $qString = "REPLACE INTO
        {$tablePrefix}users (
            `name`,
            `shortName`,
            `uuid`,
            `password`,
            `role`,
            `comment`,
            `email` )
        VALUES (
            :name ,
            :shortName ,
            :uuid,
            :password,
            :role,
            :comment,
            :email );
        COMMIT;";

    $rep = $db->prepare($qString);
    $rep->bindValue(':uuid', $uuid, PDO::PARAM_STR);
    $rep->bindValue(':name', $name, PDO::PARAM_STR);
    $rep->bindValue(':shortName', $shortName, PDO::PARAM_STR);
    $rep->bindValue(':password', $pswd, PDO::PARAM_STR);
    $rep->bindValue(':comment', $comment, PDO::PARAM_STR);
    $rep->bindValue(':role', $role, PDO::PARAM_STR);
    $rep->bindValue(':email', $email, PDO::PARAM_STR);

    $ok = $rep->execute();
    $rep->closeCursor();

    if (!$ok)
    {
        echo( "Could not create the administrator, here's the error:<br />" );
        die( print_r($db->errorInfo(), true) );
    }
    
    echo ( "<p><p>User <strong>\"Admin\" with password \"password\"</strong> has been added.<br />Do not forget to change this name and password!</p>" );
?>