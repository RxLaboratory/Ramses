<?php
    require_once($__ROOT__."/config/config_logs.php");

    class Logger
    {
        private $logsPath = "";
        private $connexionLogFile = "";

        public function __construct()
        {
            // Check if we're logging
            global $enableLogs; 
            global $connexionLogs;
            if (!$enableLogs) return;

            // Prepare folder
            $this->logsPath = "logs/";
            createFolder($this->logsPath, true);
            // Create folder for current month too
            createFolder($this->logsPath . date("Y/m/") , true);

            if (!$connexionLogs) return;

            // Prepare files
            $this->connexionLogFile = $this->logsPath . date("Y/m/") . date("Y-m-d") . "_connexionLog.csv";
            if (!is_file($this->connexionLogFile)) $this->appendConnexionLog("UUID,ID,Name,Role,Action,Client version,Date", false);

            // Delete old logs
            $this->cleanLogs();
        }

        public function login($uuid, $role, $id, $name)
        {
            // Check if we're logging
            global $enableLogs; 
            global $connexionLogs;
            if (!$enableLogs) return;
            if (!$connexionLogs) return;

            $this->appendConnexionLog("{$uuid},{$id},{$name},{$role},login");
        }

        public function logout($uuid, $reason)
        {
            // Check if we're logging
            global $enableLogs; 
            global $connexionLogs;
            if (!$enableLogs) return;
            if (!$connexionLogs) return;

            $this->appendConnexionLog("{$uuid},,,,{$reason}");
        }

        private function appendConnexionLog($text, $appendDateAndVersion = true)
        {
            $date = date("Y/m/d H:i:s");
            if (isset($_SESSION["clientVersion"])) $clientVer = $_SESSION["clientVersion"];
            else $clientVer = "unknown";
            
            $file = fopen($this->connexionLogFile, "a");

            if (!$file) return;
            
            if ($appendDateAndVersion) fwrite($file, "{$text},{$clientVer},{$date}\n");
            else fwrite($file, "{$text}\n");

            fclose($file);
        }

        private function cleanLogs()
        {
            global $logsExpiration;
            // convert timeout in seconds
            $timeout = ($logsExpiration+1) * 86400;
            // get limit
            $timeLimit = strtotime("now") - $timeout;
            $yearLimit = date("Y", $timeLimit);
            $monthLimit = date("m", $timeLimit);
            $dayLimit = date("d", $timeLimit);

            // Year
            $yearFolders = glob($this->logsPath . "*/", GLOB_MARK);
            foreach( $yearFolders as $yearFolder)
            {
                if (basename($yearFolder) < $yearLimit) deleteDir($yearFolder);
                else break;
            }
            // Month and day
            $currentYearFolder = $this->logsPath . $yearLimit . "/";
            if (is_dir($currentYearFolder))
            {
                $monthFolders = glob($currentYearFolder . "*/", GLOB_MARK);
                foreach( $monthFolders as $monthFolder)
                {
                    if (basename($monthFolder) < $monthLimit) deleteDir($monthFolder);
                    else break;
                }
                // Day
                $currentMonthFolder = $currentYearFolder . $monthLimit . "/";
                if (is_dir($currentMonthFolder))
                {
                    $dayFiles = glob($currentMonthFolder . "*");
                    foreach( $dayFiles as $dayFile)
                    {
                        $mtime = filemtime($dayFile);
                        if ($mtime < $timeLimit) {
                            unlink($dayFile);
                        }
                    }
                }
            }
            
        }
    }
?>