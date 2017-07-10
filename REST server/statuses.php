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
		$id = "";

		$data = json_decode(file_get_contents('php://input'));
		if ($data)
		{
			$name = $data->{'name'};
			$shortName = $data->{'shortName'};
			$color = $data->{'color'};
			$description = $data->{'description'};
			$id = $data->{'id'};
		}

		if (strlen($name) > 0 AND strlen($shortName) > 0 AND (strlen($color) == 6 OR strlen($color) == 7))
		{
			//add # on color if needed
			if (strlen($color) == 6) $color = "#" . $color;
			//if an id is provided
			if (strlen($id) > 0)
			{
				$qString = "INSERT INTO " . $tablePrefix . "status (name,shortName,color,description,id) VALUES ( :name , :shortName , :color , :description , :id ) ON DUPLICATE KEY UPDATE shortName = VALUES(shortName), name = VALUES(name);";
				$values = array('name' => $name, 'shortName' => $shortName, 'color' => $color, 'description' => $description, 'id' => $id);
			}
			else
			{
				$qString = "INSERT INTO " . $tablePrefix . "status (name,shortName,color,description) VALUES ( :name , :shortName , :color , :description ) ON DUPLICATE KEY UPDATE shortName = VALUES(shortName), name = VALUES(name);";
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

		$rep = $bdd->query("SELECT name,shortName,color,description,id FROM " . $tablePrefix . "status ORDER BY shortName;");
		$statuses = Array();
		while ($status = $rep->fetch())
		{
			$stat = Array();
			$stat['name'] = $status['name'];
			$stat['shortName'] = $status['shortName'];
			$stat['color'] = $status['color'];
			$stat['description'] = $status['description'];
			$stat['id'] = (int)$status['id'];
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
		$id = "";

		$data = json_decode(file_get_contents('php://input'));
		if ($data)
		{
			$name = $data->{'name'};
			$shortName = $data->{'shortName'};
			$color = $data->{'color'};
			$description = $data->{'description'};
			$id = $data->{'id'};
		}

		if (strlen($name) > 0 AND strlen($shortName) > 0 AND (strlen($color) == 6 OR strlen($color) == 7) AND strlen($id) > 0)
		{
			//add # on color if needed
			if (strlen($color) == 6) $color = "#" . $color;
			$qString = "UPDATE " . $tablePrefix . "status SET name= :name ,shortName= :shortName ,color= :color ,description= :description WHERE id= :id ;";

			$rep = $bdd->prepare($qString);
			$rep->execute(array('name' => $name, 'shortName' => $shortName, 'color' => $color, 'description' => $description, 'id' => $id));
			$rep->closeCursor();

			$reply["message"] = "Status " . $shortName . " (" . $id . ") updated.";
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
		$id = "";

		$data = json_decode(file_get_contents('php://input'));
		if ($data)
		{
			$id = $data->{'id'};
		}
		if (strlen($id) > 0)
		{

			$rep = $bdd->prepare("DELETE " . $tablePrefix . "status FROM " . $tablePrefix . "status WHERE id= :id ;");
			$rep->execute(array('id' => $id));
			$rep->closeCursor();

			$reply["message"] = "Status " . $id . " removed.";
			$reply["success"] = true;

		}
		else
		{
			$reply["message"] = "Invalid request, missing values";
			$reply["success"] = false;
		}
	}
?>
