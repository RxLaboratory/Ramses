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
		$id = "";

		$data = json_decode(file_get_contents('php://input'));
		if ($data)
		{
			$name = $data->{'name'};
			$shortName = $data->{'shortName'};
			$id = $data->{'id'};
		}

		if (strlen($name) > 0 AND strlen($shortName) > 0)
		{
			if (strlen($id) > 0)
			{
				$qString = "INSERT INTO " . $tablePrefix . "projects (name,shortName,id) VALUES ( :name , :shortName , :id ) ON DUPLICATE KEY UPDATE name = VALUES(name) , shortName = VALUES(shortName);";
				$values = array('name' => $name, 'shortName' => $shortName, 'id' => $id);
			}
			else
			{
				$qString = "INSERT INTO " . $tablePrefix . "projects (name,shortName) VALUES ( :name , :shortName );";
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


		$rep = $bdd->query("SELECT name,shortName,id FROM " . $tablePrefix . "projects ORDER BY shortName;");
		$projects = Array();
		while ($project = $rep->fetch())
		{
			$proj = Array();
			$proj['name'] = $project['name'];
			$proj['shortName'] = $project['shortName'];
			$proj['id'] = (int)$project['id'];
			//get stages
			$projectStages = Array();
			$repS = $bdd->query("SELECT stageId FROM " . $tablePrefix . "projectstage JOIN " . $tablePrefix . "stages ON " . $tablePrefix . "stages.id = " . $tablePrefix . "projectstage.stageId WHERE projectId=" . $project['id'] . " ORDER BY " . $tablePrefix . "stages.shortName;");
			while ($projectStage = $repS->fetch())
			{
				$projectStages[] = (int)$projectStage['stageId'];
			}
			$proj['stages'] = $projectStages;
			//get shots
			$projectShots = Array();
			$repShots = $bdd->query("SELECT shotId FROM " . $tablePrefix . "projectshot WHERE projectId=" . $project['id'] . ";");
			while ($projectShot = $repShots->fetch())
			{
				$projectShots[] = (int)$projectShot['shotId'];
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
		$id = "";

		$data = json_decode(file_get_contents('php://input'));
		if ($data)
		{
			$name = $data->{'name'};
			$shortName = $data->{'shortName'};
			$id = $data->{'id'};
		}

		if (strlen($name) > 0 AND strlen($shortName) > 0 AND strlen($id) > 0)
		{

			$rep = $bdd->prepare("UPDATE " . $tablePrefix . "projects SET name= :name ,shortName= :shortName WHERE id= :id ;");
			$rep->execute(array('name' => $name,'shortName' => $shortName,'id' => $id));
			$rep->closeCursor();

			$reply["message"] = "Project " . $shortName . " (" . $id . ") updated.";
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
		$id = "";

		$data = json_decode(file_get_contents('php://input'));
		if ($data)
		{
			$id = $data->{'id'};
		}
		if (strlen($id) > 0)
		{

			$rep = $bdd->prepare("DELETE " . $tablePrefix . "projects FROM " . $tablePrefix . "projects WHERE id= :id ;");
			$rep->execute(array('id' => $id));
			$rep->closeCursor();

			$reply["message"] = "Project " . $id . " removed.";
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
			$stageId = $data->{'stageId'};
			$projectId = $data->{'projectId'};
		}

		if (strlen($stageId) > 0 AND strlen($projectId) > 0)
		{

			$rep = $bdd->prepare("INSERT INTO " . $tablePrefix . "projectstage (stageId,projectId) VALUES ( :stageId , :projectId ) ON DUPLICATE KEY UPDATE stageId = VALUES(stageId) , projectId = VALUES(projectId);");
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
			$stageId = $data->{'stageId'};
			$projectId = $data->{'projectId'};
		}
		if (strlen($projectId) > 0 AND strlen($stageId) > 0)
		{
			$rep = $bdd->prepare("DELETE " . $tablePrefix . "projectstage FROM " . $tablePrefix . "projectstage WHERE stageId= :stageId AND projectId= :projectId ;");
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
