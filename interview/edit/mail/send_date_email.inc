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
 | Filename: send_date_email.inc                                                |
 +------------------------------------------------------------------------------+
 | Description: This file is called by edit_jobinfo.inc when an interview has   |
 | its date changed, the shortlisted students have been e-mailed, and there     |
 | are still students left who have not yet signed up.                          |
 +------------------------------------------------------------------------------+

*/

include("misc/table2.inc");


switch ($email_employer)
{

	case "Send":
		global $auth;

		$from = ($from = getCiEmailUsingID(getContactID($auth->department, $auth->login))) ? $from : BRAND_NAME_SYSTEM_EMAIL;
        $reply_id = $reply_to;
		$reply_to = ($reply_to = getCiEmailUsingID($reply_to)) ? $reply_to : "";
		
		$sql = "SELECT MAX(date_edited) as last_modified FROM interview_edit WHERE job_id = '" . $job_id . "' AND menu_edited = 'Job Info'";
		$row = $GLOBALS['dbh']->GetRow($sql);
		
		if ($row["last_modified"] > $last_modified)
		{
			error ("An administrator has been modifying data corresponding to this interview since you last saw it.  The most recent
			data is shown below, ensure it is correct before sending the e-mail.");
			$interview->last_modified = $row["last_modified"];
		}
		else
		{
			if (send_email($job_id, $to, $cc, $bcc, $subject, $body, $from, $reply_to, $reply_id))
			{
				if ($students_dropped)
				{
					if (is_string($students_dropped))
					{
						$students_dropped = unpackObject(($students_dropped));
					}
	
					unset($email_employer);
					include('interview/edit/mail/send_dropped_email2.inc');
				}
				else
				{
					notify("Date successfully changed, and notification of date change has been successfully e-mailed to students.");
				}
			}
			break;
		}
	case "create":
	default:
		display_email_form($job_id, $department_id);
		break;
}




function display_email_form($job_id, $department_id)
{
	global $PHP_SELF;
	global $auth;
	global $interview;
	global $students_dropped;

	/*
	 Pull the details for this interview out of the database.
	*/

	$sql = ("
		SELECT a.job_code, b.company_name
		FROM job_info AS a, employer_company AS b
		WHERE a.job_id='" . $job_id . "'
		AND b.employer_id=a.employer_id
		");
	$result = $GLOBALS['dbh']->Execute($sql);
	$row = $result->FetchRow();
	
	$email_job = array(
		"job_code" => $row["job_code"],
		"company_name" => $row["company_name"]
		);

	/*
	 Get the e-mails for students that are shortlisted but have not yet signed up.
	*/

	$sql = ("
		SELECT DISTINCT student_number
		FROM interview_time
		WHERE job_id='" . $job_id . "'
		");
	$result = $GLOBALS['dbh']->Execute($sql);
	while ($row = $result->FetchRow())
	{
		$shortlist .= ("'" . $row["student_number"] . "',");
	}

	for ($i = 0; $i < sizeof($students_dropped); $i++)
	{
		$shortlist .= ("'" . $students_dropped[$i] . "',");
	}

	$shortlist = substr($shortlist, 0, (strlen($shortlist) - 1));

	if (!$shortlist)
	{
		$shortlist = "''";
	}
	
	$sql = ("
		SELECT DISTINCT a.student_number, CONCAT(a.first_name, ' ', a.last_name) AS name
		FROM students_shortlisted AS b, student AS a
		WHERE b.job_id='" . $job_id . "'
		AND b.student_number NOT IN (" . $shortlist . ")
		AND a.student_number=b.student_number
		");
	$result = $GLOBALS['dbh']->Execute($sql);
	while ($row = $result->FetchRow())
	{
		$students_email = getStudentEmail($row["student_number"]);

		$email_students[] = array(
			"name" => $row["name"],
			"student_number" => $row["student_number"],
			"email" => $students_email
			);
	}

	/*
	 Get all of the department_ids that are in this department's group.
	*/

	$sql = ("
                SELECT DISTINCT a.department_id
                FROM department_group AS a, department_group AS b
                WHERE b.department_id='" . $auth->department . "'
                AND a.group_id=b.group_id
                ");
        $group_result = $GLOBALS['dbh']->Execute($sql);

        if ($group_result->RecordCount())
        {
                while($group_row = $group_result->FetchRow())
                {
                        $department_ids .= ("'" . $group_row["department_id"] . "',");
                }

                $department_ids = substr($department_ids, 0, (strlen($department_ids) - 1));
        }
        else
        {
                $department_ids = ("'" . $auth->department . "'");
        }


	/*
         Find the users contact_id, and use this to find their e-mail address.
        */

        $users_contact_id = getContactID($auth->department, $auth->login);
        $user_email = getCiEmailUsingID($users_contact_id);

	/*
	 Go through the array of students, and if they have no e-mail address, put them in
	 a noemail_students array so the administrator can be warned.
	*/

	for ($i = 0; $i < sizeof($email_students); $i++)
	{
		if (!$email_students[$i]["email"])
		{
			$noemail_students[] = $email_students[$i]["name"];
		} else
		{
			$yesemail_students[] = $email_students[$i]["email"];
		}
	}
	if (sizeof($noemail_students) > 1)
	{
		for ($i = 0; $i < (sizeof($noemail_students) - 1); $i++)
		{
			$email_warn .= $noemail_students[$i] . ", ";
		}
		$email_warn .= "and " . $noemail_students[$i] . " do not have Netlink or login IDs.  You will need to enter an e-mail address for these students in the BCC field.  Separate e-mail addresses with commas if necessary.";
	} elseif (sizeof($noemail_students) == 1)
	{
		$email_warn = $noemail_students[0] . " does not have a Netlink or login ID.  You will need to enter an e-mail address for this student in the BCC field.  Separate e-mail addresses with commas if necessary.";
	}

	if ($email_warn)
	{
		error($email_warn);
	}

	/*
	 Pull all of the users eligible as contacts for this interview out of the database.  This list includes
	 anyone that is a part of the current users group and allows their name to be used as a contact,
	 as well as the default contact for the interview (if not included in the first group).
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


	?>
	
	<h4>Interview date change for <?php echo($email_job["job_code"]); ?>:</h4>
	
	<form action="<?php echo(($return_to ? $return_to : $PHP_SELF)); ?>" method="post" enctype="multipart/form-data">
		<input type="hidden" name="select" value="edit" />
		<input type='hidden' name='level1' value='timedate' />
		<input type='hidden' name='last_modified' value='<?php echo $interview->last_modified?>' />
		<input type='hidden' name='choice' value='Email shortlist' />
		<input type='hidden' name='interview' value='<?php echo(packObject(($interview))); ?>' />
		<input type='hidden' name='job_id' value='<?php echo($job_id); ?>' />
		<input type='hidden' name='submit' value='Send Date Email' />
		<input type='hidden' name='students_dropped' value='<?php echo(packObject(($students_dropped))); ?>' />
		<table class='row1' cellspacing='0' cellpadding='4' border='0'>
			<tr><td>To:</td>
			<td colspan='2'><input type="text" name="to" size='70' value="<?php echo($user_email); ?>" /></td></tr>
			<tr><td>Cc:</td>
			<td colspan='2'><input type="text" size='70' name="cc" /></td></tr>
			<tr><td>Bcc:</td>
			<td colspan='2'>
			<textarea name='bcc' cols='70' rows='4'><?php
			for ($i = 0; $i < (sizeof($yesemail_students) - 1); $i++)
			{
				echo($yesemail_students[$i] . ", ");
			}
			echo($yesemail_students[$i]);
			?></textarea>
			</td></tr>
			<tr><td>From:</td>
			<td colspan='2'><?php echo(($user_email) ? $user_email : BRAND_NAME_SYSTEM_EMAIL); ?></td></tr>
			<tr><td valign='top'>Reply to:</td><td valign='top'>
				<select name="reply_to">
				<?php

				while ($row = $result->FetchRow())
				{
					echo("<option value='" . $row["contact_id"] . "'");
					if ($interview->contact == $row["contact_id"])
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
			<tr><td>Subject:</td><td colspan='2'><input type="text" name="subject" size='55' value='Notification of interview date change for job <?php echo($email_job["job_code"]); ?>' /></td></tr>
			<tr><td colspan="3"><textarea cols="70" rows="20" name="body"><?php echo(create_body2($job_id)); ?></textarea></td></tr>
			<tr><td colspan="3" align="center"><hr />
				<input type="submit" name="email_employer" value="Send" />
				</td></tr>
		</table>
	</form>
	<?php
}


function send_email($job_id, $to, $cc, $bcc, $subject, $body, $from, $reply_to, $reply_id)
{

	global $PHP_SELF;
	global $job_id;
	global $include_after;
	global $case;
	global $return_to;
	global $interview;
	global $email_job;
	global $email_students;

	global $department_in_str;
	
	include('misc/email.inc');
	
	$mail = new email($to, $subject, unslash($body));
		
	$mail->from = $from;
	$mail->cc = $cc;
	$mail->bcc = $bcc;
	$mail->headers = "Reply-To: " . $reply_to . "\n";
    $mail->reply_id = $reply_id;
	
	if (!$mail->send())
	{
		error($mail->error_text . "  Please push the BACK button on your browser and try again.");
		unset($mail);
		return false;
	}
	else
	{
		unset($mail);	
		return true;
	}

}











function create_body2($job_id)
{
	$sql = ("
		SELECT DISTINCT a.job_code, b.company_name
		FROM job_info AS a, employer_company AS b
		WHERE a.job_id='" . $job_id . "'
		AND b.employer_id=a.employer_id
		");
	$result = $GLOBALS['dbh']->Execute($sql);
	$row = $result->FetchRow();
	
	$email_job = array(
		"job_code" => $row["job_code"],
		"company_name" => $row["company_name"]
		);

	$sql = ("
		SELECT DISTINCT int_date
		FROM interview_time 
		WHERE job_id='" . $job_id . "'
		ORDER BY int_date
		");
	$result = $GLOBALS['dbh']->Execute($sql);
	while ($row = $result->FetchRow())
	{
		$new_dates[] = $row["int_date"];
	}
		
	// instructions
		$output .= ("This e-mail has been sent to notify you of a change of dates in an interview you have been shortlisted for.\n\n");
		$output .= ("Details of interview:\n\n");

		$output .= ("Job Code: " . $email_job["job_code"] . "\n");
		$output .= ("Company Name: " . $email_job["company_name"] . "\n\n");
		
		if (sizeof($new_dates) > 1)
		{
			$output .= "The new interview dates are:\n";
			for ($i = 0; $i < sizeof($new_dates); $i++)
			{
				$output .= formatLongDate($new_dates[$i]) . "\n";
			}
			$output .= "\n";
		} else
		{
			$output .= "The new interview date is " . formatLongDate($new_dates[0]) . ".\n\n";
		}

		$output .= "Please go to our Interview Sign-up web-site at \n";
		$output .= ORGANIZATION_DB_URL." to select an interview time.";
	
	return $output;
}

?>
