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

include('misc/isEmailSent.inc');
$interview = new interview($job_id, $student_number);
$interview->display();



class interview
{
	var $error_output;
	var $notify_output;
	var $output = "";

	var $job_id;
	var $student_number;
	
	var $page;
	var $action;
	var $time_id;
	var $confirm;
	var $phone;
		
	var $editable=false;

	var $job_code; // not for general use				
	
	function interview($job_id, $student_number)
	{
		global $page;
		global $action;
		global $time_id;
		global $confirm;
		global $phone;
		$this->page = $page;
		$this->action = $action;
		$this->time_id = $time_id;
		$this->confirm = $confirm;
		$this->phone = $phone;
						
		$this->student_number = $student_number;
		$this->job_id = $job_id;

		/* 
		$this->editable should be set before each check against
		an interview in the database, to allow for the possibility
		of an interview spanning more than one date.  
		
		$this->editable = isJobIDStudentEditable($this->job_id, $this->student_number);
		*/
		$this->editable = true;
	}

	
	function display()	
	{
		$this->generate();
		
		if ($this->error_output) { echo(error("<h1 class='black'>Error</h1>" . $this->error_output)); }
		if ($this->notify_output) { echo(notify($this->notify_output)); }
		echo($this->output);
	}
		
	
	function generate()
	{
		if (!$this->editable) { $page = "main"; }
		
		switch ($this->page)
		{
			case "phone":
				$this->generate_phone();
				break;
			
			case "main":
			default:
				$this->generate_main();
				break;
		}
	}
	
	function error($error_text)
	{
		$this->error_output .= "<p>" . $error_text . "</p>";
	}

	function notify($notify_text)
	{
		$this->notify_output .= "<p>" . $notify_text . "</p>";
	}

	
	function generate_phone()
	{
		$this->generate_main_head();
		$this->generate_main_job_info();

		if (!$this->generate_phone_page())
		{
			$this->generate_main_interview_details();
		}
				
		$this->generate_main_presentation_info();
		$this->generate_main_notes();
		$this->generate_main_schedule();
		$this->generate_main_conflict();
		if ($display) {
			$this->generate_main_shortlist();
		}
	}

	function generate_main()
	{
		$this->generate_main_head();
		$this->generate_main_job_info();

		$this->add_or_remove_sign_up();
				
		$this->generate_main_interview_details();
		$this->generate_main_presentation_info();
		$this->generate_main_notes();
		$this->generate_main_schedule();
		$this->generate_main_conflict();
		if ($display) {
			$this->generate_main_shortlist();
		}
	}
	
	function generate_main_head()
	{
		$this->output .= "\n\n<!--main_head-->\n";
		$this->output .= "<h3>Interview Schedule</h3>\n";
	}

