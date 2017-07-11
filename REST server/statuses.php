<?php
/*
		Rainbox Asset Manager
		Statuses management
	*/

	// ========= ADD STATUS ==========
	if ($reply["type"] == "addStatus")
	{
		$reply["accepted"] = true;

		$name = "";
		$shortName = "";
		$color = "";
		$description = "";
		$uuid = "";

		$data = json_decode(file_get_contents('php://input'));
		if ($data)
		{
			if (isset($data->{'name'})) $name = $data->{'name'};
			if (isset($data->{'shortName'})) $shortName = $data->{'shortName'};
			if (isset($data->{'color'})) $color = $data->{'color'};
			if (isset($data->{'description'})) $description = $data->{'description'};
			if (isset($data->{'uuid'})) $uuid = $data->{'uuid'};
		}

		if (strlen($name) > 0 AND strlen($shortName) > 0 AND (strlen($color) == 6 OR strlen($color) == 7))
		{
			//add # on color if needed
			if (strlen($color) == 6) $color = "#" . $color;
			//if an id is provided
			if (strlen($uuid) > 0)
			{
				$qString = "INSERT INTO " . $tablePrefix . "status (name,shortName,color,description,uuid) VALUES ( :name , :shortName , :color , :description , :uuid ) ON DUPLICATE KEY UPDATE shortName = VALUES(shortName), name = VALUES(name);";
				$values = array('name' => $name, 'shortName' => $shortName, 'color' => $color, 'description' => $description, 'uuid' => $uuid);
			}
			else
			{
				$qString = "INSERT INTO " . $tablePrefix . "status (name,shortName,color,description,uuid) VALUES ( :name , :shortName , :color , :description, uuid() ) ON DUPLICATE KEY UPDATE shortName = VALUES(shortName), name = VALUES(name);";
				$values = array('name' => $name, 'shortName' => $shortName, 'color' => $color, 'description' => $description);
			}


			$rep = $bdd->prepare($qString);
			$rep->execute($values);
			$rep->closeCursor();

			$reply["message"] = "Status " . $shortName . " added.";
			$reply["success"] = true;

		}
		else
		{
			$reply["message"] = "Invalid request, missing values";
			$reply["success"] = false;
		}
	}

	// ========= GET STATUSES ==========
	if ($reply["type"] == "getStatuses")
	{
		$reply["accepted"] = true;

		$rep = $bdd->query("SELECT name,shortName,color,description,uuid FROM " . $tablePrefix . "status ORDER BY shortName;");
		$statuses = Array();
		while ($status = $rep->fetch())
		{
			$stat = Array();
			$stat['name'] = $status['name'];
			$stat['shortName'] = $status['shortName'];
			$stat['color'] = $status['color'];
			$stat['description'] = $status['description'];
			$stat['uuid'] = $status['uuid'];
			$statuses[] = $stat;
		}
		$rep->closeCursor();

		$reply["content"] = $statuses;
		$reply["message"] = "Statuses list retreived";
		$reply["success"] = true;

	}

	// ========= UPDATE STATUS ==========
	if ($reply["type"] == "updateStatus")
	{
		$reply["accepted"] = true;

		$name = "";
		$shortName = "";
		$color = "";
		$description = "";
		$uuid = "";

		$data = json_decode(file_get_contents('php://input'));
		if ($data)
		{
			if(isset($data->{'name'})) $name = $data->{'name'};
			if(isset($data->{'shortName'})) $shortName = $data->{'shortName'};
			if(isset($data->{'color'})) $color = $data->{'color'};
			if(isset($data->{'description'})) $description = $data->{'description'};
			if(isset($data->{'uuid'})) $uuid = $data->{'uuid'};
		}

		if (strlen($name) > 0 AND strlen($shortName) > 0 AND (strlen($color) == 6 OR strlen($color) == 7) AND strlen($uuid) > 0)
		{
			//add # on color if needed
			if (strlen($color) == 6) $color = "#" . $color;
			$qString = "UPDATE " . $tablePrefix . "status SET name= :name ,shortName= :shortName ,color= :color ,description= :description WHERE uuid= :uuid ;";

			$rep = $bdd->prepare($qString);
			$rep->execute(array('name' => $name, 'shortName' => $shortName, 'color' => $color, 'description' => $description, 'uuid' => $uuid));
			$rep->closeCursor();

			$reply["message"] = "Status " . $shortName . " (" . $uuid . ") updated.";
			$reply["success"] = true;
		}
		else
		{
			$reply["message"] = "Invalid request, missing values";
			$reply["success"] = false;
		}

	}

	// ========= REMOVE STATUS ==========
	if ($reply["type"] == "removeStatus")
	{
		$reply["accepted"] = true;
		$uuid = "";

		$data = json_decode(file_get_contents('php://input'));
		if ($data)
		{
			if(isset($data->{'uuid'})) $uuid = $data->{'uuid'};
		}
		if (strlen($uuid) > 0)
		{

			$rep = $bdd->prepare("DELETE " . $tablePrefix . "status FROM " . $tablePrefix . "status WHERE uuid= :uuid ;");
			$rep->execute(array('uuid' => $uuid));
			$rep->closeCursor();

			$reply["message"] = "Status " . $uuid . " removed.";
			$reply["success"] = true;

		}
		else
		{
			$reply["message"] = "Invalid request, missing values";
			$reply["success"] = false;
		}
	}
?>
