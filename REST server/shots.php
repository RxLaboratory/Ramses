<?php
	/*
		Rainbox Asset Manager
		Shots management
	*/

	if ($reply["type"] == "addShots")
	{
		$reply["accepted"] = true;

		$data = json_decode(file_get_contents('php://input'));
		if ($data)
		{
			if (isset($data->{'projectId'})) $projectId = $data->{'projectId'};
			if (isset($data->{'shotOrder'})) $shotOrder = $data->{'shotOrder'};
			if (isset($data->{'shots'})) $shots = $data->{'shots'};
		}

		if (isset($projectId) AND strlen($projectId) > 0 AND isset($shots) AND count($shots) > 0)
		{
			if (!isset($shotOrder)) $shotOrder = 0;

			//update order of shots after the ones we insert
			$qOrder = "UPDATE shots JOIN projectshot ON projectshot.shotId = shots.id  SET projectshot.shotOrder = projectshot.shotOrder + :shotsCount WHERE projectshot.shotOrder >= :shotOrder ;";

			try
			{
				//create shots
				$repOrder = $bdd->prepare($qOrder);
				$repOrder->execute(array('shotsCount' => count($shots), 'shotOrder' => $shotOrder));
				$repOrder->closeCursor();
			}
			catch (Exception $e)
			{
				$reply["message"] = "Server issue: SQL Query failed moving shots. | " + $qOrder;
				$reply["success"] = false;
			}

			if (isset($repOrder))
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
				//add assignment query
				$qShots = $qShots . "INSERT INTO projectshot (shotId,projectId,shotOrder) VALUES ";
				$qShots = $qShots . implode(",",$placeHolders) . " ;";

				$order = (int)$shotOrder;
				foreach($shots as $shot)
				{
					$values[] = $shot->{'id'};
					$values[] = $projectId;
					$values[] = $order;
					$order = $order + 1;
				}

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
		}
		else
		{
			$reply["message"] = "Invalid request, missing values";
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

		$data = json_decode(file_get_contents('php://input'));
		if ($data)
		{
			$ids = $data->{'ids'};
		}

		if (isset($ids) AND count($ids) > 0)
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
			$q = $q . implode(" OR ",$placeHolders) . ";";
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
