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
 | Filename: confirm_shortlist.inc                                              |
 +------------------------------------------------------------------------------+
 | Description: Students have been chosen for a shortlist.  Check to confirm,   |
 | and then see if a notice to the office should be sent.                       |
 +------------------------------------------------------------------------------+

*/

echo ("<h3>Confirm Shortlist</h3>");

// check to see if the notification has been sent
$sql = "SELECT interview_notify_sent
	FROM job_info
	WHERE job_id = '$job_id'";
$result = $GLOBALS['dbh']->Execute ($sql);

if ($row = $result->FetchRow()) {
	if ($row["interview_notify_sent"])
		$notify_sent = true;
	else
		$notify_sent = false;
}
else
	$notify_sent = false;

// check if there are already students shorlisted
$sql = "SELECT student_number
	FROM students_shortlisted
	WHERE job_id = '$job_id'";
$result = $GLOBALS['dbh']->Execute ($sql);

if ($result->RecordCount())
	$already_students = true;
else
	$already_students = false;



// first get the student names of those checked
$sql = "SELECT DISTINCT a.student_number, CONCAT(s.first_name, ' ', s.last_name) AS name
	FROM applications AS a, student AS s";

switch ($shortlist_who) {
	case 'flagged':
		$sql .= ", employer_apps_flagged AS eaf
			WHERE eaf.contact_id = '$contact_id'
			AND a.application_id = eaf.application_id
			AND job_id = '$job_id'";
		break;

	case 'all':
		$sql .= " WHERE a.application_id IN ('". implode ( unpackObject(($all_apps)), "','") ."')";
		break;

	case 'selected':
	default:
		$sql .= " WHERE a.application_id IN ('". (sizeof ($selected_app_id) > 0 ? implode ($selected_app_id, "','") : '') ."')";
}

$sql .= " AND a.student_number = s.student_number";

$result = $GLOBALS['dbh']->Execute ($sql);
// array used below
$shortlist_numbers = array ();

// setup up a nice little confirmation box
echo ("<form method='post' action='$PHP_SELF'>");
echo ("<input type='hidden' name='select' value='applications' />");
echo ("<input type='hidden' name='shortlist' value='1' />");
echo ("<input type='hidden' name='job_id' value='$job_id' />");

echo ("<table border='0' class='row1' cellpadding='8' cellspacing='5'>");
	echo ("<tr>");
		echo ("<td align='center' valign='top'>");
		if ($already_students)
			echo ("You have selected the following student(s) to add to your shortlist:<br /><br />");
		else
			echo ("You have selected the following student(s) to shortlist for an interview:<br /><br />");
		// print 'em out, and put numbers into an array
		$row = $result->FetchRow();
		echo ($row["name"]);
		$shortlist_numbers[] = $row["student_number"];
		while ($row = $result->FetchRow()) {
			echo (",<br />". $row["name"]);
			$shortlist_numbers[] = $row["student_number"];
		}
		echo ("<br /><br />");

		if ($notify_sent) {
			echo ("You have already notified the Co-op office of the student(s) you've shortlisted and your 
				interview preferences.<br />");
			echo ("Do you want to change or add to your interview preferences?<br />");
		}
		
		else {
			echo ("You have not yet notified the Co-op office of your shortlist selections. A notification is an 
				e-mail sent through ".BRAND_NAME." in which you can specify shortlisted students and interview 
				preferences (i.e. date, time, location, format, etc.).<br /><br />");
			echo ("Would you like to send a notification now?<br />");
		}

		echo ("<input type='radio' name='send_notify' value='yes'". (!$notify_sent ? " checked='checked'" : "") ." />Yes &nbsp;");
		echo ("<input type='radio' name='send_notify' value='no'". ($notify_sent ? " checked='checked'" : "") ." />No <br /><br />");
		
		echo ("<br />");
		echo ("<input type='hidden' name='shortlist_numbers' value='". packObject(($shortlist_numbers)) ."' />");
		echo ("<input type='submit' name='shortlist_confirmed' value='Confirm & Continue' /> &nbsp; &nbsp; ");
		echo ("<input type='submit' name='cancel_confirm' value=' Cancel ' />");

		echo ("</td>");
	echo ("</tr>");
echo ("</table>");
echo ("</form>");

?>
