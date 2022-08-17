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
 | Filename: cancel_job.inc                                                     |
 +------------------------------------------------------------------------------+
 | Description: mark the interview as being cancelled in the database           |
 |                                                                              |
 |                                                                              |
 +------------------------------------------------------------------------------+

*/
$GLOBALS['dbh']->Execute("
	UPDATE interview 
	SET cancelled=1 
	WHERE job_id='$job_id'
	");

if ($associated_job_ids)
{
	// Update the admin statuses for the associated jobs.

	$associated_job_ids = urldecode($associated_job_ids);
	$sql = ("
		UPDATE job_info
		SET admin_status='" . $associated_job_status . "'
		WHERE job_id IN (" . $associated_job_ids . ")
		");
	$result = $GLOBALS['dbh']->Execute($sql);
}

$result = $GLOBALS['dbh']->Execute("
	SELECT a.job_code, b.emailed_newsgroup
	FROM job_info AS a, interview AS b
	WHERE a.job_id='$job_id'
	AND b.job_id=a.job_id
	");

$row = $result->FetchRow();
$job_code = $row["job_code"];
$emailed_newsgroup = $row["emailed_newsgroup"];

$GLOBALS['dbh']->Execute("
	REPLACE INTO interview_edit(job_id, date_edited, editor, menu_edited)
	VALUES ('$job_id', NULL, '" . addslashes($auth->login) . "', 'Button')
	");

notify($job_code." cancelled");

//delete the automated action 'interview conducted' for the needed contacts
$sql = ("
    SELECT status
    FROM job_info
    WHERE job_id = '".$job_id."'
    ");
$result = $GLOBALS['dbh']->Execute($sql);
$row = $result->FetchRow();
$status = $row[0];

$arr_job_id = array();
// jobs can be INT_ONLY, meaning there is one interview held for more than 1 job.
// thus we could be delete an action for more than one contact. If this is the case, we must get 
// the job_ids that this INT_ONLY job_id refers to in the interview_job_join table. Otherwise, just store the single job_id
if($status == INT_ONLY)
{
    $sql = ("
        SELECT job_code
        FROM job_info
        WHERE job_id = '".$job_id."'
        ");
    $result = $GLOBALS['dbh']->Execute($sql);
    $row = $result->FetchRow();
    $job_code = $row[0];

    $sql = ("
        SELECT job_id
        FROM interview_job_join
        WHERE interview_job_id = '".$job_id."'
        ");
    $result = $GLOBALS['dbh']->Execute($sql);
    $count = 0;
    if($result->RecordCount()==0)
    {
        //this job_id has been manually entered (for an oral presentation etc) so it is not linked to a posting
        // ultimately, it will not have a contact_id, but we'll check for one later anyway
        $arr_job_id[0] = $job_id;
    }
    else
    {
        while($row = $result->FetchRow())
        {
            $arr_job_id[$count++] = $row['job_id'];
        }
        $note = ("Interview for multiple job codes: ".$job_code);
    }
}
else
{
    $arr_job_id[0] = $job_id;
    $note = ("Job Code: ".$job_code);
}

// go through the job_ids to get the contact_id of the employer to delete an action for
foreach($arr_job_id as $aji)
{
    $sql = ("
        SELECT contact_id
        FROM job_info
        WHERE job_id = '".$aji."'
        ");
    $result = $GLOBALS['dbh']->Execute($sql);
    $row = $result->FetchRow();
    $employer_contact_id = $row['contact_id'];
        
    if(!($employer_contact_id == '0' || $employer_contact_id == NULL))
    {
        $delete_sql = ("
            DELETE FROM contact_actions_set 
            WHERE contact_id = '".$employer_contact_id."'
            AND action_id = '".INT_CONDUCTED."'
            AND action_note LIKE '%".$job_code."%'
            ");
        $GLOBALS['dbh']->Execute($delete_sql);
    }
}

/*
 If the shortlist for this interview has been e-mailed to the students,
 it is important that we send out an e-mail to them letting them know
 that this interview was cancelled.
*/

if ($emailed_newsgroup)
{	
	include("interview/edit/mail/send_cancel_interview_email.inc");
}

?>
