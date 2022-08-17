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
 | Filename: mail.inc                                                           |
 +------------------------------------------------------------------------------+
 | Description: This file is used to send out e-mail to matches of students     |
 | from the student search screen.                                              |
 +------------------------------------------------------------------------------+

*/

switch ($email_lunch)
{
	case "Send":
		global $auth;
        global $record_id;

		$from = ($from = getCiEmailUsingID(getContactID($auth->department, $auth->login))) ? $from : BRAND_NAME_SYSTEM_EMAIL;
        $reply_id = $reply_to;
		$reply_to = ($reply_to = getCiEmailUsingID($reply_to)) ? $reply_to : "";
		
		if (send_email($job_id, $to, $cc, $bcc, $subject, $body, $from, $reply_to, $reply_id))
		{ 
			$message_sent="yes";
			if ($after==NULL){
                $select="view_student";
				$btnSearch="Search";
                $continue="Advanced Search";
				//$searchmode="advanced";
                include_once('student_info/view/view_student_header.inc');
				include('student_info/view/advanced_search.inc');
			}else{
                $continue = 'Chosen Student';

                if ($_POST['after'] == 'student_info/view/view_student.inc') {
                    $clean_after = $_POST['after'];
                    include ($clean_after);
                }
			}

		}
		break;
	
	default:
		include ('student_info/functions.inc');
		display_email_form($bcc);
		break;
}

function display_email_form($bcc)
{
	global $PHP_SELF;
	global $auth;
    global $record_id;
	global $interview;
	global $return_to;
	global $case;
	global $email_students;
    global $students_without_email;
	global $departments_in_group;
	global $time_id;
	global $cid;
	
	global $searchStudents;
	global $after;
	global $student_num;
	
	global $to;
	
    // Find the users contact_id.

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
//		$to=$email;
		$to=$users_email;
	}

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

	?>
	</td><td align="left"><img src="<?php echo $GLOBALS['colorscheme']['topbar_right']; ?>" alt='' /></td></tr></table>	
	<h3>Student Information - E-mail Students</h3>
    <?php
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
		<input type='hidden' name='case' value='<?php echo($case); ?>' />
		<table class='row1' cellspacing='0' cellpadding='4' border='0'>
			<tr><td>To:</td>
			<td colspan='2'><input type="text" name="to" size='70' value="<?php echo($to); ?>" /></td></tr>
			<tr><td>Cc:</td>
			<td colspan='2'><input type="text" size='70' name="cc" /></td></tr>
			<tr><td>Bcc:</td>
			<td colspan='2'>
			<textarea name='bcc' cols='70' rows='4' ><?php echo $bcc?></textarea>
			</td></tr>
			<tr><td>From:</td>
			<td colspan='2'><?php echo(($users_email) ? $users_email : BRAND_NAME_SYSTEM_EMAIL); ?></td></tr>
			<tr><td valign='top'>Reply to:</td><td valign='top'>
			<select name="reply_to">
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

				<input type='hidden' name='searchStudents' value='<?php echo $searchStudents?>' />        
				<input type='hidden' name='btnSearch' value='Search' />
				<input type='hidden' name='searchmode' value='advanced' />
	        		<input type='hidden' name='select' value='email_students' />
				<input type="hidden" name="message_sent" value="yes" />
				<input type="hidden" name="record_id" value="<?php echo $record_id?>" />
				<input type="hidden" name="after" value="<?php echo $after?>" />
				
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
