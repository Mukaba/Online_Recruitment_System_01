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
 | Filename: view_by_job_code.inc                                               |
 +------------------------------------------------------------------------------+
 | Description:                                                                 |
 +------------------------------------------------------------------------------+

*/

// filename: view_by_job_code.inc
?>
<h3 align="left">Interviews - View by Job Code</h3>
<?php

switch ($submit)
{
   case "Select from List":
      $level2 = "job_code_list";
      break;

   case "View Interview":
      $level2 = "job_id_or_job_code";
      break;
}
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
	error("You do not have access to that department.");
	$department_id = $auth->department;
	$level2= "";
}

if ($department_id == "0"){
	$department_str = "'" . implode("', '", $departments_in_group) . "'";
}else{
	$department_str = "'" . $department_id . "'";
}
// Check if students in department are in the job id
// If so.. bypass is set to yes, so that if the job isnt in the department, it still
// displays it, only without the quickmenu. (job_id must be found too)
//  --  //
	if ($job_id) {
		$sql = "SELECT c.first_name, a.job_id 
			FROM job_info AS a, students_shortlisted AS b, student AS c, student_department as sd
			WHERE ((c.student_number = b.student_number)  AND sd.department_id IN (".$department_str."))
			AND sd.student_number=c.student_number
			AND a.job_id = b.job_id 
			AND a.job_id = '$job_id'";
	} else {
		$sql = "SELECT c.first_name, a.job_id from job_info as a, students_shortlisted as b, student as c, student_department as sd
			WHERE ((c.student_number = b.student_number) and (sd.department_id IN (".$department_str.")))
			and sd.student_number = c.student_number
			and a.job_id = b.job_id and a.job_code = '$job_code'";

	}
	$result = $GLOBALS['dbh']->Execute($sql);

	if ($result->RecordCount()) {
		$bypass = "yes";
		$row = $result->FetchRow();
		$job_id = $row["job_id"];
	}

	$result->Close();
//  -- //

if ($level2 == "job_id_or_job_code" && !isValidJobID($job_id, $department_str) && $bypass != "yes")
{
	if (!(isValidJobCode($job_code, $department_str)))
        {
                /*
                 The job code and department that they have typed in didn't work, so lets
                 do a search for similar job codes and see what we get.
                */
		if (trim($job_code))
                {
			/*
			 Below has been added to ensure that we only pull out jobs that have interviews,
			 rather than interviews AND job descriptions.
			*/
			
			$sql = ("
			        SELECT DISTINCT a.job_id
			        FROM interview AS a, job_info AS b
			        WHERE b.department_id IN (" . $department_str . ")
			        AND a.job_id=b.job_id
			        ");
			$result = $GLOBALS['dbh']->Execute($sql);
			
			while ($row = $result->FetchRow())
			{
			        $valid_ids .= ("'" . $row["job_id"] . "',");
			}
	
			$valid_ids = substr($valid_ids, 0, (strlen($valid_ids) - 1));

			if ($valid_ids==NULL){
				$valid_ids="''";
			}

                        $sql = ("
                                SELECT j.job_code
                                FROM employer_company AS c, job_info AS j
                                WHERE c.employer_id=j.employer_id
                                AND j.department_id IN (" . $department_str . ")
                                AND j.job_code LIKE '" . $job_code . "%'
				AND j.job_id IN (" . $valid_ids . ")
                                ");
                        $result = $GLOBALS['dbh']->Execute($sql);

                        if ($result->RecordCount())
                        {
                                $level2 = "close_match";
                        }
                        else
                        {
                                notify("No interviews for the selected company and department.");
                                $level2 = "";
                        }
                }
                else
                {
                        notify("No interviews for the selected job code and department.");
                        $level2 = "";
                }

        }
}
switch ($level2)
{
   case "job_code_list":
      include("interview/view/main/view_job_codes.inc");
      break;

   case "job_id_or_job_code":
      // $job_id and/or $job_code variables are already set.  The included file can use them.
      // Use $job_id if it is present, else use $job_code
      include("interview/view/main/view_interviews_by_job_code.inc");
      break;

   case "close_match":
      include("interview/view/main/job_code_choose_list.inc");
      break;

   default:
      include("interview/view/list/view_by_job_code_main.inc");
      break;
}



?>

