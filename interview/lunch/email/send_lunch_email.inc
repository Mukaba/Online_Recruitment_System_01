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
 | Filename: send_lunch_email.inc                                               |
 +------------------------------------------------------------------------------+
 | Description: This file is called when the user assigns a lunch to someone    |
 | who is not themselves.                                                       |
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
			include ("interview/lunch/all.inc");
		}
		break;
	
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
	global $time_id;
	global $cid;
	
	
	                
        /*
         Find the users contact_id.
        */      
                
        $users_contact_id = getContactID($auth->department, $auth->login);
                
        /*      
         Use this contact id to find the users email.
        */      
        
        $users_email = getCiEmailUsingID($users_contact_id);

	$sql = ("
		SELECT DISTINCT a.login_id, a.netlink_id, a.contact_id, CONCAT(b.first_name, ' ', b.last_name) AS name, b.email
		FROM contact_internal AS a, contact AS b
        WHERE a.department_id IN ('" . implode(department_groups($auth->department), "', '"). "')
		AND (a.interview_contact_list OR a.contact_id='".$users_contact_id."')
		AND a.contact_id=b.contact_id
		ORDER BY b.last_name, b.first_name
		");
	$result = $GLOBALS['dbh']->Execute($sql);
	
	$email_sql="SELECT ii.interviewer_name, it.int_date, it.int_time, ec.company_name 
        FROM interview_interviewer as ii, interview_time as it, employer_company as ec, job_info as ji 
        WHERE it.time_id=".$time_id." AND it.interviewer_id=ii.interviewer_id AND ji.job_id=it.job_id AND ji.employer_id=ec.employer_id";
	$email_results=$GLOBALS['dbh']->Execute($email_sql);
	$email_row=$email_results->FetchRow();

	$host_sql="SELECT ci.login_id,ci.netlink_id,c.email as cemail,d.email,d.default_login 
        FROM contact_internal as ci,contact as c , department as d 
        WHERE d.department_id=ci.department_id AND c.contact_id=ci.contact_id AND ci.contact_id=".$cid;
	$host_results=$GLOBALS['dbh']->Execute($host_sql);
	$host_row=$host_results->FetchRow();

	if ($host_row['cemail']==NULL){
		if ($host_row['default_login'] == SCRIPT_LOGIN){
			$email=$host_row['netlink_id']."@".$host_row['email'];
		}elseif($host_row['default_login'] == NIS_LOGIN){
			$email=$host_row['login_id']."@".$host_row['email'];
		}else{
			$email="";
		}
	}else{
		$email=$host_row['cemail'];
	}

	$anArray=explode("-",$email_row['int_date']);
	$thedate=date("l F d, Y",mktime(0,0,0,$anArray[1],$anArray[2],$anArray[0]));	

	?>
	</td><td align="left"><img src="misc/images<?php echo $_SESSION['SESSION_colorscheme']?>/topbar_right.jpg" alt='' /></td></tr></table>	
	<h3>Interviews - E-mail the New Lunch Host</h3>
	

	<form action="<?php echo $PHP_SELF."&amp;select=lunch_send_email" ?>" method="post" enctype="multipart/form-data">
		<input type="hidden" name="mail[0]" value='<?php echo($email_job["job_id"]);?>' />
		<input type='hidden' name='department_id' value='<?php echo $department_id?>' />
		<input type='hidden' name='case' value='<?php echo($case); ?>' />
		<table class='row1' border='0' cellspacing='0' cellpadding='4'>
			<tr><td>To:</td>
			<td colspan='2'><input type="text" name="to" size='70' value="<?php echo($email); ?>" /></td></tr>
			<tr><td>Cc:</td>
			<td colspan='2'><input type="text" size='70' name="cc" /></td></tr>
			<tr><td>Bcc:</td>
			<td colspan='2'>
			<textarea name='bcc' cols='70' rows='4'></textarea>
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
			<tr><td>Subject:</td><td colspan='2'><input type="text" name="subject" size='55' value='You have been assigned to a lunch with an employer.' /></td></tr>
			<tr><td colspan="3"><textarea cols="70" rows="20" name="body">You have been assigned to host the lunch for <?php echo $email_row['interviewer_name']?> (<?php echo $email_row['company_name']?>) on <?php echo $thedate." at ".substr($email_row['int_time'],0,strlen($email_row['int_time'])-3)?>.</textarea></td></tr>
			<tr><td colspan="3" align="center"><hr />
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
