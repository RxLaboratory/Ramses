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

    function parseRow( $row, $tableName )
    {
        $outRow = array();
        $outRow["uuid"] = $row["uuid"];
        
        $outRow["modified"] = $row["modified"];
        $outRow["removed"] = (int)$row["removed"];
        if ($tableName == "RamUser")
        {
            $outRow["userName"] = $row["userName"];
            // We need to decrypt the user data
            $data = $row["data"];
            $outRow["data"] = decrypt( $data );
        }
        else
        {
            $outRow["data"] = $row["data"];
        }

        return $outRow;
    }

    if ( acceptReply( "sync" ) )
    {
        $tables = getArg("tables", array());
        $prevSync = getArg("previousSyncDate", "1970-01-01 00:00:00");

        $outTables = array();
        $rowsToDelete = array();
        foreach( $tables as $table )
        {
            if (!isset($table["name"]))
            {
                $reply["success"] = false;
                $reply["message"] = "Malformed request, sorry. I've found a table without name.";
                printAndDie();
            }

            $tableName = $table["name"];
            $incomingRows = array();
            if (isset($table["modifiedRows"])) $incomingRows = $table["modifiedRows"];

            $outTable = array();
            $outTable["name"] = $tableName;
            $outTable["modifiedRows"] = array();
  
            // Create the table if it doesn't exists
            createTable( $tableName );

            // Get all rows (more recent than prevSync)
            $q = new DBQuery();

            $qStr = "SELECT `uuid`, `data`, `modified`, `removed` ";
            if ($tableName == "RamUser") $qStr = $qStr . ", `userName` ";
            if ($sqlMode == 'sqlite') $qStr = $qStr . " FROM {$tablePrefix}{$tableName} WHERE `modified` >= :modified;"; //  
            else $qStr = $qStr . " FROM {$tablePrefix}{$tableName} WHERE `modified` >= :modified;"; // 
            
            $q->prepare($qStr);
            $q->bindStr("modified", $prevSync);
            $q->execute();

            // For each row, check if it is more recent or equal or older
            $qStr = "";
            while ($row = $q->fetch())
            {
                // Check UUID
                $found = false;
                               
                for ($i = count($incomingRows) -1; $i >= 0; $i--)
                {
                    $inRow = $incomingRows[$i];
                    
                    // If the row is older than previous sync, ignore it
                    $inRowDate = strtotime( $inRow["modified"] );
                    /*if ($inRowDate < strtotime($prevSync)) {
                        array_splice($incomingRows, $i, 1);
                        continue;
                    }*/

                    if ($inRow["uuid"] != $row["uuid"]) continue;

                    // Found it!
                    $found = true;
                    array_splice($incomingRows, $i, 1);

                    $rowDate = strtotime( $row["modified"] );
                    // If in row is newer, update our side
                    if ($inRowDate > $rowDate)
                    {
                        // Encrypt user data
                        $data = $inRow["data"];
                        if ($tableName == "RamUser") $data = encrypt($data);

                        $qStr = "UPDATE {$tablePrefix}{$tableName} SET `data` = :data, `modified` = :modified, `removed` = :removed";
                        if ($tableName == "RamUser") $qStr = $qStr . ", `userName` = :userName";
                        $qStr = $qStr . " WHERE `uuid` = :uuid";
                        
                        $qr = new DBQuery();
                        $qr->prepare($qStr);
                        $qr->bindStr("data", $data);
                        $qr->bindStr("modified", $inRow["modified"]);
                        $qr->bindInt("removed", $inRow["removed"]);
                        $qr->bindStr("uuid", $inRow["uuid"]);
                        if ($tableName == "RamUser") $qr->bindStr("userName", $inRow["userName"]);
                        $qr->execute();
                        $qr->close();
                    }
                    // If it's older, send new data
                    else if ($inRowDate < $rowDate) $outTable["modifiedRows"][] = parseRow($row, $tableName);
                    // Done!
                    break;
                }

                // Not found, it's a new row, send it
                if (!$found) $outTable["modifiedRows"][] = parseRow($row, $tableName);
            }
                       
            $q->close();

            // Add remaining rows to table (if it's not been deleted)
            $tableRowsToDelete = array();
            $tableRowsToDelete["name"] = $tableName;
            $tableRowsToDelete["rows"] = array();
            foreach( $incomingRows as $inRow)
            {
                $qr = new DBQuery();

                // Check if the row has been deleted
                $qStr = "SELECT `uuid` FROM {$tablePrefix}deletedData WHERE `uuid` = :uuid ;";
                $qr->prepare($qStr);
                $qr->bindStr("uuid", $inRow["uuid"]);
                $qr->execute();
                
                if ($qr->fetch()) {
                    $tableRowsToDelete["rows"][] = $inRow["uuid"];
                    $qr->close();
                    continue;
                }

                if ($tableName == "RamUser")
                    $qStr = "INSERT INTO {$tablePrefix}{$tableName} (`uuid`, `data`, `modified`, `removed`, `password`, `userName` ) 
                            VALUES ( :uuid, :data, :modified, :removed, '-', :userName ) ";
                else
                    $qStr = "INSERT INTO {$tablePrefix}{$tableName} (`uuid`, `data`, `modified`, `removed`) 
                            VALUES ( :uuid, :data, :modified, :removed ) ";
                
                if ($sqlMode == 'sqlite') $qStr = $qStr . " ON CONFLICT(uuid) DO UPDATE SET ";
                else $qStr = $qStr . " ON DUPLICATE KEY UPDATE ";

                if ($tableName == "RamUser")
                    $qStr = $qStr . "`uuid` = :uuid, `data` = :data, `modified` = :modified, `removed` = :removed, `userName` = :userName ;";
                else
                    $qStr = $qStr . "`uuid` = :uuid, `data` = :data, `modified` = :modified, `removed` = :removed ;";

                // Encrypt user data
                $data = $inRow["data"];
                if ($tableName == "RamUser") $data = encrypt($data);
                
                $qr->prepare($qStr);
                $qr->bindStr("data", $data);
                $qr->bindStr("modified", $inRow["modified"]);
                $qr->bindInt("removed", (int)$inRow["removed"]);
                $qr->bindStr("uuid", $inRow["uuid"]);
                if ($tableName == "RamUser") $qr->bindStr("userName", $inRow["userName"]);
                $qr->execute();
                $qr->close();
            }

            // Add table to complete list
            $outTables[] = $outTable;
            $rowsToDelete[] = $tableRowsToDelete;
        }

        $reply["content"]["tables"] = $outTables;
        $reply["content"]["deletedData"] = $rowsToDelete;
        $reply["success"] = true;
        $reply["message"] = "Data Sync: OK!";
        printAndDie();
    }
?>
