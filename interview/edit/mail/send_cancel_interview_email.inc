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
 | Filename: send_cancel_interview_email.inc                                    |
 +------------------------------------------------------------------------------+
 | Description: This file is called when an interview that has students         |
 | shortlisted for it has been cancelled.  This e-mail is only sent if the      |
 | interview has been posted, and the shortlist has been e-mailed out to        |
 | students.                                                                    |
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
		
		if (send_email($job_id, $to, $cc, $bcc, $subject, $body, $from, $reply_to, $reply_id))
		{ 
			notify("Notification of cancellation has been e-mailed to shortlisted students.");
		}
		break;
	
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
	global $return_to;
	global $case;
	global $departments_in_group;

	
	/*
	 We don't have the default contact for this interview yet, so pull it out of the database.
	*/
	
	$sql = ("
		SELECT DISTINCT contact
		FROM interview
		WHERE job_id='" . $job_id . "'
		");
	$result = $GLOBALS['dbh']->Execute($sql);
	$row = $result->FetchRow();
	
	$default_contact = $row["contact"];
	
	/*
         Set email_students array, which contains name, student_number, and 
         e-mail for all students shortlisted for this job.
        */      
                
        $sql = ("
                SELECT DISTINCT CONCAT(a.first_name, ' ', a.last_name) AS name, a.student_number
                FROM student AS a, students_shortlisted AS b
                WHERE b.job_id='" . addslashes($job_id) . "'
                AND a.student_number=b.student_number
                ORDER BY a.last_name
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
            
        /*
         Find the users contact_id, and use that to find the users e-mail address.
        */      
                
        $users_contact_id = getContactID($auth->department, $auth->login);
        $user_email = getCiEmailUsingID($users_contact_id);


	/*
	 Determine which students have e-mails, and which do not.  Put the students with e-mails into
	 an array.
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
		$email_warn .= ("and " . $noemail_students[$i] . " do not have login IDs, are not assigned to a department,");
		$email_warn .= (" or do not have valid e-mail addresses.  You will need to enter an e-mail address for these students in the BCC field.");
		$email_warn .= ("  Separate e-mail addresses with commas if necessary.");
	} 
	elseif (sizeof($noemail_students) == 1)
	{
		$email_warn = ($noemail_students[0] . " does not have a department, login ID, or e-mail address associated with him/her.  ");
		$email_warn .= ("You will need to enter an e-mail address for this student in the BCC field.  Separate e-mail addresses with commas if necessary.");
	}

	if ($email_warn)
	{
		error($email_warn);
	}

	/*
	 Grab all of the contacts that can be used in the contact list for this interview from the database.
	 Also, get the information for the contact that is the default contact for this interview.
	*/


    $sql = "SELECT DISTINCT b.email, a.login_id, a.netlink_id, a.contact_id, CONCAT(b.first_name, ' ', b.last_name) AS name
            FROM contact_internal AS a, contact AS b, interview AS i
            WHERE ( a.department_id IN ('1', '2') OR (i.contact=b.contact_id AND i.job_id=12044)) 
            AND (a.interview_contact_list OR a.contact_id=200050)
            AND a.contact_id=b.contact_id
            GROUP BY b.email
            ORDER BY b.last_name, b.first_name";
    
	$result = $GLOBALS['dbh']->Execute($sql);

	if (!$result) 
	{ 
		return false; 
	}

	?>
	<h4>Notification of Cancelled Interview:</h4>
	
	<form action="<?php echo($PHP_SELF); ?>" method="post" ENCTYPE="multipart/form-data">
		<input type="hidden" name="select" value="edit" />
		<input type='hidden' name='department_id' value='<?php echo $department_id?>' />
		<input type='hidden' name='last_modified' value='<?php echo $interview->last_modified?>' />
		<input type='hidden' name='interview' value='<?php echo(packObject(($interview))); ?>' />
		<input type='hidden' name='case' value='<?php echo($case); ?>' />
		<input type='hidden' name='submit' value='Confirm' />
		<input type='hidden' name='email_send' value='true' />
		<input type='hidden' name='job_id' value='<?php echo($job_id); ?>' />
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
					if ($default_contact == $row["contact_id"])
					{
						echo(" selected='selected'");
					}
					echo(">" . $row["name"] . "</option>\n");
				}
				$result->Close();
							
				?>
				</select><br /> Contact information for this address will be automatically appended to the e-mail.
			</td>
			<td>&nbsp;</td>
			</tr>
			<tr><td>Subject:</td><td colspan='2'><input type="text" name="subject" size='55' value='Notification of Cancelled Interview' /></td></tr>
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
	global $email_students;

	$sql = ("
		SELECT DISTINCT a.job_code, b.company_name
		FROM job_info AS a, employer_company AS b
		WHERE a.job_id='" . addslashes($job_id) . "'
		AND b.employer_id=a.employer_id
		");
	$result = $GLOBALS['dbh']->Execute($sql);
	$row = $result->FetchRow();
	
	// instructions

	$output .= ("This e-mail has been sent to notify you that an interview you had been shortlisted for has been CANCELLED.\n\n");
	
	$output .= ("Interview details:\n");
	$output .= ("Job Code: " . $row["job_code"] . "\n");
	$output .= ("Company Name: " . $row["company_name"] . "\n");

	return $output;
}

?>
