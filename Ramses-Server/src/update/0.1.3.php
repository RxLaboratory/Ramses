<?php
    if ( $currentVersion == '0.1.3-alpha' )
    {
        // ==== Encryption keys ====
        if ( !file_exists("../config_security.php") )
        {
            echo ( " ▸ Generating new encryption key.<br />" );

            $encrypt_key = createEncryptionKey();
            $encrypt_key_txt = base64_encode($encrypt_key);
            echo( "This will be the encryption key for this server:<br /><strong>{$encrypt_key_txt}</strong><br />" );
            echo( "It's been saved in <code>config_security.php</code>. You may backup this file now.<br />" );

            echo ( "     ▪ OK!<br />" );

            ob_flush();
            flush();
        }

        include('../config_security.php');

        // ==== Update Table Structure ====

        echo ( " ▸ Updating Database structure.<br />" );
        
        $rep = $db->query( "LOCK TABLES
                {$tablePrefix}users WRITE,
                {$tablePrefix}pipes WRITE,
                {$tablePrefix}pipefile WRITE,
                {$tablePrefix}applications WRITE,
                {$tablePrefix}colorspaces WRITE,
                {$tablePrefix}filetypes WRITE,
                {$tablePrefix}shotasset WRITE,
                {$tablePrefix}templateassetgroups WRITE,
                {$tablePrefix}schedule WRITE;

            ALTER TABLE {$tablePrefix}users
                ADD COLUMN `email` VARCHAR(255) NULL AFTER `password`,
                ADD COLUMN `color` VARCHAR(15) NOT NULL DEFAULT '#e3e3e3' AFTER `role`,
                CHANGE COLUMN `role` `role` VARCHAR(255) NOT NULL DEFAULT 'standard',
                CHANGE COLUMN `shortName` `shortName` VARCHAR(255) NOT NULL ;

            ALTER TABLE {$tablePrefix}pipes
                DROP FOREIGN KEY fk_pipes_input,
                DROP FOREIGN KEY fk_pipes_output,
                DROP INDEX `step_UNIQUE`,
                ADD CONSTRAINT `fk_pipes_input` FOREIGN KEY (`inputStepId`) REFERENCES `ram_steps`(`id`) ON DELETE CASCADE ON UPDATE CASCADE,
                ADD CONSTRAINT `fk_pipes_output` FOREIGN KEY (`outputStepId`) REFERENCES `ram_steps`(`id`) ON DELETE CASCADE ON UPDATE CASCADE,
                ADD UNIQUE `pipes_in_out_unique` (`outputStepId`, `inputStepId`) USING BTREE; 

            ALTER TABLE {$tablePrefix}pipefile ADD COLUMN `customSettings` TEXT NULL DEFAULT NULL AFTER `colorSpaceId`;

            ALTER TABLE {$tablePrefix}applications DROP INDEX `exec_unique`;

            ALTER TABLE {$tablePrefix}colorspaces DROP INDEX `name_UNIQUE`;

            ALTER TABLE {$tablePrefix}filetypes DROP INDEX `filetypeName`;

            ALTER TABLE {$tablePrefix}shotasset
                ADD UNIQUE `shot_asset_unique` (`shotId`, `assetId`) USING BTREE,
                ADD COLUMN `removed` TINYINT(4) NOT NULL DEFAULT '0' AFTER `latestUpdate`; 

            ALTER TABLE {$tablePrefix}templateassetgroups DROP INDEX `shortName_UNIQUE`;

            ALTER TABLE {$tablePrefix}schedule ADD COLUMN `removed` TINYINT(4) NOT NULL DEFAULT '0' AFTER `latestUpdate`; 

            UNLOCK TABLES;");
        
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

        // ==== Update Data ====

        echo ( " ▸ Encrypting user data.<br />" );
        
        ob_flush();
        flush();

        // get users
        $rep = $db->query( "SELECT `id`, `role`, `name`, `email` FROM {$tablePrefix}users;" );
        
        while ($user = $rep->fetch())
		{
            $role = checkRole( $user['role'] );
            $role = hashRole( $role );

            $name = $user['name'];
            $email = $user['email'];

            // Only if not already encrypted
            if( !isEncrypted( $name ) )
            {
                $name = encrypt( $name );
                $email = encrypt( $email );
            }
            if( !isEncrypted( $email ) )
            {
                $email = encrypt( $email );
            }


            $id = (int)$user['id'];

            $roleQuery = $db->prepare("UNLOCK TABLES;
                UPDATE {$tablePrefix}users
                SET `role`= :role, `name`= :name, `email`= :email
                WHERE `id`= :id;" );

            $roleQuery->bindValue(':role', $role, PDO::PARAM_STR);
            $roleQuery->bindValue(':name', $name, PDO::PARAM_STR);
            $roleQuery->bindValue(':email', $email, PDO::PARAM_STR);
            $roleQuery->bindValue(':id', $id, PDO::PARAM_INT);
            $ok = $roleQuery->execute();
            $roleQuery->closeCursor();

            if (!$ok)
            {
                echo( "    ▫ Failed. Could not update data, here's the error:<br />" );
                die( print_r($db->errorInfo(), true) );
            }

            echo ( "." );
            ob_flush();
            flush();
        }

        echo ( "<br />" );
        echo ( "     ▪ OK!<br />" );

        // Ready for the next step
        $currentVersion = '0.2.10-alpha';
    }
?>
