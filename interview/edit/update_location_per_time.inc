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
 | Filename: update_location_per_time.inc                                       |
 +------------------------------------------------------------------------------+
 | Description: This file is called by edit_location.inc when the user opts to  |
 | save the changes he has made.                                                |
 +------------------------------------------------------------------------------+

*/
$edit_conflict = 0;
$error = 0;
$row = $GLOBALS['dbh']->GetRow("SELECT GET_LOCK('$interview->job_id', 10)");
if ($row[0] != "1"){
	$error = 1;
	$error_message = "Can't get record lock.  Please try again.";
}else{
	
	$row = $GLOBALS['dbh']->GetRow("SELECT MAX(date_edited) as last_mod FROM interview_edit WHERE job_id = '$interview->job_id' AND menu_edited = 'Location'");
	if ($row["last_mod"] > $interview->last_modified){
		$row = $GLOBALS['dbh']->GetRow("SELECT CONCAT(c.first_name, ' ', c.last_name) as name
					FROM contact as c, contact_internal as i, interview_edit as e
					WHERE c.contact_id = i.contact_id AND i.login_id = e.editor AND
						e.job_id = '$interview->job_id' AND e.menu_edited='Location' AND e.date_edited = '" . $row["last_mod"] . "'");
		$error_message = "<b class='black'>WARNING:&nbsp;&nbsp;</b>";
		if ($row["name"] == ""){
			$error_message .= "An unknown person";
		}else{
			$error_message .= $row["name"];
		}
		
		
		$error = 1;
		$error_message .= " has been edited the location of this interview since you last reloaded.  Your changes have NOT been saved, but are shown below in bold.";
		$job_id = $interview->job_id;
		$interview = new EditInterview('');
		$interview->loadValues($job_id, 'Location');
		$GLOBALS['dbh']->GetRow("SELECT RELEASE_LOCK('$interview->job_id')");
		$edit_conflict= 1;
	}
}
if ($error == 0){
	if (isOnCampus($interview->interview_place) || isPhone($interview->interview_medium)){
		$result = $GLOBALS['dbh']->Execute("SELECT time_id
				FROM interview_time
				WHERE job_id = '" . $job_id . "'");

		while ($row = $result->FetchRow()){
			$varName = "room" . $row["time_id"];
			$int_room_id = $$varName;
			$varName = "wait_room" . $row["time_id"];
			$int_wait_room_id = $$varName;

			$student_sql = ("
                                SELECT DISTINCT a.student_number, CONCAT(b.first_name, ' ', b.last_name) AS name
                                FROM interview_time AS a, student AS b
                                WHERE time_id='" . addslashes($row["time_id"]) . "'
                                AND a.student_number IS NOT NULL
                                AND a.int_wait_room_id!='" . addslashes($int_wait_room_id) . "'
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

			$sql = ("UPDATE interview_time
					SET int_room_id = '" . addslashes($int_room_id) . "',
					    int_wait_room_id = '" . addslashes($int_wait_room_id) . "'
					WHERE time_id = '" . addslashes($row["time_id"]) . "' AND job_id = '" .  
						addslashes($job_id) . "'");
			$result2 = $GLOBALS['dbh']->Execute($sql);
		}
		$sql = "REPLACE INTO interview_edit(job_id, date_edited, editor, menu_edited) VALUES('" . addslashes($job_id) . "', NULL, '" . addslashes($auth->login) . "', 'Location')";
		$result = $GLOBALS['dbh']->Execute($sql);
	}

	$interview = new EditInterview('');
	$interview->loadValues($job_id, 'Location');
}else{
	error ($error_message);
}

if ($email_students)
{
	include('interview/edit/mail/send_waitroom_email.inc');
}
else
{
	notify("Interview location changes have been saved.");
	include('interview/edit/edit_location_per_time_form.inc');
}

?>
