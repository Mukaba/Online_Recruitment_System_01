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
 | Filename: authenticate.inc                                                   |
 +------------------------------------------------------------------------------+
 | Description: This file is responsible for checking login and password, and   |
 | for setting the appropriate variables. Variables such as                     |
 | $authenticate->department and $authenticate->login are used throughout the   |
 | code.                                                                        |
 |                                                                              |
 | Any authenticate_class class should expose the following: Variables:         |
 | $userlevel, $login, $department, and $name Function: function                |
 | authenticate()                                                               |
 +------------------------------------------------------------------------------+

*/

include_once('LDAP/ldap_include.inc.php');

class authenticate_class {
    var $userlevel  = USERLEVEL_PUBLIC;
    var $login      = "";
    var $department = NONE;
    var $name       = "";
    var $contact_id;
    var $group_departments;
    var $student_number;


    function authenticate_class() {
		if (AUTH_USING_NIS_LOGIN) {
			// $validators and $group_validators are used for NIS/YP authentication
			$validators = Array(
					Array("domain"=>"", "map"=>"")
					);
			$group_validators = Array(
					Array("domain"=>"", "map"=>"", "node"=>"")
					);
		}

        /*
Note:	The first group in this array that a user matches is the one that determines
their userlevel and department.
         */

        $result = $GLOBALS['dbh']->Execute("
                SELECT department_id, department_code 
                FROM department
                ");
        while ($row = $result->FetchRow())
        {
            $departments[$row["department_code"]] = $row["department_id"];
        }

	if (AUTH_USING_NIS_LOGIN) {
        $this->group_departments = Array(
                Array("group"=>"",
                    "userlevel"=>OFFICE,
                    "department"=>$departments[""])
                );
	}

    } // function authenticate_class()


    function check_authentication() {
    }

    function authenticate() {
        // variables coming in from the login form.
        $loginID = $_POST['loginID'];
        $passwd  = $_POST['passwd'];

        /* session variables:
           $_SESSION['SESSION_loginID'];
           $_SESSION['SESSION_passwd'];
           $_SESSION['SESSION_fullname'];
           $_SESSION['SESSION_encrypt'];
           $_SESSION['SESSION_check'];
           $_SESSION['SESSION_department'];
           $_SESSION['SESSION_netlog'];
           $_SESSION['SESSION_new_employer'];
           $_SESSION['SESSION_forgotten_password'];
           $_SESSION['SESSION_multiple_type'];
         */
        global $new_employer;
        global $forgotten_password;
        global $student_number_login, $student_number_login2; // Needed to get student department_id with ADMINSTUDENT logins.

        if ($_SESSION['SESSION_encrypt'] != "encrypted") {
            if (LOGIN_LOWER_CASE) {
                $loginID = strtolower($loginID);
            }

            $validated = false;

            // NetLink login
            if (!$validated) {
                $validator_info = array();
                $passwd = stripslashes($passwd);
                if ($loginID != NULL) {
                    if (AUTH_USING_SCRIPT_LOGIN) {
                        exec(SCRIPT_PATH."/".SCRIPT_LOGIN_FILE." $loginID $passwd", $validator_info);
                    } else if (AUTH_USING_LDAP_LOGIN) {
                        $netlink = new ldap_netlink($loginID, $passwd);
                        $validator_info[] = $netlink->get_validation_info();
                    }
                }
                list ($junk,$validator_string) = split("ret = ", $validator_info[0]);
                list ($success_code, $validator_type, $validator_number) = split(":",$validator_string);
                if (DEBUG)
                {  // *** DEBUG CODE BELOW ***
                    dumpToLog("validator_number=".$validator_number." \n");
                }  // *** DEBUG CODE ABOVE ***
                if ($success_code == "1") {
                    $authCR = crypt($validator_number, $validator_type.$validator_type );
                    $_SESSION['SESSION_loginID'] = $loginID;
                    $_SESSION['SESSION_passwd']  = $authCR;
                    $_SESSION['SESSION_encrypt'] = 'encrypted';
                    $validated = true;
                }
            }

            // If the constant shows that NIS logins are enabled, attempt an NIS login
            if (AUTH_USING_NIS_LOGIN) {
                if (!$validated) {
                    $limit = count($this->validators);
                    for ($i = 0; $i < $limit && !$validated; $i++) {
                        $entry = yp_match($this->validators[$i]["domain"],
                                $this->validators[$i]["map"], $loginID);
                        list ($id, $authCR, , , $fullname,) = split(":", $entry);
                        if ($authCR != "" && crypt(stripslashes($passwd), substr($authCR, 0, 2)) == $authCR)
                        {
                            $_SESSION['SESSION_loginID'] = $loginID;
                            $_SESSION['SESSION_passwd']  = $authCR;
                            $_SESSION['SESSION_encrypt'] = 'encrypted';
                            $validated = true;
                        }
                    }
                }
            }

            //employer login
            if (!$validated) {
                $sql = ("
                        SELECT login_id, password as password
                        FROM employer_login
                        WHERE login_id = ?
                        ");
                $stmt = $GLOBALS['dbh']->Prepare($sql);
                $result = $GLOBALS['dbh']->Execute($stmt,
                        array(stripslashes($loginID)));

                if ($result && $result->RecordCount() == 1) {
                    $row  = $result->FetchRow();
                    $pass = $row["password"];

                    if ($pass != "" && $pass==stripslashes(crypt($passwd, "EM"))) 
                    {
                        $_SESSION['SESSION_loginID'] = $loginID;
                        $_SESSION['SESSION_passwd']  = $pass;
                        $_SESSION['SESSION_encrypt'] = 'encrypted';
                        $validated = true;
                    }
                }
            }

            // see misc/release_constants.inc for ADMIN_LOGIN and ADMIN_STUDENT
            if (!$validated) {
                if ((($loginID == ADMIN_LOGIN) && ADMIN_ENABLED) || (($loginID == STUDENT_LOGIN) && STUDENT_ENABLED))
                {
                    $_SESSION['SESSION_loginID'] = $loginID;
                    $_SESSION['SESSION_passwd']  = $loginID;
                    $_SESSION['SESSION_encrypt'] = 'encrypted';
                }
                else
                {
                    $this->login = $_SESSION['SESSION_loginID'] = $loginID;
                    $_SESSION['SESSION_passwd'] = $passwd;
                    $_SESSION['SESSION_encrypt'] = $encrypt;
                }
            }

            // They've gotten here without matching anything, so they must be a new employer, let's check and make sure..
            if ($new_employer) {
                //New employer
                $this->userlevel = NEW_EMPLOYER;
                $_SESSION['SESSION_new_employer'] = 'new_employer';
                $_SESSION['SESSION_encrypt'] = 'encrypted';
                $_SESSION['SESSION_passwd'] = 'not_needed';
            }

            // Have the employers forgotten their password?
            if ($forgotten_password) {
                // Forgotten password
                $this->userlevel = FORGOTTEN_PASSWORD;

                $_SESSION['SESSION_forgotten_password'] = 'forgotten_password';
                $_SESSION['SESSION_encrypt'] = 'encrypted';
                $_SESSION['SESSION_passwd'] = 'not_needed';
            }
        } // if ($_SESSION['SESSION_encrypt'] == "encrypted")

        // We are encrypted now.  Proceed.  This is the second time through.
        if (($_SESSION['SESSION_passwd'] == "") || ($_SESSION['SESSION_passwd'] == "nopass"))
        {
            $this->userlevel = USERLEVEL_PUBLIC;
        }
        else
        {
            // Reset validated.
            $validated = false;

            // check NetLink if validated = false;
            if (!$validated)
            {
                $salt = substr($_SESSION['SESSION_passwd'], 0, 2);
                $id_number = substr($_SESSION['SESSION_passwd'], 2);

                $sql = ("
                        SELECT ci.employee_number
                        , ci.department_id
                        , c.type
                        FROM contact_internal AS ci
                        INNER JOIN contact AS c ON c.contact_id=ci.contact_id
                        WHERE ci.netlink_id = ?
                        ");
                $stmt = $GLOBALS['dbh']->Prepare($sql);
                $result = $GLOBALS['dbh']->Execute($stmt,
                        array(stripslashes($_SESSION['SESSION_loginID'])));

                if (DEBUG)
                {  // *** DEBUG CODE BELOW ***
                    dumpToLog("In NetLink2 but not checked \n");
                }  // *** DEBUG CODE ABOVE ***

                if ($result && $result->RecordCount() == 1 && $row = $result->FetchRow())
                {
                    $authCR = crypt($row["employee_number"], $salt);

                    if (DEBUG)
                    {  // *** DEBUG CODE BELOW ***
                        dumpToLog("authCR=".$authCR." \n");
                        dumpToLog("salt=".$salt." \n");
                    }  // *** DEBUG CODE ABOVE ***

                    if ($authCR != "" && $_SESSION['SESSION_passwd'] == $authCR)
                    {
                        if ($salt == EMPLOYER_SALT || $salt == OFFICE_SALT)
                        {
                            if (DEBUG)
                            {  // *** DEBUG CODE BELOW ***
                                dumpToLog("Employer or Office salt \n");
                                dumpToLog("type=".$row['type']."\n");
                            }  // *** DEBUG CODE ABOVE ***
                            $validated = true;
                            if ($row["type"] == "internal")
                            {
                                $this->userlevel=OFFICE;
                            } 
                            elseif ($row["type"] == "faculty")
                            {
                                $this->userlevel=FACULTY;
                            }

                            $this->department=$row["department_id"];

                            if (!$_SESSION['SESSION_department']) {
                                $_SESSION['SESSION_department'] = $this->department;
                            }

                            $stmt = $GLOBALS['dbh']->Prepare("
                                    SELECT CONCAT(a.first_name, ' ', a.last_name) AS name 
                                    FROM contact AS a, contact_internal AS b
                                    WHERE a.contact_id=b.contact_id 
                                    AND b.netlink_id = ?
                                    ");
                            $result = $GLOBALS['dbh']->Execute($stmt,
                                    array($_SESSION['SESSION_loginID']));
                            while ($row=$result->FetchRow())
                            {
                                $myname=$row['name'];
                            }
                            $_SESSION['SESSION_fullname'] = $myname;
                            $_SESSION['SESSION_netlog'] = SCRIPT_LOGIN;

                            global $admin_student_login;
                            if ($admin_student_login=='yes')
                            {
                                $this->userlevel = ADMINSTUDENT;

                                // Set the department variable to this student's department
                                if(trim($student_number_login) == "") 
                                {
                                    $student_number_login = $student_number_login2;
                                }

                                $this->department = $_SESSION['SESSION_department'];
                                $_SESSION['SESSION_department'] = $this->department;
                                $departments_in_group = department_groups($this->department);

                                $stmt = $GLOBALS['dbh']->Prepare("
                                        SELECT s.student_number 
                                        FROM student AS s, student_department AS sd 
                                        WHERE s.student_number= ?
                                        AND sd.student_number=s.student_number 
                                        AND sd.department_id IN ('" . implode("', '", $departments_in_group) . "')
                                        ");
                                $result = $GLOBALS['dbh']->Execute($stmt,
                                        array($student_number_login));
                                $row = $result->FetchRow();
                                $this->student_number = $row["student_number"];
                            }

                            global $admin_employer_login;
                            if ($admin_employer_login=='yes')
                            {
                                $this->userlevel = ADMINEMPLOYER;
                            }
                            $this->login = $_SESSION['SESSION_loginID'];
                            $this->name = $_SESSION['SESSION_fullname'];

                            $stmt = $GLOBALS['dbh']->Prepare("
                                    SELECT DISTINCT contact_id 
                                    FROM contact_internal 
                                    WHERE netlink_id = ?
                                    AND department_id = ?
                                    ");
                            $result3 = $GLOBALS['dbh']->Execute($stmt,
                                    array($_SESSION['SESSION_loginID'],
                                        $this->department));
                            $row3 = $result3->FetchRow();

                            $this->contact_id = $row3["contact_id"];

                            if ($_SESSION['SESSION_department'] == TRAFFIC_DIRECTOR_DEPARTMENT)
                            {
                                // If their department is the traffic director one, set the userlevel.
                                if ($admin_employer_login=='yes')
                                {
                                    $this->userlevel = ADMINEMPLOYER;
                                }
                                else
                                {
                                    $this->userlevel = TRAFFICDIRECTOR;
                                }
                            }

                            if ($_SESSION['SESSION_check'] != 'checked')
                                $this->save_login("netlink_id");
                        } 
                        else 
                        {
                            $validated = false;
                            $this->userlevel=USERLEVEL_PUBLIC;
                        }
                    }
                } // if ($result && $result->RecordCount() == 1 && $row = $result->FetchRow())
                elseif ($result && $result->RecordCount() > 1 && $row=$result->FetchRow())
                {
                    $authCR = crypt($row["employee_number"], $salt);

                    if ($authCR != "" && $_SESSION['SESSION_passwd'] == $authCR)
                    {
                        if ($salt == EMPLOYER_SALT || $salt == OFFICE_SALT)
                        {
                            if (!$_SESSION['SESSION_department'])
                            {
                                $validated = true;
                                $this->userlevel = MULTIPLE;
                                $multiple_type = "office";
                                $_SESSION['SESSION_multiple_type'] = $multiple_type;
                                $_SESSION['SESSION_fullname'] = '';
                                $_SESSION['SESSION_netlog'] = SCRIPT_LOGIN;
                            }
                            else 
                            {
                                $validated = true;
                                $this->department = $_SESSION['SESSION_department'];
                                $sql = ("
                                        SELECT CONCAT(a.first_name, ' ', a.last_name) AS name
                                        , a.type
                                        FROM contact AS a, contact_internal AS b
                                        WHERE a.contact_id=b.contact_id
                                        AND b.netlink_id = ?
                                        AND b.department_id = ?
                                        ");
                                $stmt = $GLOBALS['dbh']->Prepare($sql);
                                $result = $GLOBALS['dbh']->Execute($stmt,
                                        array($_SESSION['SESSION_loginID'],
                                            $_SESSION['SESSION_department']));
                                while ($row = $result->FetchRow())
                                {
                                    if (DEBUG)
                                    {  // *** DEBUG CODE BELOW ***
                                        dumpToLog("Employer or Office salt 2\n");
                                        dumpToLog("type=".$row['type']."\n");
                                    }  // *** DEBUG CODE ABOVE ***
                                    $myname = $row["name"];
                                    if ($row["type"] == "internal")
                                    {
                                        $this->userlevel = OFFICE;
                                    }
                                    elseif ($row["type"] == "faculty")
                                    {
                                        $this->userlevel = FACULTY;
                                    }
                                }

                                $_SESSION['SESSION_fullname'] = $myname;
                                $_SESSION['SESSION_netlog'] = SCRIPT_LOGIN;

                                global $admin_student_login;	
                                if ($admin_student_login == 'yes')
                                {
                                    $this->userlevel = ADMINSTUDENT;

                                    // Set the department variable to this student's department
                                    if(trim($student_number_login) == "") 
                                    {
                                        $student_number_login = $student_number_login2;
                                    }

                                    $this->department = $_SESSION['SESSION_department'];
                                    $_SESSION['SESSION_department'] = $this->department;
                                    $departments_in_group = department_groups($this->department);

                                    $stmt = $GLOBALS['dbh']->Prepare("
                                            SELECT s.student_number 
                                            FROM student AS s, student_department AS sd 
                                            WHERE s.student_number = ?
                                            AND sd.student_number=s.student_number 
                                            AND sd.department_id IN ('" . implode("', '", $departments_in_group) . "')
                                            ");
                                    $result = $GLOBALS['dbh']->Execute($stmt,
                                            array($student_number_login));
                                    $row = $result->FetchRow();
                                    $this->student_number = $row["student_number"];
                                }

                                global $admin_employer_login;	
                                if ($admin_employer_login == 'yes')
                                {
                                    $this->userlevel = ADMINEMPLOYER;
                                }
                                $this->login = $_SESSION['SESSION_loginID'];
                                $this->name = $_SESSION['SESSION_fullname'];

                                if ($_SESSION['SESSION_department'] == TRAFFIC_DIRECTOR_DEPARTMENT)
                                {
                                    // If their department is the traffic director one, set the userlevel.
                                    if ($admin_employer_login=='yes')
                                    {
                                        $this->userlevel = ADMINEMPLOYER;
                                    }
                                    else
                                    {
                                        $this->userlevel = TRAFFICDIRECTOR;
                                    }
                                }

                                if ($_SESSION['SESSION_check'] != 'checked')
                                {
                                    $this->save_login("netlink_id");
                                }
                            }
                            $stmt = $GLOBALS['dbh']->Prepare("
                                    SELECT DISTINCT contact_id 
                                    FROM contact_internal
                                    WHERE netlink_id = ?
                                    AND department_id = ?
                                    ");
                            $result3 = $GLOBALS['dbh']->Execute($stmt,
                                    array($_SESSION['SESSION_loginID'],
                                        $_SESSION['SESSION_department']));
                            $row3 = $result3->FetchRow();

                            $this->contact_id = $row3["contact_id"];
                        }
                        else
                        {
                            $validated = false;
                            $this->userlevel = USERLEVEL_PUBLIC;
                        }
                    }
                } // elseif ($result && $result->RecordCount() > 1 && $row=$result->FetchRow())
                else 
                {

                    if (DEBUG)
                    {  // *** DEBUG CODE BELOW ***
                        dumpToLog("In NetLink3 but not checked \n");
                    }  // *** DEBUG CODE ABOVE ***

                    // Pull out student records matching our criteria, but only if they do not have a withdraw date set.
                    $sql = ("
                            SELECT s.student_number, sd.department_id
                            FROM student AS s
                            INNER JOIN student_department AS sd ON s.student_number=sd.student_number
                            WHERE s.netlink_id = ?
                            AND (sd.withdraw='' OR sd.withdraw IS NULL)
                            ORDER BY sd.department_id DESC
                            ");
                    $stmt = $GLOBALS['dbh']->Prepare($sql);
                    $result = $GLOBALS['dbh']->Execute($stmt,
                            array(stripslashes($_SESSION['SESSION_loginID'])));

                    $sql = ("
                            SELECT DISTINCT d.menus_being_used_id
                            FROM student AS s
                            INNER JOIN student_department AS sd ON s.student_number=sd.student_number
                            INNER JOIN department AS d ON d.department_id=sd.department_id
                            AND (sd.withdraw='' OR sd.withdraw IS NULL)
                            WHERE s.netlink_id = ?
                            ORDER BY sd.department_id DESC
                            ");
                    $stmt = $GLOBALS['dbh']->Prepare($sql);
                    $result2 = $GLOBALS['dbh']->Execute($stmt,
                            array(stripslashes($_SESSION['SESSION_loginID'])));

                    // Determine if we've got any menu conflicts.
                    $conflicting_menus = FALSE;
                    while ($row = $result2->FetchRow())
                    {
                        if (!is_array($differing_menus) || in_array($row["menus_being_used_id"], $differing_menus))
                        {
                            $differing_menus[] = $row["menus_being_used_id"];
                        }
                        else
                        {
                            $conflicting_menus = TRUE;
                            break;
                        }
                    }

                    if ($result && $result->RecordCount() > 1 && $conflicting_menus && $row=$result->FetchRow())
                    {
                        $authCR = crypt($row["student_number"], $salt);

                        if (DEBUG)
                        {  // *** DEBUG CODE BELOW ***
                            dumpToLog("authCRs".$authCR." \n");
                            dumpToLog("salts".$salt." \n");
                        }  // *** DEBUG CODE ABOVE ***

                        // Student netlink login
                        if ($authCR != "" && $_SESSION['SESSION_passwd'] == $authCR)
                        {
                            if ($salt == STUDENT_SALT)
                            {
                                if (!$_SESSION['SESSION_department'])
                                {
                                    // The student has multiple departments, and those departments have conflicting menus_being_used_id's.
                                    // menu_being_used_ids conflicts, so give them the option of choosing their department.
                                    $validated = true;
                                    $this->userlevel = MULTIPLE;
                                    $multiple_type = "student";
                                    $_SESSION['SESSION_multiple_type'] = $multiple_type;
                                    $_SESSION['SESSION_fullname'] = '';
                                    $_SESSION['SESSION_netlog'] = SCRIPT_LOGIN;
                                }
                                else
                                {
                                    $validated = true;
                                    $this->userlevel=STUDENT;
                                    $this->department = $_SESSION['SESSION_department'];

                                    $stmt = $GLOBALS['dbh']->Prepare("
                                            SELECT student_number 
                                            FROM student 
                                            WHERE netlink_id = ?");
                                    $result = $GLOBALS['dbh']->Execute($stmt,
                                            array($_SESSION['SESSION_loginID']));

                                    $row = $result->FetchRow();
                                    $this->student_number = $row["student_number"];

                                    // Check to see that the student does NOT have a withdraw date set.  If they do, don't let them login.
                                    // Here we know the student has only one department
                                    $sql = ("
                                            SELECT DISTINCT sd.record_id
                                            FROM student_department AS sd
                                            WHERE sd.student_number = ?
                                            AND (sd.withdraw='' OR sd.withdraw IS NULL)
                                            ");
                                    $stmt = $GLOBALS['dbh']->Prepare($sql);
                                    
                                    $result = $GLOBALS['dbh']->Execute($stmt,
                                        array($this->student_number));
                                    if (!$result->RecordCount())
                                    {
                                        // Don't let this student login
                                        $validated = false;
                                        $this->userlevel = USERLEVEL_PUBLIC;
                                        $this->department = '';
                                        $this->student_number = '';
                                    }
                                    else
                                    {
                                        // The student can login
                                        $stmt = $GLOBALS['dbh']->Prepare("
                                                SELECT CONCAT(a.first_name, ' ', a.last_name) AS name 
                                                FROM student AS a 
                                                WHERE a.netlink_id= ?
                                                ");
                                        $result =$GLOBALS['dbh']->Execute($stmt,
                                                array($_SESSION['SESSION_loginID']));
                                        while ($row=$result->FetchRow())
                                        {
                                            $myname=$row['name'];
                                        }
                                        $_SESSION['SESSION_fullname'] = $myname;
                                        $_SESSION['SESSION_netlog'] = SCRIPT_LOGIN;
                                        $this->login = $_SESSION['SESSION_loginID'];
                                        $this->name = $_SESSION['SESSION_fullname'];
                                        if ($_SESSION['SESSION_check'] != 'checked')
                                        {
                                            $this->save_login("netlink_id");
                                        }
                                    }
                                }
                            } 
                            else 
                            {
                                $validated = false;
                                $this->userlevel=USERLEVEL_PUBLIC;
                            }
                        }
                    } // if ($result && $result->RecordCount() > 1 && $conflicting_menus && $row=$result->FetchRow())
                    elseif ($result && $result->RecordCount() && $row=$result->FetchRow())
                    {
                        $authCR = crypt($row["student_number"], $salt);

                        if (DEBUG)
                        {  // *** DEBUG CODE BELOW ***
                            dumpToLog("authCRs".$authCR." \n");
                            dumpToLog("salts".$salt." \n");
                        }  // *** DEBUG CODE ABOVE ***

                        if ($authCR != "" && $_SESSION['SESSION_passwd'] == $authCR)
                        {
                            if ($salt == STUDENT_SALT)
                            {
                                $validated = true;
                                $this->userlevel=STUDENT;
                                $this->department=$row["department_id"];

                                $stmt = $GLOBALS['dbh']->Prepare("
                                        SELECT student_number 
                                        FROM student 
                                        WHERE netlink_id = ?");
                                $result = $GLOBALS['dbh']->Execute($stmt,
                                        array($_SESSION['SESSION_loginID']));

                                $row = $result->FetchRow();
                                $this->student_number = $row["student_number"];

                                // Check to see that the student does NOT have a withdraw date set.  If they do, don't let them login.
                                // Here we know the student has only one department
                                $sql = ("
                                        SELECT DISTINCT sd.record_id
                                        FROM student_department AS sd
                                        WHERE sd.student_number = ?
                                        AND (sd.withdraw='' OR sd.withdraw IS NULL)
                                        ");
                                $stmt = $GLOBALS['dbh']->Prepare($sql);
                                $result = $GLOBALS['dbh']->Execute($stmt,
                                        array($this->student_number));
                                if (!$result->RecordCount())
                                {
                                    // Don't let this student login
                                    $validated = false;
                                    $this->userlevel = USERLEVEL_PUBLIC;
                                    $this->department = '';
                                    $this->student_number = '';
                                }
                                else
                                {
                                    // They can login
                                    $stmt = $GLOBALS['dbh']->Prepare("
                                            SELECT CONCAT(a.first_name, ' ', a.last_name) AS name 
                                            FROM student AS a 
                                            WHERE a.netlink_id= ?
                                            ");
                                    $result = $GLOBALS['dbh']->Execute($stmt,
                                           array($_SESSION['SESSION_loginID']));
                                    while ($row=$result->FetchRow())
                                    {
                                        $myname=$row['name'];
                                    }
                                    $_SESSION['SESSION_fullname'] = $myname;
                                    $_SESSION['SESSION_netlog'] = SCRIPT_LOGIN;
                                    $this->login = $_SESSION['SESSION_loginID'];
                                    $this->name = $_SESSION['SESSION_fullname'];
                                    if ($_SESSION['SESSION_check'] != 'checked')
                                    {
                                        $this->save_login("netlink_id");
                                    }
                                }
                            } 
                            else 
                            {
                                $validated = false;
                                $this->userlevel=USERLEVEL_PUBLIC;
                            }
                        }
                    } // elseif ($result && $result->RecordCount() && $row=$result->FetchRow())
                } // else 
            } // if (!$validated)

            if (!$validated)
            {
                if (AUTH_USING_NIS_LOGIN)
                {
                    // NIS login
                    for ($i = 0; $i < count($this->validators) && !$validated; $i++)
                    {
                        $entry = yp_match($this->validators[$i]["domain"], $this->validators[$i]["map"], $_SESSION['SESSION_loginID']);
                        list ($id, $authCR, $junk) = split(":", $entry);
                        $validated = $authCR != "" && $_SESSION['SESSION_passwd'] == $authCR;
                    }
                    for ($i = 0; $i < count($this->group_validators); $i++)
                    {
                        $pieces[$i] = explode(",", yp_match($this->group_validators[$i]["domain"], $this->group_validators[$i]["map"], $_SESSION['SESSION_loginID'] . $this->group_validators[$i]["node"]));
                    }
                }
                if ($validated && $this->userlevel != EMPLOYER)
                {
                    $this->userlevel = LIMBO;
                    $elements = count($this->group_departments);
                    $pieces_num = count($pieces);
                    for ($i = 0, $no_match = true; $i < $elements && $no_match; $i++)
                    {
                        for ($j = 0; $j < $pieces_num && $no_match; $j++)
                        {
                            if (in_array($this->group_departments[$i]["group"], $pieces[$j]))
                            {
                                $this->userlevel = $this->group_departments[$i]["userlevel"];
                                $no_match = false;
                            }
                        }
                    }
                    if ($this->userlevel == OFFICE || $this->userlevel == FACULTY) 
                    {
                        /*
                           Check if they have another department and contact id that they would
                           use their netlink ID to get into, as it will be bypassed otherwise.
                         */

                        $sql = ("
                                SELECT employee_number
                                FROM contact_internal
                                WHERE login_id = ?
                                ");
                        $stmt = $GLOBALS['dbh']->Prepare($sql);
                        $result = $GLOBALS['dbh']->Execute($stmt,
                                array($_SESSION['SESSION_loginID']));

                        if ($result->RecordCount() > 1)
                        {
                            if (!$_SESSION['SESSION_department'])
                            {
                                if (DEBUG)
                                {  // *** DEBUG CODE BELOW ***
                                    dumpToLog("no department \n");
                                }  // *** DEBUG CODE ABOVE ***
                                $validated = true;
                                $this->userlevel = MULTIPLE;
                                $multiple_type = "office";
                                $_SESSION['SESSION_multiple_type'] = $multiple_type;
                                $_SESSION['SESSION_fullname'] = '';
                                $_SESSION['SESSION_netlog'] = NIS_LOGIN;
                            }
                            else
                            {
                                if (DEBUG)
                                {  // *** DEBUG CODE BELOW ***
                                    dumpToLog("yes department \n");
                                }  // *** DEBUG CODE ABOVE ***
                                if ($_SESSION['SESSION_netlog'] == NIS_LOGIN)
                                {
                                    $sql = ("
                                            SELECT CONCAT(a.first_name, ' ', a.last_name) AS name
                                            FROM contact AS a, contact_internal AS b
                                            WHERE a.contact_id=b.contact_id
                                            AND b.login_id='" . $_SESSION['SESSION_loginID'] . "'
                                            AND b.department_id='" . $_SESSION['SESSION_department'] . "'
                                            ");
                                    $result = $GLOBALS['dbh']->Execute($sql);
                                    $login_used = NIS_LOGIN;
                                    $save_login_var = 'login_id';
                                }
                                else
                                {
                                    $sql = ("
                                            SELECT CONCAT(a.first_name, ' ', a.last_name) AS name
                                            FROM contact AS a, contact_internal AS b
                                            WHERE a.contact_id=b.contact_id
                                            AND b.netlink_id='" . $_SESSION['SESSION_loginID'] . "'	
                                            AND b.department_id='" . $_SESSION['SESSION_department'] . "'
                                            ");
                                    $result = $GLOBALS['dbh']->Execute($sql);
                                    $login_used = SCRIPT_LOGIN;
                                    $save_login_var = 'netlink_id';
                                }

                                while ($row = $result->FetchRow())
                                {
                                    $myname = $row["name"];
                                }

                                $this->department = $_SESSION['SESSION_department'];
                                $this->login = $_SESSION['SESSION_loginID'];

                                $_SESSION['SESSION_fullname'] = $myname;
                                $_SESSION['SESSION_netlog'] = $login_used;

                                global $admin_student_login;
                                if ($admin_student_login == 'yes')
                                {
                                    $this->userlevel = ADMINSTUDENT;

                                    if(trim($student_number_login) == "") 
                                    {
                                        $student_number_login = $student_number_login2;
                                    }

                                    // Set the department variable to this student's department
                                    $this->department = $_SESSION['SESSION_department'];
                                    $_SESSION['SESSION_department'] = $this->department;
                                    $departments_in_group = department_groups($this->department);

                                    $stmt = $GLOBALS['dbh']->Prepare("
                                            SELECT s.student_number 
                                            FROM student AS s, student_department AS sd 
                                            WHERE s.student_number= ?
                                            AND sd.student_number=s.student_number 
                                            AND sd.department_id IN ('" . implode("', '", $departments_in_group) . "')
                                            ");
                                    $result = $GLOBALS['dbh']->Execute($stmt,
                                            array($student_number_login));
                                    $row = $result->FetchRow();
                                    $this->student_number = $row["student_number"];
                                }

                                global $admin_employer_login;
                                if ($admin_employer_login == 'yes')
                                {
                                    $this->userlevel = ADMINEMPLOYER;
                                }

                                if ($_SESSION['SESSION_department'] == TRAFFIC_DIRECTOR_DEPARTMENT)
                                {
                                    // We have a traffic director, so give them that userlevel.
                                    if ($admin_employer_login=='yes')
                                    {
                                        $this->userlevel = ADMINEMPLOYER;
                                    }
                                    else
                                    {
                                        $this->userlevel = TRAFFICDIRECTOR;
                                    }
                                }

                                if ($_SESSION['SESSION_check'] != 'checked')
                                {
                                    $this->save_login($save_login_var);
                                }
                            }	
                        } // if ($result->RecordCount() > 1)
                        else
                        {
                            if (DEBUG)
                            {  // *** DEBUG CODE BELOW ***
                                dumpToLog("one department \n");
                            }  // *** DEBUG CODE ABOVE ***

                            $result = $GLOBALS['dbh']->Execute("
                                    SELECT CONCAT(a.first_name, ' ', a.last_name) AS name, b.department_id 
                                    FROM contact AS a, contact_internal AS b
                                    WHERE a.contact_id=b.contact_id 
                                    AND b.login_id='".$_SESSION['SESSION_loginID']."'
                                    ");

                            while ($row=$result->FetchRow())
                            {
                                $myname=$row['name'];
                                $this->department = $row['department_id'];
                                if (DEBUG)
                                {  // *** DEBUG CODE BELOW ***
                                    dumpToLog("In cscengr office \n");
                                }  // *** DEBUG CODE ABOVE ***
                            }
                            $_SESSION['SESSION_fullname'] = $myname;
                            $_SESSION['SESSION_netlog'] = NIS_LOGIN;

                            global $admin_student_login;
                            if ($admin_student_login=='yes')
                            {
                                $this->userlevel = ADMINSTUDENT;

                                if(trim($student_number_login) == "") 
                                {
                                    $student_number_login = $student_number_login2;
                                }

                                // Set the department variable to this student's department
                                $this->department = $_SESSION['SESSION_department'];
                                $_SESSION['SESSION_department'] = $this->department;
                                $departments_in_group = department_groups($this->department);

                                $stmt = $GLOBALS['dbh']->Prepare("
                                        SELECT s.student_number 
                                        FROM student AS s, student_department AS sd 
                                        WHERE s.student_number= ?
                                        AND sd.student_number=s.student_number 
                                        AND sd.department_id IN ('" . implode("', '", $departments_in_group) . "')
                                        ");
                                $result = $GLOBALS['dbh']->Execute($stmt,
                                        array($student_number_login));
                                $row = $result->FetchRow();
                                $this->student_number = $row["student_number"];
                            }

                            global $admin_employer_login;
                            if ($admin_employer_login=='yes')
                            {
                                $this->userlevel = ADMINEMPLOYER;
                            }

                            if ($_SESSION['SESSION_check'] != 'checked')
                            {
                                $this->save_login("login_id");
                            }

                            if (!$_SESSION['SESSION_department'])
                            {
                                $_SESSION['SESSION_department'] = $this->department;
                            }

                            if ($_SESSION['SESSION_department'] == TRAFFIC_DIRECTOR_DEPARTMENT)
                            {
                                // If their in the traffic director department, set the userlevel.
                                $this->userlevel = TRAFFICDIRECTOR;
                            }
                        } // else

                        $result3 = $GLOBALS['dbh']->Execute("
                                SELECT DISTINCT contact_id from contact_internal 
                                WHERE login_id='".$_SESSION['SESSION_loginID']."' 
                                AND department_id='".$_SESSION['SESSION_department']."'
                                ");
                        $row3 = $result3->FetchRow();
                        $this->contact_id = $row3["contact_id"];
                    } // if ($this->userlevel == OFFICE) 
                    elseif ($this->userlevel == STUDENT)
                    {
                        $result = $GLOBALS['dbh']->Execute("
                                SELECT CONCAT(a.first_name, ' ', a.last_name) AS name, a.student_number, sd.department_id 
                                FROM student AS a
                                INNER JOIN student_department AS sd ON a.student_number=sd.student_number
                                WHERE a.login_id='".$_SESSION['SESSION_loginID']."' 
                                AND (sd.withdraw='' OR sd.withdraw IS NULL)
                                ORDER BY sd.department_id DESC
                                ");

                        $sql = ("
                                SELECT DISTINCT d.menus_being_used_id
                                FROM student AS s
                                INNER JOIN student_department AS sd ON sd.student_number=s.student_number
                                INNER JOIN department AS d ON d.department_id=sd.department_id
                                WHERE s.login_id= ?
                                AND (sd.withdraw='' OR sd.withdraw IS NULL)
                                ORDER BY sd.department_id DESC
                                ");
                        $stmt = $GLOBALS['dbh']->Prepare($sql);
                        $result2 = $GLOBALS['dbh']->Execute($stmt,
                                array(stripslashes($_SESSION['SESSION_loginID'])));

                        // Determine if we've got any menu conflicts.
                        $conflicting_menus = FALSE;
                        while ($row = $result2->FetchRow())
                        {
                            if (!is_array($differing_menus) || in_array($row["menus_being_used_id"], $differing_menus))
                            {
                                $differing_menus[] = $row["menus_being_used_id"];
                            }
                            else
                            {
                                $conflicting_menus = TRUE;
                                break;
                            }
                        }

                        if ($result->RecordCount() > 1 && $conflicting_menus)
                        {
                            if (!$_SESSION['SESSION_department'])
                            {
                                // The student has multiple departments, and conflicting menus_being_used_ids.
                                $validated = true;
                                $this->userlevel = MULTIPLE;
                                $multiple_type = "student";
                                $_SESSION['SESSION_multiple_type'] = $multiple_type;
                                $_SESSION['SESSION_fullname'] = '';
                                $_SESSION['SESSION_netlog'] = NIS_LOGIN;
                            }
                            else
                            {
                                $this->department = $_SESSION['SESSION_department'];

                                $result = $GLOBALS['dbh']->Execute("
                                        SELECT CONCAT(a.first_name, ' ', a.last_name) AS name, a.student_number
                                        FROM student AS a
                                        WHERE a.login_id='".$_SESSION['SESSION_loginID']."'
                                        ");
                                while ($row=$result->FetchRow())
                                {
                                    $myname=$row['name'];
                                    $this->student_number = $row["student_number"];
                                }
                                $_SESSION['SESSION_fullname'] = $myname;
                                $_SESSION['SESSION_netlog'] = NIS_LOGIN;

                                if ($_SESSION['SESSION_check'] != 'checked')
                                {
                                    $this->save_login("login_id");
                                }
                            }
                        } // if ($result->RecordCount() > 1 && $conflicting_menus)
                        elseif ($result && $result->RecordCount() && $row = $result->FetchRow())
                        {
                            $myname=$row['name'];
                            $this->student_number = $row["student_number"];
                            $this->department=$row['department_id'];
                            if (DEBUG)
                            {  // *** DEBUG CODE BELOW ***
                                dumpToLog("In cscengr student \n");
                            }  // *** DEBUG CODE ABOVE ***

                            // Check to see that the student does NOT have a withdraw date set.  If they do, don't let them login.
                            // Here we know the student has only one department
                            $sql = ("
                                    SELECT DISTINCT sd.record_id
                                    FROM student_department AS sd
                                    WHERE sd.student_number='" . $this->student_number . "'
                                    AND (sd.withdraw='' OR sd.withdraw IS NULL)
                                    ");
                            $result = $GLOBALS['dbh']->Execute($sql);
                            if (!$result->RecordCount())
                            {
                                // Don't let this student login
                                $validated = false;
                                $this->userlevel = USERLEVEL_PUBLIC;
                                $this->department = '';
                                $this->student_number = '';
                            }
                            else
                            {
                                // They can login
                                $_SESSION['SESSION_fullname'] = $myname;
                                $_SESSION['SESSION_netlog'] = NIS_LOGIN;

                                if ($_SESSION['SESSION_check'] != 'checked')
                                {
                                    $this->save_login("login_id");
                                }
                            }
                        } // elseif ($result && $result->RecordCount() && $row = $result->FetchRow())
                        else
                        {
                            $this->userlevel = LIMBO;
                        }
                    } // elseif ($this->userlevel == STUDENT)
                    $this->login = $_SESSION['SESSION_loginID'];
                    $this->name = $_SESSION['SESSION_fullname'];
                } // if ($validated && $this->userlevel != EMPLOYER)
            } // if (!$validated)

            // check employer if validated = false;
            if(!$validated)
            {
                $sql = "
                    SELECT password AS password
                    FROM employer_login
                    WHERE login_id = ?
                    ";
                $stmt = $GLOBALS['dbh']->Prepare($sql);
                $result = $GLOBALS['dbh']->Execute($stmt,
                    array(stripslashes($_SESSION['SESSION_loginID'])));

                if ($result && $result->RecordCount() == 1 && $row=$result->FetchRow())
                {
                    $authCR = $row["password"];
                    if ($authCR != "" && $_SESSION['SESSION_passwd'] == $authCR)
                    {
                        $validated = true;
                        $this->userlevel=EMPLOYER;
                    }
                }
            }

            // see misc/release_constants.inc for ADMIN_LOGIN, STUDENT_LOGIN, ADMIN_ENABLED, STUDENT_ENABLED
            if (($_SESSION['SESSION_loginID'] == ADMIN_LOGIN) && ADMIN_ENABLED) 
            {
                //$this->userlevel = TRAFFICDIRECTOR;
                $this->userlevel = OFFICE;

                /*
                   The line below is the sole determinant of the admin login's department ID.
                   Change this number to the desired department to login under a different 
                   department.
                 */

                $this->department = 1; // see department table in DB for appropriate value

                $_SESSION['SESSION_department'] = $this->department;

                global $admin_student_login;
                if ($admin_student_login=='yes')
                {
                    $this->userlevel = ADMINSTUDENT;

                    if(trim($student_number_login) == "") 
                    {
                        $student_number_login = $student_number_login2;
                    }

                    // Set the department variable to this student's department
                    $this->department = $_SESSION['SESSION_department'];
                    $_SESSION['SESSION_department'] = $this->department;
                    $departments_in_group = department_groups($this->department);

                    $stmt = $GLOBALS['dbh']->Prepare("
                            SELECT s.student_number 
                            FROM student AS s, student_department AS sd 
                            WHERE s.student_number = ?
                            AND sd.student_number=s.student_number 
                            AND sd.department_id IN ('" . implode("', '", $departments_in_group) . "')
                            ");
                    $result = $GLOBALS['dbh']->Execute($stmt,
                            array($student_number_login));
                    $row = $result->FetchRow();
                    $this->student_number = $row["student_number"];
                }

                global $admin_employer_login;
                if ($admin_employer_login=='yes')
                {
                    $this->userlevel = ADMINEMPLOYER;
                }
                $this->login = $_SESSION['SESSION_loginID'];
                $this->name = $_SESSION['SESSION_fullname'];
            }
            elseif (($_SESSION['SESSION_loginID'] == STUDENT_LOGIN) && STUDENT_ENABLED) 
            {
                $this->userlevel = STUDENT;

                /*
                   The line below is the sole determinant of the ADMINSTUDENT department ID.
                   Change this number to the desired department to log in under a different department.
                 */

                $this->department = 1; // see department table in DB for appropriate value
                $this->login = $_SESSION['SESSION_loginID'];
                $this->name = $_SESSION['SESSION_fullname'];
            }
            elseif($validated && $this->userlevel == EMPLOYER)
            {
                $result = $GLOBALS['dbh']->Execute("
                        SELECT CONCAT(a.first_name, ' ', a.last_name) AS name, b.contact_id, b.department_id
                        FROM contact AS a, employer_login as b
                        WHERE a.contact_id=b.contact_id 
                        AND b.login_id = '".$_SESSION['SESSION_loginID']."'
                        ");
                while($row = $result->FetchRow())
                {
                    $myname = $row['name'];
                    $contact_id = $row['contact_id'];
                    $department_id = $row['department_id'];
                }

                $_SESSION['SESSION_fullname'] = $myname;
                $_SESSION['SESSION_netlog'] = NIS_LOGIN;
                //$this->userlevel = HREMPLOYER;	
                $this->contact_id = $contact_id;
                $this->department = $department_id;
                $this->login = $_SESSION['SESSION_loginID'];
                $this->name = $_SESSION['SESSION_fullname'];

                if (DEBUG)
                {  // *** DEBUG CODE BELOW ***
                    dumpToLog("employer about to save\n");
                }  // *** DEBUG CODE ABOVE ***

                if ($_SESSION['SESSION_check'] != "checked")
                {
                    $this->save_login("login_id");
                }
            }
            elseif ($_SESSION['SESSION_new_employer'] == "new_employer")
            {
                $this->userlevel = NEW_EMPLOYER;
            }
            elseif ($_SESSION['SESSION_forgotten_password'] == "forgotten_password")
            {
                $this->userlevel = FORGOTTEN_PASSWORD;
            }
            elseif(!(($this->userlevel == STUDENT) || ($this->userlevel == MULTIPLE) || ($this->userlevel == OFFICE) 
                        || ($this->userlevel == ADMINSTUDENT) || ($this->userlevel == ADMINEMPLOYER) || ($this->userlevel == NEW_EMPLOYER)
                        || ($this->userlevel == FORGOTTEN_PASSWORD) || ($this->userlevel == TRAFFICDIRECTOR)
                        || ($this->userlevel == FACULTY)))
            {
                $this->userlevel = USERLEVEL_PUBLIC;
            }
        } // else
    } // function authenticate()

    function save_login($logvalue)
    {
        if ($_SESSION['SESSION_check'] != "checked")
        {
            if ($this->login && $this->userlevel)
            {
                global $client;
                global $REMOTE_ADDR;

                $log_login_id = $this->login;
                $log_last_datetime_in = date("Y-m-d H:i:s");
                $log_last_browser = $client->browser;
                $log_last_platform = $client->platform;
                $log_last_IP_address = $REMOTE_ADDR;

                if ($this->userlevel == EMPLOYER || $this->userlevel == HREMPLOYER)
                {

                    if (DEBUG)
                    {  // *** DEBUG CODE BELOW ***
                        dumpToLog("In employer log\n");
                    }  // *** DEBUG CODE ABOVE ***

                    $sql = ("
                            SELECT DISTINCT contact_id
                            FROM employer_login
                            WHERE $logvalue= ?
                            ");
                    //:TODO FINISH CHECKING THIS OUT
                    $stmt = $GLOBALS['dbh']->Prepare($sql);
                    $result = $GLOBALS['dbh']->Execute($stmt, $this->login);
                    $row = $result->FetchRow();
                    $log_contact_id = $row["contact_id"];

                    $sql = ("
                            SELECT DISTINCT num_logins
                            FROM login_info
                            WHERE contact_id='" . $log_contact_id . "'
                            ");
                    $num_result = $GLOBALS['dbh']->Execute($sql);
                    if ($num_result->RecordCount() == 0)
                    {
                        $log_num_logins = 1;
                    }
                    else
                    {
                        $num_rows = $num_result->FetchRow();
                        $log_num_logins = $num_rows["num_logins"] + 1;
                    }

                    if ($log_num_logins == 1)
                    {

                        if (DEBUG)
                        {  // *** DEBUG CODE BELOW ***
                            dumpToLog("new log\n");
                        }  // *** DEBUG CODE ABOVE ***

                        $sql = ("
                                INSERT INTO login_info(
                                    login_id, contact_id, last_datetime_in, num_logins, last_browser,
                                    last_platform, last_IP_address)
                                VALUES(	?, ?, ?, ?, ?, ?, ?)  ");
                        $stmt = $GLOBALS['dbh']->Prepare($sql);
                        $GLOBALS['dbh']->Execute($stmt,
                                array($log_login_id,
                                    $log_contact_id,
                                    $log_last_datetime_in,
                                    $log_num_logins,
                                    $log_last_browser,
                                    $log_last_platform,
                                    $log_last_IP_address));
                    }
                    else
                    {

                        if (DEBUG)
                        {  // *** DEBUG CODE BELOW ***
                            dumpToLog("update log\n");
                        }  // *** DEBUG CODE ABOVE ***

                        $sql = ("
                                UPDATE login_info SET
                                last_datetime_in= ?,
                                num_logins= ?,
                                last_browser= ?,
                                last_platform=?,
                                last_IP_address=?
                                WHERE contact_id=?
                                ");
                        $stmt = $GLOBALS['dbh']->Prepare($sql);
                        $GLOBALS['dbh']->Execute($stmt,
                            array($log_last_datetime_in,
                                $log_num_logins,
                                $log_last_browser,
                                $log_last_platform,
                                $log_last_IP_address,
                                $log_contact_id));
                    }
                } // if ($this->userlevel == EMPLOYER || $this->userlevel == HREMPLOYER)
                elseif (($this->userlevel == OFFICE || $this->userlevel == FACULTY) && $this->department)
                {
                    $sql = ("
                            SELECT DISTINCT contact_id
                            FROM contact_internal
                            WHERE department_id = ?
                            AND $logvalue = ?
                            ");
                    $stmt = $GLOBALS['dbh']->Prepare($sql);
                    $result = $GLOBALS['dbh']->Execute($stmt,
                            array($this->department, $this->login));
                    $row = $result->FetchRow();

                    $log_contact_id = $row["contact_id"];

                    $sql = ("
                            SELECT DISTINCT num_logins
                            FROM login_info
                            WHERE contact_id='" . $log_contact_id . "'
                            ");
                    $num_result = $GLOBALS['dbh']->Execute($sql);

                    if ($num_result->RecordCount() == 0)
                    {
                        $log_num_logins = 1;
                    }
                    else
                    {
                        $num_rows = $num_result->FetchRow();
                        $log_num_logins = $num_rows["num_logins"] + 1;
                    }

                    if ($log_num_logins == 1)
                    {
                        $sql = ("
                                INSERT INTO login_info(
                                    login_id, contact_id, last_datetime_in, num_logins, last_browser,
                                    last_platform, last_IP_address)
                                VALUES (?, ?, ?, ?, ?, ?, ?)
                                ");
                        $stmt = $GLOBALS['dbh']->Prepare($sql);
                        $GLOBALS['dbh']->Execute($stmt, 
                                array($log_login_id,
                                    $log_contact_id,
                                    $log_last_datetime_in,
                                    $log_num_logins,
                                    $log_last_browser,
                                    $log_last_platform,
                                    $log_last_IP_address));
                    }
                    else
                    {
                        if ($log_contact_id == 0)
                        {
                            $sql = ("
                                    UPDATE login_info SET
                                    last_datetime_in= ?,
                                    num_logins= ?,
                                    last_browser= ?,
                                    last_platform= ?,
                                    last_IP_address= ?,
                                    login_id= ?
                                    WHERE contact_id= ?
                                    ");
                            $params = array($log_last_datetime_in,
                                    $log_num_logins,
                                    $log_last_browser,
                                    $log_last_platform,
                                    $log_last_IP_address,
                                    $this->login,
                                    $log_contact_id);
                        }
                        else
                        {
                            $sql = ("
                                    UPDATE login_info SET
                                    last_datetime_in= ?,
                                    num_logins= ?,
                                    last_browser= ?,
                                    last_platform= ?,
                                    last_IP_address= ?
                                    WHERE contact_id= ?
                                    ");
                            $params = array($log_last_datetime_in,
                                    $log_num_logins,
                                    $log_last_browser,
                                    $log_last_platform,
                                    $log_last_IP_address,
                                    $log_contact_id);
                        }
                        $stmt = $GLOBALS['dbh']->Prepare($sql);
                        $GLOBALS['dbh']->Execute($stmt, $params);
                    }
                } // elseif (($this->userlevel == OFFICE || $this->userlevel == FACULTY) && $this->department)
                elseif ($this->userlevel == STUDENT && $this->department)
                {
                    $sql = ("
                            SELECT DISTINCT s.student_number
                            FROM student AS s, student_department AS sd
                            WHERE s.$logvalue= ?
                            AND sd.department_id= ?
                            AND sd.student_number=s.student_number
                            ");
                    $stmt = $GLOBALS['dbh']->Prepare($sql);
                    $result = $GLOBALS['dbh']->Execute($stmt,
                            array($this->login, $this->department));	
                    $row = $result->FetchRow();	

                    $log_student_number = $row["student_number"];

                    $sql = ("
                            SELECT DISTINCT num_logins
                            FROM login_info
                            WHERE student_number= ?
                            ");
                    $stmt = $GLOBALS['dbh']->Prepare($sql);
                    $num_result = $GLOBALS['dbh']->Execute($stmt, 
                        array($log_student_number));

                    if ($num_result->RecordCount() == 0)
                    {
                        $log_num_logins = 1;
                    }
                    else
                    {
                        $num_rows = $num_result->FetchRow();
                        $log_num_logins = $num_rows["num_logins"] + 1;
                    }

                    if ($log_num_logins == 1)
                    {
                        $sql = ("
                                INSERT INTO login_info(
                                    login_id, student_number, last_datetime_in, num_logins, last_browser,
                                    last_platform, last_IP_address)
                                VALUES ( ?, ?, ?, ?, ?, ?, ?)
                                ");
                        $stmt = $GLOBALS['dbh']->Prepare($sql);
                        $GLOBALS['dbh']->Execute($stmt,
                                array($log_login_id,
                                    $log_student_number,
                                    $log_last_datetime_in,
                                    $log_num_logins,
                                    $log_last_browser,
                                    $log_last_platform,
                                    $log_last_IP_address));
                    }
                    else
                    {
                        if (!$log_student_number)
                        {
                            $sql = ("
                                    UPDATE login_info SET
                                    last_datetime_in= ?,
                                    num_logins= ?,
                                    last_browser= ?,
                                    last_platform= ?,
                                    last_IP_address= ?,
                                    login_id= ?
                                    WHERE student_number= ?
                                    ");
                            $params = array($log_last_datetime_in,
                                    $log_num_logins,
                                    $log_last_browser,
                                    $log_last_platform,
                                    $log_last_IP_address,
                                    $this->login,
                                    $log_student_number);
                        }
                        else
                        {
                            $sql = ("
                                    UPDATE login_info SET
                                    last_datetime_in= ?,
                                    num_logins= ?,
                                    last_browser= ?,
                                    last_platform= ?,
                                    last_IP_address= ?
                                    WHERE student_number= ?
                                    ");
                            $params = array($log_last_datetime_in,
                                    $log_num_logins,
                                    $log_last_browser,
                                    $log_last_platform,
                                    $log_last_IP_address,
                                    $log_student_number);
                        }
                        $stmt = $GLOBALS['dbh']->Prepare($sql);
                        $GLOBALS['dbh']->Execute($stmt, $params);
                    }
                } // elseif ($this->userlevel == STUDENT && $this->department)

                // Set the check variable now, so that we know we've recorded the login.
                $_SESSION['SESSION_check'] = 'checked';
            }
        } // if ($_SESSION['SESSION_check'] != "checked")
    } // function save_login($logvalue)

    function in_group($group_list, $pieces)
    {
        $groups_num = count($group_list);
        $pieces_num = count($pieces);
        for ($i = 0; $i < $groups_num; $i++)
        {
            for ($j = 0; $j < $pieces_num; $j++)
            {
                if (in_array($group_list[$i], $pieces[$j])) return true;
            }
        }
        return false;
    }

    function logout() {
        session_unset();
    }
} // end of class authenticate

?>
