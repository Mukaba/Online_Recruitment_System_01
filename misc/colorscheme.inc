<?php
/*

 +------------------------------------------------------------------------------+
 | Mamook(R) Software                                                           |
 +------------------------------------------------------------------------------+
 | Copyright (c) 2000-2005 University of Victoria.  All rights reserved.        |
 +------------------------------------------------------------------------------+
 | THE LICENSED WORK IS PROVIDED UNDER THE TERMS OF THE ADAPTIVE PUBLIC LICENSE |
 | ("LICENSE") AS FIRST COMPLETED BY: The University of Victoria. ANY USE,      |
 | PUBLIC DISPLAY, PUBLIC PERFORMANCE, REPRODUCTION OR DISTRIBUTION OF, OR      |
 | PREPARATION OF DERIVATIVE WORKS BASED ON, THE LICENSED WORK CONSTITUTES      |
 | RECIPIENT'S ACCEPTANCE OF THIS LICENSE AND ITS TERMS, WHETHER OR NOT SUCH    |
 | RECIPIENT READS THE TERMS OF THE LICENSE. "LICENSED WORK" AND "RECIPIENT"    |
 | ARE DEFINED IN THE LICENSE. A COPY OF THE LICENSE IS LOCATED IN THE TEXT     |
 | FILE ENTITLED "LICENSE.TXT" ACCOMPANYING THE CONTENTS OF THIS FILE. IF A     |
 | COPY OF THE LICENSE DOES NOT ACCOMPANY THIS FILE, A COPY OF THE LICENSE MAY  |
 | ALSO BE OBTAINED AT THE FOLLOWING WEB SITE: http://www.mamook.net            |  
 |                                                                              |
 | Software distributed under the License is distributed on an "AS IS" basis,   |
 | WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License for |
 | the specific language governing rights and limitations under the License.    | 
 +------------------------------------------------------------------------------+
 | Filename: colorscheme.inc                                                    |
 +------------------------------------------------------------------------------+
 | Description:                                                                 |
 +------------------------------------------------------------------------------+

*/


//  colorscheme.inc  

//	Does all the thinking for the colorscheme the user has chosen in the preferences...
//	Defaults to blue... stores in the preferences table under colorscheme column.
// 	contact_id being the unique key for Employers and Office
//  	student_number being the unique key for students.

	// Code Used for Color Scheme
        
	/*
		Code first checks if user is logged in, if so, it then checks if its his first time in.
		If so, it either gets info from the table or adds him in (default = blue).
		If not, it check whether a colorscheme variable was sent with the page, and updates the table.
		If no colorscheme variable was sent, life goes on.
		If the user is not logged on.. the color defaults to blue.. the COOKIE is unset in authenticate.inc

		// added check for public or limbo users, they default to blue.
		// uvic is now default, switch blue for uvic.

	*/

// Array of All Current Schemes (will later on be pulled out of database..)
// New Schemes must be added here, or else big problems arise ! :)


$schemes = get_hooks("colorscheme.getidentifiers");

if (ALLOW_GENERIC_COLORSCHEME){
    $schemes[] = 'none';
}


if (isset($_GET['colorscheme'])) {
    $colorscheme = $_GET['colorscheme'];
}

if (($auth->userlevel == USERLEVEL_PUBLIC || $auth->userlevel == LIMBO || $auth->userlevel == MULTIPLE)) {
    $_SESSION['SESSION_colorscheme'] = DEFAULT_COLORSCHEME;
} else {
    if ($_SESSION['SESSION_loginID'] != "") {		
        if (!$_SESSION['SESSION_checked_colorscheme']) {
            $_SESSION['SESSION_checked_colorscheme'] = true;
            if ($auth->userlevel == EMPLOYER ||$auth->userlevel == HREMPLOYER) {
                $gettable = "employer_login";
                $getvar = "contact_id";
            }
            elseif ($auth->userlevel == OFFICE || $auth->userlevel == TRAFFICDIRECTOR || 
                    $auth->userlevel == ADMINSTUDENT || $auth->userlevel == ADMINISTRATOR || 
                    $auth->userlevel == FACULTY) {
                $gettable = "contact_internal";
                $getvar = "contact_id";
            } elseif ($auth->userlevel == STUDENT) {
                $gettable = "student";
                $getvar = "student_number";
            }
            if (getfromTable($local_contact_id, $gettable, $getvar, $auth->department, $auth->login)) {

                $result = $GLOBALS['dbh']->Execute("select colorscheme from preferences where $getvar = '$local_contact_id'");
                if ($row = $result->FetchRow()) { 
                    $colorscheme = $row["colorscheme"];
                } else {  
                    $result = $GLOBALS['dbh']->Execute("insert into preferences ($getvar) values ('$local_contact_id')");
                    $colorscheme = DEFAULT_COLORSCHEME;
                }
            } else {
                //error getting user info
                $colorscheme = DEFAULT_COLORSCHEME;
            }
            if ($colorscheme == "random") {
                $colorscheme = randomscheme();
            }
            $_SESSION['SESSION_colorscheme'] = $colorscheme;	
        } else {
            if ($colorscheme != "") {
                if ($auth->userlevel == EMPLOYER || $auth->userlevel == HREMPLOYER) {
                    $gettable = "employer_login";
                    $getvar = "contact_id";
                }
                elseif ($auth->userlevel == OFFICE || $auth->userlevel == ADMINSTUDENT || 
                        $auth->userlevel == TRAFFICDIRECTOR || $auth->userlevel == FACULTY) {
                    $gettable = "contact_internal";
                    $getvar = "contact_id";
                }
                elseif ($auth ->userlevel == STUDENT) {
                    $gettable = "student";
                    $getvar = "student_number";
                }

                if(getfromTable($local_contact_id, $gettable, $getvar, $auth->department, $auth->login)) {
                    $result = $GLOBALS['dbh']->Execute("update preferences set colorscheme = '$colorscheme' where $getvar = '$local_contact_id'");
                }
                else {
                    if ($_SESSION['SESSION_loginID'] != ADMIN_LOGIN) {
                        $colorscheme = DEFAULT_COLORSCHEME; 
                    }
                }

                if ($colorscheme == "random") {
                    $colorscheme = randomscheme();
                }
            
                $_SESSION['SESSION_colorscheme'] = $colorscheme;
            }	

        }
    } else {
        $_SESSION['SESSION_colorscheme'] = DEFAULT_COLORSCHEME;
    } 	
}

