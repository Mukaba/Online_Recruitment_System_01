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
 | Filename: remove_applicant.inc                                               |
 +------------------------------------------------------------------------------+
 | Description: This file is called when the user has specified that they       |
 | would like to remove a students application for a job.  One of two things    |
 | can happen here.  If the job's status is closed, the application is merely   |
 | marked as unavailable (so as not to confuse the employer).  If the job is    |
 | still posted, then the application is completely deleted.                    |
 +------------------------------------------------------------------------------+

*/

if (!$remove_confirmed)
{
	/*
	 They have not yet confirmed that they want to remove this applicant, so lets get confirmation.
	*/

	include('applications/admin/confirm_removal.inc');
}
else
{
	/*
	 They've confirmed that they would like to remove/make unavailable the student, so get rid of him.
	*/

	$sql = ("
		SELECT DISTINCT status
		FROM job_info
		WHERE job_id='" . $job_id . "'
		");
	$result = $GLOBALS['dbh']->Execute($sql);
	$row = $result->FetchRow();

	/*
	 If the job is closed, render the applications unavailable.  Otherwise delete it from the database.
	*/

	if ($row["status"] == CLOSED)
	{
		for ($i = 0; $i < sizeof($selected_app_id); $i++)
		{
			$sql = ("
				UPDATE applications
				SET app_status='".APP_UNAVAILABLE."'
				WHERE application_id = '" . $selected_app_id[$i] . "'
				");
			$result = $GLOBALS['dbh']->Execute($sql);
		}
	}
	else
	{
		for ($i = 0; $i < sizeof($selected_app_id); $i++)
		{
			$sql = ("
				UPDATE applications
				SET app_status = '".APP_ADMIN_DELETED."'
				WHERE application_id = '" . $selected_app_id[$i] . "'
				");
	
			$result = $GLOBALS['dbh']->Execute($sql);
		}
	}
	notify("Student applications have been successfully removed.");
	
	include('applications/admin/applicant_list.inc');
}

?>
