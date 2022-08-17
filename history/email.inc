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
 | Filename: email.inc                                                          |
 +------------------------------------------------------------------------------+
 | Description: This file sends out an e-mail to matches drawn from history     |
 | results (I think, I really have no clue what Chris is doing here).           |
 +------------------------------------------------------------------------------+

*/

include("misc/table2.inc");

switch ($email_lunch)
{
	case "Send":
		global $auth;

		$from = ($from = getCiEmailUsingID(getContactID($auth->department, $auth->login))) ? $from : BRAND_NAME_SYSTEM_EMAIL;
        $reply_id = $reply_to;
		$reply_to = ($reply_to = getCiEmailUsingID($reply_to)) ? $reply_to : "";
		
		if (send_email($job_id, $to, $cc, $bcc, $subject, $body, $from, $reply_to, $reply_id))
		{ 
            include('history/advanced_search_form.inc');
		}
		break;
	
	default:
		include ('history/functions.inc');
		display_email_form($bcc);
		break;
}




function display_email_form($bcc)
{
	global $PHP_SELF;
	global $auth;
	global $interview;
	global $return_to;
	global $case;
	global $email_students;
	global $departments_in_group;
	global $time_id;
	global $cid;
	
	global $searchHistory;
	global $students_without_email;
	
        // Find the users contact_id.
        $users_contact_id = getContactID($auth->department, $auth->login);
	
        // Use this contact id to find the users email.
	$users_email = getCiEmailUsingID($users_contact_id);

	// Pull out all of the information that we need for the current user.
	$sql = ("
		SELECT DISTINCT a.login_id, a.netlink_id, a.contact_id, CONCAT(b.first_name, ' ', b.last_name) AS name, b.email
		FROM contact_internal AS a, contact AS b
		WHERE a.department_id='" . addslashes($auth->department) . "'
		AND a.contact_id=b.contact_id AND
		(a.general_email='1' OR (a.login_id='".addslashes($auth->login)."' OR a.netlink_id='".addslashes($auth->login)."'))
		ORDER BY b.last_name, b.first_name
		");
	$result = $GLOBALS['dbh']->Execute($sql);

	$anArray=explode("-",$email_row['int_date']);
	$thedate=date("l F d, Y",mktime(0,0,0,$anArray[1],$anArray[2],$anArray[0]));	

	// Restore the student without e-mail array, and grab each of their names.
	if ($students_without_email)
	{
		if (is_string($students_without_email))
		{
			$students_without_email = unpackObject($students_without_email);
		}

        if(sizeof($students_without_email) > 0) 
        {
            foreach ($students_without_email AS $swe)
            {
                $name_sql = ("
                    SELECT DISTINCT CONCAT(first_name, ' ', last_name) AS student_name
                    FROM student
                    WHERE student_number='" . $swe . "'
                    ");
                $name_result = $GLOBALS['dbh']->Execute($name_sql);
                $student_row = $name_result->FetchRow();
                $student_names_no_email .= ($student_row["student_name"] . ", ");
            }

            $student_names_no_email = substr($student_names_no_email, 0, -2);
        }
        else 
        {
            $student_names_no_email = NULL;
        }
            
	}

	echo("</td>");
	echo("<td align='left'>");
	echo("<img src= '".$GLOBALS['colorscheme']['topbar_right']."' alt='' />");
	echo("</td>");
	echo("</tr>");
	echo("</table>");

	echo("<h3>History - E-mail Students</h3>");
	
	if ($student_names_no_email)
	{
		$warn = ("The following students did not have an e-mail address registered in the system: ");
		$warn .= ("<b>" . $student_names_no_email . "</b>");
		$warn .= ("  These students will not receive this e-mail.");

		notify($warn);
	}
?>

	<form action="<?php echo $PHP_SELF."&amp;select=email_students" ?>" method="post" enctype="multipart/form-data">
		<input type='hidden' name='department_id' value='<?php echo $department_id?>' />
		<table class='row1' cellspacing='0' cellpadding='4' border='0'>
			<tr><td>To:</td>
			<td colspan='2'><input type="text" name="to" size='70' value="<?php echo($users_email); ?>" /></td></tr>
			<tr><td>Cc:</td>
			<td colspan='2'><input type="text" size='70' name="cc" /></td></tr>
			<tr><td>Bcc:</td>
			<td colspan='2'>
			<textarea name='bcc' cols='70' rows='4' WRAP><?php echo $bcc?></textarea>
			</td></tr>
			<tr><td>From:</td>
			<td colspan='2'><?php echo(($users_email) ? $users_email : BRAND_NAME_SYSTEM_EMAIL); ?></td></tr>
			<tr><td valign='top'>Reply to:</td><td valign='top'>
				<select name="reply_to">
				<?php
				
				while ($row = $result->FetchRow())
				{
					echo("<option value='" . $row["contact_id"] . "'" . (($auth->contact_id == $row["contact_id"]) ? " selected='selected'" : "") . ">" . $row["name"] . "</option>\n");
				}
				$result->Close();
							
				?>
				</select><br /> Contact information for this address will be automatically appended to the e-mail.
			</td>
			<td>&nbsp;</td>
			</tr>
			<tr><td>Subject:</td><td colspan='2'><input type="text" name="subject" size='55' /></td></tr>
			<tr><td colspan="3"><textarea cols="70" rows="20" name="body" wrap="soft"></textarea></td></tr>
			<tr><td colspan="3" align="center"><hr />

			<input type='hidden' name='searchHistory' value='<?php echo $searchHistory?>' />        
			<input type='hidden' name='btnSearch' value='Search' />
			<input type='hidden' name='searchmode' value='advanced' />
			<input type='hidden' name='select' value='history_email_students' />
			<input type="hidden" name="message_sent" value="yes" />
			<input type="submit" name="email_lunch" value="Send" />
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
		echo ("</td><td align='left'><img src='".$GLOBALS['colorscheme']['topbar_right']."' alt='' /></td></tr></table>");
        
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
