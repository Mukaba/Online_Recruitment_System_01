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
 | Filename: update_datetime.inc                                                |
 +------------------------------------------------------------------------------+
 | Description: This file is called by edit_datetime.inc after the user opts    |
 | to save his changes.  This will save all of the changes made, prompting the  |
 | user for confirmation in case of conflict, and then if a conflict was        |
 | confirmed, will set a flag so that e-mail will be sent to the student        |
 | afterwards.                                                                  |
 +------------------------------------------------------------------------------+

*/

/*******					Cancel Checked Days' Schedule  	 		*******/

global $current_date;
global $date_changed;
global $auth;

$error = 0;
if( isset($cancel_day) )
{
	$count = 0;
	
	if( count($interview->int_dates) <= count( $cancel_day ) )
	{
	   $error = 1;
	   $error_message = "You must have at least one interview date";
	}
	else
	{
		for( $i = 0; $i < count( $cancel_day ); $i++ )
		{
			$j = intval($cancel_day[$i]);

			remove_element($interview->int_dates, $j-$count);		//remove selected day's date, break( lunch ) info
			remove_element($interview->schedules, $j-$count);
		
			$interview->int_dates = restoreIndex( $interview->int_dates );
			$interview->schedules = restoreIndex( $interview->schedules );
			$count++;	
			
		}
	}
}
	
/*******					Error checking					*******/

//check date format
if ( $error == 0 && $date_changed )
{
	$date_in_future = 1;
	for ($i = 0; $i < count( $interview->int_dates ); $i++)
	{
		if(  ($temp_date = validDate($interview->int_dates[$i])) == -1 ) 
		{ 
			$error =1; 
			$error_message = (count($interview->int_dates) > 1)? "You must enter a valid interview date for day " . ($i+1) . "<br /><br />" : "You must enter a valid interview date<br /><br />"; 
			break;
		}
		elseif( ($temp_date < $current_date) && !($ignore[$i]) ) 
		{ 
			$date_in_future = 0; 
		}
		$interview->int_dates[$i] = $temp_date;
	}
	if( ($error == 0) && ( $date_in_future != 1 ) )
	{ 
		$error = 1; 
		$error_message = "You may not enter interview dates earlier than " . $current_date . "<br />"; 
	}
	if( ($error == 0) && isDuplicate( $interview->int_dates, $duplicate ) )
	{ 
		$error = 1;   
		$error_message = "Duplicate interview dates $duplicate<br /><br />"; 
	}
	if( ($error == 0) && ( $error_message = check_student_name($interview->students) ) )
	{ 
		$error = 1;
	}
}

/*******						End of validation 								*******/

