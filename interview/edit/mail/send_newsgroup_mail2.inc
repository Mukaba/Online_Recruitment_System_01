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
 | Filename: send_newsgroup_mail2.inc                                           |
 +------------------------------------------------------------------------------+
 | Description: This file is called by send_newsgroup_mail.inc after the        |
 | shortlisted students have been sent e-mail notification of their upcoming    |
 | interview.  This file is only called if the department this interview is     |
 | for has a newsgroup associated with it or it's co-op group.                  |
 +------------------------------------------------------------------------------+

*/


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
			if (send_email2($job_id, $to, $cc, $bcc, $subject, $body, $from, $reply_to, $reply_id))
			{ 
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

				if ($include_after == "interview/add/show_data.inc")
				{
					if ($case == "post and email")
					{
						notify("Interview has been posted, and the shortlist has been e-mailed to students and newsgroup(s).");
					} 
					elseif ($case == "email")
					{
						notify("Interview shortlist has been e-mailed to students and newsgroup(s).");
					}

					include($include_after);
				} 
				else
				{
					$select = edit;
					$choice = "";

					notify("Interview shortlist has been e-mailed to students and newsgroup(s).");

					echo("<table border='0' cellspacing='0' cellpadding='10' width='100%'><tr><td>");
					include($include_after);
				}
			}
			$GLOBALS['dbh']->Execute("REPLACE INTO interview_edit(job_id, date_edited, editor, menu_edited)
					VALUES('" . $job_id . "', NULL, '" . addslashes($auth->login) . "', 'Button')");
			
			break;
		}
	
	case "create":
	default:
		display_email_form2($job_id, $department_id);
		break;
}




function display_email_form2($job_id, $department_id)
{
	global $PHP_SELF;
	global $auth;
	global $interview;
	global $return_to;
	global $include_after;
	global $case;
	global $email_students;
	global $email_job;
	global $departments_in_group;

	/*
	 Find the department ids of all the departments that belong to the current department's group.
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
         Find the users contact_id, and use this to find the users e-mail address.
        */

        $users_contact_id = getContactID($auth->department, $auth->login);
        $user_email = getCiEmailUsingID($users_contact_id);
	
	/*
	 Find the newsgroup email.
	*/

        $sql = ("
                SELECT DISTINCT newsgroup_email
                FROM department
                WHERE newsgroup_email IS NOT NULL
		AND newsgroup_email!=''
                AND department_id IN (" . $department_ids . ")
                ");
        $news_result = $GLOBALS['dbh']->Execute($sql);

	while ($news_row = $news_result->FetchRow())
	{
		$newsgroup_email[] = $news_row["newsgroup_email"];
	}
	$news_result->Close();

	/*
	 Pull the default contact for this interview out of the database.
	*/

	$sql = ("
		SELECT DISTINCT contact
		FROM interview
		WHERE job_id='" . $email_job["job_id"] . "'
		");
	$contact_result = $GLOBALS['dbh']->Execute($sql);
	$contact_row = $contact_result->FetchRow();

	$default_contact = $contact_row["contact"];

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
            OR (i.contact=b.contact_id AND i.job_id='" . $email_job["job_id"] . "')
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
	<h4>Interview Newsgroup Notification for <?php echo($email_job["job_code"]); ?>:</h4>
	
	<form action="<?php echo(($return_to ? $return_to : $PHP_SELF)); ?>" method="post" enctype="multipart/form-data">
		<input type="hidden" name="select" value="edit" />
		<input type="hidden" name="mail[0]" value='<?php echo($email_job["job_id"]);?>' />
		<input type='hidden' name='department_id' value='<?php echo $department_id?>' />
		<input type='hidden' name='last_modified' value='<?php echo $interview->last_modified?>' />
		<input type='hidden' name='choice' value='Email shortlist2' />
		<input type='hidden' name='interview' value='<?php echo(packObject(($interview))); ?>' />
		<input type='hidden' name='case' value='<?php echo($case); ?>' />
		<table class='row1' cellspacing='0' cellpadding='4' border='0'>
			<tr><td>To:</td>
			<td colspan='2'><input type="text" name="to" size='70' value="<?php 
			if (sizeof($newsgroup_email) == 1)
			{
				echo($newsgroup_email[0]);
			}
			else
			{
				for ($i = 0; $i < (sizeof($newsgroup_email) - 1); $i++)
				{
					echo($newsgroup_email[$i] . ", ");
				}
				echo($newsgroup_email[$i]);
			}
			?>" /></td></tr>
			<tr><td>Cc:</td>
			<td colspan='2'><input type="text" size='70' name="cc" /></td></tr>
			<tr><td>Bcc:</td>
			<td colspan='2'>
			<textarea name='bcc' cols='70' rows='4'><?php echo($user_email); ?></textarea>
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
				</select>
			</td>
			<td valign='top'> Contact information for this address will be automatically appended to the e-mail.</td>
			</tr>
			<tr><td>Subject:</td><td colspan='2'><input type="text" name="subject" size='55' value='Interview Shortlist for job <?php echo($email_job["job_code"]); ?>' /></td></tr>
			<tr><td colspan="3"><textarea cols="70" rows="20" name="body"><?php echo(create_body($job_id)); ?></textarea></td></tr>
			<tr><td colspan="3" align="center"><hr />
				<table border='0'>
				<tr>
				<td>
				<table border='0'>
					<tr>
					<td>
					<input type="submit" name="email_employer" value="Send" />
					</form>
					</td>
					</tr>
				</table>
				</td>
				<td>
				<table border='0'>
					<tr>
					<td>
					<form method='post' action='<?php echo($PHP_SELF); ?>'>
					<input type='hidden' name='select' value='edit' />
					<input type="submit" name="email_employer" value="Don't Send" />
					</form>
					</td>
					</tr>
				</table>
				</td>
				</tr>
				</table>
			</td></tr>
		</table>
	<?php
}


function send_email2($job_id, $to, $cc, $bcc, $subject, $body, $from, $reply_to, $reply_id)
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











function create_body($job_id)
{
	global $email_job;
	global $email_students;

	
	// instructions
		$output .= ("Interviews have been scheduled for the following job:\n\n");
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

		$output .= ("If you have been shortlisted for this job, you will be notified via e-mail.\n\n");

		$output .= "Please go to our Interview Sign-up web-site at \n";
		$output .= ORGANIZATION_DB_URL." to select an interview time.";
	
	return $output;
}

?>
