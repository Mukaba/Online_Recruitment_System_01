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
 | Filename: mail_company.inc                                                   |
 +------------------------------------------------------------------------------+
 | Description: This file is used to send out e-mail to matched companies.      |
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
			$message_sent="yes";

		}
		break;
	
	default:
		display_email_form($bcc);
		break;
}

function display_email_form($bcc)
{
	global $PHP_SELF;
	global $auth;
    global $searchContact;
    global $employer_id;
    global $employer_id_list;
	global $departments_in_group;
	
	global $after;
	global $to;
    global $no_buttons;


    $users_contact_id = getContactID($auth->department, $auth->login);

    // Use this contact id to find the users email.
    $users_email = getCiEmailUsingID($users_contact_id);

	$sql = ("
		SELECT DISTINCT a.login_id, a.netlink_id, a.contact_id, CONCAT(b.first_name, ' ', b.last_name) AS name, b.email
		FROM contact_internal AS a, contact AS b
		WHERE a.department_id='" . addslashes($auth->department) . "' 
		AND a.contact_id=b.contact_id AND 
		(a.general_email='1' OR (a.login_id='".addslashes($auth->login)."' OR a.netlink_id='".addslashes($auth->login)."'))
		ORDER BY b.last_name, b.first_name
		");
	$result = $GLOBALS['dbh']->Execute($sql);	
	if ($bcc<>NULL){
		$to=$users_email;
	}


	?>


	<form action="<?php echo $PHP_SELF."&amp;select=email_company" ?>" method="post" enctype="multipart/form-data">
		<input type='hidden' name='department_id' value='<?php echo $department_id?>' />
		<input type='hidden' name='case' value='<?php echo($case); ?>' />
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
			<select name="reply_to">
            <?php
            $flag = 0;
            while ($row = $result->FetchRow())
            {
                echo("<option value='" . $row["contact_id"] . "'" . ($users_contact_id == $row["contact_id"] ? " selected='selected'" : "") . ">" . $row["name"] . "</option>");
                if($users_contact_id == $row["contact_id"]){
                    $flag = 1;
                }
            }
            if ($flag == 0){
                echo("<option value='" .$users_contact_id . "' selected ='selected '>".getContactName($users_contact_id)."</option>");
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

                <input type='hidden' name='select' value='send_company_mail' />
                <input type="hidden" name="message_sent" value="yes" />
				<input type="hidden" name="after" value="<?php echo $after?>" />
                <input type="hidden" name="employer_id" value="<?php echo $employer_id?>" />
                <input type="hidden" name="employer_id_list" value="<?php echo $employer_id_list?>" />
                <input type="hidden" name="searchContact" value="<?php echo $searchContact?>" />
                <input type="hidden" name="no_buttons" value="<?php echo $no_buttons?>" />
                				
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
