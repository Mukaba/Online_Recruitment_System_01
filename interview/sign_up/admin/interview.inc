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
 | Filename: interview.inc                                                      |
 +------------------------------------------------------------------------------+
 | Description:                                                                 |
 +------------------------------------------------------------------------------+

*/

// filename: interview.inc

/* Programmer's Notes:
	
	2000-12-11	10:36	Stephen
	Subject: Adding functionality for allowing admins to specify a phone number
		for out of town students regardless of the interview type.
	Notes: Add a new button with a new action value (ex. action='special_phone').
		The button should only appear when the interview is not already an off-campus
		phone interview.  In the update function, check for action='special_phone'
		and handle it.   Use update_phone function to update the phone number.
		Remember, phone number should be set only for time_ids for which a student is
		signed up.
	Status: No work has been done on this yet.
	
	2000-12-11	10:44	Stephen
	Subject: Time conflicts
	Notes: For switching interviews, the switch interview time may not be being
		checked for time conflicts.  Check this out and fix it if this is true.
	Status: No work has been done on this yet.
		2001-04-19 Emily has fixed this by adding increasing the length of the time checked to being
				twice the length of the interviews.  This modification was made in
				 the function isTimeConflict in misc/functions.inc

*/

include('misc/isEmailSent.inc');
define("MAX_FORM_LINES", 250);	// prevents malicious user input from sending
											// this script into an (almost) infinite loop.
											// effectively, the maximum number of interview
											// time-slots it will look for is MAX_FORM_LINES.

$interview = new interview($job_id, $department_id, $last_modified, $last_edited);
$interview->display();



class interview
{
	var $error_output = "";
	var $notify_output = "";
	var $output = "";

	var $job_id;
	var $department_id;
	var $display_rooms;
	var $display_phone;
		
	var $action;
	var $num_form_lines;	
		
	var $last_modified = "";			
	var $last_edited = "";

