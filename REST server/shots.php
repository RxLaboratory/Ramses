<?php
	/*
		Rainbox Asset Manager
		Shots management
	*/

	if ($reply["type"] == "addShots")
	{
		$reply["accepted"] = true;

		$shots = array();

		$data = json_decode(file_get_contents('php://input'));
		if ($data)
		{
			if (isset($data->{'shots'})) $shots = $data->{'shots'};
		}

		if (count($shots) > 0)
		{
			//construct add shots query
			$qShots = "INSERT INTO " . $tablePrefix . "shots (uuid,name,duration) VALUES ";
			$placeHolder = "(?,?,?)";

			$placeHolders = array();
			$values = array();
			foreach($shots as $shot)
			{
				$placeHolders[] = $placeHolder;
				$values[] = $shot->{'uuid'};
				$values[] = $shot->{'name'};
				$values[] = $shot->{'duration'};
			}

			$qShots = $qShots . implode(",",$placeHolders);
			$qShots = $qShots . " ON DUPLICATE KEY UPDATE duration = VALUES(duration);\n";

			//create shots
			$rep = $bdd->prepare($qShots);
			$rep->execute($values);
			$rep->closeCursor();
			$reply["message"] = "Shots inserted.";
			$reply["success"] = true;

		}
		else
		{
			$reply["message"] = "Invalid request, missing values";
			$reply["success"] = false;
		}
	}

	if ($reply["type"] == "insertShots")
	{
		$reply["accepted"] = true;

		$shots = array();
		$shotOrder = 0;
		$projectId = "";

		$data = json_decode(file_get_contents('php://input'));
		if ($data)
		{
			if (isset($data->{'shots'})) $shots = $data->{'shots'};
			if (isset($data->{'projectId'})) $projectId = $data->{'projectId'};
			if (isset($data->{'shotOrder'})) $shotOrder = (int)$data->{'shotOrder'};
		}

		if (count($shots) > 0 AND strlen($projectId) > 0)
		{
			//update order of shots after the ones we insert
			$qOrder = "UPDATE " . $tablePrefix . "shots JOIN " . $tablePrefix . "projectshot ON " . $tablePrefix . "projectshot.shotId = " . $tablePrefix . "shots.id
			SET " . $tablePrefix . "projectshot.shotOrder = " . $tablePrefix . "projectshot.shotOrder + :shotsCount
			WHERE " . $tablePrefix . "projectshot.shotOrder >= :shotOrder
			AND " . $tablePrefix . "projectshot.projectId = (SELECT " . $tablePrefix . "projects.id FROM " . $tablePrefix . "projects WHERE " . $tablePrefix . "projects.uuid = :projectId ) ;";

			//update order
			$repOrder = $bdd->prepare($qOrder);
			$repOrder->execute(array('shotsCount' => count($shots), 'shotOrder' => $shotOrder, 'projectId' => $projectId ));
			$repOrder->closeCursor();


			if (isset($repOrder))
			{
				$q = "INSERT INTO " . $tablePrefix . "projectshot (shotId,projectId,shotOrder) VALUES ";
				$placeHolder = "((SELECT " . $tablePrefix . "shots.id FROM " . $tablePrefix . "shots WHERE " . $tablePrefix . "shots.uuid = ? ),(SELECT " . $tablePrefix . "projects.id FROM " . $tablePrefix . "projects WHERE " . $tablePrefix . "projects.uuid = ? ),?)";

				$placeHolders = array();
				$values = array();

				foreach($shots as $shot)
				{
					$placeHolders[] = $placeHolder;
					$values[] = $shot->{'shotId'};
					$values[] = $projectId;
					$values[] = $shotOrder;
					$shotOrder = $shotOrder + 1;
				}

				$q = $q . implode(",",$placeHolders);
				$q = $q . " ON DUPLICATE KEY UPDATE shotOrder = VALUES(shotOrder);";

				$rep = $bdd->prepare($q);
				$rep->execute($values);
				$rep->closeCursor();

				$reply["message"] = "Shots assigned.";
				$reply["success"] = true;
			}
		}
		else
		{
			$reply["message"] = "Invalid request, missing values.";
			$reply["success"] = false;
		}

	}

	if ($reply["type"] == "addInsertShots")
	{
		$reply["accepted"] = true;

		$shots = array();
		$shotOrder = 0;
		$projectId = "";

		$data = json_decode(file_get_contents('php://input'));
		if ($data)
		{
			if (isset($data->{'shots'})) $shots = $data->{'shots'};
			if (isset($data->{'projectId'})) $projectId = $data->{'projectId'};
			if (isset($data->{'shotOrder'})) $shotOrder = (int)$data->{'shotOrder'};
		}

		if (count($shots) > 0 AND strlen($projectId) > 0)
		{
			//update order of shots after the ones we insert
			$qOrder = "UPDATE " . $tablePrefix . "shots JOIN " . $tablePrefix . "projectshot ON " . $tablePrefix . "projectshot.shotId = " . $tablePrefix . "shots.id
			SET " . $tablePrefix . "projectshot.shotOrder = " . $tablePrefix . "projectshot.shotOrder + :shotsCount
			WHERE " . $tablePrefix . "projectshot.shotOrder >= :shotOrder
			AND " . $tablePrefix . "projectshot.projectId = (SELECT " . $tablePrefix . "projects.id FROM " . $tablePrefix . "projects WHERE " . $tablePrefix . "projects.uuid = :projectId ) ;";

			//create shots
			$repOrder = $bdd->prepare($qOrder);
			$repOrder->execute(array('shotsCount' => count($shots), 'shotOrder' => $shotOrder, 'projectId' => $projectId ));
			$repOrder->closeCursor();

			$qShots = "INSERT INTO " . $tablePrefix . "shots (uuid,name,duration) VALUES ";
			$qInsert = "INSERT INTO " . $tablePrefix . "projectshot (shotId,projectId,shotOrder) VALUES ";
			$placeHolderShots = "(?,?,?)";
			$placeHolderInsert = "((SELECT " . $tablePrefix . "shots.id FROM " . $tablePrefix . "shots WHERE " . $tablePrefix . "shots.uuid = ? ),(SELECT " . $tablePrefix . "projects.id FROM " . $tablePrefix . "projects WHERE " . $tablePrefix . "projects.uuid = ? ),?)";

			$placeHoldersShots = array();
			$placeHoldersInsert = array();
			$values = array();

			//addShots values
			foreach($shots as $shot)
			{
				$placeHoldersShots[] = $placeHolderShots;
				$values[] = $shot->{'uuid'};
				$values[] = $shot->{'name'};
				$values[] = $shot->{'duration'};
			}
			//insert values
			foreach($shots as $shot)
			{
				$placeHoldersInsert[] = $placeHolderInsert;
				$values[] = $shot->{'uuid'};
				$values[] = $projectId;
				$values[] = $shotOrder;
				$shotOrder = $shotOrder + 1;
			}

			$qShots = $qShots . implode(",",$placeHoldersShots);
			$qShots = $qShots . " ON DUPLICATE KEY UPDATE duration = VALUES(duration);\n";
			$qInsert = $qInsert . implode(",",$placeHoldersInsert);
			$qInsert = $qInsert . " ON DUPLICATE KEY UPDATE shotOrder = VALUES(shotOrder);";
			$q = $qShots . $qInsert;

			$rep = $bdd->prepare($q);
			$rep->execute($values);
			$rep->closeCursor();

			$reply["message"] = "Shots added and inserted.";
			$reply["success"] = true;

		}
		else
		{
			$reply["message"] = "Invalid request, missing values.";
			$reply["success"] = false;
		}
	}

	// ========= GET SHOTS ==========
	if ($reply["type"] == "getShots")
	{
		$reply["accepted"] = true;

		$projectId = "";

		$data = json_decode(file_get_contents('php://input'));
		if ($data)
		{
			if (isset($data->{'projectId'})) $projectId = $data->{'projectId'};
		}

		$q = "SELECT " . $tablePrefix . "shots.name as shotName," . $tablePrefix . "shots.duration," . $tablePrefix . "shots.uuid as shotId," . $tablePrefix . "projectshot.shotOrder as shotOrder
		FROM " . $tablePrefix . "shots
		JOIN " . $tablePrefix . "projectshot ON " . $tablePrefix . "projectshot.shotId = " . $tablePrefix . "shots.id
		WHERE projectId= (SELECT " . $tablePrefix . "projects.id FROM " . $tablePrefix . "projects WHERE " . $tablePrefix . "projects.uuid = :projectId )
		ORDER BY " . $tablePrefix . "projectshot.shotOrder," . $tablePrefix . "shots.name;";

		//get shots
		$rep = $bdd->prepare($q);
		$rep->execute(array('projectId' => $projectId));

		$shots = Array();

		while ($shot = $rep->fetch())
		{
			$s = Array();
			$s['shotName'] = $shot['shotName'];
			$s['duration'] = (double)$shot['duration'];
			$s['shotId'] = $shot['shotId'];
			$s['shotOrder'] = (int)$shot['shotOrder'];

			$shots[] = $s;
		}
		$rep->closeCursor();

		$reply["content"] = $shots;
		$reply["message"] = "Shots list retrieved ";
		$reply["success"] = true;

	}

	// ========= UPDATE SHOT ==========
	if ($reply["type"] == "updateShot")
	{
		$reply["accepted"] = true;

		$name = "";
		$duration = "";
		$uuid = "";

		$data = json_decode(file_get_contents('php://input'));
		if ($data)
		{
			if (isset($data->{'name'})) $name = $data->{'name'};
			if (isset($data->{'duration'})) $duration = $data->{'duration'};
			if (isset($data->{'uuid'})) $uuid = $data->{'uuid'};
		}

		if (strlen($name) > 0 AND strlen($duration) > 0 AND strlen($uuid) > 0)
		{
			$q = "UPDATE " . $tablePrefix . "shots SET name= :name ,duration= :duration WHERE uuid= :uuid ;";

			$rep = $bdd->prepare($q);
			$rep->execute(array('name' => $name, 'duration' => $duration, 'uuid' => $uuid));
			$rep->closeCursor();

			$reply["message"] = "Shot " . $name . " (" . $uuid . ") updated.";
			$reply["success"] = true;

		}
		else
		{
			$reply["message"] = "Invalid request, missing values";
			$reply["success"] = false;
		}
	}

	if ($reply["type"] == "updateShots")
	{
		$reply["accepted"] = true;

		$shots = array();

		$data = json_decode(file_get_contents('php://input'));
		if ($data)
		{
			if (isset($data->{'shots'})) $shots = $data->{'shots'};
		}

		if (count($shots) > 0)
		{
			//construct add shots query
			$qShots = "INSERT INTO " . $tablePrefix . "shots (uuid,name,duration) VALUES ";
			$placeHolder = "(?,?,?)";

			$placeHolders = array();
			$values = array();
			foreach($shots as $shot)
			{
				$placeHolders[] = $placeHolder;
				$values[] = $shot->{'uuid'};
				$values[] = $shot->{'name'};
				$values[] = $shot->{'duration'};
			}

			$qShots = $qShots . implode(",",$placeHolders);
			$qShots = $qShots . " ON DUPLICATE KEY UPDATE duration = VALUES(duration), name = VALUES(name);\n";

			//update shots
			$rep = $bdd->prepare($qShots);
			$rep->execute($values);
			$rep->closeCursor();
			$reply["message"] = "Shots inserted.";
			$reply["success"] = true;
		}
		else
		{
			$reply["message"] = "Invalid request, missing values";
			$reply["success"] = false;
		}
	}

	// ========= REMOVE SHOT ==========
	if ($reply["type"] == "removeShots")
	{
		$reply["accepted"] = true;

		$ids = array();
		$projectId = "";

		$data = json_decode(file_get_contents('php://input'));
		if ($data)
		{
			if (isset($data->{'ids'})) $ids = $data->{'ids'};
			if (isset($data->{'projectId'})) $projectId = $data->{'projectId'};
		}

		if (count($ids) > 0 AND strlen($projectId) > 0)
		{
			$q = "DELETE " . $tablePrefix . "shots FROM " . $tablePrefix . "shots WHERE ";
			$placeHolder = "uuid = ?";
			$placeHolders = array();
			$values = array();
			foreach($ids as $id)
			{
				$placeHolders[] = $placeHolder;
				$values[] = $id;
			}
			$q = $q . implode(" OR ",$placeHolders) . ";\n";
			//reset the order
			$q = $q . "SET @count = -1;\n";
			$q = $q . "UPDATE " . $tablePrefix . "projectshot SET " . $tablePrefix . "projectshot.shotOrder = @count:= @count + 1 WHERE " . $tablePrefix . "projectshot.projectId = (SELECT " . $tablePrefix . "projects.id FROM " . $tablePrefix . "projects WHERE " . $tablePrefix . "projects.uuid = ? ) ;";
			$values[] = $projectId;


			$rep = $bdd->prepare($q);
			$rep->execute($values);
			$rep->closeCursor();

			$reply["message"] = "Multiple shots removed.";
			$reply["success"] = true;

		}
		else
		{
			$reply["message"] = "Invalid request, missing values";
			$reply["success"] = false;
		}
	}

	// ========= RESET SHOT ORDER ======
	if ($reply["type"] == "resetShotsOrder")
	{
		$reply["accepted"] = true;

		$data = json_decode(file_get_contents('php://input'));
		if ($data)
		{
			if (isset($data->{'ids'})) $ids = $data->{'ids'};
		}

		if (isset($ids) AND count($ids) > 0)
		{
			$shotOrder = 0;
			$first = true;
			$queries = array();
			$values = array();
			foreach($ids as $id)
			{
				$queries[] = "UPDATE " . $tablePrefix . "projectshot SET shotOrder = ? WHERE shotId = (SELECT id FROM " . $tablePrefix . "shots WHERE uuid = ? ) ;";
				$values[] = $shotOrder;
				$values[] = $id;
				$shotOrder = $shotOrder + 1;
			}


			$rep = $bdd->prepare(implode("\n",$queries));
			$rep->execute($values);
			$rep->closeCursor();

			$reply["message"] = "Shot order successfully changed.";
			$reply["success"] = true;

		}
		else
		{
			$reply["message"] = "Invalid request, missing values";
			$reply["success"] = false;
		}
	}

?>
