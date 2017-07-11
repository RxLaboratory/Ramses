<?php
/*
		Rainbox Asset Manager
		Stages management
	*/

	// ========= ADD STAGE ==========
	if ($reply["type"] == "addStage")
	{
		$reply["accepted"] = true;

		$name = "";
		$shortName = "";
		$uuid = "";

		$data = json_decode(file_get_contents('php://input'));
		if ($data)
		{
			if (isset($data->{'name'})) $name = $data->{'name'};
			if (isset($data->{'shortName'})) $shortName = $data->{'shortName'};
			if (isset($data->{'uuid'})) $uuid = $data->{'uuid'};
		}

		if (strlen($name) > 0 AND strlen($shortName) > 0)
		{
			if (strlen($uuid) > 0)
			{
				$qString = "INSERT INTO " . $tablePrefix . "stages (name,shortName,uuid) VALUES ( :name , :shortName , :uuid ) ON DUPLICATE KEY UPDATE shortName = VALUES(shortName), name = VALUES(name);";
				$values = array('name' => $name,'shortName' => $shortName, 'uuid' => $uuid);
			}
			else
			{
				$qString = "INSERT INTO " . $tablePrefix . "stages (name,shortName,uuid) VALUES ( :name , :shortName , uuid() ) ON DUPLICATE KEY UPDATE shortName = VALUES(shortName), name = VALUES(name);";
				$values = array('name' => $name,'shortName' => $shortName);
			}

			$rep = $bdd->prepare($qString);
			$rep->execute($values);
			$rep->closeCursor();

			$reply["message"] = "Stage " . $shortName . " added.";
			$reply["success"] = true;

		}
		else
		{
			$reply["message"] = "Invalid request, missing values";
			$reply["success"] = false;
		}
	}

	// ========= GET STAGES ==========
	if ($reply["type"] == "getStages")
	{
		$reply["accepted"] = true;


		$rep = $bdd->query("SELECT name,shortName,uuid FROM " . $tablePrefix . "stages ORDER BY shortName,name,id;");
		$stages = Array();
		while ($stage = $rep->fetch())
		{
			$sta = Array();
			$sta['name'] = $stage['name'];
			$sta['shortName'] = $stage['shortName'];
			$sta['uuid'] = $stage['uuid'];
			$stages[] = $sta;
		}
		$rep->closeCursor();

		$reply["content"] = $stages;
		$reply["message"] = "Stages list retreived";
		$reply["success"] = true;

	}

	// ========= UPDATE STAGE ==========
	if ($reply["type"] == "updateStage")
	{
		$reply["accepted"] = true;

		$name = "";
		$shortName = "";
		$uuid = "";

		$data = json_decode(file_get_contents('php://input'));
		if ($data)
		{
			if (isset($data->{'name'})) $name = $data->{'name'};
			if (isset($data->{'shortName'})) $shortName = $data->{'shortName'};
			if (isset($data->{'uuid'})) $uuid = $data->{'uuid'};
		}

		if (strlen($name) > 0 AND strlen($shortName) > 0 AND strlen($uuid) > 0)
		{

			$rep = $bdd->prepare("UPDATE " . $tablePrefix . "stages SET name= :name ,shortName= :shortName WHERE uuid= :uuid ;");
			$rep->execute(array('name' => $name, 'shortName' => $shortName, 'uuid' => $uuid));
			$rep->closeCursor();

			$reply["message"] = "Stage " . $shortName . " (" . $uuid . ") updated.";
			$reply["success"] = true;

		}
		else
		{
			$reply["message"] = "Invalid request, missing values";
			$reply["success"] = false;
		}

	}

	// ========= REMOVE STAGE ==========
	if ($reply["type"] == "removeStage")
	{
		$reply["accepted"] = true;
		$id = "";

		$data = json_decode(file_get_contents('php://input'));
		if ($data)
		{
			if (isset($data->{'uuid'})) $uuid = $data->{'uuid'};
		}
		if (strlen($uuid) > 0)
		{

			$rep = $bdd->prepare("DELETE " . $tablePrefix . "stages FROM " . $tablePrefix . "stages WHERE uuid= :uuid ;");
			$rep->execute(array('uuid' => $uuid));
			$rep->closeCursor();

			$reply["message"] = "Stage " . $uuid . " removed.";
			$reply["success"] = true;

		}
		else
		{
			$reply["message"] = "Invalid request, missing values";
			$reply["success"] = false;
		}
	}
?>
