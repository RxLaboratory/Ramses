<?php

	/*
		Rainbox Asset Manager
		Handles login requests
	*/

	if ($reply["type"] == "login")
	{
		$reply["accepted"] = true;

		$username = "";
		$password = "";

        $data = file_get_contents('php://input');
        if (isset($data["username"]) and isset($data["password"]))
        {
            $username = $data["username"];
			$password = $data["password"];
        }
        else
        {
            $data = json_decode(file_get_contents('php://input'));
            if ($data)
            {
                if (isset($data->{'username'})) $username = $data->{'username'};
				if (isset($data->{'password'})) $password = $data->{'password'};
            }
        }

		if (strlen($username) > 0 AND strlen($password) > 0)
		{
			//query the database
			$rep = $bdd->prepare("SELECT password,firstName,lastName,uuid FROM " . $tablePrefix . "users WHERE username = :username ;");
			$rep->execute(array('username' => $username));
			$testPass = $rep->fetch();
			$rep->closeCursor();

			//check password
			if ($testPass["password"] == $password)
			{
				$_SESSION["login"] = true;
				$content = array();
				$content["firstName"] = $testPass["firstName"];
				$content["lastName"] = $testPass["lastName"];
				$content["uuid"] = $testPass["uuid"];
				$reply["content"] = $content;
				$reply["message"] = "Successful login. Welcome " . $username . "!";
				$reply["success"] = true;
				echo json_encode($reply);
			}
			else
			{
				$_SESSION["login"] = false;
				$reply["message"] = "Invalid username or password";
				$reply["success"] = false;
				session_destroy();
				echo json_encode($reply);
			}
		}
		else
		{
			$_SESSION["login"] = false;
			$reply["message"] = "Invalid request, missing username or password";
			$reply["success"] = false;
			session_destroy();
			echo json_encode($reply);
		}
	}
?>
