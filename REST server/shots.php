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
			$qOrder = "UPDATE shots SET shotOrder = shotOrder + " . count($shots) . " WHERE shotOrder >= " . $shotOrder . " ;";

			try
			{
				//create shots
				$repOrder = $bdd->query($qOrder);
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
				$qShots = "INSERT INTO shots (id,name,projectId,shotOrder,duration) VALUES ";
				$reply["message"] = $qShots;

				$first = true;
				$order = (int)$shotOrder;
				foreach($shots as $shot)
				{
					if (!$first) $qShots = $qShots . ",";
					$qShots = $qShots . "('" . $shot->{'id'} . "','" . $shot->{'name'} . "'," . $projectId . "," . $order . "," . $shot->{'duration'} . ")";
					$order = $order + 1;
					$first = false;
				}

				$qShots = $qShots . " ON DUPLICATE KEY UPDATE duration = VALUES(duration);";

				//add shots
				try
				{
					//create shots
					$rep = $bdd->query($qShots);
					$rep->closeCursor();
					$reply["message"] = "Shots inserted.";
					$reply["success"] = true;
				}
				catch (Exception $e)
				{
					$reply["message"] = "Server issue: SQL Query failed adding shots. |\n" . $qShots;
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

		$q = "SELECT shots.name as shotName,shots.duration,shots.id as shotId,shots.shotOrder FROM shots WHERE projectId= :projectId ORDER BY shots.shotOrder,shots.name;";

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
			$reply["message"] = "Server issue: SQL Query failed retrieving shots list. | " . $q;
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
			$q = "UPDATE shots SET name='" . $name . "',duration=" . $duration . " WHERE id=" . $id . ";";
			try
			{
				$rep = $bdd->query($q);
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
			$q = "DELETE shots FROM shots WHERE";
			$first = true;
			foreach($ids as $id)
			{
				if (!$first) $q = $q . " OR";
				$q = $q . " id=" . $id;
				$first = false;
			}
			$q = $q . ";";
			try
			{
				$rep = $bdd->query($q);
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

	// ========= MOVE SHOT UP =========
	if ($reply["type"] == "moveShotsUp")
	{
		$reply["accepted"] = true;

		$data = json_decode(file_get_contents('php://input'));
		if ($data)
		{
			$ids = $data->{'ids'};
		}

		if (isset($ids) AND count($ids) > 0)
		{
			$q = "";

			//make sure the shots are sorted
			sort($ids);

			foreach($ids as $id)
			{
				//get this shot order and the order just before
				$qOrders = "SELECT shotOrder,id FROM shots WHERE shotOrder <= (SELECT shotOrder FROM shots WHERE id=" . $id . ") ORDER BY shotOrder DESC;";

				try
				{
					$repOrder = $bdd->query($qOrders);
					$orderCurrent = $repOrder->fetch();
					$orderBefore = $repOrder->fetch();
					$repOrder->closeCursor();



					$q = $q . "UPDATE shots SET shotOrder=" . $orderCurrent["shotOrder"] . " WHERE id=" . $orderBefore['id'] . ";\n";
					$q = $q . "UPDATE shots SET shotOrder=" . $orderBefore["shotOrder"] . " WHERE id=" . $orderCurrent['id'] . ";\n";

					$rep = $bdd->query($q);
					$rep->closeCursor();

					$reply["message"] = "Shot order successfully changed.";
					$reply["success"] = true;
				}
				catch (Exception $e)
				{
					$reply["message"] = "Server issue: SQL Query failed retrieving shots orders. | " . $q;
					$reply["success"] = false;
					break;
				}
			}
		}
		else
		{
			$reply["message"] = "Invalid request, missing values";
			$reply["success"] = false;
		}
	}

	// ========= MOVE SHOT DOWN =========
	if ($reply["type"] == "moveShotsDown")
	{
		$reply["accepted"] = true;

		$data = json_decode(file_get_contents('php://input'));
		if ($data)
		{
			$ids = $data->{'ids'};
		}

		if (isset($ids) AND count($ids) > 0)
		{
			$q = "";

			//make sure the shots are sorted
			rsort($ids);

			foreach($ids as $id)
			{
				//get this shot order and the order just before
				$qOrders = "SELECT shotOrder,id FROM shots WHERE shotOrder >= (SELECT shotOrder FROM shots WHERE id=" . $id . ") ORDER BY shotOrder ASC;";

				try
				{
					$repOrder = $bdd->query($qOrders);
					$orderCurrent = $repOrder->fetch();
					$orderAfter = $repOrder->fetch();
					$repOrder->closeCursor();

					if (count($orderAfter) > 0 AND count($orderCurrent) > 0)
					{
						$q = $q . "UPDATE shots SET shotOrder=" . $orderCurrent["shotOrder"] . " WHERE id=" . $orderAfter['id'] . ";\n";
						$q = $q . "UPDATE shots SET shotOrder=" . $orderAfter["shotOrder"] . " WHERE id=" . $orderCurrent['id'] . ";\n";

						$rep = $bdd->query($q);
						$rep->closeCursor();
					}

					$reply["message"] = "Shot order successfully changed.";
					$reply["success"] = true;
				}
				catch (Exception $e)
				{
					$reply["message"] = "Server issue: SQL Query failed retrieving shots orders. | " . $q;
					$reply["success"] = false;
					break;
				}
			}
		}
		else
		{
			$reply["message"] = "Invalid request, missing values";
			$reply["success"] = false;
		}
	}


?>