	function generate_main_job_info()
	{
		$this->output .= "\n\n<!--main_job_info-->\n";
		
		$result = $GLOBALS['dbh']->Execute("
			SELECT	a.job_code, a.department_id, b.company_name, c.cancelled
			FROM		job_info AS a, employer_company AS b, interview AS c
			WHERE		a.job_id='" . addslashes($this->job_id) . "' AND a.job_id = c.job_id AND
						a.employer_id=b.employer_id");

		if (!$result) {
			$this->error("Query failed.");
		}
		else
		{
			if ($row = $result->FetchRow())
			{
				$company_name = $row["company_name"];
				$job_code = $row["job_code"];
				$this->job_code = $job_code;
				$cancelled = $row["cancelled"];
				$department_d = $row["department_id"];
			}
			$result->Close();
		}
		if ($cancelled){
			$this->output .= "<h2><b class='red'>This intervew has been cancelled.</b></h2>\n";
		}
		$this->output .= "<table cellpadding='0' cellspacing='0'>\n";
		$this->output .= "	<tr>\n";
		$this->output .= "		<td align='right'><b>Company:</b>&nbsp;</td><td>" . $company_name . "</td>\n";
		$this->output .= "	</tr>\n";
		$this->output .= "	<tr>\n";
		$this->output .= "		<td align='right'><b>Job Code:</b>&nbsp;</td><td>" . $job_code . "</td>\n";
		$this->output .= "	</tr>\n";
		$this->output .= "</table>\n";
	}

	function generate_main_interview_details()
	{
		global $PHP_SELF;
		$this->output .= "\n\n<!--main_interview_details-->\n";

		$result = $GLOBALS['dbh']->Execute("
			SELECT   m.description as medium, a.interview_type_id, p.description as place, b.int_date,

			         CONCAT(c2.building_name, ' ', c.room_number) AS interview_room,
			         CONCAT(d2.building_name, ' ', d.room_number) AS wait_room,
			         e.phone AS phone_number,
			         f.street_address_1, f.street_address_2, f.city, f.province, f.country, f.directions
			FROM     interview AS a, interview_medium as m, interview_place as p
			LEFT OUTER JOIN   interview_time AS b
			ON       a.job_id=b.job_id AND
			         b.student_number='" . addslashes($this->student_number) . "' AND
			         b.switch=0
			LEFT OUTER JOIN   room AS c
			ON       b.int_room_id=c.room_id
			LEFT OUTER JOIN   building AS c2
			ON       c.building_id=c2.building_id
			LEFT OUTER JOIN   room AS d
			ON       b.int_wait_room_id = d.room_id
			LEFT OUTER JOIN   building AS d2
			ON       d.building_id=d2.building_id
			LEFT OUTER JOIN   interview_phone AS e
			ON       b.time_id=e.time_id
			LEFT OUTER JOIN   interview_location AS f
			ON       a.job_id=f.job_id
			WHERE    a.job_id='" . $this->job_id . "' AND m.interview_medium_id = a.interview_medium_id
				AND p.interview_place_id = a.interview_place_id");
 	
		if (!$result) {
			$this->error("Query failed.");
		}
		else
		{
			if (!$row = $result->FetchRow())
			{
				$this->error("Could not find interview information.  Try reloading.  If the error "
						. "persists please contact the Co-op Office for interview details.");
			}
			else
			{
				$interview_room = $row["interview_room"];
				$wait_room = $row["wait_room"];
				if (!$interview_room) { $interview_room = "TBA"; }
				if (!$wait_room) { $wait_room = "TBA"; }
				
				$this->output .= "<table class='backgrey' cellpadding='5' width='100%'><tr><td>\n";
				$this->output .= "<h4>Interview Details:</h4>\n";
				
				//added emily feb 12
				if(isSwitching($row["interview_type_id"])){
					$this->output .= "<p>This is a switching interview.  For the first half of your
								interview you will be interviewed by the interviewer
								you sign up with.  For the second half of the interview you
								will be interviewed by the second interviewer.</p>\n";
				}
				if(isSequential($row["interview_type_id"]))
				{
					$this->output .= "<p>This is a sequential interview.  You will be required to
								see multiple interviewers sequentially.  Ie.  You will
								first go and see Interviewer A, then Interviewer B, then
								Interviewer C, etc..  You need only sign up for one slot.
							</p>\n";
				}
				
				if ($row["place"] == ONCAMPUS)	// on-campus
				{
					if ($row["medium"]==PHONE)	// oncampus phone interview
					{						
						$this->output .= "<p>This is an on-campus phone interview.</p>\n";
					}
					else	// oncampus non-phone interview
					{
						$this->output .= "<p>This is an on-campus interview.</p>\n";
					}
					$this->output .= "<div align='center'>\n";
					$this->output .= "<table cellpadding='0' cellspacing='0'>\n";
//					$this->output .= "<tr><td align='right'>Interview Room:&nbsp;</td><td>" . $interview_room . "</td></tr>\n";
					$this->output .= "<tr><td align='right'>Wait Room:&nbsp;</td><td>" . $wait_room . "</td></tr>\n";
					$this->output .= "</table>\n";
					$this->output .= "</div>\n";
				}
				else	// off-campus
				{
					if ($row["medium"] == PHONE)
					{
						// phone interview with on-campus/off-campus option
						$this->output .= "<p>This is a phone interview.  You may either have the "
												. " interview on-campus or at a phone number you specify. "
												. "  You must be signed up for a timeslot before you can "
												. " specify a phone number.</p>\n";
						if ($row["phone_number"])	// phone_number specified
						{
							$this->output .= "<div align='center'>\n";
							$this->output .= "<table cellpadding='0' cellspacing='0'>\n";
							$this->output .= "<tr><td align='right'>Phone Number:&nbsp;</td><td>" . $row["phone_number"] . "</td></tr>\n";
							$this->output .= "</table>\n";

	/*
	Changed code right after comment block.  Old if statement was
	if($this->editable).  Changed by Adam on 2001-01-25.
	*/

	if (isJobIDStudentEditable($this->job_id, $this->student_number, $row["int_date"])) {
								$this->output .= "<form action='" . $PHP_SELF . "' method='post'>\n";
								$this->output .= "<input type='hidden' name='select' value='student_sign_up' />\n";
								$this->output .= "<input type='hidden' name='job_id' value='" . $this->job_id . "' />\n";
								$this->output .= "<input type='hidden' name='page' value='phone' />\n";
								$this->output .= "<input type='submit' name='action' value='On Campus' />\n";
								$this->output .= "<input type='submit' name='action' value='Change Phone Number' />\n";
								$this->output .= "</form>\n";
							}
							$this->output .= "</div>\n";
						}
						else	// phone_number not specified
						{
							$this->output .= "<div align='center'>\n";
							$this->output .= "<table cellpadding='0' cellspacing='0' align='center'>\n";
//							$this->output .= "<tr><td align='right'>Interview Room:&nbsp;</td><td>" . $interview_room . "</td></tr>\n";
							$this->output .= "<tr><td align='right'>Wait Room:&nbsp;</td><td>" . $wait_room . "</td></tr>\n";
							$this->output .= "</table>\n";

	/*
	Changed code below comment block.  Old if statement was
	if($this->editable).  Changed by Adam on 2001-01-25.
	*/
							if (isJobIDStudentEditable($this->job_id, $this->student_number, $row["int_date"])) {
								if (isStudentSignedUp($this->job_id, $this->student_number))
								{
									$this->output .= "<form action='" . $PHP_SELF . "' method='post'>\n";
									$this->output .= "<input type='hidden' name='select' value='student_sign_up' />\n";
									$this->output .= "<input type='hidden' name='job_id' value='" . $this->job_id . "' />\n";
									$this->output .= "<input type='hidden' name='page' value='phone' />\n";
									$this->output .= "<input type='submit' name='action' value='Specify Phone Number' />\n";
									$this->output .= "</form>\n";
								}
							}
							$this->output .= "</div>\n";
						}
					}
					else
					{
						// off-campus: use interview_location address
						$this->output .= "<p>This is an off-campus interview.</p>\n";
						$this->output .= "<table cellpadding='0' cellspacing='0'>\n";
						if ($row["street_address_1"] && $row["city"])
						{	
							$this->output .= "<tr><td valign='top' align='right'>Interview Location:&nbsp;</td>";
							$this->output .= "<td>";
							$this->output .= $row["street_address_1"];
							if ($row["street_address_2"]) { $this->output .= "<br />" . $row["street_address_2"]; }
							$this->output .= "<br />" . $row["city"];
							if ($row["province"]) { $this->output .= "<br />" . $row["province"]; }
							if ($row["country"]) { $this->output .= "<br />" . $row["country"]; }
							$this->output .= "</td>";
							$this->output .= "</tr>\n";
							if ($row["directions"]) { $this->output .= "<tr><td valign='top'>Directions:&nbsp;</td><td>" . $row["directions"] . "</td></tr>\n"; }
						}
						else
						{
							$this->output .= "<tr><td align='right' valign='top'>Interview Location:&nbsp;</td><td>Please check with the Co-op Office as soon as possible.</td></tr>\n";
						}
						$this->output .= "</table>\n";
					}
				}
				$this->output .= "</td></tr></table>\n";			
			}
			$result->Close();
		}
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
				$this->output .="<br />";
				$this->output .= "<table class='backgrey' cellpadding='5' width='100%'><tr><td>\n";
				$this->output .= "<h4>*** EMPLOYER INFO SESSION ***</h4>\n";
			
				$this->output .= "<table cellspacing='0' cellpadding='0'>\n";
				$this->output .= "<tr><td align='right'>Date:&nbsp;</td><td>" . formatLongDate($row["pres_date"]) . "</td></tr>\n";
				$this->output .= "<tr><td align='right'>Time:&nbsp;</td><td>" . formatTime($row["pres_time"], false) . " to " . formatTime($row["end_time"], false) . "</td></tr>\n";
				$this->output .= "<tr><td align='right'>Location:&nbsp;</td><td>" . $row["pres_loc"] . "</td></tr>\n";
				if ($row["pres_notes"])
				{
					$this->output .= "<tr><td valign='top' align='right'>Notes:&nbsp;</td><td valign='top'>" . $row["pres_notes"] . "</td></tr>\n";
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
		$result = $GLOBALS['dbh']->Execute("SELECT notes FROM interview WHERE job_id='" . addslashes($this->job_id) . "'");
		if (!$result)
		{
			$this->error("Query failed.  (Any notes for this interview will not be displayed.)");
		}
		else
		{
			if (($row = $result->FetchRow()) && ($notes = $row["notes"]))
			{
				$this->output .= "<table class='backgrey' cellpadding='5' width='100%'><tr><td>\n";
				$this->output .= "<h4>Notes:</h4>\n";
				$this->output .= "<p>" . $notes . "</p>\n";
				$this->output .= "</td></tr></table>\n";
			}
		}
	}
	
	function generate_main_schedule()
	{
		$this->output .= "\n\n<!--main_schedule-->\n";
		$this->output .= "<a name='schedule'></a>\n";

		global $PHP_SELF;		

		$job_id = $this->job_id;
		$student_number = $this->student_number;
		$signed_up = isStudentSignedUp($job_id, $student_number);
		$cancelled = isJobCancelled($job_id);
		
		$result = $GLOBALS['dbh']->Execute("SELECT i.interview_type_id
					FROM interview as i, interview_type as t
					WHERE i.interview_type_id =  t.interview_type_id AND job_id = '" . addslashes($job_id) . "'");
		$row = $result->FetchRow();
		$isswitching = 0;
		if (isSwitching($row["interview_type_id"])){
			$isswitching = 1;
		}
		if (isSequential($row["interview_type_id"]))
		{
			$issequential = 1;
		}
		
		$result = $GLOBALS['dbh']->Execute("
			SELECT DISTINCT a.int_date, a.interviewer_id, b.interviewer_name
			FROM interview_time AS a, interview_interviewer AS b
			WHERE a.job_id='" . addslashes($job_id) . "' 
			AND a.interviewer_id=b.interviewer_id
			AND a.sequential='0'
			ORDER BY a.int_date, b.interviewer_name");

		while ($row = $result->FetchRow())
		{
			
		//Newly inserted code below, this is to account for 
		//interviews that span multiple days.  Added by 
		//Adam on 2001-01-25.

			$this->editable = isJobIDStudentEditable($this->job_id, $this->student_number, $row["int_date"]);
		
		//End of new code.

			$this->output .= "<table>\n";
			
			if ($issequential)
			{
				/*
		 		 In the case of a sequential interview, we want to display all of the interviewers names at once.
		 		 The below query facilitates this.
				*/
				
				$sql = ("
					SELECT DISTINCT a.interviewer_id, b.interviewer_name
					FROM interview_time AS a, interview_interviewer AS b
					WHERE a.job_id='" . addslashes($job_id) . "'
					AND a.interviewer_id=b.interviewer_id
					ORDER BY a.sequential
					");
				$seq_result = $GLOBALS['dbh']->Execute($sql);
				while ($seq_row = $seq_result->FetchRow())
				{
					$seq_names[] = $seq_row["interviewer_name"];
				}
				for ($i = 0; $i < (sizeof($seq_names) - 1); $i++)
				{
					$seq_int .= $seq_names[$i] . ", ";
				}
				$seq_int .= "and " . $seq_names[$i];

				$this->output .= "<tr><td><b>Interviewers</b>:</td><td>" . $seq_int . "</td></tr>\n";

				unset($seq_names);
				unset($seq_int);
				$seq_result->Close();
			}
			else
			{
				$this->output .= "<tr><td><b>Interviewer:</b></td><td>" . $row["interviewer_name"] . "</td></tr>\n";
			}
			$this->output .= "<tr><td><b>Date:</b></td><td>" . formatLongDate($row["int_date"]) . "</td></tr>\n";
			$this->output .= "</table>\n";

			$sql ="
				SELECT 	job_id, switch, time_id, int_time, b.student_number, end_time,
							ELT(int_type, 'interview', 'break', 'lunch') AS int_type,
							CONCAT(b.first_name, ' ', b.last_name) AS student_name
				FROM	interview_time
				LEFT OUTER JOIN	student AS b
				ON		interview_time.student_number=b.student_number
				WHERE	int_date='" . $row["int_date"] . "' AND
						interviewer_id='" . $row["interviewer_id"] . "' AND
						job_id='" . addslashes($job_id) . "' AND switch=0 
				ORDER BY int_time";
			
			$result_times = $GLOBALS['dbh']->Execute($sql);	
			$this->output .= "<table cellspacing='0' cellpadding='0' border='1'><tr><td>\n";
			$this->output .= "	<table border='0' cellpadding='2'>\n";
			$this->output .= "		<tr>\n";
			$this->output .= "			<td class='rowgrey' align='center'>&nbsp;<b class='white'>Interview Time</b>&nbsp;</td>\n";
			$this->output .= "			<td class='rowgrey' align='center'>&nbsp;<b class='white'>Status</b>&nbsp;</td>\n";
			$this->output .= "		</tr>\n";
			
			while ($row_times = $result_times->FetchRow())
			{
				switch($row_times["int_type"])
				{
					// display var added so students cannot view other student names in interview.
					case "interview":
						//added emily feb 12
						if ($isswitching){
							$interview_length = cal_length($row_times["int_time"], $row_times["end_time"]);
							$end_time = add_times($row_times["int_time"], $interview_length * 2);
						}
						elseif ($issequential)
						{
							$end_time = getSequentialEndTime($row_times["time_id"]);
						}
						else
						{
							$end_time = $row_times["end_time"];
						}
						//

						$student_num = $row_times["student_number"];
						if ($this->editable && !$cancelled && (($student_num == "" && !$signed_up) || ($student_num == $student_number)) )
						{
							$link = $PHP_SELF . "&amp;select=student_sign_up&amp;job_id=" . $job_id . "&amp;time_id=" . $row_times["time_id"] . (($student_num == $student_number) ? "&amp;action=remove" : "&amp;action=add");
							$anchor = "<a class='blue' href=\"" . $link . "\">" . formatTime($row_times["int_time"]) . "&nbsp;-&nbsp;" . formatTime($end_time) .  "</a>";
							$display = 1;
						}
						else
						{
							$display = 0;
							$anchor = formatTime($row_times["int_time"]) . "&nbsp;-&nbsp;" . formatTime($end_time);
							if ($student_num == "" || $student_num == $student_number) {
								$display = 1;
							} else {
								$display = 2;
							}
						}
							if ($display == 1) {
									if ($row_times["student_name"] == "") {
										$slot = "<font color='#CCCCCC'>Available</font>";
									} else { $slot = $row_times["student_name"]; }
								$this->output .= "		<tr>\n";
								$this->output .= "			<td class='row0d' align='center'>" . $anchor . "</td>\n";
								$this->output .= "			<td class='row0d'><b>&nbsp;" . $slot ."&nbsp;</b></td>\n";
								$this->output .= "		</tr>\n";
							   $display = 0;
							} elseif ($display == 2) {
								$this->output .= "		<tr>\n";
								$this->output .= "			<td class='row0d' align='center'>" . $anchor . "</td>\n";
								$this->output .= "			<td class='row0d'>&nbsp;Occupied&nbsp;</td>\n";
								$this->output .= "		</tr>\n";
							  $display = 0;
							}
							break;
				
					case "break":
						if ($display) {  // dont show breaks to students
							if (!$issequential)
							{
								$this->output .= "		<tr><td class='row1d' align='center'>" . formatTime($row_times["int_time"]) . "&nbsp;-&nbsp;" . formatTime($row_times["end_time"]) . "</td>\n";
								$this->output .= "			<td class='row1d'>&nbsp;Break&nbsp;</td>\n";
								$this->output .= "		</tr>\n";
							}
						}
						break;
				
					case "lunch":
						if ($display) { // dont show lunches to students, they may get confused.
							if (!$issequential)
							{
								$this->output .= "		<tr><td class='row1d' align='center'>" . formatTime($row_times["int_time"]) . "&nbsp;-&nbsp;" . formatTime($row_times["end_time"]) . "</td>\n";
								$this->output .= "			<td class='row1d'>&nbsp;Lunch&nbsp;</td>\n";
								$this->output .= "		</tr>\n";
							}
						}
						break;
				}
			}
		
			$this->output .= "	</table>\n";
			$this->output .= "</td></tr></table>\n";
		}
	}

	
	
	
		
	function generate_main_conflict()
	{
		$this->output .= "\n\n<!--main_conflict-->\n";

		$result = $GLOBALS['dbh']->Execute("
			SELECT DISTINCT b.department_name, CONCAT(c.first_name, ' ', c.last_name) AS contact_name,
					c.email, c.phone
			FROM	interview AS a, department AS b, job_info as d
			LEFT OUTER JOIN contact AS c
			ON		a.contact=c.contact_id
			WHERE	d.department_id=b.department_id AND d.job_id = a.job_id AND d.job_id='" . addslashes($this->job_id) . "'");

		if ($result)
		{
			if ($row = $result->FetchRow())
			{
				if ($contact = $row["contact_name"])
				{
					if ($email = $row["email"]) { $contact = "<a href='mailto:" . $email ."?subject=" . urlencode($this->job_code) . "'>" . $contact . "</a> &lt;" . $email. "&gt;"; }
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
		
		$this->output .= "<p>If you have a conflict with all available interview times, please do NOT sign up for an interview.  Instead, please contact <b>$contact</b>.</p>";
	}

		
	function generate_main_shortlist()
	{
		$this->output .= "\n\n<!--main_shortlist-->\n";

		$result = $GLOBALS['dbh']->Execute("
		   SELECT CONCAT(a.first_name, ' ', a.last_name) AS student_name
		   FROM student AS a, students_shortlisted AS b
		   WHERE b.job_id='" . addslashes($this->job_id) . "' AND a.student_number=b.student_number
		   ORDER BY a.last_name, a.first_name");

		if (!$result) { return false; }
		
		if ($result->RecordCount())
		{
			$this->output .= "<table cellspacing='0' cellpadding='0' border='1'><tr><td>\n";
			$this->output .= "<table border='0' cellpadding='2'>\n";
			$this->output .= "<tr>\n";
			$this->output .= "<td class='rowgrey' align='center'>&nbsp;<b class='white'>Students shortlisted</b>&nbsp;</td>\n";
			$this->output .= "</tr>\n";

            while ($row = $result->FetchRow()) 
            {
				$this->output .= "<tr><td class='" . (($rowcount % 2) ? "row0d" : "row1d") . "'>&nbsp;" . $row['student_name'] . "&nbsp;</td></tr>\n";
			}

			$this->output .= "</table>\n";
			$this->output .= "</td></tr></table>\n";
		}
		
		$result->Close();
	}

	
	
	
	function add_or_remove_sign_up()
	{
		global $userlevel;
		if (($this->action != "add") && ($this->action != "remove"))
		{
			// nothing to do
			return false;
		}
		if ($userlevel == ADMINSTUDENT){
			$this->error("You are not allowed to edit a student's information from here.  You must log out of the student section and edit it using Modify Sign-Up.");
			return false;
		}
		/*
			validation - already guaranteed
			1. valid job_id
			2. student_number is shortlisted
			3. job_id isshowable
		*/

		/*
			validation - add AND remove
			1. job is editable
			2. time_id<->job_id valid interview time with switch=0
		*/

		/*
		Fix written in here to account for jobs that have multiple 
		days.  Now, rather than simply checking if the job is editable
		based on variable set at start, the date for the given 
		interview is pulled from database, and is then used to check
		if the JobID is editable by students.  Added by Adam on
		2001-01-25.
		
		NEW CODE BELOW
		*/

		$result_date = $GLOBALS['dbh']->Execute("
			SELECT int_date
			FROM interview_time
			WHERE time_id='" . addslashes($this->time_id) . "'
			");
		if(!$result_date)
		{
			return false;
		}
		$row = $result_date->FetchRow();
		$result_date->Close();
		if (!isJobIDStudentEditable($this->job_id, $this->student_number, $row["int_date"]))
		{
			$this->notify("This job is now locked.  Changes cannot be made in the schedule.");
			return false;
		}
		if (!isValidTimeID($this->time_id, $this->job_id))
		{
			// someone is probably URL hacking if they end up here.
			return false;
		}
			
		
		if ($this->action == "add") { $this->add_sign_up(); }
		else { $this->remove_sign_up(); }
	}




	
	function add_sign_up()
	{
		/*
			validation - already guaranteed
			1. valid job_id
			2. student_number is shortlisted
			3. job_id isshowable
			4. job is editable
			5. time_id<->job_id valid interview time with switch=0
			
			validation - add only
			1. student is not currently signed up for an interview for job_id
			2. time_id is currently empty
			3. time won't cause a time conflict for the student
		*/
		
		if (isStudentSignedUp($this->job_id, $this->student_number))
		{
			$this->notify("You must cancel your current interview time (by clicking on it) before "
								 . " you sign up for a different time.");
			return false;
		}

		
		if (!isUnsignedTimeID($this->time_id))
		{
			$this->notify("Select an <b>empty</b> timeslot.");
			return false;
		}
				
		
		if (!$datetime = getDateTime($this->time_id))
		{
			$this->error("Query failed.  The sign-up could not be processed.  Please try again. "
								. " If this problem persists please contact the Co-op Office.");
			return false;
		}
		$date = $datetime["date"];
		$time = $datetime["time"];

		
		if (($time_conflict = isTimeConflict($this->student_number, $this->time_id)) && ($time_conflict["return"] == T_ERROR))
		{
			$this->error($time_conflict["value"] . " The sign-up could not be processed.  Please try again. "
							. " If this problem persists please contact the Co-op Office.");
			return false;
		}
		if ($time_conflict["return"] == T_TRUE)
		{
			$this->notify("Could not sign-up for this time as it would cause a conflict with "
							. $time_conflict["value"] . ".  If all open times are conflicting "
							. " please contact the Co-op Office.");
			return false;
		}
				
		
		if (($switching = isSwitchingInterview($this->job_id)) == T_ERROR)
		{
			$this->error("Query failed (or job could not be found).  The sign-up could not be "
							. " processed.  Please try again.  If this problem persists, please "
							. " contact the Co-op Office.");
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
		
			if (!$time_ids = $this->get_sequential($this->time_id))
			{
				return false;
			}
			
			$sql = ("
				UPDATE interview_time
				SET student_number='" . addslashes($this->student_number) . "'
				WHERE time_id IN (" . $time_ids . ")
				AND (student_number='' OR ISNULL(student_number))
				");
			$result = $GLOBALS['dbh']->Execute($sql);
			if (!$result || !$GLOBALS['dbh']->Affected_Rows())
			{
				$this->error("Query failed.  Sign-up may not have occurred (please check)."
				. "  If problem persists, please contact the Co-op office.");
				return false;
			}
			$this->notify("You were signed up for the " . formatLongDate($date) . " "
			. formatTime($time, false) . " interview.");
		}	
		else
		{
			if (!$time_ids = $this->get_switch($this->time_id)) { return false; }

			$result = $GLOBALS['dbh']->Execute("
				UPDATE	interview_time
				SET		student_number='" . addslashes($this->student_number) . "'
				WHERE		time_id IN (" . $time_ids . ") AND
							(student_number='' OR ISNULL(student_number))");
			if (!$result || !$GLOBALS['dbh']->Affected_Rows())
			{
				$this->error("Query failed.  Sign-up may not have occurred (please check).  "
								. "If problem persists, please contact the Co-op office.");
				return false;
			}
			$this->notify("You were signed up for the " . formatLongDate($date) . " " . formatTime($time, false) . " interview.");
		}
		$GLOBALS['dbh']->Execute("REPLACE INTO signup_edit (job_id, last_edited, type) VALUES ('$this->job_id', NULL, 'Student')");
	}

	
	
	function remove_sign_up()
	{
		/*
			validation - already guaranteed
			1. valid job_id
			2. student_number is shortlisted
			3. job_id isshowable
			4. job is editable
			5. time_id<->job_id valid interview time with switch=0
		*/
				
		if (!$datetime = getDateTime($this->time_id))
		{
			$this->error("Query failed.  The sign-up could not be processed.  Please try again. "
								. " If this problem persists please contact the Co-op Office.");
			return false;
		}
		$date = $datetime["date"];
		$time = $datetime["time"];

		
		// has remove been confirmed
		if (!$this->confirm)
		{
			$this->remove_confirm($date, $time);
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
			 We need to remove students differently if we're dealing with sequential interviews.
			*/
		
			if (!$time_ids = $this->get_sequential($this->time_id))
			{
				return false;
			}
			
			$sql = ("
				UPDATE interview_time
				SET student_number=NULL
				WHERE time_id IN (" . $time_ids . ")
				AND (student_number='" . addslashes($this->student_number) . "')
				");
			$result = $GLOBALS['dbh']->Execute($sql);

			if (!$result || !$GLOBALS['dbh']->Affected_Rows())
			{
				$this->error("Query failed.  Sign-up may not have been removed (please check)."
				. "  If problem persists, please contact the Co-op office.");
				return false;
			}
			$this->notify("Your sign-up for the " . formatLongDate($date) . " "
			. formatTime($time, false) . " timeslot has been cleared.  Please select another time.");
			
			$sql = ("
				DELETE FROM interview_phone
				WHERE time_id IN (" . $time_ids . ")
				");
			$result = $GLOBALS['dbh']->Execute($sql);
			
			global $job_id;
			if (isEmployerEmailSent($job_id))
			{
				$reason = "remove student";
				include('interview/sign_up/student/send_secretary_email.inc');
				unset($reason);
			}
		}
		else
		{
		
				
			if (!$time_ids = $this->get_switch($this->time_id)) { return false; }
	
			$result = $GLOBALS['dbh']->Execute("
				UPDATE	interview_time
				SET		student_number=NULL
				WHERE		time_id IN (" . $time_ids . ") AND
							student_number='" . addslashes($this->student_number) . "'");
			if (!$result || !$GLOBALS['dbh']->Affected_Rows())
			{
				$this->error("Query failed.  Sign-up may not have been removed (please check).  "
								. "If problem persists, please contact the Co-op office.");
				return false;
			}
			$this->notify("Your sign-up for the " . formatLongDate($date) . " " . formatTime($time, false)
								. " timeslot has been cleared.  Please select another time.");
			
			$result = $GLOBALS['dbh']->Execute("
				DELETE FROM interview_phone
				WHERE		time_id IN (" . $time_ids . ")");
	
			global $job_id;
			if (isEmployerEmailSent($job_id))
			{
				$reason = "remove student";
				include('interview/sign_up/student/send_secretary_email.inc');
				unset($reason);
			}
		}
		$GLOBALS['dbh']->Execute("REPLACE INTO signup_edit (job_id, last_edited, type) VALUES ('$this->job_id', NULL, 'Student')");
	}

	
	
	function remove_confirm($date, $time)
	{
		global $PHP_SELF;
		$this->output .= "<table class='error' cellpadding='5' width='100%'><tr><td>\n";
		$this->output .= "<h4>Remove Sign-up Confirmation:</h4>\n";
		$this->output .= "<p>Are you sure you want to cancel your sign-up for the " . formatLongDate($date) . " " . formatTime($time, false) . " timeslot?</p>\n";
		$this->output .= "<table cellpadding='0' cellspacing='0' align='center'>\n";
		$this->output .= "<tr>\n";
		$this->output .= "<td>\n";
		$this->output .= "<form action='" . $PHP_SELF . "' method='post'>\n";
		$this->output .= "<input type='hidden' name='select' value='student_sign_up' />\n";
		$this->output .= "<input type='hidden' name='job_id' value='" . $this->job_id . "' />\n";
		$this->output .= "<input type='hidden' name='time_id' value='" . $this->time_id . "' />\n";
		$this->output .= "<input type='hidden' name='action' value='" . $this->action . "' />\n";
		$this->output .= "<input type='submit' name='confirm' value='Yes' />\n";
		$this->output .= "</form>\n";
		$this->output .= "</td>\n";
		$this->output .= "<td>\n";
		$this->output .= "<form action='" . $PHP_SELF . "' method='post'>\n";
		$this->output .= "<input type='hidden' name='select' value='student_sign_up' />\n";
		$this->output .= "<input type='hidden' name='job_id' value='" . $this->job_id . "' />\n";
		$this->output .= "<input type='submit' value='No' />\n";
		$this->output .= "</form>\n";
		$this->output .= "</td>\n";
		$this->output .= "</tr>\n";
		$this->output .= "</table>\n";
		$this->output .= "</td></tr></table>\n";
	}

	
	
	function generate_phone_page()
	{
		global $PHP_SELF;
		// if function returns false then the normal interview_details page is displayed


		//	validation - already guaranteed
		//	1. valid job_id
		//	2. student_number is shortlisted
		//	3. job_id isshowable


		if (($this->action != "On Campus") && ($this->action != "Change Phone Number") && ($this->action != "Specify Phone Number"))
		{
			// nothing to do
			return false;
		}

		// is job editable
		
		/*
		Changed code directly below comment block.  Previous code
		contained if(!$this->editable).  Now the code accounts for
		interviews that span multiple days.  Change made by Adam
		on 2001-01-25.
		*/

		$time_id = getTimeID($this->job_id, $this->student_number);
		
		// If there was an error getting the timeID, let user know
		// and tell them to try again.  Then return false.

		if ($time_id["return"] == T_ERROR)
		{
			$this->error($time_id["value"] . " Please try again.  If problem persists please contact the Co-op office.");
			return false;
		}

		// If there was no timeID affiliated with the jobID and 
		// the student number, return false.

		if ($time_id["return"] == T_FALSE) 
		{
			return false; 
		}

		// A valid timeID was returned, get the date of the interview
		// using this timeID.

		$time_id = $time_id["value"];
	
		$result_date = $GLOBALS['dbh']->Execute("
					SELECT int_date, int_time
					FROM interview_time
					WHERE time_id='" . addslashes($time_id) . "'
					");
		if(!$result_date)
		{
			return false;
		}
		$row = $result_date->FetchRow();
		$result_date->Close();

		$date = $row["int_date"];
		$time = $row["int_time"];

		global $userlevel;
		if ($userlevel == ADMINSTUDENT){
			$this->error("You are not allowed to modify student's information from here.  You must
					log off as the student and use Modify Sign-up.");
			return false;
		}

		if (!isJobIDStudentEditable($this->job_id, $this->student_number, $row["int_date"])) 
		{ 
			return false; 
		}

		/*
		END OF NEW CODE
		*/

		// is the student signed up for the job
		if (!isStudentSignedUp($this->job_id, $this->student_number)) { return false; }

		// is it an off-campus phone interview
		if (!isOffCampusPhone($this->job_id)) { return false; }
		$phone_number = getInterviewPhone($this->job_id, $this->student_number);
		if ($phone_number["return"] == T_ERROR)
		{
			$this->error($phone_number["value"] . " Please try again.  If problem persists "
							. " please contact the Co-op office.");
			return false;
		}
				
		switch ($this->action)
		{
			case "On Campus":
				if ($phone_number["return"] == T_FALSE) { return false; }

				if ($this->confirm)
				{
					global $job_id;
					if (isEmployerEmailSent($job_id))
					{
						$reason = "remove phone-number";
						include('interview/sign_up/student/send_secretary_email.inc');
						unset($reason);
					}
					// remove interview_phone (don't forget any switch interviews)
					$time_id = getTimeID($this->job_id, $this->student_number);
					if ($time_id["return"] == T_ERROR)
					{
						$this->error($time_id["value"] . " Please try again.  If problem persists "
										. " please contact the Co-op office.");
						return false;
					}
					if ($time_id["return"] == T_FALSE) { return false; }
					
					$time_id = $time_id["value"];
					if (!$time_ids = $this->get_switch($time_id)) { return false; }
				
					$sql = ("
						SELECT DISTINCT interview_type_id
						FROM interview 
						WHERE job_id='" . $this->job_id . "'
						");
					$tempresult = $GLOBALS['dbh']->Execute($sql);
					$temprow = $tempresult->FetchRow();
					
					if (isSequential($temprow["interview_type_id"]))
					{
						if (!$time_ids = $this->get_sequential($time_id))
						{
							return false;
						}
					}
				
					$result = $GLOBALS['dbh']->Execute("
						DELETE FROM	interview_phone
						WHERE	time_id IN (" . $time_ids . ")");
					
					if (!$result || !$GLOBALS['dbh']->Affected_Rows())
					{
						$this->error("Query failed.  Please check to make sure that the interview "
										. " was reverted to on campus.  If problem persists, please contact "
										. " the Co-op Office.");
						return false;
					}
					$GLOBALS['dbh']->Execute("REPLACE INTO signup_edit (job_id, last_edited, type) VALUES ('$this->job_id', NULL, 'Student')");

					$this->notify("The interview has been reverted to on-campus.");
					return false;
				}
				else
				{					
					$this->output .= "<table class='backgrey' cellpadding='5' width='100%'><tr><td>\n";
					$this->output .= "<h4>Interview Details:</h4>\n";
					$this->output .= "<p>Are you sure want to revert to an on-campus interview?</p>\n";
					$this->output .= "<table cellpadding='0' cellspacing='0' align='center'>\n";
					$this->output .= "<tr>\n";
					$this->output .= "<td>\n";
					$this->output .= "<form action='" . $PHP_SELF . "' method='post'>\n";
					$this->output .= "<input type='hidden' name='select' value='student_sign_up' />\n";
					$this->output .= "<input type='hidden' name='page' value='phone' />\n";
					$this->output .= "<input type='hidden' name='job_id' value='" . $this->job_id . "' />\n";
					$this->output .= "<input type='hidden' name='action' value='" . $this->action . "' />\n";
					$this->output .= "<input type='submit' name='confirm' value='Yes' />\n";
					$this->output .= "</form>\n";
					$this->output .= "</td>\n";
					$this->output .= "<td>\n";
					$this->output .= "<form action='" . $PHP_SELF . "' method='post'>\n";
					$this->output .= "<input type='hidden' name='select' value='student_sign_up' />\n";
					$this->output .= "<input type='hidden' name='job_id' value='" . $this->job_id . "' />\n";
					$this->output .= "<input type='submit' value='No' />\n";
					$this->output .= "</form>\n";
					$this->output .= "</td>\n";
					$this->output .= "</tr>\n";
					$this->output .= "</table>\n";
					$this->output .= "</td></tr></table>\n";
					return true;
				}
				break;
				
			case "Change Phone Number":
			case "Specify Phone Number":
				if ($this->confirm)
				{
					if ($this->confirm != "OK") { return false; }
					
					// if phone_number is blank, remove interview_phone if present (don't forget any switch interviews)
					// if phone_number, update interview_phone if present, insert if not present
					//																	(don't forget any switch interviews)
					
					$this->phone = trim($this->phone);
					if ($this->phone == "")
					{
						global $job_id;
						if (isEmployerEmailSent($job_id))
						{
							$reason = "remove phone-number";
							include('interview/sign_up/student/send_secretary_email.inc');
							unset($reason);
						}
						$this->remove_phone_number();
						return false;
					}
					
					global $job_id;
					if (isEmployerEmailSent($job_id))
					{
						$reason = "add phone-number";
						include('interview/sign_up/student/send_secretary_email.inc');
						unset($reason);
					}
					$this->set_phone_number();
					return false;
				}
				else
				{
					// load current phone_number, if present
					// allow user to edit it
					// provide option to set, or cancel
					
					$this->output .= "<table class='backgrey' cellpadding='5' width='100%'><tr><td>\n";
					$this->output .= "<h4>Interview Details:</h4>\n";
					$this->output .= "<p>The interviewer will call you at the following phone number.</p>\n";
					$this->output .= "<form action='" . $PHP_SELF . "' method='post'>\n";
					$this->output .= "<input type='hidden' name='select' value='student_sign_up' />\n";
					$this->output .= "<input type='hidden' name='page' value='phone' />\n";
					$this->output .= "<input type='hidden' name='job_id' value='" . $this->job_id . "' />\n";
					$this->output .= "<input type='hidden' name='action' value='" . $this->action . "' />\n";
					$this->output .= "<table cellpadding='0' cellspacing='0' align='center'>\n";
					$this->output .= "<tr>\n";
					$this->output .= "<td>Phone:&nbsp;</td>\n";
					$this->output .= "<td><input type='text' name='phone' maxlength='25' value='" . $phone_number["value"] . "' /></td>";
					$this->output .= "</tr>\n";
					$this->output .= "<tr>\n";
					$this->output .= "<td colspan='2' align='center'>\n";
					$this->output .= "<input type='submit' name='confirm' value='OK' />\n";
					$this->output .= "<input type='submit' name='confirm' value='Cancel' />";
					$this->output .= "</td>\n";
					$this->output .= "</tr>\n";
					$this->output .= "</table>\n";
					$this->output .= "</form>\n";
					$this->output .= "</td></tr></table>\n";
					
					return true;				
				}
				break;
		}		
		return true;
	}

	
	function get_switch($time_id)
	{
		$time_ids = "'" . addslashes($time_id) . "'";

		if (($switching = isSwitchingInterview($this->job_id)) == T_ERROR)
		{
			$this->error("Query failed (or job could not be found).  The sign-up could not be "
							. " processed.  Please try again.  If this problem persists, please "
							. " contact the Co-op Office.");
			return false;
		}
						
		if ($switching)
		{
			// find corresponding switch time
			if (!$switch_time_id = getSwitchInterview($time_id))
			{
				$this->error("Query failed (or complement interview time could not be found). "
								. " The sign-up could not be processed.  Please try again.  If this "
								. " problems persists, please contact the Co-op Office.");
				return false;
			}
			
			// remove both
			$time_ids .= ", '" . addslashes($switch_time_id) . "'";
		}
		return $time_ids;	
	}

	
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
		 I only goes up to 5 in the next loop, because there should NEVER be any more than 5 people a student has
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
				 There are no more time_id's that match correspond, so jump out and return the time_id array.
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

	function remove_phone_number($message=true)
	{
		// remove interview_phone (don't forget any switch interviews)
		$time_id = getTimeID($this->job_id, $this->student_number);
		if ($time_id["return"] == T_ERROR)
		{
			$this->error($time_id["value"] . " Please try again.  If problem persists "
							. " please contact the Co-op office.");
			return false;
		}
		if ($time_id["return"] == T_FALSE) { return false; }
				
		$time_id = $time_id["value"];
		if (!$time_ids = $this->get_switch($time_id)) { return false; }
		
		/*
		 Find out if this is a sequential interview, and if so, get the associated time_id's for this interview.
		*/

		$sql = ("
			SELECT DISTINCT interview_type_id
			FROM interview
			WHERE job_id='" . $this->job_id . "'
			");
		$tempresult = $GLOBALS['dbh']->Execute($sql);
		$temprow = $tempresult->FetchRow();
		
		if (isSequential($temprow["interview_type_id"]))
		{
			if (!$time_ids = $this->get_sequential($time_id))
			{
				return false;
			}
		}
			
		$result = $GLOBALS['dbh']->Execute("
			DELETE FROM	interview_phone
			WHERE	time_id IN (" . $time_ids . ")");
		
		if (!$result)
		{
			$this->error("Query failed.  Please check to make sure that the interview "
							. " was reverted to on campus.  If problem persists, please contact "
							. " the Co-op Office.");
			return false;
		}
					
		if ($message) { 
			$this->notify("The interview has been reverted to on-campus.");
			$GLOBALS['dbh']->Execute("REPLACE INTO signup_edit (job_id, last_edited, type) VALUES ('$this->job_id', NULL, 'Student')");
		}
		return true;
	}

	
	function set_phone_number()
	{
		// set phone number
		// (remember, the phone number may or may not be present already)
		
		if (!$this->remove_phone_number(false)) { return true; }
	

		$time_id = getTimeID($this->job_id, $this->student_number);
		if ($time_id["return"] == T_ERROR)
		{
			$this->error($time_id["value"] . " Please try again.  If problem persists "
							. " please contact the Co-op office.");
			return false;
		}
		if ($time_id["return"] == T_FALSE) { return false; }
				
		$time_id = $time_id["value"];
		if (!$time_ids = $this->get_switch($time_id)) { return false; }
		
		$sql = ("
			SELECT DISTINCT interview_type_id
			FROM interview
			WHERE job_id='" . $this->job_id . "'
			");
		$tempresult = $GLOBALS['dbh']->Execute($sql);
		$temprow = $tempresult->FetchRow();
		
		if (isSequential($temprow["interview_type_id"]))
		{
			if (!$time_ids = $this->get_sequential($time_id))
			{
				return false;
			}
		}
				
		$time_id = explode(",", $time_ids);
		
		for ($i = 0; $i < sizeof($time_id); $i++)
		{
			$result = $GLOBALS['dbh']->Execute("
				INSERT INTO interview_phone
					(time_id, phone)
				VALUES
					('" . addslashes(preg_replace("/'(\d*)'/", "\\1", $time_id[$i])) . "', '" . addslashes($this->phone) . "')");
			
			if (!$result)
			{
				$this->error("Query failed.  Please check to make sure that the interview "
								. " phone number was set.  If problem persists, please contact "
								. " the Co-op Office.");
				return false;
			}
		}
		
		$this->notify("The interview phone number has been set to " . $this->phone . ".");
		$GLOBALS['dbh']->Execute("REPLACE INTO signup_edit (job_id, last_edited, type) VALUES ('$this->job_id', NULL, 'Student')");
		return true;
	}
	
}


?>
