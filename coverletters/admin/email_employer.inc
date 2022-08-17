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
 | Filename: email_employer.inc                                                 |
 +------------------------------------------------------------------------------+
 | Description: Brings up an email form used to email the employer the          |
 | information about where they can view the applications for their job.        |
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

	/*
         Find the users contact_id.
        */

        $users_contact_id = getContactID($auth->department, $auth->login);

        /*
         Use this contact id to find the users e-mail.
        */

        $user_email = getCiEmailUsingID($users_contact_id);
		
	$result = $GLOBALS['dbh']->Execute("
		SELECT DISTINCT a.login_id, a.netlink_id, a.contact_id, CONCAT(b.first_name, ' ', b.last_name) AS name, b.email
		FROM contact_internal AS a, contact AS b
		WHERE (a.department_id='" . addslashes($auth->department) . "' OR a.login_id='" . addslashes($auth->login) . "') 
		AND a.interview_contact_list 
		AND a.contact_id=b.contact_id
		ORDER BY b.last_name, b.first_name
		");

	if (!$result) 
	{ 
		return false; 
	}
	
	?>
	<h3>Job Applications Employer E-mail</h3>
	<form action="<?php echo($PHP_SELF); ?>" method="post" enctype="multipart/form-data">
		<input type="hidden" name="select" value="coverletter" />
		<input type="hidden" name="page" value="email" />
		<input type="hidden" name="job_id" value="<?php echo($job_id); ?>" />
		<table>
			<tr><td>To:</td><td colspan='2'><input type="text" name="to" /></td></tr>
			<tr><td>Cc:</td><td colspan='2'><input type="text" name="cc" /></td></tr>
			<tr><td>Bcc:</td><td colspan='2'><input type="text" name="bcc" /></td></tr>
			<tr><td>From:</td><td colspan='2'><?php echo(($user_email) ? $user_email : BRAND_NAME_SYSTEM_EMAIL); ?></td></tr>
			<tr><td valign='top'>Reply to:</td><td valign='top'>
				<select name="reply_to">
				<?php
				
				while ($row = $result->FetchRow())
				{
					echo(" <option value='" . $row["contact_id"] . "'" . (($_SESSION['SESSION_netlog'] == SCRIPT_LOGIN && $auth->login == $row["netlink_id"]) || ($_SESSION['SESSION_netlog'] == NIS_LOGIN && $auth->login == $row["login_id"]) ? " selected='selected'" : "") . ">" . $row["name"] . "</option>\n");
				}
				$result->Close();
							
				?>
				</select>
			</td>
			<td valign='top'> Contact information for this address will be automatically appended to the e-mail.</td>
			</tr>
			<tr><td>Subject:</td><td colspan='2'><input type="text" name="subject" value="Job Applications" /></td></tr>
			<tr><td colspan="3"><textarea cols="60" rows="20" name="body" wrap="soft"><?php echo(create_body2($job_id)); ?></textarea></td></tr>
			<tr><td colspan="3" align="center">
				<input type="submit" name="email_employer" value="Send" />
				<input type="submit" name="email_employer" value="Cancel" />
			</td></tr>
		</table>
	</form>
	<?php
}


function send_email($job_id, $to, $cc, $bcc, $subject, $body, $from, $reply_to, $reply_id)
{

	global $PHP_SELF;
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
			?>
			<div align="center">
			<p>E-mail was sent.</p>
			<form action="<?php echo($PHP_SELF); ?>">
				<input type="hidden" name="select" value="coverletter" />
				<input type="hidden" name="page" value="activate" />
				<input type="hidden" name="job_id" value="<?php echo($job_id); ?>" />
				<input type="submit" value="OK" />
			</form>
			</div>
		<?php
	}
	unset ($mail);
}

function create_body2($job_id)
{
	$result = $GLOBALS['dbh']->Execute("
		SELECT	j.job_code 
		FROM	job_info AS j
		WHERE	j.job_id='" . addslashes($job_id) . "'"); 
	
	if (!$result) { return false; }
	if ($row = $result->FetchRow())
	{
		$job_code = $row["job_code"];
	}
	$result->Close();
	if (!$row) { return false; }
	
	$output = "Job Applications for Job Code: $job_code\n\n";
	$output .= "Please go to ".ORGANIZATION_DB_URL." and log in using your user name and password.  Once you are logged on click on the 'View Applicants' menu item.\n\n";

	$result = $GLOBALS['dbh']->Execute("
			SELECT CONCAT(s.first_name, ' ', s.last_name) as name
				FROM student as s, coverletter as c
				WHERE s.student_number = c.student_number AND
					c.job_id = '" . addslashes($job_id) . "' AND
					c.status_id = '" . ACTIVE . "'");
	if (!$result) { return false; }
	
	$output .= "The following students have provided applications:\n";
	while( $row =  $result->FetchRow()){
		$output .= $row["name"] . "\n";
	}
	
	return $output;
}











class interviewer_date
{
	var $int_date;
	var $interviewer_name;
	var $time_slots;
	
	function interviewer_date($int_date, $interviewer_name, $time_slots)
	{
		$this->int_date = $int_date;
		$this->interviewer_name = $interviewer_name;
		$this->time_slots = $time_slots;
	}
}

class time_slots
{
	var $start_time;
	var $end_time;
	var $type;
	var $student_name;
	var $phone_number;
	
	function time_slots($start_time, $end_time, $type, $student_name, $phone_number)
	{
		$this->start_time = $start_time;
		$this->end_time = $end_time;
		$this->type = $type;
		$this->student_name = $student_name;
		$this->phone_number = $phone_number;
	}
}




function info_block_2($interviewer_date)
{
	$num_interviewer_dates = count($interviewer_date);
	for ($i = 0; $i < $num_interviewer_dates; $i++)
	{
		$output .= "Date: ". formatLongDate($interviewer_date[$i]->int_date) . "\n";
		$output .= "Interviewer: " . $interviewer_date[$i]->interviewer_name . "\n";

		$table = new table(new th(new td("Time", "center"), new td("Student", "center")));
		
		$num_time_slots = count($interviewer_date[$i]->time_slots);
		for ($j = 0; $j < $num_time_slots; $j++)
		{
			$time = txtFormatTime($interviewer_date[$i]->time_slots[$j]->start_time);
			$type = $interviewer_date[$i]->time_slots[$j]->type;
			$student_name = $interviewer_date[$i]->time_slots[$j]->student_name;

			if ($type == "interview") { $td = new td($student_name, "left"); }
			else { $td = new td("-- " . strtoupper($type) . " --", "center"); }
			if ($phone_number = $interviewer_date[$i]->time_slots[$j]->phone_number)
			{
				$table->rows[] = new tr(new td($time, "center"), $td, new td($phone_number, "left"));
			}
			else
			{
				$table->rows[] = new tr(new td($time, "center"), $td);
			}
		}
		
		$output .= $table->display() . "\n\n";
		unset($table);
	}
	return $output;
}





?>
