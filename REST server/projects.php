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
				$qString = "INSERT INTO projects (name,shortName,id) VALUES ('" . $name . "','" . $shortName . "'," . $id . ");";
			}
			else
			{
				$qString = "INSERT INTO projects (name,shortName) VALUES ('" . $name . "','" . $shortName . "');";
			}
			try
			{
				$rep = $bdd->query($qString);
				$rep->closeCursor();

				$reply["message"] = "Project " . $shortName . " added.";
				$reply["success"] = true;
			}
			catch (Exception $e)
			{
				$reply["message"] = "Server issue: SQL Query failed adding project " . $shortName . ".";
				$reply["success"] = false;
			}
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

		try
		{
			$rep = $bdd->query("SELECT name,shortName,id FROM projects ORDER BY shortName;");
			$projects = Array();
			while ($project = $rep->fetch())
			{
				$proj = Array();
				$proj['name'] = $project['name'];
				$proj['shortName'] = $project['shortName'];
				$proj['id'] = (int)$project['id'];
				//get stages
				$projectStages = Array();
				$repS = $bdd->query("SELECT stageId FROM projectstage WHERE projectId=" . $project['id'] . ";");
				while ($projectStage = $repS->fetch())
				{
					$projectStages[] = (int)$projectStage['stageId'];
				}
				$proj['stages'] = $projectStages;
				//get shots
				$projectShots = Array();
				$repShots = $bdd->query("SELECT shotId FROM projectshot WHERE projectId=" . $project['id'] . ";");
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
		catch (Exception $e)
		{
			$reply["message"] = "Server issue: SQL Query failed retrieving projects list.";
			$reply["success"] = false;
		}
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
			try
			{
				$rep = $bdd->query("UPDATE projects SET name='" . $name . "',shortName='" . $shortName . "' WHERE id=" . $id . ";");
				$rep->closeCursor();

				$reply["message"] = "Project " . $shortName . " (" . $id . ") updated.";
				$reply["success"] = true;
			}
			catch (Exception $e)
			{
				$reply["message"] = "Server issue: SQL Query failed updating project " . $shortName . ".";
				$reply["success"] = false;
			}
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
			try
			{
				$rep = $bdd->query("DELETE projects FROM projects WHERE id=" . $id . ";");
				$rep->closeCursor();

				$reply["message"] = "Project " . $id . " removed.";
				$reply["success"] = true;
			}
			catch (Exception $e)
			{
				$reply["message"] = "Server issue: SQL Query failed deleting project " . $id . ".";
				$reply["success"] = false;
			}
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
			try
			{
				$rep = $bdd->query("INSERT INTO projectstage (stageId,projectId) VALUES (" . $stageId . "," . $projectId . ");");
				$rep->closeCursor();

				$reply["message"] = "Stage " . $stageId . " associated with project " . $projectId . ".";
				$reply["success"] = true;
			}
			catch (Exception $e)
			{
				$reply["message"] = "Server issue: SQL Query failed associating stage " . $stageId . " with project " . $projectId . " | " . "INSERT INTO projectstage (stageId,projectId) VALUES (" . $stageId . "," . $projectId . ");";
				$reply["success"] = false;
			}
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
			try
			{
				$rep = $bdd->query("DELETE projectstage FROM projectstage WHERE stageId=" . $stageId . " AND projectId=" . $projectId . ";");
				$rep->closeCursor();

				$reply["message"] = "Stage " . $stageId . " removed from project " . $projectId . ".";
				$reply["success"] = true;
			}
			catch (Exception $e)
			{
				$reply["message"] = "Server issue: SQL Query failed removing stage " . $stageId . " from project " . $projectId . ".";
				$reply["success"] = false;
			}
		}
		else
		{
			$reply["message"] = "Invalid request, missing values";
			$reply["success"] = false;
		}
	}
?>
