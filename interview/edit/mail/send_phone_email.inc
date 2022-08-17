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
 | Filename: send_phone_email.inc                                               |
 +------------------------------------------------------------------------------+
 | Description: This file is called when an off-campus phone interview is       |
 | changed to on-campus, provided there are students that already entered       |
 | their own phone numbers.  It grabs their e-mail addresses from the database  |
 | and lets them know that they will now be expected to have the interview on   |
 | campus.                                                                      |
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
	global $phone_changed;
	global $noemail_studentnum;
	global $noemail_studentname;

	/*
	 Find the students e-mail addresses and the students without e-mail addresses.
	*/

	for ($i = 0; $i < sizeof($phone_changed); $i++)
	{

	/* 
	 If a student does not have an associated department in the database
	 the pull out his student number and name to notify the secretary
	 that they will need to manually enter an e-mail address.
	*/

		$result = $GLOBALS['dbh']->Execute("
			SELECT DISTINCT first_name, last_name, student_number
			FROM student
			WHERE student_number='" . trim($phone_changed[$i]["student_number"]) . "'
			");
		$row = $result->FetchRow();
	
		if(getStudentEmail($phone_changed[$i]["student_number"]))
                {       
                        $studentemail[] = getStudentEmail($phone_changed[$i]["student_number"]);
                }
		else
		{
			$noemail_studentnum[] = $row["student_number"];
			$noemail_studentname[] = $row["first_name"] . " " . $row["last_name"];
		}
	}

	/*
	 Job has changed, so clear emailed_employer flag.
	*/
	
	$email_sql = ("
		UPDATE interview
		SET emailed_employer='0'
		WHERE job_id='" . $job_id . "'
		");
	$email_result = $GLOBALS['dbh']->Execute($email_sql);
		
	/*
	 Find the users contact_id, then use this to find the users e-mail address.
	*/

	$users_contact_id = getContactID($auth->department, $auth->login);
	$users_email = getCiEmailUsingID($users_contact_id);
	
	/*
	 For each student without a known e-mail address, warn the user that they will need
	 to manually enter their address into the BCC field.
	*/

	if (sizeof($noemail_studentname) > 1)
	{
		for ($i = 0; $i < (sizeof($noemail_studentname) - 1); $i++)
		{
			$emailwarn .= $noemail_studentname[$i] . ", ";
		}
		$emailwarn .= "and " . $noemail_studentname[$i];
		$emailwarn .= " do not have e-mail addresses registered in the database.  You will need to enter their e-mail addresses into the Bcc field (separate the addresses with a comma).";
	}
	elseif (sizeof($noemail_studentname) == 1)
	{
		$emailwarn .= $noemail_studentname[0] . " does not have an e-mail address registered in the database.  You will need to enter their e-mail address into the Bcc field.  If necessary, separate addresses with a comma.";
	}
	
	echo("<br />");
	$msg = ("<b class=instruct>Warning:</b> The changes to the interview have already been saved.  It is important that you send this e-mail to students that had previously");
	$msg .= (" signed up with a phone number so that they know they must now come to campus for the interview.");
	instruct($msg);
	
	/*
         Pull all of the users eligible as contacts for this interview out of the database.  This list includes
         anyone that is a part of the current users group and allows their name to be used as a contact,
         as well as the default contact for the interview (if not included in the first group).
        */

	$sql = ("
		SELECT DISTINCT	a.login_id, b.contact_id, CONCAT(b.first_name, ' ', b.last_name) AS name, b.email 
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

	echo("<h4>Notification of interview location change:</h4>");

	if ($emailwarn)
	{
		error($emailwarn);
	}

	?>
	<form action="<?php echo($PHP_SELF); ?>" method="post" enctype="multipart/form-data">
		<input type="hidden" name="select" value="edit" />
		<input type="hidden" name="job_id" value="<?php echo($job_id); ?>" />
		<input type='hidden' name='level1' value='jobinfo' />
		<input type='hidden' name='interview' value='<?php echo(packObject(($interview))); ?>' />
		<input type='hidden' name='students' value='<?php echo($interview->students);?>' />
		<input type='hidden' name='submit' value='Send Email' />
			<table class='row1' border='0' cellspacing='0' cellpadding='4'>
			<tr><td>To:</td>
			<td colspan='2'><input type="text" name="to" size='70' value="<?php echo($users_email); ?>" /></td></tr>
			<tr><td>Cc:</td>
			<td colspan='2'><input type="text" name="cc" size='70' /></td></tr>
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
			<td colspan='2'><?php echo(($users_email) ? $users_email : BRAND_NAME_SYSTEM_EMAIL); ?></td></tr>
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
			<tr><td>Subject:</td><td colspan='2'><input type="text" name="subject" size='55' value="Notification of change" /></td></tr>
			<tr><td colspan="3"><textarea cols="60" rows="20" name="body"><?php echo(create_body2($job_id)); ?></textarea></td></tr>
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
		error($mail->error_text . "  Please push the BACK button on your browser and try again.");
	}	
	else
	{
		$msg = ("The interview has been changed from an <b class='black'>off-campus</b> phone interview to an <br />");
		$msg .= ("<b class='black'>on-campus</b> phone interview, and the e-mail has been successfully sent.");
		notify($msg);
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
		$output .= "This e-mail has been sent to notify you that an interview format has changed from an off-campus phone interview to an on-campus phone interview.  You will now be expected to come to campus for the phone interview.  \n\n";
	
		$output .= "Details of the interview: \n\n";
		$output .= "Job Code: ";
		$output .= $job_code;
		$output .= "\nCompany name: ";
		$output .= $company_name;
		
	return $output;
}

?>
