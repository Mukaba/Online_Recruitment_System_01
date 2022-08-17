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
 | Filename: send_shortlisted_students_email.inc                                |
 +------------------------------------------------------------------------------+
 | Description: This file is called when the user wishes to send the students   |
 | shortlisted for an interview a new e-mail.  This will only happen after the  |
 | initial shortlist has been e-mailed, at which point this option becomes      |
 | available.                                                                   |
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
			if ($include_after == "interview/add/show_data.inc")
			{
				notify("The e-mail has been sent to students successfully.");
				include($include_after);
			} 	
			else
			{
				$select = edit;
				$choice = "";
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
                                if ($news_result->RecordCount()) {
				  notify("Interview shortlist has been e-mailed to students and newsgroup(s).");
				} else {
				  notify("Notification has been e-mailed to shortlisted students.");
				}
				echo("<table border='0' cellspacing='0' cellpadding='10' width='100%'><tr><td>");
				include($include_after);
			}
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
	global $email_students;
	global $departments_in_group;

	// If we don't have the default contact for this interview yet, so pull it out of the database.

        if (!$interview->contact)
        {
                $sql = ("
                        SELECT DISTINCT contact
                        FROM interview
                        WHERE job_id='" . $job_id . "'
                        ");
                $result = $GLOBALS['dbh']->Execute($sql);
                $row = $result->FetchRow();

                $default_contact = $row["contact"];

                $interview->contact = $default_contact;
        }

        // Find the users contact_id, and then use this to find the user's e-mail address.
                
        $users_contact_id = getContactID($auth->department, $auth->login);
        $user_email = getCiEmailUsingID($users_contact_id);

	/*
	 For each student without a known e-mail address, warn the user that they
	 will have to manually enter their address into the BCC field.
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
		$email_warn .= ("and " . $noemail_students[$i] . " do not have Netlink or login IDs.  ");
		$email_warn .= ("You will need to enter an e-mail address for these students in the BCC field.");
		$email_warn .= ("  Separate e-mail addresses with commas if necessary.");
	} 
	elseif (sizeof($noemail_students) == 1)
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
	<h4>Send Shortlisted Students an E-mail:</h4>
	
	<form action="<?php echo(($return_to ? $return_to : $PHP_SELF)); ?>" method="post" enctype="multipart/form-data">
		<input type="hidden" name="select" value="edit" />
		<input type="hidden" name="mail[0]" value='<?php echo($email_job["job_id"]);?>' />
		<input type='hidden' name='department_id' value='<?php echo $department_id?>' />
		<input type='hidden' name='last_modified' value='<?php echo $interview->last_modified?>' />
		<input type='hidden' name='choice' value='Email shortlist' />
		<input type='hidden' name='interview' value='<?php echo(packObject(($interview))); ?>' />
		<input type='hidden' name='case' value='<?php echo($case); ?>' />
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
				</select><br /> Contact information for this address will be automatically appended to the e-mail.
			</td>
			<td>&nbsp;</td>
			</tr>
			<tr><td>Subject:</td><td colspan='2'><input type="text" name="subject" size='55' value='' /></td></tr>
			<tr><td colspan="3"><textarea cols="70" rows="20" name="body"></textarea></td></tr>
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


?>
