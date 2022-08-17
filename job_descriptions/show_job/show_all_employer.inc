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
 | Filename: show_all_employer.inc                                              |
 +------------------------------------------------------------------------------+
 | Description: This is the main file that directs the appropriate show page    |
 | to the EMPLOYER or HREMPLOYER.                                               |
 +------------------------------------------------------------------------------+

*/


include('job_descriptions/edit_job/EditJob.class');

$nextTermYear = getNextTermYear();

// The employer_id gotten below is used to determine which jobs should be shown to the employer.

if ($userlevel != ADMINEMPLOYER)
{
    $sql = ("
        SELECT DISTINCT employer_id
        FROM employer_login
        WHERE login_id='" . addslashes($auth->login) . "'
        ");
    $result = $GLOBALS['dbh']->Execute($sql);
    $row = $result->FetchRow();
    $employer_id = $row["employer_id"];
}

switch ($level1)
{
	case "job_id":
	if ($userlevel == EMPLOYER || $userlevel == HREMPLOYER || $userlevel == ADMINEMPLOYER)
	{
		include('job_descriptions/show_job/jobshow/showjob_employer.inc');
	} 
    else
	{
		include('misc/loguser.inc');
	}
	break;

	default:
	if ($userlevel == EMPLOYER || $userlevel == HREMPLOYER || $userlevel == ADMINEMPLOYER)
	{
		include("job_descriptions/show_job/employer/show_job_list.inc");
	} 
    else
	{
		include("misc/loguser.inc");
	}
	break;
}

?>
