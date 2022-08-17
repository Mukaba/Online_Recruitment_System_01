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
 | Filename: update_jobinfo.inc                                                 |
 +------------------------------------------------------------------------------+
 | Description: This file is called by edit_jobinfo.inc when the user opts to   |
 | save the changes he has made.                                                |
 +------------------------------------------------------------------------------+

*/


$error = 0;

/*******					Error checking					*******/

//check to ensure no other interviews with the given job code are scheduled for any time greater than today

$next_time = interviewDate($job_code);
if ($next_time >= $current_time && $next_time != ""){

                // Build Department ID string..

		$department_arr = department_groups($auth->department);

		for ($i = 0; $i < sizeof($department_arr); $i++)
		{
			$dept_string .= ("'" . $department_arr[$i] . "',");
		}

		$dept_string = substr($dept_string, 0, -1);

                if (strlen($dept_string) < 2) { $dept_string = "''"; }

        $sql = "SELECT i.job_id
                FROM interview_time as i, job_info as j
                WHERE j.department_id IN (".$dept_string.") 
		AND int_date = '" . addslashes($next_time) . "' 
		AND i.job_id = j.job_id 
		AND j.job_id != '$interview->job_id' 
		AND job_code = '" . addslashes($job_code) . "'";
	$result = $GLOBALS['dbh']->Execute( $sql );

        if ($result->RecordCount()) 
	{
                
		$row = $result->FetchRow();
                $job_id = $row["job_id"];

        	$error = 1;
                $error_message = "Job code <b>$job_code</b> already exists for selected term.";

		$get_jobcode = $GLOBALS['dbh']->Execute("select job_code from job_info where job_id = '$interview->job_id'");
		if ($get_jobcode AND $get_jobcode->RecordCount()) {
			$row = $get_jobcode->FetchRow();
			$interview->job_code = $row["job_code"];
			$job_code = $row["job_code"];
        	}
	}
}


if ( ($error == 0) && (trim($interview->job_code) == '') )
{
   $error = 1;
   $error_message = "You must enter a job code";
}
if ( ($error == 0) && (trim($interview->company_name) == '') )
{
   $error = 1;
   $error_message = "You must enter a company name";
}
if ($error == 0)
{
	for ($i = 0; $i < sizeof($interview->interviewers); $i++)
	{
		if (trim($interview->interviewers[$i]) == '')
		{
			$error = 1;
			$error_message = "Interviewer's names may not be left blank.";
			break;
		}
	}
}

/*
 Split the e-mail employer field so we can check if it contains valid e-mail address.
*/

$address_list = preg_split("/\s*,\s*/", $employer_email);
for ($i = 0; $i < sizeof($address_list); $i++)
{
	if (!ereg('^[-!#$%&\'*+\\./0-9=?A-Z^_`a-z{|}~]+'.'@'.'[-!#$%&\'*+\\/0-9=?A-Z^_`a-z{|}~]+\.'.'[-!#$%&\'*+\\./0-9=?A-Z^_`a-z{|}~]+$',( trim($address_list[$i]))))
	{
		$bad_address_list = true;
	}
}
if($bad_address_list && $employer_email != '')
{
	$error = 1;
	$error_message = "You must either enter valid employer e-mail addresses or leave the field blank.";
}

/*** verify presentation info format ***/
if ( $error == 0 && $interview->presentation == 1)
{
	if( validDate($interview->pres_date) == -1 ) { $error = 1; $error_message = "Invalid presentation date."; }
	elseif( ($interview->pres_time = validTime($interview->pres_time)) == -1 ) { $interview->pres_time = ''; $error = 1; $error_message = "Invalid presentation time."; }
	elseif( ($interview->pres_len = intval($interview->pres_len) ) <= 0 )  { $error = 1; $error_message = "Invalid presentation length."; }
	elseif( trim($interview->pres_loc) == '' ) { $error = 1; $error_message = "Invalid presentation location."; }
	
}

/*******						End of validation 								*******/

