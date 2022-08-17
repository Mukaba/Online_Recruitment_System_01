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
 | Filename: email_employer.inc                                                 |
 +------------------------------------------------------------------------------+
 | Description:                                                                 |
 +------------------------------------------------------------------------------+

*/

// filename: email_employer.inc

include("misc/table2.inc");


switch ($email_employer)
{
	case "Send":
		global $auth;

		$from = ($from = getCiEmailUsingID(getContactID($auth->department, $auth->login))) ? $from : BRAND_NAME_SYSTEM_EMAIL;
        $reply_id = $reply_to;
        $reply_to = ($reply_to = getCiEmailUsingID($reply_to)) ? $reply_to : "";
		
		send_email($job_id, $to, $cc, $bcc, $subject, $body, $from, $reply_to, $reply_id);
		break;
		
	case "create":
	default:
		
		display_email_form($job_id);
		break;
}




function display_email_form($job_id)
{
	global $PHP_SELF;
	global $auth;

	/*
         Find the users contact_id, and use this to get their e-mail address.
        */

        $users_contact_id = getContactID($auth->department, $auth->login);
        $user_email = getCiEmailUsingID($users_contact_id);
		
	/*
         Pull the default contact for this interview out of the database.
        */

        $sql = ("
                SELECT DISTINCT contact
                FROM interview
                WHERE job_id='" . $job_id . "'
                ");
        $contact_result = $GLOBALS['dbh']->Execute($sql);
        $contact_row = $contact_result->FetchRow();

        $default_contact = $contact_row["contact"];

	/*
	 Get all of the contacts from the database that can be displayed for this purpose
	 (based on whether or not they have their interview_contact_list flag set), as well
	 as the default interview contact.
	*/

	$sql = ("
		SELECT DISTINCT a.login_id, a.netlink_id, a.contact_id, CONCAT(b.first_name, ' ', b.last_name) AS name, b.email
		FROM contact_internal AS a, contact AS b, interview AS i
		WHERE (
            a.department_id IN ('" . implode(department_groups($auth->department), "', '"). "')
            OR (i.contact=b.contact_id AND i.job_id='" . $job_id . "')
            )
		AND (a.interview_contact_list OR a.contact_id='".$users_contact_id."')
		AND a.contact_id=b.contact_id
		ORDER BY b.last_name, b.first_name
		");
	$result = $GLOBALS['dbh']->Execute($sql);

	if (!$result) 
	{ 
		return false; 
	}

	$emp_result = $GLOBALS['dbh']->Execute("
		SELECT a.email
		FROM contact AS a, contact_job AS b
		WHERE b.job_id='" . addslashes($job_id) . "'
		AND a.contact_id=b.contact_id
		");
	$emp_row = $emp_result->FetchRow();
	$emp_result->Close();
	$employer_email = $emp_row["email"];
	
	$jobcode_result = $GLOBALS['dbh']->Execute("
		SELECT job_code 
		FROM job_info 
		WHERE job_id='" . addslashes($job_id) . "'
		");
	$jobcode_row = $jobcode_result->FetchRow();
	$jobcode_result->Close();
	$job_code = $jobcode_row["job_code"];

	?>
	<h3>Interviews - Employer Schedule E-mail</h3>
	
	<form action="<?php echo($PHP_SELF); ?>" method="post" enctype="multipart/form-data">
		<input type="hidden" name="select" value="sign_up" />
		<input type="hidden" name="job_id" value="<?php echo($job_id); ?>" />
		<table border='0' cellspacing='0' cellpadding='4' class='row1'>
			<tr><td>To:</td><td colspan='2'><input type="text" name="to" size='52' value='<?php echo(($employer_email) ? $employer_email : ""); ?>' /></td></tr>
			<tr><td>Cc:</td><td colspan='2'><input type="text" name="cc" size='52' /></td></tr>
			<tr><td>Bcc:</td><td colspan='2'><input type="text" name="bcc" size='52' value='<?php echo(($user_email) ? $user_email : ""); ?>' /></td></tr>
			<tr><td>From:</td><td colspan='2'><?php echo(($user_email) ? $user_email : BRAND_NAME_SYSTEM_EMAIL); ?></td></tr>
			<tr><td valign='top'>Reply to:</td><td valign='top'>
				<select name="reply_to">
				<?php
				
				while ($row = $result->FetchRow())
				{
					echo("<option value='" . $row["contact_id"] . "'");
					if ($default_contact == $row["contact_id"])
					{
						echo(" selected='selected'");
					}
					echo(">" . $row["name"] . "</option>\n");
				}
				$result->Close();
							
				?>
				</select>
			</td>
			<td valign='top'> Contact information for this address will be automatically appended to the e-mail.</td>
			</tr>
			<tr><td>Subject:</td><td colspan='2'><input type="text" name="subject" size='52' value="Interview Schedule for UVic:<?php echo($job_code); ?>" /></td></tr>
			<tr><td colspan="3"><textarea class='fixed_font' cols="60" rows="20" name="body"><?php echo(create_body2($job_id)); ?></textarea></td></tr>
			<tr><td colspan="3" align="center"><hr />
				<input type="submit" name="email_employer" value="Send" />&nbsp;&nbsp;
				<input type="submit" name="email_employer" value="Cancel" />
			</td></tr>
		</table>
	</form>
	<?php
}


function send_email($job_id, $to, $cc, $bcc, $subject, $body, $from, $reply_to, $reply_id)
{
	global $PHP_SELF;
	include('misc/email.inc');
	
	$mail = new email($to, $subject, unslash($body));
		
	$mail->from = $from;
	$mail->cc = $cc;
	$mail->bcc = $bcc;
	$mail->headers = "Reply-To: " . $reply_to . "\n";
    $mail->reply_id = $reply_id;

	echo("<h3>Interviews - Employer Schedule E-mail</h3>");
	
	if (!$mail->send())
	{
		error($mail->error_text." Please push the BACK button on your browser and try again.");
	}	
	else
	{
		$result = $GLOBALS['dbh']->Execute("
			UPDATE	interview
			SET		emailed_employer=1
			WHERE		job_id='" . addslashes($job_id) . "'");
		
		?>
			<div align="left">
			<?php
			notify("E-mail was sent.");
			?>
			<form action="<?php echo($PHP_SELF); ?>">
				<input type="hidden" name="select" value="sign_up" />
				<input type="hidden" name="job_id" value="<?php echo($job_id); ?>" />
				<input type="submit" value="OK" />
			</form>
			</div>
		<?php
	}
	unset ($mail);
}










function create_body2($job_id)
{
	$sql = ("
		SELECT	j.job_code, j.department_id, a.interview_place_id as place, a.interview_medium_id as medium, a.presentation,
			b.department_name, b.general_instructions, b.oncampus_instructions, b.offcampus_instructions, b.video_instructions,
			b.phone_instructions, c.company_name
		FROM	interview AS a, department AS b, employer_company AS c, interview_place AS p, interview_medium AS m, job_info AS j
		WHERE	a.job_id='" . addslashes($job_id) . "' AND
			j.department_id=b.department_id AND
			j.employer_id=c.employer_id AND
			j.job_id = a.job_id
		");

	$result = $GLOBALS['dbh']->Execute($sql);

	if (!$result) { return false; }

	if ($row = $result->FetchRow())
	{
		$job_code = $row["job_code"];
		$oncampus = isOnCampus($row["place"]);
		$department_name = $row["department_name"];
		global $department_id;
		$department_id = $row["department_id"];
		$general_instructions = $row["general_instructions"];
		$oncampus_instructions = $row["oncampus_instructions"];
		$offcampus_instructions = $row["offcampus_instructions"];
		$video_instructions = $row["video_instructions"];
		$phone_instructions = $row["phone_instructions"];
		$company_name = $row["company_name"];
		$presentation = $row["presentation"];
		$medium = $row["medium"];
	}
		$phone = isPhone($medium);
		$video = isVideo($medium);
	$result->Close();
	if (!$row) { return false; }
	
	// instructions
	{
		// department specific employer instructions
		$output .= $general_instructions . "\n\n";
		// department specific interview-type instructions
		if ($phone)
		{
			$output .= $phone_instructions . "\n\n";
		}
		elseif ($video)
		{
			$output .= $video_instructions . "\n\n";
		}
		elseif ($oncampus)
		{
			$output .= $oncampus_instructions . "\n\n";
		}
		else
		{
			$output .= $offcampus_instructions . "\n\n";
		
			// interview location if off-campus (non-phone)
			$result = $GLOBALS['dbh']->Execute("
				SELECT	street_address_1, street_address_2, city, province, country
				FROM	interview_location
				WHERE	job_id='" . addslashes($job_id) . "'
				");
			
			if (!$result) { return false; }
			if ($row = $result->FetchRow())
			{
				$output .= "Interview location:" . "\n";
				$output .= $row["street_address_1"] . "\n";
				if ($row["street_address_2"]) { $output .= $row["street_address_2"] . "\n"; }
				$output .= $row["city"] . "\n";
				if ($row["province"]) { $output .= $row["province"] . "\n"; }
				if ($row["country"]) { $output .= $row["country"] . "\n"; }
			}
			$result->Close();
		}
		$output .= "\n";
	}
	
	
	// head
	{
		$table = new table(
			new tr(new td("Interview Schedule", "center")),
			new tr(new td($department_name, "center"))
		);
		$output .= $table->display() . "\n";
		
		$result = $GLOBALS['dbh']->Execute("
			SELECT DISTINCT
				b.interviewer_name
			FROM	interview_time AS a,
				interview_interviewer AS b
			WHERE	a.job_id='" . addslashes($job_id) . "' AND
				a.interviewer_id=b.interviewer_id");
		
		if (!$result) { return false; }
		unset($interviewer_names);
		$interviewer_names = Array();
		while ($row = $result->FetchRow())
		{
			$interviewer_names[] = $row["interviewer_name"];
		}
		$result->Close();

		$result = $GLOBALS['dbh']->Execute("
			SELECT DISTINCT int_date
			FROM	interview_time
			WHERE	job_id='" . addslashes($job_id) . "'
			ORDER BY int_date");
		
		if (!$result) { return false; }
		unset($int_dates);
		while ($row = $result->FetchRow())
		{
			$int_dates[] = formatStandardDate($row["int_date"]);
		}
		$result->Close();
												
		$table = new table(
			new tr(new td("Employer:", "right"), new td($company_name, "left")),
			new tr(new td("Co-op Job Code:", "right"), new td($job_code, "left"))
		);
		for ($i = 0; $i < sizeof($interviewer_names); $i++)
		{
			$table->rows[] = new tr(new td(((!$i) ? "Interviewer(s):" : ""), "right"), new td($interviewer_names[$i], "left"));
		}
		for ($i = 0; $i < sizeof($int_dates); $i++)
		{
			$table->rows[] = new tr(new td("Date(s):", "right"), new td($int_dates[$i], "left"));
		}		
		$output .= $table->display() . "\n\n";
	}
	
	
	// presentation
	{
		if ($presentation){
			$result = $GLOBALS['dbh']->Execute("
				SELECT pres_date, pres_time, pres_loc, pres_notes, end_time
				FROM presentation
				WHERE job_id = '" . $job_id . "'");
			if (!$result || !$result->RecordCount()){ return false;};

			$presentation_row = $result->FetchRow();
			$table = new table(
					new tr(new td("Date:", "left"), new td($presentation_row["pres_date"], "left")),
					new tr(new td("Start Time:", "left"), new td(txtFormatTime($presentation_row["pres_time"]), "left")),
					new tr(new td("End Time:", "left"), new td(txtFormatTime($presentation_row["end_time"]), "left")),
					new tr(new td("Location:", "left"), new td($presentation_row["pres_loc"], "left"))
	/*commented out for looks*/	//new tr(new td("Notes:", "left"), new td($presentation_row["pres_notes"], "left"))
			);

			$output .= "Presentation Information\n" . $table->display() . "\n\n";
		}
	}
	// schedule
	{
		$result = $GLOBALS['dbh']->Execute("
			SELECT DISTINCT a.int_date, a.interviewer_id, b.interviewer_name
			FROM	interview_time AS a, interview_interviewer AS b
			WHERE	a.job_id='" . addslashes($job_id) . "' 
			AND a.interviewer_id=b.interviewer_id
			ORDER BY	a.int_date, a.interviewer_id 
			");
		
		if (!$result || !$result->RecordCount()) { return false; }
		
		$last_int_date=-99;
		while ($row = $result->FetchRow())
		{
			if ($last_int_date == -99){
				$last_int_date = $row["int_date"];
			}
			$int_date = $row["int_date"];
			$interviewer_id = $row["interviewer_id"];
			
			if ($int_date != $last_int_date){
				$int_date_schedule[] = new interview_date($last_int_date, $interviewer_names1, $time_slots_1);
				unset($interviewer_names1);
				unset($time_slots_1);
			}
			$last_int_date = $int_date;
		
			//  IF on second line of query instead of the following because
			//		of a MySQL bug
			//		ELT(a.int_type, 'interview', 'break', 'lunch') AS int_type,

			$sql = ("
				SELECT a.int_time, a.end_time, a.student_number, a.time_id,
				IF(a.int_type=1, 'interview', IF(a.int_type=2, 'break', IF(a.int_type=3, 'lunch', ''))) AS int_type,
				CONCAT(b.first_name, ' ', b.last_name) AS student_name, c.phone AS phone_number
				FROM interview_time AS a
				LEFT OUTER JOIN	student AS b ON a.student_number=b.student_number
				LEFT OUTER JOIN	interview_phone AS c ON a.time_id=c.time_id
				WHERE a.job_id='" . addslashes($job_id) . "' 
				AND a.interviewer_id='" . addslashes($interviewer_id) . "' 
				AND a.int_date='" . addslashes($int_date) . "'
				ORDER BY int_time
				");
			$result2 = $GLOBALS['dbh']->Execute($sql);
			
			if (!$result2 || !$result2->RecordCount()) { return false; }

			while ($row2 = $result2->FetchRow())
			{
				/*
				 Get the appropriate department for this student.
				*/

				$students_department = getStudentEmailDepartment($row2["student_number"]);

				$time_slots[] = new time_slots($row2["int_time"], $row2["end_time"], $row2["int_type"], $row2["student_name"], $row2["phone_number"], $students_department, $row2["time_id"]);
			}
			$result2->Close();

			$interviewer_names1[] = $row["interviewer_name"];
			$time_slots_1[] = $time_slots;
			//$interviewer_dates[] = new interviewer_date($int_date, $row["interviewer_name"], $time_slots);
			unset($time_slots);
		}
		$int_date_schedule[] = new interview_date($int_date, $interviewer_names1, $time_slots_1);
		unset($interviewer_names1);
		unset($time_slots_1);
		
		$result->Close();

		$output .= info_block_2($int_date_schedule, $medium, $job_id);
		//$output .= info_block_2($interviewer_dates, $medium);
	}
	
	return $output;
}





class interview_date
{
	var $int_date;
	var $interviewer_names = array();
	var $time_slots = array();
	
	function interview_date($int_date, $interviewer_names, $time_slots)
	{
		$i=0;
		$this->int_date = $int_date;
		$this->interviewer_names = $interviewer_names;
		$this->time_slots = $time_slots;
	}
}



class interviewer_date
{
	var $int_date;
	var $interviewer_name;
	var $time_slots;
	
	function interviewer_date($int_date, $interviewer_name, $time_slots)
	{
		$this->int_date = $int_date;
		$this->interviewer_name = $interviewer_name;
		$this->time_slots = $time_slots;
	}
}

class time_slots
{
	var $start_time;
	var $end_time;
	var $type;
	var $student_name;
	var $phone_number;
	var $department_id;
	var $time_id;
	
	function time_slots($start_time, $end_time, $type, $student_name, $phone_number, $department_id, $time_id)
	{
		$this->start_time = $start_time;
		$this->end_time = $end_time;
		$this->type = $type;
		$this->student_name = $student_name;
		$this->phone_number = $phone_number;
		$this->department_id = $department_id;
		$this->time_id = $time_id;
	}
}




function info_block_2($interviewer_date, $medium, $job_id)
{
	$num_interviewers = count($interviewer_date[0]->interviewer_names);
	$num_interviewer_dates = count($interviewer_date);
	for ($i = 0; $i < $num_interviewer_dates; $i++)
	{
		$output .= "Date: ". formatLongDate($interviewer_date[$i]->int_date) . "\n";
		if (isPhone($medium))
		{
			if ($num_interviewers > 1){
				$table = new table(new th(new td("Interviewer", "center"), new td("Time", "center"), 
					new td("Student", "center"), new td("Phone", "center")));
			}else{
				$table = new table(new th(new td("Time", "center"), 
					new td("Student", "center"), new td("Phone", "center")));
			}
		}
		else
		{
			if ($num_interviewers > 1){
				$table = new table(new th(new td("Interviewer", "center"), new td("Time", "center"), 
					new td("Student", "center")));
			}else{
				$table = new table(new th(new td("Time", "center"), 
					new td("Student", "center")));
			}
		}

		for($k=0;$k<count($interviewer_date[$i]->interviewer_names);$k++){
			//$output .= "Interviewer: " . $interviewer_date[$i]->interviewer_name . "\n";		
			$num_time_slots = count($interviewer_date[$i]->time_slots[$k]);
			for ($j = 0; $j < $num_time_slots; $j++)
			{
				$time = txtFormatTime($interviewer_date[$i]->time_slots[$k][$j]->start_time);
				$type = $interviewer_date[$i]->time_slots[$k][$j]->type;
				$student_name = $interviewer_date[$i]->time_slots[$k][$j]->student_name;
			
				global $department_id;

				$sql = ("
					SELECT DISTINCT default_int_phone
					FROM department
					WHERE department_id='" . $department_id . "'
					");
				$result = $GLOBALS['dbh']->Execute($sql);
				$row = $result->FetchRow();
				
				$default_int_phone = $row["default_int_phone"];

				if ($type == "interview") { $td = new td($student_name, "left"); }
				else { $td = new td("-- " . strtoupper($type) . " --", "center"); }
				if (isPhone($medium))
				{
					$phone_set = 0;

					$sql = "select use_room_phone_number from interview where job_id=\"$job_id\"";
					$response_ = $GLOBALS['dbh']->Execute ($sql);
					$row_ = $response_->FetchRow();

					// Off-campus interviewee who has set his phone number.
					if ($phone_number = $interviewer_date[$i]->time_slots[$k][$j]->phone_number)
					{
					  if ($num_interviewers>1)
					  {
					    $table->rows[] = new tr(new td($interviewer_date[$i]->interviewer_names[$k], "center"), new td($time, "center"), $td, new td($phone_number, "left"));
					  }
					  else
					  {
					    $table->rows[] = new tr(new td($time, "center"), $td, new td($phone_number, "left"));
					  }

					  $phone_set = 1;
					}

					else if (($row_["use_room_phone_number"] == 1) &&
						 strlen($interviewer_date[$i]->time_slots[$k][$j]->student_name))
					{
					  $sql = "select r.phone from room as r, interview_time as it where 
						  it.time_id=\"".$interviewer_date[$i]->time_slots[$k][$j]->time_id."\"
						  and it.int_room_id=r.room_id"; 
					  $response_ = $GLOBALS['dbh']->Execute ($sql);
					  $response_row = $response_->FetchRow();
					  $phone_number = $response_row["phone"];

					  if ($phone_number)
					  {
					    $phone_set = 1;
					    if ($num_interviewers>1)
					    {
			 		      $table->rows[] = new tr(new td($interviewer_date[$i]->interviewer_names[$k], "center"), new td($time, "center"), $td, new td($phone_number, "left"));
					    }
					    else
					    {
					      $table->rows[] = new tr(new td($time, "center"), $td, new td($phone_number, "left"));
					    }
					  }
					}	

					if (!$phone_set)
					{
					  if ($student_name)
					  {
						if ($num_interviewers>1){
							$table->rows[] = new tr(new td($interviewer_date[$i]->interviewer_names[$k], "center"), new td($time, "center"), $td, new td($default_int_phone, "left"));
						}else{
							$table->rows[] = new tr(new td($time, "center"), $td, new td($default_int_phone, "left"));
						}
					  }else{
						if ($num_interviewers>1){
							$table->rows[] = new tr(new td($interviewer_date[$i]->interviewer_names[$k], "center"), new td($time, "center"), $td, new td("", "center"));
						}else{
							$table->rows[] = new tr(new td($time, "center"), $td, new td("", "center"));
						}
					  }
					}
				}
				else
				{
					if ($num_interviewers>1){
						$table->rows[] = new tr(new td($interviewer_date[$i]->interviewer_names[$k], "center"), new td($time, "center"), $td);
					}else{
						$table->rows[] = new tr(new td($time, "center"), $td);
					}
				}
			}
			if ($k < count($interviewer_date[$i]->interviewer_names)-1){
				if (isPhone($medium)){
					if ($num_interviewers>1){
						$table->rows[] = new tr(new td("", "center"), new td("", "center"), new td("", "center"), new td("", "center"));
					}else{
						$table->rows[] = new tr(new td("", "center"), new td("", "center"), new td("", "center"));
					}
				}else{
					if ($num_interviewers>1){
						$table->rows[] = new tr(new td("", "center"), new td("", "center"), new td("", "center"));
					}else{
						$table->rows[] = new tr(new td("", "center"), new td("", "center"));
					}
				}
			}	
		}
		
		$output .= $table->display() . "\n\n";
		unset($table);
	}
	return $output;
}





?>
