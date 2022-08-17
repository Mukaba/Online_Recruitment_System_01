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
 | Filename: send_newsgroup_email.inc                                           |
 +------------------------------------------------------------------------------+
 | Description: This file is called by add job module after an interview has    |
 | been posted. After this file, if the department associated with this         |
 | interview has a newsgroup, send_newsgroup_email2.inc is called, otherwise    |
 | the show_data.inc file is called.                                            |
 +------------------------------------------------------------------------------+

*/

include("misc/table2.inc");


switch ($email_employer)
{
	case "pass_through":

                // This case happens when the code wants to pass through this file and load send_newsgroup_email2.inc.
                // This statement allows the functions in this file to be used by the above mentioned file.

                $email_employer = 'Send';
                include('interview/add/send_newsgroup_email2.inc');
                break;

	case "Send":
		global $auth;

		$from = ($from = getCiEmailUsingID(getContactID($auth->department, $auth->login))) ? $from : BRAND_NAME_SYSTEM_EMAIL;
        $reply_id = $reply_to;
		$reply_to = ($reply_to = getCiEmailUsingID($reply_to)) ? $reply_to : "";		
				
		if(send_email($job_id, $to, $cc, $bcc, $subject, $body, $from, $reply_to, $reply_id));
		{

			$GLOBALS['dbh']->Execute("
				UPDATE interview 
				SET emailed_newsgroup='1'
				WHERE job_id='" . $job_id . "' 
				");
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
			 Find out if this department, or a department in it's co-op group has a newsgroup.
			*/
	
			$sql = ("
				SELECT DISTINCT newsgroup_email
				FROM department
				WHERE department_id IN (" . $department_ids . ")
				");
			$result = $GLOBALS['dbh']->Execute($sql);
			$row = $result->FetchRow();
	
			if ($row["newsgroup_email"])
			{
				/*
				 The department has a newsgroup associated with it or its group, so load the next page up.
				*/
		
				$msg = ("Interview has been posted, and notification has been successfully e-mailed to the shortlisted");
				$msg .= (" students.  If you <b class='black'>do not</b> wish to send an additional e-mail to the newsgroup, click on the ");
				$msg .= ("<b class='black'>Don't Send</b> button below.");
	
				instruct($msg);
	
				/*
				 Unset the mail variables.
				*/
	
				unset($email_employer);

				$email_job = unpackObject(($email_job));
		        	$email_students = unpackObject(($email_students));
		        	for ($i = 0; $i < sizeof($email_students); $i++)
			        {
		               		$email_students[$i] = unpackObject(($email_students[$i]));
		        	}
	
				include('interview/add/send_newsgroup_email2.inc');
			}
			else
			{
				$msg = ("Interview has been posted, and notification has been successfully e-mailed to the shortlisted students.");
				notify($msg);

				include('interview/add/show_data.inc');
			}	
		}

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
	global $interview;
	global $email_students;
	global $email_job;

	/*
	 Set email_job_id for e-mailing newsgroup.
	*/

	$email_job_id = $job_id;

	/*
	 Set email_students array, which contains name, student_number, and 
	 e-mail for all students shortlisted for this job.
	*/

	$sql = ("
		SELECT DISTINCT CONCAT(a.first_name, ' ', a.last_name) AS name, a.student_number, a.login_id
		FROM student AS a, students_shortlisted AS b
		WHERE b.job_id='" . addslashes( $email_job_id ) . "'
		AND a.student_number=b.student_number
		");
	$result = $GLOBALS['dbh']->Execute($sql);

	while ($row = $result->FetchRow())
	{
		$email_address = getStudentEmail($row["student_number"]);
		
		$email_students[] = array(
			"name" => $row["name"],
			"student_number" => $row["student_number"],
			"email" => $email_address
			);
	}
	$result->Close();

	/*
	 Set email_job array, which contains job_id, job_code, int_dates, and
	 the interview medium.
	*/

	$sql = ("
		SELECT DISTINCT int_date
		FROM interview_time
		WHERE job_id='" . addslashes($email_job_id) . "'
		");
	$result = $GLOBALS['dbh']->Execute($sql);
	while ($row = $result->FetchRow())
	{
		$email_int_dates[] = $row["int_date"];
	}
	$result->Close();

	$sql = ("
		SELECT DISTINCT a.job_code, b.company_name
		FROM job_info AS a, employer_company AS b
		WHERE a.job_id='" . addslashes($email_job_id) . "'
		AND b.employer_id=a.employer_id
		");
	$result = $GLOBALS['dbh']->Execute($sql);
	$row = $result->FetchRow();
	$result->Close();
	$email_job_code = $row["job_code"];
	$email_company_name = $row["company_name"];

	$sql = ("
		SELECT DISTINCT a.description
		FROM interview_medium AS a, interview AS b
		WHERE b.job_id='" . addslashes($email_job_id) . "'
		AND a.interview_medium_id=b.interview_medium_id
		");
	$result = $GLOBALS['dbh']->Execute($sql);
	$row = $result->FetchRow();
	$result->Close();
	$email_job_medium = $row["description"];

	/*
	 Now we have all the variables, just assign them to the job array.
	*/

	$email_job = array(
		"job_id" => $email_job_id,
		"job_code" => $email_job_code,
		"company_name" => $email_company_name,
		"medium" => $email_job_medium,
		"int_dates" => $email_int_dates
		);
	

	/*
         Find the users contact_id.
        */

        $users_contact_id = getContactID($auth->department, $auth->login);

        /*
         Use this contact id to find the users e-mail.
        */

        $user_email = getCiEmailUsingID($users_contact_id);

	$sql = ("
		SELECT DISTINCT a.login_id, a.netlink_id, a.contact_id, CONCAT(b.first_name, ' ', b.last_name) AS name, b.email
		FROM contact_internal AS a, contact AS b
        WHERE a.department_id IN ('" . implode(department_groups($auth->department), "', '"). "')
		AND (a.interview_contact_list OR a.contact_id='".$users_contact_id."')
		AND a.contact_id=b.contact_id
		ORDER BY b.last_name, b.first_name
		");
	$result = $GLOBALS['dbh']->Execute($sql);

	if (!$result) 
	{ 
		return false; 
	}

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

	$sql = ("
		SELECT DISTINCT newsgroup_email
		FROM department
		WHERE newsgroup_email IS NOT NULL
		AND newsgroup_email!=''
		AND department_id IN (" . $department_ids . ")
		");
	$news_result = $GLOBALS['dbh']->Execute($sql);

	if ($news_result->RecordCount())
	{
		while ($news_row = $news_result->FetchRow())
		{
			$newsgroup_email[] = $news_row["newsgroup_email"];
		}
		$news_result->Close();
	}
	else
	{
		/*
                 There is no default newsgroup for this department, so instead, send then
                 mail to the current user's e-mail address.
                */

                /*
                 Find the users contact_id.
                */

                $users_contact_id = getContactID($auth->department, $auth->login);

                /*
                 Use this contact id to find the users e-mail.
                */

                $newsgroup_email[] = getCiEmailUsingID($users_contact_id);

        }
	
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
	} 
	elseif (sizeof($noemail_students) == 1)
	{
		$email_warn = $noemail_students[0] . " does not have a Netlink or login ID.  You will need to enter an e-mail address for this student in the BCC field.  Separate e-mail addresses with commas if necessary.";
	}

	if ($email_warn)
	{
		error($email_warn);
		echo("<br />");
	}

	instruct("This interview has been added to the database.  By clicking the send button below, you will e-mail the shortlist"
	. " to the students.  If you do not wish to do this, do not click the send button.");

	?>
	<h3>Interview Shortlist E-mail for <?php echo($email_job["job_code"]); ?></h3>
	
	<form action="<?php echo($PHP_SELF); ?>" method="post" enctype="multipart/form-data">
		<input type='hidden' name='email_job' value='<?php echo(packObject(($email_job))); ?>' />
		<?php
		for ($i = 0; $i < sizeof($email_students); $i++)
		{
			$email_students[$i] = packObject(($email_students[$i]));
		}
		?>
		<input type='hidden' name='email_students' value='<?php echo(packObject(($email_students))); ?>' />
		<input type="hidden" name="select" value="add" />
		<input type="hidden" name="submit" value="Send off the e-mail" />
		<input type="hidden" name="job_id" value='<?php echo($job_id);?>' />
		<input type='hidden' name='choice' value='Email shortlist' />
		<input type='hidden' name='interview' value='<?php echo(packObject(($interview))); ?>' />
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
					echo(" <option value='" . $row["contact_id"] . "'" . ($interview->contact == $row["contact_id"] ? " selected='selected'" : ((($_SESSION['SESSION_netlog'] == SCRIPT_LOGIN && $auth->login == $row["netlink_id"]) || ($_SESSION['SESSION_netlog'] == NIS_LOGIN && $auth->login == $row["login_id"])) ? " selected='selected'" : "")) . ">" . $row["name"] . "</option>\n");
				}
				$result->Close();
							
				?>
				</select>
			</td>
			<td valign='top'> Contact information for this address will be automatically appended to the e-mail.</td>
			</tr>
			<tr><td>Subject:</td><td colspan='2'><input type="text" name="subject" size='55' value='Interview Shortlist for job <?php echo($email_job["job_code"]); ?>' /></td></tr>
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
	global $include_after;
	global $interview;
	global $email_job;
	global $email_students;
	global $auth;

	include('misc/email.inc');
	
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

	$sql = ("
		SELECT DISTINCT newsgroup_email
		FROM department
		WHERE newsgroup_email IS NOT NULL
		AND department_id IN (" . $department_ids . ")
		");
	$news_result = $GLOBALS['dbh']->Execute($sql);
	
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
                $GLOBALS['dbh']->Execute("
                        UPDATE interview
                        SET emailed_newsgroup='1'
                        WHERE job_id='" . $email_job["job_id"] . "'
                        ");
                unset($mail);
                return true;
        }

}

function create_body2($job_id)
{
	global $email_job;
	global $email_students;
	global $auth;

	
	// instructions
		$output .= ("You have been shortlisted for an upcoming interview.\n\n");
                $output .= ("Details of interview:\n\n");

                $output .= ("Job Code: " . $email_job["job_code"] . "\n");
                $output .= ("Company Name: " . $email_job["company_name"] . "\n\n");

                if (sizeof($email_job["int_dates"]) > 1)
                {
                        $output .= "The interview dates are:\n";
                        for ($i = 0; $i < sizeof($email_job["int_dates"]); $i++)
                        {
                                $output .= formatLongDate($email_job["int_dates"][$i]) . "\n";
                        }
                        $output .= "\n";
                } else
                {
                        $output .= "The interview date is " . formatLongDate($email_job["int_dates"][0]) . ".\n\n";
                }

		$sql_instructions = "SELECT student_interview_instructions from department where department_id='$auth->department'";
		$instruction_result=$GLOBALS['dbh']->Execute($sql_instructions);
		while ($instruction_row=$instruction_result->FetchRow()) {
                	$output .= $instruction_row["student_interview_instructions"];
		}

	return $output;
}

?>
