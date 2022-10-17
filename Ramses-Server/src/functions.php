<?php
    require_once($__ROOT__."/config/config.php");
    require_once($__ROOT__."/logger.php");
    require_once($__ROOT__."/session_manager.php");
    require_once($__ROOT__."/reply.php");

    if (!function_exists('getallheaders')) {
        function getallheaders() {
            $headers = [];
            foreach ($_SERVER as $name => $value) {
                if (substr($name, 0, 5) == 'HTTP_') {
                    $headers[str_replace(' ', '-', ucwords(strtolower(str_replace('_', ' ', substr($name, 5)))))] = $value;
                }
            }
            return $headers;
        }
    }

    function createEncryptionKey ()
    {
        global $__ROOT__;
        // Compute appropriate cost for passwords
        $timeTarget = 0.1; // 100 milliseconds 
        $cost = 7;
        do {
            $cost++;
            $start = microtime(true);
            password_hash("test", PASSWORD_BCRYPT, ["cost" => $cost]);
            $end = microtime(true);
        } while (($end - $start) < $timeTarget);


        // Generate unique encryption key
        $key_size = 32; // 256 bits
        $encryption_key = openssl_random_pseudo_bytes($key_size, $strong);

        $configSecFile = fopen($__ROOT__."/config/config_security.php", "w");
        $encryption_key_txt = base64_encode($encryption_key);
        $ok = fwrite($configSecFile, "<?php\n\$encrypt_key = base64_decode('{$encryption_key_txt}');\n\$pass_cost = {$cost};\n?>");
        fclose($configSecFile);

        if (!$ok || !file_exists($__ROOT__."/config/config_security.php"))
        {
            echo( "    ▫ Failed. Could not write the key file. We need write permissions in the ramses folder." );
            echo("<p>If you can't grant this permission, copy the code below, and paste it in a new <strong>config/config_security.php</strong> file,");
            echo("<br />upload this new file to the server, and refresh this page.</p>");
            die("<strong><code>&lt;?php\n\$encrypt_key = base64_decode('{$encryption_key_txt}');\n\$pass_cost = {$cost};\n?&gt;</strong></code>");
        }

        chmod( $__ROOT__."/config/config_security.php", 0600 );

        return $encryption_key;
    }

    /**
     * Encrypts some text to store in the database
     */
    function encrypt( $txt )
    {
        global $encrypt_key;
        if ( $encrypt_key == '' ) return '';

        // Generate an initialization vector
        $iv = openssl_random_pseudo_bytes(openssl_cipher_iv_length('aes-256-cbc'));

        $enc_txt = openssl_encrypt(
            $txt,                 // data
            'AES-256-CBC',        // cipher and mode
            $encrypt_key,         // secret key
            0,                    // options (not used)
            $iv                   // initialisation vector
        );

        // The IV may contain the separator ('::'), base64 encoding it fixes the issue
        $iv = base64_encode( $iv );
        return base64_encode($enc_txt . '::' . $iv);
    }

    /**
     * Decrypts text stored in the database (base64)
     */
    function decrypt( $data )
    {
        global $encrypt_key;
        if ( $encrypt_key == '' ) return '';
        if (!isEncrypted($data)) return $data;

        $dec_txt = "";

        try {
            list($encrypted_data, $iv) = explode('::', base64_decode($data), 2);
            $iv = base64_decode( $iv );    

            $dec_txt = openssl_decrypt(
                $encrypted_data,
                'AES-256-CBC',
                $encrypt_key,
                0,
                $iv
            );
        }
        catch (exception $e) {
            return "";
        }

        return $dec_txt;
    }

    /**
     * Checks if a data is already encrypted
     */
    function isEncrypted( $data )
    {
        $test = base64_decode($data, true);
        if (!$test) return false;
        if( !strpos($test, '::') ) return false;
        return true;
    }

    /**
     * Logs in and returns the new session token
     */
    function login($uuid, $role, $id, $name)
    {
        global $log, $_SESSION;

        // Keep session info
        $_SESSION["uuid"] = $uuid;
        // Generate a new token
        $_SESSION["token"] = bin2hex(random_bytes(20));
        //Log
        $log->login($uuid, $role, $id, $name);

        SessionManager::regenerateSession();

        return $_SESSION["token"];
    }

    /**
     * Logs out and reset the session token
     */
    function logout($reason="logout", $message = "Logged out.")
    {
        global $log, $reply, $_SESSION;

        $uuid = "unknown";
        if (isset($_SESSION["uuid"])) $uuid = $_SESSION["uuid"];
        //Log
        $log->logout($uuid, $reason);

        $reply["message"] = $message;
        $reply["query"] = "loggedout";
        $reply["success"] = false;
        $reply["accepted"] = false;

        SessionManager::sessionEnd();
        
        printAndDie();
    }

    function isAdmin()
    {
        global $_SESSION, $tablePrefix;
        $q = new DBQuery();
        $q->prepare("SELECT `data` FROM {$tablePrefix}RamUser WHERE `uuid` = :uuid ;");
        $q->bindStr("uuid", $_SESSION['uuid']);
        $q->execute();
        $row = $q->fetch();
        $q->close();
        
        $data = decrypt( $row['data'] );
        $data = json_decode($data, true);

        if (!isset($data['role'])) return false;
        return strtolower($data['role']) == 'admin';
    }

    /**
     * Hashes a password adding the user id at the beginning
     */
    function hashPassword($pswd, $uuid)
    {
        global $pass_cost;
        return password_hash(  $uuid . $pswd ,  PASSWORD_DEFAULT, ['cost' => $pass_cost]);
    }

    function checkPassword( $pswd, $uuid, $testPswd )
    {
        $pswd = $uuid . $pswd;
        return password_verify($pswd, $testPswd);
    }

    /**
     * Tests if a string starts with a substring
     */
    function startsWith( $string, $substring ) {
        $length = strlen( $substring );
        return substr( $string, 0, $length ) === $substring;
    }

    /**
        * Tests if a string ends with a substring
        */
    function endsWith( $string, $substring ) {
        $length = strlen( $substring );
        if( !$length ) {
            return true;
        }
        return substr( $string, -$length ) === $substring;
    }

    /**
        * Prepares the prefix for SQL table names (adds a "_" at the end if needed)
        */
    function setupTablePrefix() {
        global $tablePrefix;
        if (strlen($tablePrefix) > 0 && !endsWith($tablePrefix, "_")) $tablePrefix = $tablePrefix . "_";
    }

    function checkForbiddenWords($str)
    {
        // These may be forbidden with some providers; use %word% instead and update.
        // We know it's safe, all queries are correctly prepared here using PDO queries.
        $forbidden = array("and", "or", "if", "else", "insert", "update", "select", "drop", "alter");
        foreach($forbidden as $word)
        {
            // Replace correctly formatted forbidden words with actual words
            $str = str_replace("%" . $word . "%", " " . $word, $str);
        }
        
        return $str;
    }

    /**
        * Gets an argument from the url
        */
    function getArg($name, $defaultValue = "")
    {
        global $bodyContent;

        $decordedArg = "";

        if (isset($bodyContent[$name]))
            $decordedArg = $bodyContent[$name];

        if ($decordedArg == "") return $defaultValue;       

        return checkForbiddenWords( $decordedArg );
    }

    function getAttr($name, $obj, $defaultValue = "")
    {
        $attr = "";
        if (isset($obj[$name]))
            $attr = $obj[$name];

        if ($attr == "") return $defaultValue;

        return $attr;
    }

    /**
        * Check if the URL has the given arg
        */
    function hasArg( $name )
    {
        return isset($_GET[$name]);
    }

    /**
        * Generates a pseudo-random UUID
        */
    function uuid() {
        return sprintf('%04x%04x-%04x-%04x-%04x-%04x%04x%04x',

        // 32 bits for "time_low"
        mt_rand(0, 0xffff), mt_rand(0, 0xffff),

        // 16 bits for "time_mid"
        mt_rand(0, 0xffff),

        // 16 bits for "time_hi_and_version",
        // four most significant bits holds version number 4
        mt_rand(0, 0x0fff) | 0x4000,

        // 16 bits, 8 bits for "clk_seq_hi_res",
        // 8 bits for "clk_seq_low",
        // two most significant bits holds zero and one for variant DCE1.1
        mt_rand(0, 0x3fff) | 0x8000,

        // 48 bits for "node"
        mt_rand(0, 0xffff), mt_rand(0, 0xffff), mt_rand(0, 0xffff)
        );
    }

    /**
     * Recursively deletes a directory
     */
    function deleteDir($dirPath) {
        if (! is_dir($dirPath)) {
            throw new InvalidArgumentException("$dirPath must be a directory");
        }
        if (substr($dirPath, strlen($dirPath) - 1, 1) != '/') {
            $dirPath .= '/';
        }
        $files = glob($dirPath . '*', GLOB_MARK);
        foreach ($files as $file) {
            if (is_dir($file)) {
                deleteDir($file);
            } else {
                unlink($file);
            }
        }
        rmdir($dirPath);
    }

    function acceptReply($queryName)
    {
        global $reply;

        // Already accepted
        if ($reply["accepted"]) return false;
        // Not this query
        if (!hasArg($queryName)) return false;

        // Got the right one!
        $reply["query"] = $queryName;
        $reply["accepted"] = true;

        return true;
    }
    
    function dateTimeStr()
    {
        $currentDate = new DateTime();
        return $currentDate->format('Y-m-d H:i:s');
    }

    function createFolder( $path, $recursive=false, $addIndex = true )
    {
        if (is_file($path)) return;
        if (!is_dir($path)) mkdir($path, 0700, $recursive);
        
        if ($addIndex)
        {
            if (substr($path, strlen($path) - 1, 1) != '/') {
                $path .= '/';
            }

            if (is_file($path . "index.html")) return;

            $file = fopen($path . "index.html", "a");
            if (!$file) return;
            fwrite($file, "<h1>Forbidden</h1>");
            fclose($file);
        }
    }

    function versionLowerThan( $version, $other )
    {
        $re = "/^(0|[1-9]\d*)\.(0|[1-9]\d*)\.(0|[1-9]\d*)(?:-((?:0|[1-9]\d*|\d*[a-zA-Z-][0-9a-zA-Z-]*)(?:\.(?:0|[1-9]\d*|\d*[a-zA-Z-][0-9a-zA-Z-]*))*))?(?:\+([0-9a-zA-Z-]+(?:\.[0-9a-zA-Z-]+)*))?$/i";
        $c = array();
        $v = array();
        preg_match($re, $version, $c);
        preg_match($re, $other, $v);

        $cn = count($c);
        $vn = count($v);
        if ($cn < 2 || $vn < 2) return strcmp($version,$other ) > 0;

        //compare major version
        if ( strcmp( $c[1],$v[1] ) > 0 ) return true;
        if ( strcmp( $c[1],$v[1] ) < 0 ) return false;

        if ($cn < 3 || $vn < 3) return strcmp($version,$other ) > 0;

        //minor
        if ( strcmp( $c[2],$v[2] ) > 0 ) return true;
        if ( strcmp( $c[2],$v[2] ) < 0 ) return false;

        if ($cn < 4 || $vn < 4) return strcmp($version,$other ) > 0;

        //patch
        if ( strcmp( $c[3],$v[3] ) > 0 ) return true;
        if ( strcmp( $c[3],$v[3] ) < 0 ) return false;

        if ($cn < 5 && $vn < 5) return strcmp($version,$other ) > 0;
		if ($vn < 5) return false;
		if ($cn < 5) return true;

        //build 
        if ( strcmp( $c[4],$v[4] ) > 0 ) return true;
        if ( strcmp( $c[4],$v[4] ) < 0 ) return false;

        return strcmp($version,$other ) > 0;
    }

    function createTable( $name, $drop = false )
    {
        global $tablePrefix, $sqlMode;
        
        $q = new DBQuery();
        $qStr = "";
        if ($drop) $qStr = "DROP TABLE IF EXISTS `{$tablePrefix}{$name}`; ";
        if ($sqlMode == 'sqlite') $qStr = $qStr . "CREATE TABLE IF NOT EXISTS `{$tablePrefix}{$name}` (
                    `id`	INTEGER NOT NULL UNIQUE,
                    `uuid`	TEXT NOT NULL UNIQUE,
                    `data`	TEXT NOT NULL DEFAULT '{}',
                    `modified`	timestamp NOT NULL,
                    `removed`	INTEGER NOT NULL DEFAULT 0,
                    PRIMARY KEY(`id` AUTOINCREMENT) );";
        else $qStr = $qStr . "CREATE TABLE IF NOT EXISTS `{$tablePrefix}{$name}` (
                    `id` int(11) NOT NULL,
                    `uuid` varchar(36) COLLATE utf8_unicode_ci NOT NULL,
                    `data` mediumtext NOT NULL,
                    `modified` timestamp NOT NULL,
                    `removed` tinyint(4) NOT NULL DEFAULT 0
                    ) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;
                ALTER TABLE `{$tablePrefix}{$name}`
                    ADD PRIMARY KEY (`id`),
                    ADD UNIQUE KEY `uuid` (`uuid`);
                ALTER TABLE `{$tablePrefix}{$name}`
                    MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;
                ";
        $q->prepare($qStr);

        $q->execute();
        $q->close();
        return $q->isOK();
    }

    function createDeletedDataTable()
    {
        global $tablePrefix, $sqlMode;

        $q = new DBQuery();
        $qStr = "";

        if ($sqlMode == 'sqlite') $qStr = "CREATE TABLE `{$tablePrefix}deletedData` (
                    `id`	INTEGER NOT NULL UNIQUE,
                    `uuid`	TEXT NOT NULL UNIQUE,
                    PRIMARY KEY(`id` AUTOINCREMENT) );";

        else $qStr = "CREATE TABLE IF NOT EXISTS `{$tablePrefix}deletedData` (
                    `id` int(11) NOT NULL,
                    `uuid` varchar(36) COLLATE utf8_unicode_ci NOT NULL
                    ) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;
                ALTER TABLE `{$tablePrefix}deletedData`
                    ADD PRIMARY KEY (`id`),
                    ADD UNIQUE KEY `uuid` (`uuid`);
                ALTER TABLE `{$tablePrefix}deletedData`
                    MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;
                ";

        $q->prepare($qStr);

        $q->execute();
        $q->close();
        return $q->isOK();
    }

    function printAndDie()
    {
        global $reply, $sessionTimeout, $_SESSION;
        // Set time out
        $_SESSION['discard_after'] = time() + $sessionTimeout;

        die( json_encode($reply) );
    }
?>
