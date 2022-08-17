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
 | Filename: view_by_date.inc                                                   |
 +------------------------------------------------------------------------------+
 | Description: show form to select format for viewing current interviews  can  |
 | view by date, job_code or student                                            |
 |                                                                              |
 |                                                                              |
 +------------------------------------------------------------------------------+

*/
?>
<h3 align="left">Interviews - View by Date</h3>
<?php

$date = $cal_date;
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

if ($department_id != "0" && !in_array($department_id, $departments_in_group)){
	error("You do not have access to this department.");
	$level2 == "";
	$department_id = $auth->department;
}
if ($department_id == "0"){
	$department_in_str = "'" . implode("', '" , $departments_in_group) . "'";
}else{
	$department_in_str = "'" . $department_id . "'";
}
if ($btnsubmit != "")
{
   if ($level2 != "" && !isValidDate($date))
   {
      error("A valid date must be specified.");
      $level2 = "";
   }

   switch ($level2)
   {
      case "all_interviews":
         // $date and $department variables are already set.  The included file can use them.
        include("interview/view/main/view_interviews_by_date.inc");
         break;

      case "all_interviews_by_student":
         // $date and $department variables are already set.  The included file can use them.
        include("interview/view/main/view_interviews_by_date_by_student.inc");
         break;

      case "day_schedule":
         // $date and $department variables are already set.  The included file can use them.
         include("interview/view/main/view_day_schedule.inc");
         break;

      case "lunch_schedule":
         // $date and $department variables are already set.  The included file can use them.
         include("interview/view/main/view_lunch_schedule.inc");
         break;

      default:
         include("interview/view/list/view_by_date_main.inc");
         break;
   }
}
else
{
   include("interview/view/list/view_by_date_main.inc");
}

?>
