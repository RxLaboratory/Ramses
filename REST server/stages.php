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
		$type = "";
		
		$data = json_decode(file_get_contents('php://input'));
		if ($data)
		{
			$name = $data->{'name'};
			$shortName = $data->{'shortName'};
			$type = $data->{'type'};
		}
				
		if (strlen($name) > 0 AND strlen($shortName) > 0 AND strlen($type) == 1)
		{
			try
			{
				$rep = $bdd->query("INSERT INTO stages (name,shortName,type) VALUES ('" . $name . "','" . $shortName . "','" . $type . "');");
				$rep->closeCursor();
			
				$reply["message"] = "Stage " . $shortName . " added.";
				$reply["success"] = true;
			}
			catch (Exception $e)
			{
				$reply["message"] = "Server issue: SQL Query failed adding stage " . $shortName . ".";
				$reply["success"] = false;
			}
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
		
		try
		{
			$rep = $bdd->query("SELECT name,shortName,type,id FROM stages ORDER BY shortName;");
			$stages = Array();
			while ($status = $rep->fetch())
			{
				$stat = Array();
				$stat['name'] = $status['name'];
				$stat['shortName'] = $status['shortName'];
				$stat['type'] = $status['type'];
				$stat['id'] = (int)$status['id'];
				$stages[] = $stat;
			}
			$rep->closeCursor();
		
			$reply["content"] = $stages;
			$reply["message"] = "Stages list retreived";
			$reply["success"] = true;
		}
		catch (Exception $e)
		{
			$reply["message"] = "Server issue: SQL Query failed retreiving stages list.";
			$reply["success"] = false;
		}
	}
	
	// ========= UPDATE STAGE ==========
	if ($reply["type"] == "updateStage")
	{
		$reply["accepted"] = true;
		
		$name = "";
		$shortName = "";
		$type = "";
		$id = "";
		
		$data = json_decode(file_get_contents('php://input'));
		if ($data)
		{
			$name = $data->{'name'};
			$shortName = $data->{'shortName'};
			$type = $data->{'type'};
			$id = $data->{'id'};
		}
		
		if (strlen($name) > 0 AND strlen($shortName) > 0 AND strlen($type) == 1 AND strlen($id) > 0)
		{
			try
			{
				$rep = $bdd->query("UPDATE stages SET name='" . $name . "',shortName='" . $shortName . "',type='" . $type . "' WHERE id=" . $id . ";");
				$rep->closeCursor();
			
				$reply["message"] = "Stage " . $shortName . " (" . $id . ") updated.";
				$reply["success"] = true;
			}
			catch (Exception $e)
			{
				$reply["message"] = "Server issue: SQL Query failed updating stage " . $shortName . ".";
				$reply["success"] = false;
			}
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
			$id = $data->{'id'};
		}
		if (strlen($id) > 0)
		{
			try
			{
				$rep = $bdd->query("DELETE stages FROM stages WHERE id=" . $id . ";");
				$rep->closeCursor();
			
				$reply["message"] = "Stage " . $id . " removed.";
				$reply["success"] = true;
			}
			catch (Exception $e)
			{
				$reply["message"] = "Server issue: SQL Query failed deleting stage " . $id . ".";
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