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
 | Filename: add_student.inc                                                    |
 +------------------------------------------------------------------------------+
 | Description: Add a student to the student table.  The student may or may     |
 | not have a ENGR/CSC account, if they don't           they will not be able   |
 | to log in though (an admin can sign them up for interviews though) :TODO     |
 | Fix this code, but also be sure to allow the user to edit a student after    |
 | they've added it (add a button to do this).                                  |
 |                                                                              |
 |                                                                              |
 +------------------------------------------------------------------------------+

*/
?>     
</td><td align="left" valign="top"><img src="misc/images<?php echo $_SESSION['SESSION_colorscheme']; ?>/topbar_right.jpg" alt="" /></td></tr></table>

<table border="0" cellspacing="0" cellpadding="2" width="99%"><tr><td>
<?php

$first_name = trim($first_name);
$last_name = trim($last_name);
$student_num = trim($student_num);
$student_login = trim($student_login);
$department_id = $department;

//Set the NIS variable

$sql = ("
        SELECT DISTINCT default_login
        FROM department
        WHERE department_id='" . $department . "'
        ");
$result = $GLOBALS['dbh']->Execute($sql);
$row = $result->FetchRow();

$default_login = $row["default_login"];

echo("<h3 align='left'>Student Information - Add Student</h3>");
echo("</td></tr></table>");
echo("<table border='0' cellspacing='0' cellpadding='0' width='99%'><tr><td>");

// check to see what point of the adding process we've reached
switch ($step)
{
    case "2":
        if (!validate_step_1($first_name, $last_name, $student_num, $department_id, $gender, $admit_date, $applied_flag))
        {
            form_step_1($first_name, $last_name, $student_num, $department_id, $gender, $admit_date, $applied_flag);
        }
        else
        {
            // check to see if names match anything in the database already, or whether
            // the student number matches anything in the database already.
            if (isStudentDeptInDB($student_num, $department_id))
            {
                alreadyInDB($first_name, $last_name, $student_num, $department_id);
            }
            elseif (isStudentInDB($student_num) && !$verified)
            {
                // check to see if the information being entered corresponds to what exists
                // if we're dealing with someone being entered with the same student number but
                // different department
                $sql = "SELECT first_name, last_name, student_number, gender
                        FROM student 
                        WHERE student_number='".$student_num."'";
                $result = $GLOBALS['dbh']->Execute($sql);

                $student = $result->FetchRow();

                if ($gender != $student["gender"] || $first_name != $student["first_name"] || $last_name != $student["last_name"])
                {
                    instruct("The student you are adding has an existing entry in the database under a different department with the following information. 
                            If you add the student to your department now and wish to change the existing information below, you must edit it explicitly under the Edit section of Student Info.  
                            <br /><br />Are you sure you wish to add the student to your department?<br /><br />
                            <form method=\"post\" name=\"myform\" action=\"".$PHP_SELF."&amp;select=add_student\">
                            <input type=\"hidden\" name=\"step\" value=\"3\" />
                            <input type=\"hidden\" name=\"first_name\" value=\"".$first_name."\" />
                            <input type=\"hidden\" name=\"last_name\" value=\"".$last_name."\" />
                            <input type=\"hidden\" name=\"student_num\" value=\"".$student_num."\" />
                            <input type=\"hidden\" name=\"department\" value=\"".$department_id."\" />
                            <input type=\"hidden\" name=\"gender\" value=\"".$gender."\" />
                            <input type=\"hidden\" name=\"admit_date\" value=\"".$admit_date."\" />
                            <input type=\"hidden\" name=\"applied_flag\" value=\"".$applied_flag."\" />
                            <input type=\"submit\" value=\"Add Student\" />
                            </form>
                            ");
                    echo("<br /><br />");
                    echo("<table border=\"0\" width=\"100%\" cellspacing=\"0\">");
                    echo("<tr>");
                        echo("<td colspan=\"2\" align=\"left\" class=\"tableheaders\">Existing student information:</td>");
                        echo("<td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td>");
                        echo("<td colspan=\"2\" align=\"left\" class=\"tableheaders\">Ignored information:</td>");
                    echo("</tr>");
                   
                    echo("<tr><td><b>First Name:</b></td><td>" . $student["first_name"] . "</td>");
                    echo("<td></td>");
                    echo("<td><b>First Name:</b></td><td>" . $first_name . "</td></tr>");
                    
                    echo("<tr><td><b>Last Name:</b></td><td>" . $student["last_name"] . "</td>");
                    echo("<td></td>");
                    echo("<td><b>Last Name:</b></td><td>" . $last_name . "</td></tr>");

                    echo("<tr><td><b>Student Number:</b></td><td>" . $student["student_number"] . "</td>"); 
                    echo("<td></td>");
                    echo("<td><b>Student Number:</b></td><td>" . $student_num . "</td></tr>"); 

                    echo("<tr><td><b>Gender:</b></td><td>". (($student["gender"] == "M") ? "Male" : "Female") ."</td>");
                    echo("<td></td>");
                    echo("<td><b>Gender:</b></td><td>". (($gender == "M") ? "Male" : "Female") ."</td></tr>");
                    echo("</table>");
                }
                else
                {
                    add_student($first_name, $last_name, $student_num, $department_id, $gender, $admit_date, $applied_flag);
                }
            }
            else 
            {
                add_student($first_name, $last_name, $student_num, $department_id, $gender, $admit_date, $applied_flag);
            }
        }
    break;

    case "3":
        add_student($first_name, $last_name, $student_num, $department_id, $gender, $admit_date, $applied_flag, $student_login);
    break;

    default:
    form_step_1();
}
// END OF SCRIPT, FUNCTIONS BELOW

// function form_step_1
// purpose: Display the form for entering the student's name, student_number, and department 
function form_step_1($first_name = "", $last_name = "", $student_num = "", $department_id = "", $gender = "", $admit_date = "", $applied_flag = "", $verified = false)
{
    global $PHP_SELF;
    global $auth;

    ?>
        <form method="post" name="myform" action="<?php echo($PHP_SELF); ?>&amp;select=add_student">
        <table class="row1" cellspacing="0" cellpadding="0" border="0"><tr><td>
        <input type="hidden" name="step" value="2" />
        <table>
        <tr><td align="right">First Name:</td>
        <td><input type="text" name="first_name" maxlength="40" size="20" value="<?php echo(removeSlashes($first_name)); ?>" /></td></tr>
        <tr><td align="right">Last Name:</td>
        <td><input type="text" name="last_name" maxlength="40" size="20" value="<?php echo(removeSlashes($last_name)); ?>" /></td></tr>
        <tr><td align="right">Student Number:</td>
        <td><input type="text" name="student_num" maxlength="8" size="11" value="<?php echo(removeSlashes($student_num)); ?>" /></td></tr>
        <tr><td align="right">Department:</td>
        <td>
        <?php
        $sql = "SELECT department_id, department_code
        FROM department WHERE job_list = 1 ORDER BY department_code";
    $result = $GLOBALS['dbh']->Execute($sql);
    ?>
        <select name="department">

        <?php
        while ($row = $result->FetchRow()){
            if (removeSlashes($department_id) == $row["department_id"])
            {
                print "<option value=\"".$row["department_id"]."\" selected='selected'>".$row["department_code"]."</option>";
            }
            elseif ($department_id == "" && $auth->department == $row["department_id"])
            {
                print "<option value=\"".$row["department_id"]."\" selected='selected'>".$row["department_code"]."</option>";
            }
            else
            {
                print "<option value=\"".$row["department_id"]."\">".$row["department_code"]."</option>";
            }
        }
    ?>
        </select>
        </td></tr> 
        <tr><td align="right">Gender:</td>
        <td>
        <select name="gender">
        <option value="M" <?php (($gender == "M") ? print "selected='selected'":"") ?>>Male</option>
        <option value="F" <?php (($gender == "F") ? print "selected='selected'":"") ?>>Female</option>
        </select>
        </td></tr>
        <tr><td align="right">Date Admitted:</td>
        <td><input type="text" name="admit_date" maxlength="40" size="20" value="<?php echo(removeSlashes($admit_date)); ?>" />&nbsp;<?php echo(popup('admit_date', 'myform'));?></td>
        <td><b>-Or-</b></td>
        <td><input type="checkbox" name="applied_flag" value="true" <?php (($applied_flag) ? print "checked='checked'":"") ?> /> Applied</td></tr>

        <tr><td colspan="4" align="center"><hr /><input type="submit" value="Submit" /></td></tr>
        </table>
        </td></tr></table>
        </form>
        <?php
}


// function validate_step_1
// purpose: Validates the fields from form_1 checking whether they are filled in and the student number looks like a 
//          student number.  It returns true on validation, false otherwise and prints a message saying what fields
//          have not been filled in correctly if any fields were not valid.
function validate_step_1($first_name, $last_name, $student_num, $department_id, $gender, $admit_date, $applied_flag)
{
    $valid = true;

    if (!isValidName($first_name)) { error("A first name must be specified.<br />"); $valid = false; }
    if (!isValidName($last_name)) { error("A last name must be specified.<br />"); $valid = false; }
    if (!local_isValidStudentNumber($student_num)) { error("A valid student number must be specified.<br />"); $valid = false; }
    if (!isvalidDate($admit_date) xor $applied_flag) { error("Either a valid admittance date in YYYY-MM-DD format must be specified, or the Applied flag must be checked (not both).<br />"); $valid = false; }

    return $valid;
}



// function isValidName
// purpose: Determines whether a text is a non-empty string - not all whitespace.  Returns true if it is, false othewise.
function isValidName($name)
{
    return (strlen($name) > 0);
}


// function local_isValidStudentNumber
// purpose: Determines whether the string is numbers and is 5 to 8 numbers long.  True if it is, false otherwise.
function local_isValidStudentNumber($num)
{
    return preg_match("/^\d{5,8}$/", $num);
}


// function form_step_2
// purpose: Displays the second form of the process.  This form only applies to students in ENGR/CSC, it allows 
//          the input of the login for their ENGR or CSC account.
function form_step_2($first_name = "", $last_name = "", $student_num = "", $department_id = "", $student_login = "")
{
    global $PHP_SELF;
    ?>
        <form method="post" action="<?php echo($PHP_SELF); ?>&amp;select=add_student">
        <table class="row1" cellspacing="0" cellpadding="0" border="0"><tr><td>
        <input type="hidden" name="step" value="3" />
        <input type="hidden" name="first_name" value="<?php echo($first_name); ?>" />
        <input type="hidden" name="last_name" value="<?php echo($last_name); ?>" />
        <input type="hidden" name="student_num" value="<?php echo($student_num); ?>" />
        <input type="hidden" name="department" value="<?php echo($department_id); ?>" />
        <table>
        <tr><td align="right">First Name:</td>
        <td><?php echo(removeSlashes($first_name)); ?></td></tr>
        <tr><td align="right">Last Name:</td>
        <td><?php echo(removeSlashes($last_name)); ?></td></tr>
        <tr><td align="right">Student Number:</td>
        <td><?php echo(removeSlashes($student_num)); ?></td></tr>
        <tr><td align="right" valign="top">ENGR / CSC login:</td>
        <td><input type="text" name="student_login" value="<?php echo($student_login); ?>" /></td></tr> 
        <tr><td colspan="2" align="center"><input type="submit" value="Submit" /></td></tr>
        </table>
        </td></tr></table>
        </form>
        <?php
}


// function: add_student
// purpose: Performs final validation then adds student to the student table
function add_student($first_name, $last_name, $student_num, $department_id, $gender, $admit_date, $applied_flag, $student_login = "*")
{
    global $auth;

    if (validate_step_1($first_name, $last_name, $student_num, (($student_login == "*") ? "no" : "yes"), $gender, $admit_date, $applied_flag))
    {
        $sql = ("
            SELECT student_number
            FROM student
            WHERE student_number = '".addslashes(removeSlashes($student_num))."'
            ");
        $result = $GLOBALS['dbh']->Execute($sql);

        if ($result->RecordCount())
        {
            $sql="INSERT INTO student_department (student_number,department_id) values ('".$student_num."',".$department_id.")";
            $GLOBALS['dbh']->Execute($sql);

            $sql = "SELECT record_id from student_department where student_number='".$student_num."' and department_id=".$department_id;
            $results=$GLOBALS['dbh']->Execute($sql);
            $row=$results->FetchRow();

            // Automatically sets the current flag	
            $sql = "INSERT into student_flags_join (student_flags_id,record_id) values (".CURRENT_FLAG.",".$row['record_id'].")";
            $GLOBALS['dbh']->Execute($sql);

            // set the applied flag if it was checked
            if ($applied_flag)
            {
                $sql = "INSERT INTO student_flags_join (student_flags_id,record_id) values (".APPLIED_FLAG.",".$row['record_id'].")";
                $GLOBALS['dbh']->Execute($sql);
            }
        }
        else
        {
            $result = $GLOBALS['dbh']->Execute("INSERT INTO student (first_name, last_name, student_number, current, gender) VALUES('" . addslashes(removeSlashes($first_name)) . "', '" . addslashes(removeSlashes($last_name)) . "', '" . addslashes(removeSlashes($student_num)) . "','1','" . addslashes(removeSlashes($gender)) . "')");
            $result = $GLOBALS['dbh']->Execute("INSERT INTO student_department (student_number,department_id, admit) values ('".$student_num."',".$department_id.",'".$admit_date."')");	

            $sql = "SELECT record_id from student_department where student_number='".$student_num."' and department_id=".$department_id;
            $results=$GLOBALS['dbh']->Execute($sql);
            $row=$results->FetchRow();

            // Automatically sets the current flag	
            $sql = "INSERT into student_flags_join (student_flags_id,record_id) values (".CURRENT_FLAG.",".$row['record_id'].")";
            $GLOBALS['dbh']->Execute($sql);	

            // set the applied flag if it was checked
            if ($applied_flag)
            {
                $sql = "INSERT INTO student_flags_join (student_flags_id,record_id) values (".APPLIED_FLAG.",".$row['record_id'].")";
                $GLOBALS['dbh']->Execute($sql);
            }
            
            if (SCRIPT_LOGIN == 'netlink'){
                // Query Netlink server for Netlink ID
                $string_code = "N:S:".$student_num;
                $validator_info[]="";
                exec(SCRIPT_PATH . "/client.pl $string_code", $validator_info);
                list ($return_code, $return_number) = split(":", $validator_info[1]);
                if ($return_code == '1'){
                    $sql = "UPDATE student SET netlink_id='" . $return_number . "' where student_number='" . $student_num . "'";
                    $result=$GLOBALS['dbh']->Execute($sql);
                }
            }

            $sql_department_type= "SELECT DISTINCT default_login FROM department WHERE department_id='$department_id'";
            $result_department_type = $GLOBALS['dbh']->Execute($sql_department_type);
            while ($row_login = $result_department_type->FetchRow()) {
                $default_login2 = $row_login['default_login'];
            }
        }

        $result = $GLOBALS['dbh']->Execute("
                SELECT s.student_number, d.department_code, d.department_id, s.netlink_id 
                FROM student as s, department as d, student_department as sd
                WHERE s.student_number='" . addslashes(removeSlashes($student_num)) . "'  
                AND sd.department_id=d.department_id AND s.student_number=sd.student_number AND sd.department_id=".$department_id);

        if ($result->RecordCount())
        {
            $row = $result->FetchRow();
            notify("The following record was successfully added:<br />");
            echo("<table>");
            echo("<tr><td><b>First Name:</b></td><td>" . removeSlashes($first_name) . "</td></tr>");
            echo("<tr><td><b>Last Name:</b></td><td>" . removeSlashes($last_name) . "</td></tr>");
            echo("<tr><td><b>Student Number:</b></td><td>" . removeSlashes($student_num) . "</td></tr>"); 
            echo("<tr><td><b>Netlink:</b></td><td>" . (($row['netlink_id'] == "*") ? "(none)" : $row['netlink_id']) . "</td></tr>");
            echo("<tr><td><b>Department:</b></td><td>" . (($row["department_code"]== "") ? "Other" : $row["department_code"]) . "</td></tr>");
            echo("<tr><td><b>Gender:</b></td><td>". (($gender == "M") ? "Male" : "Female") ."</td></tr>");

            if ($applied_flag) 
            {
                echo("<tr><td><b>Applied flag set</b></td></tr>");
            }
            else
            {
                echo("<tr><td><b>Date Admitted:</b></td><td>" . removeSlashes($admit_date) ."</td></tr>");
            }
            echo("</table>");
            echo("<br />");
            echo("<h3>Enter Another:</h3>");
            form_step_1();
        }
        else
        {
            echo("An error occured: the entry for " . removeSlashes($first_name) . " " .  removeSlashes($last_name) . " failed.<br />");
        }
    }
    else
    {
        echo("An error occured: the entry could not be added.");
    }
    // validate first_name, last_name, student_num, student_login then proceed with adding it if okay
}


// function isStudentDeptInDB
// purpose: Checks to see whether a student number and department combination is in the student_department table.  Returns number of rows returned.  Remember, 
//          false = 0, true = non-zero.
function isStudentDeptInDB($student_num, $department_id)
{
    $sql = ("
        SELECT student_number
        FROM student_department
        WHERE student_number = '".addslashes(removeSlashes($student_num))."'
        AND department_id = '".addslashes(removeSlashes($department_id))."'
        ");
    $result = $GLOBALS['dbh']->Execute($sql);
    return $result->RecordCount();
}


// function isStudentInDB
// purpose: Checks to see whether a student number is in the student table.  Returns number of rows returned.  Remember, 
//          false = 0, true = non-zero.
function isStudentInDB($student_num)
{
    $sql = ("
        SELECT student_number
        FROM student
        WHERE student_number = '".addslashes(removeSlashes($student_num))."'
        ");
    $result = $GLOBALS['dbh']->Execute($sql);
    return $result->RecordCount();
}


// function alreadyInDB
// purpose: If a corresponding entry is already in the database this function calls the correct form to return
function alreadyInDB($first_name, $last_name, $student_num, $department_id)
{
    global $default_login;

    form_no_changes($first_name, $last_name, $student_num);
}



// function form_warn
function form_warn($first_name, $last_name, $student_num, $department_id, $returned_login)
{
    global $PHP_SELF;
    ?>
        <form method="post" action="<?php echo($PHP_SELF); ?>&amp;select=add_student">
        <table cellspacing="0" cellpadding="0" border="1"><tr><td>
        <input type="hidden" name="step" value="3" />
        <input type="hidden" name="first_name" value="<?php echo(removeSlashes($first_name)); ?>" />
        <input type="hidden" name="last_name" value="<?php echo(removeSlashes($last_name)); ?>" />
        <input type="hidden" name="student_num" value="<?php echo(removeSlashes($student_num)); ?>" />
        <input type="hidden" name="department_id" value="<?php echo($department_id); ?>" />
        <input type="hidden" name="bypass" value="true" />
        <table>
        <tr><td>
        <?php

        if ($returned_login == "*")
        {
            error(removeSlashes($first_name) . " " . removeSlashes($last_name) . ", student number " . removeSlashes($student_num) . ", is already recorded in the system.  There is 
                    no login currently (that is, no record of a corresponding ENGR or CSC account).  The record should 
                    not be changed unless " . removeSlashes($first_name) . " " . removeSlashes($last_name) . " now has an ENGR or CSC account.  If you still 
                    wish to continue select proceed.");
        }
        else
        {
            error(removeSlashes($first_name) . " " . removeSlashes($last_name) . ", student number " . removeSlashes($student_num) . ", already has an account with login '$returned_login'.
                    It is not recommended that you modify this unless the login is incorrect.  If you still wish to 
                    continue select proceed.");
        }

    ?>
        </td></tr>
        <tr><td align="center"><input type="submit" value="Proceed" /></td></tr>
        </table>
        </td></tr></table>
        </form>
        <?php
}


// function form_no_changes
// purpose: Output for when the entry is already in the student table and no changes need to be made.
function form_no_changes($first_name, $last_name, $student_num)
{
    $sql = "Select first_name, last_name from student where student_number='".$student_num."'";
    $results=$GLOBALS['dbh']->Execute($sql);
    $row=$results->FetchRow();

    error("An entry for student number " . removeSlashes($student_num) . " is already in the database under the name ".$row['first_name']." ".$row['last_name'].".<br />No changes were made.");
}



?>

</td></tr></table>