//User tries to enter his own (unknown) colorscheme...
if (!(in_array($_SESSION['SESSION_colorscheme'], $schemes))) {
    $_SESSION['SESSION_colorscheme'] = DEFAULT_COLORSCHEME;
}


$scheme = get_hooks("colorscheme.getscheme", $_SESSION['SESSION_colorscheme']);
$GLOBALS['colorscheme'] = $scheme[0];
if (is_null($GLOBALS['colorscheme']) || $_SESSION['SESSION_colorscheme'] == 'none'){
        $scheme = array();
        $scheme['stylesheet']   = "misc/genericskin/stylesheet.css";
        $scheme['topbar_bg']    = "misc/genericskin/images/topbar_bg.jpg";
        $scheme['bg']    = "misc/genericskin/images/bg.jpg";
        $scheme['topbar_left']  = "misc/genericskin/images/topbar_left.jpg";
        $scheme['topbar_right'] = "misc/genericskin/images/topbar_right.jpg";
        $scheme['topright']     = "misc/genericskin/images/topright.jpg";
        $scheme['menutop']      = "misc/genericskin/images/menutop.jpg";
        $scheme['menubottom']   = "misc/genericskin/images/menubottom.jpg";
        $scheme['logo1']        = "misc/genericskin/images/logo1.jpg";
        $scheme['bullet_menuitem']      = "misc/genericskin/images/bullet_menuitem.jpg";
        $scheme['bullet_menusubitem']   = "misc/genericskin/images/bullet_menusubitem.jpg";
        $scheme['bullet_menusubitem1']  = "misc/genericskin/images/bullet_menusubitem1.jpg";
        $scheme['bullet_menusubitem2']  = "misc/genericskin/images/bullet_menusubitem2.jpg";
        $scheme['poweredby']  = "misc/genericskin/images/poweredby.gif";
        $GLOBALS['colorscheme'] = $scheme;
} 
// End of Code For Color Scheme

// code saving function ! pulls out the necessary info for the queries
// The department and login are both checked to make sure its unique (multiple login_ids can be the same in different depts)

function getfromTable(&$contact, $gettable, $getvar, $dep, $login) 
{
    // Some Login ID's might be getting pulled from netlink_id column.	
    if ($_SESSION['SESSION_netlog'] == SCRIPT_LOGIN) 
    {
        $login_column = "netlink_id";
    } 
    else 
    {
        $login_column = "login_id";
    }

    if ($gettable == 'student')
    {
        /*
         Since student's departments are now in student_department, this query must be treated differently.
        */

        $sql = ("
            SELECT DISTINCT student_number
            FROM student
            WHERE " . $login_column . "='" . $login . "'
            ");
        $result = $GLOBALS['dbh']->Execute($sql);
        while ($row = $result->FetchRow())
        {
            $student_ids .= ("'" . $row["student_number"] . "',");
        }
        $student_ids = substr($student_ids, 0, -1);

        if (!$student_ids)
        {
        //	 If no student_ids matched the login provided, do nothing.

            return false;
        }

        $sql = ("
            SELECT DISTINCT student_number
            FROM student_department
            WHERE department_id='" . $dep . "'
            AND student_number IN (" . $student_ids . ")
            ");
        $result = $GLOBALS['dbh']->Execute($sql);

        if ($result->RecordCount() == 1)
        {
            $row = $result->FetchRow();
            $contact = $row["student_number"];
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        if ($gettable && $getvar)
        {
        // Protection for cases such as forgotten password where login may be known but person isn't logging in
            $result = $GLOBALS['dbh']->Execute("
                SELECT $getvar AS number 
                FROM $gettable 
                WHERE department_id='" . $dep . "'
                AND $login_column='" . $login . "'
                ");
    
            if ($row = $result->FetchRow()) {
                $contact = $row['number'];
                return true;
            }
            else {
                return false;
            }
        } else {
            return false;
        }
    }
}
?>