$edit_conflict = 0;
$row = $GLOBALS['dbh']->GetRow("SELECT GET_LOCK('$interview->job_id', 10)");
if ($row[0] != "1")
{
	$error = 1;
	$error_message = "Unable to get row lock. Please try again.";
}
else
{
	$row = $GLOBALS['dbh']->GetRow("SELECT MAX(date_edited) as last_mod FROM interview_edit WHERE job_id='$interview->job_id' AND (menu_edited = 'Date/Time' OR menu_edited = 'Button')");
	if($row["last_mod"] > $interview->last_modified)
    {
		$row = $GLOBALS['dbh']->GetRow("SELECT CONCAT(c.first_name, ' ', c.last_name) as name
					FROM contact as c, contact_internal as i, interview_edit as e
					WHERE c.contact_id = i.contact_id AND i.login_id = e.editor AND e.job_id = '$interview->job_id' AND menu_edited = 'Date/Time'
					AND date_edited = '" . $row["last_mod"] . "'");


		$error_message = "<b class='black'>WARNING:&nbsp;&nbsp;</b>";
		if ($row["name"] == "")
        {
			$error_message .= "An Unknown person";
		}
        else
        {
			$error_message .= $row["name"];
		}
		$error_message .= " has edited the Date/Time of this interview since 
					you last loaded the Date/Time information from the database.  Your changes have NOT been saved, but are shown below the current schedule.";

		$error = 1;
		$edit_conflict = 1;
		$interview2 = $interview;
		$interview = new EditInterview('');
		$interview->loadValues($interview2->job_id, 'Date/Time');
		$GLOBALS['dbh']->GetRow("SELECT RELEASE_LOCK('$interview->job_id')");
	}
}
/*******					update interview information in database		  	*******/

if ($error == 0 )
{
	assert( $interview->job_id != '' );
	
	$job_id = $interview->job_id;
	$int_dates = $interview->int_dates;

	$interviewers = $interview->interviewers;
	$interviewer_ids = $interview->interviewer_ids;

	$locations = $interview->locations;
        $wait_locations = $interview->wait_locations;

	for ($i=0;$i<count($interviewer_ids);$i++)
    {
		if($interview->location_names[$i] == 'Many')
        {
			notify("WARNING:  Multiple rooms have been specified for this interview and may have been modified by saving the changes.  After you have made the changes to the Date/Time you should ensure the rooms and wait rooms are specified correctly.");
			break;
		}
	}
	/*** 		check if there are enough slots  		***/
	$result =isSlotsEnough(	$interview->schedules, $interview->students, ((isSwitching($interview->interview_type) || isSimultaneous($interview->interview_type) )? true : false ));
	if( $result > 0 )
		error("Warning: More interview time slots than short-listed students.<br />");		
	elseif( $result < 0 )
		error("Warning: More short-listed students than scheduled time slots.<br />");		
	
	
	/***    	update daily interview schedule			***/

	//retrieve previous interview dates and store daily schedules

	/*
	 Get a list of all the phone numbers we will need to update, to do this,
	 first get a list of all the time id's associated with this job.
	*/

	if (isPhone($interview->interview_medium))
	{

		$phoneresult1 = $GLOBALS['dbh']->Execute("
			SELECT DISTINCT time_id
			FROM interview_time
			WHERE job_id='$job_id'
			");


		$time_id_in_str = "";
		while ($phonerow1 = $phoneresult1->FetchRow())
		{
			$timeids[] = $phonerow1["time_id"];
			$time_id_in_str .= "'" . $phonerow1["time_id"] . "',";
		}
		$time_id_in_str = substr($time_id_in_str, 0, strlen($time_id_in_str) - 1);

		$phoneresult2 = $GLOBALS['dbh']->Execute("
            SELECT p.time_id, p.phone, t.int_time, t.int_date, t.end_time, t.student_number, t.interviewer_id, t.job_id
		    FROM interview_phone as p, interview_time as t
			WHERE t.time_id IN (" . $time_id_in_str . ")
			AND phone IS NOT NULL AND t.time_id = p.time_id
            ");
        
		while ($row = $phoneresult2->FetchRow())
        {
			$phonearray[] = array(
				"time_id" => $phonerow2["time_id"],
				"phone" => $phonerow2["phone"]
			    );

			$phoneinfo[] = array (
				"time_id" => $row["time_id"],
				"phone" => $row["phone"],
				"int_time" => $row["int_time"],
				"int_date" => $row["int_date"],
				"end_time" => $row["end_time"],
				"student_number" => $row["student_number"],
				"interviewer_id" => $row["interviewer_id"],
				"job_id" => $row["job_id"]
				);
		}

		$phonedelete = $GLOBALS['dbh']->Execute("
            DELETE FROM interview_phone 
            WHERE time_id IN (" . $time_id_in_str . ")
            ");

		$phoneresult2->Close();
	
		/*
		 We have now deleted all of the previously entered phone numbers
		 and have a list of all the ones that we need to preserve.  This 
		 list of numbers is kept in phoneinfo[].
		*/

	} // if($phone)
		
	$result1a = $GLOBALS['dbh']->Execute("
		SELECT DISTINCT int_date 
		FROM interview_time 
		WHERE job_id='$job_id' 
		ORDER BY int_date
		");

	while( ($row1a = $result1a->FetchRow()) )
	{
		$old_date[] = $row1a['int_date'];
		$result1b[] = $GLOBALS['dbh']->Execute("
			SELECT * 
			FROM interview_time 
			WHERE job_id='$job_id' 
			AND int_date='$row1a[0]' 
			ORDER BY int_time, interviewer_id, sequential
            ");
	}

	/*
	 Before doing anything else, determine if we need to send out an e-mail to students
	 notifying them of this date changed.  Do this if the interview shortlist has been
	 e-mailed, and if a date has actually been changed.
	*/

	$sql = ("
		SELECT DISTINCT emailed_newsgroup
		FROM interview
		WHERE job_id='" . $job_id . "'
		");
	$result = $GLOBALS['dbh']->Execute($sql);
	$row = $result->FetchRow();

	$emailed_newsgroup = $row["emailed_newsgroup"];

	for ($i = 0; $i < sizeof($old_date); $i++)
	{
		if (!in_array($old_date[$i], $interview->int_dates))
		{
		
			if ($emailed_newsgroup)
			{
				$date_email = true;
			}
		}
	}
	
	assert( $job_id );

	/*
	 Here we are destroying all the previously entered time slots for
	 this interview, and then later on we will put back in all of the 
	 new time slots.
	*/

	$result1c = $GLOBALS['dbh']->Execute("
		DELETE FROM interview_time 
		WHERE job_id='$job_id'
		");
	
	/* 
	 Split daily schedules as how they are stored in database ( not including break or lunch ) 
	*/

	$schedules = $interview->splitSchedule();
	
	if (count($interview->int_dates) > count($old_date))
	{
		$limit = count($interview->int_dates);
	}
    else
	{
		$limit = count($old_date);
	}
	
	for($j = 0; $j < $limit; $j++)
	{
		if( $j < count($old_date) )
	   	{
			while($row = $result1b[$j]->FetchRow()) 
	   		{
	        	$time_ids[] = $row["time_id"];
				// Used for restoring phone #'s
	
	        	if( $row["int_type"] == INTERVIEW )
				{
					$old_rows[] = $row;
					$old_ints[] = $row;
				}
				if ( trim($row["student_number"]) != '' && $row["sequential"] == '0')
				{
					$students_signedup[] = array(
						"student_number" => $row["student_number"],
						"int_date" => $row["int_date"],
						"int_time" => $row["int_time"],
						"end_time" => $row["end_time"],
					);
				}
			}
		}
		
		if ($j < count($interview->int_dates) )
		{	
			if (!isSimultaneous($interview->interview_type) && !isSwitching($interview->interview_type) && !isSequential($interview->interview_type)) 
			{
				include('interview/edit/update_times.inc');
			}
			elseif (!isSimultaneous($interview->interview_type) && isSwitching($interview->interview_type) && !isSequential($interview->interview_type))
			{
				include('interview/edit/update_times_switching.inc');
			}
  			elseif (isSimultaneous($interview->interview_type) && !isSwitching($interview->interview_type) && !isSequential($interview->interview_type))
  			{
  	        		include('interview/edit/update_times_simultaneous.inc');
  	    		}
  	    		elseif (!isSimultaneous($interview->interview_type) && !isSwitching($interview->interview_type) && isSequential($interview->interview_type))
	     		{
				$stagger_amount = cal_length($interview->schedules[$j][0]["START"], $interview->schedules[$j][0]["END"]);
				$stagger_amount = intval($stagger_amount);
				include('interview/edit/update_times_sequential.inc');
	      		}
		}

		//find the term - need month of last interview date, then update the term id in the job_info table.

	        list($year, $month, $int_day) = explode("-", $interview->int_dates[0]);
	        $term_id = getTerm($month); 

		$sql = ("
			UPDATE job_info
			SET interview_term_id='" . addslashes($term_id) . "', interview_year='" . addslashes($year) . "'
			WHERE job_id='" . $job_id . "'
			");
		$GLOBALS['dbh']->Execute($sql);
  
  		// update the interview sign up to restore previously signed up students


		for( $i = 0; $i < count($old_ints); $i++ )
		{
   			if( $old_ints[$i]["student_number"] )
   			{
				$query = ("	
					UPDATE interview_time 
					SET student_number='" . $old_ints[$i]["student_number"] . "'
   					WHERE job_id='$job_id' 
					AND int_date='" . $old_ints[$i]["int_date"] . "'
   					AND int_time='" . $old_ints[$i]["int_time"] . "'
					AND end_time='" . $old_ints[$i]["end_time"] . "'
					AND interviewer_id='" . $old_ints[$i]["interviewer_id"] . "'
					AND int_type='1'
					AND sequential='" . $old_ints[$i]["sequential"] . "'
					");
				$GLOBALS['dbh']->Execute($query);
			
			}
			$query = ("
				UPDATE interview_time
				SET int_room_id = '" . $old_ints[$i]["int_room_id"] . "',
				int_wait_room_id = '" . $old_ints[$i]["int_wait_room_id"] . "'
				WHERE job_id = '$job_id'
				AND int_date='" . $old_ints[$i]["int_date"] . "'
				AND int_time='" . $old_ints[$i]["int_time"] . "'
				AND end_time='" . $old_ints[$i]["end_time"] . "'
				AND interviewer_id='" . $old_ints[$i]["interviewer_id"] . "'
				");
			$result = $GLOBALS['dbh']->Execute($query);
		}
		
		unset($time_array);
		unset($old_rows);
		unset($time_ids);
		unset($int_time);
		unset($old_time);
		unset($old_id);
		unset($old_interviewer_id);
	}	


	/*
	 We now need to figure out which students were dropped from interviews they
	 had signed up for.  Now find out what students are still signed up for
	 interviews, and use those to find out which students have been dropped from
	 interviews.  If there are any students that have been dropped, send e-mail
	 to those students.
	*/ 

	global $studentemail;
	unset($studentemail);

	$tempresult = $GLOBALS['dbh']->Execute("
			SELECT DISTINCT student_number, int_date, int_time, end_time 
			FROM interview_time
			WHERE job_id='$job_id' 
			AND student_number IS NOT NULL
			AND sequential='0'
			");

	while($temprow = $tempresult->FetchRow())
	{
		$students_still_in[] = array(
			"student_number" => $temprow["student_number"],
			"int_date" => $temprow["int_date"],
			"int_time" => $temprow["int_time"],
			"end_time" => $temprow["end_time"]
			);
	}

	for ($i = 0; $i < sizeof($students_signedup); $i++)
	{
		$dropped = true;
		for ($j = 0; $j < sizeof($students_still_in); $j++)
		{
			if (
			(trim($students_signedup[$i]["student_number"]) == trim($students_still_in[$j]["student_number"])) 
			&& (trim($students_signedup[$i]["int_date"]) == trim($students_still_in[$j]["int_date"])) 
			&& (trim($students_signedup[$i]["int_time"]) == trim($students_still_in[$j]["int_time"])) 
			&& (trim($students_signedup[$i]["end_time"]) == trim($students_still_in[$j]["end_time"]))
			)
			{
				$dropped = false;
				break;
			}
		}
		if ($dropped)
		{
			global $students_dropped;
			global $studentemail;
			if (!(is_array($students_dropped)) || !(in_array($students_signedup[$i]["student_number"], $students_dropped)))
			{
				$students_dropped[] = $students_signedup[$i]["student_number"];
			}
		}
	}



	for ($i = 0; $i < sizeof($students_dropped); $i++)
	{

		/* 
		 If student does not have an associated department in the database
		 then pull out his student number and name to notify the secretary
		 that they will need to manually enter an e-mail address.
		*/

		$noemail_result = $GLOBALS['dbh']->Execute("
			SELECT DISTINCT first_name, last_name, student_number
			FROM student
			WHERE student_number='" . trim( $students_dropped[$i]) . "'
			");
		$noemail_row = $noemail_result->FetchRow();

		if(getStudentEmail($students_dropped[$i]))
		{
			$studentemail[] = getStudentEmail($students_dropped[$i]);
		}
		else
		{
			$noemail_studentnum[] = $noemail_row["student_number"];
			$noemail_studentname[] = $noemail_row["first_name"] . " " . $noemail_row["last_name"];
		}
	
		/*
		 Code below is used to drop students that are part of switching
		 interviews.
		*/
	

		if(!isSwitching($interview->interview_type))
		{
			$GLOBALS['dbh']->Execute("
				DELETE 
				FROM interview_time
				WHERE student_number='" . $students_dropped[$i] . "'
				AND job_id='" . $job_id . "'
				");
		}
	
	}

	

	// $students_dropped has been set.  We're now done.

	if (isPhone($interview->interview_medium))

	{

		for ($i = 0; $i < sizeof($phoneinfo); $i++)
		{
			$newphoneresult = $GLOBALS['dbh']->Execute("
				SELECT time_id
				FROM interview_time
				WHERE int_time = '" . $phoneinfo[$i]["int_time"] . "'
				AND int_date = '" . $phoneinfo[$i]["int_date"] . "'
				AND end_time = '" . $phoneinfo[$i]["end_time"] . "'
				AND student_number = '" . $phoneinfo[$i]["student_number"] . "'
				AND job_id = '" . $phoneinfo[$i]["job_id"] . "'
				");
			if ($newphoneresult->RecordCount() != 0)
			{
				$newphonerow = $newphoneresult->FetchRow();
				$newtimeids[] = array(
					"time_id" => $newphonerow["time_id"],
					"phone" => $phoneinfo[$i]["phone"]
					);
			}
			$newphoneresult->Close();
		}
	
		/*
		 $newtimeids now holds the old phone numbers and the new time_id's to assign
		 them to.  All that's left to do is put them in.
		*/

		for ($i = 0; $i < sizeof($newtimeids); $i++)
		{
			$timeidresult = $GLOBALS['dbh']->Execute("
				INSERT INTO interview_phone(time_id, phone)
				VALUES (
				'" . $newtimeids[$i]["time_id"] . "', 
				'" . $newtimeids[$i]["phone"] . "' )
				");
		}

	}//if (phone)

	/*
	 The phone numbers have now all been updated.
	*/

	$GLOBALS['dbh']->Execute("
		REPLACE INTO interview_edit(job_id, date_edited, editor, menu_edited) 
		VALUES (
		'$job_id', 
		NULL, 
		'" . addslashes($auth->login) . "', 
		'Date/Time')
		");
	$row = $GLOBALS['dbh']->GetRow("
		SELECT MAX(date_edited) AS last_mod 
		FROM interview_edit 
		WHERE job_id = '$job_id' 
		AND menu_edited = 'Date/Time'
		");
	$interview->last_modified = $row["last_mod"];
	unset($old_ints);
	$GLOBALS['dbh']->GetRow("SELECT RELEASE_LOCK('$job_id')");
	
	notify("Changes have been successfully saved.");

/***	End of updating schedule	***/

}
else
{
   error($error_message . "<br />");
   $error_happened = true;
}


?>
