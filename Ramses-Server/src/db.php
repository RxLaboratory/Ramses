<?php
	require_once($__ROOT__."/config/config.php");
	
	/*
		Rainbox Asset Manager
		Database access
	*/

	// sqlMode may not be set if the config file is from an old version
	if (!isset($sqlMode)) $sqlMode = 'mysql';

	// Security, chmod the data file
	if (is_file($__ROOT__."/data/ramses_data")) chmod($__ROOT__."/data/ramses_data", 0600);

	try
	{
		if ( $sqlMode == 'mysql' )
		{
			$options = array(PDO::ATTR_ERRMODE => PDO::ERRMODE_EXCEPTION, PDO::MYSQL_ATTR_INIT_COMMAND => "SET NAMES 'utf8'");
			$db = new PDO('mysql:host=' . $sqlHost . ';port=' . $sqlPort . ';dbname=' . $sqlDBName . ';charset=utf8', $sqlUser, $sqlpassword,$options);
		}
			
		else if ( $sqlMode == 'sqlite' )
		{
			$db = new PDO( 'sqlite:' .$__ROOT__."/data/ramses_data" );
			$db->setAttribute( PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION );
		}
		else
		{
			die ( "Sorry, unknown database mode: " . $sqlMode );
		}
	}
	catch (Exception $e)
	{
		echo ("Oops, something went wrong with the database. Here's the error: <br />");
		die('Error: ' . $e->getMessage());
	}

	// A Wrapper class for the DB interface and queries
	class DBQuery
	{
		private $query;
		private $ok = false;
		private $closed = true;
		private $errorInfo = "";

		public function isOK()
		{
			return $this->ok;
		}

		public function errorInfo()
		{
			return $this->errorInfo;
		}

		// Insert/Replace a new row in a table
		public function insert( $table, $keys )
		{
			global $tablePrefix;

			$qKeys = array();
			$qVals = array();

			foreach( $keys as $key )
			{
				array_push( $qKeys, '`' . $key . '`');
				array_push( $qVals, ':' . $key);
			}

			array_push( $qKeys, '`latestUpdate`');
			array_push( $qKeys, '`removed`');
			array_push( $qVals, ':udpateTime');
			array_push( $qVals, ':removed');

			$q = "REPLACE INTO {$tablePrefix}{$table} (" . join(",",$qKeys) . ") VALUES (" . join(",",$qVals) . ");";

			$this->prepare($q);
			$this->bindStr('udpateTime', dateTimeStr() );
			$this->bindInt('removed', '0' );
		}

		// Removes a row from a table
		public function remove( $table, $uuid, $deleteNEW = true )
		{
			global $tablePrefix;

			$sName = '';

			if ($deleteNEW)
			{
				$q = "SELECT `shortName` FROM {$tablePrefix}{$table} WHERE `uuid`= :uuid ;";

				$this->prepare($q);
				$this->bindStr( "uuid", $uuid, true);
				$this->execute();

				$item = $this->fetch();
				if ($item) $sName = $item['shortName'];

				$this->close();
			}
			
			// DELETE
			if ($sName == "NEW")
			{
				$q = "DELETE FROM {$tablePrefix}{$table} WHERE uuid= :uuid ;";
				
				$this->prepare($q);
				$this->bindStr('uuid', $uuid, true );
				$this->execute( "'NEW' item deleted." );
				$this->close();
			}
			// SET REMOVED
			else
			{
				$q = "UPDATE {$tablePrefix}{$table} SET removed = 1, latestUpdate = :udpateTime WHERE uuid= :uuid ;";
				
				$this->prepare($q);
				$this->bindStr('uuid', $uuid, true );
				$this->bindStr('udpateTime', dateTimeStr() );
				$this->execute( "Item '{$sName}' removed from {$table}." );
				$this->close();
			}
		}

		// Updates a row in a table
		public function update( $table, $keys, $uuid )
		{
			global $tablePrefix;

			$qKeys = array();

			foreach( $keys as $key )
			{
				array_push( $qKeys, '`' . $key . '`= :' . $key);
			}
			array_push($qKeys, '`latestUpdate`= :updateTime');

			$q = "UPDATE {$tablePrefix}{$table} SET " . join(",",$qKeys) . " WHERE `uuid`= :uuid;";

			$this->prepare($q);
			$this->bindStr('uuid',  $uuid );
			$this->bindStr('updateTime', dateTimeStr() );
		}

		// Gets values from a single row
		public function get( $table, $keys, $uuid )
		{
			global $tablePrefix;

			$qKeys = array();
			foreach( $keys as $key )
			{
				array_push( $qKeys, '`' . $key . '`');
			}

			$q = "SELECT " . join(',',$qKeys) . " FROM {$tablePrefix}{$table} WHERE `uuid`= :uuid;";

			$this->prepare($q);
			$this->bindStr('uuid',  $uuid );

			$result = Array();
			$this->execute();
			if ($r = $this->fetch())
			{
				foreach($keys as $key)
				{
					$result[$key] = $r[$key];
				}
			}
			$this->close();
			return $result;
		}

		// Bind a string
		public function bindStr( $key, $str, $mandatory = false )
		{
			global $reply;

			if ( !$this->ok ) return;

			if ($str == "" && $mandatory)
			{
				$reply["message"] = "Invalid request, missing value (string): '{$key}'";
            	$reply["success"] = false;
				$this->ok = false;
				return;
			}

			$this->query->bindValue( ":{$key}", $str, PDO::PARAM_STR );
		}

		// Bind an int
		public function bindInt( $key, $int, $mandatory = false )
		{
			global $reply;

			if ( !$this->ok ) return;

			if ($int === "" && $mandatory)
			{
				$reply["message"] = "Invalid request, missing value (int): '{$key}'";
            	$reply["success"] = false;
				$this->ok = false;
				return;
			}
			else if ($int === "")
			{
				$this->bindNull( $key );
			}
			else 
			{
				$this->query->bindValue( ":{$key}", $int, PDO::PARAM_INT );
			}
		}

		public function bindNull( $key )
		{
			$this->query->bindValue( ":{$key}", null, PDO::PARAM_STR );
		}

		// Bind a float (rounds it)
		public function bindFloat( $key, $float, $precision=6, $mandatory = false)
		{
			global $reply;

			if ( !$this->ok ) return;

			if ($float == "" && $mandatory)
			{
				$reply["message"] = "Invalid request, missing value (float): '{$key}'";
            	$reply["success"] = false;
				$this->ok = false;
				return;
			}
			else if ($float == "")
			{
				$this->query->bindValue( ":{$key}", null, PDO::PARAM_STR );
			}
			else 
			{
				$float = (float)$float;
				$float = round($float, $precision);
				$this->query->bindValue( ":{$key}", $float, PDO::PARAM_STR );
			}

		}

		// Request
		public function execute( $successMessage = "", $debug = false )
		{
			global $reply;

			if (!$this->closed) $this->close();
			if ( !$this->ok ) return;

			// dump params if debug
			if ($debug) $this->query->debugDumpParams();
			// execute
			$this->ok = $this->query->execute();
			$this->closed = false;

			// update reply
			if (!$this->ok)
			{
				$this->errorInfo = $this->query->errorInfo();
				$reply["message"] = "Database query failed. Here's the error\n\n" . $rep->errorInfo()[2];
				$reply["success"] = false;
			}
			else if ($successMessage != "")
			{
				$reply["message"] = $successMessage;
				$reply["success"] = true;
			}
		}

		// Close cursor
		public function close()
		{
			if (isset( $this->query )) $this->query->closeCursor();
			$this->closed = true;
		}

		public function fetch()
		{
			$this->closed = false;

			if ( !$this->ok ) return false;
			return $this->query->fetch();
		}

		public function prepare( $qstr, $debug = false )
		{
			global $db;

			$this->query = $db->prepare($qstr);
			if ($this->query) $this->ok = true;
			else
			{
				global $reply;
				$reply["message"] = "Could not prepare the Database query.";
        		$reply["success"] = false;
				$this->ok = false;
				if ($debug) echo( "Could not prepare the Database query." );
				printAndDie();
			}
		}
	}
?>