<?php
/*
		Rainbox Asset Manager
		Shots management
	*/
	
	if ($reply["type"] == "addShots")
	{
		$reply["accepted"] = true;
		
		$names = Array();
		$projectId = "";
		$statusId = "";
		$shotOrder = 0;
		
		$data = json_decode(file_get_contents('php://input'));
		if ($data)
		{
			if (isset($data->{'names'})) $names = $data->{'names'};
			if (isset($data->{'projectId'})) $projectId = $data->{'projectId'};
			if (isset($data->{'statusId'})) $statusId = $data->{'statusId'};
			if (isset($data->{'shotOrder'})) $shotOrder = $data->{'shotOrder'};
		}
		
		if (count($names) > 0 AND strlen($projectId) > 0 AND strlen($statusId) > 0)
		{
			//construct add shots query
            $qShots = "INSERT INTO shots (name,projectId,shotOrder) VALUES ";
            $first = true;
            foreach($names as $name)
			{
				if (!$first) $qShots = $qShots . ",";
				$qShots = $qShots . "('" . $name . "'," . $projectId . "," . $shotOrder . ")";
				$first = false;
			}
            $qShots = $qShots . ";";

            //add shots
			try
			{
				//create shots
				$rep = $bdd->query($qShots);
				$rep->closeCursor();
			}
			catch (Exception $e)
			{
				$reply["message"] = "Server issue: SQL Query failed adding shots. | " + $qShots;
				$reply["success"] = false;
			}
			
			//create statuses if shots added
			if (isset($rep))
			{
				//get (shot) stages from project
				try
				{
					$q = "SELECT stages.id FROM stages JOIN projectStage ON stages.id = projectStage.stageId WHERE projectStage.projectId = " . $projectId . " AND stages.type = 's';";
					$repStages = $bdd->query($q);
				}
				catch (Exception $e)
				{
					$reply["message"] = "Server issue: SQL Query failed adding shots (stages retrieval failed) | " . $q;
					$reply["success"] = false;
				}
				
				if (isset($repStages))
				{
                    //construct query
                    $qStatuses = "INSERT INTO shotstatuses (statusId,shotId,stageId) VALUES ";
					
                    //for each stage
                    $first = true;
					while ($stage = $repStages->fetch())
					{
                        $stageId = $stage['id'];
                        //for each shot
                        foreach($names as $name)
                        {
							if (!$first) $qStatuses = $qStatuses . ",";
                            $qStatuses = $qStatuses . "(" . $statusId . ",";
                            $qStatuses = $qStatuses . "(SELECT id FROM shots WHERE name='" . $name . "' AND projectId=" . $projectId . "),";
                            $qStatuses = $qStatuses .  $stageId . ")";
							$first = false;
                        }
                       
					}
                    $qStatuses = $qStatuses . ";";
					  
                    try
                    {
                        //create status for this stage/shot
                        $rep3 = $bdd->query($qStatuses);
                        $rep3->closeCursor();
                        $reply["message"] = "Shots added.";
                        $reply["success"] = true;
                    }
                    catch (Exception $e)
                    {
                       $reply["message"] = "Server issue: SQL Query failed adding shots (statuses failed) | " . $qStatuses;
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
		
		$q = "SELECT shotstatuses.comment,shotstatuses.stageId,shotstatuses.statusId,shots.name as shotName,shots.duration,shots.id as shotId,shots.shotOrder FROM shotstatuses JOIN shots ON shots.id = shotstatuses.shotId WHERE projectId=" . $projectId . " ORDER BY shots.shotOrder,shots.name;";
		
		try
		{
			//get statuses (and shots)
			$rep = $bdd->query($q);
		}
		catch (Exception $e)
		{
			$reply["message"] = "Server issue: SQL Query failed retrieving shots list. | " . $q;
			$reply["success"] = false;
		}
		if (isset($rep))
		{
			$shots = Array();
			while ($shot = $rep->fetch())
			{
				$s = Array();
				$s['shotName'] = $shot['shotName'];
				$s['duration'] = (double)$shot['duration'];
				$s['shotId'] = (int)$shot['shotId'];
				$s['comment'] = $shot['comment'];
				$s['stageId'] = (int)$shot['stageId'];
				$s['statusId'] = (int)$shot['statusId'];
				$s['shotOrder'] = (int)$shot['shotOrder'];
	
				$shots[] = $s;
			}
			$rep->closeCursor();
			
			//get assets
			$q = "SELECT assets.id as assetId, assets.name as assetName,assets.shortName as assetShortName,assets.stageId,assets.statusId,assets.comment,shots.name as shotName,shots.duration,shots.id as shotId FROM assets JOIN shotassets ON shotassets.assetId = assets.id JOIN shots ON shotassets.shotId = shots.id WHERE shots.projectId=" . $projectId . ";";
			try
			{
				$repAsset = $bdd->query($q);
			}
			catch (Exception $e)
			{
				$reply["message"] = "Server issue: SQL Query failed retrieving assets list. | " . $q;
				$reply["success"] = false;
			}
			
			if (isset($repAsset))
			{
				while ($asset = $repAsset->fetch())
				{
					$a = Array();
					$a['shotName'] = $asset['shotName'];
					$a['duration'] = (double)$asset['duration'];
					$a['shotId'] = (int)$asset['shotId'];
					$a['assetId'] = (int)$asset['assetId'];
					$a['assetName'] = $asset['assetName'];
					$a['assetShortName'] = $asset['assetShortName'];
					$a['stageId'] = (int)$asset['stageId'];
					$a['statusId'] = (int)$asset['statusId'];
					$a['comment'] = $asset['comment'];
					$shots[] = $a;
				}
				$repAsset->closeCursor();
			}
			
			$reply["content"] = $shots;
			$reply["message"] = "Shots list retrieved ";
			$reply["success"] = true;
		}
	}
	
	// ========= UPDATE SHOT ==========
	if ($reply["type"] == "updateShot")
	{
		$reply["accepted"] = true;
		
		$name = "";
		$duration = "";
		$id = "";
		$shotOrder = 0;
		
		$data = json_decode(file_get_contents('php://input'));
		if ($data)
		{
			$name = $data->{'name'};
			$duration = $data->{'duration'};
			$id = $data->{'id'};
			$shotOrder = $data->{'shotOrder'};
		}
		
		if (strlen($name) > 0 AND strlen($duration) > 0 AND strlen($id) > 0)
		{
			try
			{
				$rep = $bdd->query("UPDATE shots SET name='" . $name . "',duration=" . $duration . ",shotOrder=" . $shotOrder . "WHERE id=" . $id . ";");
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
	}
?>