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
		$stageId = "";
		$projectId = "";
		$comment = "";
		$id = "";

		$data = json_decode(file_get_contents('php://input'));
		if ($data)
		{
			$name = $data->{'name'};
			$shortName = $data->{'shortName'};
			$statusId = $data->{'statusId'};
			$stageId = $data->{'stageId'};
			$projectId = $data->{'projectId'};
			$comment = $data->{'comment'};
			$id = $data->{'id'};
		}

		if (strlen($name) > 0 AND strlen($shortName) > 0 AND strlen($statusId) > 0 AND strlen($stageId) > 0)
		{
			//construct add asset query
            $q = "INSERT INTO " . $tablePrefix . "assets (name,shortName,statusId,stageId,projectId,comment,id)
			VALUES ( :name , :shortName , :statusId , :stageId , :projectId, :comment , :id ) ON DUPLICATE KEY UPDATE shortName = VALUES(shortName), name = VALUES(name) ;";

			//create asset
			$repCreateAsset = $bdd->prepare($q);
			$repCreateAsset->execute(array('name' => $name , 'shortName' => $shortName , 'statusId' => $statusId , 'stageId' => $stageId , 'projectId' => $projectId , 'comment' => $comment , 'id' => $id));
			$repCreateAsset->closeCursor();
			$reply["message"] = "Asset added";
			$reply["success"] = true;
		}
		else
		{
			$reply["message"] = "Invalid request, missing values";
			$reply["success"] = false;
		}
	}

	if ($reply["type"] == "addAssets")
	{
		$reply["accepted"] = true;

		$assets = array();
		$projectId = "";
		$stageId = "";

		$data = json_decode(file_get_contents('php://input'));
		if ($data)
		{
			if (isset($data->{'projectId'})) $assets = $data->{'assets'};
			if (isset($data->{'assets'})) $projectId = $data->{'projectId'};
			if (isset($data->{'stageId'})) $stageId = $data->{'stageId'};
		}

		if (strlen($projectId) > 0 AND strlen($stageId) > 0 AND count($assets) > 0)
		{
			$q = "INSERT INTO " . $tablePrefix . "assets (name,shortName,statusId,comment,id,stageId,projectId) VALUES ";
			$placeHolder = "(?,?,?,?,?,?,?)";

			$placeHolders = array();
			$values = array();
			foreach($assets as $asset)
			{
				$placeHolders[] = $placeHolder;
				$values[] = $asset->{'name'};
				$values[] = $asset->{'shortName'};
				$values[] = $asset->{'statusId'};
				$values[] = $asset->{'comment'};
				$values[] = $asset->{'id'};
				$values[] = $stageId;
				$values[] = $projectId;
			}

			$q = $q . implode(",",$placeHolders);
			$q = $q . " ON DUPLICATE KEY UPDATE shortName = VALUES(shortName), name = VALUES(name);";

			$rep = $bdd->prepare($q);
			$rep->execute($values);
			$rep->closeCursor();
			$reply["message"] = "Assets added.";
			$reply["success"] = true;

		}
		else
		{
			$reply["message"] = "Invalid request, missing values";
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

		if (strlen($assetId) > 0 AND strlen($shotId) > 0)
		{
			$q = "INSERT INTO " . $tablePrefix . "assetstatuses (shotId,assetId)
			VALUES ( :shotId , :assetId ) ON DUPLICATE KEY UPDATE shotId = VALUES(shotId);";

			$rep = $bdd->prepare($q);
			$rep->execute(array('shotId' => $shotId , 'assetId' => $assetId));
			$rep->closeCursor();
			$reply["message"] = "Asset assigned.";
			$reply["success"] = true;

		}
		else
		{
			$reply["message"] = "Invalid request, missing values.";
			$reply["success"] = false;
		}
	}

	if ($reply["type"] == "assignAssets")
	{
		$reply["accepted"] = true;

		$assignments = array();

		$data = json_decode(file_get_contents('php://input'));
		if ($data)
		{
			if (isset($data->{'assignments'})) $assignments = $data->{'assignments'};
		}

		if (count($assignments) > 0)
		{
			$q = "INSERT INTO " . $tablePrefix . "assetstatuses (shotId,assetId) VALUES ";
			$placeHolder = "(?,?)";

			$placeHolders = array();
			$values = array();
			foreach($assignments as $assignment)
			{
				$placeHolders[] = $placeHolder;
				$values[] = $assignment->{'shotId'};
				$values[] = $assignment->{'assetId'};
			}

			$q = $q . implode(",",$placeHolders);
			$q = $q . " ON DUPLICATE KEY UPDATE shotId = VALUES(shotId);";

			$rep = $bdd->prepare($q);
			$rep->execute($values);
			$rep->closeCursor();
			$reply["message"] = "Assets assigned.";
			$reply["success"] = true;
		}
		else
		{
			$reply["message"] = "Invalid request, missing values";
			$reply["success"] = false;
		}
	}

	if ($reply["type"] == "addAssignAssets")
	{
		$reply["accepted"] = true;

		$assets = array();
		$projectId = "";
		$stageId = "";

		$data = json_decode(file_get_contents('php://input'));
		if ($data)
		{
			if (isset($data->{'projectId'})) $assets = $data->{'assets'};
			if (isset($data->{'assets'})) $projectId = $data->{'projectId'};
			if (isset($data->{'stageId'})) $stageId = $data->{'stageId'};
		}

		if (strlen($projectId) > 0 AND strlen($stageId) > 0 AND count($assets) > 0)
		{
			$qAdd = "INSERT INTO " . $tablePrefix . "assets (name,shortName,statusId,comment,id,stageId,projectId) VALUES ";
			$qAssign = "INSERT INTO " . $tablePrefix . "assetstatuses (shotId,assetId) VALUES ";
			$placeHolderAdd = "(?,?,?,?,?,?,?)";
			$placeHolderAssign = "(?,?)";

			$placeHoldersAdd = array();
			$placeHoldersAssign = array();
			$values = array();

			//add values
			foreach($assets as $asset)
			{
				$placeHoldersAdd[] = $placeHolderAdd;
				$values[] = $asset->{'name'};
				$values[] = $asset->{'shortName'};
				$values[] = $asset->{'statusId'};
				$values[] = $asset->{'comment'};
				$values[] = $asset->{'id'};
				$values[] = $stageId;
				$values[] = $projectId;
			}
			//assign values
			foreach($assets as $asset)
			{
				$placeHoldersAssign[] = $placeHolderAssign;
				$values[] = $asset->{'shotId'};
				$values[] = $asset->{'id'};
			}

			$qAdd = $qAdd . implode(",",$placeHoldersAdd);
			$qAdd = $qAdd . " ON DUPLICATE KEY UPDATE shortName = VALUES(shortName), name = VALUES(name);\n";
			$qAssign = $qAssign . implode(",",$placeHoldersAssign);
			$qAssign = $qAssign . " ON DUPLICATE KEY UPDATE shotId = VALUES(shotId);";
			$q = $qAdd . $qAssign;

			$rep = $bdd->prepare($q);
			$rep->execute($values);
			$rep->closeCursor();
			$reply["message"] = "Assets added and assigned.";
			$reply["success"] = true;
		}
		else
		{
			$reply["message"] = "Invalid request, missing values";
			$reply["success"] = false;
		}

	}

	if ($reply["type"] == "unAssignAsset")
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

		if (strlen($assetId) > 0 AND strlen($shotId) > 0)
		{
			$q = "DELETE " . $tablePrefix . "assetstatuses FROM " . $tablePrefix . "assetstatuses WHERE shotId = :shotId AND assetId = :assetId ;";

			$rep = $bdd->prepare($q);
			$rep->execute(array('shotId' => $shotId , 'assetId' => $assetId));
			$rep->closeCursor();
			$reply["message"] = "Asset un-assigned.";
			$reply["success"] = true;

		}
		else
		{
			$reply["message"] = "Invalid request, missing values.";
			$reply["success"] = false;
		}



	}

	if ($reply["type"] == "getAssets")
	{
		$reply["accepted"] = true;

		$projectId = "";
		$data = json_decode(file_get_contents('php://input'));
		if ($data)
		{
			$projectId = $data->{'projectId'};
		}

		//get assets
		$qAssets = "SELECT " . $tablePrefix . "assets.id," . $tablePrefix . "assets.name," . $tablePrefix . "assets.shortName," . $tablePrefix . "assets.statusId," . $tablePrefix . "assets.comment," . $tablePrefix . "assets.stageId," . $tablePrefix . "assets.projectId
		FROM " . $tablePrefix . "assets
		JOIN " . $tablePrefix . "projectstage ON " . $tablePrefix . "assets.stageId = " . $tablePrefix . "projectstage.stageId
		WHERE " . $tablePrefix . "projectstage.projectId = :projectId";

		//get assignments
		$qAssign = "SELECT " . $tablePrefix . "assetstatuses.shotId
		FROM " . $tablePrefix . "assetstatuses
		JOIN " . $tablePrefix . "assets ON " . $tablePrefix . "assetstatuses.assetId = " . $tablePrefix . "assets.id
		WHERE " . $tablePrefix . "assetstatuses.assetId = :id";

		$repAssets = $bdd->prepare($qAssets);
		$repAssets->execute(array('projectId' => $projectId));

		$assets = Array();
		while ($asset = $repAssets->fetch())
		{
			$a = Array();
			$a['id'] = (int)$asset['id'];
			$a['name'] = $asset['name'];
			$a['shortName'] = $asset['shortName'];
			$a['statusId'] = (int)$asset['statusId'];
			$a['stageId'] = (int)$asset['stageId'];
			$a['comment'] = $asset['comment'];
			$a['projectId'] = $asset['projectId'];

			$repAssign = $bdd->prepare($qAssign);
			$repAssign->execute(array('id' => $a['id']));

			$assignments = Array();
			while ($assignment = $repAssign->fetch())
			{
				$as = (int)$assignment['shotId'];
				$assignments[] = $as;
			}
			$repAssign->closeCursor();

			$a['assignments'] = $assignments;

			$assets[] = $a;
		}
		$repAssets->closeCursor();

		$reply["content"] = $assets;
		$reply["message"] = "Assets list retrieved ";
		$reply["success"] = true;
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
			$q = "UPDATE " . $tablePrefix . "assets SET statusId= :statusId WHERE id= :assetId ;";

			$rep = $bdd->prepare($q);
			$rep->execute(array('statusId' => $statusId, 'assetId' => $assetId));
			$rep->closeCursor();

			$reply["message"] = "Status for the asset (id:" . $assetId . ") has been updated.";
			$reply["success"] = true;

		}
		else
		{
			$reply["message"] = "Invalid request, missing values.";
			$reply["success"] = false;
		}
	}

	if ($reply["type"] == "updateAsset")
	{
		$reply["accepted"] = true;

		$assetId = "";
		$name = "";
		$shortName = "";
		$comment = "";

		$data = json_decode(file_get_contents('php://input'));
		if ($data)
		{
			$assetId = $data->{'assetId'};
			$name = $data->{'name'};
			$shortName = $data->{'shortName'};
			$comment = $data->{'comment'};
		}

		if (strlen($assetId) > 0)
		{
			$q = "UPDATE " . $tablePrefix . "assets SET name= :name , shortName = :shortName , comment = :comment WHERE id= :assetId ;";

			//create asset
			$rep = $bdd->prepare($q);
			$rep->execute(array('name' => $name , 'shortName' => $shortName , 'comment' => $comment , 'assetId' => $assetId ));
			$rep->closeCursor();

			$reply["message"] = "Status for the asset (id:" . $assetId . ") has been updated.";
			$reply["success"] = true;

		}
		else
		{
			$reply["message"] = "Invalid request, missing values.";
			$reply["success"] = false;
		}
	}

	if ($reply["type"] == "removeAsset")
	{
		$reply["accepted"] = true;

		$assetId = "";

		$data = json_decode(file_get_contents('php://input'));
		if ($data)
		{
			if (isset($data->{'assetId'})) $assetId = $data->{'assetId'};
		}

		if (strlen($assetId) > 0)
		{
			$q = "DELETE " . $tablePrefix . "assets FROM " . $tablePrefix . "assets WHERE id = :assetId ;";

			$rep = $bdd->prepare($q);
			$rep->execute(array('assetId' => $assetId));
			$rep->closeCursor();

			$reply["message"] = "Asset removed.";
			$reply["success"] = true;

		}
		else
		{
			$reply["message"] = "Invalid request, missing values";
			$reply["success"] = false;
		}

	}

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
