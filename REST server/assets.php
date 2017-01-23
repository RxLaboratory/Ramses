<?php
/*
		Rainbox Asset Manager
		Shots management
	*/
	
	if ($reply["type"] == "addAsset")
	{
		$reply["accepted"] = true;
		
		$name = "";
		$shortName = "";
		$stageId = "";
		$shotId = "";
		$statusId = "";
		
		$data = json_decode(file_get_contents('php://input'));
		if ($data)
		{
			$name = $data->{'name'};
			$shortName = $data->{'shortName'};
			$stageId = $data->{'stageId'};
			$shotId = $data->{'shotId'};
			$statusId = $data->{'statusId'};
		}
				
		if (count($name) > 0 AND strlen($stageId) > 0 AND strlen($shortName) > 0)
		{
			//construct add asset query
            $q = "INSERT INTO assets (name,shortName,stageId,statusId) VALUES ('" . $name . "','" . $shortName . "'," . $stageId . "," . $statusId . ");";	
			try
			{
				//create asset
				$rep = $bdd->query($q);
				$rep->closeCursor();
				$reply["message"] = "Asset added";
				$reply["success"] = true;
			}
			catch (Exception $e)
			{
				$reply["message"] = "Server issue: SQL Query failed adding asset. | " . $q;
				$reply["success"] = false;
			}
			
			//assign to shot if specified
			if (isset($rep) AND strlen($shotId) > 0 AND (int)strlen($shotId) >= 0)
			{
				$q = "SELECT id FROM assets WHERE name='" . $name . "' AND shortName='" . $shortName . "';";
				try
				{
					$repId = $bdd->query($q);
				}
				catch (Exception $e)
				{
					$reply["message"] = "Server issue: SQL Query failed getting asset id | " . $q;
					$reply["success"] = false;
				}
				
				if (isset($repId))
				{
					$assetId = $repId->fetch();
					$q = "INSERT INTO shotassets (shotId,assetId) VALUES (" . $shotId . "," . $assetId['id'] . ");";
					
					try
					{
						$rep = $bdd->query($q);
                        $rep->closeCursor();
                        $reply["message"] = "Asset added and assigned.";
                        $reply["success"] = true;
                    }
                    catch (Exception $e)
                    {
                       $reply["message"] = "Server issue: SQL Query failed adding assigning asset | " . $q;
					   $reply["success"] = false;
                    }
				}
			}
		}
		else
		{
			$reply["message"] = "Invalid request, missing values";
			$reply["success"] = false;
		}
	}

	if ($reply["type"] == "setAssetStatus")
	{
		$reply["accepted"] = true;
		
		$assetId = "";
		$statusId = "";
		
		$data = json_decode(file_get_contents('php://input'));
		if ($data)
		{
			$statusId = $data->{'statusId'};
			$assetId = $data->{'assetId'};
		}
		
		if (strlen($statusId) > 0 AND strlen($assetId) > 0)
		{
			$q = "UPDATE assets SET statusId=" . $statusId . " WHERE id=" . $assetId . ";";
			try
			{
				$rep = $bdd->query($q);
				$rep->closeCursor();
			
				$reply["message"] = "Status for the asset (id:" . $assetId . ") has been updated.";
				$reply["success"] = true;
			}
			catch (Exception $e)
			{
				$reply["message"] = "Server issue: SQL Query failed updating asset (id:" . $assetId . "). | " . $q;
				$reply["success"] = false;
			}
		}
		else
		{
			$reply["message"] = "Invalid request, missing values.";
			$reply["success"] = false;
		}
	}
	
	if ($reply["type"] == "assignAsset")
	{
		$reply["accepted"] = true;
		
		$assetId = "";
		$shotId = "";
		
		$data = json_decode(file_get_contents('php://input'));
		if ($data)
		{
			$assetId = $data->{'assetId'};
			$shotId = $data->{'shotId'};
		}
		
		$q = "INSERT INTO shotassets (shotId,assetId) VALUES (" . $shotId . "," . $assetId . ");";
					
		try
		{
			$rep = $bdd->query($q);
			$rep->closeCursor();
			$reply["message"] = "Asset assigned.";
			$reply["success"] = true;
		}
		catch (Exception $e)
		{
		   $reply["message"] = "Server issue: SQL Query failed adding assigning asset | " . $q;
		   $reply["success"] = false;
		}
		
		
	}
	/*// ========= GET SHOTS ==========
	if ($reply["type"] == "getShots")
	{
		$reply["accepted"] = true;
		
		$projectId = "";
		
		$data = json_decode(file_get_contents('php://input'));
		if ($data)
		{
			$projectId = $data->{'projectId'};
		}
		
		$q = "SELECT shotstatuses.comment,shotstatuses.stageId,shotstatuses.statusId,shots.name,shots.duration,shots.id FROM shotstatuses JOIN shots ON shots.id = shotstatuses.shotId WHERE projectId=" . $projectId . " ORDER BY shots.name;";
		
		try
		{
			//get statuses (and shots)
			$rep = $bdd->query($q);
			$shots = Array();
			while ($shot = $rep->fetch())
			{
				$s = Array();
				$s['name'] = $shot['name'];
				$s['duration'] = (double)$shot['duration'];
				$s['id'] = (int)$shot['id'];
				$s['comment'] = $shot['comment'];
				$s['stageId'] = (int)$shot['stageId'];
				$s['statusId'] = (int)$shot['statusId'];
				$shots[] = $s;
			}
			$rep->closeCursor();
		
			$reply["content"] = $shots;
			$reply["message"] = "Shots list retrieved";
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
			try
			{
				$rep = $bdd->query("UPDATE shots SET name='" . $name . "',duration=" . $duration . " WHERE id=" . $id . ";");
				$rep->closeCursor();
			
				$reply["message"] = "Shot " . $name . " (" . $id . ") updated.";
				$reply["success"] = true;
			}
			catch (Exception $e)
			{
				$reply["message"] = "Server issue: SQL Query failed updating shot " . $name . ".";
				$reply["success"] = false;
			}
		}
		else
		{
			$reply["message"] = "Invalid request, missing values";
			$reply["success"] = false;
		}
	}

	if ($reply["type"] == "setShotStatus")
	{
		$reply["accepted"] = true;
		
		$shotId = "";
		$statusId = "";
		$stageId = "";
		
		$data = json_decode(file_get_contents('php://input'));
		if ($data)
		{
			$shotId = $data->{'shotId'};
			$statusId = $data->{'statusId'};
			$stageId = $data->{'stageId'};
		}
		
		if (strlen($shotId) > 0 AND strlen($stageId) > 0 AND strlen($stageId) > 0)
		{
			try
			{
				$rep = $bdd->query("UPDATE shotstatuses SET statusId=" . $statusId . " WHERE shotId=" . $shotId . " AND stageId=" . $stageId . ";");
				$rep->closeCursor();
			
				$reply["message"] = "Status for the shot (id:" . $shotId . ") has been updated.";
				$reply["success"] = true;
			}
			catch (Exception $e)
			{
				$reply["message"] = "Server issue: SQL Query failed updating shot (id:" . $shotId . ").";
				$reply["success"] = false;
			}
		}
		else
		{
			$reply["message"] = "Invalid request, missing values.";
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
	}*/
?>