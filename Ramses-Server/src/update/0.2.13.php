<?php
    if ( versionLowerThan('0.2.13-alpha', $currentVersion) )
    {

        // ==== Update Table Structure ====

        echo ( " ▸ Updating Database structure.<br />" );

        if ($sqlmode == 'sqlite') {
            $rep = $db->query( 
                "CREATE TABLE \"{$tablePrefix}schedulecomments\" (
                    \"id\"	INTEGER NOT NULL UNIQUE,
                    \"uuid\"	varchar(36) NOT NULL UNIQUE,
                    \"projectId\"	INTEGER NOT NULL,
                    \"date\"	datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
                    \"comment\"	TEXT,
                    \"color\"	TEXT DEFAULT '#e3e3e3',
                    \"latestUpdate\"	timestamp NOT NULL,
                    \"removed\"	tinyint(4) NOT NULL DEFAULT 0,
                    PRIMARY KEY(\"id\" AUTOINCREMENT),
                    UNIQUE(\"uuid\"),
                    FOREIGN KEY(\"projectId\") REFERENCES \"{$tablePrefix}projects\"(\"id\") ON DELETE CASCADE ON UPDATE CASCADE
                );
                
                ALTER TABLE `{$tablePrefix}steps` ADD `publishSettings` TEXT NULL DEFAULT NULL AFTER `comment`;
                
                ");
        }
        else {
            $rep = $db->query(
                "CREATE TABLE `{$tablePrefix}schedulecomments` (
                    `id` int(11) NOT NULL,
                    `uuid` varchar(36) NOT NULL,
                    `projectId` int(11) NOT NULL,
                    `date` datetime NOT NULL,
                    `comment` text DEFAULT NULL,
                    `color` varchar(8) NOT NULL DEFAULT '#e3e3e3',
                    `latestUpdate` timestamp NOT NULL DEFAULT current_timestamp() ON UPDATE current_timestamp(),
                    `removed` tinyint(4) NOT NULL DEFAULT 0
                  ) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;
                  
                  
                  ALTER TABLE `{$tablePrefix}schedulecomments`
                    ADD PRIMARY KEY (`id`),
                    ADD UNIQUE KEY `uuid_unique` (`uuid`),
                    ADD UNIQUE KEY `unique_project_date` (`projectId`,`date`);
                  
                  
                  ALTER TABLE `{$tablePrefix}schedulecomments`
                    MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;
                  
                  
                  ALTER TABLE `{$tablePrefix}schedulecomments`
                    ADD CONSTRAINT `fk_project` FOREIGN KEY (`projectId`) REFERENCES `{$tablePrefix}projects` (`id`) ON DELETE CASCADE ON UPDATE CASCADE;
                
                  ALTER TABLE `{$tablePrefix}steps` ADD `publishSettings` TEXT NULL DEFAULT NULL AFTER `comment`; 
                
                ");
        }

        $ok = $rep->execute();
        $rep->closeCursor();

        if (!$ok)
        {
            echo( "    ▫ Failed. Could not update data, here's the error:<br />" );
            die( print_r($db->errorInfo(), true) );
        }

        // ==== Add/Update an admin user to re-set passwords ====

        echo ( " ▸ Updating User Data.<br />" );

        //Setup admin user
        $uuid = uuid();
        $shortName = "Admin";
        $name = encrypt("Administrator");
        $email = encrypt("");
        //Prepare password
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
            echo( "    ▫ Failed. Could not update data, here's the error:<br />" );
            die( print_r($db->errorInfo(), true) );
        }

        echo ( "     ▪ OK!<br />" );

        echo ( "<p>User <strong>\"Admin\" with password \"password\"</strong> has been added.<br />Do not forget to change this name and password!</p>" );
        echo ( "<p>You now have to use this Admin account to update the passwords for all users and re-enable their accounts.</p>" );

        ob_flush();
        flush();
    }
?>
