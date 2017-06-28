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
		$statusId = "";
		$comment = "";
		$id = "";

		$data = json_decode(file_get_contents('php://input'));
		if ($data)
		{
			$name = $data->{'name'};
			$shortName = $data->{'shortName'};
			$statusId = $data->{'statusId'};
			$comment = $data->{'comment'};
			$id = $data->{'id'};
		}

		if (strlen($name) > 0 AND strlen($shortName) > 0 AND strlen($statusId) > 0)
		{
			//construct add asset query
            $q = "INSERT INTO assets (name,shortName,statusId,comment,id) VALUES ('" . $name . "','" . $shortName . "'," . $statusId . ",'" . $comment . "'," . $id . ");";
			try
			{
				//create asset
				$repCreateAsset = $bdd->query($q);
				$repCreateAsset->closeCursor();
				$reply["message"] = "Asset added";
				$reply["success"] = true;
			}
			catch (Exception $e)
			{
				$reply["message"] = "Server issue: SQL Query failed adding asset. | " . $q;
				$reply["success"] = false;
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
		$stageId = "";

		$data = json_decode(file_get_contents('php://input'));
		if ($data)
		{
			$statusId = $data->{'statusId'};
			$assetId = $data->{'assetId'};
			$stageId = $data->{'stageId'};
		}

		if (strlen($statusId) > 0 AND strlen($assetId) > 0 AND strlen($stageId) > 0)
		{
			$q = "UPDATE assetstatuses SET statusId=" . $statusId . " WHERE assetId=" . $assetId . " AND stageId=" . $stageId . ";";
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
		$stageId = "";

		$data = json_decode(file_get_contents('php://input'));
		if ($data)
		{
			$assetId = $data->{'assetId'};
			$shotId = $data->{'shotId'};
			$stageId = $data->{'stageId'};
		}

		if (strlen($assetId) > 0 AND strlen($shotId) > 0 AND strlen($stageId) > 0)
		{
			$q = "INSERT INTO assetstatuses (shotId,assetId,stageId) VALUES (" . $shotId . "," . $assetId . "," . $stageId . ");";

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
		else
		{
			$reply["message"] = "Invalid request, missing values.";
			$reply["success"] = false;
		}



	}

	//TODO add status (stage)



/*

			//get assets
			$q = "SELECT assets.id as assetId, assets.name as assetName,assets.shortName as assetShortName,assetstatuses.stageId as stageId,assetstatuses.statusId as statusId,assetstatuses.comment as comment,shots.name as shotName,shots.duration,shots.id as shotId FROM assetstatuses JOIN assets ON assets.id = assetstatuses.assetId JOIN shotassets ON shotassets.assetId = assetstatuses.assetId JOIN shots ON shotassets.shotId = shots.id WHERE shots.projectId=" . $projectId . ";";
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
*/
?>
