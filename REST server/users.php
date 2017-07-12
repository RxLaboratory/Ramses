<?php
    /*
        Rainbox Asset Manager
        User Management
    */
    // ========= UPDATE STATUS ==========
    if ($reply["type"] == "updateUser")
    {
        $reply["accepted"] = true;

        $userName = "";
        $firstName = "";
        $lastName = "";
        $password = "";
        $uuid = "";

        $data = json_decode(file_get_contents('php://input'));
        if ($data)
        {
            if(isset($data->{'userName'})) $userName = $data->{'userName'};
            if(isset($data->{'firstName'})) $firstName = $data->{'firstName'};
            if(isset($data->{'lastName'})) $lastName = $data->{'lastName'};
            if(isset($data->{'password'})) $password = $data->{'password'};
            if(isset($data->{'uuid'})) $uuid = $data->{'uuid'};

        }

        if (strlen($userName) > 0 AND strlen($uuid) > 0 AND strlen($password) > 0)
        {
            $qString = "UPDATE " . $tablePrefix . "users SET userName= :userName ,firstName= :firstName ,lastName= :lastName ,password= :password WHERE uuid= :uuid ;";

            $rep = $bdd->prepare($qString);
            $rep->execute(array('userName' => $userName, 'firstName' => $firstName, 'lastName' => $lastName, 'password' => $password, 'uuid' => $uuid));
            $rep->closeCursor();

            $reply["message"] = "User " . $userName . " updated.";
            $reply["success"] = true;
        }
        else
        {
            $reply["message"] = "Invalid request, missing values";
            $reply["success"] = false;
        }

    }

?>
