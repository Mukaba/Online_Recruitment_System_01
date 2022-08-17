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
 | Filename: add_job.inc                                                        |
 +------------------------------------------------------------------------------+
 | Description: *  interview information is moved from the  object              |
 | "interview" to the Mamook[tm] database. * * Modified by Robin, 2000-09-25    |
 | 11:22                                                                        |
 +------------------------------------------------------------------------------+

*/

// get data from object "interview" and set variables

assert( isset($auth) );

$job_code = $interview->job_code;

$current_time = date("Y-m-d H:i:s");

//check to ensure no other interviews with the given job code are scheduled for any time greater than today

$next_time = interviewDatetime($job_code);

if ($next_time >= $current_time && $next_time != ""){

		// Build Department ID string..

		$department_arr = department_groups($auth->department);

		for ($i = 0; $i < sizeof($department_arr); $i++)
		{
			$dept_string .= ("'" . $department_arr[$i] . "',");
		}

		$dept_string = substr($dept_string, 0, -1);

                if (strlen($dept_string) < 2) { $dept_string = "''"; }

    /*
     This query would be used if everyone was using only interviews (so no sequential numbering).
	$sql = ("
        SELECT i.job_id
		FROM interview_time as i, job_info as j
		WHERE j.department_id IN (".$dept_string.") 
		AND i.job_id=j.job_id 
		AND int_date='" . addslashes($next_time) . "' 
		AND job_code='" . addslashes($job_code) . "'
        ");
    */
    // Otherwise, do below query
    
    // Get the date of the start of this term.  No duplicate job codes are allowed from that date onwards.
    $current_term = getCurrentTermID();
    $current_year = getCurrentYear();
    $sql = ("
        SELECT DISTINCT start_date
        FROM term
        WHERE term_id='" . $current_term . "'
        ");
    $result = $GLOBALS['dbh']->Execute($sql);
    $row = $result->FetchRow();
    $term_start_month = $row["start_date"];
    $term_start_date = date("Y-m-d", mktime(0, 0, 0, $term_start_month, 1, $current_year));

    $sql = ("
        SELECT DISTINCT it.job_id
        FROM interview_time AS it, job_info AS ji
        WHERE ji.job_id=it.job_id
        AND ji.job_code='" . addslashes($job_code) . "'
        AND it.int_date>='" . $term_start_date . "'
        ");
	$result = $GLOBALS['dbh']->Execute($sql);

	if ($result->RecordCount()) 
    {
        $error = 1;
		$row = $result->FetchRow();
		$job_id = $row["job_id"];

		$msg = ("Interview <b class='instruct'>" . $interview->job_code . "</b> has already been scheduled for ");

		$result = $GLOBALS['dbh']->Execute("
            SELECT DISTINCT int_date 
            FROM interview_time 
            WHERE job_id='" . $job_id . "'
            ");
		while(($row = $result->FetchRow()))
		{
			$msg .= ("<b class='instruct'>" . $row[int_date] . "</b>&nbsp;");
		}

        $msg .= ("<br /><br />If this interview is part of a multipart interview, it is suggested that you create a new interview using the same job code, but append");
        $msg .= (" <b>-#</b> to the end of it, where <b>#</b> is the number of the part of the interview.  Eg.  If this is part 2 of an interview for");
        $msg .= (" job code 2002F-00001, then use 2002F-00001-2 as the job code for this interview.");
	
		$msg .= ("<br /><br />Would you like to edit it?<br /><br />");
		promptEdit($msg, $job_id);
	}	
	
}

if ($error != 1) 
{ 
	$students = explode(chr(10), $interview->students);
	
	$interviewers = $interview->interviewers;
	
	$locations = $interview->locations;
	$wait_locations = $interview->wait_locations;
	$int_date = $interview->int_date;
	
	//insert data into database
	
	/*** 		check if there are enough time slots  		***/
	$result =isSlotsEnough(	$interview->schedules, $interview->students, ((isSwitching($interview->interview_type) || isSimultaneous($interview->interview_type)) ? true : false ));
	if( $result > 0 )
		error("Warning: More interview time slots than short-listed students.<br />
			   ");		
	elseif( $result < 0 )
		error("Warning: More short-listed students than scheduled time slots.<br />
		       ");		
	
	// insert employer's info into table "employer_company" if not exist, unless we already have an employer id, in which case do nothing.
	if (!$interview->employer_id)
	{
		$result10 = $GLOBALS['dbh']->Execute("SELECT employer_id FROM employer_company WHERE company_name='" . addslashes( $interview->company_name ) . "'");
		if( ($row10 = $result10->FetchRow()) )
		{
		 	$employer_id = $row10["employer_id"];
		}
		else
		{
			$GLOBALS['dbh']->Execute("INSERT INTO employer_company ( company_name ) VALUES ('" . addslashes( $interview->company_name ) . "')");
			$employer_id = $GLOBALS['dbh']->Insert_ID();
		}
	}
	else
	{
		$employer_id = $interview->employer_id;
	}
			
	/***		general interview info		***/
	
	//job  information into job_info table;
	
	//find the term - need month of last interview date 
	list($year, $month, $day) = explode("-", $int_date[0]);
	$term_id = getTerm($month);
	
	if ($interview->existing_single_job)
	{
		$sql = ("
			UPDATE job_info
			SET job_code='" . addslashes($interview->job_code) . "',
			employer_id='" . addslashes($employer_id) . "',
			department_id='" . $auth->department . "',
			interview_term_id='" . $term_id . "',
			interview_year='" . $year . "'
			WHERE job_id='" . $interview->job_id . "'
			");
		$result = $GLOBALS['dbh']->Execute($sql);
		$job_id = $interview->job_id;
	}
	else
	{
        $temp_date_entered = getTempDateEntered($term_id, $year);

		$sql = ("
			INSERT INTO job_info
			(job_code, employer_id, department_id, interview_term_id, interview_year, temp_date_entered) 
			VALUES (
			'" . addslashes( $interview->job_code) . "', 
			'" . $employer_id . "', 
			'" . $auth->department . "', 
			'" . $term_id . "', 
			'" . $year . "',
            '" . $temp_date_entered . "'
			)
			");
		$GLOBALS['dbh']->Execute($sql);
		$job_id = $GLOBALS['dbh']->Insert_ID();
	}

	// Now put the proper job ids in the interview_job_join table.

	if ($interview->multi_job_ids)
	{
		$multi_job_ids = explode(",", $interview->multi_job_ids);
		foreach($multi_job_ids AS $mji)
		{
			$sql = ("
				SELECT DISTINCT term_id, year, job_code
				FROM job_info
				WHERE job_id='" . $mji . "'
				");
			$term_year_result = $GLOBALS['dbh']->Execute($sql);
			$term_year_row = $term_year_result->FetchRow();

			$sql = ("
				INSERT INTO interview_job_join
				(interview_job_id, job_id, term_id, year, job_code)
				VALUES (
				'" . $job_id . "',
				'" . $mji . "',
				'" . $term_year_row["term_id"] . "',
				'" . $term_year_row["year"] . "',
				'" . $term_year_row["job_code"] . "'
				)
				");
			$result = $GLOBALS['dbh']->Execute($sql);
		}
	}
	
	$sql = "
		INSERT INTO contact(email, type)
		VALUES ('" . addslashes( $interview->employer_email ) . "', 3)
		";
	$GLOBALS['dbh']->Execute($sql);
	$contact_id = $GLOBALS['dbh']->Insert_ID();

	$sql = "
		INSERT INTO contact_job(job_id, contact_id)
		VALUES ('" . addslashes($job_id) . "', '" . addslashes($contact_id) . "')
		";
	$GLOBALS['dbh']->Execute($sql);
	
	
	if( $post )
	{
		$sql = ("
			INSERT INTO interview
			(job_id, notes, staff_notes, interview_medium_id, presentation, creator, 
			interview_type_id, date_created, contact, interview_place_id, student_visible)
			VALUES (
			'" . $job_id . "', 
			'" . addslashes( $interview->notes ) . "', 
			'" . addslashes( $interview->staff_notes ) . "',  
			'$interview->interview_medium', 
			'$interview->presentation', 
			'" . addslashes( $auth->login ) . "', 
			'$interview->interview_type', 
			'$current_time', 
			'$interview->contact', 
			'$interview->interview_place', 
			1)
			");
		$result = $GLOBALS['dbh']->Execute($sql);
		

		// We need to set this jobs student_status to 'Students Shortlisted'

		if ($interview->multi_job_interview)
		{
			$multi_job_ids = explode(",", $interview->multi_job_ids);
			foreach ($multi_job_ids AS $mji)
			{
				$sql = ("
					UPDATE job_info
					SET student_status='3'
					WHERE job_id='" . $mji . "'
					");
				$result = $GLOBALS['dbh']->Execute($sql);
			}
		}
		else
		{
			$sql = ("
				UPDATE job_info
				SET student_status='3'
				WHERE job_id='" . $interview->job_id . "'
				");
			$result = $GLOBALS['dbh']->Execute($sql);
		}
	}	
	else
	{
		$sql = ("
			INSERT INTO interview
			(job_id, notes, staff_notes, interview_medium_id, presentation, creator, 
			interview_type_id, date_created, contact, interview_place_id)
			VALUES (
			'" . $job_id . "', 
			'" . addslashes( $interview->notes ) . "', 
			'" . addslashes( $interview->staff_notes ) . "',  
			'$interview->interview_medium', 
			'$interview->presentation', 
			'" . addslashes( $auth->login ) . "', 
			'$interview->interview_type', 
			'$current_time', 
			'$interview->contact', 
			'$interview->interview_place')
			");
		$result = $GLOBALS['dbh']->Execute($sql);

	}

    //store these variables to set an automated contact action 'interview conducted' 
    $auto_action_medium = $interview->interview_medium;

    //switch these mediums to their respective action_methods
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

	if ($interview->multi_job_interview)
	{
		$multi_job_ids = explode(",", $interview->multi_job_ids);
		foreach ($multi_job_ids AS $mji)
		{
			$sql = ("
				UPDATE job_info
				SET admin_status='16'
				WHERE job_id='" . $mji . "'
				");
			$result = $GLOBALS['dbh']->Execute($sql);
		}
	}
	else
	{
		$sql = ("
			UPDATE job_info
			SET admin_status='16'
			WHERE job_id='" . $interview->job_id . "'
			");
		$result = $GLOBALS['dbh']->Execute($sql);
	}
	
	// Stephen's modification 2000-09-06 11:37
	// new code below
	
	/***	update presentation info	***/
	if ($interview->presentation == 1)
	{
		$pres_time = $interview->pres_time . ":00";
		$GLOBALS['dbh']->Execute("INSERT INTO presentation(job_id, pres_date, pres_time, end_time, pres_loc, pres_notes, viewable_by_all) VALUES ('$job_id', '$interview->pres_date', '$pres_time', '" . add_times( $interview->pres_time, $interview->pres_len ) . "', '" . addslashes( $interview->pres_loc ) . "', '" . addslashes( $interview->pres_notes ) . "', '" . ($interview->pres_viewable_by_all) . "')");
	}
	
	/***	 insert interviewer's info into table "interview_interviewer"		***/
	for ($i = 0; $i < count($interviewers); $i++)
	{
		
		$sql = "INSERT INTO interview_interviewer(interviewer_name) VALUES ('" . addslashes( $interviewers[$i] ) . "')";
		$GLOBALS['dbh']->Execute($sql);
		$interviewer_id[$i] = $GLOBALS['dbh']->Insert_ID();
		
	}

    // save all interviewers for automated 'interview conducted' action
    $interviewer_list = implode($interviewer_id, ",");
	
	// add interview time slots
	// the times need to be added in slightly different ways if there are two interviewers interviewing 
	// simultaneously or switching, or if we have sequential interviews.
	
	$isswitching = isSwitching($interview->interview_type);
	$issimultaneous = isSimultaneous($interview->interview_type);
	$issequential = isSequential($interview->interview_type);
	
	if (!($isswitching) && !($issimultaneous) && !($issequential))
	{
	   include('interview/add/add_times.inc');
	}
	elseif ($isswitching)
	{
	   include('interview/add/add_times_switch.inc');
	}
	elseif ($issimultaneous)
	{
	   include('interview/add/add_times_simultaneous.inc');
	}
	elseif ($issequential)
	{
	   include('interview/add/add_times_sequential.inc');
	}

    // set automated action 'interview conducted' for this interview
    // begin by analyzing job_id
    $sql = ("
        SELECT status 
        FROM job_info
        WHERE job_id='".$job_id."'
        ");

    $result = $GLOBALS['dbh']->Execute($sql);
    $row = $result->FetchRow();
    $status = $row[0];
    
    // get job_code to store in action_note of contact_actions_set 
    $sql = ("
        SELECT job_code
        FROM job_info
        WHERE job_id = '".$job_id."'
        ");
    $result = $GLOBALS['dbh']->Execute($sql);
    $row = $result->FetchRow();
    $job_code = $row[0];

    // check that job id does not have interview only status(job id created to record an interview for multiple jobs)
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

    // go through the job_ids to get the contact_id of the employer to set an action for
    // we could be setting actions for different contacts because of the INT_ONLY job_ids
    foreach($arr_job_id as $aji)
    {
        $sql = ("
            SELECT contact_id
            FROM job_info 
            WHERE job_id = '".$aji."'
            ");
        $result = $GLOBALS['dbh']->Execute($sql);

        $row = $result->FetchRow();
        $employer_contact_id = $row[0];

        // get interview date
        $sql = ("
            SELECT DISTINCT int_date
            FROM interview_time
            WHERE job_id='".$job_id."'
            ");
        $result = $GLOBALS['dbh']->Execute($sql);
        $row = $result->FetchRow();
        $date = $row[0];

        //set automated 'Interview Conducted' Action for the employer of this job
        if(!($employer_contact_id==NULL || $employer_contact_id == '0'))
        {
            // check that we haven't set this action before (dont want duplicates)
            $check_sql = ("
                SELECT *
                FROM contact_actions_set
                WHERE action_id = '".INT_CONDUCTED."'
                AND contact_id = '".$employer_contact_id."'
                AND action_on = '".$date."'
                AND action_by = '".$interviewer_list."'
                AND action_method_id = '".$auto_action_medium."'
                AND action_note = '".$note."'
                ");
            $check_result = $GLOBALS['dbh']->Execute($check_sql);

            if($check_result->RecordCount() == '0')
            {
                $insert_sql = ("
                    INSERT INTO contact_actions_set
                    (
                        contact_id,
                        action_id,
                        action_on,
                        action_by,
                        action_method_id,
                        action_note
                    )
                    VALUES
                    (
                        '".$employer_contact_id."',
                        '".INT_CONDUCTED."',
                        '".$date."',
                        '".$interviewer_list."',
                        '".$auto_action_medium."',
                        '".$note."'
                    )
                    ");
                $insert_result = $GLOBALS['dbh']->Execute($insert_sql);
            }
        }
    }

	if( !(isOnCampus($interview->interview_place)) && !(isPhone($interview->interview_medium)))
	{
		$sql = "INSERT INTO interview_location( job_id, street_address_1, street_address_2, city, province, country, directions )
			VALUES ('$job_id', '" . addslashes($interview->address1) . "','" . addslashes($interview->address2) . "',
			'" . addslashes($interview->city) . "',
			'" . addslashes($interview->province) . "',
			'" . addslashes($interview->country) . "',
			'" . addslashes($interview->direction) . "'
			)";
		$GLOBALS['dbh']->Execute($sql);
	}
	
	assert( empty( $duplicate_students ) );
	
	/**** add the short listed students' student number into the students_shortlisted table ****/
	
	for($i = 0; $i < count($students); $i++)
	{
		  $studentNum = findStudentNumbers($students[$i]);
	
	      if( count($studentNum) > 1 )
				$duplicate_students[] = $studentNum;	
	      elseif( count($studentNum) == 1 )
				$sid_array[] = $studentNum[0];
	      else
	         	error("<b>Warning: $students[$i]</b> doesn't exist in the database.  The student must be added to the database before being added to an interview shortlist.<br />" );
		
		  unset($studentNum);
	}

        $sql_ = "update interview set use_room_phone_number=\"".$interview->use_room_number."\" where job_id=\"$job_id\"";	
	$GLOBALS['dbh']->Execute ($sql_);

	update_duplicate("add");
	
	//delete object "interview"
	unset($interview);
	unset($interviewer_id);
}
?>
