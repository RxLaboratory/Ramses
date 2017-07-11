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
		$comment = "";
		$uuid = "";

		$data = json_decode(file_get_contents('php://input'));
		if ($data)
		{
			if (isset($data->{'name'})) $name = $data->{'name'};
			if (isset($data->{'shortName'})) $shortName = $data->{'shortName'};
			if (isset($data->{'statusId'})) $statusId = $data->{'statusId'};
			if (isset($data->{'stageId'})) $stageId = $data->{'stageId'};
			if (isset($data->{'comment'})) $comment = $data->{'comment'};
			if (isset($data->{'uuid'})) $uuid = $data->{'uuid'};
		}

		if (strlen($name) > 0 AND strlen($shortName) > 0 AND strlen($statusId) > 0 AND strlen($stageId) > 0)
		{
			if (strlen($uuid) > 0)
			{
				//construct add asset query
				$q = "INSERT INTO " . $tablePrefix . "assets (name,shortName,statusId,stageId,comment,uuid)
				VALUES (
					:name ,
					:shortName ,
					( SELECT id FROM " . $tablePrefix . "status WHERE uuid = :statusId ) ,
					( SELECT id FROM " . $tablePrefix . "stages WHERE uuid = :stageId ) ,
					:comment ,
					:uuid
					) ON DUPLICATE KEY UPDATE shortName = VALUES(shortName), name = VALUES(name) ;";
				//VALUES
				$values = array('name' => $name , 'shortName' => $shortName , 'statusId' => $statusId , 'stageId' => $stageId , 'comment' => $comment , 'uuid' => $uuid);
			}
			else
			{
				//construct add asset query
				$q = "INSERT INTO " . $tablePrefix . "assets (name,shortName,statusId,stageId,comment,uuid)
				VALUES (
					:name ,
					:shortName ,
					( SELECT id FROM " . $tablePrefix . "status WHERE uuid = :statusId ) ,
					( SELECT id FROM " . $tablePrefix . "stages WHERE uuid = :stageId ) ,
					:comment ,
					uuid()
					) ON DUPLICATE KEY UPDATE shortName = VALUES(shortName), name = VALUES(name) ;";
				//VALUES
				$values = array('name' => $name , 'shortName' => $shortName , 'statusId' => $statusId , 'stageId' => $stageId  , 'comment' => $comment);
			}

			//create asset
			$repCreateAsset = $bdd->prepare($q);
			$repCreateAsset->execute($values);
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
		$stageId = "";

		$data = json_decode(file_get_contents('php://input'));
		if ($data)
		{
			if (isset($data->{'assets'})) $projectId = $data->{'projectId'};
			if (isset($data->{'stageId'})) $stageId = $data->{'stageId'};
		}

		if (strlen($projectId) > 0 AND strlen($stageId) > 0 AND count($assets) > 0)
		{
			$q = "INSERT INTO " . $tablePrefix . "assets (name,shortName,statusId,comment,uuid,stageId) VALUES ";
			$placeHolder = "(?,?,( SELECT id FROM " . $tablePrefix . "status WHERE uuid = ? ),?,?,( SELECT id FROM " . $tablePrefix . "stages WHERE uuid = ? ))";

			$placeHolders = array();
			$values = array();
			foreach($assets as $asset)
			{
				$placeHolders[] = $placeHolder;
				$values[] = $asset->{'name'};
				$values[] = $asset->{'shortName'};
				$values[] = $asset->{'statusId'};
				$values[] = $asset->{'comment'};
				$values[] = $asset->{'uuid'};
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
			if (isset($data->{'assetId'})) $assetId = $data->{'assetId'};
			if (isset($data->{'shotId'})) $shotId = $data->{'shotId'};
		}

		if (strlen($assetId) > 0 AND strlen($shotId) > 0)
		{
			$q = "INSERT INTO " . $tablePrefix . "assetstatuses (shotId,assetId)
			VALUES (
				 ( SELECT id FROM " . $tablePrefix . "shots WHERE uuid = :shotId ) ,
				 ( SELECT id FROM " . $tablePrefix . "assets WHERE uuid = :assetId )
				 ) ON DUPLICATE KEY UPDATE shotId = VALUES(shotId);";

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
			$placeHolder = "(( SELECT id FROM " . $tablePrefix . "shots WHERE uuid = ? ),( SELECT id FROM " . $tablePrefix . "assets WHERE uuid = ? ))";

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
		$stageId = "";

		$data = json_decode(file_get_contents('php://input'));
		if ($data)
		{
			if (isset($data->{'assets'})) $assets = $data->{'assets'};
			if (isset($data->{'stageId'})) $stageId = $data->{'stageId'};
		}

		if (strlen($stageId) > 0 AND count($assets) > 0)
		{
			$qAdd = "INSERT INTO " . $tablePrefix . "assets (name,shortName,statusId,comment,uuid,stageId) VALUES ";
			$qAssign = "INSERT INTO " . $tablePrefix . "assetstatuses (shotId,assetId) VALUES ";
			$placeHolderAdd = "(?,?,( SELECT id FROM " . $tablePrefix . "status WHERE uuid = ? ),?,?,( SELECT id FROM " . $tablePrefix . "stages WHERE uuid = ? ))";
			$placeHolderAssign = "(( SELECT id FROM " . $tablePrefix . "shots WHERE uuid = ? ),( SELECT id FROM " . $tablePrefix . "assets WHERE uuid = ? ))";

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
				$values[] = $asset->{'uuid'};
				$values[] = $stageId;
			}
			//assign values
			foreach($assets as $asset)
			{
				$placeHoldersAssign[] = $placeHolderAssign;
				$values[] = $asset->{'shotId'};
				$values[] = $asset->{'uuid'};
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
			$q = "DELETE " . $tablePrefix . "assetstatuses FROM " . $tablePrefix . "assetstatuses WHERE shotId = ( SELECT id FROM " . $tablePrefix . "shots WHERE uuid = :shotId ) AND assetId = ( SELECT id FROM " . $tablePrefix . "assets WHERE uuid = :assetId ) ;";

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
		$qAssets = "SELECT " . $tablePrefix . "assets.id," . $tablePrefix . "assets.uuid," . $tablePrefix . "assets.name," . $tablePrefix . "assets.shortName," . $tablePrefix . "status.uuid as statusId," . $tablePrefix . "assets.comment," . $tablePrefix . "stages.uuid as stageId," . $tablePrefix . "projects.uuid as projectId
		FROM " . $tablePrefix . "assets
		JOIN " . $tablePrefix . "status ON " . $tablePrefix . "assets.statusId = " . $tablePrefix . "status.id
		JOIN " . $tablePrefix . "stages ON " . $tablePrefix . "assets.stageId = " . $tablePrefix . "stages.id
		JOIN " . $tablePrefix . "projectstage ON " . $tablePrefix . "assets.stageId = " . $tablePrefix . "projectstage.stageId
		JOIN " . $tablePrefix . "projects ON " . $tablePrefix . "projectstage.projectId = " . $tablePrefix . "projects.id
		WHERE " . $tablePrefix . "projects.uuid = :projectId";

		//get assignments
		$qAssign = "SELECT " . $tablePrefix . "shots.uuid as shotId
		FROM " . $tablePrefix . "assetstatuses
		JOIN " . $tablePrefix . "shots ON " . $tablePrefix . "shots.id = " . $tablePrefix . "assetstatuses.shotId
		JOIN " . $tablePrefix . "assets ON " . $tablePrefix . "assetstatuses.assetId = " . $tablePrefix . "assets.id
		WHERE " . $tablePrefix . "assetstatuses.assetId = :id";

		$repAssets = $bdd->prepare($qAssets);
		$repAssets->execute(array('projectId' => $projectId));

		$assets = Array();
		while ($asset = $repAssets->fetch())
		{
			$a = Array();
			$a['uuid'] = $asset['uuid'];
			$a['name'] = $asset['name'];
			$a['shortName'] = $asset['shortName'];
			$a['statusId'] = $asset['statusId'];
			$a['stageId'] = $asset['stageId'];
			$a['comment'] = $asset['comment'];
			$a['projectId'] = $asset['projectId'];

			$repAssign = $bdd->prepare($qAssign);
			$repAssign->execute(array('id' => $asset['id']));

			$assignments = Array();
			while ($assignment = $repAssign->fetch())
			{
				$as = $assignment['shotId'];
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
			if (isset($data->{'statusId'})) $statusId = $data->{'statusId'};
			if (isset($data->{'assetId'})) $assetId = $data->{'assetId'};
		}

		if (strlen($statusId) > 0 AND strlen($assetId) > 0)
		{
			$q = "UPDATE " . $tablePrefix . "assets SET statusId= ( SELECT id FROM " . $tablePrefix . "status WHERE uuid = :statusId ) WHERE uuid= :assetId ;";

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
			if (isset($data->{'assetId'})) $assetId = $data->{'assetId'};
			if (isset($data->{'name'})) $name = $data->{'name'};
			if (isset($data->{'shortName'})) $shortName = $data->{'shortName'};
			if (isset($data->{'comment'})) $comment = $data->{'comment'};
		}

		if (strlen($assetId) > 0)
		{
			$q = "UPDATE " . $tablePrefix . "assets SET name= :name , shortName = :shortName , comment = :comment WHERE uuid= :assetId ;";

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
			$q = "DELETE " . $tablePrefix . "assets FROM " . $tablePrefix . "assets WHERE uuid = :assetId ;";

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

?>
