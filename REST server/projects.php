<?php
/*
		Rainbox Asset Manager
		Projects management
	*/
	// ========= ADD PROJECT ==========
	if ($reply["type"] == "addProject")
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
				$qString = "INSERT INTO " . $tablePrefix . "projects (name,shortName,uuid) VALUES ( :name , :shortName , :uuid ) ON DUPLICATE KEY UPDATE name = VALUES(name) , shortName = VALUES(shortName);";
				$values = array('name' => $name, 'shortName' => $shortName, 'uuid' => $uuid);
			}
			else
			{
				$qString = "INSERT INTO " . $tablePrefix . "projects (name,shortName,uuid) VALUES ( :name , :shortName , uuid() );";
				$values = array('name' => $name, 'shortName' => $shortName);
			}

			$rep = $bdd->prepare($qString);
			$rep->execute($values);
			$rep->closeCursor();

			$reply["message"] = "Project " . $shortName . " added.";
			$reply["success"] = true;

		}
		else
		{
			$reply["message"] = "Invalid request, missing values";
			$reply["success"] = false;
		}
	}

	// ========= GET PROJECTS ==========
	if ($reply["type"] == "getProjects")
	{
		$reply["accepted"] = true;


		$rep = $bdd->query("SELECT name,shortName,uuid,id FROM " . $tablePrefix . "projects ORDER BY shortName;");
		$projects = Array();
		while ($project = $rep->fetch())
		{
			$proj = Array();
			$proj['name'] = $project['name'];
			$proj['shortName'] = $project['shortName'];
			$proj['uuid'] = $project['uuid'];
			//get stages
			$projectStages = Array();
			$repS = $bdd->query("SELECT " . $tablePrefix . "stages.uuid as stageId FROM " . $tablePrefix . "projectstage JOIN " . $tablePrefix . "stages ON " . $tablePrefix . "stages.id = " . $tablePrefix . "projectstage.stageId WHERE projectId=" . $project['id'] . " ORDER BY " . $tablePrefix . "stages.shortName;");
			while ($projectStage = $repS->fetch())
			{
				$projectStages[] = $projectStage['stageId'];
			}
			$proj['stages'] = $projectStages;
			//get shots
			$projectShots = Array();
			$repShots = $bdd->query("SELECT " . $tablePrefix . "shots.uuid as shotId FROM " . $tablePrefix . "projectshot JOIN " . $tablePrefix . "shots ON " . $tablePrefix . "shots.id = " . $tablePrefix . "projectshot.shotId WHERE projectId=" . $project['id'] . ";");
			while ($projectShot = $repShots->fetch())
			{
				$projectShots[] = $projectShot['shotId'];
			}
			$projects[] = $proj;
		}
		$rep->closeCursor();

		$reply["content"] = $projects;
		$reply["message"] = "Projects list retrieved";
		$reply["success"] = true;

	}

	// ========= UPDATE PROJECT ==========
	if ($reply["type"] == "updateProject")
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

			$rep = $bdd->prepare("UPDATE " . $tablePrefix . "projects SET name= :name ,shortName= :shortName WHERE uuid= :uuid ;");
			$rep->execute(array('name' => $name,'shortName' => $shortName,'uuid' => $uuid));
			$rep->closeCursor();

			$reply["message"] = "Project " . $shortName . " (" . $uuid . ") updated.";
			$reply["success"] = true;

		}
		else
		{
			$reply["message"] = "Invalid request, missing values";
			$reply["success"] = false;
		}

	}

	// ========= REMOVE PROJECT ==========
	if ($reply["type"] == "removeProject")
	{
		$reply["accepted"] = true;
		$uuid = "";

		$data = json_decode(file_get_contents('php://input'));
		if ($data)
		{
			if (isset($data->{'uuid'})) $uuid = $data->{'uuid'};
		}
		if (strlen($uuid) > 0)
		{

			$rep = $bdd->prepare("DELETE " . $tablePrefix . "projects FROM " . $tablePrefix . "projects WHERE uuid= :uuid ;");
			$rep->execute(array('uuid' => $uuid));
			$rep->closeCursor();

			$reply["message"] = "Project " . $uuid . " removed.";
			$reply["success"] = true;

		}
		else
		{
			$reply["message"] = "Invalid request, missing values";
			$reply["success"] = false;
		}
	}

	// ========= ASSOCIATE STAGE WITH PROJECT ==========
	if ($reply["type"] == "addProjectStage")
	{
		$reply["accepted"] = true;

		$stageId = "";
		$projectId = "";

		$data = json_decode(file_get_contents('php://input'));
		if ($data)
		{
			if (isset($data->{'stageId'})) $stageId = $data->{'stageId'};
			if (isset($data->{'projectId'})) $projectId = $data->{'projectId'};
		}

		if (strlen($stageId) > 0 AND strlen($projectId) > 0)
		{
			$q = "INSERT INTO " . $tablePrefix . "projectstage (stageId,projectId) VALUES (
			( SELECT " . $tablePrefix . "stages.id FROM " . $tablePrefix . "stages WHERE " . $tablePrefix . "stages.uuid = :stageId )
			,
			( SELECT " . $tablePrefix . "projects.id FROM " . $tablePrefix . "projects WHERE " . $tablePrefix . "projects.uuid = :projectId )
			) ON DUPLICATE KEY UPDATE " . $tablePrefix . "projectstage.id = " . $tablePrefix . "projectstage.id ;";

			$rep = $bdd->prepare($q);
			$rep->execute(array('stageId' => $stageId,'projectId' => $projectId));
			$rep->closeCursor();

			$reply["message"] = "Stage " . $stageId . " associated with project " . $projectId . ".";
			$reply["success"] = true;

		}
		else
		{
			$reply["message"] = "Invalid request, missing values";
			$reply["success"] = false;
		}
	}

	// ========= REMOVE STAGE FROM PROJECT ==========
	if ($reply["type"] == "removeProjectStage")
	{
		$reply["accepted"] = true;
		$stageId = "";
		$projectId = "";

		$data = json_decode(file_get_contents('php://input'));
		if ($data)
		{
			if (isset($data->{'stageId'})) $stageId = $data->{'stageId'};
			if (isset($data->{'projectId'})) $projectId = $data->{'projectId'};
		}
		if (strlen($projectId) > 0 AND strlen($stageId) > 0)
		{
			$q = "DELETE " . $tablePrefix . "projectstage FROM " . $tablePrefix . "projectstage WHERE
			stageId= ( SELECT " . $tablePrefix . "stages.id FROM " . $tablePrefix . "stages WHERE " . $tablePrefix . "stages.uuid = :stageId )
			AND
			projectId= ( SELECT " . $tablePrefix . "projects.id FROM " . $tablePrefix . "projects WHERE " . $tablePrefix . "projects.uuid = :projectId )
			;";
			$rep = $bdd->prepare($q);
			$rep->execute(array('stageId' => $stageId,'projectId' => $projectId));
			$rep->closeCursor();

			$reply["message"] = "Stage " . $stageId . " removed from project " . $projectId . ".";
			$reply["success"] = true;

		}
		else
		{
			$reply["message"] = "Invalid request, missing values";
			$reply["success"] = false;
		}
	}
?>
