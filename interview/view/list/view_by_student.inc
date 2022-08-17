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
 | Filename: view_by_student.inc                                                |
 +------------------------------------------------------------------------------+
 | Description:                                                                 |
 +------------------------------------------------------------------------------+

*/

// filename: view_by_student.inc

if ($student_num)
{
    // Get record_id for use in quickmenu
    $department_str = "'" . implode("', '", $departments_in_group) . "'";
    $sql_get_recordid = "Select distinct record_id from student_department
        WHERE student_number='".$student_num."' and department_id='".$auth->department."'";
    $result_get_recordid=$GLOBALS['dbh']->Execute($sql_get_recordid);
    if ($result_get_recordid->RecordCount()==1)
    {
        $row_get_recordid = $result_get_recordid->FetchRow();
        $record_id = $row_get_recordid['record_id'];
    } elseif ($result_get_recordid->RecordCount()==0) {
        $sql_get_recordid2 = "Select distinct record_id from student_department
            WHERE student_number='".$student_num."' and department_id IN (".$department_str.")";
        $result_get_recordid2 = $GLOBALS['dbh']->Execute($sql_get_recordid2);
        if ($result_get_recordid2->RecordCount()==1)
        {
            $row_get_recordid2 = $result_get_recordid2->FetchRow();
            $record_id = $row_get_recordid2['record_id'];
        }
    }
}
elseif($student_name)
{

    $student_names = explode(",", $student_name);
    $first_name = trim($student_names[1]);
    $last_name = trim($student_names[0]);

    $department_str = "'" . implode("', '", $departments_in_group) . "'";
    $sql = ("
            SELECT DISTINCT sd.record_id
            FROM student AS s
            INNER JOIN student_department AS sd 
            ON sd.student_number=s.student_number
            WHERE sd.department_id IN (" . $department_str . ")
            ");

    if ($first_name)
    {
        $sql .= ("                                                                                                                       
                AND s.first_name LIKE '" . addslashes($first_name) . "%'                                                                                 
                ");
    }

    if ($last_name)
    {
        $sql .= ("                                                                                                                       
                AND s.last_name LIKE '" . addslashes($last_name) . "%'                                                                                   
                ");
    }

    $result_get_recordid = $GLOBALS['dbh']->Execute($sql);
    
    if ($result_get_recordid->RecordCount() == 1)
    {
        $row_get_recordid = $result_get_recordid->FetchRow();
        $record_id = $row_get_recordid['record_id'];
    }

}

echo("<table width='100%'>");
echo("<tr>");
echo("<td><h3 align='left'>Interviews - View by Student</h3></td>\n");

// Display the quickmenu
echo("<td align='right'>");
if ($record_id && !$go_button && $auth->department != TRAFFIC_DIRECTOR_DEPARTMENT)
{
    include('student_info/student_quickmenu.inc');
}
echo("</td>\n");
echo("</tr>");
echo("</table>");

?>
<?php
if ($department_id == ""){
        if ($departments_in_group > 1){
                if ($_SESSION['SESSION_netlog']==SCRIPT_LOGIN) {
                        $group_sql = "SELECT b.pulldown_menu_group
                        FROM contact_internal AS b
                        WHERE b.department_id='$auth->department' AND b.netlink_id='$login_id'";
                } else {
                        $group_sql = "SELECT b.pulldown_menu_group
                        FROM contact_internal AS b
                        WHERE b.department_id='$auth->department' AND b.login_id='$login_id'";
                }
                $result_group=$GLOBALS['dbh']->Execute($group_sql);
                while($row=$result_group->FetchRow()){
                        $pulldown_menu_group=$row['pulldown_menu_group'];
                }
                if ($pulldown_menu_group=='1'){
                        $department_in_str = "'" . implode("', '", $departments_in_group) . "'";
                        $department_id='0';
                } else {
                        $department_id = $auth->department;
                }
        } else {
                $department_id = $auth->department;
        }
}

switch ($submit)
{
   case "View Interviews":
      $level2 = "student_num_or_student_name";
      break;

   case "Select from List":
      $level2 = "student_list";
      break;
}
if (!in_array($auth->department, $departments_in_group)){
	error("You do not have access to that department.");
	$level2 = "";
}
$department_str = "";
$department_str = "'" . implode("', '", $departments_in_group) . "'";

if ($level2 == "student_num_or_student_name")
{
    if(!$go_button)
    {
        if($student_num)
        {
            if(!isValidStudentNumber($student_num, $department_str))
            {
                $level2 = "";
                error("A valid student number must be specified.");
            }
        }
        elseif($student_name)
        {
            
            $students_found = isValidStudentName($student_name, $department_str);

            //Changes the student's name to the right format
            $student_names = explode(",", $student_name);
            $first_name = trim($student_names[1]);
            $last_name = trim($student_names[0]);
            
            if($students_found == 0)
            {
                $level2 = "";
                error("A valid student name must be specified.");
            }
            elseif($students_found > 1)
            {
                //This variable prevents any other page besides multiple_students.inc from processing until 
                //otherwise set to 0 (see below)
                $error = 1;
                include("interview/view/list/multiple_students.inc");
            }
        }
        else
        {
            error("Please enter a valid student number, student name or choose a student below");
            $level2 = "";
        }
    }
    //Changes the faculty while remaining on current page
    else
    {
        $level2 = "";
    }    
}

//This loop ensures that no other page loads until multiple_students.inc is finished 
if($error == 0)
switch ($level2)
{
   case "student_list":
      include("interview/view/main/view_students.inc");
      break;

   case "student_num_or_student_name":
      // $student_num and/or $student_name variables are already set.  The included file can use them.
      // Use $student_num if it is present, else use $student_name
      include("interview/view/main/view_interviews_by_student.inc");
      break;
    
   default:
      include("interview/view/list/view_by_student_main.inc");
      break;
}

?>

