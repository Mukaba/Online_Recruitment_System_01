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
 | Filename: update_location.inc                                                |
 +------------------------------------------------------------------------------+
 | Description: This file is called by edit_location.inc when the user opts to  |
 | save the changes they have made.                                             |
 +------------------------------------------------------------------------------+

*/


//check duplicate interview rooms
if( ($error == 0) && ( isOnCampus($interview->interview_place) || isPhone($interview->interview_medium) ) )
{
	//check duplicated room ( not including T.B.A )
	if( isDuplicate( $interview->locations, $duplicate ) && $duplicate != '')				
	{		   			
		$error = 1;
		$error_message = "Duplicate interview rooms.";
	}				
}	   			

$edit_conflict=0;
$row = $GLOBALS['dbh']->GetRow("SELECT GET_LOCK('$interview->job_id', 10)");
if ($row[0] != "1"){
	$error_message = ("Can't get lock. Please try again.");
	$error = 1;
}else{
	$row = $GLOBALS['dbh']->GetRow("SELECT MAX(date_edited) as last_mod FROM interview_edit WHERE job_id = '$interview->job_id' AND menu_edited='Location'");
	if ($row["last_mod"] > $interview->last_modified){
		$row = $GLOBALS['dbh']->GetRow("SELECT CONCAT(c.first_name, ' ', c.last_name) as name
					FROM contact as c, contact_internal as i, interview_edit as e
					WHERE c.contact_id = i.contact_id AND i.login_id = e.editor AND e.job_id = '$interview->job_id' AND menu_edited='Location'
					AND date_edited = '" . $row["last_mod"] . "'");
		
		
		$error_message = "<b class='black'>WARNING:&nbsp;&nbsp;</b>";
		if ($row["name"] == ""){
			$error_message .= "An Unknown person";
		}else{
			$error_message .= $row["name"];
		}
		$error_message .= (" has edited the location of this interview since you last loaded the page.  Your changes have NOT been saved, but are displayed below in bold.");
		$error = 1;
		$edit_conflict=1;
		$job_id = $interview->job_id;
		$interview2 = $interview;
		$interview = new EditInterview('');
		$interview->loadValues($job_id, 'Location');
		$GLOBALS['dbh']->GetRow("SELECT RELEASE_LOCK('$job_id')");
	}
}
	


/*******					update interview information in database		  	*******/
if ($error == 0 )
{

	/***		update off interview location		***/
	$result = $GLOBALS['dbh']->Execute("UPDATE interview
				SET interview_place_id = '" . addslashes($interview->interview_place) . "'
				WHERE job_id = '" . addslashes($interview->job_id) . "'");
	
	
	/***		update off campus interview location		***/
	
	if ( !isOnCampus($interview->interview_place) && !isPhone($interview->interview_medium) )
	{
        $existence_sql = ("
            SELECT job_id
            FROM interview_location
            WHERE job_id = '".$job_id."'
            ");
        $existence_result = $GLOBALS['dbh']->Execute($existence_sql);
        
		if($existence_result->RecordCount())
		{
			$GLOBALS['dbh']->Execute("UPDATE interview_location
	  		 	 SET street_address_1='" . addslashes($interview->address1) . "', street_address_2='" . addslashes($interview->address2) . "',
	  	   	 	     city='" . addslashes($interview->city) . "', province='" . addslashes($interview->province) . "',
	  	   	 	     country='" . addslashes($interview->country) . "', directions='" . addslashes($interview->direction) . "'
		  		 WHERE job_id='$interview->job_id'");
		}
		else
		{
			$GLOBALS['dbh']->Execute( "INSERT INTO interview_location( job_id, street_address_1, street_address_2, city, province, 
					country, directions )
				VALUES ('$job_id', '" . addslashes($interview->address1) . "','" . addslashes($interview->address2) . "',
					'" . addslashes($interview->city) . "',
					'" . addslashes($interview->province) . "',
					'" . addslashes($interview->country) . "',
					'" . addslashes($interview->direction) . "')");
			
		}		  	   			
	}
	
	if (isOnCampus($interview->interview_place) || isPhone($interview->interview_medium)){
		

		for ($i = 0; $i < count( $interview->interviewers); $i++)
		{

			/*
			 Update the rooms for this interview.
			*/

			$student_sql = ("
				SELECT DISTINCT a.student_number, CONCAT(b.first_name, ' ', b.last_name) AS name
				FROM interview_time AS a, student AS b
				WHERE a.job_id='" . $job_id . "'
				AND a.interviewer_id='" . $interview->interviewer_ids[$i] . "'
				AND a.student_number IS NOT NULL
				AND a.int_wait_room_id!='" . $interview->wait_locations[$i] . "'
				AND b.student_number=a.student_number
				");

			$student_res = $GLOBALS['dbh']->Execute($student_sql);
			while ($student_row = $student_res->FetchRow())
			{
				$email_address = getStudentEmail($student_row["student_number"]);
			
				$email_students[] = array(
					"name" => $student_row["name"],
					"student_number" => $student_row["student_number"],
					"email" => $email_address
					);
			}

			$result3b = $GLOBALS['dbh']->Execute("
				UPDATE interview_time
				SET 
				int_room_id = '" . $interview->locations[$i] . "',
				int_wait_room_id = '" . $interview->wait_locations[$i] . "'
				WHERE job_id = '" . $job_id . "' 
				AND interviewer_id = '" . $interview->interviewer_ids[$i] . "'
				");

			/*
			 * Old code to check to see if we are to use the default room number for
                         * this room if available. 
			 */
/*
			if (isset (${"use-room-number$i"}))
			{
			  // Find out the already existing phone number for this room.
			  $sql_ = "select r.phone,r.room_number from room as r, interview_time as it where it.int_room_id=r.room_id
				   and it.job_id=\"$job_id\" and interviewer_id=\"".$interview->interviewer_ids[$i]."\""; 
			  $result_ = $GLOBALS['dbh']->Execute ($sql_);
			  $response_ = $result_->FetchRow();
			  $roomsNumber = $response_["room_number"];
			  $roomsPhoneNumber = $response_["phone"];

			  // Now see if we've set a phone number for this room.
			  $sql_ = "select ip.phone,it.time_id from interview_phone as ip, interview_time as it where 
                                   ip.time_id=it.time_id and it.job_id=\"$job_id\"
				   and interviewer_id=\"".$interview->interviewer_ids[$i]."\"";
			  $result_ = $GLOBALS['dbh']->Execute ($sql_);
			  $response_ = $result_->FetchRow();

			  // Let's find out the jobs that already have phone numbers and exclude them.
			  $sql_ = "select distinct it.time_id from interview_time as it, interview_phone as ip where 
			            it.job_id=\"$job_id\" and it.interviewer_id=\"".$interview->interviewer_ids[$i]."\"
				    and it.time_id=ip.time_id";
			  $result_ = $GLOBALS['dbh']->Execute ($sql_);
			  $exclude_items = array ();
			  while ($response_ = $result_->FetchRow())
			  {
			    $exclude_items[] = $response_["time_id"]; 
			  }

			  if (count ($exclude_items))
			    $sql_ = "select distinct it.time_id from interview_time as it, interview_phone as ip where 
			              it.job_id=\"$job_id\" and it.interviewer_id=\"".$interview->interviewer_ids[$i]."\"
			              and it.time_id not in (".implode(", ", $exclude_items).")";
			  else
			    $sql_ = "select distinct it.time_id from interview_time as it, interview_phone as ip where 
			              it.job_id=\"$job_id\" and it.interviewer_id=\"".$interview->interviewer_ids[$i]."\"";
			  $result_ = $GLOBALS['dbh']->Execute ($sql_);

			  // Now update it for each job.
			  while ($response_ = $result_->FetchRow())
			  {
			    $sql_ = "insert into interview_phone (time_id, phone) values (\"".
			      $response_["time_id"]."\", \"$roomsPhoneNumber\")";
			    $GLOBALS['dbh']->Execute ($sql_);
			  }
			}
*/
			/** END BILL'S CHANGES. **/
		}
	}else{
		for ($i = 0; $i < count( $interview->interviewers); $i++)
		{
			$result3b = $GLOBALS['dbh']->Execute("
				UPDATE interview_time
				SET 
				int_room_id = NULL, int_wait_room_id = NULL
				WHERE job_id = '" . $job_id . "' 
				AND interviewer_id = '" . $interview->interviewer_ids[$i] . "'
				");
		}
	}

	/*
	Check here to see if we have changed an off-campus phone interview into
	an on-campus phone interview.  If this is the case, we will need to
	remove any student phone numbers from the database, and send them an
	e-mail letting know that they will now be expected to come to campus for
	the phone interview.
	*/

	/*
	 Note: $old_interview_place -> will be the interview place before the update was made (set in edit_locatin.inc)
	*/
	if (isPhone($interview->interview_medium) && isOnCampus($interview->interview_place) && !isOnCampus($old_interview_place))
	{
		$phoneresult = $GLOBALS['dbh']->Execute("
					SELECT DISTINCT a.student_number, a.first_name, a.last_name, c.time_id
					FROM student AS a, interview_time as b, interview_phone as c
					WHERE b.job_id='" . $interview->job_id . "'
					AND c.time_id=b.time_id
					AND b.student_number=a.student_number
					AND c.phone IS NOT NULL
				");
		if ($phoneresult->RecordCount() != 0)
		{
			while ($phonerow = $phoneresult->FetchRow())
			{
				$name = $phonerow["first_name"] . " " . $phonerow["last_name"];
				$phone_changed[] = array(
					"student_number" => $phonerow["student_number"],
					"name" => $name,
					"time_id" => $phonerow["time_id"]
				);
			}
		}
		$phoneresult->Close();

		/*
		 Now delete those phone numbers.
		*/

		for ($i = 0; $i < sizeof($phone_changed); $i++)
		{
			$GLOBALS['dbh']->Execute("
				DELETE
				FROM interview_phone
				WHERE time_id='" . $phone_changed[$i]["time_id"] . "'
				");
		}
	}

	/** Start Bill's new changes **/
	// If this is a phone interview, we've done something to the checkbox to use the room's phone number ...
	if (isPhone ($interview->interview_medium))
	{
	  $use_room_number = 0;

          //error ("Hey, \$interview->use_room_number is ".$interview->use_room_number);

	  // Use the room's number?
	  if ($interview->use_room_number == "on")
	    $use_room_number = 1;

	  $squirrel = "update interview set use_room_phone_number=\"$use_room_number\" where job_id=\"$job_id\""; 
	  $GLOBALS['dbh']->Execute ($squirrel);

	}
	/**  End Bill's new changes  **/

	$sql = "REPLACE INTO interview_edit (job_id, date_edited, editor, menu_edited) VALUES('" . $interview->job_id . "', NULL, '" . $auth->login . "', 'Location')";
	$result = $GLOBALS['dbh']->Execute($sql);

	if ($phone_changed)
	{
		include('interview/edit/mail/send_phone_email.inc');
	} elseif ($email_students)
	{
		include('interview/edit/mail/send_waitroom_email.inc');
	}
	else
	{
		notify("Interview location changes have been saved.");
	
		$job_id = $interview->job_id;
		$interview = new EditInterview('');
		$interview->loadValues($job_id, 'Location');
		include('interview/edit/edit_location_form.inc');
	}
	$GLOBALS['dbh']->GetRow("SELECT RELEASE_LOCK($interview->job_id)");
}
else
{
   error($error_message . "<br />");
   include('interview/edit/edit_location_form.inc');
}

?>