	var $edit_conflict = 0;	
	function interview($job_id, $department_id, $last_modified, $last_edited)
	{
		global $action;
		global $num_form_lines;
		$this->action = $action;
		$this->num_form_lines = $num_form_lines;
		$this->department_id = $department_id;	
		$this->job_id = $job_id;
		
		$this->last_modified = $last_modified;
		$this->last_edited = $last_edited;	
	}

	
	function error($error_text) { $this->error_output .= "<p>" . $error_text . "</p>"; }
	function notify($notify_text) { $this->notify_output .= "<p>" . $notify_text . "</p>"; }

		
	function display()	
	{
		$this->generate();
		
		if ($this->error_output) { echo(error("<h1 class='black'>Error</h1>" . $this->error_output)); }
		if ($this->notify_output) { echo(notify($this->notify_output)); }
		echo($this->output);
	}
		
	
	function generate()
	{
		$this->update();
	
		$result = $GLOBALS['dbh']->Execute("
			SELECT	i.interview_medium_id as medium, i.interview_place_id AS place
			FROM interview AS i
			WHERE
			      i.job_id = '" . addslashes($this->job_id) . "'");
		if (!$result)
		{
			$this->error("Query failed.  Could not find job.");
			return false;
		}
		
		$success = false;
		if (($result->RecordCount() == 1) && ($row = $result->FetchRow()))
		{
			$this->display_rooms = isOnCampus($row["place"]) || isPhone($row["medium"]);
			$this->display_phone = isPhone($row["medium"]) && !isOnCampus($row["place"]);
			
			$success = true;
		}
		$result->Close();
		if (!$success)
		{
			$this->error("Query returned either no rows or multiple rows.");
			return false;
		}
		
		$this->generate_main();
	}
	

	

	function generate_main()
	{
		$this->generate_main_head();
		
		$this->generate_main_job_info();
		$this->generate_main_shortlist();

		$this->generate_main_schedule();
		
		$this->generate_main_presentation_info();
		$this->generate_main_notes();
		
		$this->generate_main_links();
	}
	
	function generate_main_head()
	{
		$this->output .= "\n\n<!--main_head-->\n";
		$this->output .= "<h3>Interviews - Modify Sign-up</h3>\n";
	}

	function generate_main_job_info()
	{
		global $PHP_SELF;
		$this->output .= "\n\n<!--main_job_info-->\n";
	

		$result = $GLOBALS['dbh']->Execute("
				SELECT interview_place_id, interview_medium_id
				FROM interview
				WHERE job_id = '" . addslashes($this->job_id) . "'");
		$row = $result->FetchRow();
		$interview_place_id = $row["interview_place_id"];
		$interview_medium_id = $row["interview_medium_id"];
		if (!isOnCampus($interview_place_id) && !isPhone($interview_medium_id)){
			$result = $GLOBALS['dbh']->Execute("
				SELECT * 
				FROM interview_location
				WHERE job_id = '" . addslashes($this->job_id) . "'");
			echo $GLOBALS['dbh']->ErrorMsg();
			$row = $result->FetchRow();
			$loc_str = $row["street_address_1"] . "<br />" . (($row["street_address_2"] == "") ? "" : $row["street_address_2"] .
					"<br />") . $row["city"] . ", " . $row["province"] . "<br />" . $row["country"];
		}	
		$result = $GLOBALS['dbh']->Execute("
			SELECT	a.job_code, a.department_id, b.company_name, c.interview_place_id, c.interview_medium_id 
			FROM	job_info AS a, employer_company AS b, interview AS c 
			WHERE	a.job_id='" . addslashes($this->job_id) . "' AND
				a.employer_id=b.employer_id AND c.job_id = a.job_id");
		
		if (!$result) {
			$this->error("Query failed.");
		}
		else
		{
			if ($row = $result->FetchRow())
			{
				$company_name = $row["company_name"];
				$department_id = $row["department_id"];
				$job_code = $row["job_code"];
				// Get Location
				$location = $row["interview_place_id"];
				$location_query = $GLOBALS['dbh']->Execute("SELECT description from interview_place where interview_place_id = '$location'");
				if ($row2 = $location_query->FetchRow()) {
					$location = $row2["description"];
				} else {
					$location = "Unknown";
				}
				// Get Medium
				$medium = $row["interview_medium_id"];
				$medium_query = $GLOBALS['dbh']->Execute("SELECT description from interview_medium where interview_medium_id = '$medium'");
				if ($row3 = $medium_query->FetchRow()) {
					$medium = $row3["description"];
				} else {
					$medium = "Unknown";
				}
			}
			$result->Close();
		}
        $sql = "SELECT contact_id, employer_department_id, employer_id, job_code
            FROM job_info
            WHERE job_id='" . addslashes($this->job_id) . "'";
        $result = $GLOBALS['dbh']->Execute($sql);
        $row_quick = $result->FetchRow();

        $this->output .= "<table width='100%'>\n";
		$this->output .= "<tr>\n";
		$this->output .= "<td align='right'>\n";
		$this->output .= "<form method='post' action='" . $PHP_SELF . "&amp;PDF=1&amp;job_id=" . urlencode(addslashes($this->job_id)) . "&amp;department_id=" . urlencode($this->department_id) . "'>\n";
        $this->output .= "<input type='hidden' name='department_id' value='".$row_quick['employer_department_id']."' />";
        $this->output .= "<input type='hidden' name='employer_id' value='".$row_quick['employer_id']."' />";
        $this->output .= "<input type='hidden' name='job_id' value='".addslashes($this->job_id)."' />";
        $this->output .= "<input type='hidden' name='contact_id' value='".$row_quick['contact_id']."' />";
        $this->output .= "<input type='hidden' name='job_code' value='".$row_quick['job_code']."' />";
        
		$this->output .= "<select name='selection'>\n";
        $this->output .= "<option value='view_company'>View this Company</option>\n";
        $this->output .= (($row_quick['employer_department_id']) ? "<option value='view_department'>View this Job's Division</option>\n" : "");
        $this->output .= (($row_quick['contact_id']) ? "<option value='view_contact'>View this Job's Contact</option>\n" : "");
        $this->output .= "<option value='view_applications'>View this Job's Applications</option>\n";
        
        
		$this->output .= "<option value='edit'>Edit this Interview</option>\n";
		$this->output .= "<option value='single'>Preview Schedule</option>\n";
		$this->output .= "<option value='final'>Final Interview Schedule</option>\n";
		$this->output .= "<option value='final_student'>Final Interview Schedule (Student Numbers)</option>\n";
		$this->output .= "<option value='email_schedule'>E-mail Schedule to Employer</option>\n";

		$sql = ("
			SELECT DISTINCT emailed_newsgroup
			FROM interview
			WHERE job_id='" . $this->job_id . "'
			");
		$result = $GLOBALS['dbh']->Execute($sql);
		$row = $result->FetchRow();
		
		if (trim($row["emailed_newsgroup"]))
		{
			$this->output .= "<option value='email_students'>E-mail Shortlisted Students</option>\n";
		}

		$this->output .= "</select>\n";
		$this->output .= "&nbsp;\n";
		$this->output .= "<input type='image' src='misc/images/arrowg.gif' alt='go' />\n";
		$this->output .= "</form>\n";
		$this->output .= "</td>\n";
		$this->output .= "</tr>\n";
		$this->output .= "</table>\n\n";
		
		$this->output .= "<table cellpadding='0' cellspacing='0'>\n";
		$this->output .= "	<tr>\n";
		$this->output .= "		<td><b>Company:</b>&nbsp;</td><td>" . $company_name . "</td>\n";
		$this->output .= "	</tr>\n";
		$this->output .= "	<tr>\n";
		$this->output .= "		<td><b>Job Code:</b>&nbsp;</td>";
		$this->output .= "		<td><a href='$PHP_SELF&amp;select=view_job&amp;level1=job_id&amp;job_id=$this->job_id' target='_blank'>" . $job_code . "</a></td>\n";
		$this->output .= "	</tr>\n";
        $this->output .= "  <tr>\n";
        $this->output .= "      <td align='left'><b>Host Department:&nbsp;</b></td><td>".getDepartmentName($department_id)."</td>";
        $this->output .= "  </tr>\n";
		$this->output .= "	<tr>\n";
		$this->output .= "		<td align='left'><b>Location:</b>&nbsp;</td><td>" . $location . "</td>\n";
		$this->output .= "	</tr>\n";
		$this->output .= "	<tr>\n";
		$this->output .= "		<td><b>Medium:</b>&nbsp;</td><td>" . $medium . "</td>\n";
		$this->output .= "	</tr>\n";
		if (!isOnCampus($interview_place_id) && !isPhone($interview_medium_id)){
			$this->output .= "	<tr>\n";
			$this->output .= "		<td align='left'><b>Location:</b>&nbsp;</td><td>" . $loc_str . "</td>\n"; 
			$this->output .= "	</tr>\n";
		}
		$this->output .= "</table>\n";
	}

	function generate_main_presentation_info()
	{
		$this->output .= "\n\n<!--main_presentation_info-->\n";
	
		$result = $GLOBALS['dbh']->Execute("
			SELECT pres_date, pres_time, pres_loc, pres_notes, end_time
			FROM presentation
			WHERE job_id='" . addslashes($this->job_id) . "'");

		if (!$result)
		{
			$this->error("Query failed.  (There may be a presentation for this interview.)");
		}
		else
		{
			if ($row = $result->FetchRow())
			{
				$this->output .= "<h4>Presentation:</h4>\n";
			
				$this->output .= "<table class='backgrey' cellpadding='5' width='100%'><tr><td>\n";
				$this->output .= "<table cellspacing='0' cellpadding='0'>\n";
				$this->output .= "<tr><td><b>Date:</b>&nbsp;</td><td>" . formatLongDate($row["pres_date"]) . "</td></tr>\n";
				$this->output .= "<tr><td><b>Time:</b>&nbsp;</td><td>" . formatTime($row["pres_time"], false) . " to " . formatTime($row["end_time"], false) . "</td></tr>\n";
				$this->output .= "<tr><td><b>Location:</b>&nbsp;</td><td>" . $row["pres_loc"] . "</td></tr>\n";
				if ($row["pres_notes"])
				{
					$this->output .= "<tr><td valign='top'><b>Notes:</b>&nbsp;</td><td valign='top'>" . $row["pres_notes"] . "</td></tr>\n";
				}
				$this->output .= "</table>\n";
				$this->output .= "</td></tr></table>\n";
			}
			$result->Close();
		}
	
	}
	
	function generate_main_notes()
	{
		$this->output .= "\n\n<!--main_notes-->\n";
		$result = $GLOBALS['dbh']->Execute("
			SELECT notes, staff_notes 
			FROM interview 
			WHERE job_id='" . addslashes($this->job_id) . "'
			");

		if (!$result)
		{
			$this->error("Query failed.  (Any notes for this interview will not be displayed.)");
		}
		else
		{
			if ($row = $result->FetchRow())
			{
				if ($notes = $row["notes"])
				{
					$this->output .= "<h4>Interview Notes:</h4>\n";
					$this->output .= "<table class='backgrey' cellpadding='5' width='100%'><tr><td>\n";
					$this->output .= $notes . "\n";
					$this->output .= "</td></tr></table>\n";
				}
				if ($staff_notes = $row["staff_notes"])
				{
					$this->output .= ("<h4>Co-op Staff Notes:</h4>\n");
					$this->output .= ("<table class='backgrey' cellpadding='5' width='100%'><tr><td>\n");
					$this->output .= $staff_notes . "\n";
					$this->output .= ("</td></tr></table>\n");
				}
			}
		}
	}
	
	function generate_main_schedule()
	{
		$this->output .= "\n\n<!--main_schedule-->\n";
		
	/*
	 New code has been added below to prevent students being signed up
	 for interviews in the past.
	*/

		global $PHP_SELF;
		$date_result = $GLOBALS['dbh']->Execute("SELECT CURRENT_DATE");
		$date_row = $date_result->FetchRow();
		$current_date = $date_row["CURRENT_DATE"];

		$unsigned_students = getUnsignedStudents($this->job_id);
		if (!is_array($unsigned_students)) {
			error("Query failed.  Unsigned students could not be returned.");
		}
	
		$row = $GLOBALS['dbh']->GetRow("SELECT MAX(last_edited) as last_mod FROM signup_edit WHERE job_id = '" . addslashes($this->job_id) . "'");
		$mod = $row["last_mod"];
		$row = $GLOBALS['dbh']->GetRow("SELECT MAX(date_edited) as last_mod FROM interview_edit WHERE job_id = '" . addslashes($this->job_id) . "'");
		$mod_edit  = $row["last_mod"];
		$this->output .= "<form action='" . $PHP_SELF . "' method='post'>\n";
		$this->output .= "<input type='hidden' name='select' value='sign_up' />\n";
		$this->output .= "<input type='hidden' name='job_id' value='" . $this->job_id . "' />\n";
		$this->output .= "<input type='hidden' name='last_modified' value='" . $mod . "' />\n";
		$this->output .= "<input type='hidden' name='last_edited' value='" . $mod_edit . "' />\n";
		
		global $PHP_SELF;		

		$job_id = $this->job_id;
		$student_number = $this->student_number;
		$signed_up = isStudentSignedUp($job_id, $student_number);
		$cancelled = isJobCancelled($job_id);
	
		$sql = ("
			SELECT DISTINCT a.int_date, a.interviewer_id, b.interviewer_name, a.sequential
			FROM interview_time AS a, interview_interviewer AS b
			WHERE a.job_id='" . addslashes($job_id) . "' AND a.interviewer_id=b.interviewer_id
			ORDER BY a.int_date, a.sequential, b.interviewer_name
			");

		$result = $GLOBALS['dbh']->Execute($sql);

		$form_line = 0;
		$form_line_advance = false;
		while ($row = $result->FetchRow())
		{

			$this->output .= "<table>\n";
			$this->output .= "<tr><td><b>Interviewer:</b></td><td>" . $row["interviewer_name"] . "</td></tr>\n";
			$this->output .= "<tr><td><b>Date:</b></td><td>" . formatLongDate($row["int_date"]) . "</td></tr>\n";
			$this->output .= "</table>\n";

			// Due to buggy behaviour in MySQL, the line
			// 	IF(a.int_type=1, 'interview', IF(a.int_type=2, 'break', 'lunch')) AS int_type
			// has been used instead of
			//    ELT(a.int_type, 'interview', 'break', 'lunch') AS int_type
			// The latter simply does not work with this query (for no particular reason)
			//    - If you removed the ORDER BY, it would work
			//    - If you don't JOIN the interview_phone table it works
			//    - As it is, it doesn't work
			
			$result_times = $GLOBALS['dbh']->Execute("
				SELECT 	a.job_id, a.time_id, a.int_time, a.switch, a.sequential,
				IF(a.int_type=1, 'interview', IF(a.int_type=2, 'break', IF(a.int_type=3, 'lunch', 'other'))) AS int_type,
				b.student_number, CONCAT(b.first_name, ' ', b.last_name) AS student_name
				" . ((!$this->display_rooms) ? "" :
				"
				, CONCAT(c2.building_code, ' ', c.room_number) AS interview_room
				, CONCAT(d2.building_code, ' ', d.room_number) AS wait_room
				"
				) . "
				" . ((!$this->display_phone) ? "" :
				"
				, e.phone
				"
				) . "
				FROM	interview_time AS a
				LEFT OUTER JOIN	student AS b
				ON		a.student_number=b.student_number
				" . ((!$this->display_rooms) ? "" :
				"
				LEFT OUTER JOIN	room AS c
				ON		a.int_room_id=c.room_id
				LEFT OUTER JOIN	building AS c2
				ON		c.building_id=c2.building_id
				LEFT OUTER JOIN	room AS d
				ON		a.int_wait_room_id=d.room_id
				LEFT OUTER JOIN	building AS d2
				ON		d.building_id=d2.building_id
				"
				) . "
				" . ((!$this->display_phone) ? "" :
					"
					LEFT OUTER JOIN	interview_phone AS e
					ON		a.time_id=e.time_id
					"
				) . "
				WHERE	int_date='" . $row["int_date"] . "' AND
						interviewer_id='" . $row["interviewer_id"] . "' AND
						job_id='" . addslashes($job_id) . "'
				ORDER BY int_time");
	
				
			
			$this->output .= "<table cellspacing='0' cellpadding='0' border='1'><tr><td>\n";
			$this->output .= "	<table border='0' cellpadding='2'>\n";
			$this->output .= "		<tr>\n";
			$this->output .= "			<td class='rowgrey' align='center'>&nbsp;<b class='white'>Interview Time</b>&nbsp;</td>\n";
			$this->output .= "			<td class='rowgrey' align='center'>&nbsp;<b class='white'>Student</b>&nbsp;</td>\n";
			if ($this->display_rooms)
			{
				$this->output .= "			<td class='rowgrey' align='center'>&nbsp;<b class='white'>Room</b>&nbsp;</td>\n";
				$this->output .= "			<td class='rowgrey' align='center'>&nbsp;<b class='white'>Wait Room</b>&nbsp;</td>\n";
			}
			if ($this->display_phone)				
			{
				$this->output .= "			<td class='rowgrey' align='center'>&nbsp;<b class='white'>Phone</b>&nbsp;</td>\n";
			}
			$this->output .= "			<td class='rowgrey' align='center'>&nbsp;<b class='white'>Del</b>&nbsp;</td>\n";
			$this->output .= "		</tr>\n";
			
			while ($row_times = $result_times->FetchRow())
			{
				if ($this->edit_conflict){
					$match=false;
					for($i=0;$i<$this->num_form_lines;$i++){
						$var_time = "time_id_" . $i;
						global $$var_time;
						if ($$var_time == $row_times["time_id"]){
							$match=true;
							$var_student_num = "student_num_last_" . $i;
							global $$var_student_num;
							if ($$var_student_num == "combo"){
								$var_student_num = "student_num_" . $i;
							}
							$var_phone = "phone_" . $i;
							global $$var_student_num;
							global $$var_phone;
							
							break;
						}
					}
				}
				switch($row_times["int_type"])
				{
					case "interview":
						$student_num = $row_times["student_number"];
						$this->output .= "		<tr>\n";
						$this->output .= "			<td class='row0d' align='center'>";
						if (!$cancelled && !$row_times["switch"] && (($student_num != "") || (($student_num == "") && sizeof($unsigned_students))))
						{
							$this->output .= "<input type='hidden' name='time_id_" . $form_line . "' value='" . $row_times["time_id"] . "' />\n";
							$form_line_advance = true;
						}	
						$this->output .= formatTime($row_times["int_time"]) . "</td>\n";
						if ((!$cancelled) && ($student_num == "") && (!$row_times["sequential"]) && (!$row_times["switch"]) && sizeof($unsigned_students) && !($row["int_date"] < $current_date))
						{
							$this->output .= "			<td class='row0d'>&nbsp;\n";
					    	$this->output .= "<input type='hidden' name='student_num_last_" . $form_line . "' value='combo' />\n";	
                            $this->output .= "<select name='student_num_" . $form_line . "'>\n";
							$this->output .= "	<option value=''>&nbsp;</option>\n";
							$num_students = sizeof($unsigned_students);
							for ($i = 0; $i < $num_students; $i++)
							{
								$this->output .= "<option value='" . $unsigned_students[$i]["student_number"] . "'>" . $unsigned_students[$i]["student_name"] . "</option>\n";
							}
							$this->output .= "</select>\n";
						}
						else
						{
							$this->output .= "			<td class='row0d'>&nbsp;";
							$this->output .= $row_times["student_name"] . "&nbsp;";
							$this->output .= "<input type='hidden' name='student_num_last_" . $form_line . "' value='" . $row_times["student_number"] . "' />\n";	
						}
							
						if ($this->edit_conflict && $match && !$row_times["sequential"] && !$row_times["switch"]){
							$this->output .="<br /><b class='black'>(" . getStudentName($$var_student_num) .  ")</b>\n";
						}
						
						$this->output .= "			&nbsp;</td>\n";

						if ($this->display_rooms)
						{
							$this->output .= "			<td class='row0d'>&nbsp;" . (($row_times["interview_room"]=="") ? "TBA" : $row_times["interview_room"]) . "&nbsp;</td>\n";
							$this->output .= "			<td class='row0d'>&nbsp;" . (($row_times["wait_room"]=="") ? "TBA" : $row_times["wait_room"]) . "&nbsp;</td>\n";
						}
						
						if ($this->display_phone)
						{
							if ((!$cancelled) && ($student_num != "") && (!$row_times["switch"]) && (!$row_times["sequential"]))
							{
								$this->output .= "			<td class='row0d'>&nbsp;<input type='text' name='phone_" . $form_line . "' maxlength='25' size='20' value='" . $row_times["phone"] . "' />&nbsp;\n";
							}
							elseif ((!$cancelled) && ($student_num != "") && (!$row_times["switch"]) && ($row_times["sequential"]))
							{
								$this->output .= "			<td class='row0d'>&nbsp;" . $row_times["phone"] . "&nbsp;<input type='hidden' name='phone_" . $form_line . "' value='" . $row_times["phone"] . "' />\n";
							}
							else
							{
								$this->output .= "			<td class='row0d'>&nbsp;" . $row_times["phone"] . "&nbsp;\n";
							}
							if ($this->edit_conflict && $match && !$row_times["switch"] && !$row_times["sequential"]){ $this->output .= "<br /><b class='black'>(" . $$var_phone . ")</b>\n"; }
							$this->output .= "</td>\n";
						}

						$this->output .= "			<td class='row0d' align='center'>&nbsp;";
						if ((!$cancelled) && ($student_num != "") && (!$row_times["sequential"]) && (!$row_times["switch"]))
						{
							$this->output .= "<input type='checkbox' name='del_" . $form_line . "' />";
						}
						$this->output .= "</td>\n";
						
						$this->output .= "		</tr>\n";
						break;
				
					case "break":
						$this->output .= "		<tr><td class='row1d' align='center'>" . formatTime($row_times["int_time"]) . "</td>\n";
						$this->output .= "			<td class='row1d'>&nbsp;Break&nbsp;</td>\n";
						if ($this->display_rooms)
						{
							$this->output .= "			<td class='row1d'>&nbsp;</td>\n";
							$this->output .= "			<td class='row1d'>&nbsp;</td>\n";
						}
						if ($this->display_phone) { $this->output .= "			<td class='row1d'>&nbsp;</td>\n"; }
						$this->output .= "			<td class='row1d' align='center'>&nbsp;</td>\n";
						$this->output .= "		</tr>\n";
						break;
				
					case "lunch":
						$this->output .= "		<tr><td class='row1d' align='center'>" . formatTime($row_times["int_time"]) . "</td>\n";
						$this->output .= "			<td class='row1d'>&nbsp;Lunch&nbsp;</td>\n";
						if ($this->display_rooms)
						{
							$this->output .= "			<td class='row1d'>&nbsp;</td>\n";
							$this->output .= "			<td class='row1d'>&nbsp;</td>\n";
						}
						if ($this->display_phone) { $this->output .= "			<td class='row1d'>&nbsp;</td>\n"; }
						$this->output .= "			<td class='row1d' align='center'>&nbsp;</td>\n";
						$this->output .= "		</tr>\n";
						break;
				}
				if ($form_line_advance)
				{
					$form_line++;
					$form_line_advance = false;
				}
			
			}
		
			$this->output .= "	</table>\n";
			$this->output .= "</td></tr></table>\n";
		}
		$this->output .= "<br />\n";
		$this->output .= "<div align='left'><input type='submit' name='action' value='Save Changes' />&nbsp;&nbsp;<input type='submit' name='action' value='Cancel' /></div>\n";
		$this->output .= "<input type='hidden' name='num_form_lines' value='" . $form_line . "' />\n";
		$this->output .= "</form>\n";
	}

	
	
	
		
	function generate_main_conflict()
	{
		$this->output .= "\n\n<!--main_conflict-->\n";

		$result = $GLOBALS['dbh']->Execute("
			SELECT DISTINCT b.department_name, CONCAT(c.first_name, ' ', c.last_name) AS contact_name,
					c.email, c.phone
			FROM	interview AS a, department AS b
			LEFT OUTER JOIN contact AS c
			ON		a.contact=c.contact_id
			WHERE	a.department_id=b.department_id AND a.job_id='" . addslashes($this->job_id) . "'");

		if ($result)
		{
			if ($row = $result->FetchRow())
			{
				if ($contact = $row["contact_name"])
				{
					if ($email = $row["email"]) { $contact = "<a href='mailto:" . $email ."'>" . $contact . "</a> &lt;" . $email. "&gt;"; }
					if ($phone = $row["phone"]) { $contact .= " (" . $phone. ")"; }
				}
				else
				{
					$contact = $row["department_name"] . " Office";
				}
			}
			$result->Close();
		}
		else
		{
			$contact = "your Co-op office";
		}
		
		$this->output .= "<p>If you have a conflict with all available interview times, please contact <b>$contact</b>.</p>";
	}

		
	function generate_main_shortlist()
	{
		$this->output .= "\n\n<!--main_shortlist-->\n";

		$result = $GLOBALS['dbh']->Execute("
		   SELECT a.student_number,CONCAT(a.first_name, ' ', a.last_name) AS student_name, c.department_name
		   FROM student AS a, students_shortlisted AS b, department as c, student_department as sd
		   WHERE b.job_id='" . addslashes($this->job_id) . "' AND a.student_number=b.student_number AND sd.department_id = c.department_id AND a.student_number=sd.student_number 
		   ORDER BY a.last_name, a.first_name, c.department_name");

		if (!$result) { return false; }
		
		$intRow=-1;
		$lastStudentNumber=0;
		while ($row=$result->FetchRow()){
			if ($lastStudentNumber<>$row['student_number']){
				$intRow=$intRow+1;
				$arrStudentsShortlisted[$intRow][0]=$row['student_name'];
				$arrStudentsShortlisted[$intRow][1]=$row['department_name'];
				$lastStudentNumber=$row['student_number'];
			}else{
				$arrStudentsShortlisted[$intRow][1]=$arrStudentsShortlisted[$intRow][1].", ".$row['department_name'];
			}
		}
	
		if ($result->RecordCount())
		{
			$this->output .= "<table cellspacing='0' cellpadding='0' border='1'><tr><td>\n";
			$this->output .= "<table border='0' cellpadding='2'>\n";
			$this->output .= "<tr>\n";
			$this->output .= "<td class='rowgrey' align='center'>&nbsp;<b class='white'>Students shortlisted</b>&nbsp;</td>
					  <td class='rowgrey' align='center'>&nbsp;<b class='white'>Department</b>&nbsp;</td>\n";
			$this->output .= "</tr>\n";

			$rowcount = 0;
			while ($rowcount <= $intRow)
			{
				$this->output .= "<tr><td class='" . (($rowcount % 2) ? "row0d" : "row1d") . "'>&nbsp;" . $arrStudentsShortlisted[$rowcount][0] . "&nbsp;</td>
							<td class='" . (($rowcount % 2) ? "row0d" : "row1d") . "'>&nbsp;". $arrStudentsShortlisted[$rowcount][1]."</td></tr>\n";
			$rowcount++;
			}

			$this->output .= "</table>\n";
			$this->output .= "</td></tr></table>\n";
		}
		
		$result->Close();
	}

	
	function generate_main_links()
	{
		$this->output .= "\n\n<!--main_links-->\n";
		global $PHP_SELF;
				
		// Edit
		$this->output .= "<a class='blue' href=\"" . $PHP_SELF . "&amp;select=edit&amp;job_id=" . $this->job_id . "&amp;department_id=" . $this->department_id . "\">Edit this interview</a><br />";
		// Employer e-mail
		$this->output .= "<a class='blue' href=\"" . $PHP_SELF . "&amp;select=sign_up&amp;job_id=" . $this->job_id . "&amp;department_id=" . $this->department_id . "&amp;email_employer=create\">E-mail Schedule to Employer</a>" . ((isEmployerEmailSent($this->job_id)) ? " (already sent)": "");
		// Print Preview
		$this->output .= "<br /><a class='blue' href='$PHP_SELF&amp;PDF=1&amp;selection=final&amp;job_id=" . $this->job_id . "&amp;department_id=" . $this->department_id . "'>Final Schedule Preview</a>";
		
	}

	
	/*
	 Function below returns the other associated time_id's for the given sequential time_id.
	*/

	function get_sequential($time_id)
	{
		$time_ids = "'" . addslashes($time_id) . "'";
	
		$sql = ("
			SELECT DISTINCT int_date, int_time, end_time, sequential, job_id
			FROM interview_time
			WHERE time_id='" . addslashes($time_id) . "'
			");
		$result = $GLOBALS['dbh']->Execute($sql);
		$row = $result->FetchRow();
	
		$stagger_amount = cal_length($row["int_time"], $row["end_time"]);
		$cur_sequential = $row["sequential"];  
		$job_id = $row["job_id"];
		$int_date = $row["int_date"];
		$int_time = $row["int_time"];
		$end_time = $row["end_time"];
		$sequential_position = $row["sequential"];
	
		/*
		 $i only goes up to 5 in the next loop, because there should NEVER be any more than 5 people a student has
		 to see sequentially.  This can be changed later if necessary.
		*/
	
		for ($i = $sequential_position; $i < 5; $i++)
		{
			$int_time = add_times($int_time, $stagger_amount);
			$end_time = add_times($end_time, $stagger_amount);
			$cur_sequential++;
	
			$sql = ("
				SELECT DISTINCT time_id
				FROM interview_time
				WHERE int_date='" . addslashes($int_date) . "'
				AND int_time='" . addslashes($int_time) . "'
				AND end_time='" . addslashes($end_time) . "'
				AND job_id='" . addslashes($job_id) . "'
				AND sequential='" . addslashes($cur_sequential) . "'
				");
			$result = $GLOBALS['dbh']->Execute($sql);
			if ($result->RecordCount() == 0)
				{
				/*
				 There are no more time_id's that correspond, so jump out and begin checking time_id's 
				 before this one.
				*/
				break;
			}
			$row = $result->FetchRow();
			$time_ids .= ", '" . addslashes($row["time_id"]) . "'";
		}
	
		$sql = ("
       	         SELECT DISTINCT int_date, int_time, end_time, sequential, job_id
       	         FROM interview_time
       	         WHERE time_id='" . addslashes($time_id) . "'
       	         ");
		$result = $GLOBALS['dbh']->Execute($sql);
		$row = $result->FetchRow();

	        $stagger_amount = cal_length($row["int_time"], $row["end_time"]);
	        $cur_sequential = $row["sequential"];
	        $job_id = $row["job_id"];
	        $int_date = $row["int_date"];
	        $int_time = $row["int_time"];
	        $end_time = $row["end_time"];
	        $sequential_position = $row["sequential"];
	
		for ($i = $sequential_position; $i >= 0; $i--)
		{
			$int_time = sub_times($int_time, $stagger_amount);
			$end_time = sub_times($end_time, $stagger_amount);
			$cur_sequential--;

			$sql = ("
				SELECT DISTINCT time_id
				FROM interview_time
				WHERE int_date='" . addslashes($int_date) . "'
				AND int_time='" . addslashes($int_time) . "'
				AND end_time='" . addslashes($end_time) . "'
				AND job_id='" . addslashes($job_id) . "'
				AND sequential='" . addslashes($cur_sequential) . "'
				");
			$result = $GLOBALS['dbh']->Execute($sql);
			if ($result->RecordCount() == 0)
			{
					/*
				 There are no more time_id's that correspond, so jump out and return the time_id array.
				*/
				break;
			}
			$row = $result->FetchRow();
			$time_ids .= ", '" . addslashes($row["time_id"]) . "'";
		}
		return $time_ids;
	}	

	function get_switch($time_id)
	{
		$time_ids = "'" . addslashes($time_id) . "'";

		if (($switching = isSwitchingInterview($this->job_id)) == T_ERROR)
		{
			$this->error("Query failed (or job could not be found).  The sign-up could not be "
							. " processed.  Please try again.");
			return false;
		}
						
		if ($switching)
		{
			// find corresponding switch time
			if (!$switch_time_id = getSwitchInterview($time_id))
			{
				$this->error("Query failed (or complement interview time could not be found). "
								. " The sign-up could not be processed.  Please try again.");
				return false;
			}
			
			// remove both
			$time_ids .= ", '" . addslashes($switch_time_id) . "'";
		}
		
		return $time_ids;	
	}

	
	function remove_phone_number($time_id, $phone, $message=true)
	{
		// remove interview_phone (don't forget any switch interviews)
				if (!$time_ids = $this->get_switch($time_id)) { return false; }

		/*
		 Find out if this is a sequential interview or not.  To do this we first need the interview type ID.
		*/

		$sql = ("
			SELECT DISTINCT a.interview_type_id
			FROM interview AS a, interview_time AS b
			WHERE b.time_id='" . $time_id . "'
			AND a.job_id=b.job_id
			");
		$tempresult = $GLOBALS['dbh']->Execute($sql);
		$temprow = $tempresult->FetchRow();
		
		if (isSequential($temprow["interview_type_id"]))
		{
			/*
			 Bang on the money!  It is a sequential interview.  We need to grab all of the other time_id's 
			 associated with this one so that we can remove the phone number from them as well.
			*/
	
			if (!$time_ids = $this->get_sequential($time_id)) 
			{
				return false;
			}
		}
		
		$result = $GLOBALS['dbh']->Execute("
			DELETE FROM interview_phone
			WHERE time_id IN (" . $time_ids . ")
			");
		
		if (!$result)
		{
			$this->error("Query failed.  Please check to make sure that the interview "
							. " was reverted to on campus.  If problem persists, please contact "
							. " the Co-op Office.");
			return false;
		}
		if ($message) { $this->notify("The interview has been reverted to on-campus."); }
		return true;
	}

	
	function set_phone_number($time_id, $phone)
	{
		// set phone number
		// (remember, the phone number may or may not be present already)
		
		if (!$this->remove_phone_number($time_id, $phone, false)) { return true; }
		
		if (!$time_ids = $this->get_switch($time_id)) { return false; }
	
		$sql = ("
			SELECT DISTINCT a.interview_type_id
			FROM interview AS a, interview_time AS b
			WHERE b.time_id='" . $time_id . "'
			AND a.job_id=b.job_id
			");
		$temp_result = $GLOBALS['dbh']->Execute($sql);
		$temp_row = $temp_result->FetchRow();
		if (isSequential($temp_row["interview_type_id"]))
		{
			if (!$time_ids = $this->get_sequential($time_id)) { return false; }
		}	
			
		$time_id = explode(",", $time_ids);
		
		for ($i = 0; $i < sizeof($time_id); $i++)
		{
			$sql = ("
				INSERT INTO interview_phone (time_id, phone)
				VALUES ('" . addslashes(preg_replace("/'(\d*)'/", "\\1", $time_id[$i])) . "', '" . addslashes($phone) . "')
				");
			$result = $GLOBALS['dbh']->Execute($sql);	

			if (!$result)
			{
				$this->error("Query failed.  Please check to make sure that the interview "
								. " phone number was set.  If problem persists, please contact "
								. " the Co-op Office.");
				return false;
			}
		}

		return true;
	}





	function update()
	{
		if ($this->action != "Save Changes") { return false; }
		if (!isInteger($this->num_form_lines))
		{
			$this->notify("Bad form data.  Please reload and try again.");
			return false;
		}
		if ($this->num_form_lines > MAX_FORM_LINES)
		{
			// Sarcastic remark for someone who attempts to send the system into a
			// really long loop.
			$error_message = "Why should I run in circles for a very long time?  I will never get anywhere.";
			$this->error($error_message);
			return false;
		}
	
		
		$row = $GLOBALS['dbh']->GetRow("SELECT GET_LOCK('$this->job_id', 10)");
		if ($row[0] != "1"){
			$this->error("Cannot get lock, please try again.");
			return false;
		}

		$row = $GLOBALS['dbh']->GetRow("SELECT MAX(date_edited) as last_mod FROM interview_edit WHERE job_id = '$this->job_id'");
		if ($row["last_mod"] > $this->last_edited){
			$this->error ("This interview was edited since you last loaded the page.  The modified interview is shown below - your changes have not been saved.");
			$GLOBALS['dbh']->GetRow("SELECT RELEASE_LOCK('$this->job_id')");
			return false;
		}
		
		$sql = "SELECT MAX(last_edited) as last_mod FROM signup_edit WHERE job_id = '$this->job_id' AND type='Student'";
		$row = $GLOBALS['dbh']->GetRow($sql);
		
		if ($row["last_mod"] > $this->last_modified ){
			$this->error ("A students has been modifying their signup for this interview since you last loaded the page.  Your changes have not been
					saved - they are shown below in bold.");
			$this->edit_conflict = 1;
			$GLOBALS['dbh']->GetRow("SELECT RELEASE_LOCK('$this->job_id')");
			return false;
		}
		$row = $GLOBALS['dbh']->GetRow("SELECT MAX(last_edited) as last_mod FROM signup_edit WHERE job_id = '$this->job_id' AND type='Admin'");
		if ($row["last_mod"] > $this->last_modified){
			$this->error ("Another administrator has been modifying the sign up since you last loaded this page.  Your changes have not been saved - they
					are shown below in bold.");
			$this->edit_conflict = 1;
			$GLOBALS['dbh']->GetRow("SELECT RELEASE_LOCK('$this->job_id')");
			return false;
		}
	
		for ($i = 0; $i < $this->num_form_lines; $i++)
 		{
			eval("global \$time_id_" . $i . ";");
 			eval ("\$time_id = \$time_id_" . $i . ";");
 			
 			if (isValidTimeID($time_id, $this->job_id, true, true))
 			{
 				eval("global \$student_num_" . $i . ";");
	 			eval ("\$student_num = \$student_num_" . $i . ";");
                $student_num = trim($student_num);
 				
                eval("global \$phone_" . $i . ";");
 				eval ("\$phone = \$phone_" . $i . ";");
 			
	 			eval("global \$del_" . $i . ";");
 				eval ("\$del = \$del_" . $i . ";");
 			    			
                if ($del && $student_num)
				{
					$this->error("Bad form data.  Please reload and try again.");
				}
				elseif ( !empty( $student_num ) )
				{ 
					$this->add_sign_up($time_id, $student_num);
				}
				elseif ($del)
				{
					$this->remove_sign_up($time_id);
				}
				else
				{
					$this->update_phone($time_id, $phone);
				}
 			}
 		}
		$GLOBALS['dbh']->Execute("REPLACE INTO signup_edit VALUES('$this->job_id', NULL, 'Admin')");
		$GLOBALS['dbh']->GetRow("SELECT RELEASE_LOCK('$this->job_id')");
	}	


	function update_phone($time_id, $phone)
	{
		$phone = trim($phone);

		/*
		 If this is a sequential interview, we need to get the phone number associated with the first in the
		 series of interviews.
		*/

		$sql = ("
			SELECT DISTINCT a.interview_type_id
			FROM interview AS a, interview_time AS b
			WHERE b.time_id='" . $time_id . "'
			AND a.job_id=b.job_id
			");
		$result = $GLOBALS['dbh']->Execute($sql);
		$row = $result->FetchRow();
		$interview_type_id = $row["interview_type_id"];
		
		if (isSequential($interview_type_id))
		{
			/*
			 It IS a sequential interview, so we need to get all of the associated time_ids, and then
			 choose whichever one of those is first in the sequential series (has it's sequential flag
			 equal to 0 in the database).
			*/
			
			$time_ids = $this->get_sequential($time_id);
			
			$sql = ("
				SELECT DISTINCT time_id
				FROM interview_time
				WHERE sequential='0'
				AND time_id IN (" . $time_ids . ")
				");
			$result = $GLOBALS['dbh']->Execute($sql);
			if (!$result)
			{
				$this->error("Query failed.  If any phone numbers were modified they may not be updated.");
				return false;
			}
			$row = $result->FetchRow();
			$first_time_id = $row["time_id"];
			
			/*
			 We now have the time_id that is first in the series for this set of time_ids.  We just need
			 to get the phone number for this time_id, and we're set.
			*/
			
			if ($first_time_id != $time_id)
			{
				$sql = ("
					SELECT DISTINCT phone
					FROM interview_phone
					WHERE time_id='" . addslashes($first_time_id) . "'
					");
				$phone_result = $GLOBALS['dbh']->Execute($sql);
				$phone_row = $phone_result->FetchRow();
				$phone = trim($phone_row["phone"]);
			}
		}
			
		$result = $GLOBALS['dbh']->Execute("
			SELECT	phone
			FROM		interview_phone
			WHERE		time_id='" . addslashes($time_id) . "'");		
			
		if (!$result)
		{
			$this->error("Query failed.  If any phone numbers were modified they may not be updated.");
			return false;
		}
		if ($row = $result->FetchRow())
		{
			$current_phone = $row["phone"];
			$interview_phone = true;
		}
		else
		{
			$current_phone = "";
			$interview_phone = false;
		}
		$result->Close();
	
		if ($phone != $current_phone)
		{
			if ($phone)
			{
				// set phone
				$this->set_phone_number($time_id, $phone);
			}
			else
			{
				// remove phone
				if ($first_time_id != $time_id)
				{
					$this->remove_phone_number($time_id, $phone, 0);
				}
				else
				{
					$this->remove_phone_number($time_id, $phone);
				}
			}
		}
	}
	
	
	
	function add_sign_up($time_id, $student_number)
	{
		/*
			validation - already guaranteed
			1. valid job_id
			
			validation - add only
			1. time_id<->job_id valid interview time with switch=0
			2. student_number is shortlisted
			3. student is not currently signed up for an interview for job_id
			4. time_id is currently empty
			5. time won't cause a time conflict for the student
		*/

		if (!isValidTimeID($time_id, $this->job_id))
		{
			// someone is probably URL hacking if they end up here.
			return false;
		}

		if (!isValidStudentNumber($student_number))
		{
			$this->notify("Bad form data.  A specified student number was not valid.");;
			return false;
		}
		
		if (!isStudentShortlisted($this->job_id, $student_number))
		{
			// student is not shortlisted for this interview
			// again, you probably have to be URL hacking to end up here.
			return false;
		}
		
		if (isStudentSignedUp($this->job_id, $student_number))
		{
			$this->notify("You must cancel " . getStudentName($student_number) . "'s current "
								 . " interview time before signing him/her up for a different time.");
			return false;
		}

		
		if (!isUnsignedTimeID($time_id))
		{
			$this->notify("You must cancel the current sign-up before signing a student "
								. " up for the same interview time.");
			return false;
		}
				
		
		if (!$datetime = getDateTime($time_id))
		{
			$this->error("Query failed.  The sign-up could not be processed.");
			return false;
		}
		$date = $datetime["date"];
		$time = $datetime["time"];

		
		if (($time_conflict = isTimeConflict($student_number, $time_id)) && ($time_conflict["return"] == T_ERROR))
		{
			$this->error($time_conflict["value"] . " The sign-up could not be processed.");
			return false;
		}
		if ($time_conflict["return"] == T_TRUE)
		{
			$this->notify("Could not sign-up " . getStudentName($student_number) . " for the "
								. formatStandardDate($date) . " " . formatTime($time, false)
								. " interview as it would cause a conflict with "
								. $time_conflict["value"] . ".");
			return false;
		}
				
		
		if (($switching = isSwitchingInterview($this->job_id)) == T_ERROR)
		{
			$this->error("Query failed (or job could not be found).  The sign-up could not be "
							. " processed.");
			return false;
		}
						
		$sql = ("
                        SELECT DISTINCT interview_type_id
                        FROM interview
                        WHERE job_id='" . addslashes($this->job_id) . "'
                        ");
                $temp_result = $GLOBALS['dbh']->Execute($sql);
                $temp_row = $temp_result->FetchRow();
                $temp_result->Close();

                if (isSequential($temp_row["interview_type_id"]))
                {
                        /*
                         We need to add students differently if we're dealing with sequential interviews.
                        */

                        if (!$time_ids = $this->get_sequential($time_id))
                        {
                                return false;
                        }

                        $sql = ("
                                UPDATE interview_time
                                SET student_number='" . addslashes($student_number) . "'
                                WHERE time_id IN (" . $time_ids . ")
                                AND (student_number='' OR ISNULL(student_number))
                                ");
                        $result = $GLOBALS['dbh']->Execute($sql);
                        if (!$result || !$GLOBALS['dbh']->Affected_Rows())
                        {
                                $this->error("Query failed (or job could not be found).  The sign-up could not be "
                                . "  processed.");
                                return false;
                        }
                        $this->notify(getStudentName($student_number) . " was signed up for the " . formatStandardDate($date) . " " . formatTime($time, false) . " interview.");
                }
		else
		{
			if (!$time_ids = $this->get_switch($time_id)) { return false; }

			$result = $GLOBALS['dbh']->Execute("
				UPDATE	interview_time
				SET		student_number='" . addslashes($student_number) . "'
				WHERE		time_id IN (" . $time_ids . ") AND
							(student_number='' OR ISNULL(student_number))");
			if (!$result || !$GLOBALS['dbh']->Affected_Rows())
			{
				$this->error("Query failed.  Sign-up may not have occurred (please check).");
				return false;
			}
			$this->notify(getStudentName($student_number) . " was signed up for the " . formatStandardDate($date) . " " . formatTime($time, false) . " interview.");
		}
	}


	
	
	function remove_sign_up($time_id)
	{
		/*
			validation - already guaranteed
			1. valid job_id
			
			validation - add only
			1. time_id<->job_id valid interview time with switch=0
			2. time_id is currently not empty
		*/
				
		if (!isValidTimeID($time_id, $this->job_id))
		{
			// someone is probably URL hacking if they end up here.
			return false;
		}

		if (isUnsignedTimeID($time_id))
		{
			// you can't cancel a non-existent interview time
			return false;
		}
		
		if (!$datetime = getDateTime($time_id))
		{
			$this->error("Query failed.  The sign-up could not be processed.  Please try again.");
			return false;
		}
		$date = $datetime["date"];
		$time = $datetime["time"];

		$sql = ("
                        SELECT DISTINCT interview_type_id
                        FROM interview
                        WHERE job_id='" . addslashes($this->job_id) . "'
                        ");
                $temp_result = $GLOBALS['dbh']->Execute($sql);
                $temp_row = $temp_result->FetchRow();
                $temp_result->Close();


                if (isSequential($temp_row["interview_type_id"]))
                {
                        /*
                         We need to remove students differently if we're dealing with sequential interviews.
                        */

                        if (!$time_ids = $this->get_sequential($time_id))
                        {
                                return false;
                        }

                        $sql = ("
                                UPDATE interview_time
                                SET student_number=NULL
                                WHERE time_id IN (" . $time_ids . ")
                                ");
                        $result = $GLOBALS['dbh']->Execute($sql);

                        if (!$result || !$GLOBALS['dbh']->Affected_Rows())
                        {
                                $this->error("Query failed.  The timeslot(s) may not have been cleared (please check).");
                                return false;
                        }
                        $this->notify("The " . formatLongDate($date) . " " . formatTime($time, false)
                        . " timeslot has been successfully cleared.");

                        $sql = ("
                                DELETE FROM interview_phone
                                WHERE time_id IN (" . $time_ids . ")
                                ");
                        $result = $GLOBALS['dbh']->Execute($sql);

                }
                else
                {
			if (!$time_ids = $this->get_switch($time_id)) { return false; }

			$result = $GLOBALS['dbh']->Execute("
				UPDATE	interview_time
				SET		student_number=NULL
				WHERE		time_id IN (" . $time_ids . ")");
			if (!$result || !$GLOBALS['dbh']->Affected_Rows())
			{
				$this->error("Query failed.  The timeslot(s) may not have been cleared (please check).");
				return false;
			}
			$this->notify("The " . formatLongDate($date) . " " . formatTime($time, false)
								. " timeslot has been successfully cleared.");
			
			$result = $GLOBALS['dbh']->Execute("
				DELETE FROM interview_phone
				WHERE		time_id IN (" . $time_ids . ")");
		}
	}

}





?>
