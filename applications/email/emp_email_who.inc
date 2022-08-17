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
 | Filename: emp_email_who.inc                                                  |
 +------------------------------------------------------------------------------+
 | Description: This file contains one function, which is used to determine     |
 | who is being e-mailed.                                                       |
 +------------------------------------------------------------------------------+

*/

function emp_email_who ($email_who = 'all', $selected_app_id = NULL) {

	global $job_id;
	global $contact_id;
	global $employer_contact_id_login;
	global $userlevel;

	if (($email_who == 'all') || ($email_who == 'selected')){
		
		$sql = "SELECT DISTINCT application_id, student_number
			FROM applications
			WHERE application_id IN ('". implode (unpackObject(($selected_app_id)), "','") ."')";
		$result = $GLOBALS['dbh']->Execute ($sql);

		$get_emails_ready = true;
	}


	elseif ($email_who == 'shortlisted') {

		$sql = "SELECT DISTINCT a.application_id, a.student_number
			FROM applications AS a, students_shortlisted AS ss
			WHERE a.student_number = ss.student_number
			AND a.job_id = '$job_id'
			AND ss.job_id = '$job_id'";
		$result = $GLOBALS['dbh']->Execute ($sql);

		$get_emails_ready = true;
	}


	elseif ($email_who == 'flagged') {

		$sql = "SELECT DISTINCT a.application_id, a.student_number
			FROM applications AS a, employer_apps_flagged AS eaf
			WHERE a.application_id = eaf.application_id
			AND eaf.contact_id = '". ($userlevel == ADMINEMPLOYER ? $employer_contact_id_login : $contact_id) ."'
			AND a.job_id = '$job_id'";
		$result = $GLOBALS['dbh']->Execute ($sql);

		$get_emails_ready = true;
	}


	if ($get_emails_ready) {

		// now play with it to get it ready
		// make sure there's e-mail for everybody
		while ($row = $result->FetchRow()) 
			$email_list[] = $row["student_number"];

		for ($i = 0; $i < sizeof($email_list); $i ++) {

			$check_email[] = array(
				"student_number" => $email_list[$i],
				"email" => getStudentEmail($email_list[$i]),
				"name" => getStudentName($email_list[$i])
				);

			if (!$check_email[$i]["email"]) 
				$noemail_addy .= ", ". $check_email[$i]["name"]; 
			else
				$email_addy[] = $check_email[$i]["email"];
		}

		if (is_array($email_addy)) {
			if (sizeof ($noemail_addy) > 0) {
				error ("There were errors sending the e-mail to the following student(s):<br />".
					$noemail_addy ."<br />Please inform the Co-op office of this error, thank you.");
			}
			return $email_addy;
		}
		else {
			error ("No students have been selected to e-mail.");
			include ('applications/employer/applicant_list.inc');
		}
	}
}

?>
