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
 | Filename: edit_host_department.inc                                           |
 +------------------------------------------------------------------------------+
 | Description: This file is called when a member of the co-op staff would      |
 | like to edit the host department for a job.  Employers are not given access  |
 | to this part.                                                                |
 +------------------------------------------------------------------------------+

*/

if ($continue == '')
{
	// They have just arrived here, so load up the edit_host_department_form page.

	include('job_descriptions/edit_job/edit_host_department_form.inc');
}
elseif ($continue == 'Save Changes')
{
	// They've changed the host department.  Save it and return them to the job choice screen.

	$job = new EditJob('');

	$job->set_value_2($host_department);

	include('job_descriptions/edit_job/update_host_department.inc');

	include('job_descriptions/edit_job/mail_host_department.inc');

	$msg = ("The host department has been successfully saved.  If your department is no longer the host ");
	$msg .= ("department for this job, you will no longer be able to edit any of the job's attributes.  The");
	$msg .= (" new host department has been e-mailed to alert them that you have made them the new host department");
	$msg .= (" for this job.");
	notify($msg);
}

?>
