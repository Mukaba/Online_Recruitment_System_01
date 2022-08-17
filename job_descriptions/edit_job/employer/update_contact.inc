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
 | Filename: update_contact.inc                                                 |
 +------------------------------------------------------------------------------+
 | Description: This file is used to transfer information from the job object   |
 | to the database.                                                             |
 +------------------------------------------------------------------------------+

*/

assert(isset($auth));

/*
 If we have the division set to none, then clear it's value.
*/

if ($job->department_id == 'none')
{
	$job->department_id = '';
}

/*
 Update the job with this new contact_id (or the same old one).
*/

$sql = ("
        UPDATE job_info
        SET
        contact_id='" . $job->contact_id . "',
	employer_id='" . $job->employer_id . "',
	employer_department_id='" . $job->department_id . "'
	WHERE job_id = '". $job->job_id ."'
        ");
$GLOBALS['dbh']->Execute($sql);

//echo("<h3>All changes for " . $job->job_code . " have been successfully saved.</h3>");
notify("All changes have been successfully saved.");
echo("<hr />");





//delete object "job"
unset($job);

?>

