<?php

	/*
		Rainbox Asset Manager
		Installs the SQL Database
	*/

    $__ROOT__ = dirname(dirname(__FILE__)); 

    require_once($__ROOT__."/config/config.php");

    //connect to database

    if ($sqlMode == "sqlite") // Copy the default db first
    {
        echo ( "Writing the new database scheme (using SQLite)...<br />" );
        flush();

        if (!is_dir($__ROOT__."/data")) mkdir($__ROOT__."/data");
        $ok = copy($__ROOT__."/install/ramses.sqlite", $__ROOT__."/data/ramses_data");

        if (!$ok)
        {
            die( "Sorry, something went wrong while writing the database. Make sure the server has write access to its folder." );
        }

        echo ( "The new database is ready!<br />" );
        flush();
    }

    echo ( "Connecting to the database...<br />" );
    flush();

    require_once($__ROOT__."/functions.php");
    require_once($__ROOT__."/db.php");

    echo ( "Database found and working!<br />" );
    flush();

    setupTablePrefix();

    echo ( "Generating encryption keys...<br />" );
    flush();

    $encrypt_key = createEncryptionKey();
    $encrypt_key_txt = base64_encode($encrypt_key);
    echo( "The encryption key has been saved in <code>config/config_security.php</code>. You should backup this file now.<br />" );
    flush();

    include($__ROOT__."/config/config_security.php");

    // Set the DB if MySQL (if SQLite, the file is already available)
    if ($sqlMode != "sqlite")
    {
        echo ( "Writing the new database scheme (using MySQL)...<br />" );
        flush();

        // Create the RamUser Table
        createTable("RamUser", true);

        // Add username and password rows
        $q = new DBQuery();
        $q->prepare("ALTER TABLE `{$tablePrefix}RamUser`
            ADD  `userName` VARCHAR(255) CHARACTER SET utf8 COLLATE utf8_unicode_ci NOT NULL AFTER `uuid`,
            ADD  `password` TEXT CHARACTER SET utf8 COLLATE utf8_unicode_ci NOT NULL AFTER `userName`,
            ADD UNIQUE KEY `userName` (`userName`);
            ");

        $q->execute();
        $q->close();

        if ( !$q->isOK() )
        {
            die( "Sorry, something went wrong while writing the database." );
        }

        echo ( "Database tables are ready!<br />" );
    }
    // Rename the user table to use the prefix
    else
    {
        $q = new DBQuery();
        $q->prepare("ALTER TABLE RamUser
            RENAME TO {$tablePrefix}RamUser;");

        $q->execute();
        $q->close();
    }

    echo( "Setuping the administrator user...<br />" );
    flush();
    
    //Setup admin user
    $uuid = uuid();
    //Prepare password
    $pswd = str_replace("/", "", $serverAddress) . "password" . $clientKey;
    $pswd = hash("sha3-512", $pswd);
    $pswd = hashPassword($pswd, $uuid);
    $data = encrypt("{\"name\":\"Administrator\",\"shortName\":\"Admin\",\"comment\":\"The default Administrator user. Don't forget to rename it and change its password!\",\"color\":\"#b3b3b3\",\"role\":\"admin\"}");
    
    $q = new DBQuery();
    $qStr = "REPLACE INTO {$tablePrefix}RamUser ( `uuid`, `userName`, `password`, `data`, `modified` )
		VALUES ( :uuid, 'Admin', :password, :data, '1970-01-01 12:00:00' );
		COMMIT;";
    $q->prepare( $qStr );

    $q->bindStr('uuid', $uuid);
    $q->bindStr('data', $data);
    $q->bindStr('password', $pswd);
    
    $q->execute();
    $q->close();

    if ( !$q->isOK() )
    {
        echo( "Could not create the administrator, here's the error:<br />" );
        die( print_r($q->errorInfo(), true) );
    }
    
    echo ( "<p>Ramses has been correctly installed, you can now <strong>remove the <code>install</code> directory</strong>.</p><p>The default user is <strong>\"Admin\" with password <code>password</code></strong>.<br />Do not forget to change this name and password!</p>" );
?>
