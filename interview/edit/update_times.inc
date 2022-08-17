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
 | Filename: update_times.inc                                                   |
 +------------------------------------------------------------------------------+
 | Description: Update an interview's times when interview isn't simultaneous   |
 | or switching                                                                 |
 +------------------------------------------------------------------------------+

*/

$location = $locations[0];
$wait_location = $wait_locations[0];


foreach( $interview->schedules[$j] as $s )
{
	$start = $s['START'];
	$end = $s['END'];
	$type = $s['TYPE'];

	if( $interview->no_time && $type == INTERVIEW )
		$type = NO_TIME;

	if( count( $time_ids ) > 0 )	 	
	{
		$time_id = array_pop($time_ids);
		
		$sql = ("
            INSERT INTO interview_time
            (
                time_id,
                job_id, 
                int_time, 
                end_time, 
                int_type, 
                int_date, 
                interviewer_id, 
                int_room_id,
                int_wait_room_id
            )
      		VALUES 
            (
                '$time_id', 
                '$job_id', 
                '$start',
                '$end',
                '$type', 
                '$int_dates[$j]', 
                '$interviewer_ids[0]', 
                '$location', 
                '$wait_location'
            )
            ");
		$GLOBALS['dbh']->Execute($sql);
 	}
 	else
 	{
  		$sql = ("
            INSERT INTO interview_time
            (
                job_id,
                int_time, 
                end_time, 
                int_type,
                int_date, 
                interviewer_id, 
                int_room_id, 
                int_wait_room_id
            )
  			VALUES 
            (
                '$job_id', 
                '$start', 
                '$end', 
                '$type', 
                '$int_dates[$j]', 
                '$interviewer_ids[0]', 
                '$location', 
                '$wait_location'
            )
            ");
		$GLOBALS['dbh']->Execute($sql);
	} 	
}

// update contact_actions_set for automated INT CONDUCTED action
$sql = ("
    SELECT status
    FROM job_info
    WHERE job_id = '".$job_id."'
    ");
$result = $GLOBALS['dbh']->Execute($sql);
$row = $result->FetchRow();
$status = $row[0];

// get job_code to find in the note of the action
$sql = ("
    SELECT job_code
    FROM job_info
    WHERE job_id = '".$job_id."'
    ");
$result = $GLOBALS['dbh']->Execute($sql);
$row = $result->FetchRow();
$job_code = $row[0];

$arr_job_id = array(); 
// jobs can be INT_ONLY, meaning there is one interview held for more than 1 job. If this is the case, we must get 
// the job_ids that this INT_ONLY job_id refers to in the interview_job_join table. Otherwise, just store the single job_id
if($status == INT_ONLY)
{
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
    
// go through the job_ids to get the contact_id of the employer to update action for
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
    
    // memorize some info from the query (like  action_by and action_method_id))        
    $copy_sql = ("
        SELECT * 
        FROM contact_actions_set
        WHERE contact_id = '".$employer_contact_id."'
        AND action_id = '".INT_CONDUCTED."'
        AND action_note = '".$note."'
        ");
    $result = $GLOBALS['dbh']->Execute($copy_sql);
    $row = $result->FetchRow();
    
    // since all int_dates are stored in an array already (even when adding new int dates) we can delete all
    // contact_actions pertaining to this interview, then restore them with an insert clause (make it MUCH easier for
    // changing dates (especially when moving dates 1 day forward for two consecutive interview days)
    $delete_sql = ("
        DELETE FROM contact_actions_set
        WHERE contact_id = '".$employer_contact_id."'
        AND action_id = '".INT_CONDUCTED."'
        AND action_note = '".$note."'
            ");
    $GLOBALS['dbh']->Execute($delete_sql);

    //now just insert 'fresh and new' contact actions (they will have all the same info except their dates)  
    for($k = 0; $k < count($int_dates); $k++)
    {
        if(!($employer_contact_id =='0' || $employer_contact_id == NULL))
        {
            $insert_sql = ("
                INSERT INTO contact_actions_set
                (
                    contact_id,
                    action_on,
                    action_by,
                    action_method_id,
                    action_id,
                    action_note
                )
                VALUES
                (
                    '".$employer_contact_id."',
                    '".$int_dates[$k]."',
                    '".$row['action_by']."',
                    '".$row['action_method_id']."',
                    '".INT_CONDUCTED."',
                    '".$note."'
                )    
                ");
            $GLOBALS['dbh']->Execute($insert_sql);
        }
    }
}
?>
