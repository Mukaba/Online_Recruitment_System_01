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
 | Filename: email_all_matches.inc                                              |
 +------------------------------------------------------------------------------+
 | Description: edited version of the send_lunch_email.inc in the interview     |
 | module                                                                       |
 +------------------------------------------------------------------------------+

//:TODO: This file carries a lot of useless variables that aren't used because
//       this code was copied from somewhere. They should be removed at some 
//       point.
*/


include("misc/table2.inc");

switch ($email_choice)
{
	case "Send":
		global $auth;

		$from = ($from = getCiEmailUsingID(getContactID($auth->department, $auth->login))) ? $from : BRAND_NAME_SYSTEM_EMAIL;
        $reply_id = $reply_to;
		$reply_to = ($reply_to = getCiEmailUsingID($reply_to)) ? $reply_to : "";
		
		if (send_email($job_id, $to, $cc, $bcc, $subject, $body, $from, $reply_to, $reply_id))
		{ 
			notify("The e-mail has been successfully sent to all of the matches returned from your search.");
		}
		break;
	
	default:
		display_email_form($contact_id_string);
		break;
}




function display_email_form($contact_id_string)
{
	global $PHP_SELF;
	global $auth;
	global $departments_in_group;
	global $searchContact;
	
	global $to;
	
	                
    // Find the users contact_id.

    $users_contact_id = getContactID($auth->department, $auth->login);

    // Use this contact id to find the users email.

    $users_email = getCiEmailUsingID($users_contact_id);

	// Create BCC field from contact_id_string

	$contact_id_string = unpackObject($contact_id_string);

	$sql = ("
		SELECT DISTINCT email, contact_id
		FROM contact
		WHERE contact_id IN (" . $contact_id_string . ")
        ORDER BY last_name, first_name
		");
	$result = $GLOBALS['dbh']->Execute($sql);
	while ($row = $result->FetchRow())
	{
		if (trim($row["email"]))
		{
			$row_email["email"] = preg_replace("/\//","", $row_email["email"]);
			$row_email["email"] = preg_replace("/\"/","", $row_email["email"]);
			$bcc .= $row["email"] . ", ";
		}
		else
		{
			$contacts_without_email[] = $row["contact_id"];
		}
	}

	if ($bcc != NULL)
	{
		$bcc = substr($bcc, 0, -2);
		$to = $users_email;
	}

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

	echo("<h4>Send Matched Contacts an E-mail:</h4>");

	if ($contacts_without_email)
	{
		$msg .= ("Please note: The following contacts do not have e-mail addresses, and will not be e-mailed.");
		$msg .= ("<br /><br /><b>");
		foreach ($contacts_without_email AS $cwe)
		{
			$contact_name = getContactName($cwe);
			$name = $contact_name["first_name"] . " " . $contact_name["last_name"];
			$msg .= ($name . ", ");
		}
		$msg = substr($msg, 0, -2);
		error($msg);
	}
	
	?>

	<form action='<?php echo($PHP_SELF); ?>' method='post' ENCTYPE='multipart/form-data'>
		<input type='hidden' name='select' value='view_contact' />
		<input type='hidden' name='level1' value='contact' />
		<input type='hidden' name='searchContact' value='<?php echo(packObject($searchContact)); ?>' />
		<input type='hidden' name='contact_id_list' value='<?php echo($contact_id_list); ?>' />
		<input type='hidden' name='continue' value='send_email_to_matches' />

		<table class='row1' cellspacing='0' cellpadding='4' border='0'>
			<tr><td>To:</td>
			<td colspan='2'><input type="text" name="to" size='70' value="<?php echo($to); ?>" /></td></tr>
			<tr><td>Cc:</td>
			<td colspan='2'><input type="text" size='70' name="cc" /></td></tr>
			<tr><td>Bcc:</td>
			<td colspan='2'>
			<textarea name='bcc' cols='70' rows='4'><?php echo $bcc?></textarea>
			</td></tr>
			<tr><td>From:</td>
			<td colspan='2'><?php echo(($users_email) ? $users_email : BRAND_NAME_SYSTEM_EMAIL); ?></td></tr>
			<tr><td valign='top'>Reply to:</td><td valign='top'>
			<select name='reply_to'>
			<?php
			
			while ($row = $result->FetchRow())
			{
				echo("<option value='" . $row["contact_id"] . "'" . (($_SESSION['SESSION_netlog'] == SCRIPT_LOGIN && $auth->login == $row["netlink_id"]) || ($_SESSION['SESSION_netlog'] == NIS_LOGIN && $auth->login == $row["login_id"]) ? " selected='selected'" : "") . ">" . $row["name"] . "</option>\n");
			}
			$result->Close();
						
			?>
			</select><br /> Contact information for this address will be automatically appended to the e-mail.
			</td>
			<td>&nbsp;</td>
			</tr>
			<tr><td>Subject:</td><td colspan='2'><input type="text" name="subject" size='55' /></td></tr>
			<tr><td colspan="3"><textarea cols="70" rows="20" name="body"></textarea></td></tr>
			<tr><td colspan="3" align="center"><hr />

			<input type="submit" name="email_choice" value="Send" />
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
