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
 | Filename: send_dropped_email.inc                                             |
 +------------------------------------------------------------------------------+
 | Description: This file is called by edit_datetime.inc when an interview      |
 | time slot that a student was signed up for is cancelled or deleted in some   |
 | way. This form allows the secretary to send the student e-mail notifying     |
 | him of this change.                                                          |
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
				
		send_email($job_id, $to, $cc, $bcc, $subject, $body, $from, $reply_to, $reply_id);
		unset($studentemail);
		unset($students_dropped);
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
	global $students_dropped;
	global $studentemail;
	global $noemail_studentnum;
	global $noemail_studentname;

	/*
	 Update the interview in the database to no longer show that the employer has been e-mailed.
	*/

	$email_sql = ("
		UPDATE interview
		SET emailed_employer='0',
		emailed_secretary='0'
		WHERE job_id='" . $job_id . "'
		");
	$email_result = $GLOBALS['dbh']->Execute($email_sql);

	/*
         Find the users contact_id, and use this to find their e-mail address.
        */

        $users_contact_id = getContactID($auth->department, $auth->login);
        $user_email = getCiEmailUsingID($users_contact_id);

	/*
	 For each of the students without e-mail addresses, warn the user so that they
	 know they will need to manually enter their address in the BCC field.
	*/

	if (sizeof($noemail_studentname) > 1)
	{
		for ($i = 0; $i < (sizeof($noemail_studentname) - 1); $i++)
		{
			$emailwarn .= $noemail_studentname[$i] . ", ";
		}
		$emailwarn .= "and " . $noemail_studentname[$i];
		$emailwarn .= " do not have e-mail addresses registered in the database.  You will need to enter their e-mail addresses into the Bcc field (sepearate the addresses with a comma).";
	} 
	elseif (sizeof($noemail_studentname) == 1)
	{
		$emailwarn .= $noemail_studentname[0] . " does not have an e-mail address registered in the database.  You will need to enter their e-mail address into the Bcc field.  If necessary, separate addresses with a comma.";
	}
	
	instruct("<b class=instruct>Warning:</b> The schedule changes have already been saved.  It is important that you send this e-mail to any students that have been dropped from interviews they had signed up for.");

	echo("<h4>Notification of drop from signed up interview slot:</h4>");

	if ($emailwarn)
	{
		error($emailwarn);
	}

	/*
	 If the default interview contact has not already been set, pull it from the database.
	*/

	if (!$interview->contact)
	{
		$sql = ("
			SELECT DISTINCT contact
			FROM interview
			WHERE job_id='" . $job_id . "'
			");
		$contactresult = $GLOBALS['dbh']->Execute($sql);
		$contactrow = $contactresult->FetchRow();
	
		$contact = $contactrow["contact"];
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

	<form action="<?php echo($PHP_SELF); ?>" method="post" enctype="multipart/form-data">
		<input type="hidden" name="select" value="edit" />
		<input type="hidden" name="job_id" value="<?php echo($job_id); ?>" />
		<input type='hidden' name='interview' value='<?php echo(packObject(($interview))); ?>' />
		<input type='hidden' name='level1' value='timedate' />
		<input type='hidden' name='submit' value='Send email' />
		<table border='0' class='row1' cellspacing='0' cellpadding='4'>
			<tr><td>To:</td>
			<td colspan='2'><input type="text" name="to" size='70' value="<?php echo($user_email); ?>" /></td></tr>
			<tr><td>Cc:</td>
			<td colspan='2'>
			<input type="text" name="cc" size='70' /></td></tr>
			<tr><td>Bcc:</td>
			<td colspan='2'>
			<textarea name='bcc' cols='70' rows='4'><?php
                        for ($i = 0; $i < (sizeof($studentemail) - 1); $i++)
                        {
                                echo($studentemail[$i] . ", ");
                        }
                        echo($studentemail[$i]);
                        ?></textarea>
			</td></tr>
			<tr><td>From:</td>
			<td colspan='2'><?php echo(($user_email) ? $user_email : BRAND_NAME_SYSTEM_EMAIL); ?></td></tr>
			<tr><td valign='top'>Reply to:</td><td valign='top'>
				<select name="reply_to">
				<?php

				while ($row = $result->FetchRow())
				{
					echo(" <option value='" . $row["contact_id"] . "'");
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
			<tr><td>Subject:</td><td colspan='2'><input type="text" name="subject" size='55' value="Notification of drop" /></td></tr>
			<tr><td colspan="3"><textarea cols="60" rows="20" name="body"><?php echo(create_body2($job_id)); ?></textarea></td></tr>
			<tr><td colspan="3" align="center"><hr />
			<input type='submit' name='email_employer' value='Send' />
			</td></tr>
		</table>
	</form>
	<?php
}


function send_email($job_id, $to, $cc, $bcc, $subject, $body, $from, $reply_to, $reply_id)
{

	global $PHP_SELF;
	global $interview;
	global $students;
	global $job_id;
	include('misc/email.inc');
	
	$mail = new email($to, $subject, unslash($body));
		
	$mail->from = $from;
	$mail->cc = $cc;
	$mail->bcc = $bcc;
	$mail->headers = "Reply-To: " . $reply_to . "\n";
    $mail->reply_id = $reply_id;
	
	if (!$mail->send())
	{
		error($mail->error_text." Please push the BACK button on your browser and try again.");
	}	
	else
	{
		notify("Schedule has been updated, and e-mail was sent successfully.");
	}
	unset ($mail);

}











function create_body2($job_id)
{
	$result = $GLOBALS['dbh']->Execute("
		SELECT	a.job_code, b.company_name
		FROM	job_info AS a, employer_company AS b
		WHERE	a.job_id='" . addslashes($job_id) . "' AND
			a.employer_id=b.employer_id");

	if (!$result) { return false; }
	if ($row = $result->FetchRow())
	{
		$job_code = $row["job_code"];
		$company_name = $row["company_name"];
	}
	$result->Close();
	if (!$row) { return false; }
	
	
	// instructions
		$output .= "This e-mail has been sent to notify you that an interview slot you had signed up for has now been changed.  You will need to sign yourself up again in a new interview slot. \n\n";
	
		$output .= "Details of the interview: \n\n";
		$output .= "Job Code: ";
		$output .= $job_code;
		$output .= "\nCompany name: ";
		$output .= $company_name;
		
	return $output;
}

?>