/*******					update interview information in database		  	*******/

if ($error == 0)
{
	assert( $interview->job_id != '');
	
	$sql = "SELECT GET_LOCK('$interview->job_id', 10)";
	$result = $GLOBALS['dbh']->Execute($sql);
	$row =$result->FetchRow();
	if ($row[0] != 1){
		$error = 1;
		$error_message = "Unable to get record lock.  Please try again.";
	}else{


		$error_message = "<b class='black'>WARNING:&nbsp;&nbsp;</b>";
		$sql = "SELECT MAX(date_edited) as last_modified FROM interview_edit WHERE job_id = '" . $interview->job_id . "' AND menu_edited='Button'";
		$result = $GLOBALS['dbh']->Execute($sql);
		$row = $result->FetchRow();
	
		if ($row["last_modified"] > $interview->last_modified){
			$error = 1;
			$row = $GLOBALS['dbh']->GetRow("
				SELECT CONCAT(first_name, ' ', last_name) AS name
				FROM contact AS c, contact_internal AS i, interview_edit AS e
				WHERE c.contact_id = i.contact_id 
				AND i.login_id = e.editor 
				AND e.date_edited='" . $row["last_modified"] . "' 
				AND e.job_id = '" . $interview->job_id . "' 
				AND menu_edited = 'Button'
				");
			if ($row["name"] != ""){
				$error_message .= $row["name"] . " has modified this interview since you last loaded the page.  They have made the following changes:";
			}else{
				$error_message .= "An unknown person has modified this interview since you last loaded the page.  They have made the following changes:";
			}
			$sql = ("
				SELECT emailed_newsgroup, cancelled, student_visible
				FROM interview 
				WHERE job_id = '" . $interview->job_id . "'
				");
			$result = $GLOBALS['dbh']->Execute($sql);
			$row = $result->FetchRow();
		
			if ($row["emailed_newsgroup"] != $interview->emailed_newsgroup && $row["emailed_newsgroup"] != 0){
				$error_message .= "<br />-They have sent an e-mail to the shortlisted students.";
			}
			if ($row["cancelled"] != $interview->cancelled){
				$error_message .= "<br />-They have cancelled the interview.";
				$end_message .= " and <b>uncancel the interview</b>";
			}
			if ($row["student_visible"] != $interview->student_visible){
				if ($row["student_visible"] == 1){
					$error_message .= "<br />-They have posted the interview.";
						$end_message .= " and leave the interview posted";
				}else{
					$error_message .= "<br />-They have unposted the interview.";
					$end_message .= " and leave the interview unposted";
				}
			}
		}

		$sql = "SELECT MAX(date_edited) as last_modified FROM interview_edit WHERE job_id = '" . $interview->job_id . "' AND menu_edited='Job Info'";
		$result = $GLOBALS['dbh']->Execute($sql);
		$row = $result->FetchRow();
		if ($row["last_modified"] > $interview->last_modified){
			if (!$error){	
				$row = $GLOBALS['dbh']->GetRow("SELECT CONCAT(first_name, ' ', last_name) as name
						FROM contact as c, contact_internal as i, interview_edit as e
						WHERE c.contact_id = i.contact_id AND i.login_id = e.editor AND
								e.date_edited='" . $row["last_modified"] . "' AND
								e.job_id = '" . $interview->job_id . "' AND menu_edited = 'Job Info'");
				if ($row["name"] != ""){
					$error_message .= $row["name"] . " has modified this interview since you last loaded the page.  They have made the following changes:";
				}else{
					$error_message .= "An unknown person has modified this interview since you last loaded the page.  They have made the following changes:";
				}
			}
			$error = 1;
			$error_message .= "<br />-They have modified data.";
		}
		if ($error==1){
			$error_message .= "<br /><br />The changes you made have NOT been saved, but are shown in bold below the data which can be modified.  Once you have modified the information
						as required pressing the <b> \"Save Changes\" </b> button at the bottom of the page will update the data" . $end_message . ".";
		
			$interview->last_modified = $row["last_modified"];
			$job_id = $interview->job_id;
			$interview2 = $interview;
			$interview = new EditInterview('');
			$interview->loadValues($job_id, "Job Info");
		}
	}
}

if ($error == 0 )
{
	assert( $interview->job_id != '' );
	
	$job_id = $interview->job_id;
	
	$int_dates = $interview->int_dates;
	$interviewers = $interview->interviewers;
	$interviewer_ids = $interview->interviewer_ids;
    	$locations = $interview->locations;
    	$wait_locations = $interview->wait_locations;

	/*** 		check if there are enough slots  		***/
	$result =isSlotsEnough(	$interview->schedules, $interview->students, ((isSwitching($interview->interview_type) || isSimultaneous($interview->interview_type) )? true : false ));
	if( $result > 0 )
		error("Warning: More interview time slots than short-listed students.<br />
		       ");		
	elseif( $result < 0 )
		error("Warning: More short-listed students than scheduled time slots.<br />
		       ");		
	
	/*** 		update company info			 	***/
	//$GLOBALS['dbh']->Execute("UPDATE employer_company SET company_name='" . addslashes( $interview->company_name ) . "' WHERE employer_id='$interview->employer_id'");


	/***		update employer e-mail address			***/

	$result = $GLOBALS['dbh']->Execute("
		SELECT contact_id
		FROM contact_job
		WHERE job_id='" . addslashes($job_id) . "'
		");

	$row = $result->FetchRow();
	if (!$row["contact_id"])
	{
		$sql = ("
                	INSERT INTO contact(email, type)
               		VALUES ('" . addslashes( $interview->employer_email ) . "', 3)
                	");
	        $GLOBALS['dbh']->Execute($sql);
		$contact_id = $GLOBALS['dbh']->Insert_ID();
        
	        $sql = (" 
	                INSERT INTO contact_job(job_id, contact_id)
	                VALUES ('" . addslashes($job_id) . "', '" . addslashes($contact_id) . "')
        	        ");
        	$GLOBALS['dbh']->Execute($sql);
	}
	else
	{
		$result->Close();
		$GLOBALS['dbh']->Execute("
			UPDATE contact
			SET email='" . addslashes( $interview->employer_email ) . "'
			WHERE contact_id='" . addslashes($row["contact_id"]) . "'
			");
	}

	/***		update interview table info 		***/

	  //date_updated is a timestamp, so assign it a null value in order to set it to current time
	
	
	  $GLOBALS['dbh']->Execute("UPDATE interview SET notes='" . addslashes( $interview->notes ) . "', 
			staff_notes='" . addslashes( $interview->staff_notes ) . "',
	  		interview_medium_id='$interview->interview_medium', 
			interview_type_id = '$interview->interview_type',
			presentation='$interview->presentation', 
			contact='$interview->contact', 
			cancelled=0, interview_place_id='$interview->interview_place' WHERE job_id='$job_id'");
	  $GLOBALS['dbh']->Execute("UPDATE job_info SET job_code= '" . addslashes( $interview->job_code ) . "' WHERE job_id = '$job_id'");
	  
	
	
	/*
	If changed from phone to other medium we want to delete phone numbers
	*/
	if (!isPhone($interview->interview_medium) && isPhone($old_interview_medium)){
		$result = $GLOBALS['dbh']->Execute("SELECT time_id FROM interview_time WHERE job_id = '" . addslashes( $interview->job_id ) . "'");
		while ($row = $result->FetchRow()){
			$GLOBALS['dbh']->Execute("DELETE from interview_phone WHERE time_id = '" . $row["time_id"] . "'");
		}
	}
	
	
	/***		update presentation info 		***/
	 
	if ($interview->presentation == 1)
	{
		$pres_time = add_times($interview->pres_time, 0);
       		$sql = ("REPLACE INTO presentation (job_id, pres_date, pres_time, end_time, pres_loc, pres_notes, viewable_by_all)
				VALUES('$job_id', '$interview->pres_date', '$pres_time', 
					'" . add_times( $interview->pres_time, $interview->pres_len) . "', 
				        '" . addslashes( $interview->pres_loc ) . "',
					'" . addslashes( $interview->pres_notes ) . "',
					'" . $interview->pres_viewable_by_all . "')");
		$result1 = $GLOBALS['dbh']->Execute($sql);
      	}else{
		$result1 = $GLOBALS['dbh']->Execute("DELETE FROM presentation WHERE job_id = '$job_id'");
	}

	/***	  update interviewer's name		***/
	for ($i = 0; $i < count( $interview->interviewers); $i++)
	{
		$result3a = $GLOBALS['dbh']->Execute( " UPDATE interview_interviewer
       					SET interviewer_name='" . addslashes( $interviewers[$i] ) . "'
      					WHERE interviewer_id='$interviewer_ids[$i]'");
	}
    $interviewer_list = implode($interview->interviewers, ",");

	$GLOBALS['dbh']->Execute("REPLACE INTO interview_edit(job_id, date_edited, editor, menu_edited) VALUES('$job_id', NULL, '" . addslashes($auth->login) . "', 'Job Info')");
	$sql = "SELECT RELEASE_LOCK('$interview->job_id')";
	$result = $GLOBALS['dbh']->Execute($sql);

    //update automated action 'conducted interview' here
    //start by reviewing status of job_id
    $sql = ("
        SELECT status
        FROM job_info
        WHERE job_id = '".$job_id."'
        ");
    $result = $GLOBALS['dbh']->Execute($sql);
    $row = $result->FetchRow();
    $status = $row[0];

    //switch these mediums to their respective action_methods
    $auto_action_medium = $interview->interview_medium;
    switch($auto_action_medium)
    {
        case INT_IN_PERSON:
            $auto_action_medium = IN_PERSON;
            break;
        case INT_PHONE:
            $auto_action_medium = PHONE_METHOD;
            break;
        case VIDEO_CONF:
        case INT_OTHER:
            $auto_action_medium = OTHER_METHOD;
            break;
        default:
            assert(false);
            break;
    }

    $arr_job_id = array();
    // jobs can be INT_ONLY, meaning there is one interview held for more than 1 job. If this is the case, we must get 
    // the job_ids that this INT_ONLY job_id refers to in the interview_job_join table. Otherwise, just store the single job_id
    // need these job_ids to get the contacts to update for
    if($status == INT_ONLY)
    {
        // get job_code, which is placed in the note of the set contact action
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
        //single job_id case
        $arr_job_id[0] = $job_id;
        $note = ("Job Code: ".$job_code);
    }

    // go through the job_ids to get the contact_id of the employer that action was set for
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
        
        // check that we don't update the new info twice, or update unneccesarily (perhaps nothing was changed)
        $check_sql = ("
            SELECT * FROM contact_actions_set 
            WHERE contact_id = '".$employer_contact_id."'
            AND action_id = '".INT_CONDUCTED."'
            AND action_method_id = '".$auto_action_medium."'
            AND action_note LIKE '%".$job_code."%'
            ");
        $result = $GLOBALS['dbh']->Execute($check_sql);

        if(!($result->RecordCount())) 
        {
            if(!($employer_contact_id=='0' || $employer_contact_id==NULL))
            {
                $update_sql = ("
                    UPDATE contact_actions_set
                    SET action_method_id = '".$auto_action_medium."'
                    WHERE contact_id = '".$employer_contact_id."'
                    AND action_note LIKE '%".$job_code."%'
                    AND action_id= '".INT_CONDUCTED."'
                    ");
                $update_result = $GLOBALS['dbh']->Execute($update_sql);
            }
        }
    }
	include('interview/add/show_data.inc');
}
else
{
	$result = $GLOBALS['dbh']->Execute("SELECT RELEASE_LOCK('$interview->job_id')");
	error($error_message . "<br />");
	include('interview/edit/edit_jobinfo_form.inc');
}

?>
