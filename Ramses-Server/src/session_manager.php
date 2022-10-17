<?php
    /*
		Ramses: Rx Asset Management System
        
        This program is licensed under the GNU General Public License.

        Copyright (C) 20202-2021 Nicolas Dufresne and Contributors.

        This program is free software;
        you can redistribute it and/or modify it
        under the terms of the GNU General Public License
        as published by the Free Software Foundation;
        either version 3 of the License, or (at your option) any later version.

        This program is distributed in the hope that it will be useful,
        but WITHOUT ANY WARRANTY; without even the implied warranty of
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
        See the GNU General Public License for more details.

        You should have received a copy of the *GNU General Public License* along with this program.
        If not, see http://www.gnu.org/licenses/.
	*/

    /**
     * This class is used to handle the session, without using cookies:
     * it initiates a session, checks its security, destroys it when needed
     */
    class SessionManager
    {
        static function sessionStart($name, $limit = 0, $path = '/', $domain = null, $secure = null)
        {
            // If we've decided to force SSL, redirect
            if($_SERVER["HTTPS"] != "on" && $secure) {
                header("Location: https://" . $_SERVER["SERVER_NAME"] . $_SERVER["REQUEST_URI"]);
                exit();
            }

            // Set the cookie name before we start.
            session_name($name . '_Session');

            // Set the domain to default to the current domain.
            $domain = isset($domain) ? $domain : $_SERVER['SERVER_NAME'];

            // Set the default secure value to whether the site is being accessed with SSL
            $https = isset($secure) ? $secure : isset($_SERVER['HTTPS']);

            // Set the cookie settings and start the session
            session_set_cookie_params($limit, $path, $domain, $secure, true);
            session_start();

            // Make sure the session hasn't expired, and destroy it if it has
            if( self::validateSession() )
            {
                // Check to see if the session is new or a hijacking attempt
                if(!self::preventHijacking())
                {
                    // Reset session data and regenerate id
                    $_SESSION = array();
                    $_SESSION['IPaddress'] = $_SERVER['REMOTE_ADDR'];
                    $_SESSION['userAgent'] = $_SERVER['HTTP_USER_AGENT'];
                    self::regenerateSession();

                // Give a 5% chance of the session id changing on any request
                }elseif(rand(1, 100) <= 5){
                    self::regenerateSession();
                }
            } else {
                self::sessionEnd();
            }
        }

        static function sessionEnd()
        {
            $_SESSION = array();
            session_destroy();
            session_start();
        }

        static protected function preventHijacking()
        {
            if(!isset($_SESSION['IPaddress']) || !isset($_SESSION['userAgent']))
                return false;

            if ($_SESSION['IPaddress'] != $_SERVER['REMOTE_ADDR'])
                return false;

            if( $_SESSION['userAgent'] != $_SERVER['HTTP_USER_AGENT'])
                return false;

            return true;
        }

        static function regenerateSession()
        {
            // If this session is obsolete it means there already is a new id
            if(isset($_SESSION['OBSOLETE']) && $_SESSION['OBSOLETE'] == true)
                return;

            // Set current session to expire in 10 seconds
            $_SESSION['OBSOLETE'] = true;
            $_SESSION['EXPIRES'] = time() + 10;

            // Create new session without destroying the old one
            session_regenerate_id(false);

            // Grab current session ID and close both sessions to allow other scripts to use them
            $newSession = session_id();
            session_write_close();

            // Set session ID to the new one, and start it back up again
            session_id($newSession);
            session_start();

            // Now we unset the obsolete and expiration values for the session we want to keep
            unset($_SESSION['OBSOLETE']);
            unset($_SESSION['EXPIRES']);
        }

        static protected function validateSession()
        {
            if( isset($_SESSION['OBSOLETE']) && !isset($_SESSION['EXPIRES']) )
                return false;

            if(isset($_SESSION['EXPIRES']) && $_SESSION['EXPIRES'] < time())
                return false;

            return true;
        }
    }
?>