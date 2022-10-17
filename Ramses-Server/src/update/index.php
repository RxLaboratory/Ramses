<?php

	/*
		Rainbox Asset Manager
		Updates an existing server to a new version
	*/

    //connect to database

    ob_start();

    echo ( "<pre>" );
    echo ( "Connecting to the database...<br />" );

    include('../config.php');
    include('../functions.php');
    include ('../logger.php');
    include ('../init.php');
    include('../db.php');

    echo ( "Database found and working!<br />" );

    setupTablePrefix();

    echo ( "Initializing update...<br />" );
    echo ( " ▸ Creating server metadata table if needed.<br />" );

    ob_flush();
    flush();

    // ==== Server metadata table ====

    $qString = "CREATE TABLE IF NOT EXISTS {$tablePrefix}servermetadata (
        `id` INT UNSIGNED NOT NULL AUTO_INCREMENT,
        `version` VARCHAR(45) NOT NULL,
        `date` DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
        PRIMARY KEY (`id`));";

    $rep = $db->prepare($qString);
    $ok = $rep->execute();
    $rep->closeCursor();

    if (!$ok)
    {
        echo( "     ▫ Failed.<br />Could not update the databse, here's the error:<br />" );
        die( print_r($db->errorInfo(), true) );
    }
    echo ( "     ▪ OK!<br />" );

    ob_flush();
    flush();

    // ==== Get current version ====

    echo ( " ▸ Checking current version.<br />" );

    $rep = $db->query( "SELECT `version`, `date`
        FROM {$tablePrefix}servermetadata
        ORDER BY `date` DESC;" );

    $currentVersion = '0.1.3-alpha';
    if ($v = $rep->fetch()) $currentVersion = $v['version'];

    if ($currentVersion == $ramsesVersion)
    {
        die( "     ▪ OK! This server seems to be up-to-date already, nothing to do." );
    }

    echo ( "     ▪ OK! We're updating from <strong><i>{$currentVersion}</i></strong> to <strong><i>{$ramsesVersion}</i></strong>.<br />" );

    ob_flush();
    flush();

    // ==== Update ====

    echo ( "Updating...<br />" );

    include('0.1.3.php');
    include('0.2.11.php');
    include('0.2.13.php');

    // ==== Set new metadata ====

    echo ( "Finishing update...<br />" );
    echo ( " ▸ Writing new server metadata.<br />" );

    $qString = "UNLOCK TABLES;
        INSERT INTO {$tablePrefix}servermetadata (`version`)
        VALUES ( :version );";

    $rep = $db->prepare($qString);
    $rep->bindValue(':version', $ramsesVersion, PDO::PARAM_STR);
    $ok = $rep->execute();
    $rep->closeCursor();

    if (!$ok)
    {
        echo( "    ▫ Failed. Could not update server metadata, here's the error:<br />" );
        die( print_r($db->errorInfo(), true) );
    }
    echo ( "     ▪ OK!<br />" );
    
    echo ( "<p><strong>Server is updated to version <i>" . $ramsesVersion . "</i> and ready!</strong><br />You may now remove the update folder.</p>" );
    echo ( "</pre>" );

    ob_end_flush();
?>