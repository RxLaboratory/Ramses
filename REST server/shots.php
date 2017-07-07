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
			$qShots = "INSERT INTO shots (id,name,duration) VALUES ";
			$placeHolder = "(?,?,?)";

			$placeHolders = array();
			$values = array();
			foreach($shots as $shot)
			{
				$placeHolders[] = $placeHolder;
				$values[] = $shot->{'id'};
				$values[] = $shot->{'name'};
				$values[] = $shot->{'duration'};
			}

			$qShots = $qShots . implode(",",$placeHolders);
			$qShots = $qShots . " ON DUPLICATE KEY UPDATE duration = VALUES(duration);\n";

			//add shots
			try
			{
				//create shots
				$rep = $bdd->prepare($qShots);
				$rep->execute($values);
				$rep->closeCursor();
				$reply["message"] = "Shots inserted.";
				$reply["success"] = true;
			}
			catch (Exception $e)
			{
				$reply["message"] = "Server issue: SQL Query failed adding shots. |\n" . $qShots ;
				$reply["success"] = false;
			}
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
			$qOrder = "UPDATE shots JOIN projectshot ON projectshot.shotId = shots.id
			SET projectshot.shotOrder = projectshot.shotOrder + :shotsCount
			WHERE projectshot.shotOrder >= :shotOrder AND projectshot.projectId = :projectId ;";

			try
			{
				//update order
				$repOrder = $bdd->prepare($qOrder);
				$repOrder->execute(array('shotsCount' => count($shots), 'shotOrder' => $shotOrder, 'projectId' => $projectId ));
				$repOrder->closeCursor();
			}
			catch (Exception $e)
			{
				$reply["message"] = "Server issue: SQL Query failed moving shots. | " + $qOrder;
				$reply["success"] = false;
			}

			if (isset($repOrder))
			{
				$q = "INSERT INTO projectshot (shotId,projectId,shotOrder) VALUES ";
				$placeHolder = "(?,?,?)";

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
			$qOrder = "UPDATE shots JOIN projectshot ON projectshot.shotId = shots.id
			SET projectshot.shotOrder = projectshot.shotOrder + :shotsCount
			WHERE projectshot.shotOrder >= :shotOrder AND projectshot.projectId = :projectId ;";

			//create shots
			$repOrder = $bdd->prepare($qOrder);
			$repOrder->execute(array('shotsCount' => count($shots), 'shotOrder' => $shotOrder, 'projectId' => $projectId ));
			$repOrder->closeCursor();

			$qShots = "INSERT INTO shots (id,name,duration) VALUES ";
			$qInsert = "INSERT INTO projectshot (shotId,projectId,shotOrder) VALUES ";
			$placeHolder = "(?,?,?)";

			$placeHolders = array();
			$values = array();

			//addShots values
			foreach($shots as $shot)
			{
				$placeHolders[] = $placeHolder;
				$values[] = $shot->{'id'};
				$values[] = $shot->{'name'};
				$values[] = $shot->{'duration'};
			}
			//insert values
			foreach($shots as $shot)
			{
				$values[] = $shot->{'id'};
				$values[] = $projectId;
				$values[] = $shotOrder;
				$shotOrder = $shotOrder + 1;
			}

			$qShots = $qShots . implode(",",$placeHolders);
			$qShots = $qShots . " ON DUPLICATE KEY UPDATE duration = VALUES(duration);\n";
			$qInsert = $qInsert . implode(",",$placeHolders);
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
			$projectId = $data->{'projectId'};
		}

		$q = "SELECT shots.name as shotName,shots.duration,shots.id as shotId,projectshot.shotOrder as shotOrder
		FROM shots
		JOIN projectshot ON projectshot.shotId = shots.id
		WHERE projectId= :projectId
		ORDER BY projectshot.shotOrder,shots.name;";

		try
		{
			//get shots
			$rep = $bdd->prepare($q);
			$rep->execute(array('projectId' => $projectId));

			$shots = Array();

			while ($shot = $rep->fetch())
			{
				$s = Array();
				$s['shotName'] = $shot['shotName'];
				$s['duration'] = (double)$shot['duration'];
				$s['shotId'] = (int)$shot['shotId'];
				$s['shotOrder'] = (int)$shot['shotOrder'];

				$shots[] = $s;
			}
			$rep->closeCursor();

			$reply["content"] = $shots;
			$reply["message"] = "Shots list retrieved ";
			$reply["success"] = true;
		}
		catch (Exception $e)
		{
			$reply["message"] = "Server issue: SQL Query failed retrieving shots list. |\n" . $q;
			$reply["success"] = false;
		}
	}

	// ========= UPDATE SHOT ==========
	if ($reply["type"] == "updateShot")
	{
		$reply["accepted"] = true;

		$name = "";
		$duration = "";
		$id = "";

		$data = json_decode(file_get_contents('php://input'));
		if ($data)
		{
			$name = $data->{'name'};
			$duration = $data->{'duration'};
			$id = $data->{'id'};
		}

		if (strlen($name) > 0 AND strlen($duration) > 0 AND strlen($id) > 0)
		{
			$q = "UPDATE shots SET name= :name ,duration= :duration WHERE id= :id ;";
			try
			{
				$rep = $bdd->prepare($q);
				$rep->execute(array('name' => $name, 'duration' => $duration, 'id' => $id));
				$rep->closeCursor();

				$reply["message"] = "Shot " . $name . " (" . $id . ") updated.";
				$reply["success"] = true;
			}
			catch (Exception $e)
			{
				$reply["message"] = "Server issue: SQL Query failed updating shot " . $name . ". | " . $q;
				$reply["success"] = false;
			}
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
			$q = "DELETE shots FROM shots WHERE ";
			$placeHolder = "id = ?";
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
			$q = $q . "UPDATE projectshot SET projectshot.shotOrder = @count:= @count + 1 WHERE projectshot.projectId = ? ;";
			$values[] = $projectId;

			try
			{
				$rep = $bdd->prepare($q);
				$rep->execute($values);
				$rep->closeCursor();

				$reply["message"] = "Multiple shots removed.";
				$reply["success"] = true;
			}
			catch (Exception $e)
			{
				$reply["message"] = "Server issue: SQL Query failed deleting multiple shots | " . $q;
				$reply["success"] = false;
			}
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
				$queries[] = "UPDATE projectshot SET shotOrder = ? WHERE shotId = ? ;";
				$values[] = $shotOrder;
				$values[] = $id;
				$shotOrder = $shotOrder + 1;
			}

			try
			{
				$rep = $bdd->prepare(implode("\n",$queries));
				$rep->execute($values);
				$rep->closeCursor();

				$reply["message"] = "Shot order successfully changed.";
				$reply["success"] = true;
			}
			catch (Exception $e)
			{
				$reply["message"] = "Server issue: SQL Query failed setting shots orders. | " . $qString;
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
