<?php
    if ( versionLowerThan('0.2.11-alpha', $currentVersion) )
    {

        // ==== Update Table Structure ====

        echo ( " ▸ Updating Database structure.<br />" );

        $rep = $db->query( 
        
        "ALTER TABLE `{$tablePrefix}schedule` CHANGE `stepId` `stepId` INT(11) NULL DEFAULT NULL;
        ALTER TABLE `{$tablePrefix}sequences` ADD `color` VARCHAR(15) NOT NULL DEFAULT '#434343' AFTER `comment`; 

        DELETE FROM `{$tablePrefix}schedule` WHERE `removed` = 1;");

        $ok = $rep->execute();
        $rep->closeCursor();

        if (!$ok)
        {
            echo( "    ▫ Failed. Could not update data, here's the error:<br />" );
            die( print_r($db->errorInfo(), true) );
        }

        echo ( "     ▪ OK!<br />" );

        ob_flush();
        flush();
    }
?>
