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
 | Filename: edit_contact.inc                                                   |
 +------------------------------------------------------------------------------+
 | Description: This file decides whether to load up the contact and employer   |
 | information form, or to save the inputs gathered from that form.  It also    |
 | validates any of the inputs.                                                 |
 +------------------------------------------------------------------------------+

*/

if ($continue == '')
{
	// No form has been submitted yet, so give them one to fill out.
	
	if (!isset($enable_save))
        {
                $enable_save = 'true';
        }
	
	if ($javasubmit)
        {
		$job->employer_id = $new_employer_id;
		$job->department_id = $new_department_id;
		$job->contact_id = $new_contact_id;
        }
	include('job_descriptions/edit_job/employer/edit_contact_form.inc');
}

if ($continue == 'Save Changes')
{
	$job = new EditJob('');
	$job->employer_id = $new_employer_id;
	$job->department_id = $new_department_id;
	$job->contact_id = $new_contact_id;
	$job->job_id = $job_id;
	
	if ($error == 0)
	{
		include('job_descriptions/edit_job/employer/update_contact.inc');
		$job = new EditJob('');
                $job->loadValues($job_id);

                $no_buttons = true;
                include('job_descriptions/add_job/confirm_inputs.inc');
	}
}

?>
