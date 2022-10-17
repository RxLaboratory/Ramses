<?php
    require_once($__ROOT__."/functions.php");
    require_once($__ROOT__."/reply.php");

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

    if ( acceptReply( "clean" ) )
    {
        if (!isAdmin())
        {
            $reply["success"] = false;
            $reply["message"] = "Warning! Missing privileges. You must be an administrator to clean the database.";
            printAndDie();
        }

        $tables = getArg("tables", array());

        $count = 0;

        // Create the deletedData table in case it doesn't exist yet
        createDeletedDataTable();

        foreach( $tables as $table)
        {
            if (!isset($table["name"]))
            {
                $reply["success"] = false;
                $reply["message"] = "Malformed request, sorry. I've found a table without name.";
                printAndDie();
            }

            $tableName = $table["name"];
            $rows = array();
            if (isset($table["rows"])) $rows = $table["rows"];

            // For each row, delete, and add to the deleted list
            $q = new DBQuery();
            foreach( $rows as $row )
            {
                $qStr = "INSERT INTO {$tablePrefix}deletedData (`uuid`) VALUES ( :uuid ) ";

                if ($sqlMode == 'sqlite') $qStr = $qStr . " ON CONFLICT(uuid) DO UPDATE SET ";
                else $qStr = $qStr . " ON DUPLICATE KEY UPDATE ";

                $qStr = $qStr . "`uuid` = :uuid ;";

                $q->prepare($qStr);
                $q->bindStr("uuid", $row);
                $q->execute("", true);

                if ($q->isOK()) {
                    $count++;
                    $qStr = "DELETE FROM {$tablePrefix}{$tableName} WHERE `uuid` = :uuid AND `removed` = 1;";
                    $q->prepare($qStr);
                    $q->bindStr("uuid", $row);
                    $q->execute();
                }
                
            }
        }

        $reply["content"]["count"] = $count;
        $reply["success"] = true;
        $reply["message"] = "Cleaned data, removed {$count} rows.";
        printAndDie();
    }
?>