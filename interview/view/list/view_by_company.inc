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
 | Filename: view_by_company.inc                                                |
 +------------------------------------------------------------------------------+
 | Description:                                                                 |
 +------------------------------------------------------------------------------+

*/

// filename: view_by_company.inc

?>
<h3 align="left">Interviews - View by Company</h3>
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
   case "View Results":
      $level2 = "company_name_or_employer_id";
      break;

   case "Select from List":
      $level2 = "company_list";
      break;
}
$department_str = "";
if ($department_id != "0" && !in_array($department_id, $departments_in_group)){
	error("You do not have access to that department.");
	$level2 = "";
}elseif ($department_id == "0"){
	$department_str = "'" . implode("', '", $departments_in_group) . "'";
}else{
	$department_str = "'" . $department_id . "'";
}

if ($level2 == "company_name_or_employer_id" && !isValidEmployerID($employer_id, $department_str))
{
	if (!(isValidCompanyName($company_name, $department_str)))
	{
		/*
		 The company name and department that they have typed in didn't work, so lets
		 do a search for similar companies and see what we get.
		*/

		if (trim($company_name))
		{ 
			$sql = ("
				SELECT c.company_name
				FROM employer_company AS c, job_info AS j
				WHERE c.employer_id=j.employer_id
				AND j.department_id IN (" . $department_str . ")
				AND c.company_name LIKE '" . $company_name . "%'
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
			notify("No interviews for the selected company and department.");
			$level2 = "";
		}
	
	}
}

switch ($level2)
{
   case "company_list":
      include("interview/view/main/view_companies.inc");
      break;

   case "company_name_or_employer_id":
      // $employer_id and/or $company_name variables are already set.  The included file can use them.
      // Use $job_id if it is present, else use $job_code
      include("interview/view/main/view_interviews_by_company.inc");
      break;

   case "close_match":
      include("interview/view/main/company_choose_list.inc");
      break;

   default:
      include("interview/view/list/view_by_company_main.inc");
      break;
}

?>

