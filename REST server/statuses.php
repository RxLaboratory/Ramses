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

		if (strlen($name) > 0 AND strlen($shortName) > 0 AND strlen($color) == 6)
		{
			if (strlen($id) > 0)
			{
				$qString = "INSERT INTO status (name,shortName,color,description,id) VALUES ('" . $name . "','" . $shortName . "','" . $color . "','" . $description . "'," . $id . ");";

			}
			else
			{
				$qString = "INSERT INTO status (name,shortName,color,description) VALUES ('" . $name . "','" . $shortName . "','" . $color . "','" . $description . "');";
			}
			try
			{

				$rep = $bdd->query($qString);
				$rep->closeCursor();

				$reply["message"] = "Status " . $shortName . " added.";
				$reply["success"] = true;
			}
			catch (Exception $e)
			{
				$reply["message"] = "Server issue: SQL Query failed.\n" . $qString;
				$reply["success"] = false;
			}
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

		try
		{
			$rep = $bdd->query("SELECT name,shortName,color,description,id FROM status ORDER BY shortName;");
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
			$reply["message"] = "Statuses list retrived";
			$reply["success"] = true;
		}
		catch (Exception $e)
		{
			$reply["message"] = "Server issue: SQL Query failed.";
			$reply["success"] = false;
		}
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

		if (strlen($name) > 0 AND strlen($shortName) > 0 AND strlen($color) == 6 AND strlen($id) > 0)
		{
			try
			{
				$rep = $bdd->query("UPDATE status SET name='" . $name . "',shortName='" . $shortName . "',color='" . $color . "',description='" . $description . "' WHERE id=" . $id . ";");
				$rep->closeCursor();

				$reply["message"] = "Status " . $shortName . " (" . $id . ") updated.";
				$reply["success"] = true;
			}
			catch (Exception $e)
			{
				$reply["message"] = "Server issue: SQL Query failed.";
				$reply["success"] = false;
			}
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
			try
			{
				$rep = $bdd->query("DELETE status FROM status WHERE id=" . $id . ";");
				$rep->closeCursor();

				$reply["message"] = "Status " . $id . " removed.";
				$reply["success"] = true;
			}
			catch (Exception $e)
			{
				$reply["message"] = "Server issue: SQL Query failed.";
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
