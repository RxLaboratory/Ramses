<?php
    $__ROOT__ = dirname(dirname(__FILE__)); 

    //configuration and init
    require_once ($__ROOT__."/config/config.php");
    require_once ($__ROOT__."/config/config_security.php");
    require_once ($__ROOT__."/functions.php");
    require_once ($__ROOT__."/logger.php");
    require_once ($__ROOT__."/session_manager.php");
    require_once ($__ROOT__."/init.php");

    //connect to database
    include('../db.php');

    //Set a password to a specific user
    if (hasArg("setPassword"))
    {
        $username = getArg("username");
        $password = getArg("password");

        //query the database for the user uuid
        $rep = $db->prepare("SELECT uuid FROM {$tablePrefix}users WHERE shortName = :username ;");
        $rep->execute(array('username' => $username));
        $testPass = $rep->fetch();
        $rep->closeCursor();

        //hash password (official ramses client side)
        $clientPassword = hash("sha3-512", $password . "H6BuYLsW" );
        //hash password (server side)
        $uuid = $testPass["uuid"];
        $password = hashPassword( $clientPassword, $uuid );

        //set in the database
        $rep = $db->prepare("UPDATE {$tablePrefix}users SET password = :password WHERE uuid= :uuid ;" );
        $ok = $rep->execute(array('uuid' => $uuid, 'password' => $password));
        //$rep->debugDumpParams();
        $rep->closeCursor();

        //echo the new hashed password
        echo "UUID: " . $uuid . "<br />";
        echo "Server password: " . $password;
        echo "<br />Client password: " . $clientPassword;
    }
    else if (hasArg("login"))
    {
        $username = "";
        $password = "";
        
        $username = getArg("username");
        $password = getArg("password", "truc");

        if (strlen($username) > 0 AND strlen($password) > 0)
        {
            //query the database
            $rep = $db->prepare("SELECT `uuid`,`userName`,`password` FROM {$tablePrefix}RamUser WHERE `userName` = :username ;");
            $rep->execute(array('username' => $username));
            $row = $rep->fetch();
            $rep->closeCursor();
            
            if (!$row)
            {
            	echo "Invalid username!";
            	die();
            }

            //check password
            //hash password (official ramses client side)
            $password = str_replace("/", "", $serverAddress) .  $password . $clientKey;
            $password = hash("sha3-512", $password );

            $uuid = $row["uuid"];
            $testPass = $row["password"];
                       
            if ( checkPassword($password, $uuid, $testPass) )
            {
                $token = login($uuid, "test", "test", "test");
                echo $token;
                die();
            }
            else
            {
                echo "Invalid password";
                die();
            }
        }
    }
    else
    {
        echo "Nothing to do.";
    }
?>