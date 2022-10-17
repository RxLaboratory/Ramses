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

    if ( acceptReply( "login" ) )
    {
        $username = getArg( "username" );
        $password = getArg( "password" );

        if (strlen($username) == 0)
        {
            $reply["message"] = "Missing user name";
            $reply["success"] = false;
            logout($username, "Connexion refused (invalid password)");
        }

        if (strlen($password) == 0)
        {
            $reply["message"] = "Missing password";
            $reply["success"] = false;
            logout($username, "Connexion refused (invalid password)");
        }

        $q = new DBQuery();
        $q->prepare( "SELECT `uuid`,`userName`,`password`,`data` FROM {$tablePrefix}RamUser WHERE `userName` = :username AND removed = 0;" );
        $q->bindStr( "username", $username );
        $q->execute();
		$row = $q->fetch();
        $q->close();

		if (!$row)
        {
            $reply["message"] = "Invalid password or username";
            $reply["success"] = false;
            logout($username, "Connexion refused (invalid username)");
        }


        $found = true;
        $uuid = $row["uuid"];
        $data = $row["data"];
        $tPassword = $row["password"];
        // decrypt data
        $data = decrypt( $data );
        $data = json_decode( $data, true);

        //check password
        if ( !checkPassword($password, $uuid, $tPassword ) )
        {
            $reply["message"] = "Invalid password or username";
            $reply["success"] = false;
            logout($username, "Connexion refused (invalid password)");
        }

        // Get other data for the log
        $name = "unknown";
        if ( isset($data["name"]) ) $name = $data["name"];
        $role = "unknown";
        if ( isset($data["role"]) ) $role = $data["role"];

        // Login
        $token = login($uuid, $role, $username, $name);

        //reply content
        $content = array();
        $content["username"] = $username;
        $content["uuid"] = $uuid;
        $content["token"] = $token;
        $reply["content"] = $content;
        $reply["message"] = "Successful login. Welcome " . $content["username"] . "!";
        $reply["success"] = true;

        printAndDie();
    }
?>