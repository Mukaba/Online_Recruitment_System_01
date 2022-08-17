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
 | Filename: update_host_department.inc                                         |
 +------------------------------------------------------------------------------+
 | Description: This file is used to save the new host department for the       |
 | job.  After successfully saving the host department, we let the user know    |
 | all changes have been saved.                                                 |
 +------------------------------------------------------------------------------+

*/

$sql = ("
	UPDATE job_info
	SET department_id='" . addslashes($job->host_department) . "'
	WHERE job_id='" . $job_id . "'
	");
$GLOBALS['dbh']->Execute($sql);

$sql = ("
    SELECT contact_id, status
    FROM job_info
    WHERE job_id = '" . $job_id . "'
    ");
$result = $GLOBALS['dbh']->Execute($sql);
$row = $result->FetchRow();

$job_status = $row['status'];

// only jobs that have been posted, closed, or cancelled should have their contact statuses updated. 
// jobs that are pending and such should not. 
if ($job_status == POSTED || $job_status == CLOSED || $job_status == CANCELLED)
{
    // If the contact already has credit for posting this job for this department, we shouldn't do it again. 
    $existing_status_sql = ("
            SELECT department_id
            FROM department_contact_status
            WHERE contact_id = '".$row['contact_id']."' AND job_id = '".$job_id."' AND department_id = '".$job->host_department."'
            ");
    $existing_status_result = $GLOBALS['dbh']->Execute($existing_status_sql);
    
    // No contact status for this job and this department, so update that. 
    if (!$existing_status_result->RecordCount())
    {   
        updateContactStatus($row['contact_id'],$job->host_department,$job_id,null);
    }
}

?>

